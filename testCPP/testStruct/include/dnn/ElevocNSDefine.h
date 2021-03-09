//
//  ElevocNSDefine.h
//  iosdnn
//
//  Created by csyyj on 2019/3/7.
//  Copyright © 2019 Elevoc. All rights reserved.
//

#ifndef ElevocNSDefine_h
#define ElevocNSDefine_h

typedef enum{
    NS_NOISE_DECAY_BAND_0_600HZ_DB = 0,         // (-60.0~0.0db)
    NS_NOISE_DECAY_BAND_600_2500HZ_DB,          // (-60.0~0.0db)
    NS_NOISE_DECAY_BAND_2500_5000HZ_DB,         // (-60.0~0.0db)
    NS_NOISE_DECAY_BAND_GREATE_THAN_5000HZ_DB,  // (-60.0~0.0db)
    NS_NOISE_SENSITIVITY_LB,               // (0.0~2.0)
    NS_NOISE_SENSITIVITY_HB,               // (0.0~2.0)
    NS_POST_NOISE_SENSITIVITY_LB,          // (0.0~2.0)
    NS_POST_NOISE_SENSITIVITY_HB           // (0.0~2.0)
}NSParamType;

#endif /* ElevocNSDefine_h */
