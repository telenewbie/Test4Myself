//
// Created by Elevoc on 2021/1/6 0006.
//

#include "EngineProcesser.h"
#include "IEVProcessTask_c.h"


void EngineProcesser::process() {
    elevoc_write_audio(evtask, mCurMsg->buff, mCurMsg->len);
    bool isLast = false;
    elevoc_read_buffer(evtask, mCurMsg->buff, mCurMsg->len, &isLast, true);
}

EngineProcesser::EngineProcesser() {
    evtask = elevoc_new_evtask("1001045", "PP90oYgQASM75dky");
    elevoc_set_samplerate(evtask, 16000);
    elevoc_set_channels(evtask, 2);
    elevoc_start_task(evtask);
}
