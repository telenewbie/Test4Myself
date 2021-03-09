//
// Created by Elevoc on 2021/1/5 0005.
//

#ifndef TEST_MSGCREATOR_H
#define TEST_MSGCREATOR_H


#include "DataMsg.h"

class MsgCreator {

private:
    MsgCreator();

public:
    static MsgCreator *getInstance();

    DataMsg *create(int len, int sampleRate, int channel, int bytesPerSample);

    void destroyMsg(DataMsg *);
};


#endif //TEST_MSGCREATOR_H
