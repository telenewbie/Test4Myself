#include <stdio.h>

class A{
    protected:
        A(){
            printf("hello A\n");
        }
        virtual ~A(){};
        int a  =10;
};

class B:public A{
    public :
        B():A(){
            printf("hello B ,A::a = %d\n", a);

        }
};

int main (int argc ,char* argv []){
    B b;
    printf("b.a = %d\n",b.a);
}
