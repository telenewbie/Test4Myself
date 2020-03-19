 cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

 project(t8)
 # 在ANDROID 的编译环境下会有哪些参数被指定，分别是什么
 include(CheckIncludeFiles)
 include_directories(${ANDROID_NDK}/sysroot/usr/include)
 # 测试这个接口，会产生宏开关EVENT__HAVE_IFADDRS_H
 CHECK_INCLUDE_FILES("sys/types.h;ifaddrs.h" EVENT__HAVE_IFADDRS_H)
 add_executable(t8 main.cpp)
 message(STATUS "------> ANDROID_ARCH_NAME ${ANDROID_ARCH_NAME}")
 message(STATUS "------> ANDROID_SYSROOT ${ANDROID_SYSROOT}")
 message(STATUS "------> ANDROID_TOOLCHAIN ${ANDROID_TOOLCHAIN}")

