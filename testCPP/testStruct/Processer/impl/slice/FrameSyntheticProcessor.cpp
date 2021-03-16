//
// Created by Elevoc on 2021/3/11 0011.
//

#include "FrameSyntheticProcessor.h"


FrameSyntheticProcessor::FrameSyntheticProcessor(const ProcessorConfig *cfg) : BaseProcesser(cfg) {
    memcpy(m_mic_index, cfg->mMicIndex, sizeof(cfg->mMicIndex));
    memcpy(m_ref_index, cfg->mRefIndex, sizeof(cfg->mRefIndex));
#ifdef DEBUG_FILE
    dumpFile = new DumpFileUtil(cfg, "frame_synthetic_");
#endif
}

FrameSyntheticProcessor::FrameSyntheticProcessor(const FrameSyntheticProcessor &processor) : BaseProcesser(processor) {
    memcpy(m_slice_data, processor.m_slice_data, sizeof(processor.m_slice_data));
    memcpy(m_mic_index, processor.m_mic_index, sizeof(processor.m_mic_index));
    memcpy(m_ref_index, processor.m_ref_index, sizeof(processor.m_ref_index));
}

FrameSyntheticProcessor::~FrameSyntheticProcessor() {
#ifdef DEBUG_FILE
    delete dumpFile;
#endif
}

FrameSyntheticProcessor &FrameSyntheticProcessor::operator=(const FrameSyntheticProcessor &processor) {
    memcpy(m_slice_data, processor.m_slice_data, sizeof(processor.m_slice_data));
    memcpy(m_mic_index, processor.m_mic_index, sizeof(processor.m_mic_index));
    memcpy(m_ref_index, processor.m_ref_index, sizeof(processor.m_ref_index));
    return *this;
}

void FrameSyntheticProcessor::process(DataMsg *curMsg) {
    // 开始分片的处理
    for (int j = 0; j < CHANNEL_MIC; ++j) {
        int index = m_mic_index[j];
        if (index == -1) { // 表示按照 顺序执行
            index = j;
        }
        for (int i = 0; i < FRAME_SIZE_ONE; ++i) {
            m_slice_data[i * curMsg->micChannel + j] = curMsg->micBuff[index * curMsg->sample_num + i];
        }
    }
#ifdef DEBUG_FILE
    dumpFile->write(DumpFileUtil::OUT_MIC, 0, m_slice_data, sizeof(TYPE_SAMPLE_t), FRAME_SIZE_ONE * CHANNEL_MIC);
#endif

    memcpy(curMsg->micBuff, m_slice_data, FRAME_SIZE_ONE * CHANNEL_MIC * sizeof(TYPE_SAMPLE_t));

}


std::string FrameSyntheticProcessor::getTag() {
    return "FrameSyntheticProcessor";
}

bool FrameSyntheticProcessor::canProcess(DataMsg *) {
    return CHANNEL_MIC > 1;
}
