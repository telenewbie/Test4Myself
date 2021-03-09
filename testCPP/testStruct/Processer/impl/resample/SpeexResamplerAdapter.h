//
// Created by Elevoc on 2021/3/9 0009.
//

#ifndef TEST1_SPEEXRESAMPLERADAPTER_H
#define TEST1_SPEEXRESAMPLERADAPTER_H
#include "BaseResampleAdapter.h"

class SpeexResamplerAdapter : public BaseResampleAdapter{

public:
    virtual int initSample(uint32_t nb_channels, uint32_t in_rate, uint32_t out_rate) override;

    virtual int resampler_process(short * inBuffer,uint32_t inLen, short* outBuffer , uint32_t & outLen) override;
    virtual int resampler_process(float * inBuffer,uint32_t inLen, float* outBuffer , uint32_t & outLen) override;

    virtual void release() override;

};


#endif //TEST1_SPEEXRESAMPLERADAPTER_H
