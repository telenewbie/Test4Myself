/*
 * mem_operation.h
 *
 *  Created on: 2019年11月28日
 *      Author: MING
 */

#ifndef __MEM_OPERATION_H__
#define __MEM_OPERATION_H__
#include <stdint.h>
#include <string>

bool GetMemContent(
        void* startPtr,
        size_t offset,
        size_t len,
        std::string& content);

bool SetMemContent(
        void* startPtr,
        size_t offset,
        size_t len,
        const std::string& content);

#endif /* __MEM_OPERATION_H__ */
