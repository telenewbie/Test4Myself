//
//  EVByteQueue.hpp
//  SingleChannelEngine
//
//  Created by csyyj on 2017/8/2.
//  Copyright © 2017年 elevoc. All rights reserved.
//

#ifndef EVByteQueue_h
#define EVByteQueue_h

#ifdef _MSC_VER
#define HAVE_STRUCT_TIMESPEC
#endif // _MSC_VER

#include "stdio.h"
#include <string>
#include <mutex>
#include <condition_variable>

#define QUEUE_SIZE (320 * 50 + 1)

/**
 管理内存的循环队列,并非管理线程
 
 队列回调满足的长度
 
 @return 长度
 */
typedef size_t(*fullBufferSize)();


typedef void(*onGetBufferCallback)(char *audioData, size_t len);

typedef enum {
	ParameErr,
	WriteReleased,
	WriteStop,
	WriteSuccess
}WritingResult;

typedef enum {
    NotLast,
    IsLast,
    Released
} ReadingResult;

class EVByteQueue
{
public:

    EVByteQueue();
    
    ~EVByteQueue();
    
    size_t availableLen();
    
    /**
     线程安全：写入队列数据
     
     @param audioData 写入的数据
     @return 是否写入成功，YES-写入成功 NO-队列已满，无法写入时返回NO
     */
	WritingResult writeAudio(const char *audioData, size_t len);

    /**
     线程安全：读取len长度的数据，不足len时输出队列中全部剩余数据
     @param outAudio 读到的数据
     @param readLen 数据长度
     @return 读到的真正长度
     */
    ReadingResult readAudio(char *outAudio, size_t* readLen);

    bool readAudioWithoutWait(char *outAudio, size_t* readLen);

    void writeAudioEnd();

    /**
     重置队列，头尾指针归零
     */
    void cleanQueue();

    void setQueName(std::string queName);

    void release();
    
private:
    
    mutable std::mutex mutex;
    std::condition_variable readCond;
    std::condition_variable writeCond;

    char queue[QUEUE_SIZE];
    size_t front;
    size_t rear;
    size_t max;
    bool isWriteStop;

    size_t sumReadLen; // for debug

    std::string queName; // que name for debug
    
    size_t leftLen();

    bool isReleased;
};


#endif /* EVByteQueue_hpp */
