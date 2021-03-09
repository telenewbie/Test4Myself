//
// Created by Elevoc on 2021/1/7 0007.
//

#ifndef TEST_RESAMPLEOUTPROCESSER_H
#define TEST_RESAMPLEOUTPROCESSER_H


#include <Processer/BaseProcesser.h>
#include "BaseResample.h"


class ResampleOutProcesser : public BaseProcesser {
    int getMsgIndex() override;

    void process(DataMsg *) override;

    bool canProcess(DataMsg *) override;

    SpeexResamplerState *mSpeexResamplerState = nullptr;
    char input_resampler_buffer[MAX_FRAME_BYTE_SIZE];
public:
    virtual std::string getTag() override;
    ResampleOutProcesser();
};


#endif //TEST_RESAMPLEOUTPROCESSER_H