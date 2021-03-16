//
// Created by Elevoc on 2021/1/8 0008.
//

#ifndef TEST_AECPROCESSER_H
#define TEST_AECPROCESSER_H


#include <Processer/BaseProcesser.h>
#include "TEST_CONSTANT.h"
#include "BaseAECAdapter.h"
#include "TEST_CONSTANT.h"
#include "DumpFileUtil.h"

// 使用webrtc 的AEC模块进行 处理
// 10ms 为一帧，需要输入 参考信号【远端】 和 麦克风信号【近端】 的原始信号
// 远端信号 和近端信号存在先后的因果关系， 只有远端播放出来，才会被近端采集到，所以同一时刻远端一定先于近端



class AECProcesser : public BaseProcesser {

    void process(DataMsg *) override;

    ~AECProcesser();

public:
    std::string getTag() override;

    AECProcesser(const ProcessorConfig *cfg);

private:

    // 每个通道一个处理器
    std::array<BaseAECAdapter*,CHANNEL_MIC> mAecAdapters;
//    BaseAECAdapter *mAECAdapter = nullptr;
    TYPE_SAMPLE_t linear_out_buffer[CHANNEL_MIC * FRAME_SIZE_ONE] = {0};
//    TYPE_SAMPLE_t mRefBuffer[CHANNEL_REF * FRAME_SIZE_ONE + 1] = {0};
//    TYPE_SAMPLE_t mMicBuffer[CHANNEL_MIC * FRAME_SIZE_ONE] = {0};
    bool mAecRunning = false;

#ifdef DEBUG_FILE
    DumpFileUtil * dumpFile = nullptr;
#endif
};


#endif //TEST_AECPROCESSER_H
