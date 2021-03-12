/* Copyright (C) 2003-2008 Jean-Marc Valin

   File: mdf.c
   Echo canceller based on the MDF algorithm (see below)

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:

   1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
   STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
*/

/*
   The echo canceller is based on the MDF algorithm described in:

   J. S. Soo, K. K. Pang Multidelay block frequency adaptive filter, 
   IEEE Trans. Acoust. Speech Signal Process., Vol. ASSP-38, No. 2, 
   February 1990.
   
   We use the Alternatively Updated MDF (AUMDF) variant. Robustness to 
   double-talk is achieved using a variable learning rate as described in:
   
   Valin, J.-M., On Adjusting the Learning Rate in Frequency Domain Echo 
   Cancellation With Double-Talk. IEEE Transactions on Audio,
   Speech and Language Processing, Vol. 15, No. 3, pp. 1030-1034, 2007.
   http://people.xiph.org/~jm/papers/valin_taslp2006.pdf
   
   There is no explicit double-talk detection, but a continuous variation
   in the learning rate based on residual echo, double-talk and background
   noise.
   
   About the fixed-point version:
   All the signals are represented with 16-bit words. The filter weights 
   are represented with 32-bit words, but only the top 16 bits are used
   in most cases. The lower 16 bits are completely unreliable (due to the
   fact that the update is done only on the top bits), but help in the
   adaptation -- probably by removing a "threshold effect" due to
   quantization (rounding going to zero) when the gradient is small.
   
   Another kludge that seems to work good: when performing the weight
   update, we only move half the way toward the "goal" this seems to
   reduce the effect of quantization noise in the update phase. This
   can be seen as applying a gradient descent on a "soft constraint"
   instead of having a hard constraint.
   
*/
#define HAVE_CONFIG_H
#ifdef HAVE_CONFIG_H
#include "webrtc/modules/audio_processing/aec/speexhead/config.h"
#endif

#include "webrtc/modules/audio_processing/aec/speexhead/arch.h"
#include "webrtc/modules/audio_processing/aec/speexhead/speex_echo.h"
#include "webrtc/modules/audio_processing/aec/speexhead/fftwrap.h"
#include "webrtc/modules/audio_processing/aec/speexhead/pseudofloat.h"
#include "webrtc/modules/audio_processing/aec/speexhead/math_approx.h"
#include "webrtc/modules/audio_processing/aec/speexhead/os_support.h"



#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef FIXED_POINT
#define WEIGHT_SHIFT 11
#define NORMALIZE_SCALEDOWN 5
#define NORMALIZE_SCALEUP 3
#else
#define WEIGHT_SHIFT 0
#endif

#ifdef FIXED_POINT
#define WORD2INT(x) ((x) < -32767 ? -32768 : ((x) > 32766 ? 32767 : (x)))  
#else
#define WORD2INT(x) ((x) < -32767.5f ? -32768 : ((x) > 32766.5f ? 32767 : floor(.5+(x))))  
#endif

/* If enabled, the AEC will use a foreground filter and a background filter to be more robust to double-talk
   and difficult signals in general. The cost is an extra FFT and a matrix-vector multiply */
#define TWO_PATH

#ifdef FIXED_POINT
static const spx_float_t MIN_LEAK = {20972, -22};

/* Constants for the two-path filter */
static const spx_float_t VAR1_SMOOTH = {23593, -16};
static const spx_float_t VAR2_SMOOTH = {23675, -15};
static const spx_float_t VAR1_UPDATE = {16384, -15};
static const spx_float_t VAR2_UPDATE = {16384, -16};
static const spx_float_t VAR_BACKTRACK = {16384, -12};
#define TOP16(x) ((x)>>16)

#else

static const spx_float_t MIN_LEAK = .1f;// .005f;

/* Constants for the two-path filter */
static const spx_float_t VAR1_SMOOTH = .36f;
static const spx_float_t VAR2_SMOOTH = .7225f;
static const spx_float_t VAR1_UPDATE = .5f;
static const spx_float_t VAR2_UPDATE = .25f;
static const spx_float_t VAR_BACKTRACK = 4.f;
#define TOP16(x) (x)
#endif


#define PLAYBACK_DELAY 2

void speex_echo_get_residual(SpeexEchoState *st, spx_word32_t *Yout, int len);


static inline void filter_dc_notch16(const spx_word16_t *in, spx_word16_t *out, int len)
{
   int i;
   for (i=0;i<len;i++)
   {
	  out[i] = in[i];
   }
}

/* This inner product is slightly different from the codec version because of fixed-point */
static inline spx_word32_t mdf_inner_prod(const spx_word16_t *x, const spx_word16_t *y, int len)
{
   spx_word32_t sum=0;
   len >>= 1;
   while(len--)
   {
      spx_word32_t part=0;
      part = MAC16_16(part,*x++,*y++);
      part = MAC16_16(part,*x++,*y++);
      /* HINT: If you had a 40-bit accumulator, you could shift only at the end */
      sum = ADD32(sum,SHR32(part,6));
   }
   return sum;
}

