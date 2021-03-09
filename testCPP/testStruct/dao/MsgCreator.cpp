//
// Created by Elevoc on 2021/1/5 0005.
//

#include <cstdio>
#include <cstdlib>
#include "MsgCreator.h"
#include <algorithm>

MsgCreator *MsgCreator::getInstance() {
    static auto *instance = new MsgCreator();
    return instance;
}

const int MAX_BUFFER_LENGTH = 2000;//1s 的数据
int getNextBufferIndex() {
    static int index = 0;
    ++index;
    if (index >= MAX_BUFFER_LENGTH) {
        index = 0;
    }
    return index;
}


DataMsg *MsgCreator::create(int sampleNum, int sampleRate, int channel, int bytesPerSample) {
//    static auto *reserveMic = (short *) malloc(sample  * bytesPerSample * MAX_BUFFER_LENGTH); // 1s 数据
//    static auto *reserveRef = (short *) malloc(sample  * bytesPerSample * MAX_BUFFER_LENGTH);
//    static auto *reserveProcessMic = (short *) malloc(sample  * bytesPerSample * MAX_BUFFER_LENGTH);
//    static auto *reserveProcessRef = (short *) malloc(sample  * bytesPerSample * MAX_BUFFER_LENGTH);
//    static auto *reserveDataMsg = (DataMsg *) malloc(sizeof(DataMsg) * MAX_BUFFER_LENGTH);
    int index = getNextBufferIndex();
    auto *msg = new DataMsg;//&reserveDataMsg[index];
    msg->init();
    msg->micBuff =(TYPE_SAMPLE_t*) malloc(sampleNum*sizeof(TYPE_SAMPLE_t));//(short*)malloc(1024*sizeof(short));//&reserveMic[sampleNum  * bytesPerSample*index]; // todo:统一一块
    msg->refBuff =  (TYPE_SAMPLE_t*) malloc(sampleNum*sizeof(TYPE_SAMPLE_t));//(short*)malloc(1024*sizeof(short));//&reserveRef[sampleNum  * bytesPerSample*index];
//    msg->proMicBuff =  (TYPE_SAMPLE_t*) malloc(sampleNum*sizeof(TYPE_SAMPLE_t));//(short*)malloc(1024*sizeof(short));//&reserveProcessMic[sampleNum  * bytesPerSample*index];
//    msg->proRefBuff =  (TYPE_SAMPLE_t*) malloc(sampleNum*sizeof(TYPE_SAMPLE_t));//(short*)malloc(1024*sizeof(short));//&reserveProcessRef[sampleNum  * bytesPerSample*index];
    msg->index = 0;
    msg->sample_num = sampleNum;
    msg->inSampleRate = sampleRate;
    msg->outSampleRate = msg->inSampleRate;
    msg->channel = channel;
    msg->bytesPerSample = bytesPerSample;
    return msg;
}

void MsgCreator::destroyMsg(DataMsg *msg) {
    if (msg != nullptr) {
////        printf("delete msg [%llu]\n", msg->getId());
        free(msg->micBuff);
        free(msg->refBuff);
//        free(msg->proMicBuff);
//        free(msg->proRefBuff);
        delete msg;
        msg = nullptr;
    }
}

MsgCreator::MsgCreator() {

}