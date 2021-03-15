//
// Created by elevoc on 2019/4/23.
//

#include "EVAECProcess.h"
#include "EVPCMHelper.h"


#ifdef ENABLE_AEC

#define BASE_PATH "C:\\test\\"

using namespace webrtc;

int EVAECProcess::ElevocAecProcess(size_t nDataLen, float *szInData, float *aecLinearOut, float *aecOutPut, int testmode) {

//	convertFloatMulGain(szInData, nDataLen, aec_near_buffer_copy_with_gain);

    float *sInPointer = (float *)szInData;
    
    if (samplerate == 48000) {
        IFChannelBuffer* bufIn=(IFChannelBuffer*) splittingFilterIn;
        IFChannelBuffer* bufOut=(IFChannelBuffer*) splittingFilterOut;

		IFChannelBuffer* linear_bufIn = (IFChannelBuffer*)splittingFilter_linear_in;
		IFChannelBuffer* linear_bufOut = (IFChannelBuffer*)splittingFilter_linear_out;
        
        memcpy(bufIn->fbuf()->bands(0)[0], sInPointer, nDataLen * sizeof(float));
        
        ((SplittingFilter*)splittingFilter)->Analysis(bufIn, bufOut);

		int delay_samples = 48;
		int left_samples = 160 - delay_samples;

		float band_low[160], band_two[160], band_three[160];
		memcpy(band_two, aec_near_two_band, sizeof(float) * delay_samples);
		memcpy(band_three, aec_near_three_band, sizeof(float) * delay_samples);

		/*memmove(aec_near_two_band, aec_near_two_band + 160, sizeof(aec_near_two_band) - 160 * sizeof(float));
		memmove(aec_near_three_band, aec_near_three_band + 160, sizeof(aec_near_three_band) - 160 * sizeof(float));*/

		memcpy(band_two + delay_samples, bufOut->fbuf_const()->bands(0)[1], sizeof(float) * left_samples);
		memcpy(band_three + delay_samples, bufOut->fbuf_const()->bands(0)[2], sizeof(float) * left_samples);

		float* two_band = (float *)(bufOut->fbuf_const()->bands(0)[1]);
		float* three_band = (float *)(bufOut->fbuf_const()->bands(0)[2]);
		memcpy(aec_near_two_band, two_band + left_samples, sizeof(float) * delay_samples);
		memcpy(aec_near_three_band, three_band + left_samples, sizeof(float) * delay_samples);

        const float* aecIn[3];
        float* aecOut[3]; float _aecOut[3][160];
		float* linearOut[3]; float _linearOut[3][160];

        for(int i=0;i<3;i++){
            aecIn[i]=bufOut->fbuf_const()->bands(0)[i];
            aecOut[i]=_aecOut[i];
			linearOut[i] = _linearOut[i];
        }

		/*if (fp == NULL) {
			fp = fopen("C:\\test\\aec_near_low_band.pcm", "wb");
		}

		if (fp != NULL) {
			float temp[160];
			convertFloatDevGain(bufOut->fbuf()->bands(0)[0], 160, temp);
			fwrite(temp, sizeof(float), 160, fp);
		}*/

        WebRtcAec_Process(this->aecInst, aecIn, 3, aecOut, linearOut, 160, 0, 0, volumeValue, testmode);

		memcpy(bufOut->fbuf()->bands(0)[0], _aecOut[0], 160 * sizeof(float));
		memcpy(bufOut->fbuf()->bands(0)[1], _aecOut[1], 160 * sizeof(float));
		memcpy(bufOut->fbuf()->bands(0)[2], _aecOut[2], 160 * sizeof(float));

		((webrtc::SplittingFilter*)splittingFilter)->Synthesis(bufOut, bufIn);
		memcpy(aecOutPut, bufIn->fbuf_const()->bands(0)[0], 480 * sizeof(float));

		memcpy(band_low, linearOut[0], sizeof(float) * 160);
		memcpy(linear_bufOut->fbuf()->bands(0)[0], band_low, 160 * sizeof(float));
		memcpy(linear_bufOut->fbuf()->bands(0)[1], band_two, 160 * sizeof(float));
		memcpy(linear_bufOut->fbuf()->bands(0)[2], band_three, 160 * sizeof(float));

		((webrtc::SplittingFilter*)splittingFilter_linear)->Synthesis(linear_bufOut, linear_bufIn);
		memcpy(aecLinearOut, linear_bufIn->fbuf_const()->bands(0)[0], 480 * sizeof(float));
        
    }else if (samplerate == 32000) {
        /*IFChannelBuffer* bufIn=(IFChannelBuffer*) splittingFilterIn_32k;
        IFChannelBuffer* bufOut=(IFChannelBuffer*) splittingFilterOut_32k;
        
        memcpy(bufIn->fbuf()->bands(0)[0], sInPointer, nDataLen * sizeof(float));
        
        ((SplittingFilter*)splittingFilter_32k)->Analysis(bufIn, bufOut);
        
        const float* aecIn[2];
        float* aecOut[2];
        float _aecOut[2][160];

		float* linearAecOut[2];
		float _linearAecOut[2][160];

        for(int i=0;i<2;i++){
            aecIn[i]=bufOut->fbuf_const()->bands(0)[i];
            aecOut[i]=_aecOut[i];
			linearAecOut[i] = _linearAecOut[i];
        }
        WebRtcAec_Process(this->aecInst, aecIn, 2, aecOut, linearAecOut,160, 0, 0);*/
    }else { 
        size_t DataLen = nDataLen;
        
        float m_sOutNear_frame[AEC_MAX_FRAME_SIZE];
		float m_sLinearOutNear_frame[AEC_MAX_FRAME_SIZE];
        float m_sInNear_frame[AEC_MAX_FRAME_SIZE];
        
        memset(m_sInNear_frame, 0, sizeof(float) * AEC_MAX_FRAME_SIZE);
        memset(m_sOutNear_frame, 0, sizeof(float) * AEC_MAX_FRAME_SIZE);
		memset(m_sLinearOutNear_frame, 0, sizeof(float) * AEC_MAX_FRAME_SIZE);

        size_t i = 0;

        for (i = 0; i < DataLen; i++)
        {
            m_sInNear_frame[i] = *(sInPointer + i);
        }

        float* const p = m_sInNear_frame;
        const float* const*  in = &p;

        float* const q = m_sOutNear_frame;
        float* const* out = &q;

		float* const m = m_sLinearOutNear_frame;
		float* const* linear_out = &m;

    
        WebRtcAec_Process(this->aecInst, in, 1, out, linear_out, DataLen, 0, 0, volumeValue, testmode);

		memcpy(aecLinearOut, m_sLinearOutNear_frame, sizeof(float) * DataLen);
		memcpy(aecOutPut, m_sOutNear_frame, sizeof(float) * DataLen);
    }
    
    return 0;
}

