cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
include(CheckCCompilerFlag)
project(t7)

#需要导入cmake自带的包 ChekckCCompilerFlag
foreach(_CCOPT -pedantic -Wall)
    check_c_compiler_flag(${_CCOPT} OPT${_CCOPT})
    message(STATUS "check flags ${_CCOPT}")
    if(OPT${_CCOPT})
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_CCOPT}")
    endif()
endforeach()

include(CMakeDependentOption)
option(ENABLE_ARES "Set to ON to enable c-ares support" ON)
# 对GUI隐藏该选项，当然如果加了-A 则隐藏不了
mark_as_advanced(ENABLE_ARES)
# 条件判断不成立则用后面的值，成立则用前面的值
cmake_dependent_option(ENABLE_THREADED_RESOLVER "Set to ON to enable threaded DNS lookup"
    ON "NOT ENABLE_ARES"
    OFF)
message(STATUS ${ENABLE_THREADED_RESOLVER})

option(ENABLE_IPV6 "Define if you want to enable IPv6 support" ON)
mark_as_advanced(ENABLE_IPV6)
if(ENABLE_IPV6 AND NOT WIN32)
    include(CheckStructHasMember)
    message(STATUS "check 1")
    # 判断某个结构体是否含有指定的函数
    check_struct_has_member("struct sockaddr_in6" sin6_addr "netinet/in.h"
        HAVE_SOCKADDR_IN6_SIN6_ADDR)
    check_struct_has_member("struct sockaddr_in6" sin6_scope_id "netinet/in.h"
        HAVE_SOCKADDR_IN6_SIN6_SCOPE_ID)
    if(NOT HAVE_SOCKADDR_IN6_SIN6_ADDR)
        message(WARNING "struct sockaddr_in6 not available, disabling IPv6 support")
        # Force the feature off as this name is used as guard macro...
        set(ENABLE_IPV6 OFF
            CACHE BOOL "Define if you want to enable IPv6 support" FORCE)
    endif()
endif()

find_package(Threads REQUIRED)
# todo:这个变量为何是1？
message(STATUS " pthread: ${CMAKE_USE_PTHREADS_INIT}")

include(CheckLibraryExists)

macro(check_library_exists_concat LIBRARY SYMBOL VARIABLE)
    check_library_exists("${LIBRARY};${CURL_LIBS}" ${SYMBOL} "${CMAKE_LIBRARY_PATH}"
        ${VARIABLE})
    if(${VARIABLE})
        set(CURL_LIBS ${LIBRARY} ${CURL_LIBS})
    endif()
endmacro()
check_library_exists_concat("dl"     dlopen       HAVE_LIBDL)
message(STATUS "have_libdl" ${HAVE_LIBDL})

find_package(OpenSSL)
if(OpenSSL_FOUND)
    message(STATUS "find it")
else()
    message(STATUS "not found it")
endif()
message(STATUS "-------------> ${PROJECT_NAME}_SELF" @${PROJECT_NAME}_SELF@)
