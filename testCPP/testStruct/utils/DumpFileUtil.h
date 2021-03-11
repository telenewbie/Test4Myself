//
// Created by Elevoc on 2021/3/10 0010.
//

#ifndef TEST1_DUMPFILEUTIL_H
#define TEST1_DUMPFILEUTIL_H

#include "BaseProcesser.h"

class DumpFileUtil {

public:
    DumpFileUtil() = delete;

    DumpFileUtil(const ProcessorConfig *, const std::string &);

    ~DumpFileUtil();

public:
    void writeRef(int channelIndex, const void *ptr, size_t size, size_t nmemb);

    void writeMic(int channelIndex, const void *ptr, size_t size, size_t nmemb);

private:
#ifdef DEBUG_FILE
    FILE *fpMics[MAX_CHANNEL] = {0};
    FILE *fpRefs[MAX_CHANNEL] = {0};
#endif
};


#endif //TEST1_DUMPFILEUTIL_H