void EVAECProcess::processAgc(float* audio, int len, float* output) {
	if (samplerate == 48000) {


		IFChannelBuffer* bufIn = (IFChannelBuffer*)splittingFilterNSIn;
		IFChannelBuffer* bufOut = (IFChannelBuffer*)splittingFilterNSOut;

		memcpy(bufIn->fbuf()->bands(0)[0], audio, len * sizeof(float));

		((SplittingFilter*)splittingFilterNS)->Analysis(bufIn, bufOut);

		const float* agcIn[3];
		float* agcOut[3]; float _agcOut[3][160];

		for (int i = 0; i < 3; i++) {
			agcIn[i] = bufOut->fbuf_const()->bands(0)[i];
			agcOut[i] = _agcOut[i];
		}

		const int16_t* agcIn_s[3]; int16_t _agcin_s[3][160];
		int16_t* agcOut_s[3]; int16_t _agcOut_s[3][160];
		for (int i = 0; i < 3; ++i) {
			agcIn_s[i] = _agcin_s[i];
			agcOut_s[i] = _agcOut_s[i];
			for (int j = 0; j < 160; ++j)
				_agcin_s[i][j] = (short)(agcIn[i][j] * 32767);
		}

		int inMicLevel = 0, outMicLevel = -1;
		int16_t echo = 0;
		uint8_t saturationWarning = 1;
		WebRtcAgc_Process(agcInst, (const int16_t* const*)&agcIn_s, 3, 160,
			(int16_t* const*)&agcOut_s, inMicLevel, &outMicLevel, echo,
			&saturationWarning);

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 160; ++j)
				_agcOut[i][j] = (agcOut_s[i][j] / 32767.f);
		}

		memcpy(bufOut->fbuf()->bands(0)[0], agcOut[0], 160 * sizeof(float));
		memcpy(bufOut->fbuf()->bands(0)[1], agcOut[1], 160 * sizeof(float));
		memcpy(bufOut->fbuf()->bands(0)[2], agcOut[2], 160 * sizeof(float));

		((webrtc::SplittingFilter*)splittingFilterNS)->Synthesis(bufOut, bufIn);
		memcpy(output, bufIn->fbuf_const()->bands(0)[0], 480 * sizeof(float));

		//for (int i = 0; i < 480; ++i) {
		//	output[i] = tmpAudio[i] / 32767.0f;
		//}
	}
}

