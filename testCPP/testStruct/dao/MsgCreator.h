//
// Created by Elevoc on 2021/1/5 0005.
//

#ifndef TEST_MSGCREATOR_H
#define TEST_MSGCREATOR_H


#include <mutex>
#include "DataMsg.h"

class MsgCreator {

private:
    MsgCreator();
    ~MsgCreator();

public:
    static MsgCreator *getInstance();

    DataMsg *create();

    void destroyMsg(DataMsg *);

private:
    std::mutex mMutex;
    std::condition_variable mWaitCond;

    char *reserveMic ;
    char *reserveRef ;
    DataMsg *reserveDataMsg;
    std::unique_lock<std::mutex> lk;

    bool mIsRelease = false;
};


#endif //TEST_MSGCREATOR_H
