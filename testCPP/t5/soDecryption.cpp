// #include "hello.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "so_run_operation.h"
#include "mem_operation.h"
#include <unistd.h>

//#define ENCYPTION
typedef void (*FP)(void);
#ifdef ENCYPTION
#define CALL_METHOD "helloB"
#define CALL_METHOD_C "helloC"
#else
#include "helloso/hello.h"
#endif

#include "cryption_addr.h"

// #define SONAME "libtxz.so"
#define SONAME "libhello.so"

void decryption(uint32_t start_, uint32_t end_, std::string &helloValue)
{
    int int_s = sizeof(uint32_t);
    int a[5] = {0, 1, 2, 3, 0};
    int index = a[0];
    int len = end_ - start_ + 1;

    uint32_t end_end = len % int_s;

    for (int i = 0; i < len - end_end; i += int_s) // 全闭区间 ，int的大小加密
    {
        for (int j = 0; j < int_s; j++) // 循环跳过解密
        {
            if (j == index) // 循环跳过加密
                continue;
            if (j == 0)
            {
                helloValue[i + j] ^= (((start_ & 0xFF00)) >> 8);
            }
            else if (j == 1)
            {
                helloValue[i + j] ^= ((start_ & 0xFF));
            }
            else if (j == 2)
            {
                helloValue[i + j] ^= (((end_ & 0xFF00)) >> 8);
            }
            else if (j == 3)
            {
                helloValue[i + j] ^= ((end_ & 0xFF));
            }
        }
        index = a[index + 1];
    }
}

void call(void *handler, std::string &&method)
{
    printf("调用 %s\n", method.data());
    FP p = (FP)dlsym(handler, method.data());

    if (p == nullptr)
    {
        printf("cant find symbol  :%s\n", method.data());
        return;
    }
    p();
}

static void so_init(void);

void so_init(void)
{
    // printf("call so init.\n");
#ifndef NONDECRYPTION
    void *helloSo;

    bool ret = GetRunSoPtr(SONAME, &helloSo);

    printf("GetRunSoPtr %d[%s]\n", ret, __FUNCTION__);
    if (ret == 0)
    {
        void *handle = dlopen(SONAME, RTLD_NOW);
        if (!handle)
        {
            printf("dlopen error\n");
            return;
        }
        bool ret = GetRunSoPtr(SONAME, &helloSo);

        printf("GetRunSoPtr %d[%s]\n", ret, __FUNCTION__);
    }
    // // 解密流程
    uint32_t array_size = sizeof(cryption_addr_array) / sizeof(cryption_addr_array[0]);
    for (int i = 0; i < array_size; ++i)
    {
        std::string helloValue;
        uint32_t start_index = cryption_addr_array[i].start_index;
        uint32_t end_index = cryption_addr_array[i].end_index;

        printf("decryption %u,%u \n", start_index, end_index);
        int content_len = end_index - start_index + 1;
        printf("content len = %d\n", content_len);
        ret = GetMemContent(helloSo, start_index, content_len, helloValue);
        printf("GetMemContent %d\n", ret);

        printf("start decryption %x %x %x %x\n", helloValue[0], helloValue[0 + 1], helloValue[0 + 2], helloValue[0 + 3]);
        decryption(start_index, end_index, helloValue);
        // decryption(helloValue);
        // /////////
        printf("end decryption %x %x %x %x\n", helloValue[0], helloValue[0 + 1], helloValue[0 + 2], helloValue[0 + 3]);
        ret = SetMemContent(helloSo, start_index, content_len, helloValue);
        printf("SetMemContent %d\n", ret);
    }
#endif
}

int main(int argc, char **argv)
{

    if (argc < 3)
    {
        printf("you must give me 3 parameters!!!\n");
        return -1;
    }

    std::string soName_fullname = argv[1];
    printf("soName_fullname %s\n", soName_fullname.data());
    std::string type = argv[2]; // 读取的方式
    printf("type %s\n", type.data());
    std::string::size_type index = soName_fullname.find_last_of("/");
    printf("size %d\n", index);
    std::string soName = soName_fullname.substr(index + 1, soName_fullname.length() - index);
    printf("soName %s\n", soName.data());
    // 这里需要用dlopen
#ifdef ENCYPTION
    void *handle = dlopen(soName_fullname.data(), RTLD_NOW);

    if (!handle)
    {
        printf("dlopen error\n");
        return -1;
    }
    so_init();
#endif
    // if (type == "1")
    // {
    //     void *helloSo;

    //     bool ret = GetRunSoPtr(soName.data(), &helloSo);

    //     printf("GetRunSoPtr %d\n", ret);

    //     std::string helloValue;

    //     int content_len = END_INDEX - START_INDEX + 1;
    //     printf("content len = %d\n", content_len);
    //     ret = GetMemContent(helloSo, START_INDEX, content_len, helloValue);
    //     printf("GetMemContent %d\n", ret);

    //     printf("start decryption %x %x %x %x\n", helloValue[0], helloValue[0 + 1], helloValue[0 + 2], helloValue[0 + 3]);
    //     // 解密流程
    //     decryption(helloValue);
    //     /////////
    //     printf("end decryption %x %x %x %x\n", helloValue[0], helloValue[0 + 1], helloValue[0 + 2], helloValue[0 + 3]);

    //     ret = SetMemContent(helloSo, START_INDEX, content_len, helloValue);
    //     printf("SetMemContent %d\n", ret);
    // }
    // else if (type == "2")
    // {
    //     printf("直接调用，不用解密\n");
    // }
    // else
    // {
    // }
#ifdef ENCYPTION

    call(handle, CALL_METHOD);
    call(handle, CALL_METHOD_C);

    dlclose(handle);
#else
    helloB();
    helloC();
#endif

    // hello();

    return 0;
}