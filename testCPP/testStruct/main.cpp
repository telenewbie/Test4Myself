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

#else

#include <impl/EngineProcesser.h>

#endif

#define SLEEP_TIME_MILLISECOND 2
#define MAIN_SLEEP_TIME 30
#include "TEST_CONSTANT.h"
FILE* log_fp;
void readRefFile() {
// 读文件
    FILE *fp = std::fopen(TEST_REF_PCM_PATH, "rb");
    if (fp == nullptr) {
        LOGD("open ref file error!!!");
        return;
    }
    char *buf = (char *) malloc(READ_SIZE);
    memset(buf, 0, READ_SIZE);
    size_t count = 0;
    while (!std::feof(fp)) {
        size_t readSize = std::fread(buf, 1,  READ_SIZE, fp);
        LOGD("ref read size:%zu",readSize);
        MyRefBuffer::getInstance()->write(buf, readSize);
        count += readSize;
        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_TIME_MILLISECOND));
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
    char *buf = (char *) malloc(READ_SIZE);
    size_t count = 0;
    while (!std::feof(fp)) {
        memset(buf, 0, READ_SIZE);
        size_t readSize = std::fread(buf, 1, READ_SIZE, fp);
        LOGD("mic read size:%zu",readSize);
        MyMicBuffer::getInstance()->write(buf, readSize);
        count += readSize;
        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_TIME_MILLISECOND));
    }
    std::fclose(fp);
    LOGD("mic write count :%zu", count);
}

void process() {
    ProcessCenter::getInstance()->init(TEST_SAMPLE_RATE, TEST_CHANNEL, sizeof(TYPE_SAMPLE_t));
    ProcessCenter::getInstance()->start();
}

int main() {
#ifdef LOG_RELEASE
    log_fp=(FILE*) fopen("D:/tmp/log.txt","w+");
#endif

    std::cout << "hello" << std::endl;
//    EventCenter::getInstance()->addRegister(new Hello2Processer());
//    EventCenter::getInstance()->addRegister(new HelloProcesser());
//    EventCenter::getInstance()->addRegister(new WorldProcesser());
#ifndef USE_ENGINE
    EventCenter::getInstance()->addRegister(new ResampleInProcesser());
//    EventCenter::getInstance()->addRegister(new AECProcesser());
//    EventCenter::getInstance()->addRegister(new DnnProcesser());
    EventCenter::getInstance()->addRegister(new ResampleOutProcesser());
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
    std::cout<<"over"<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(MAIN_SLEEP_TIME));


#ifdef LOG_RELEASE
    fclose(log_fp);
#endif
    return 0;
}