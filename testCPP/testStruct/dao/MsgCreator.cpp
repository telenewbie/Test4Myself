//
// Created by Elevoc on 2021/1/5 0005.
//

#include <cstdio>
#include <cstdlib>
#include "MsgCreator.h"
#include <algorithm>
#include "ElevocLog.h"

MsgCreator *MsgCreator::getInstance() {
    static auto *instance = new MsgCreator();
    return instance;
}


int getNextBufferIndex() {
    static int index = 0;
    if (index >= MAX_BUFFER_LENGTH) {
        index = 0;
        return index;
    } else {
        return index++;
    }
}


DataMsg *MsgCreator::create() {

//    memset(reserveMic,0,sizeof(FRAME_SIZE_ONE * sizeof(TYPE_SAMPLE_t) * MAX_BUFFER_LENGTH * CHANNEL_MIC));
//    memset(reserveRef,0,sizeof(FRAME_SIZE_ONE * sizeof(TYPE_SAMPLE_t) * MAX_BUFFER_LENGTH * CHANNEL_MIC));
//    memset(reserveDataMsg,0,sizeof(sizeof(DataMsg) * MAX_BUFFER_LENGTH));
    int index = getNextBufferIndex();
    auto *msg = &reserveDataMsg[index];

    // 停住
    if (msg->index != 0) {
        mWaitCond.wait(lk, [&]() { return mIsRelease || msg->index == 0; });
    }

    if (mIsRelease) {
        LOGD("create Msg error , is release now!");
        return nullptr;
    }
    msg->micBuff = (TYPE_SAMPLE_t *) &reserveMic[FRAME_SIZE_ONE * sizeof(TYPE_SAMPLE_t) * CHANNEL_MIC * index]; //
    msg->refBuff = (TYPE_SAMPLE_t *) &reserveRef[FRAME_SIZE_ONE * sizeof(TYPE_SAMPLE_t) * CHANNEL_REF * index];//

    msg->init();
    msg->index = 0;
    msg->sample_num = FRAME_SIZE_ONE;
    msg->mic_buff_size = FRAME_SIZE_ONE;
    msg->inSampleRate = TEST_SAMPLE_RATE;
    msg->outSampleRate = RESAMPLE_OUT_RATE;
    msg->refChannel = CHANNEL_REF;
    msg->micChannel = CHANNEL_MIC;
    msg->bytesPerSample = sizeof(TYPE_SAMPLE_t);
    LOGD("create msg[%p],sample_num:%d", msg, msg->sample_num);
    return msg;
}

void MsgCreator::destroyMsg(DataMsg *msg) {
    if (msg != nullptr) {
        msg->index = 0;
        mWaitCond.notify_one();
    }
}

MsgCreator::MsgCreator() {
    reserveMic = (char *) malloc(FRAME_SIZE_ONE * sizeof(TYPE_SAMPLE_t) * MAX_BUFFER_LENGTH * CHANNEL_MIC);
    reserveRef = (char *) malloc(FRAME_SIZE_ONE * sizeof(TYPE_SAMPLE_t) * MAX_BUFFER_LENGTH * CHANNEL_REF);
    reserveDataMsg = (DataMsg *) malloc(sizeof(DataMsg) * MAX_BUFFER_LENGTH);
    memset(reserveDataMsg, 0, sizeof(DataMsg) * MAX_BUFFER_LENGTH);
    lk = std::unique_lock<std::mutex>(mMutex);
}

MsgCreator::~MsgCreator() {
    mIsRelease = true;
    mWaitCond.notify_all();
    free(reserveMic);
    free(reserveRef);
    free(reserveDataMsg);
}
