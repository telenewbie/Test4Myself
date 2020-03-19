/*
 * so_run_operation.h
 *
 *  Created on: 2019年11月28日
 *      Author: MING
 */

#ifndef __SO_RUN_OPERATION_H__
#define __SO_RUN_OPERATION_H__

#include <string>
#include <iostream>
#include "txz_log.h"
#include <fstream>

bool GetRunSoPtr(
        const std::string& soName,
        void** prt)
{
    std::ifstream maps("/proc/self/maps");
    std::string soPtr = "";

    if (maps.is_open() == false) // 有该文件
    {
        LOGE("open /proc/self/maps error");
        return false;
    }

    std::string line = "";
    while (getline(maps, line)) // line中不包括每行的换行符
    {
        if (line.find(soName) != std::string::npos)
        {
            std::string::size_type pos = line.find("-");
            soPtr = line.substr(0, pos);
            break;
        }
    }
    if(soPtr.empty() == true)
    {
        LOGE("open /proc/self/maps error");
        return false;
    }

    sscanf(soPtr.c_str(), "%p", prt);
    return true;
}

#endif /* __SO_RUN_OPERATION_H__ */
