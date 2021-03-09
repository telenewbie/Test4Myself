//
// Created by Elevoc on 2021/2/26 0026.
//

#ifndef TEST1_HELLOPROCESSER_H
#define TEST1_HELLOPROCESSER_H


#include "BaseProcesser.h"

class HelloProcesser: public BaseProcesser {
private:
    virtual void process(DataMsg*);

public:

    virtual int getMsgIndex();
};


#endif //TEST1_HELLOPROCESSER_H
