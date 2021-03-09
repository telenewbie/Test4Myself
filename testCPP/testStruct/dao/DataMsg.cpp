//
// Created by Elevoc on 2021/1/5 0005.
//

#include <chrono>
#include "DataMsg.h"

uint32_t DataMsg::getId() {
// 当前时间戳
    return id;
}


static uint32_t g_id = 0;

DataMsg::DataMsg() {
    //init();
}

void DataMsg::init() {
    id = g_id++;
}
