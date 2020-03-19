#include <stdio.h>
#include <stdlib.h>
#include <iostream>
class A{
private :
    int a=0;
};

int main(){
    A a;
    int *pa = (int*)&a;
    printf("content:%d\n",*pa);
}
