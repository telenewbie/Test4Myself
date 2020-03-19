#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void test1()
    {
        printf("call test1.\n");
    }

    void helloA()
    {
        printf("%s\n", __FUNCTION__);
    }

    void helloB()
    {
        printf("%s\n", __FUNCTION__);
        printf("%s12234\n", __FUNCTION__);
        helloA();
    }
    void helloC()
    {
        printf("%s\n", __FUNCTION__);
        helloB();
    }

#ifdef __cplusplus
}
#endif