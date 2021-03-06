//
// Created by Elevoc on 2021/1/5 0005.
//

#include "FinalProcesser.h"
#include "TEST_CONSTANT.h"
#include "ElevocLog.h"

static int readSum = 0;

void FinalProcesser::process(DataMsg* msg) {
// 写文件
    readSum += msg->sample_num;
    printf("read size:%d\n", readSum);
    std::fwrite(msg->micBuff, sizeof(short), msg->sample_num, fpw);
}

FinalProcesser::FinalProcesser(ProcessorConfig* cfg):BaseProcesser(cfg) {
    fpw = std::fopen(TEST_PCM_OUT_PATH, "wb+");
}

FinalProcesser::~FinalProcesser() {
    std::fclose(fpw);
}

bool FinalProcesser::canProcess(DataMsg *msg) {
    return BaseProcesser::canProcess(msg);
}


