/**
* 对so 里面的elf 进行加密和解密
* 原理： 
* 1. 通过readelf -S xxx 查找 .text 段的地址 =add_1
* 2. 通过objdump -d -s xxx 找到 函数的地址 = add_2
* 3. 找到函数截至的位置地址 = add_3
* 4. 读取文件指定位置开始的长度(add_3 - add_2) 的长度 ，char[] value;
* 5. 进行异或加密
* 6. 再进行覆盖写
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <algorithm>
#include <iostream>
#include <string.h>
#include "cryption_addr.h"

#define TXZ_LOGD(_fmt, _args...)                                                           \
    do                                                                                     \
    {                                                                                      \
        fprintf(stderr, "[D][TXZMusic] " _fmt " [%s][%d]\n", ##_args, __FILE__, __LINE__); \
        fflush(stderr);                                                                    \
    } while (0);
#define TXZ_LOGW(_fmt, _args...)                                                           \
    do                                                                                     \
    {                                                                                      \
        fprintf(stderr, "[W][TXZMusic] " _fmt " [%s][%d]\n", ##_args, __FILE__, __LINE__); \
        fflush(stderr);                                                                    \
    } while (0);
#define TXZ_LOGE(_fmt, _args...)                                                           \
    do                                                                                     \
    {                                                                                      \
        fprintf(stderr, "[E][TXZMusic] " _fmt " [%s][%d]\n", ##_args, __FILE__, __LINE__); \
        fflush(stderr);                                                                    \
    } while (0);

typedef std::shared_ptr<FILE> fileptr;

void encryption(uint32_t start_, uint32_t end_, char *helloValue)
{
    int int_s = sizeof(uint32_t);
    int a[5] = {0, 1, 2, 3, 0};
    int index = a[0];
    int len = end_ - start_ + 1;
    // for(int i = 0; i<(len)/int_s;++i)
    // {
    //      for (int j = 0; j < int_s; j++) // 循环跳过解密
    //     {
    //         if (j == index) // 循环跳过加密
    //             continue;
    //         if (j == 0)
    //         {
    //             helloValue[start_+ i*len + j] ^= (((start_ & 0xFF00)) >> 8);
    //         }
    //         else if (j == 1)
    //         {
    //             helloValue[start_ + i*len + j] ^= ((start_ & 0xFF));
    //         }
    //         else if (j == 2)
    //         {
    //             helloValue[start_ + i*len + j] ^= (((end_ & 0xFF00)) >> 8);
    //         }
    //         else if (j == 3)
    //         {
    //             helloValue[start_ + i*len + j] ^= ((end_ & 0xFF));
    //         }
    //     }
    //     index = a[index + 1];
    // }

    uint32_t end_end = end_ % int_s;

    for (int i = start_; i < end_ - end_end; i += int_s) // 全闭区间 ，int的大小加密
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

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        TXZ_LOGE("you must give me two parameters!!!");
        return -1;
    }

    // 全部数据读取到内存，然后再 进行修改，再写回文件

    FILE *r_fd = std::fopen(argv[1], "r");
    if (!r_fd)
    {
        TXZ_LOGE("can't open %s", argv[1]);
        return -1;
    }

    std::fseek(r_fd, 0, SEEK_END);
    int length = std::ftell(r_fd);

    TXZ_LOGD("length:%d", length);

    char *data = (char *)malloc(length);

    std::fseek(r_fd, 0, SEEK_SET);

    int read_size = std::fread(data, 1, length, r_fd);
    TXZ_LOGD("read size ::%d", read_size);

    std::fclose(r_fd);

    //加密部分数据

    uint32_t array_size = sizeof(cryption_addr_array) / sizeof(cryption_addr_array[0]);
    for (int i = 0; i < array_size; ++i)
    {
        TXZ_LOGD("start encryption %u,%u", cryption_addr_array[i].start_index, cryption_addr_array[i].end_index);
        encryption(cryption_addr_array[i].start_index, cryption_addr_array[i].end_index, data);
    }

    // int int_s = sizeof(uint32_t);
    // int a[5] = {0, 1, 2, 3, 0};
    // int index = a[0];
    // for (int i = START_INDEX; i <= END_INDEX; i += int_s) // 全闭区间 ，int的大小加密
    // {
    //     for (int j = 0; j < int_s; j++) // 循环跳过加密
    //     {
    //         if (j == index) // 循环跳过加密
    //             continue;
    //         if (j == 0)
    //         {
    //             data[i + j] ^= (((START_INDEX & 0xFF00)) >> 8);
    //         }
    //         else if (j == 1)
    //         {
    //             data[i + j] ^= ((START_INDEX & 0xFF));
    //         }
    //         else if (j == 2)
    //         {
    //             data[i + j] ^= (((END_INDEX & 0xFF00)) >> 8);
    //         }
    //         else if (j == 3)
    //         {
    //             data[i + j] ^= ((END_INDEX & 0xFF));
    //         }
    //     }
    //     index = a[index + 1];
    // }
    TXZ_LOGD("加密完成");

    // 写数据
    FILE *w_fd = std::fopen(argv[1], "w");
    int write_size = fwrite(data, 1, length, w_fd);
    TXZ_LOGD("write size ::%d", write_size);

    std::fclose(w_fd);
    free(data);
    return 0;
}
