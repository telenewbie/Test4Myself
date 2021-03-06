#include <iostream>

#include "ProcessCenter.h"
#include "MyMicBuffer.h"
#include "EventCenter.h"

#include <thread>
#include <EVByteQueue.h>
#include <impl/FinalProcesser.h>

#ifndef USE_ENGINE

#include <Processer/impl/dnn/DnnProcesser.h>
#include <impl/resample/ResampleInProcesser.h>
#include <Processer/impl/resample/ResampleOutProcesser.h>
#include <Processer/impl/aec/AECProcesser.h>
#include <Buffer/MyRefBuffer.h>
#include <Processer/HelloProcesser.h>
#include <dao/MsgCreator.h>

#include "impl/slice/FrameSliceProcessor.h"
#include "impl/slice/FrameSyntheticProcessor.h"

#else

#include <impl/EngineProcesser.h>

#endif

#define SLEEP_TIME_MILLISECOND 0
#define MAIN_SLEEP_TIME 15


FILE *log_fp;

void readRefFile() {
// 读文件
    FILE *fp = std::fopen(TEST_REF_PCM_PATH, "rb");
    if (fp == nullptr) {
        LOGD("open ref file error!!!");
        return;
    }
    char *buf = (char *) malloc(READ_REF_SIZE);
    memset(buf, 0, READ_REF_SIZE);
    size_t count = 0;
    while (!std::feof(fp)) {
        size_t readSize = std::fread(buf, 1, READ_REF_SIZE, fp);
        LOGD("ref read size:%zu", readSize);
        MyRefBuffer::getInstance()->write(buf, readSize);
        count += readSize;
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_MILLISECOND));
    }
    std::fclose(fp);
    LOGD("ref write count :%zu", count);
}

void readFile() {
// 读文件
    FILE *fp = std::fopen(TEST_PCM_PATH, "rb");
    if (fp == nullptr) {
        LOGD("open mic file error!!!");
        return;
    }
    char *buf = (char *) malloc(READ_MIC_SIZE);
    size_t count = 0;
    while (!std::feof(fp)) {
        memset(buf, 0, READ_MIC_SIZE);
        size_t readSize = std::fread(buf, 1, READ_MIC_SIZE, fp);
        LOGD("mic read size:%zu", readSize);
        MyMicBuffer::getInstance()->write(buf, readSize);
        count += readSize;
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_MILLISECOND));
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::fclose(fp);
    LOGD("mic write count :%zu", count);
}

void process() {
    ProcessCenter::getInstance()->init(TEST_SAMPLE_RATE, CHANNEL_REF + CHANNEL_MIC, sizeof(TYPE_SAMPLE_t));
    ProcessCenter::getInstance()->start();
}

int main() {
#ifdef LOG_RELEASE
    log_fp = (FILE *) fopen("D:/tmp/log.txt", "w+");
#endif

    std::cout << "hello" << std::endl;
//    EventCenter::getInstance()->addRegister(new Hello2Processer());
//    EventCenter::getInstance()->addRegister(new HelloProcesser());
//    EventCenter::getInstance()->addRegister(new WorldProcesser());
#ifndef USE_ENGINE
    ProcessorConfig config;
    config.mRefChannel = CHANNEL_REF;
    config.mMicChannel = CHANNEL_MIC;
    config.mInSampleRate = TEST_SAMPLE_RATE;
    config.mOutSampleRate = RESAMPLE_OUT_RATE;
    config.mNeedThread = true;

    EventCenter::getInstance()->addRegister(new FrameSliceProcessor(&config));
    EventCenter::getInstance()->addRegister(new ResampleInProcesser(&config));
    EventCenter::getInstance()->addRegister(new AECProcesser(&config));
//    EventCenter::getInstance()->addRegister(new DnnProcesser());
//    EventCenter::getInstance()->addRegister(new DnnProcesser());//agc
//    EventCenter::getInstance()->addRegister(new DnnProcesser());//vad
//    EventCenter::getInstance()->addRegister(new DnnProcesser());//bf
    EventCenter::getInstance()->addRegister(new ResampleOutProcesser(&config));
    EventCenter::getInstance()->addRegister(new FrameSyntheticProcessor(&config));
#else
    EventCenter::getInstance()->addRegister(new EngineProcesser());
#endif
//    EventCenter::getInstance()->addRegister(new FinalProcesser());

    std::thread treadMic = std::thread(&readFile);
    treadMic.detach();
    std::thread treadRef = std::thread(&readRefFile);
    treadRef.detach();
#if 1
    std::thread name = std::thread(&process);
    name.detach();
#else
    for(int  i = 0; i<1000;++i){
        DataMsg *msg = MsgCreator::getInstance()->create(480*2, 48000, 2, 2);
        MyMicBuffer::getInstance()->read((char*)msg->micBuff, 960);
        MyRefBuffer::getInstance()->read((char*)msg->refBuff, 960);
        MsgCreator::getInstance()->destroyMsg(msg);
    }
#endif
    std::cout << "over" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(MAIN_SLEEP_TIME));


#ifdef LOG_RELEASE
    fclose(log_fp);
#endif
    return 0;
}