//
// Created by Elevoc on 2021/1/7 0007.
//

#include <Processer/ProcesserPori.h>
#include "ResampleOutProcesser.h"
#include "TEST_CONSTANT.h"

static FILE *fpMic = nullptr;

int ResampleOutProcesser::getMsgIndex() {
    return PORI_RESAMPLE_OUT;
}

void ResampleOutProcesser::process(DataMsg *msg) {
    if (mSpeexResamplerState == nullptr) {
        int err = 0;
        mSpeexResamplerState = elevoc_resampler_init(msg->channel, msg->outSampleRate, msg->inSampleRate, 9,
                                                     &err);
        if (mSpeexResamplerState != nullptr) {
            elevoc_resampler_set_rate(mSpeexResamplerState, msg->outSampleRate, msg->inSampleRate);
        } else {
            LOGE("error from %d to %d sample rate", msg->outSampleRate, msg->inSampleRate);
            return;
        }
    }
#ifdef TEST_FLOAT_BYTE
    float *input = (float *) msg->micBuff;
    float *input_resampler = (float *) input_resampler_buffer;
    unsigned int out_len = msg->inSampleRate / 100;
    unsigned int in_len = msg->outSampleRate / 100;
    elevoc_resampler_process_float(mSpeexResamplerState, 0, input, &in_len, input_resampler, &out_len);
    memcpy(input, input_resampler, sizeof(float) * out_len);
    msg->len = sizeof(float) * out_len;
#else

    short *input = msg->micBuff;
    short *input_resampler = (short *) input_resampler_buffer;
    unsigned int out_len = msg->inSampleRate / 100;
    unsigned int in_len = msg->outSampleRate / 100;
    elevoc_resampler_process_int(mSpeexResamplerState, 0, input, &in_len, input_resampler, &out_len);
    memcpy(input, input_resampler, sizeof(short) * out_len);
    msg->sample_num = out_len;
#endif
    std::fwrite(msg->micBuff, sizeof(short), out_len, fpMic);
}

bool ResampleOutProcesser::canProcess(DataMsg *msg) {
    return msg->inSampleRate != SAMPLE_RATE_16k && msg->outSampleRate == SAMPLE_RATE_16k;
}

ResampleOutProcesser::ResampleOutProcesser() {
    fpMic = std::fopen(TEST_PCM_DIR "mic_resample_out.pcm", "wb+");
}

std::string ResampleOutProcesser::getTag() {
    return "ResampleOutProcesser";
}
