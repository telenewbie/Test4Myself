//
// Created by Elevoc on 2021/1/7 0007.
//

#ifndef TEST_BASERESAMPLE_H
#define TEST_BASERESAMPLE_H

#define SAMPLE_RATE_8k  8000
#define SAMPLE_RATE_16k 16000
#define SAMPLE_RATE_32k 32000
#define SAMPLE_RATE_48k 48000
#define SAMPLE_RATE_441k 44100
#define MAX_FRAME_BYTE_SIZE (48*10 *2) // 需要被修改 成具体的
#include "ElevocLog.h"
#include "Processer/impl/resample/speex/speex_resampler.h"




#if 0
#define INIT_RESAMPLE(inChannel, inSampleRate, outSampleRate) \
        ({\
            int err;\
    SpeexResamplerState *sample = elevoc_resampler_init(inChannel, inSampleRate, outSampleRate, 9, &err);\
    if (sample != NULL) {\
        elevoc_resampler_set_rate(sample, inSampleRate, outSampleRate);\
    }else {\
        LOGE("resampler error 44to48 in");\
    }     \
    sample;\
        })

#define STATEMENT_RESAMPLE1(inSampleRate,outSampleRate) \
SpeexResamplerState * resample_##inSampleRate##_to_##outSampleRate##_in = NULL;\
SpeexResamplerState * resample_##inSampleRate##_to_##outSampleRate##_far = NULL;\
SpeexResamplerState * resample_##inSampleRate##_to_##outSampleRate##_out = NULL;

#define STATEMENT_RESAMPLE(inSampleRate,outSampleRate) STATEMENT_RESAMPLE1(inSampleRate,outSampleRate)

#define INIT_RESAMPLE_GROUP1(inChannel, inSampleRate, outSampleRate) \
resample_##inSampleRate##_to_##outSampleRate##_in =  INIT_RESAMPLE(inChannel, inSampleRate, outSampleRate); \
resample_##inSampleRate##_to_##outSampleRate##_far =  INIT_RESAMPLE(inChannel, inSampleRate, outSampleRate); \
resample_##inSampleRate##_to_##outSampleRate##_out =  INIT_RESAMPLE(inChannel, outSampleRate,inSampleRate);

#define INIT_RESAMPLE_GROUP(inChannel, inSampleRate, outSampleRate) INIT_RESAMPLE_GROUP1(inChannel, inSampleRate, outSampleRate)

#define RESAMPLE_METHOD(TAG) \
SpeexResamplerState * getResampleStatePtr##TAG(int inSample,int outSample) {\
    if(inSample == SAMPLE_RATE_441k){\
        return resample_44100_to_16000_##TAG;\
    }else if (inSample == SAMPLE_RATE_48k){\
        return resample_48000_to_16000_##TAG;\
    }else if (inSample == SAMPLE_RATE_32k){\
        return resample_32000_to_16000_##TAG;\
    }else{                   \
    return NULL;                         \
    }\
}


#define DESTROY_RESAMPLE_GROUP1(inSampleRate, outSampleRate) \
if (resample_##inSampleRate##_to_##outSampleRate##_in != NULL) {\
    elevoc_resampler_destroy(resample_##inSampleRate##_to_##outSampleRate##_in);\
    resample_##inSampleRate##_to_##outSampleRate##_in = NULL;\
}\
if (resample_##inSampleRate##_to_##outSampleRate##_out != NULL) {\
    elevoc_resampler_destroy(resample_##inSampleRate##_to_##outSampleRate##_out);\
    resample_##inSampleRate##_to_##outSampleRate##_out = NULL;\
}\
if (resample_##inSampleRate##_to_##outSampleRate##_far!= NULL) {\
    elevoc_resampler_destroy(resample_##inSampleRate##_to_##outSampleRate##_far);\
    resample_##inSampleRate##_to_##outSampleRate##_far = NULL;\
}
#define DESTROY_RESAMPLE_GROUP(inSampleRate, outSamleRate) DESTROY_RESAMPLE_GROUP1(inSampleRate, outSamleRate)


#endif
#endif //TEST_BASERESAMPLE_H