/** Compute power spectrum of a half-complex (packed) vector */
static inline void power_spectrum(const spx_word16_t *X, spx_word32_t *ps, int N)
{
   int i, j;
   ps[0]=MULT16_16(X[0],X[0]);
   for (i=1,j=1;i<N-1;i+=2,j++)
   {
      ps[j] =  MULT16_16(X[i],X[i]) + MULT16_16(X[i+1],X[i+1]);
   }
   ps[j]=MULT16_16(X[i],X[i]);
}

/** Compute power spectrum of a half-complex (packed) vector and accumulate */
static inline void power_spectrum_accum(const spx_word16_t *X, spx_word32_t *ps, int N)
{
   int i, j;
   ps[0]+=MULT16_16(X[0],X[0]);
   for (i=1,j=1;i<N-1;i+=2,j++)
   {
      ps[j] +=  MULT16_16(X[i],X[i]) + MULT16_16(X[i+1],X[i+1]);
   }
   ps[j]+=MULT16_16(X[i],X[i]);
}

/** Compute cross-power spectrum of a half-complex (packed) vectors and add to acc */
#ifdef FIXED_POINT
static inline void spectral_mul_accum(const spx_word16_t *X, const spx_word32_t *Y, spx_word16_t *acc, int N, int M)
{
   int i,j;
   spx_word32_t tmp1=0,tmp2=0;
   for (j=0;j<M;j++)
   {
      tmp1 = MAC16_16(tmp1, X[j*N],TOP16(Y[j*N]));
   }
   acc[0] = PSHR32(tmp1,WEIGHT_SHIFT);
   for (i=1;i<N-1;i+=2)
   {
      tmp1 = tmp2 = 0;
      for (j=0;j<M;j++)
      {
         tmp1 = SUB32(MAC16_16(tmp1, X[j*N+i],TOP16(Y[j*N+i])), MULT16_16(X[j*N+i+1],TOP16(Y[j*N+i+1])));
         tmp2 = MAC16_16(MAC16_16(tmp2, X[j*N+i+1],TOP16(Y[j*N+i])), X[j*N+i], TOP16(Y[j*N+i+1]));
      }
      acc[i] = PSHR32(tmp1,WEIGHT_SHIFT);
      acc[i+1] = PSHR32(tmp2,WEIGHT_SHIFT);
   }
   tmp1 = tmp2 = 0;
   for (j=0;j<M;j++)
   {
      tmp1 = MAC16_16(tmp1, X[(j+1)*N-1],TOP16(Y[(j+1)*N-1]));
   }
   acc[N-1] = PSHR32(tmp1,WEIGHT_SHIFT);
}
static inline void spectral_mul_accum16(const spx_word16_t *X, const spx_word16_t *Y, spx_word16_t *acc, int N, int M)
{
   int i,j;
   spx_word32_t tmp1=0,tmp2=0;
   for (j=0;j<M;j++)
   {
      tmp1 = MAC16_16(tmp1, X[j*N],Y[j*N]);
   }
   acc[0] = PSHR32(tmp1,WEIGHT_SHIFT);
   for (i=1;i<N-1;i+=2)
   {
      tmp1 = tmp2 = 0;
      for (j=0;j<M;j++)
      {
         tmp1 = SUB32(MAC16_16(tmp1, X[j*N+i],Y[j*N+i]), MULT16_16(X[j*N+i+1],Y[j*N+i+1]));
         tmp2 = MAC16_16(MAC16_16(tmp2, X[j*N+i+1],Y[j*N+i]), X[j*N+i], Y[j*N+i+1]);
      }
      acc[i] = PSHR32(tmp1,WEIGHT_SHIFT);
      acc[i+1] = PSHR32(tmp2,WEIGHT_SHIFT);
   }
   tmp1 = tmp2 = 0;
   for (j=0;j<M;j++)
   {
      tmp1 = MAC16_16(tmp1, X[(j+1)*N-1],Y[(j+1)*N-1]);
   }
   acc[N-1] = PSHR32(tmp1,WEIGHT_SHIFT);
}

#else
static inline void spectral_mul_accum(const spx_word16_t *X, const spx_word32_t *Y, spx_word16_t *acc, int N, int M)
{
   int i,j;
   for (i=0;i<N;i++)
      acc[i] = 0;
   for (j=0;j<M;j++)
   {
      acc[0] += X[0]*Y[0];
      for (i=1;i<N-1;i+=2)
      {
         acc[i] += (X[i]*Y[i] - X[i+1]*Y[i+1]);
         acc[i+1] += (X[i+1]*Y[i] + X[i]*Y[i+1]);
      }
      acc[i] += X[i]*Y[i];
      X += N;
      Y += N;
   }
}
#define spectral_mul_accum16 spectral_mul_accum
#endif

