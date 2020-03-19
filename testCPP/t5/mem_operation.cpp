/*
 * mem_operation.cpp
 *
 *  Created on: 2019年11月28日
 *      Author: MING
 */

#include <stdint.h>
#include <string>
#include <string.h>
#include <sys/mman.h>
#include "txz_log.h"
bool GetMemContent(
        void* startPtr,
        size_t offset,
        size_t len,
        std::string& content)
{
    size_t nsize = (offset + len) / 4096 + ((offset + len) % 4096 == 0 ? 0 : 1);
    if (mprotect(startPtr, nsize, PROT_READ | PROT_EXEC | PROT_WRITE) != 0)
    {
        LOGE("mem privilege change failed");
        return false;
    }
    content = std::string((char*)startPtr + offset, len);
    return true;
}

bool SetMemContent(
        void* startPtr,
        size_t offset,
        size_t len,
        const std::string& content)
{
    size_t nsize = (offset + len) / 4096 + ((offset + len) % 4096 == 0 ? 0 : 1);
    if (mprotect(startPtr, nsize, PROT_READ | PROT_EXEC | PROT_WRITE) != 0)
    {
        LOGE("mem privilege change failed");
        return false;
    }
    memcpy((char*)startPtr + offset, &(content[0]), len);
    return true;
}

