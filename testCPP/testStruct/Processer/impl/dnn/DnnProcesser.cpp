//
// Created by Elevoc on 2021/1/5 0005.
//

#include <EVPCMHelper.h>
#include "DnnProcesser.h"
#include "EVProcessEnumType.h"
#include "ElevocLog.h"

FILE* fpdnn = nullptr;


void DnnProcesser::process(DataMsg* msg) {

    if (feature.sample_rate != msg->outSampleRate) {
        LOGI("%s,feature sample rate:%d,msg sample Rate:%d",getTag().data(),feature.sample_rate,msg->outSampleRate);
        feature_reset(&feature, msg->outSampleRate);
        synthetic_reset(&synthetic, msg->outSampleRate);
        dnn_ns_reset(&ns, msg->outSampleRate);
    }

    printf("aecprocesser!!!\n");
    int expect_frameSize = msg->sample_num;
    MODE mode = DEFAULT;
    size_t floatLen;
    memmove(input_buffer_f32, input_buffer_f32 + expect_frameSize,
            sizeof(float) * expect_frameSize);

    // 如果已经是float类型
    for (int i = 0; i < expect_frameSize; ++i) {
        input_buffer_f32[i+expect_frameSize] = 1.0f*msg->micBuff[i] / 32768.f;
    }

    int res = 1;
    int is_fft_right = 1;
    res = feature_push_one_frame(&feature, input_buffer_f32);
    if (res != 1) {
        is_fft_right = 0;
    }
    dnn_ns_run(&ns, msg->outSampleRate, feature.mic_fft, feature.random_mic_fft,
               feature.mic_fft);
    feature_apply_mask(&feature, ns.dnnmask, 1, 1);
    /*res =*/ feature_pop_one_frame(&feature);
    if (res != 1) {
        is_fft_right = 0;
    }
    if (is_fft_right == 1) {
        synthetic_write(&synthetic, feature.out_time_data);
        convertFloatToShort(synthetic.synthetic_out, expect_frameSize, msg->micBuff,
                           NULL, mode);

        fwrite(msg->micBuff,sizeof(TYPE_SAMPLE_t),msg->sample_num,fpdnn);
    }
}

DnnProcesser::DnnProcesser() {
//    feature_reset(&feature, TEST_SAMPLERATE);
//    synthetic_reset(&synthetic, TEST_SAMPLERATE);
//    dnn_ns_reset(&ns, TEST_SAMPLERATE);
    memset(input_buffer_f32, 0, sizeof(input_buffer_f32));
    fpdnn = fopen(TEST_PCM_DIR "dnn_out.pcm","wb+");
}

std::string DnnProcesser::getTag() {
    return "DnnProcesser";
}

DnnProcesser::~DnnProcesser() {
    fclose(fpdnn);
}
