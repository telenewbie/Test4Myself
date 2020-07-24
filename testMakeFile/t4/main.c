#include "defs.h"
#include "test.h"
#include <stdio.h>
#include "app.h"

int main(int argc, char ** argv)
{
    printf("hello main %s:%s\n",HELLO,APP_TIME);
    testA();
    return 0;
}
