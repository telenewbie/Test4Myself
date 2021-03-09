//
// Created by Elevoc on 2021/1/5 0005.
//

#ifndef TEST_DNNPROCESSER_H
#define TEST_DNNPROCESSER_H


#include <ElevocFeature.h>
#include <ElevocSynthetic.h>
#include <ElevocDnnNS.h>
#include <Elevoc_NS.h>
#include "BaseProcesser.h"

class DnnProcesser : public BaseProcesser {

    int getMsgIndex() override;

    void process(DataMsg*) override;

public:
    std::string getTag();
    DnnProcesser();
    ~DnnProcesser();
private:
    float input_buffer_f32[160 * 2];
    ElevocFeature feature;
    Synthetic synthetic;
    ElevocDnnNS ns;
//    Elevoc_NS pf_ns;
};


#endif //TEST_DNNPROCESSER_H
