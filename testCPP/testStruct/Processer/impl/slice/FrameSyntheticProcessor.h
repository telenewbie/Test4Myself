//
// Created by Elevoc on 2021/3/11 0011.
//

#ifndef TEST1_FRAMESYNTHETICPROCESSOR_H
#define TEST1_FRAMESYNTHETICPROCESSOR_H

#include "BaseProcesser.h"
#include "DumpFileUtil.h"

class FrameSyntheticProcessor : public BaseProcesser {
private:
    void process(DataMsg *) override;

public:
    FrameSyntheticProcessor(const ProcessorConfig *cfg);

    FrameSyntheticProcessor(const FrameSyntheticProcessor &processor);

    ~FrameSyntheticProcessor();

    FrameSyntheticProcessor &operator=(const FrameSyntheticProcessor&);

    std::string getTag() override;

    int getMsgIndex() override;

    bool canProcess(DataMsg *) override;

private:
    TYPE_SAMPLE_t m_slice_data[FRAME_SIZE_ONE * CHANNEL_MIC] = {0};
    int m_mic_index[MAX_CHANNEL] = {0};
    int m_ref_index[MAX_CHANNEL];
#ifdef DEBUG_FILE
    DumpFileUtil *dumpFile = nullptr;
#endif
};


#endif //TEST1_FRAMESYNTHETICPROCESSOR_H
