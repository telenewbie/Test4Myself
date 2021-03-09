//
// Created by Elevoc on 2021/1/4 0004.
//

#ifndef TEST_MYMICBUFFER_H
#define TEST_MYMICBUFFER_H

#include <mutex>
#include <condition_variable>
#include "EVByteQueue.h"

class MyMicBuffer {
    MyMicBuffer();

    ~MyMicBuffer();

public:
    static MyMicBuffer *getInstance();

    int write(char *data, int len);

    int read(char *data, int len);

//    int write(short *data, int sampleLen);
//    int write(float *data, int sampleLen);
//
//    int read(short *data, int sampleLen);
//    int read(float *data, int sampleLen);

private:
    //char mBuffer[1024];
    EVByteQueue mInputBuffer;

//    int mReadIndex = 0;
//    int mWriteIndex = 0;
//
//    std::mutex mMutex;
//    std::condition_variable mCond;
};


#endif //TEST_MYMICBUFFER_H
