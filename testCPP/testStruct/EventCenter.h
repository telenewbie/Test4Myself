//
// Created by Elevoc on 2021/1/5 0005.
//

#ifndef TEST_EVENTCENTER_H
#define TEST_EVENTCENTER_H


#include "dao/DataMsg.h"
#include "Processer/BaseProcesser.h"
#include <vector>
#include <map>
typedef std::vector<BaseProcesser *> Processers;



class EventCenter {
private:
    EventCenter();

public:
    static EventCenter *getInstance();

    void send(DataMsg*);

    void addRegister(BaseProcesser *processer);

    void unRegister(BaseProcesser *processer);

private:

    Processers mProcessers;
    std::map<uint32_t,Processers > mQueueProcess;
};


#endif //TEST_EVENTCENTER_H
