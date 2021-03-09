//
//  EVPCMHelper.cpp
//  SingleChannelEngine
//
//  Created by csyyj on 2017/8/2.
//  Copyright © 2017年 elevoc. All rights reserved.
//

#include "EVPCMHelper.h"
#include "EVAssert.h"
#include "ElevocLog.h"

void convertFloatToByte(const float *inputData, size_t inputLen, char *outputData, size_t *outputLen, MODE mode) {
    int i;

    float scale = (mode == VOIP) ? 4.f : 1.f;

    for (i = 0; i < inputLen; i++) {
        float t = inputData[i] * 0.95f * scale;
        if (t > 0.98f) {
            t = 0.98f;
        } else if (t < -0.98f) {
            t = -0.98f;
        }
        short value = (t * 32767);
        outputData[i * 2] = value & (0xff);
        outputData[i * 2 + 1] = (value >> 8) & 0xff;
    }
    if (outputLen != NULL) {
        *outputLen = inputLen * 2;
    }
}

void convertFloatToShort(const float *inputData, size_t inputLen, short *outputData, size_t *outputLen, MODE mode) {
    int i;

    float scale = (mode == VOIP) ? 4.f : 1.f;

    for (i = 0; i < inputLen; i++) {
        float t = inputData[i] * 0.95f * scale;
        if (t > 0.98f) {
            t = 0.98f;
        } else if (t < -0.98f) {
            t = -0.98f;
        }
        short value = (t * 32767);
        outputData[i] = value & (0xffff);
    }
    if (outputLen != NULL) {
        *outputLen = inputLen;
    }
}

void convertFloatToByte_AEC(const float *inputData, size_t inputLen, char *outputData, size_t *outputLen) {
    int i;

    for (i = 0; i < inputLen; i++) {
        float t = inputData[i] * 0.96f;
        if (t > 32760.f) {
            t = 32760.f;
        } else if (t < -32760.f) {
            t = -32760.0f;
        }
        short value = (t);
        outputData[i * 2] = value & (0xff);
        outputData[i * 2 + 1] = (value >> 8) & 0xff;
    }
    if (outputLen != NULL) {
        *outputLen = inputLen * 2;
    }
}

void convertByteToFloat(const char *inputData, size_t inputLen, float *outputData, size_t *outputLen, MODE mode) {
    int i;
    ele_assert(inputLen % 2 == 0, "Input len must be even."); //传入长度必须为偶数

    float scale = (mode == VOIP) ? 0.25f : 1.f;

    for (i = 0; i < inputLen / 2; i++) {
        outputData[i] = (short) (((inputData[i * 2 + 1] << 8) & 0xFF00) | (0xff & inputData[i * 2])) / 32768.0f * scale;
    }
    if (outputLen != NULL) {
        *outputLen = inputLen / 2;
    }
}


void maxVolum(short *audioData, size_t len, int strongValue) {
    for (int i = 0; i < len; i++) {
        audioData[i] *= strongValue;
    }
}
