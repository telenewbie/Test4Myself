#ifndef GOWILDPCMREADER_ELE_VAD_H
#define GOWILDPCMREADER_ELE_VAD_H

#include <stddef.h>
#include "ElevocTypeDef.h"

#define AMPLITUDE_VALUD_BUF_LEN (10)

typedef void(*vad_start_call_back)(void* externalInfo, unsigned long frameIndex);

typedef void(*vad_stop_call_back)(void* externalInfo, unsigned long frameIndex);

typedef enum ElevocVADState {
    ElevocVadWait,      // vad is waiting for speech
    ElevocVadOn,        // vad detected start
} ElevocVADState;

typedef struct ElevocVAD {
    int probOut;
    ElevocVADState state;
    float frame_threshold;
    float threshold;
    float tmp_x[128]; // for lstm
    float tmp_a[128]; // for lstm
    float state_h[1][32]; // for lstm
    float state_c[1][32]; // for lstm
    int is_one_shot;
    float tmp_mem[64];
    float vad_out[2];
    int no_speech_time;
    int speech_dissolve_time;
    int min_no_speech_time;
    int one_shot_min_no_speech_time;
    int minSpeechDissolveTime;     // voice end time，after vad stop
    float amplitude_threshold;    
    float amplitude_value_buf[AMPLITUDE_VALUD_BUF_LEN]; 
    float smoothProbOut;
    int iframe;
    float smoothWin[100];
    int smoothWinLen;
    int preVoiceState;
    int currentVoiceState;
    int voiceState;
    unsigned long frameIndex;
    vad_start_call_back start_block;
    vad_stop_call_back stop_block;
    void* externalInfo;
} ElevocVAD;

#ifdef __cplusplus
extern "C" {
#endif
    
    DNNEVAPI void vad_init(ElevocVAD *vad);
    
	DNNEVAPI void vad_reset(ElevocVAD *vad);
    
	DNNEVAPI void vad_set_one_shot(ElevocVAD *vad, int is_one_shot);

	DNNEVAPI void vad_setcallback(ElevocVAD *vad, vad_start_call_back start_block, vad_stop_call_back stop_block, void* ext_info);

	DNNEVAPI int vad_write_mask(ElevocVAD *vad, float *current_mask, ele_complex_float *fft_in, int samplerate);

#ifdef __cplusplus
}
#endif

#endif //GOWILDPCMREADER_ELE_VAD_H
