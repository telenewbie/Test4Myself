//
// Created by Elevoc on 2021/1/4 0004.
//

#ifndef TEST_PROCESSCENTER_H
#define TEST_PROCESSCENTER_H

#include <vector>
#include "Processer/BaseProcesser.h"

class ProcessCenter {

    ProcessCenter();

    ~ProcessCenter();

public:

    void init(int sampleRate, int channel, int bytePerSample);

    void start();

    void stop();

    void release();

    static ProcessCenter *getInstance();


private:
    bool mIsRelease = false;
    int mSampleRate = 0;
    int mChannel = 0;
    int mBytePerSample = 0;
};


#endif //TEST_PROCESSCENTER_H
