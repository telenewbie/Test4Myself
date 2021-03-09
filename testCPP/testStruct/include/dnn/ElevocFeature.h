#ifndef GOWILDPCMREADER_ELEVOCFEATURE_H
#define GOWILDPCMREADER_ELEVOCFEATURE_H

#include "ElevocTypeDef.h"

typedef struct {
    float in_time_data[MAX_WLEN];
    float out_time_data[MAX_WLEN];
    float sinWin[MAX_WLEN];
    int sample_rate;
    int win_size;
    float cng_level;
    ele_complex_float mic_fft[MAX_NUM_FRQ * NUM_MIC];
    ele_complex_float ref_fft[MAX_NUM_FRQ];
    ele_complex_float random_mic_fft[MAX_NUM_FRQ * NUM_MIC];
} ElevocFeature;

#ifdef __cplusplus
extern "C" {
#endif
    
	DNNEVAPI void feature_reset(ElevocFeature* feature, int sample_rate);
    
	DNNEVAPI void feature_set_sample_rate(ElevocFeature *feature, int sample_rate);
    
	DNNEVAPI int feature_push_one_frame(ElevocFeature *feature, const float *input_time_data);
    
	DNNEVAPI void feature_apply_mask(ElevocFeature *feature, const float *ns_mask, int use_post_filter, float mask_value);
    
	DNNEVAPI void feature_pop_one_frame(ElevocFeature *feature);
    
	DNNEVAPI void feature_apply_win(ElevocFeature *feature, float *data);

#ifdef __cplusplus
}
#endif

#endif //GOWILDPCMREADER_ELEVOCFEATURE_H