/** Compute weighted cross-power spectrum of a half-complex (packed) vector with conjugate */
static inline void weighted_spectral_mul_conj(const spx_float_t *w, const spx_float_t p, const spx_word16_t *X, const spx_word16_t *Y, spx_word32_t *prod, int N)
{
   int i, j;
   spx_float_t W;
   W = FLOAT_AMULT(p, w[0]);
   prod[0] = FLOAT_MUL32(W,MULT16_16(X[0],Y[0]));
   for (i=1,j=1;i<N-1;i+=2,j++)
   {
      W = FLOAT_AMULT(p, w[j]);
      prod[i] = FLOAT_MUL32(W,MAC16_16(MULT16_16(X[i],Y[i]), X[i+1],Y[i+1]));
      prod[i+1] = FLOAT_MUL32(W,MAC16_16(MULT16_16(-X[i+1],Y[i]), X[i],Y[i+1]));
   }
   W = FLOAT_AMULT(p, w[j]);
   prod[i] = FLOAT_MUL32(W,MULT16_16(X[i],Y[i]));
}

static inline void mdf_adjust_prop(const spx_word32_t *W, int N, int M, spx_word16_t *prop)
{
   int i, j, p;
   spx_word16_t max_sum = 1;
   spx_word32_t prop_sum = 1;
   for (i=0;i<M;i++)
   {
      spx_word32_t tmp = 1;
		for (j=0;j<N;j++)
		tmp += MULT16_16(EXTRACT16(SHR32(W[i*N+j],18)), EXTRACT16(SHR32(W[i*N+j],18)));

      prop[i] = spx_sqrt(tmp);
      if (prop[i] > max_sum)
         max_sum = prop[i];
   }
   for (i=0;i<M;i++)
   {
      prop[i] += MULT16_16_Q15(QCONST16(.1f,15),max_sum);
      prop_sum += EXTEND32(prop[i]);
   }
   for (i=0;i<M;i++)
   {
      prop[i] = DIV32(MULT16_16(QCONST16(.99f,15), prop[i]),prop_sum);
      /*printf ("%f ", prop[i]);*/
   }
   /*printf ("\n");*/
}

#ifdef DUMP_ECHO_CANCEL_DATA
#include <stdio.h>
static FILE *rFile=NULL, *pFile=NULL, *oFile=NULL;

static void dump_audio(const spx_int16_t *rec, const spx_int16_t *play, const spx_int16_t *out, int len)
{
   if (!(rFile && pFile && oFile))
   {
      speex_fatal("Dump files not open");
   }
   fwrite(rec, sizeof(spx_int16_t), len, rFile);
   fwrite(play, sizeof(spx_int16_t), len, pFile);
   fwrite(out, sizeof(spx_int16_t), len, oFile);
}
#endif

/** Creates a new echo canceller state */
EXPORT SpeexEchoState *speex_echo_state_init(int frame_size, int filter_length)
{
   return speex_echo_state_init_mc(frame_size, filter_length);
}

