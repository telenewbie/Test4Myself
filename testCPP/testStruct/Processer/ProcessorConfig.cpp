//
// Created by Elevoc on 2021/3/10 0010.
//

#include "ProcessorConfig.h"
#include <string>

ProcessorConfig::ProcessorConfig() {
    memset(mRefIndex, -1, sizeof(mRefIndex));
    memset(mMicIndex, -1, sizeof(mMicIndex));
}
