//
// Created by Elevoc on 2021/3/10 0010.
//

#ifndef TEST1_BASEAECADAPTER_H
#define TEST1_BASEAECADAPTER_H

#include "AECConfig.h"

class BaseAECAdapter {
public:

public:
    BaseAECAdapter(const AECConfig &);

    // 输入 远端数据
    virtual int writeNearFrame(short *data, int frames, short *outData, int &outFrames, short *linearOut, int &
    linearFrames) = 0;

    virtual int writeNearFrame(float *data, int frames, float *outData, int &outFrames, float *linearOut, int &
    linearFrames) = 0;


    // 输入 近端数据
    virtual int writeFarFrame(short *data, int frames) = 0;

    virtual int writeFarFrame(float *data, int frames) = 0;

    virtual bool start() = 0;

    virtual void release() = 0;

    virtual ~BaseAECAdapter();

protected:

    AECConfig mAecConfig;

};


#endif //TEST1_BASEAECADAPTER_H
