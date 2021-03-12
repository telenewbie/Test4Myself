//
// Created by Elevoc on 2021/1/8 0008.
//

#include <Processer/ProcesserPori.h>
#include <TEST_CONSTANT.h>
#include <utils/EVPCMHelper.h>
#include "AECProcesser.h"

#include "WebrtcAec.h"

using namespace webrtc;

int AECProcesser::getMsgIndex() {
    return PORI_AEC;
}

void AECProcesser::process(DataMsg *msg) {

    static_assert(CHANNEL_MIC == CHANNEL_REF, "mic ");
    if (!mAecRunning) {
        mAecRunning = mAECAdapter->start();
    }

    int aec_sample_num = msg->sample_num;
    for (int i = 0; i < CHANNEL_MIC; ++i) {
        // 远端数据
        mAECAdapter->writeFarFrame(&msg->refBuff[msg->mic_buff_size * i], aec_sample_num);
        dumpFile->writeRef(i, &msg->refBuff[msg->mic_buff_size * i], sizeof(TYPE_SAMPLE_t), aec_sample_num);
        int out_len = 0, linear_out_len = 0;
        // 近端数据
        mAECAdapter->writeNearFrame(&msg->micBuff[msg->mic_buff_size * i], aec_sample_num,
                                    &msg->micBuff[msg->mic_buff_size * i], out_len,
                                    &linear_out_buffer[msg->mic_buff_size * i], linear_out_len);
#ifdef DEBUG_FILE
        dumpFile->writeMic(i, &msg->micBuff[msg->mic_buff_size * i], sizeof(TYPE_SAMPLE_t), out_len);
//        dumpFile->writeRef(i, &linear_out_buffer[msg->mic_buff_size * i], sizeof(TYPE_SAMPLE_t), linear_out_len);
#endif
    }
}

AECProcesser::AECProcesser(const ProcessorConfig *cfg) : BaseProcesser(cfg) {
    AECConfig config;
    config.enableAGC = false;
    config.sampleRate = cfg->mOutSampleRate;
    mAECAdapter = new AEC::WebrtcAec(std::move(config));
#ifdef DEBUG_FILE
    dumpFile = new DumpFileUtil(cfg, "aec_");
#endif
}

AECProcesser::~AECProcesser() {
    mAECAdapter->release();
    delete mAECAdapter;
#ifdef DEBUG_FILE
    delete dumpFile;
#endif

}

std::string AECProcesser::getTag() {
    return "AECProcesser";
}

