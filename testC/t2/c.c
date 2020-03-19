#include "c.h"

void testInnerC(){
    printf("hello Inner C\n");
}

__attribute__((visibility("default"))) void testC(){
    printf("hello C\n");
}
