//
// Created by Elevoc on 2021/3/10 0010.
//

#ifndef TEST1_DUMPFILEUTIL_H
#define TEST1_DUMPFILEUTIL_H

#include "BaseProcesser.h"
#include <map>
#include <array>

class DumpFileUtil {
public:
    enum DUMP_TYPE {
        IN_MIC,
        IN_REF,
        OUT_MIC,
        OUT_REF
    };

public:
    DumpFileUtil() = delete;

    DumpFileUtil(const ProcessorConfig *, const std::string &);

    ~DumpFileUtil();

public:
    void write(DUMP_TYPE, int channelIndex, const void *ptr, size_t size, size_t nmemb);

//    void writeMic(int channelIndex, const void *ptr, size_t size, size_t nmemb);

private:
#ifdef DEBUG_FILE
    std::map<int, std::array<FILE *, MAX_CHANNEL>> fpMaps;
    std::array<FILE *, MAX_CHANNEL> fpOutMics = {0};
    std::array<FILE *, MAX_CHANNEL> fpOutRefs = {0};
    std::array<FILE *, MAX_CHANNEL> fpInMics = {0};
    std::array<FILE *, MAX_CHANNEL> fpInRefs = {0};
#endif
};


#endif //TEST1_DUMPFILEUTIL_H
