//
// Created by Elevoc on 2021/1/4 0004.
//

#include "BaseProcesser.h"
#include "../EventCenter.h"
#include <ElevocLog.h>

void BaseProcesser::onNext(DataMsg * msg) {
    // 打包
    if (msg != nullptr) {
        ++msg->index;

        // 发包吧！！！
        EventCenter::getInstance()->send(msg);
    }
}

void BaseProcesser::start() {

    mProcessThread = std::thread(&BaseProcesser::innerProcess, this);
    mProcessThread.detach();
}

void BaseProcesser::innerProcess() {
    std::unique_lock<std::mutex> lk(mMutex);
    while (!mIsRelease) {
        mWaitCond.wait(lk, [&]() -> bool { return !mVecMsg.empty(); });
        DataMsg* curMsg = mVecMsg.front();
        LOGD("BaseProcesser obtain:%p[id=%d,index=%d]",curMsg,curMsg->getId(),curMsg->index);
        if (canProcess(curMsg)) {
            process(curMsg);
        }
        onNext(curMsg);
        mVecMsg.pop();
    }
}

void BaseProcesser::release() {
    mIsRelease = true;
}

void BaseProcesser::notify(DataMsg *msg) {
    LOGD("BaseProcesser notify:%p[id=%d,index=%d]",msg,msg->getId(),msg->index);
        mVecMsg.push(msg);
        mWaitCond.notify_one();
//    } else {
//        LOGD("[%p][%s][id=%d,index=%d]not process",msg,getTag().data(),msg->getId(),msg->index);
////        mCurMsg = msg;
//        onNext(msg);
//    }
}

bool BaseProcesser::canProcess(DataMsg *) {
    return true;
}

bool BaseProcesser::needTransport() {
    return true;
}
