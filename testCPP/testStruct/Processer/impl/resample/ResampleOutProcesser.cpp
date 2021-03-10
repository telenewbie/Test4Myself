//
// Created by Elevoc on 2021/1/7 0007.
//

#include <Processer/ProcesserPori.h>
#include "ResampleOutProcesser.h"
#include "TEST_CONSTANT.h"
#include "SpeexResamplerAdapter.h"

using RESAMPLE_TYPE = SpeexResamplerAdapter;
static FILE *fpMic = nullptr;

int ResampleOutProcesser::getMsgIndex() {
    return PORI_RESAMPLE_OUT;
}

void ResampleOutProcesser::process(DataMsg *msg) {

    if (!resampleAdapter->ptr) {
        resampleAdapter->initSample(msg->channel, msg->outSampleRate, msg->inSampleRate);
    }

    uint32_t out_len = msg->buff_size / msg->bytesPerSample;
    resampleAdapter->resampler_process(msg->micBuff, msg->sample_num, msg->micBuff, out_len);

    msg->sample_num = out_len;

    std::fwrite(msg->micBuff, sizeof(TYPE_SAMPLE_t), msg->sample_num, fpMic);
}

bool ResampleOutProcesser::canProcess(DataMsg *msg) {
    return msg->inSampleRate != SAMPLE_RATE_16k && msg->outSampleRate == SAMPLE_RATE_16k;
}

ResampleOutProcesser::ResampleOutProcesser() {
    resampleAdapter = new RESAMPLE_TYPE;
    fpMic = std::fopen(TEST_PCM_DIR "mic_resample_out.pcm", "wb+");
}

std::string ResampleOutProcesser::getTag() {
    return "ResampleOutProcessor";
}

ResampleOutProcesser::~ResampleOutProcesser() {
    if (resampleAdapter) {
        resampleAdapter->release();
        delete (RESAMPLE_TYPE *) resampleAdapter;
    }
}
