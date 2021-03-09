//
//  ElevocDnnNs.h
//  macdnn
//
//  Created by csyyj on 2019/4/23.
//  Copyright © 2019 Elevoc. All rights reserved.
//

#ifndef ElevocDnnNs_h
#define ElevocDnnNs_h

#include <stdio.h>
#include "ElevocNSDefine.h"
#include "ElevocTypeDef.h"

#define ELEVOC_DNN_NS_MEM (1024 * 1100)

typedef struct ElevocDnnNS {
    char memory[ELEVOC_DNN_NS_MEM];
    float *dnnmask;
	float *pfmask;
}ElevocDnnNS;

#ifdef __cplusplus
extern "C" {
#endif
	DNNEVAPI    void dnn_ns_reset(ElevocDnnNS *ns, unsigned int sample_rate);
	DNNEVAPI    void dnn_ns_set_param(ElevocDnnNS *ns, unsigned int sample_rate, NSParamType type, float param);
	DNNEVAPI    int dnn_ns_run(ElevocDnnNS* ns, unsigned int sample_rate, ele_complex_float *fft_in,  ele_complex_float *random_fft_in, ele_complex_float *fft_out);
#ifdef __cplusplus
}
#endif

#endif /* ElevocDnnNs_h */