EXPORT SpeexEchoState *speex_echo_state_init_mc(int frame_size, int filter_length)
{
   int i,N,M;
   SpeexEchoState *st = (SpeexEchoState *)speex_alloc(sizeof(SpeexEchoState));
   
   st->frame_size = frame_size;
   st->window_size = 2*frame_size;
   N = st->window_size;
   M = st->M = (filter_length+st->frame_size-1)/frame_size;
   st->cancel_count=0;
   st->sum_adapt = 0;
   st->saturated = 0;
   st->screwed_up = 0;
   /* This is the default sampling rate */
   st->sampling_rate = 8000;
   st->spec_average = DIV32_16(SHL32(EXTEND32(st->frame_size), 15), st->sampling_rate);
   st->beta0 = (2.0f*st->frame_size)/st->sampling_rate;
   st->beta_max = (.5f*st->frame_size)/st->sampling_rate;
   st->leak_estimate = 0;

   st->fft_table = spx_fft_init(N);
   
   st->e = (spx_word16_t*)speex_alloc(N*sizeof(spx_word16_t));
   st->x = (spx_word16_t*)speex_alloc(N*sizeof(spx_word16_t));
   st->input = (spx_word16_t*)speex_alloc(st->frame_size*sizeof(spx_word16_t));
   st->y = (spx_word16_t*)speex_alloc(N*sizeof(spx_word16_t));
   st->last_y = (spx_word16_t*)speex_alloc(N*sizeof(spx_word16_t));
   st->Yf = (spx_word32_t*)speex_alloc((st->frame_size+1)*sizeof(spx_word32_t));
   st->Rf = (spx_word32_t*)speex_alloc((st->frame_size+1)*sizeof(spx_word32_t));
   st->Xf = (spx_word32_t*)speex_alloc((st->frame_size+1)*sizeof(spx_word32_t));
   st->Yh = (spx_word32_t*)speex_alloc((st->frame_size+1)*sizeof(spx_word32_t));
   st->Eh = (spx_word32_t*)speex_alloc((st->frame_size+1)*sizeof(spx_word32_t));

   st->X = (spx_word16_t*)speex_alloc(M*N*sizeof(spx_word16_t));
   st->Y = (spx_word16_t*)speex_alloc(N*sizeof(spx_word16_t));
   st->E = (spx_word16_t*)speex_alloc(N*sizeof(spx_word16_t));
   st->W = (spx_word32_t*)speex_alloc(M*N*sizeof(spx_word32_t));
   st->foreground = (spx_word16_t*)speex_alloc(M*N*sizeof(spx_word16_t));
   st->PHI = (spx_word32_t*)speex_alloc(N*sizeof(spx_word32_t));
   st->power = (spx_word32_t*)speex_alloc((frame_size+1)*sizeof(spx_word32_t));
   st->power_1 = (spx_float_t*)speex_alloc((frame_size+1)*sizeof(spx_float_t));
   st->window = (spx_word16_t*)speex_alloc(N*sizeof(spx_word16_t));
   st->prop = (spx_word16_t*)speex_alloc(M*sizeof(spx_word16_t));
   st->wtmp = (spx_word16_t*)speex_alloc(N*sizeof(spx_word16_t));
#ifdef FIXED_POINT
   st->wtmp2 = (spx_word16_t*)speex_alloc(N*sizeof(spx_word16_t));
   for (i=0;i<N>>1;i++)
   {
      st->window[i] = (16383-SHL16(spx_cos(DIV32_16(MULT16_16(25736,i<<1),N)),1));
      st->window[N-i-1] = st->window[i];
   }
#else
   for (i=0;i<N;i++)
      st->window[i] = .5-.5*cos(2*M_PI*i/N);
#endif
   for (i=0;i<=st->frame_size;i++)
      st->power_1[i] = FLOAT_ONE;
   for (i=0;i<N*M;i++)
      st->W[i] = 0;
   {
      spx_word32_t sum = 0;
      /* Ratio of ~10 between adaptation rate of first and last block */
      spx_word16_t decay = SHR32(spx_exp(NEG16(DIV32_16(QCONST16(2.4,11),M))),1);
      st->prop[0] = QCONST16(.7, 15);
      sum = EXTEND32(st->prop[0]);
      for (i=1;i<M;i++)
      {
         st->prop[i] = MULT16_16_Q15(st->prop[i-1], decay);
         sum = ADD32(sum, EXTEND32(st->prop[i]));
      }
      for (i=M-1;i>=0;i--)
      {
         st->prop[i] = DIV32(MULT16_16(QCONST16(.8f,15), st->prop[i]),sum);
      }
   }
   
   st->memX = (spx_word16_t*)speex_alloc(sizeof(spx_word16_t));
   st->memD = (spx_word16_t*)speex_alloc(sizeof(spx_word16_t));
   st->memE = (spx_word16_t*)speex_alloc(sizeof(spx_word16_t));
   st->preemph = 0.9f;//QCONST16(.9,15);
   if (st->sampling_rate<12000)
      st->notch_radius = QCONST16(.9, 15);
   else if (st->sampling_rate<24000)
      st->notch_radius = QCONST16(.982, 15);
   else
      st->notch_radius = QCONST16(.992, 15);

   st->notch_mem = (spx_mem_t*)speex_alloc(2*sizeof(spx_mem_t));
   st->adapted = 0;
   st->Pey = st->Pyy = FLOAT_ONE;
   
#ifdef TWO_PATH
   st->Davg1 = st->Davg2 = 0;
   st->Dvar1 = st->Dvar2 = FLOAT_ZERO;
#endif
   
   st->play_buf = (spx_int16_t*)speex_alloc((PLAYBACK_DELAY+1)*st->frame_size*sizeof(spx_int16_t));
   st->play_buf_pos = PLAYBACK_DELAY*st->frame_size;
   st->play_buf_started = 0;
   
   return st;
}

/** Resets echo canceller state */
EXPORT void speex_echo_state_reset(SpeexEchoState *st)
{
   int i, M, N, C, K;
   st->cancel_count=0;
   st->screwed_up = 0;
   N = st->window_size;
   M = st->M;
   C=st->C;
   K=st->K;
   for (i=0;i<N*M;i++)
      st->W[i] = 0;
#ifdef TWO_PATH
   for (i=0;i<N*M;i++)
      st->foreground[i] = 0;
#endif
   for (i=0;i<N*(M+1);i++)
      st->X[i] = 0;
   for (i=0;i<=st->frame_size;i++)
   {
      st->power[i] = 0;
      st->power_1[i] = FLOAT_ONE;
      st->Eh[i] = 0;
      st->Yh[i] = 0;
   }
   for (i=0;i<st->frame_size;i++)
   {
      st->last_y[i] = 0;
   }
   for (i=0;i<N*C;i++)
   {
      st->E[i] = 0;
   }
   for (i=0;i<N*K;i++)
   {
      st->x[i] = 0;
   }
   for (i=0;i<2*C;i++)
      st->notch_mem[i] = 0;
   for (i=0;i<C;i++)
      st->memD[i]=st->memE[i]=0;
   for (i=0;i<K;i++)
      st->memX[i]=0;

   st->saturated = 0;
   st->adapted = 0;
   st->sum_adapt = 0;
   st->Pey = st->Pyy = FLOAT_ONE;
#ifdef TWO_PATH
   st->Davg1 = st->Davg2 = 0;
   st->Dvar1 = st->Dvar2 = FLOAT_ZERO;
#endif
   for (i=0;i<3*st->frame_size;i++)
      st->play_buf[i] = 0;
   st->play_buf_pos = PLAYBACK_DELAY*st->frame_size;
   st->play_buf_started = 0;

}

