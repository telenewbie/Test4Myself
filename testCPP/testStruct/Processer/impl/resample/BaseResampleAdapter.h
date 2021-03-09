//
// Created by Elevoc on 2021/3/9 0009.
//

#ifndef TEST1_BASERESAMPLEADAPTER_H
#define TEST1_BASERESAMPLEADAPTER_H

// 基础的转换适配器接口
#include <inttypes.h>

class BaseResampleAdapter {

public:
    virtual int initSample(uint32_t nb_channels, uint32_t in_rate, uint32_t out_rate) = 0;

    virtual int resampler_process(short * inBuffer,uint32_t inLen, short* outBuffer , uint32_t & outLen) = 0;
    virtual int resampler_process(float * inBuffer,uint32_t inLen, float* outBuffer , uint32_t & outLen) = 0;

    virtual void release() = 0;
    void *ptr = nullptr;

};


#endif //TEST1_BASERESAMPLEADAPTER_H
