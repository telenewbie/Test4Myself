//
// Created by Elevoc on 2021/3/11 0011.
//

#ifndef TEST1_FRAMESLICEPROCESSOR_H
#define TEST1_FRAMESLICEPROCESSOR_H

#include "BaseProcesser.h"
#include "DumpFileUtil.h"

// 分片处理器， 将LRLR 搞成 LLRR ，方便后期的处理
class FrameSliceProcessor : public BaseProcesser {

private:
    void process(DataMsg *) override;

    void init(const ProcessorConfig *);

public:
    FrameSliceProcessor(const ProcessorConfig *);

    FrameSliceProcessor(const FrameSliceProcessor &);
    ~FrameSliceProcessor();

    std::string getTag() override;


    bool canProcess(DataMsg *) override;

private:
    // 预定义 可以容纳 的空间
    TYPE_SAMPLE_t m_slice_data[FRAME_SIZE_ONE * CHANNEL_MIC] = {0};
    int m_mic_index[MAX_CHANNEL] = {0};
    int m_ref_index[MAX_CHANNEL];
#ifdef DEBUG_FILE
    DumpFileUtil *dumpFile = nullptr;
#endif
};


#endif //TEST1_FRAMESLICEPROCESSOR_H