void EVAECProcess::aec_init(int samplerate) {
    aecInst = (webrtc::Aec *)WebRtcAec_Create();

    WebRtcAec_Init(aecInst, samplerate, samplerate);
    webrtc::AecConfig aecConfig;
	aecConfig.nlpMode = 0;
	aecConfig.skewMode = 0;
	aecConfig.metricsMode = 0;
	aecConfig.delay_logging = 0;
	aecConfig.div_psdw_th = 8.0f;
	aecConfig.alpha_dts = 0.9f;
	aecConfig.hnlFbIdx_L = 0.7f;
	aecConfig.hnlFbIdx_H = 0.85f;
	aec_set_config(aecConfig);
}

void EVAECProcess::aec_set_config(webrtc::AecConfig config) {
	//if (config.hnlFbIdx < 0 || config.hnlFbIdx > 1) {
		//config.hnlFbIdx = 0.75;
	//}
	if (WebRtcAec_set_config(aecInst, config) == 0) {
		LOGI("aec config success");

		LOGI("nlpmode: %d, skewmode: %d, metricsmode: %d, delay_logging: %d, div_psdw_th: %f, alpha_dts: %f, aec_hnlFbIdx_L: %f, aec_hnlFbIdx_H: %f",
			config.nlpMode, config.skewMode, config.metricsMode, config.delay_logging, config.div_psdw_th, config.alpha_dts, config.hnlFbIdx_L, config.hnlFbIdx_H);
	}
	else {
		LOGI("failed!!!!");
	}
}

void EVAECProcess::aec_free() {
    if (this->aecInst){
        WebRtcAec_Free(this->aecInst);
        this->aecInst = nullptr;
    }
}



#endif

void EVAECProcess::start(){

    clean_buffer();
    aec_free();
    aec_init(samplerate);

    seed = AEC_NLP_SEED;
}

void EVAECProcess::writeFarendAudio(const float *audioData, size_t len)  {

//        以下操作只针对 48000 k 数据
    static_assert(ELEVOC_DNN_SAMPLERATE_48K == TEST_SAMPLE_RATE, "only support 48000 sample rate!!!");
    webrtc::IFChannelBuffer* bufIn=(webrtc::IFChannelBuffer*) splittingFilterFarendIn;
    webrtc::IFChannelBuffer* bufOut=(webrtc::IFChannelBuffer*) splittingFilterFarendOut;
    memcpy(bufIn->fbuf()->bands(0)[0], audioData, len * sizeof(float));
    WebRtcAec_BufferFarend(this->aecInst, bufOut->fbuf_const()->bands(0)[0], 160);
}

void EVAECProcess::clean_buffer() {
//		memset(aec_farend_Buffer_copy_with_gain,	0,  sizeof(aec_farend_Buffer_copy_with_gain));
//		memset(aec_near_buffer_copy_with_gain,		0,	sizeof(aec_near_buffer_copy_with_gain));
//        memset(aec_farend_Buffer,					0,  sizeof(aec_farend_Buffer));
//        memset(aec_linear_buffer,					0,  sizeof(aec_linear_buffer));

    memset(aec_near_two_band, 0, sizeof(aec_near_two_band));
    memset(aec_near_three_band, 0, sizeof(aec_near_three_band));
}

void EVAECProcess::setSamplerate(int sampleRate) {
    if (this->samplerate != sampleRate) {
        this->samplerate = sampleRate;

        switch (this->samplerate) {
            case 8000:
            {
                aec_linear_cnt = 5;
                ns_cnt = 4;
                aec_shift = 80;
            }
                break;
            case 16000:
            {
                aec_linear_cnt = 5;
                ns_cnt = 2;
                aec_shift = 160;
            }
                break;
            case 32000:
            {
                aec_linear_cnt = 5;
                ns_cnt = 2;
                aec_shift = 320;
            }
                break;
            case 48000:
            {
                aec_linear_cnt = 5;
                ns_cnt = 2;
                aec_shift = 480;
            }
                break;
            default:
                LOGE("aec not support samplerate: %d", this->samplerate);
                break;
        }

        aec_free();
        aec_init(sampleRate);
//            farendQueue.cleanQueue();
    }
}

