//
// Created by Elevoc on 2021/1/7 0007.
//

#include <Processer/ProcesserPori.h>
#include "ResampleOutProcesser.h"
#include "TEST_CONSTANT.h"
#include "SpeexResamplerAdapter.h"

using RESAMPLE_TYPE = SpeexResamplerAdapter;

int ResampleOutProcesser::getMsgIndex() {
    return PORI_RESAMPLE_OUT;
}

void ResampleOutProcesser::process(DataMsg *msg) {

    // 先处理 mic, 在处理 ref
    uint32_t sampleNum = msg->mic_buff_size;
    for (int i = msg->micChannel - 1; i >= 0; --i) {
        if (resampleNears[i]) {

//            printf("mic before num:%d\n", msg->sample_num);
            resampleNears[i]->resampler_process(i, &msg->micBuff[msg->mic_buff_size * i], msg->sample_num,
                                                &msg->micBuff[FRAME_SIZE_ONE * i], sampleNum);

//            printf("mic after num:%d\n", sampleNum);

#ifdef DEBUG_FILE
            dumpFile->write(DumpFileUtil::OUT_MIC, i, &msg->micBuff[msg->mic_buff_size * i], sizeof(TYPE_SAMPLE_t),
                            sampleNum);
#endif
        }
    }
    msg->sample_num = sampleNum;
}

bool ResampleOutProcesser::canProcess(DataMsg *msg) {
    if (msg->outSampleRate == msg->inSampleRate) return false;
    return msg->inSampleRate != SAMPLE_RATE_16k && msg->outSampleRate == SAMPLE_RATE_16k;
}

ResampleOutProcesser::ResampleOutProcesser(ProcessorConfig *cfg) : BaseProcesser(cfg) {
    if (cfg->mInSampleRate == cfg->mOutSampleRate || cfg->mInSampleRate == 0 || cfg->mOutSampleRate == 0) return;
    for (int i = 0; i < cfg->mMicChannel; ++i) {
        resampleNears[i] = new RESAMPLE_TYPE();
        resampleNears[i]->initSample(cfg->mMicChannel, cfg->mOutSampleRate, cfg->mInSampleRate);
    }
#ifdef DEBUG_FILE
    dumpFile = new DumpFileUtil(cfg, "resample_out_");
#endif
}

std::string ResampleOutProcesser::getTag() {
    return "ResampleOutProcessor";
}

ResampleOutProcesser::~ResampleOutProcesser() {
    for (auto &resampleNear : resampleNears) {
        if (resampleNear) {
            resampleNear->release();
            delete (RESAMPLE_TYPE *) resampleNear;
            resampleNear = nullptr;
        }
    }
#ifdef DEBUG_FILE
    delete dumpFile;
#endif
}
