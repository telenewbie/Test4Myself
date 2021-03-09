//
// Created by Elevoc on 2021/1/6 0006.
//

#ifndef TEST_ENGINEPROCESSER_H
#define TEST_ENGINEPROCESSER_H


#include <BaseProcesser.h>

class EngineProcesser : public BaseProcesser {

    int getMsgIndex() override;

    void process() override;

public:
    EngineProcesser();
private:
    void *evtask = nullptr;
};


#endif //TEST_ENGINEPROCESSER_H
