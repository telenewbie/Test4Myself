//
// Created by Elevoc on 2021/1/7 0007.
//

#include <Processer/ProcesserPori.h>
#include <TEST_CONSTANT.h>
#include "ResampleInProcesser.h"

static FILE *fpMic = nullptr;
static FILE *fpRef = nullptr;

int ResampleInProcesser::getMsgIndex() {
    return PORI_RESAMPLE_IN;
}


void ResampleInProcesser::process(DataMsg* msg) {
//return ;

    if (mSpeexResamplerState == nullptr) {
        initResample(&mSpeexResamplerState, msg);
    }
    if (mSpeexResamplerRef == nullptr) {
        initResample(&mSpeexResamplerRef, msg);
    }
    msg->outSampleRate = SAMPLE_RATE_16k;
#ifdef TEST_FLOAT_BYTE

    float *pinput = (float *) msg->micBuff;
    float *pinput_resampler = (float *) input_resampler_buffer;
    unsigned int in_len = msg->inSampleRate / 100;
    unsigned int out_len = msg->outSampleRate / 100;

    elevoc_resampler_process_float(
            mSpeexResamplerState,
            0, pinput, &in_len, pinput_resampler, &out_len);
    memcpy(pinput, pinput_resampler, sizeof(float) * out_len);

    msg->len = sizeof(float) * out_len;
    // far
    float *pfarinput = (float *) msg->refBuff;
    float *pfarinput_resampler = (float *) far_resampler_buffer;
    elevoc_resampler_process_float(
            mSpeexResamplerRef,
            0, pfarinput, &in_len, pfarinput_resampler, &out_len);
    memcpy(pfarinput, pfarinput_resampler, sizeof(float) * out_len);

#else

    short *pinput = msg->micBuff;
    auto *pinput_resampler = (short *) input_resampler_buffer;
    unsigned int in_len = msg->inSampleRate / 100; // 总长 = 10ms数据* sizeof(short) / sizeof(short)
    unsigned int out_len = msg->outSampleRate / 100;

    elevoc_resampler_process_int(
            mSpeexResamplerState,
            0, pinput, &in_len, msg->micBuff, &out_len);
//    memcpy(msg->proMicBuff, pinput_resampler, sizeof(short) * out_len);

    // far
    short *pfarinput =  msg->refBuff;
    auto *pfarinput_resampler = (short *) far_resampler_buffer;
    elevoc_resampler_process_int(
            mSpeexResamplerRef,
            0, pfarinput, &in_len, msg->refBuff, &out_len);
//    memcpy(msg->proRefBuff, pfarinput_resampler, sizeof(short) * out_len);

    //msg->len = sizeof(short) * out_len;
    msg->sample_num = out_len;
#endif
    // 输出
    std::fwrite(msg->micBuff, sizeof(short ), msg->sample_num, fpMic);
    std::fwrite(msg->refBuff, sizeof(short ), msg->sample_num, fpRef);

}

bool ResampleInProcesser::canProcess(DataMsg *msg) {
    return msg->inSampleRate == SAMPLE_RATE_441k || msg->inSampleRate == SAMPLE_RATE_32k ||
           msg->inSampleRate == SAMPLE_RATE_48k;
}

ResampleInProcesser::ResampleInProcesser() {
    fpMic = std::fopen(TEST_PCM_DIR "mic_resample_in.pcm", "wb+");
    fpRef = std::fopen(TEST_PCM_DIR "ref_resample_in.pcm", "wb+");
}

ResampleInProcesser::~ResampleInProcesser() {
    if (mSpeexResamplerState != nullptr) {
        elevoc_resampler_destroy(mSpeexResamplerState);
    }
    if (mSpeexResamplerRef != nullptr) {
        elevoc_resampler_destroy(mSpeexResamplerRef);
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
