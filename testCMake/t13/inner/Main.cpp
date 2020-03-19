// #include "out.h"
#include "hello.h"
#include <iostream>
#include "mid.h"
void testout()
{
    std::cout << "testout" << std::endl;
}
int main()
{
    hello_public();
    // hello_private();
    testout();
    mid();

    return 0;
}