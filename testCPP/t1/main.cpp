#include<iostream>
//这里注意下
#define private public
#include "a.h"
#include <functional>

typedef void (*fun)(void) ;

int main(int argc,char * argv[]){
    A a;
    //通过虚表来调用
    long* pa =(long*)&a;
    long* ppa = (long*)(*pa);
    void (*funA)(void) =(void (*)(void))(*ppa);
    funA();
    ((fun)*ppa)();

    fun funB =(fun)(*(++ppa));
    funB(); 

    //因为前面修改了可以访问的符号，所以这里可以直接调用
    a.printC();

   return 0;
}
