//
//  ElevocTypeDef.h
//  SingleChannelEngine
//
//  Created by 蔡鹏 on 2018/10/25.
//  Copyright © 2018 elevoc. All rights reserved.
//

#ifndef ElevocTypeDef_h
#define ElevocTypeDef_h

/*

!!!!! attention

--------- lstm ----------

 	NS_TYPE_LSTM -> MODE_TYPE_PHONE     (8k, 16k)
	NS_TYPE_LSTM -> MODE_TYPE_PHONE_VAD (16k)
    NS_TYPE_LSTM_MULI_SAMPLE_RATE -> MODE_TYPE_PHONE (支持8~48k) (下行voip场景)

--------- cnn ----------	

	NS_TYPE_CNN (only 16k)
    NS_TYPE_R10 (only 16k)

 */


//#define TRIAL_LIMITATION_DNN (60 * 10)  // 试用限制时间，单位为秒

#define NS_TYPE_LSTM				// NS_TYPE_CNN NS_TYPE_LSTM NS_TYPE_R10
#define MODE_TYPE_PHONE_VAD			// MODE_TYPE_PHONE MODE_TYPE_PHONE_VAD
//#define IS_RS_NS

typedef struct { float real, imag; } ele_complex_float;

#define complexABS(a) sqrtf(a.real * a.real + a.imag * a.imag)
#define complexMAG(a) (a.real * a.real + a.imag * a.imag)
#define elevocMin(a, b) (a) < (b) ? (a) : (b)
#define elevocMax(a, b) (a) > (b) ? (a) : (b)

#define SHIFT           (160)
#define WLEN            (320)
#define NUM_FRQ         (161)
#define FEATURE_SIZE    (161)

#define MAX_SHIFT       (480)
#define MAX_WLEN        (960)
#define MAX_NUM_FRQ     (481)

#define NUM_MIC         (1)
#define NUM_REF         (0)
#define PI              3.141592654f

#define ELEVOC_DNN_SAMPLERATE_8K    (8000)
#define ELEVOC_DNN_SAMPLERATE_16K   (16000)
#define ELEVOC_DNN_SAMPLERATE_32K   (32000)
#define ELEVOC_DNN_SAMPLERATE_48K   (48000)

//typedef int BOOLEAN;
//#define true            1
//#define false           0

#if defined(_MSC_VER)
#define  ELEVOC_ALIGNED_32_
#else
#define  ELEVOC_ALIGNED_32_   __attribute__((aligned(32)))
#endif

#if defined(_MSC_VER)
#define ELEVOC_CHECK_FMT(a, b)
#else
#define ELEVOC_CHECK_FMT(a, b) __attribute__((format(printf, a, b)))
#endif

#if defined(_MSC_VER)
//  Microsoft
#define DNNEXPORT __declspec(dllexport)
#define DNNIMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
#define DNNEXPORT __attribute__((visibility("default")))
#define DNNIMPORT
#else
//  do nothing and hope for the best?
#define DNNEXPORT
#define DNNIMPORT
#pragma warning Unknown dynamic link import/export semantics.
#endif

#ifdef DNN_EVLIB_EXPORTS
#define DNNEVAPI DNNEXPORT
#else
#define DNNEVAPI DNNIMPORT
#endif

//#define ele_complex_float  _ele_complex_float

#endif /* ElevocTypeDef_h */
