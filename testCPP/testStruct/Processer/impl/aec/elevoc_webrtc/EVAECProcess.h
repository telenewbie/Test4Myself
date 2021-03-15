//
// Created by elevoc on 2019/4/23.
//

#ifndef NNSS_DEMO_EVAECPROCESS_H
#define NNSS_DEMO_EVAECPROCESS_H

#include <string.h>
#include "EVProcessEnumType.h"
#include "EVByteQueue.h"
#include "ElevocFeature.h"
#include "ElevocSynthetic.h"
#include "ElevocLog.h"
#include <string>
#include "Elevoc_NS.h"
//#define ENABLE_AEC
#ifdef ENABLE_AEC

#include "webrtc/modules/audio_processing/aec/echo_cancellation.h"
#include "webrtc/modules/audio_processing/splitting_filter.h"
#include "webrtc/common_audio/channel_buffer.h"
#include "webrtc/common_audio/vad/include/webrtc_vad.h"
#include "webrtc/modules/audio_processing/agc/legacy/analog_agc.h"

#define AEC_MAX_FRAME_SIZE      (480 * 2)
#define AEC_MAX_FRAME_BYTE_SIZE (AEC_MAX_FRAME_SIZE * 2)

#define SHORTMAX (32768.f)
#define AEC_NLP_SEED    777

//#define DEBUG_AEC_PCM

class EVAECProcess
{
public:
    EVAECProcess();

    ~EVAECProcess();

//    void notifyChangeToSpeaker(){
//        this->aecInst->aec->cur_divergence = 50;
//    }

//    void setDumpAudioDir(std::string dir) {
//        dumpDir = dir;
//    }
//
//    void enableDumpFarAudio(bool enable) {
//        dumpFarAudio = enable;
//    }
//
//	void setVolumeValue(int value) {
//		volumeValue = value;
//	}
//
//	void ns_post_spec(float spec[2][481], float* aec_farend, float post_alpha, float post_psdth_L) {
//		webrtc::ns_post_spectrum(aecInst, spec, aec_farend, post_alpha, post_psdth_L);
//	}
//
//	void postProcess(float* input, int len) {
//		webrtc::fdesignParameq(input, len, input);
//		webrtc::fdesignParameq2(input, len, input);
//	}

    void setSamplerate(int sampleRate);

	void processAgc(float* audio, int len, float* output);

    void start( );

    void writeFarendAudio(const float *audioData, size_t len);

    int ElevocAecProcess(size_t nDataLen, float *szInData, float* aecLinearOut, float* aecOut, int testmode);

public:

	void aec_set_config(webrtc::AecConfig config);

private:
    
    int aec_linear_cnt;
    int ns_cnt;
    int aec_shift;

	int volumeValue;

    size_t seed;

    int samplerate;
    bool dumpFarAudio;
    std::string dumpDir;

	void* agcInst;

    webrtc::Aec *aecInst;
    void* splittingFilter; // webrtc::SplittingFilter
    void* splittingFilterIn; // webrtc::IFChannelBuffer
    void* splittingFilterOut; // webrtc::IFChannelBuffer
	
	void* splittingFilter_linear;	
	void* splittingFilter_linear_in;
	void* splittingFilter_linear_out;
    
    void* splittingFilterFarend; // webrtc::SplittingFilter
    void* splittingFilterFarendIn; // webrtc::IFChannelBuffer
    void* splittingFilterFarendOut; // webrtc::IFChannelBuffer

	void* splittingFilterNS;
	void* splittingFilterNSIn;
	void* splittingFilterNSOut;
    
//	EVByteQueue farendQueue;

//    float aec_farend_Buffer[AEC_MAX_FRAME_BYTE_SIZE];
//	float aec_farend_Buffer_copy_with_gain[AEC_MAX_FRAME_BYTE_SIZE];

//	float aec_linear_buffer[AEC_MAX_FRAME_SIZE];
//	float aec_near_buffer_copy_with_gain[AEC_MAX_FRAME_BYTE_SIZE];
//
	float aec_near_two_band[48];
	float aec_near_three_band[48];

    void clean_buffer() ;

    void aec_init(int samplerate);
    void aec_free();
};

#endif
#endif //NNSS_DEMO_EVAECPROCESS_H