EVAECProcess::~EVAECProcess() {
    if (splittingFilter != NULL) {
        delete (webrtc::SplittingFilter*)splittingFilter;
    }
    if (splittingFilterIn != NULL) {
        delete (webrtc::IFChannelBuffer*)splittingFilterIn;
    }
    if (splittingFilterOut != NULL) {
        delete (webrtc::IFChannelBuffer*)splittingFilterOut;
    }

    if (splittingFilter_linear != NULL) {
        delete (webrtc::SplittingFilter*)splittingFilter_linear;
    }

    if (splittingFilter_linear_in != NULL) {
        delete (webrtc::IFChannelBuffer*)splittingFilter_linear_in;
    }

    if (splittingFilter_linear_out != NULL) {
        delete (webrtc::IFChannelBuffer*)splittingFilter_linear_out;
    }

    if (splittingFilterFarend != NULL) {
        delete (webrtc::SplittingFilter*)splittingFilterFarend;
    }
    if (splittingFilterFarendIn != NULL) {
        delete (webrtc::IFChannelBuffer*)splittingFilterFarendIn;
    }
    if (splittingFilterFarendOut != NULL) {
        delete (webrtc::IFChannelBuffer*)splittingFilterFarendOut;
    }

    if (splittingFilterNS != NULL) {
        delete (webrtc::SplittingFilter*)splittingFilterNS;
    }
    if (splittingFilterNSIn != NULL) {
        delete (webrtc::IFChannelBuffer*)splittingFilterNSIn;
    }
    if (splittingFilterNSOut != NULL) {
        delete (webrtc::IFChannelBuffer*)splittingFilterNSOut;
    }

    aec_free();

    if (agcInst) {
        WebRtcAgc_Free(agcInst);
    }
};

EVAECProcess::EVAECProcess() {

    clean_buffer();

    samplerate = 16000;
    aec_linear_cnt = 5;
    ns_cnt = 2;
    aec_shift = samplerate / 100;
    dumpFarAudio = false;

    volumeValue = 40;

    this->aecInst = NULL;

//        farendQueue.setQueName("farendQue");

    splittingFilter=new webrtc::SplittingFilter(1, 3, 480);
    splittingFilterIn=new webrtc::IFChannelBuffer(480, 1, 1);
    splittingFilterOut=new webrtc::IFChannelBuffer(480, 1, 3);

    splittingFilter_linear = new webrtc::SplittingFilter(1, 3, 480);
    splittingFilter_linear_in = new webrtc::IFChannelBuffer(480, 1, 1);
    splittingFilter_linear_out = new webrtc::IFChannelBuffer(480, 1, 3);

    splittingFilterFarend=new webrtc::SplittingFilter(1, 3, 480);
    splittingFilterFarendIn=new webrtc::IFChannelBuffer(480, 1, 1);
    splittingFilterFarendOut=new webrtc::IFChannelBuffer(480, 1, 3);

    splittingFilterNS = new webrtc::SplittingFilter(1, 3, 480);
    splittingFilterNSIn = new webrtc::IFChannelBuffer(480, 1, 1);
    splittingFilterNSOut = new webrtc::IFChannelBuffer(480, 1, 3);

    WebRtcAgcConfig agcConfig;
    agcConfig.compressionGaindB = 30; // default 9 dB
    agcConfig.limiterEnable = 1; 	// default kAgcTrue (on)
    agcConfig.targetLevelDbfs = 1; // default 3 (-3 dBOv)
    agcInst = WebRtcAgc_Create();
    if (agcInst != NULL) {
        int status = WebRtcAgc_Init(agcInst, 0, 255, kAgcModeFixedDigital, 48000);
        if (status != 0) {
            LOGE("agc init failed");
            WebRtcAgc_Free(agcInst);
            agcInst = NULL;
        }
        else {
            status = WebRtcAgc_set_config(agcInst, agcConfig);
            if (status != 0) {
                LOGE("agc set config failed");
                WebRtcAgc_Free(agcInst);
                agcInst = NULL;
            }
        }
    }
}