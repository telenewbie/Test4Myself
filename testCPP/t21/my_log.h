/*
 * txz_log.h
 *
 *  Created on: 2019年11月27日
 *      Author: MING
 */

#ifndef __NEWBIE_LOG_H__
#define __NEWBIE_LOG_H__

#include <unistd.h>
//#include <sys/syscall.h>
//#define gettid(_args...) syscall(__NR_gettid)
//#define access(_args...) syscall(__NR_access, ##_args)
#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>

#define LOGD(_fmt, _args...)  do {fprintf(stdout, "[D][NEWBIE][%u][%u] " _fmt " [%s:%d]\n",(uint32_t)getpid(),(uint32_t)pthread_self(),##_args, __FILE__,__LINE__);fflush(stdout);} while(0)
#define LOGW(_fmt, _args...)  do {fprintf(stdout, "[W][NEWBIE][%u][%u] " _fmt " [%s:%d]\n",(uint32_t)getpid(),(uint32_t)pthread_self(),##_args, __FILE__,__LINE__);fflush(stdout);} while(0)
#define LOGI(_fmt, _args...)  do {fprintf(stdout, "[I][NEWBIE][%u][%u] " _fmt " [%s:%d]\n",(uint32_t)getpid(),(uint32_t)pthread_self(),##_args, __FILE__,__LINE__);fflush(stdout);} while(0)
#define LOGE(_fmt, _args...)  do {fprintf(stderr, "[E][NEWBIE][%u][%u] " _fmt " [%s:%d]\n",(uint32_t)getpid(),(uint32_t)pthread_self(),##_args, __FILE__,__LINE__);fflush(stderr);} while(0)


#endif /* __NEWBIE_LOG_H__ */
