//
// Created by Elevoc on 2021/3/10 0010.
//

#include "DumpFileUtil.h"
#include <sstream>

DumpFileUtil::DumpFileUtil(const ProcessorConfig *cfg, const std::string &tag) {


    for (int i = 0; i < cfg->mMicChannel; ++i) {
#ifdef DEBUG_FILE
        std::ostringstream pathStream;
        pathStream << TEST_PCM_DIR << "mic_" << tag << "out_" << i << ".pcm";
        fpOutMics[i] = std::fopen(pathStream.str().data(), "wb+");
        pathStream.str("");
        pathStream.clear();
        pathStream << TEST_PCM_DIR << "mic_" << tag << "in_" << i << ".pcm";
        fpInMics[i] = std::fopen(pathStream.str().data(), "wb+");
#endif
    }
    for (int i = 0; i < cfg->mRefChannel; ++i) {
#ifdef DEBUG_FILE
        std::ostringstream pathStream;
        pathStream << TEST_PCM_DIR << "ref_" << tag << "out_" << i << ".pcm";
        fpOutRefs[i] = std::fopen(pathStream.str().data(), "wb+");
        pathStream.str("");
        pathStream.clear();
        pathStream << TEST_PCM_DIR << "ref_" << tag << "in_" << i << ".pcm";
        fpInRefs[i] = std::fopen(pathStream.str().data(), "wb+");
#endif
    }

    fpMaps.insert(std::make_pair(DUMP_TYPE::IN_MIC, fpInMics));
    fpMaps.insert(std::make_pair(DUMP_TYPE::IN_REF, fpInRefs));
    fpMaps.insert(std::make_pair(DUMP_TYPE::OUT_MIC, fpOutMics));
    fpMaps.insert(std::make_pair(DUMP_TYPE::OUT_REF, fpOutRefs));
}

DumpFileUtil::~DumpFileUtil() {
#ifdef DEBUG_FILE
    for (auto &item:fpMaps) {
        for (auto &fp:item.second) {
            if (fp) {
                fclose(fp);
                fp = nullptr;
            }
        }
    }
#endif
}

void DumpFileUtil::write(DUMP_TYPE type, int channelIndex, const void *ptr, size_t size, size_t nmemb) {
#ifdef DEBUG_FILE
    if (fpMaps.size() > type) {
        std::fwrite(ptr, size, nmemb, fpMaps[type][channelIndex]);
    } else {
        printf("dump file error:[%d/%zu]\n", type, fpMaps.size());
    }
#endif
}
