//
// Created by Elevoc on 2021/1/4 0004.
//

#include <memory.h>
#include "MyMicBuffer.h"
#include "ElevocLog.h"

static size_t testCount = 0;

size_t MyMicBuffer::write(char *data, size_t len) {
#ifndef MINE
    mInputBuffer.writeAudio(data, len);
#else
    memcpy(mBuffer + mWriteIndex, data, len);
    mWriteIndex += len;
    mCond.notify_one();
#endif
    return len;
}

size_t MyMicBuffer::read(char *data, size_t len) {
//    if (mReadIndex + len < mWriteIndex) {
//        // 正常读取
//    } else {
//        // 等待
//        std::unique_lock<std::mutex> lock(mMutex);
//        mCond.wait(lock);
//    }
//
//    memcpy(data, mBuffer + mReadIndex, len);
//    mReadIndex += len;
    size_t readlen = len;
    ReadingResult result = mInputBuffer.readAudio(data, &readlen);
    LOGD("readAudio:%zu,result=%d", readlen,result);
    testCount += readlen;
    return readlen;
}

MyMicBuffer *MyMicBuffer::getInstance() {
    static auto *instance = new MyMicBuffer();
    return instance;
}

MyMicBuffer::MyMicBuffer() {

//    memset(mBuffer, 0, sizeof(mBuffer));

}

MyMicBuffer::~MyMicBuffer() {
    printf("read Size:%zd\n", testCount);
}
