#include "b.h"
#include "a.h"
extern "C" void testB(){
    printf("hello B \n");
    testA();
}
