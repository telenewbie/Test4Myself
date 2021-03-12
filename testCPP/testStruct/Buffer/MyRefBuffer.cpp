//
// Created by Elevoc on 2021/1/8 0008.
//

#include "MyRefBuffer.h"

MyRefBuffer::MyRefBuffer() {

}

MyRefBuffer *MyRefBuffer::getInstance() {
    static auto *instance = new MyRefBuffer;
    return instance;
}

int MyRefBuffer::write(char *data, int len) {
    if (WriteSuccess == mInputBuffer.writeAudio(data, len)) {
        return len;
    } else {
        return -1;
    }
}

int MyRefBuffer::read(char *data, int len) {
    size_t readlen = len;
    mInputBuffer.readAudioWithoutWait(data, &readlen);
    if(len != readlen){
        memset(data,0,len-readlen);
        printf("read size not enough:[%d/%zu],Fill remaining capacity with 0!!!\n",len,readlen);
    }
    return readlen;
}
