//
// Created by Elevoc on 2021/1/7 0007.
//

#ifndef TEST_TEST_CONSTANT_H
#define TEST_TEST_CONSTANT_H

#include <inttypes.h>

//#define TELENEWBIE_44100_c1_b24
#define TELENEWBIE_48000_c2_b16
//#define TELENEWBIE_48000_c1_f32
//#define TELENEWBIE_48000_c4_f32
#define RESAMPLE_OUT_RATE 16000
#define FRAME_TIME_MILLISECOND 10

#define DEBUG_FILE


#if defined(TELENEWBIE_48000_c2_b16)
#define TEST_PCM_DIR "D:\\code\\Test4Myself\\testCPP\\testStruct\\asserts\\"
#define TEST_PCM_NAME "48000_c2_b16_near" // 16000_c1_16b.pcm
#define TEST_REF_PCM_NAME "48000_c2_b16_far" // 16000_c1_16b.pcm
#define TEST_SAMPLE_RATE 48000
#define CHANNEL_MIC 2
#define CHANNEL_REF 2
typedef int16_t TYPE_SAMPLE_t;
#elif defined(TELENEWBIE_48000_c4_f32)
#define TEST_PCM_DIR "D:\\code\\Test4Myself\\testCPP\\testStruct\\asserts\\"
#define TEST_PCM_NAME "48000_c4_f32_near"
#define TEST_REF_PCM_NAME ""
//#define TEST_REF_PCM_NAME "48000_c2_f32_far"
#define TEST_SAMPLE_RATE 48000
#define CHANNEL_MIC 4
#define CHANNEL_REF 0
typedef float TYPE_SAMPLE_t;
#elif defined(TELENEWBIE_48000_c1_f32)
#define TEST_PCM_DIR "D:\\code\\Test4Myself\\testCPP\\testStruct\\asserts\\"
#define TEST_PCM_NAME "48000_c1_f32_mic"
#define TEST_REF_PCM_NAME "48000_c1_f32_ref"
#define TEST_SAMPLE_RATE 48000
#define CHANNEL_MIC 1
#define CHANNEL_REF 1
typedef float TYPE_SAMPLE_t;
#elif defined(TELENEWBIE_44100_c1_b24)
#define TEST_PCM_DIR "D:\\code\\Test4Myself\\testCPP\\testStruct\\asserts\\"
#define TEST_PCM_NAME "44100_c1_24b_near"
#define TEST_REF_PCM_NAME "44100_c1_24b_far" // 16000_c1_16b.pcm
#define TEST_SAMPLE_RATE 44100
#define TEST_CHANNEL 1
typedef float TYPE_SAMPLE_t;
#else
#define TEST_PCM_DIR "D:\\code\\Test4Myself\\testCPP\\testStruct\\asserts\\"
#define TEST_PCM_NAME "b-o-audio_near"
#define TEST_REF_PCM_NAME "b-o-audio_far"
#endif

#define MAX_CHANNEL (CHANNEL_MIC+CHANNEL_REF)

//#define TEST_BytePerSample 2

//#define TEST_FLOAT_BYTE

#define FRAME_SIZE_ONE (TEST_SAMPLE_RATE/1000*FRAME_TIME_MILLISECOND)
#define READ_MIC_SIZE (TEST_SAMPLE_RATE/1000*FRAME_TIME_MILLISECOND*sizeof(TYPE_SAMPLE_t)*(CHANNEL_MIC))
#define READ_REF_SIZE (TEST_SAMPLE_RATE/1000*FRAME_TIME_MILLISECOND*sizeof(TYPE_SAMPLE_t)*(CHANNEL_REF))


//#define TEST_PCM_NAME "48000_c1_16" // 16000_c1_16b.pcm

// mic  48k float
// b-o-audio_near b-o-audio_far 48k c2 16bit
//testRef04.pcm testMic.pcm


#define TEST_PCM_PATH (TEST_PCM_DIR TEST_PCM_NAME ".pcm")
#define TEST_REF_PCM_PATH (TEST_PCM_DIR TEST_REF_PCM_NAME ".pcm")
#define TEST_PCM_OUT_PATH (TEST_PCM_DIR TEST_PCM_NAME  "_out.pcm")
const int MAX_BUFFER_LENGTH = 1000;//1s 的数据

#endif //TEST_TEST_CONSTANT_H
