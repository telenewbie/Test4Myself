//
// Created by Elevoc on 2021/1/7 0007.
//

#include <Processer/ProcesserPori.h>
#include <TEST_CONSTANT.h>
#include <string>
#include "ResampleInProcesser.h"

using RESAMPLE_TYPE = SpeexResamplerAdapter;

int ResampleInProcesser::getMsgIndex() {
    return PORI_RESAMPLE_IN;
}


void ResampleInProcesser::process(DataMsg *msg) {

    // 每个通道都进行处理
    // 校验数据长度是否正确
    checkDataVerify(msg);

    // 先处理 mic, 在处理 ref
    uint32_t sampleNum = msg->sample_num;
    for (int i = 0; i < msg->micChannel; ++i) {
        if (resampleNears[i]) {
            resampleNears[i]->resampler_process(i, &msg->micBuff[msg->sample_num * i], msg->sample_num,
                                                &msg->micBuff[msg->sample_num * i], sampleNum);
#ifdef DEBUG_FILE
            dumpFile->writeMic(i, &msg->micBuff[msg->sample_num * i], sizeof(TYPE_SAMPLE_t), sampleNum);
#endif
        }
    }
    for (int i = 0; i < msg->refChannel; ++i) {
        if (resampleFars[i]) {
            resampleFars[i]->resampler_process(i, &msg->refBuff[msg->sample_num * i], msg->sample_num,
                                               &msg->refBuff[msg->sample_num * i], sampleNum);
#ifdef DEBUG_FILE
            dumpFile->writeRef(i, &msg->refBuff[msg->sample_num * i], sizeof(TYPE_SAMPLE_t), sampleNum);
#endif
        }
    }

    msg->sample_num = sampleNum;
}

bool ResampleInProcesser::canProcess(DataMsg *msg) {
    return msg->inSampleRate == SAMPLE_RATE_441k || msg->inSampleRate == SAMPLE_RATE_32k ||
           msg->inSampleRate == SAMPLE_RATE_48k;
}

ResampleInProcesser::~ResampleInProcesser() {

    for (auto &resampleNear : resampleNears) {
        if (resampleNear) {
            resampleNear->release();
            delete (RESAMPLE_TYPE *) resampleNear;
            resampleNear = nullptr;
        }
    }

    for (auto &resampleFar : resampleFars) {
        if (resampleFar) {
            resampleFar->release();
            delete (RESAMPLE_TYPE *) resampleFar;
            resampleFar = nullptr;
        }
    }
#ifdef DEBUG_FILE
    delete dumpFile;
#endif
}

void ResampleInProcesser::initResample(SpeexResamplerState **, DataMsg *) {

}

std::string ResampleInProcesser::getTag() {
    return "resample in";
}

ResampleInProcesser::ResampleInProcesser(ProcessorConfig *cfg) : BaseProcesser(cfg) {
    if (cfg->mOutSampleRate == 0 || cfg->mInSampleRate == 0) {
        LOGE("input sampleRate error!!!!!");
        return;
    }
    for (int i = 0; i < cfg->mMicChannel; ++i) {
        resampleNears[i] = new RESAMPLE_TYPE();
        resampleNears[i]->initSample(cfg->mMicChannel, cfg->mInSampleRate, cfg->mOutSampleRate);
    }
    for (int i = 0; i < cfg->mRefChannel; ++i) {
        resampleFars[i] = new RESAMPLE_TYPE();
        resampleFars[i]->initSample(cfg->mRefChannel, cfg->mInSampleRate, cfg->mOutSampleRate);
    }
#ifdef DEBUG_FILE
    dumpFile = new DumpFileUtil(cfg, "resample_in_");
#endif
}

bool ResampleInProcesser::checkDataVerify(DataMsg *) {
    // 数据量是否
    //msg->sample_num* msg->channel* msg->bytesPerSample
    return true;
}
