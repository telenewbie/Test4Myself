//
// Created by elevoc on 2020-05-22.
//

#include "EVDspProcess.h"
#include "ElevocLog.h"

#ifdef ENABLE_DSP

//#define DUMP_AEC_FILE

using namespace webrtc;

bool EVDspProcess::init(int samplerate, bool enableAgc) {

    apm_ = AudioProcessingBuilder().Create();

    if (!apm_)
    {
        return false;
    }

    AudioProcessing::Config config;
    config.echo_canceller.enabled = true;
    config.echo_canceller.mobile_mode = false;
    config.high_pass_filter.enabled = true;
    config.echo_canceller.export_linear_aec_output = true;
    if (enableAgc) {
        config.gain_controller1.enabled = enableAgc;
        config.gain_controller1.mode = AudioProcessing::Config::GainController1::kAdaptiveAnalog;
    }

    apm_->ApplyConfig(config);

    near_in_config_ = StreamConfig(samplerate, 1);
    far_in_config_ = StreamConfig(samplerate, 1);

    frameSize_ = AudioProcessing::kChunkSizeMs * samplerate / 1000;
    samplerate_ = samplerate;

    return true;
}

void EVDspProcess::setAecMaskValue(float maskValue) {
    if (maskValue_ != maskValue) {
        maskValue_ = maskValue;
        isUpdatedMask = true;
    } else {
        isUpdatedMask = false;
    }
}

int EVDspProcess::FarFrame(short* data, int frames) {

    for (int i = 0; i + frameSize_ <= frames; i += frameSize_)
    {
        farFrame_.UpdateFrame(0, data + i, frameSize_, samplerate_,
                              AudioFrame::kNormalSpeech, AudioFrame::kVadUnknown,
                              1);
        apm_->ProcessReverseStream(&farFrame_);
    }

    return 0;
}

#ifdef DUMP_AEC_FILE
FILE *fp_linear = NULL;
FILE *fp_non_linear = NULL;
#endif

int EVDspProcess::NearFrame(short* data, int frames, short* outData, int& outFrames, short* linearOut, int&
linearFrames) {
    outFrames = 0;
    
    if (isUpdatedMask) {
        apm_->setAecMaskValue(maskValue_);
    }

    int linear_frame_size = samplerate_ == 8000 ? 80 : 160;

    for (int i = 0; i + frameSize_ <= frames; i += frameSize_) {
        nearFrame_.UpdateFrame(0, data + i, frameSize_, samplerate_,
                               AudioFrame::kNormalSpeech, AudioFrame::kVadUnknown,
                               1);
        apm_->ProcessStream(&nearFrame_);

        bool res = apm_->GetLinearAecOutput(linear_out_heap);
        if (res) {

            for (int i = 0; i < linear_frame_size; ++i) {
                linearOut[i] = linear_out_heap[0][i];
            }
            linearFrames = 160;
        }else {
            linearFrames = 0;
            LOGE("linear output failed!");
        }

        if (outData != 0)
        {
            memcpy(outData + outFrames, nearFrame_.data(), nearFrame_.samples_per_channel_ * sizeof(short));
        }
        outFrames += nearFrame_.samples_per_channel_;
#ifdef DUMP_AEC_FILE
        if (fp_linear == NULL) {
            fp_linear = fopen("/sdcard/elevoc_aec_linear_out.pcm", "wb");
            fp_non_linear = fopen("/sdcard/elevoc_aec_non_linear_out.pcm", "wb");
        }
        if (fp_linear) {
            fwrite(linearOut, sizeof(short), 160, fp_linear);
        }
        if (fp_non_linear) {
            fwrite(outData, sizeof(short), nearFrame_.samples_per_channel_, fp_non_linear);
        }
#endif
    }

    return 0;
}

#endif
