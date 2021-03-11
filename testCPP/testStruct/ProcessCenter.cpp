//
// Created by Elevoc on 2021/1/4 0004.
//

#include <Buffer/MyRefBuffer.h>
#include <utils/EVPCMHelper.h>
#include <config/ElevocLog.h>
#include "ProcessCenter.h"
#include "MyMicBuffer.h"
#include "EventCenter.h"
#include "dao/MsgCreator.h"
#include "TEST_CONSTANT.h"

static FILE *fpRef = nullptr;

void ProcessCenter::start() {
    // 开始处理数据
//
//    size_t readSampleNum = FRAME_SIZE_ONE; // fix
//    //等待数据到来
//    size_t size = readSampleNum * sizeof(TYPE_SAMPLE_t);

    while (!mIsRelease) {
        // 获取一帧数据

        DataMsg *msg = MsgCreator::getInstance()->create();
        LOGD("ProcessCenter msg:[%p]%d:%d:%lld", msg, msg->inSampleRate, msg->outSampleRate, sizeof(TYPE_SAMPLE_t));
        int readLen = MyMicBuffer::getInstance()->read((char *) msg->micBuff, READ_MIC_SIZE);
        MyRefBuffer::getInstance()->read((char *) msg->refBuff, READ_REF_SIZE);

        LOGD("ProcessCenter msg:[%p]%d:%d:%lld", msg, msg->inSampleRate, msg->outSampleRate, sizeof(TYPE_SAMPLE_t));
        LOGD("[%s]read size:%d", __FILE__, readLen);

        EventCenter::getInstance()->send(msg);
    }
}

void ProcessCenter::stop() {

}

void ProcessCenter::release() {
    mIsRelease = true;
}

ProcessCenter::ProcessCenter() {
    fpRef = std::fopen(TEST_PCM_DIR "ref_center.pcm", "wb+");
}

ProcessCenter *ProcessCenter::getInstance() {
    static auto *instance = new ProcessCenter();
    return instance;
}

ProcessCenter::~ProcessCenter() {
    std::fclose(fpRef);
}

void ProcessCenter::init(int sampleRate, int channel, int bytePerSample) {
    LOGD("ProcessCenter init:%d,%d,%d", sampleRate, channel, bytePerSample);
//    mSampleRate = sampleRate;
//    mChannel = channel;
//    mBytePerSample = bytePerSample;
}
