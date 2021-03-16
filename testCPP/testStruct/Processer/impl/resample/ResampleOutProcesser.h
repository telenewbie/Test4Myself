//
// Created by Elevoc on 2021/1/7 0007.
//

#ifndef TEST_RESAMPLEOUTPROCESSER_H
#define TEST_RESAMPLEOUTPROCESSER_H


#include <Processer/BaseProcesser.h>
#include "BaseResample.h"
#include "BaseResampleAdapter.h"
#include "DumpFileUtil.h"

class ResampleOutProcesser : public BaseProcesser {

    void process(DataMsg *) override;

    bool canProcess(DataMsg *) override;

    BaseResampleAdapter *resampleNears[MAX_CHANNEL] = {0};
public:
    virtual std::string getTag() override;

    ResampleOutProcesser(ProcessorConfig *);

    ~ResampleOutProcesser();

private:
#ifdef DEBUG_FILE
    DumpFileUtil *dumpFile = nullptr;
#endif
};


#endif //TEST_RESAMPLEOUTPROCESSER_H
