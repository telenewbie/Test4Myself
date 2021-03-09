//
// Created by elevoc on 2020-05-22.
//

#ifndef NNSS_DEMO_EVDSPPROCESS_H
#define NNSS_DEMO_EVDSPPROCESS_H

#include "EVByteQueue.h"
#define ENABLE_DSP
#ifdef ENABLE_DSP

#include "modules/audio_processing/include/audio_processing.h"
#include "api/audio/echo_canceller3_factory.h"
#include "api/audio/audio_frame.h"

class EVDspProcess {
public:
    EVDspProcess():heap(1), linear_out_heap(heap.data(), 1){
        memset(aec_farend_Buffer, 0, sizeof(aec_farend_Buffer));
        farendQueue.setQueName("farendQue");
        isUpdatedMask = false;
        maskValue_ = 1.0f;
    };
    ~EVDspProcess(){}

    bool init(int samplerate, bool enableAgc);

    void setSamplerate(int samplerate) {samplerate_ = samplerate;}
    
    void setAecMaskValue(float maskValue);

    bool start(bool enableAgc) {
        bool res = init(samplerate_, enableAgc);
        farendQueue.cleanQueue();
        return res;
    }

    void stop(bool stopImmediately) {

        farendQueue.writeAudioEnd();
        if (stopImmediately) {
            farendQueue.cleanQueue();
        }
    }

    void writeFarendAudio(const char *audioData, size_t len) {

        size_t leftSize = len;
        size_t onceSize = (samplerate_ / 100) * sizeof(short);
        size_t readSize = 0;
        char *pHead = (char *)audioData;

        while (true) {
            readSize = leftSize < onceSize ? leftSize : onceSize;
            if (leftSize <= 0) {
                break;
            }
            WritingResult result = farendQueue.writeAudio(pHead, readSize);
            if (result != WriteSuccess) {
                return;
            }
            pHead += readSize;
            leftSize -= readSize;
        }
    }

    void readAudio(char *buffer, size_t *size) {
        size_t target = *size;

        farendQueue.readAudioWithoutWait((char *)aec_farend_Buffer, size);
        memcpy(buffer, aec_farend_Buffer, sizeof(char) * *size);

        if (*size < target) {
            memset(buffer + *size, 0, target - *size);
            memcpy(aec_farend_Buffer, buffer, sizeof(char) * target);
        }
        *size = target;
    }

    void release() {
        farendQueue.release();
    }

    int FarFrame(short* data, int frames);
    int NearFrame(short* data, int frames, short* outData, int& outFrames, short* linearOut, int &linearFrames);

private:
    webrtc::AudioProcessing* apm_;
    webrtc::StreamConfig near_in_config_;
    webrtc::StreamConfig far_in_config_;
    webrtc::AudioFrame nearFrame_;
    webrtc::AudioFrame farFrame_;
    int frameSize_;
    int samplerate_;
    float maskValue_;
    bool isUpdatedMask;

    EVByteQueue farendQueue;

    short aec_farend_Buffer[480 * 4];

    void clean_buffer() {
        memset(aec_farend_Buffer, 0, sizeof(aec_farend_Buffer));
    }
    std::vector<std::array<float, 160>> heap;
    rtc::ArrayView<std::array<float, 160>> linear_out_heap;
};

#endif

#endif //NNSS_DEMO_EVDSPPROCESS_H
