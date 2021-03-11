//
// Created by Elevoc on 2021/3/9 0009.
//

#include <cstdio>
#include "SpeexResamplerAdapter.h"
#include "Processer/impl/resample/speex/speex_resampler.h"

void SpeexResamplerAdapter::release() {
    if (ptr) {
        elevoc_resampler_destroy((SpeexResamplerState *) ptr);
    }
}

int SpeexResamplerAdapter::initSample(uint32_t nb_channels, uint32_t in_rate, uint32_t out_rate) {
    if (ptr) {
        printf("??? dont do that\n");
        return 0;
    }
    int err = 0;
    ptr = elevoc_resampler_init(nb_channels, in_rate, out_rate, 9, &err);
    if (ptr) {
        elevoc_resampler_set_rate((SpeexResamplerState *) ptr, in_rate, out_rate);
    }
    return err;
}

// 非0 就是 error
int SpeexResamplerAdapter::resampler_process(int index,short *inBuffer, uint32_t inLen, short *outBuffer, uint32_t &outLen) {
    if (!ptr)
        return -1;
    return elevoc_resampler_process_int(
            (SpeexResamplerState *) ptr,
            index, inBuffer, &inLen, outBuffer, &outLen);
}

int SpeexResamplerAdapter::resampler_process(int index,float *inBuffer, uint32_t inLen, float *outBuffer, uint32_t &outLen) {
    if (!ptr) {
        printf("ptr error!!!\n");
        return -1;
    }
    return elevoc_resampler_process_float(
            (SpeexResamplerState *) ptr,
            index, inBuffer, &inLen, outBuffer, &outLen);
}
