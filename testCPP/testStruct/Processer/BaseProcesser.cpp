//
// Created by Elevoc on 2021/1/4 0004.
//

#include "BaseProcesser.h"
#include "../EventCenter.h"
#include <ElevocLog.h>

void BaseProcesser::onNext(DataMsg *msg) {
    // 打包
    if (msg != nullptr) {
        ++msg->index;
        // 发包吧！！！
        EventCenter::getInstance()->send(msg);
    }
}

void BaseProcesser::start() {
    if (mNeedNewThread) {
        mProcessThread = std::thread(&BaseProcesser::innerProcess, this);
        mProcessThread.detach();
    }
}

void BaseProcesser::innerProcess() {
    std::unique_lock<std::mutex> lk(mMutex);
    while (!mIsRelease) {
        mWaitCond.wait(lk, [&]() -> bool { return mIsRelease || !mVecMsg.empty(); });

        if (!mIsRelease && mVecMsg.size() > 0) {
            if (mVecMsg.size() >= MAX_BUFFER_LENGTH) {
                printf("processing is so fast , capacity is too small!!![%zu/%d]\n", mVecMsg.size(), MAX_BUFFER_LENGTH);
            }
            DataMsg *curMsg = mVecMsg.front();
            mVecMsg.pop();
            dispose(curMsg);
        } else {
            LOGD("%s get nothing,maybe release![%s]", getTag().data(), mIsRelease ? " is release" : " is empty");
        }
    }
}

void BaseProcesser::release() {
    mIsRelease = true;
}

void BaseProcesser::notify(DataMsg *msg) {
    LOGD("%s notify:%p[id=%d,index=%d,sampleRate:%d,sample_num:%d]", getTag().data(), msg, msg->getId(), msg->index,
         msg->outSampleRate, msg->sample_num);
    if (mNeedNewThread) {
        mVecMsg.push(msg);
        mWaitCond.notify_one();
    } else {
        dispose(msg);
    }
}

bool BaseProcesser::canProcess(DataMsg *) {
    return true;
}

bool BaseProcesser::needTransport() {
    return true;
}


void BaseProcesser::dispose(DataMsg *msg) {
    if (!msg) {
        LOGE("%s,cant dispose this", getTag().data());
        return;
    }
    LOGD("%s dispose:%p[id=%d,index=%d,sampleRate:%d,sample_num:%d]", getTag().data(), msg, msg->getId(), msg->index,
         msg->outSampleRate, msg->sample_num);
    if (canProcess(msg)) {
        process(msg);
    }
    onNext(msg);
}

BaseProcesser::BaseProcesser(const ProcessorConfig *cfg) {
    mNeedNewThread = cfg->mNeedThread;
}

BaseProcesser::BaseProcesser(const BaseProcesser &processer_) {
    mNeedNewThread = processer_.mNeedNewThread;
}

BaseProcesser::~BaseProcesser() {
    release();
}

BaseProcesser &BaseProcesser::operator=(const BaseProcesser &cfg) {
    mNeedNewThread = cfg.mNeedNewThread;
    return *this;
}


