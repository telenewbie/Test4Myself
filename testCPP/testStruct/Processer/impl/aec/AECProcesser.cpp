//
// Created by Elevoc on 2021/1/8 0008.
//

#include <Processer/ProcesserPori.h>
#include <TEST_CONSTANT.h>
#include <utils/EVPCMHelper.h>
#include "AECProcesser.h"

using namespace webrtc;

static FILE *fpMic = nullptr;
static FILE *fpOriMic = nullptr;
static FILE *fpOriRef = nullptr;

int AECProcesser::getMsgIndex() {
    return PORI_AEC;
}

void AECProcesser::process(DataMsg *msg) {

    if (!mAecRunning) {
        dspProcess.setSamplerate(msg->outSampleRate);
        mAecRunning = dspProcess.start(true);
    }

    // 远端数据
    int aec_sample_num = msg->sample_num;
    dspProcess.FarFrame(msg->refBuff, aec_sample_num);
    int out_len = 0, linear_out_len = 0;
// 近端数据
    dspProcess.NearFrame(msg->micBuff, aec_sample_num, msg->micBuff, out_len,
                         linear_out_buffer, linear_out_len);


//    printf("out len =%d\n", out_len);
//    std::fwrite(msg->proMicBuff, sizeof(short ), out_len, fpMic);
    std::fwrite(msg->micBuff, sizeof(short), out_len, fpOriMic);
    std::fwrite(msg->refBuff, sizeof(short), out_len, fpOriRef);
}

AECProcesser::AECProcesser() {
    fpMic = std::fopen(TEST_PCM_DIR "aec.pcm", "wb+");
    fpOriMic = std::fopen(TEST_PCM_DIR "aecOriMic.pcm", "wb+");
    fpOriRef = std::fopen(TEST_PCM_DIR "aecOriRef.pcm", "wb+");
    memset(mRefBuffer, 0, sizeof(mRefBuffer));
    memset(mMicBuffer, 0, sizeof(mMicBuffer));
}

AECProcesser::~AECProcesser() {
    std::fclose(fpMic);
    std::fclose(fpOriMic);
    std::fclose(fpOriRef);
    dspProcess.release();

}

std::string AECProcesser::getTag() {
    return "AECProcesser";
}

