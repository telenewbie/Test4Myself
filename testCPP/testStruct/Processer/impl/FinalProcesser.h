//
// Created by Elevoc on 2021/1/5 0005.
//

#ifndef TEST_FINALPROCESSER_H
#define TEST_FINALPROCESSER_H


#include <BaseProcesser.h>

class FinalProcesser : public BaseProcesser {


    int getMsgIndex() override;

    void process(DataMsg*) override;
    bool canProcess(DataMsg *) override;

public:
    FinalProcesser(ProcessorConfig*);
    ~FinalProcesser();

private:
    FILE *fpw = nullptr;
};


#endif //TEST_FINALPROCESSER_H
