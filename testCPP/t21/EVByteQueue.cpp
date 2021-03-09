//
//  EVByteQueue.cpp
//  SingleChannelEngine
//
//  Created by csyyj on 2017/8/2.
//  Copyright © 2017年 elevoc. All rights reserved.
//

#include "EVByteQueue.h"
#include <stdlib.h>
#include <string.h>
//#include "ElevocLog.h"
//#include "EVAssert.h"
#include "my_log.h"

void ele_assert(int condition, const char *desc) {
    if (condition != 1) {
        LOGD("%s",desc);
    }
}


EVByteQueue::EVByteQueue() {
    front = rear = 0;
    sumReadLen = 0;
    isWriteStop = false;
    max = QUEUE_SIZE;
    memset(queue, 0, sizeof(char) * QUEUE_SIZE);
    isReleased = false;
}

EVByteQueue::~EVByteQueue() {

}

WritingResult EVByteQueue::writeAudio(const char *audioData, size_t len) {
    if (audioData == NULL || len <= 0) {
        LOGD("writeAudio:audioData is null or len is 0!");
        return ParameErr;
    }
    if(isReleased){
        LOGD("writeAudio:already released!");
        return WriteReleased;
    }
    if (isWriteStop) {
        ele_assert(false, "writeAudio: queue has been written stop flag, cannot be writtten again.");
        LOGE("writeAudio: queue has been written stop flag, cannot be writtten again.");
        return WriteStop;
    }
    
    std::unique_lock<std::mutex> lk(mutex);
    while (leftLen() < len && !isReleased) {
    	writeCond.wait(lk);
    }
    if (isReleased) {
        return WriteReleased;
    }

    char* begin = &(queue[rear]);      
    if (rear + len <= max - 1) {
        memcpy(begin, audioData, len);
    } else { 
        size_t dataLen1 = max - 1 - rear + 1;
        size_t dataLen2 = len - dataLen1;

        memcpy(begin, audioData, dataLen1);
        memcpy(queue, audioData + dataLen1, dataLen2);
    }
    
    rear = (rear + len) % max;
    readCond.notify_one();
    return WriteSuccess;
}


ReadingResult EVByteQueue::readAudio(char *outAudio, size_t* readLen) {
    ReadingResult result = NotLast;
    if (outAudio == NULL || *readLen <= 0) {
        return NotLast;
    }
    if(isReleased){
        LOGD("readAudio:already released!");
        return Released;
    }
    /*
	  *  1，等待条件有三个，三个条件有一个为真则不需要等待
	  *
	  *	 isWriteStop 为false
	  *  缓冲区剩下的数据不足readLen，需等待去写入数据.
	  *  isReleased 为false
	  *
	  *	 2, 当用户调用了cleanQueue()后，则两个等待条件一直为false，分别是isWriteStop和availableLen() < *readLen
	  *
	  *  3, 导致阻塞的原因就是：wait先进行了条件isReleased判断，然后再进行其它两个条件判断时，另一处更新isRelease的代码，
	  		由于没有加锁，开始执行，浪费了一次notify，之后此处的wait检测剩余两个条件也不满足，将完全处于wait状态.这个时候
	  		再无更新条件变量的时机了。
	  */
    std::unique_lock<std::mutex> lk(mutex);
    while (isWriteStop == false && availableLen() < *readLen && isReleased == false) {
    	readCond.wait(lk);
    }

    if (isReleased) {
        return Released;
    }
    
    
    char* queueBase = queue;                
    char* begin = &((queueBase)[front]);   
    if (isWriteStop && *readLen > availableLen()) { 
        LOGI("the last read...");
        result = IsLast;                   
        *readLen = availableLen();         
        isWriteStop = false;
    }
    
    if (front + *readLen <= max - 1) {
        memcpy(outAudio, begin, *readLen);
    } else {
        
        size_t readLen1 = max - 1 - front + 1;
        size_t readLen2 = *readLen - readLen1;
        
        memcpy(outAudio, begin, readLen1);
        memcpy(&outAudio[readLen1], queueBase, readLen2);
    }
    
    front = (front + *readLen) % max;
    sumReadLen += *readLen;
    writeCond.notify_one();
    return result;
}

bool EVByteQueue::readAudioWithoutWait(char *outAudio, size_t* readLen) {
    bool isLast = false;
    if (outAudio == NULL || *readLen <= 0) {
        return isLast;
    }
    std::unique_lock<std::mutex> lk(mutex);
    
    
    char* queueBase = queue;
    char* begin = &((queueBase)[front]);
    size_t queueLen = availableLen();
    if (isWriteStop && *readLen >= availableLen()) { 
        isLast = true;
        *readLen = availableLen();
    }
    
    if (queueLen < *readLen) {
        if (queueLen > 0) {
            *readLen = queueLen;
        } else {
            *readLen = 0;
            return isLast;
        }
    }
    // front < rear
    if (front + *readLen <= max - 1) {
        memcpy(outAudio, begin, *readLen);
    } else {
        
        size_t readLen1 = max - 1 - front + 1;
        size_t readLen2 = *readLen - readLen1;
        
        memcpy(outAudio, begin, readLen1);
        
        memcpy(&outAudio[readLen1], queueBase, readLen2);
    }
    front = (front + *readLen) % max;
    writeCond.notify_one();
    return isLast;
}

void EVByteQueue::writeAudioEnd() {
    std::unique_lock<std::mutex> lk(mutex);
    isWriteStop = true;
    readCond.notify_one();
}

void EVByteQueue::cleanQueue() {
    std::unique_lock<std::mutex> lk(mutex);
    rear = front = 0;
    isWriteStop = false;
    sumReadLen = 0;
}


size_t EVByteQueue::availableLen() {
    return (rear - front + max) % max;
}


size_t EVByteQueue::leftLen() {
    size_t left = max - 1 - availableLen();
    return left;
}

void EVByteQueue::setQueName(std::string _queName) {
    this->queName = _queName;
}


void EVByteQueue::release(){
    std::unique_lock<std::mutex> lk(mutex);
    isReleased = true;
    readCond.notify_one();
    writeCond.notify_one();
}
