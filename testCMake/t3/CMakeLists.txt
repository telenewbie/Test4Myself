cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(test3)

# 这个文件是做config的测试
set (TUTORIAL_VERSION_MAJOR 1)
set (TUTORIAL_VERSION_MINOR 0)
set (TEST_VER 10)
# 这里要指定目录，否则不能生成？
## 如果加一个@ONLY 则只会替换<input>文件里面@VAR@ 而不会替换${VAR}
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/test.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/test.h"
    @ONLY
    )

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(test3 test.cpp)


set(PCRE_BUILD_PCRECPP OFF)
set(PCRE_BUILD_PCRECPP ON CACHE BOOL "xxx")
#OPTION(PCRE_BUILD_PCRECPP "Build the PCRE C++ library (pcrecpp)."    ON)

message("------------>PCRE_BUILD_PCRECPP:${PCRE_BUILD_PCRECPP}")

if(PCRE_BUILD_PCRECPP)
    message("-----> ON")
else()
    message("------> OFF")
endif()
