#ifndef LOG_H
#define LOG_H

#define LOG_RELEASE
#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#include <process.h>
#ifdef LOG_RELEASE
#include <stdio.h>
extern FILE* log_fp;
#else
#ifdef QT_CORE_LIB
#include <QDebug>
#define fprintf(a,...) qDebug(##__VA_ARGS__)
#endif
#define  log_fp stdout
#endif

#define LOGD(_fmt, ...)  do {fprintf(log_fp,"[D][NEWBIE][%u][%u] " _fmt " [%d]\n",(uint32_t)GetCurrentProcessId(),(uint32_t)GetCurrentThreadId(),##__VA_ARGS__, __LINE__);fflush(log_fp);} while(0)
#define LOGI(_fmt, ...)  do {fprintf(log_fp,"[I][NEWBIE][%u][%u] " _fmt " [%d]\n",(uint32_t)GetCurrentProcessId(),(uint32_t)GetCurrentThreadId(),##__VA_ARGS__, __LINE__);fflush(log_fp);} while(0)
#define LOGW(_fmt, ...)  do {fprintf(log_fp,"[W][NEWBIE][%u][%u] " _fmt " [%d]\n",(uint32_t)GetCurrentProcessId(),(uint32_t)GetCurrentThreadId(),##__VA_ARGS__, __LINE__);fflush(log_fp);} while(0)
#define LOGE(_fmt, ...)  do {fprintf(log_fp,"[E][NEWBIE][%u][%u] " _fmt " [%d]\n",(uint32_t)GetCurrentProcessId(),(uint32_t)GetCurrentThreadId(),##__VA_ARGS__, __LINE__);fflush(log_fp);} while(0)

#else
#include <unistd.h>
#include <sys/syscall.h>
#define gettid(_args...) syscall(__NR_gettid)
#define access(_args...) syscall(__NR_access, ##_args)
#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>

#define LOGD(_fmt, _args...)  do {fprintf(stdout, "[D][NEWBIE][%u][%u] " _fmt " [%d]\n",(uint32_t)getpid(),(uint32_t)gettid(),##_args, __LINE__);fflush(stdout);} while(0)
#define LOGW(_fmt, _args...)  do {fprintf(stdout, "[W][NEWBIE][%u][%u] " _fmt " [%d]\n",(uint32_t)getpid(),(uint32_t)gettid(),##_args, __LINE__);fflush(stdout);} while(0)
#define LOGE(_fmt, _args...)  do {fprintf(stderr, "[E][NEWBIE][%u][%u] " _fmt " [%d]\n",(uint32_t)getpid(),(uint32_t)gettid(),##_args, __LINE__);fflush(stderr);} while(0)
#endif //


#endif // LOG_H
