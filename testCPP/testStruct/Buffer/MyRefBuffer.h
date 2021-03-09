//
// Created by Elevoc on 2021/1/8 0008.
//

#ifndef TEST_MYREFBUFFER_H
#define TEST_MYREFBUFFER_H

#include "EVByteQueue.h"

class MyRefBuffer {
private:
    MyRefBuffer();

    EVByteQueue mInputBuffer;
public:
    static MyRefBuffer *getInstance();

    int write(char *data, int len);

    int read(char *data, int len);

};


#endif //TEST_MYREFBUFFER_H
