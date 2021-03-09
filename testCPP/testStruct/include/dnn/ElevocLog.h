
#ifndef Log_h
#define Log_h
#include "ElevocTypeDef.h"
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <jni.h>
#else
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void EVLogSetFileHandle(FILE* fp);
int EVLogEnabled(void);
void EVLogSetEnabled(int enable);
void EVLogSetTag(const char *tag);

void LOGV(const char *fmt, ...) ELEVOC_CHECK_FMT(1, 0);
void LOGI(const char *fmt, ...) ELEVOC_CHECK_FMT(1, 0);
void LOGD(const char *fmt, ...) ELEVOC_CHECK_FMT(1, 0);
void LOGE(const char *fmt, ...) ELEVOC_CHECK_FMT(1, 0);

#ifdef __cplusplus
}
#endif

#endif /* Log_h */
