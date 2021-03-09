//
// Created by Elevoc on 2021/1/7 0007.
//

#ifndef TEST_RESAMPLEINPROCESSER_H
#define TEST_RESAMPLEINPROCESSER_H


#include <BaseProcesser.h>
#include "BaseResample.h"
#include "SpeexResamplerAdapter.h"



// 重采成16k，可以做成一个序列，依次插入 EventCenter 中
class ResampleInProcesser : public BaseProcesser {
    int getMsgIndex() override;

    void process(DataMsg *) override;

    bool canProcess(DataMsg *) override;

    ~ResampleInProcesser();

    void initResample(SpeexResamplerState **, DataMsg *mMsg);

public:
    virtual std::string getTag();

    ResampleInProcesser();

private:
//    STATEMENT_RESAMPLE(SAMPLE_RATE_441k, SAMPLE_RATE_16k);
//    STATEMENT_RESAMPLE(SAMPLE_RATE_32k, SAMPLE_RATE_16k);
//    STATEMENT_RESAMPLE(SAMPLE_RATE_48k, SAMPLE_RATE_16k);
//
//    RESAMPLE_METHOD(in);
//
//    RESAMPLE_METHOD(out);
//
//    RESAMPLE_METHOD(far);

    BaseResampleAdapter *resampleNear = nullptr;
    BaseResampleAdapter *resampleFar = nullptr;


//    SpeexResamplerState *mSpeexResamplerState = nullptr;
//    SpeexResamplerState *mSpeexResamplerRef = nullptr;

    char input_resampler_buffer[MAX_FRAME_BYTE_SIZE];
    char far_resampler_buffer[MAX_FRAME_BYTE_SIZE];
};


#endif //TEST_RESAMPLEINPROCESSER_H
