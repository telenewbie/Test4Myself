//
// Created by Elevoc on 2021/3/11 0011.
//

#include "FrameSliceProcessor.h"
#include "DumpFileUtil.h"


FrameSliceProcessor::FrameSliceProcessor(const ProcessorConfig *cfg) : BaseProcesser(cfg) {
    init(cfg);

}

FrameSliceProcessor::FrameSliceProcessor(const FrameSliceProcessor &processor) : BaseProcesser(processor) {
    memcpy(m_slice_data, processor.m_slice_data, sizeof(processor.m_slice_data));
    memcpy(m_mic_index, processor.m_mic_index, sizeof(processor.m_mic_index));
    memcpy(m_ref_index, processor.m_ref_index, sizeof(processor.m_ref_index));
}

void FrameSliceProcessor::process(DataMsg *curMsg) {

    // 开始分片的处理
    for (int j = 0; j < CHANNEL_MIC; ++j) {
        int index = m_mic_index[j];
        if (index == -1) { // 表示按照 顺序执行
            index = j;
        }
        for (int i = 0; i < FRAME_SIZE_ONE; ++i) {
            m_slice_data[index * curMsg->sample_num + i] = curMsg->micBuff[i* curMsg->micChannel + j];
        }
#ifdef DEBUG_FILE
        dumpFile->write(DumpFileUtil::OUT_MIC,index, &m_slice_data[index * curMsg->sample_num], sizeof(TYPE_SAMPLE_t), curMsg->sample_num);
#endif
    }

    memcpy(curMsg->micBuff, m_slice_data, FRAME_SIZE_ONE * CHANNEL_MIC * sizeof(TYPE_SAMPLE_t));


    for (int j = 0; j < CHANNEL_REF; ++j) {
        int index = m_ref_index[j];
        if (index == -1) { // 表示按照 顺序执行
            index = j;
        }
        for (int i = 0; i < FRAME_SIZE_ONE; ++i) {
            m_slice_data[index * curMsg->sample_num + i] = curMsg->refBuff[i* curMsg->micChannel + j];
        }
#ifdef DEBUG_FILE
        dumpFile->write(DumpFileUtil::OUT_REF,index, &m_slice_data[index * curMsg->sample_num], sizeof(TYPE_SAMPLE_t), curMsg->sample_num);
#endif
    }
    memcpy(curMsg->refBuff, m_slice_data, FRAME_SIZE_ONE * CHANNEL_REF * sizeof(TYPE_SAMPLE_t));
}

void FrameSliceProcessor::init(const ProcessorConfig *cfg) {
    memcpy(m_mic_index, cfg->mMicIndex, sizeof(cfg->mMicIndex));
    memcpy(m_ref_index, cfg->mRefIndex, sizeof(cfg->mRefIndex));
#ifdef DEBUG_FILE
    dumpFile = new DumpFileUtil(cfg, "frame_slice_");
#endif
}

std::string FrameSliceProcessor::getTag() {
    return "FrameSliceProcessor";
}

bool FrameSliceProcessor::canProcess(DataMsg *) {
    return CHANNEL_MIC > 1;
}

FrameSliceProcessor::~FrameSliceProcessor() {
#ifdef DEBUG_FILE
    delete dumpFile;
#endif
}