/** Destroys an echo canceller state */
EXPORT void speex_echo_state_destroy(SpeexEchoState *st)
{
   spx_fft_destroy(st->fft_table);

   speex_free(st->e);
   speex_free(st->x);
   speex_free(st->input);
   speex_free(st->y);
   speex_free(st->last_y);
   speex_free(st->Yf);
   speex_free(st->Rf);
   speex_free(st->Xf);
   speex_free(st->Yh);
   speex_free(st->Eh);

   speex_free(st->X);
   speex_free(st->Y);
   speex_free(st->E);
   speex_free(st->W);
   speex_free(st->foreground);
   speex_free(st->PHI);
   speex_free(st->power);
   speex_free(st->power_1);
   speex_free(st->window);
   speex_free(st->prop);
   speex_free(st->wtmp);
   speex_free(st->memX);
   speex_free(st->memD);
   speex_free(st->memE);
   speex_free(st->notch_mem);

   speex_free(st->play_buf);
   speex_free(st);
}
/*
EXPORT void speex_echo_capture(SpeexEchoState *st, const spx_word16_t *rec, spx_word16_t *out,int * extreme_filter_divergence)
{
   int i;
   //speex_warning_int("capture with fill level ", st->play_buf_pos/st->frame_size);
   st->play_buf_started = 1;
   if (st->play_buf_pos>=st->frame_size)
   {	
      speex_echo_cancellation(st, rec, st->play_buf, out,extreme_filter_divergence);
      st->play_buf_pos -= st->frame_size;
      for (i=0;i<st->play_buf_pos;i++)
         st->play_buf[i] = st->play_buf[i+st->frame_size];
   } else {
      speex_warning("No playback frame available (your application is buggy and/or got xruns)");
      if (st->play_buf_pos!=0)
      {
         speex_warning("internal playback buffer corruption?");
         st->play_buf_pos = 0;
      }
      for (i=0;i<st->frame_size;i++)
         out[i] = rec[i];
   }
}*/

EXPORT void speex_echo_playback(SpeexEchoState *st, const spx_int16_t *play)
{
   /*speex_warning_int("playback with fill level ", st->play_buf_pos/st->frame_size);*/
   if (!st->play_buf_started)
   {
      speex_warning("discarded first playback frame");
      return;
   }
   if (st->play_buf_pos<=PLAYBACK_DELAY*st->frame_size)
   {
      int i;
      for (i=0;i<st->frame_size;i++)
         st->play_buf[st->play_buf_pos+i] = play[i];
      st->play_buf_pos += st->frame_size;
      if (st->play_buf_pos <= (PLAYBACK_DELAY-1)*st->frame_size)
      {
         speex_warning("Auto-filling the buffer (your application is buggy and/or got xruns)");
         for (i=0;i<st->frame_size;i++)
            st->play_buf[st->play_buf_pos+i] = play[i];
         st->play_buf_pos += st->frame_size;
      }
   } else {
      speex_warning("Had to discard a playback frame (your application is buggy and/or got xruns)");
   }
}

/** Performs echo cancellation on a frame (deprecated, last arg now ignored) */
/*
EXPORT void speex_echo_cancel(SpeexEchoState *st, const spx_word16_t *in, const spx_word16_t *far_end, spx_word16_t *out, spx_word32_t *Yout)
{
   speex_echo_cancellation(st, in, far_end, out);
}*/

