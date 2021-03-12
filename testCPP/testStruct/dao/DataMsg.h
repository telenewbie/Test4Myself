//
// Created by Elevoc on 2021/1/4 0004.
//

#ifndef TEST_DATAMSG_H
#define TEST_DATAMSG_H

#include <inttypes.h>
#include "TEST_CONSTANT.h"

class DataMsg {
public:
    DataMsg();
    void init();// required

    int index = 0;
    uint32_t sample_num; //
    uint32_t mic_buff_size; //
    TYPE_SAMPLE_t *micBuff = nullptr; //
    TYPE_SAMPLE_t *refBuff = nullptr;


    int inSampleRate = 0;
    int outSampleRate = 0;
    int refChannel = 1;
    int micChannel = 1;
    int bytesPerSample = sizeof(TYPE_SAMPLE_t);//

//    TYPE_SAMPLE_t *proRefBuff = nullptr; //
//    TYPE_SAMPLE_t *proMicBuff = nullptr; //

    uint32_t getId();

private:
    uint32_t id;
};


#endif //TEST_DATAMSG_H
