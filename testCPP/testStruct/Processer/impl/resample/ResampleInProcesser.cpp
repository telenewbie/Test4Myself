//
// Created by Elevoc on 2021/1/7 0007.
//

#include <Processer/ProcesserPori.h>
#include <TEST_CONSTANT.h>
#include "ResampleInProcesser.h"

static FILE *fpMic = nullptr;
static FILE *fpRef = nullptr;


using RESAMPLE_TYPE = SpeexResamplerAdapter;

int ResampleInProcesser::getMsgIndex() {
    return PORI_RESAMPLE_IN;
}


void ResampleInProcesser::process(DataMsg *msg) {

    if (resampleFar->ptr == nullptr) {
        resampleFar->initSample(msg->channel, msg->inSampleRate, msg->outSampleRate);
    }
    if (resampleNear->ptr == nullptr) {
        resampleNear->initSample(msg->channel, msg->inSampleRate, msg->outSampleRate);
    }

    resampleNear->resampler_process(msg->micBuff, msg->sample_num, msg->micBuff, msg->sample_num);

    resampleFar->resampler_process(msg->refBuff, msg->sample_num, msg->refBuff, msg->sample_num);

    // 输出
    std::fwrite(msg->micBuff, sizeof(TYPE_SAMPLE_t), msg->sample_num, fpMic);
    std::fwrite(msg->refBuff, sizeof(TYPE_SAMPLE_t), msg->sample_num, fpRef);
}

bool ResampleInProcesser::canProcess(DataMsg *msg) {
    return msg->inSampleRate == SAMPLE_RATE_441k || msg->inSampleRate == SAMPLE_RATE_32k ||
           msg->inSampleRate == SAMPLE_RATE_48k;
}

ResampleInProcesser::ResampleInProcesser() {
    fpMic = std::fopen(TEST_PCM_DIR "mic_resample_in.pcm", "wb+");
    fpRef = std::fopen(TEST_PCM_DIR "ref_resample_in.pcm", "wb+");
    if (resampleFar == nullptr) {
        resampleFar = new RESAMPLE_TYPE();
    }
    if (resampleNear == nullptr) {
        resampleNear = new RESAMPLE_TYPE();
    }
}

ResampleInProcesser::~ResampleInProcesser() {
    if (resampleFar) {
        resampleFar->release();
        delete (RESAMPLE_TYPE *) resampleFar;
    }
    if (resampleNear) {
        resampleNear->release();
        delete (RESAMPLE_TYPE *) resampleNear;
    }
    std::fclose(fpRef);
    std::fclose(fpMic);
}

void ResampleInProcesser::initResample(SpeexResamplerState **resamplePtr, DataMsg *mMsg) {
    if (!resamplePtr) return;
    if (*resamplePtr == nullptr) {
        int err = 0;
        int outSampleRate = SAMPLE_RATE_16k;
        *resamplePtr = elevoc_resampler_init(mMsg->channel, mMsg->inSampleRate, outSampleRate, 9, &err);
        if (*resamplePtr != nullptr) {
            elevoc_resampler_set_rate(*resamplePtr, mMsg->inSampleRate, outSampleRate);
        } else {
            return;
        }
    }
}

std::string ResampleInProcesser::getTag() {
    return "resample in";
}
