//
// Created by Elevoc on 2021/3/11 0011.
//

#include "ElevocLog.h"
#include "WebrtcAec.h"
#include <limits>

FILE *fp1 = nullptr;
FILE *fp2 = nullptr;
FILE *fp3 = nullptr;
FILE *fp4 = nullptr;
FILE *fp5 = nullptr;
FILE *fp6 = nullptr;

typedef std::numeric_limits<int16_t> limits_int16;

static inline int16_t FloatToS16(float v) {
    if (v > 0)
        return v >= 1 ? limits_int16::max()
                      : static_cast<int16_t>(v * limits_int16::max() + 0.5f);
    return v <= -1 ? limits_int16::min()
                   : static_cast<int16_t>(-v * limits_int16::min() - 0.5f);
}

static inline float S16ToFloat(int16_t v) {
    static const float kMaxInt16Inverse = 1.f / limits_int16::max();
    static const float kMinInt16Inverse = 1.f / limits_int16::min();
    return v * (v > 0 ? kMaxInt16Inverse : -kMinInt16Inverse);
}

AEC::WebrtcAec::WebrtcAec(const AECConfig &cfg) : BaseAECAdapter(cfg) {

#ifdef TELENEWBIE_TEST_AEC
    mProcess.setSamplerate(cfg.sampleRate);
#endif

    fp1 = fopen(TEST_PCM_DIR"tmp_aec_aecnearfloat.pcm", "wb+");
    fp2 = fopen(TEST_PCM_DIR"tmp_aec_aecnearshort.pcm", "wb+");
    fp3 = fopen(TEST_PCM_DIR"tmp_aec_aecnearProShort.pcm", "wb+");
    fp4 = fopen(TEST_PCM_DIR"tmp_aec_aecnearProFloat.pcm", "wb+");
    fp5 = fopen(TEST_PCM_DIR"tmp_aec_aecfarFloat.pcm", "wb+");
    fp6 = fopen(TEST_PCM_DIR"tmp_aec_aecfarShort.pcm", "wb+");
}


int AEC::WebrtcAec::writeFarFrame(short *data, int frames) {
    // short  to  float
    for (int i = 0; i < frames; ++i) {
        mTemp[i] = S16ToFloat(data[i]);
    }
    return writeFarFrame(mTemp, frames);
}

int AEC::WebrtcAec::writeNearFrame(short *data, int frames, short *outData, int &outFrames, short *linearOut,
                                   int &linearFrames) {

    for (int i = 0; i < frames; ++i) {
        mTemp[i] = S16ToFloat(data[i]);
    }
    int err = writeNearFrame(mTemp, frames, mTemp, outFrames, mLinearAECOut, linearFrames);

    for (int i = 0; i < outFrames; ++i) {
        outData[i] = FloatToS16(mTemp[i]);
    }
    for (int i = 0; i < linearFrames; ++i) {
        linearOut[i] = FloatToS16(mLinearAECOut[i]);
    }

    return err;
}

int AEC::WebrtcAec::writeFarFrame(float *data, int frames) {
#ifdef TELENEWBIE_TEST_AEC
    mProcess.writeFarendAudio(data, frames);
#endif
    return 0;
}

int AEC::WebrtcAec::writeNearFrame(float *data, int frames, float *outData, int &outFrames, float *linearOut,
                                   int &linearFrames) {
    if (frames > FRAME_SIZE_ONE) {
        LOGE("input frame size[%d] too large", frames);
        return -1;
    }
#ifdef TELENEWBIE_TEST_AEC
    return mProcess.ElevocAecProcess(frames, data, linearOut, outData, 0);
#else
    return 0;
#endif
}

bool AEC::WebrtcAec::start() {
#ifdef TELENEWBIE_TEST_AEC
    mProcess.start();
#endif
    return true;
}

void AEC::WebrtcAec::release() {
//    mProcess.
}

AEC::WebrtcAec::~WebrtcAec() {
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);
    fclose(fp6);

}
