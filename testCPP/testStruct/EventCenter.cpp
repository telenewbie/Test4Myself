//
// Created by Elevoc on 2021/1/5 0005.
//

#include <iostream>
#include "EventCenter.h"
#include "dao/MsgCreator.h"
#include <inttypes.h>
#include "ElevocLog.h"

EventCenter::EventCenter() {

}

EventCenter *EventCenter::getInstance() {
    static auto *instance = new EventCenter();
    return instance;
}

void EventCenter::send(DataMsg *msg) {
    if (msg == nullptr) {
        printf("error!!!!!!!!!!!!!!");
        return;
    }
    uint32_t key = msg->getId();
    LOGD("send msg:[msg=%p][id = %d]%d,sampleRate:%d,processor:%llu",msg, key, msg->index,msg->outSampleRate,mProcessers.size());
    if(msg->index>=mProcessers.size()){
        MsgCreator::getInstance()->destroyMsg(msg);
    }else{
        mProcessers[msg->index++]->notify(msg);
    }
#if 0
    if (msg->index == 0) {
        LOGD("map emplace[msg=%p] key:%d",msg,key);
        mQueueProcess.emplace(std::make_pair(key, mProcessers));
    }

    auto it = mQueueProcess.find(key);
    if (it != mQueueProcess.end() && !it->second.empty()) {
        BaseProcesser * processer = it->second.front();
        LOGD("[msg=%p][id=%d][%s]doing",msg,key,processer->getTag().data());
        it->second.erase(it->second.begin());
        processer->notify(msg);
    } else {
        size_t deleteCount = mQueueProcess.erase(key);
        LOGD("queue size:%zu:deleteCount=%zu,msg=%p][key=%d]", mQueueProcess.size(),deleteCount,msg,key);
        // 清除 相应的msg
        MsgCreator::getInstance()->destroyMsg(msg);
    }
#endif
}

void EventCenter::addRegister(BaseProcesser *processer) {
    mProcessers.push_back(processer);
    processer->start();
}

void EventCenter::unRegister(BaseProcesser *processer) {

}
