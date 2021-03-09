//
//  EVProcessEnumType.h
//  SingleChannelEngine
//
//  Created by elevoc on 2017/12/25.
//  Copyright © 2017年 elevoc. All rights reserved.
//

#ifndef EVProcessEnumType_h
#define EVProcessEnumType_h
#include <stdio.h>
#include <string.h>
#include <stddef.h>

typedef enum EVProcessState {
    EVProcessInvalidState = 0,  
    EVProcessInitedState,   
    EVProcessLoadedState,   
    EVProcessStartState,    
    EVProcessPreStopState,  
    EVProcessStoppedState, 
	EVProcessReleaseState
} EVProcessState;

typedef enum {
	PROCESS_ERROR_LICENSE_VERIFY = 0,
    PROCESS_ERROR_APPID_VERIFY = 1,
    PROCESS_ERROR_STATE_ERROR = 2,
	PROCESS_ERROR_CACERT_PATH = 3,
} ProcessErrorType;

typedef enum {
	DEFAULT = 0,
    VOIP,
}MODE;

typedef enum {
	EVFuncInit,
	EVFuncReset
}EVFuncAction;

typedef enum {
    NO_DEVICE_MODE = 0,
	HAND_SET = 1,
	HAND_FREE = 2,
}DEVICE_VOICE_MODE;

typedef enum{
    DNN_NOISE_DECAY_BAND_0_600HZ_DB = 0,    	// (-60.0~0.0db)
    DNN_NOISE_DECAY_BAND_600_2500HZ_DB,     	// (-60.0~0.0db)
    DNN_NOISE_DECAY_BAND_2500_5000HZ_DB,    	// (-60.0~0.0db)
    DNN_NOISE_DECAY_BAND_GREATE_THAN_5000HZ_DB, // (-60.0~0.0db)
    DNN_NOISE_SENSITIVITY_LB,               // (0.0~2.0)
    DNN_NOISE_SENSITIVITY_HB,               // (0.0~2.0)
    DNN_POST_NOISE_SENSITIVITY_LB,          // (0.0~2.0)
    DNN_POST_NOISE_SENSITIVITY_HB           // (0.0~2.0)
}DNNParamType;

typedef void(*processFailCallBack)(ProcessErrorType errorType, const char* errorDesc);
typedef void(*howlingDetectCallBack)(bool isHowling);
typedef void(*oauthSuccessCallBack)(const char *licenseId);

// vad state
typedef void(*vadStartCallBack)(unsigned long frameIndex);
typedef void(*vadStopCallBack)(unsigned long frameIndex);

#endif /* EVProcessEnumType_h */
