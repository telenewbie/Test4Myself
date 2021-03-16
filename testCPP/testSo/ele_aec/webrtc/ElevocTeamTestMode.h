
/*
* Copyright (c) 2020 The DEREVERB project authors. All Rights Reserved.
*/

#ifndef TEAMS_TEST_H_
#define TEAMS_TEST_H_

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include "../gsl-master/gsl/gsl_sf_expint.h"
#include "ElevocLog.h"
//#include "BuildConfig.h"
#include "ElevocTypeDef.h"

#define STATIC_LEN 179
#define BYPASS_TIME 13000 //2:10

#define DETECT_PASS_FIRST_BLOCK 200
#define DETECT_FAST_CONV 500

#define HLK_TIME 18000   //3min
#define LAT_TIME 60   //600ms

typedef struct {

	float Hamming_win[960];
	float last_linearOut[480];
	void* fft_table_linear;

	int ns_bypass;
	int histogram[STATIC_LEN];
	int index;
	int last_frame;
	//ns 
	float alpha_sig;
	float C_inc;
	float C_dec;
	float ps[NUM_FRQ];
	float noise_ps[NUM_FRQ];
	float eta_n2term[NUM_FRQ];

	//hlk
	int last_hlkfar;
	int hlk_time;
	void *fft_table;
	int psd_histogram[3];

	//latency
	int latency_count;
	int latency_pass;

} TeamTestMode;

#ifdef __cplusplus
extern "C" {
#endif
	__declspec(dllexport) TeamTestMode * distortion_testmode_Init();
	__declspec(dllexport) int testmode_detection(TeamTestMode*ttm, int frame_count, float *near_sig, float *far_sig, float G[481]);
#ifdef __cplusplus
}
#endif
#endif // !TEAMS_TEST_H_
