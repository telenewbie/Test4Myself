//
// Created by csyyj on 2017/7/10.
//

#ifndef ELE_SYNTHETIC_H_
#define ELE_SYNTHETIC_H_

#include "ElevocTypeDef.h"

typedef struct Synthetic {
    float ovlap[MAX_SHIFT];
    float synthetic_out[MAX_SHIFT];
    unsigned int ovlap_len;
    unsigned int sample_rate;
} Synthetic;

#ifdef __cplusplus
extern "C" {
#endif
	DNNEVAPI void synthetic_reset(Synthetic *synthetic, unsigned int samplerate);
    
	DNNEVAPI void synthetic_set_sample_rate(Synthetic *synthetic, unsigned int sample_rate);

	DNNEVAPI void synthetic_write(Synthetic *synthetic, float *input_data);
#ifdef __cplusplus
}
#endif

#endif
