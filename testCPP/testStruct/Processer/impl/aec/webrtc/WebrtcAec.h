//
// Created by Elevoc on 2021/3/11 0011.
//

#ifndef TEST1_WEBRTCAEC_H
#define TEST1_WEBRTCAEC_H

#include "BaseAECAdapter.h"
#include "EVDspProcess.h"
#include "TEST_CONSTANT.h"

namespace AEC {
    class WebrtcAec : public BaseAECAdapter {
    public:
        WebrtcAec(const AECConfig &);
        ~WebrtcAec();


        // 输入 远端数据
        int writeNearFrame(short *data, int frames, short *outData, int &outFrames, short *linearOut, int &
        linearFrames);

        int writeNearFrame(float *data, int frames, float *outData, int &outFrames, float *linearOut, int &
        linearFrames);


        // 输入 近端数据
        int writeFarFrame(short *data, int frames);

        int writeFarFrame(float *data, int frames);


        bool start() override;

        void release() override;

    private:
        EVDspProcess mDspProcess;
        // 只能是10ms 的数据
        short mTemp[FRAME_SIZE_ONE] = {0}; // 超过则有问题
        short mLinearAECOut[FRAME_SIZE_ONE] = {0};
    };
}


#endif //TEST1_WEBRTCAEC_H
