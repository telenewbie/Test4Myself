//
// Created by Elevoc on 2021/3/10 0010.
//

#ifndef TEST1_PROCESSORCONFIG_H
#define TEST1_PROCESSORCONFIG_H

#include "TEST_CONSTANT.h"

class ProcessorConfig {
public:
    // needNewThread
    ProcessorConfig();

    bool mNeedThread = false;
    int mMicChannel = 1;
    int mRefChannel = 0; // default: no ref channel

    int mInSampleRate = 0;
    int mOutSampleRate = 0;

    int mRefIndex[MAX_CHANNEL] = {0};
    int mMicIndex[MAX_CHANNEL] = {0};


};


#endif //TEST1_PROCESSORCONFIG_H
