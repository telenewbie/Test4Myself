//
// Created by Elevoc on 2021/1/4 0004.
//

#ifndef TEST_BASEPROCESSER_H
#define TEST_BASEPROCESSER_H

#include "../dao/DataMsg.h"
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <queue>
#include "ProcessorConfig.h"

class BaseProcesser {


    void onNext(DataMsg *);


    virtual void process(DataMsg *) = 0;


public:
    BaseProcesser(const ProcessorConfig* cfg);
    BaseProcesser(const BaseProcesser&);
    BaseProcesser& operator=(const BaseProcesser&);
    virtual ~BaseProcesser();
    virtual std::string getTag() = 0;
    void notify(DataMsg *);

//    virtual int getMsgIndex() = 0;


    virtual bool canProcess(DataMsg *);

    void start();

    void release();

private:
    void innerProcess();
    void dispose(DataMsg *msg);

protected:
    virtual bool needTransport();
    std::queue<DataMsg *> mVecMsg;
    std::vector<int> a;

    bool mNeedNewThread = false; // use new thread do things?

private:
    std::thread mProcessThread;

    std::mutex mMutex;
    std::condition_variable mWaitCond;
    bool mIsRelease = false;
};


#endif //TEST_BASEPROCESSER_H
