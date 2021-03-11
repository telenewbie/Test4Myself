//
// Created by Elevoc on 2021/3/10 0010.
//

#include "DumpFileUtil.h"
#include <sstream>

DumpFileUtil::DumpFileUtil(const ProcessorConfig *cfg, const std::string &tag) {
    for (int i = 0; i < cfg->mMicChannel; ++i) {
#ifdef DEBUG_FILE
        std::ostringstream pathStream;
        pathStream << TEST_PCM_DIR << "mic_" << tag << i << ".pcm";
        fpMics[i] = std::fopen(pathStream.str().data(), "wb+");
#endif
    }
    for (int i = 0; i < cfg->mRefChannel; ++i) {
#ifdef DEBUG_FILE
        std::ostringstream pathStream;
        pathStream << TEST_PCM_DIR << "ref_" << tag << i << ".pcm";
        fpRefs[i] = std::fopen(pathStream.str().data(), "wb+");
#endif
    }
}

DumpFileUtil::~DumpFileUtil() {
#ifdef DEBUG_FILE
    for (auto &fpMic : fpMics) {
        if (fpMic) {
            fclose(fpMic);
            fpMic = nullptr;
        }
    }
    for (auto &fpRef : fpRefs) {
        if (fpRef) {
            fclose(fpRef);
            fpRef = nullptr;
        }
    }
#endif
}

void DumpFileUtil::writeRef(int channelIndex, const void *ptr, size_t size, size_t nmemb) {
#ifdef DEBUG_FILE
    std::fwrite(ptr, size, nmemb, fpRefs[channelIndex]);
#endif
}

void DumpFileUtil::writeMic(int channelIndex, const void *ptr, size_t size, size_t nmemb) {
#ifdef DEBUG_FILE
    std::fwrite(ptr, size, nmemb, fpMics[channelIndex]);
#endif
}

