//
// Created by Elevoc on 2021/2/26 0026.
//

#include "HelloProcesser.h"
#include "ElevocLog.h"
int HelloProcesser::getMsgIndex() {
    return 0;
}

void HelloProcesser::process(DataMsg* msg) {
    LOGD("hello:%d",msg->index);
}
