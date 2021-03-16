//
// Created by Elevoc on 2021/3/11 0011.
//

#include "ElevocLog.h"
#include "WebrtcAec.h"
#include <limits>


static int channel_index = 0;
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

static inline float FloatToFloatS16(float v) {
    v = std::min(v, 1.f);
    v = std::max(v, -1.f);
    return v * 32768.f;
}

static inline float FloatS16ToFloat(float v) {
    v = std::min(v, 32768.f);
    v = std::max(v, -32768.f);
    constexpr float kScaling = 1.f / 32768.f;
    return v * kScaling;
}

static inline FILE *createFD(const std::string &prefix, int index) {
    std::stringstream path;
    path << TEST_PCM_DIR << prefix << index << ".pcm";

    return fopen(path.str().data(), "wb+");
}


AEC::WebrtcAec::WebrtcAec(const AECConfig &cfg) : BaseAECAdapter(cfg) {
    ++channel_index;
#ifdef TELENEWBIE_TEST_AEC
    mProcess.setSamplerate(cfg.sampleRate);
#endif


    fp1 = createFD("tmp_aec_aecnearfloat_", channel_index);
    fp2 = createFD("tmp_aec_aecnearshort_", channel_index);
    fp3 = createFD("tmp_aec_aecnearProShort_", channel_index);
    fp4 = createFD("tmp_aec_aecnearProFloat_", channel_index);
    fp5 = createFD("tmp_aec_aecfarFloat_", channel_index);
    fp6 = createFD("tmp_aec_aecfarShort_", channel_index);
}


int AEC::WebrtcAec::writeFarFrame(short *data, int frames) {
    // short  to  float
    fwrite(data, sizeof(short), frames, fp6);
    for (int i = 0; i < frames; ++i) {
        mTemp[i] = 1.0f * data[i];
    }

    int err = writeFarFrame(mTemp, frames);
    for (int i = 0; i < frames; ++i) {
        mTemp[i] = S16ToFloat(data[i]);
    }
    fwrite(mTemp, sizeof(float), frames, fp5);

    return err;
}

int AEC::WebrtcAec::writeNearFrame(short *data, int frames, short *outData, int &outFrames, short *linearOut,
                                   int &linearFrames) {

    fwrite(data, sizeof(short), frames, fp2);
    for (int i = 0; i < frames; ++i) {
        mTemp[i] = S16ToFloat(data[i]);
    }
    fwrite(mTemp, sizeof(float), frames, fp1);
    for (int i = 0; i < frames; ++i) {
        mTemp[i] = 1.0f * data[i];
    }

    int err = writeNearFrame(mTemp, frames, mTemp, outFrames, mLinearAECOut, linearFrames);

    for (int i = 0; i < outFrames; ++i) {
        outData[i] = (short) mTemp[i];//+-1
    }
    fwrite(outData, sizeof(short), outFrames, fp3);
    for (int i = 0; i < outFrames; ++i) {
        mTemp[i] = FloatS16ToFloat(mTemp[i]);
    }
    fwrite(mTemp, sizeof(float), outFrames, fp4);

    for (int i = 0; i < linearFrames; ++i) {
        linearOut[i] = (short) (mLinearAECOut[i]);
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
    return mProcess.ElevocAecProcess(frames, data, linearOut, outData, 1);
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
