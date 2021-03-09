//
//  EVPCMHelper.h
//  SingleChannelEngine
//
//  Created by csyyj on 2017/8/2.
//  Copyright © 2017年 elevoc. All rights reserved.
//

#ifndef EVPCMHelper_hpp
#define EVPCMHelper_hpp

#include <stdio.h>
#include "EVProcessEnumType.h"

void convertFloatToByte(const float *inputData, size_t inputLen, char *outputData, size_t* outputLen, MODE mode);
void convertFloatToShort(const float *inputData, size_t inputLen, short *outputData, size_t *outputLen, MODE mode);
void convertFloatToByte_AEC(const float *inputData, size_t inputLen, char *outputData, size_t* outputLen);
void convertByteToFloat(const char *inputData, size_t inputLen, float *outputData, size_t* outputLen, MODE mode);
void maxVolum(short *audioData, size_t len, int strongValue);

#endif /* EVPCMHelper_hpp */
