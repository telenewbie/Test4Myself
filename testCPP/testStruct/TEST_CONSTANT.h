//
// Created by Elevoc on 2021/1/7 0007.
//

#ifndef TEST_TEST_CONSTANT_H
#define TEST_TEST_CONSTANT_H

#include <inttypes.h>
#define TEST_SAMPLE_RATE 48000
#define TEST_CHANNEL 2
typedef int16_t TYPE_SAMPLE_t;
//#define TEST_BytePerSample 2

//#define TEST_FLOAT_BYTE

#define READ_SIZE (TEST_SAMPLE_RATE/100*sizeof(TYPE_SAMPLE_t))

#if 0
#define TEST_PCM_DIR "C:\\Users\\Lake\\Desktop\\"
#define TEST_PCM_NAME "testMic" // 16000_c1_16b.pcm
#define TEST_REF_PCM_NAME "testRef04" // 16000_c1_16b.pcm
#else
#define TEST_PCM_DIR "D:\\code\\Test4Myself\\testCPP\\testStruct\\asserts\\"
#define TEST_PCM_NAME "b-o-audio_near"
#define TEST_REF_PCM_NAME "b-o-audio_far" // 16000_c1_16b.pcm
#endif
//#define TEST_PCM_NAME "48000_c1_16" // 16000_c1_16b.pcm

// mic  48k float
// b-o-audio_near b-o-audio_far 48k c2 16bit
//testRef04.pcm testMic.pcm


#define TEST_PCM_PATH (TEST_PCM_DIR TEST_PCM_NAME ".pcm")
#define TEST_REF_PCM_PATH (TEST_PCM_DIR TEST_REF_PCM_NAME ".pcm")
#define TEST_PCM_OUT_PATH (TEST_PCM_DIR TEST_PCM_NAME  "_out.pcm")

#endif //TEST_TEST_CONSTANT_H