/** Performs echo cancellation on a frame */
EXPORT  float* speex_echo_cancellation(SpeexEchoState *st, const spx_word16_t *in, const spx_word16_t *far_end, spx_word16_t *out,int* extreme_filter_divergence, int* changemode,float* psd_sxx, complex_t* psd_sxd)
{
   int i,j, chan, speak;
   int N, M;
   spx_word32_t Syy,See,Sxx,Sdd, Sff;
   spx_word32_t Sey;
   spx_word16_t ss, ss_1;
   spx_float_t Pey = FLOAT_ONE, Pyy=FLOAT_ONE;
   spx_float_t alpha, alpha_1;
   spx_word16_t RER;
   spx_word32_t tmp32;
   
   N = st->window_size;
   M = st->M;
   st->cancel_count++;
   ss=.35/M;
   ss_1 = 1-ss;

   if (st->cancel_count == 32) {
	//   printf("1");
   }
   spx_word16_t sumin = 0;
   for (int i = 0; i < 64; i++) {
	   sumin += *(in + i);
   }
   if (sumin) {
	  // printf("count = %d ",st->cancel_count);
   }
   float *debug= (float*)malloc(65*sizeof(float)) ;
   memset(debug, 0, 65 * sizeof(float));

    /* Apply a notch filter to make sure DC doesn't end up causing problems */
    filter_dc_notch16(in, st->input, st->frame_size);
	 
    /* Copy input data to buffer and apply pre-emphasis */
    /* Copy input data to buffer */
    for (i=0;i<st->frame_size;i++)
    {
        spx_word32_t tmp32;
        /* FIXME: This core has changed a bit, need to merge properly */
        tmp32 = SUB32(EXTEND32(st->input[i]), EXTEND32(MULT16_16_P15(st->preemph, *st->memD)));
        *st->memD = st->input[i];
        st->input[i] = EXTRACT16(tmp32);
    }

    for (i=0;i<st->frame_size;i++)
    {
        spx_word32_t tmp32;
        st->x[i] = st->x[i+st->frame_size];
        tmp32 = SUB32(EXTEND32(far_end[i]), EXTEND32(MULT16_16_P15(st->preemph, *st->memX)));
        st->x[i+st->frame_size] = EXTRACT16(tmp32);
        *st->memX = far_end[i];
    }

    /* Shift memory: this could be optimized eventually*/
    for (j=M-2;j>=0;j--)
    {
        for (i=0;i<N;i++)
			st->X[(j+1)*N+i] = st->X[j*N+i];
    }

    /* Convert x (echo input) to frequency domain */
    spx_fft(st->fft_table, &st->x[0], &st->X[0]);
	if (*changemode) {
		memset(st->W, 0,M*N * sizeof(spx_word32_t));
		st->W[0] = psd_sxd[0][0] / psd_sxx[0];
		for (int i = 1; i < N-1; i+=2) {
			st->W[i]   = psd_sxd[0][(i+1)/2] / (psd_sxx[(i + 1) / 2]+ 1e-16);
			st->W[i+1] = psd_sxd[1][(i+1)/2] / (psd_sxx[(i + 1) / 2] + 1e-16);
		}
		st->W[N-1] = psd_sxd[0][N / 2] / (psd_sxx[N/2] + 1e-16);
		*changemode = 0;
	}
	if (*extreme_filter_divergence) {
		//memset(st->W, 0,M*N * sizeof(spx_word32_t));
		/*st->W[0] = psd_sxd[0][0] / psd_sxx[0];
		for (int i = 1; i < N-1; i+=2) {
			st->W[i]   = psd_sxd[0][(i+1)/2] / (psd_sxx[(i + 1) / 2]+ 1e-16);
			st->W[i+1] = psd_sxd[1][(i+1)/2] / (psd_sxx[(i + 1) / 2] + 1e-16);
			printf("%d\n",i+1);
		}
		st->W[N-1] = psd_sxd[0][N / 2] / (psd_sxx[N/2] + 1e-16);*/
		*extreme_filter_divergence = 0;
		//st->screwed_up = 0;
	}

	/* FIXME: MC conversion required */
	/* Update weight to prevent circular convolution (MDF / AUMDF) */
	for (j = 0; j<M; j++)
	{
		/* This is a variant of the Alternatively Updated MDF (AUMDF) */
		/* Remove the "if" to make this an MDF filter */
		if (j == 0 || st->cancel_count % (M - 1) == j - 1)
		{
			spx_ifft(st->fft_table, &st->W[j*N], st->wtmp);
			for (i = st->frame_size; i<N; i++)
			{
				st->wtmp[i] = 0;
			}
			spx_fft(st->fft_table, st->wtmp, &st->W[j*N]);
		}
	}
	/* So we can use power_spectrum_accum */
	for (i = 0; i <= st->frame_size; i++)
		st->Rf[i] = st->Yf[i] = st->Xf[i] = 0;

	
	/* Difference in response, this is used to estimate the variance of our residual power estimate */
	spectral_mul_accum(st->X, st->W, st->Y, N, M);
	spx_ifft(st->fft_table, st->Y, st->y);
	for (i = 0; i<st->frame_size; i++)
		st->e[i] = SUB16(st->input[i], st->y[i + st->frame_size]);
	See = mdf_inner_prod(st->e, st->e, st->frame_size);
	/*//验证 far==0
	float tmp = 0;
	for (int i = 0; i < st->frame_size; i++) {
		tmp += st->y[i + st->frame_size];
	}
	if (tmp != 0)
		printf("不为零\n");*/
	/* Compute error signal (for the output with de-emphasis) */
	for (i = 0; i<st->frame_size; i++)
	{
		spx_word32_t tmp_out;
		tmp_out = SUB32(EXTEND32(st->input[i]), EXTEND32(st->y[i + st->frame_size]));
		/*if(tmp_out - st->input[i]>1)
		printf("tmp_out = %f  st->input[i] = %f \n",tmp_out,st->input[i]);*/
		tmp_out = ADD32(tmp_out, EXTEND32(MULT16_16_P15(st->preemph, *st->memE)));
		/* This is an arbitrary test for saturation in the microphone signal */
		if (in[i] <= -32000 || in[i] >= 32000)
		{
			if (st->saturated == 0)
				st->saturated = 1;
		}
		out[i] = WORD2INT(tmp_out);
		*st->memE = tmp_out;
	}

	/* Compute error signal (filter update version) */
	for (i = 0; i<st->frame_size; i++)
	{
		st->e[i + st->frame_size] = st->e[i];
		st->e[i] = 0;
	}

    /* Compute a bunch of correlations */
    /* FIXME: bad merge */
    Sey = mdf_inner_prod(st->e+st->frame_size, st->y+st->frame_size, st->frame_size);
    Syy = mdf_inner_prod(st->y+st->frame_size, st->y+st->frame_size, st->frame_size);
	//printf("%f\n", Syy);
    /* Convert error to frequency domain */
    spx_fft(st->fft_table, st->e, st->E);
    for (i=0;i<st->frame_size;i++)
        st->y[i] = 0;
    spx_fft(st->fft_table, st->y, st->Y);
   
    /* Compute power spectrum of echo (X), error (E) and filter response (Y) */
    power_spectrum_accum(st->E, st->Rf, N);
    power_spectrum_accum(st->Y, st->Yf, N);

	See = MAX32(See, SHR32(MULT16_16(N, 100), 6));
	Sxx = mdf_inner_prod(st->x + st->frame_size, st->x + st->frame_size, st->frame_size);
	power_spectrum_accum(st->X, st->Xf, N);

   /* Smooth far end energy estimate over time */
   for (j=0;j<=st->frame_size;j++)
      st->power[j] = MULT16_32_Q15(ss_1,st->power[j]) + 1 + MULT16_32_Q15(ss,st->Xf[j]);


   /* Compute filtered spectra and (cross-)correlations */
   for (j=st->frame_size;j>=0;j--)
   {
      spx_float_t Eh, Yh;
      Eh = PSEUDOFLOAT(st->Rf[j] - st->Eh[j]);
      Yh = PSEUDOFLOAT(st->Yf[j] - st->Yh[j]);
      Pey = FLOAT_ADD(Pey,FLOAT_MULT(Eh,Yh));
      Pyy = FLOAT_ADD(Pyy,FLOAT_MULT(Yh,Yh));
      st->Eh[j] = (1-st->spec_average)*st->Eh[j] + st->spec_average*st->Rf[j];
      st->Yh[j] = (1-st->spec_average)*st->Yh[j] + st->spec_average*st->Yf[j];
   }
   
   Pyy = FLOAT_SQRT(Pyy);
   Pey = FLOAT_DIVU(Pey,Pyy);

   /* Compute correlation updatete rate */
   tmp32 = MULT16_32_Q15(st->beta0,Syy);
   if (tmp32 > See)
      tmp32 = See;
   alpha = FLOAT_DIV32(tmp32, See);
   alpha_1 = FLOAT_SUB(FLOAT_ONE, alpha);
  // printf("%f  \n",alpha_1);
   //alpha_1 = 0.8;
   /* Update correlations (recursive average) */
   st->Pey = FLOAT_ADD(FLOAT_MULT(alpha_1,st->Pey) , FLOAT_MULT(alpha,Pey));
   st->Pyy = FLOAT_ADD(FLOAT_MULT(alpha_1,st->Pyy) , FLOAT_MULT(alpha,Pyy));
   if (FLOAT_LT(st->Pyy, FLOAT_ONE))
      st->Pyy = FLOAT_ONE;
   /* We don't really hope to get better than 33 dB (MIN_LEAK-3dB) attenuation anyway */
   if (FLOAT_LT(st->Pey, FLOAT_MULT(MIN_LEAK,st->Pyy)))
      st->Pey = FLOAT_MULT(MIN_LEAK,st->Pyy);
   if (FLOAT_GT(st->Pey, st->Pyy))
      st->Pey = st->Pyy;
   /* leak_estimate is the linear regression result */
   st->leak_estimate = FLOAT_EXTRACT16(FLOAT_SHL(FLOAT_DIVU(st->Pey, st->Pyy),14));
   /* This looks like a stupid bug, but it's right (because we convert from Q14 to Q15) */
   if (st->leak_estimate > 16383)
      st->leak_estimate = 32767;
   else
      st->leak_estimate = SHL16(st->leak_estimate,1);
    // printf ("%f\n", st->leak_estimate);
   
   /* Compute Residual to Error Ratio */

   RER = (.0001*Sxx + 3.*MULT16_32_Q15(st->leak_estimate,Syy)) / See;
   /* Check for y in e (lower bound on RER) */
   if (RER < Sey*Sey/(1+See*Syy))
      RER = Sey*Sey/(1+See*Syy);
   if (RER > .5)
      RER = .5;

   /* We consider that the filter has had minimal adaptation if the following is true*/
  if (!st->adapted && st->sum_adapt > SHL32(EXTEND32(M),15) && MULT16_32_Q15(st->leak_estimate,Syy) > MULT16_32_Q15(QCONST16(.03f,15),Syy))
   {
      st->adapted = 1;
   }
 // st->adapted = 1;
   if (st->adapted)
   {
      /* Normal learning rate calculation once we're past the minimal adaptation phase */
      for (i=0;i<=st->frame_size;i++)
      {
         double r, e;
         /* Compute frequency-domain adaptation mask */
         r = MULT16_32_Q15(st->leak_estimate,SHL32(st->Yf[i],3));
         //e = SHL32(st->Rf[i],3)+1;
		 e = (double)(st->Rf[i]) + 1;
         if (r>.5*e)
            r = .5*e;
         r = MULT16_32_Q15(QCONST16(.7,15),r) + MULT16_32_Q15(QCONST16(.3,15),(double)(MULT16_32_Q15(RER,e)));
         /*st->power_1[i] = adapt_rate*r/(e*(1+st->power[i]));*/
         st->power_1[i] = FLOAT_SHL(FLOAT_DIV32_FLOAT(r,FLOAT_MUL32U(e,st->power[i])+10),WEIGHT_SHIFT+16);
      }

   } else {
      /* Temporary adaption rate if filter is not yet adapted enough */
      spx_word16_t adapt_rate=0;

     /* if (Sxx > SHR32(MULT16_16(N, 1000),6)) 
      {
         tmp32 = MULT16_32_Q15(QCONST16(.25f, 15), Sxx);
         if (tmp32 > .25*See)
            tmp32 = .25*See;
         adapt_rate = FLOAT_EXTRACT16(FLOAT_SHL(FLOAT_DIV32(tmp32, See),15));
      }*/
	  //adapt_rate = 1;
      for (i=0;i<=st->frame_size;i++)
         st->power_1[i] = FLOAT_SHL(FLOAT_DIV32(EXTEND32(adapt_rate),ADD32(st->power[i],10)),WEIGHT_SHIFT+1);
      /* How much have we adapted so far? */
      st->sum_adapt = ADD32(st->sum_adapt,adapt_rate);
   }
   /* Adjust proportional adaption rate */
   /* FIXME: Adjust that for C, K*/
   if (st->adapted)
	   mdf_adjust_prop(st->W, N, M, st->prop);
   /* Compute weight gradient */
   if (st->saturated == 0)
   {
	   for (j = M - 1; j >= 0; j--)
	   {
		   weighted_spectral_mul_conj(st->power_1, FLOAT_SHL(PSEUDOFLOAT(st->prop[j]), -15), &st->X[j*N], st->E, st->PHI, N);
		   for (i = 0; i<N; i++)
			   st->W[j*N + i] += st->PHI[i];
	   }
   }
   else {
	   st->saturated--;
   }
   for (int a = 0; a < 65; a++) {
	   debug[a] = st->power_1[a];
   }
   return debug;
}

