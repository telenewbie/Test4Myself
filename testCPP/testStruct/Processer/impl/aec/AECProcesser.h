//
// Created by Elevoc on 2021/1/8 0008.
//

#ifndef TEST_AECPROCESSER_H
#define TEST_AECPROCESSER_H


#include <Processer/BaseProcesser.h>
#include "modules/audio_processing/include/audio_processing.h"
#include "api/audio/echo_canceller3_factory.h"
#include "api/audio/audio_frame.h"
#include "EVDspProcess.h"
#include "TEST_CONSTANT.h"

// 使用webrtc 的AEC模块进行 处理
// 10ms 为一帧，需要输入 参考信号【远端】 和 麦克风信号【近端】 的原始信号
// 远端信号 和近端信号存在先后的因果关系， 只有远端播放出来，才会被近端采集到，所以同一时刻远端一定先于近端



class AECProcesser : public BaseProcesser {
    int getMsgIndex() override;

    void process(DataMsg*) override;

    ~AECProcesser();

public:
    std::string getTag() override;
    AECProcesser();

private:

    EVDspProcess dspProcess;
    short linear_out_buffer[480] = {0};
    short mRefBuffer[480] = {0};
    short mMicBuffer[480] = {0};
    bool mAecRunning = false;
};


#endif //TEST_AECPROCESSER_H
