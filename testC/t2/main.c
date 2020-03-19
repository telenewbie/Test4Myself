#include "a.h"
#include "b.h"
#include "c.h"
#include "e.h"

int main(int argc,char* argv[])
{
    printf("hello main\n");
    testA();
    testB();
    testC();
    //testInnerC();
    testE();
    return 0;
}
