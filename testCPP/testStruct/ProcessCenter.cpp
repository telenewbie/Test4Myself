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

    size_t readSampleNum = mSampleRate / 100; //
    //等待数据到来
    size_t size = readSampleNum * sizeof(TYPE_SAMPLE_t);
    auto *readMicBuffer = (TYPE_SAMPLE_t *) malloc(size);
    auto *readRefBuffer = (TYPE_SAMPLE_t *) malloc(size);
    memset(readMicBuffer, 0, size);
    memset(readRefBuffer, 0, size);

    while (!mIsRelease) {
        // 获取一帧数据

        DataMsg *msg = MsgCreator::getInstance()->create(readSampleNum, mSampleRate, mChannel, mBytePerSample);
        int readLen = MyMicBuffer::getInstance()->read((char*)msg->micBuff, size);
        MyRefBuffer::getInstance()->read((char*)msg->refBuff, size);

        LOGD("[%s]read size:%d",__FILE__,readLen);
        //转float
//        convertFloatToShort()
//        convertByteToFloat(readMicBuffer, readLen, msg->micBuff, msg->sampleLen, DEFAULT);
//        convertByteToFloat(readRefBuffer, readLen, msg->refBuff, msg->sampleLen, DEFAULT);

//        std::fwrite(readMicBuffer, 1, readLen, fpRef);

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
    mSampleRate = sampleRate;
    mChannel = channel;
    mBytePerSample = bytePerSample;
}
