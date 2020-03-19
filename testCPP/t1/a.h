#include <iostream>


class A{
private:
    virtual void printA(){
        std::cout<<"hello A"<<std::endl;
    }
    virtual void printB(){
        std::cout<<"hello B"<<std::endl;
    }
    // 注意这里不是个虚函数
    void printC(){
        std::cout<<"hello C"<<std::endl;
    }
}

;