/* Compute spectrum of estimated echo for use in an echo post-filter */
void speex_echo_get_residual(SpeexEchoState *st, spx_word32_t *residual_echo, int len)
{
   int i;
   spx_word16_t leak2;
   int N;
   
   N = st->window_size;

   /* Apply hanning window (should pre-compute it)*/
   for (i=0;i<N;i++)
      st->y[i] = MULT16_16_Q15(st->window[i],st->last_y[i]);
      
   /* Compute power spectrum of the echo */
   spx_fft(st->fft_table, st->y, st->Y);
   power_spectrum(st->Y, residual_echo, N);
      
#ifdef FIXED_POINT
   if (st->leak_estimate > 16383)
      leak2 = 32767;
   else
      leak2 = SHL16(st->leak_estimate, 1);
#else
   if (st->leak_estimate>.5)
      leak2 = 1;
   else
      leak2 = 2*st->leak_estimate;
#endif
   /* Estimate residual echo */
   for (i=0;i<=st->frame_size;i++)
      residual_echo[i] = (spx_int32_t)MULT16_32_Q15(leak2,residual_echo[i]);
   
}

EXPORT int speex_echo_ctl(SpeexEchoState *st, int request, void *ptr)
{
   switch(request)
   {
      
      case SPEEX_ECHO_GET_FRAME_SIZE:
         (*(int*)ptr) = st->frame_size;
         break;
      case SPEEX_ECHO_SET_SAMPLING_RATE:
         st->sampling_rate = (*(int*)ptr);
         st->spec_average = DIV32_16(SHL32(EXTEND32(st->frame_size), 15), st->sampling_rate);
         st->beta0 = (2.0f*st->frame_size)/st->sampling_rate;
         st->beta_max = (.5f*st->frame_size)/st->sampling_rate;
         if (st->sampling_rate<12000)
            st->notch_radius = QCONST16(.9, 15);
         else if (st->sampling_rate<24000)
            st->notch_radius = QCONST16(.982, 15);
         else
            st->notch_radius = QCONST16(.992, 15);
         break;
      case SPEEX_ECHO_GET_SAMPLING_RATE:
         (*(int*)ptr) = st->sampling_rate;
         break;
      case SPEEX_ECHO_GET_IMPULSE_RESPONSE_SIZE:
         /*FIXME: Implement this for multiple channels */
         *((spx_int32_t *)ptr) = st->M * st->frame_size;
         break;
      case SPEEX_ECHO_GET_IMPULSE_RESPONSE:
      {
         int M = st->M, N = st->window_size, n = st->frame_size, i, j;
         spx_int32_t *filt = (spx_int32_t *) ptr;
         for(j=0;j<M;j++)
         {
            spx_ifft(st->fft_table, &st->W[j*N], st->wtmp);
            for(i=0;i<n;i++)
               filt[j*n+i] = PSHR32(MULT16_16(32767,st->wtmp[i]), WEIGHT_SHIFT-NORMALIZE_SCALEDOWN);
         }
      }
         break;
      default:
         speex_warning_int("Unknown speex_echo_ctl request: ", request);
         return -1;
   }
   return 0;
}

void test_speex_echo(int x)
{
	printf("ttttttttttt  = %d\n", SHL16(x,2));
}
