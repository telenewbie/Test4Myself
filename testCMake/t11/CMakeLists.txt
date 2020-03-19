cmake_minimum_required(VERSION 3.7)
project(t11)

include(CheckTypeSize)
check_type_size( "long" LONG_INT  )
check_type_size( "long long" LONG_LONG_INT  )
check_type_size( "int" INT  )

set(MY_INSTALL_PREFIX -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR})

set(ANDROID_BUILD_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}"
    "-DCMAKE_TOOLCHAIN_FILE=/cross/github/jni/android-ndk-r14b/build/cmake/android.toolchain.cmake"
    "-DANDROID_TOOLCHAIN=clang"
    "-DANDROID_PLATFORM=android-21"
    "-DANDROID_ABI=armeabi-v7a"
    "-DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang")


# 全局编译参数
add_compile_options("-fPIC")


# 全局的CMAKE_ARGS
set(MY_CMAKE_ARGS
    # 需要指定安装路径，否则安装会报没有权限
    ${MY_INSTALL_PREFIX}
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    )
#todo : 这里可以使用try_compile 进行测试,是否可以使用某个参数
IF(CMAKE_COMPILER_IS_GNUCXX)
    # Needs fpic to link in shared lib on Linux with GCC
    LIST(APPEND MY_CMAKE_ARGS -DCMAKE_CXX_FLAGS=-fPIC)
    LIST(APPEND MY_CMAKE_ARGS -DCMAKE_C_FLAGS=-fPIC)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

if(ANDROID)
    LIST(APPEND MY_CMAKE_ARGS ${ANDROID_BUILD_ARGS};-fPIE)
endif()
if(BUILD_SHARED_LIBS)
    LIST(APPEND MY_CMAKE_ARGS -DBUILD_SHARED_LIBS=ON)
endif()
INCLUDE(ExternalProject)
message(STATUS "CMAKE_ARGS :" ${MY_CMAKE_ARGS})
 externalproject_add(
     openssl
     GIT_REPOSITORY https://github.com/telenewbie/openssl-cmake.git
     #URL ${CMAKE_SOURCE_DIR}/vendor/openssl-cmake-1.1.1.tar.gz
     #CONFIGURE_COMMAND ""
     CMAKE_COMMAND ${CMAKE_COMMAND}
     BUILD_COMMAND make
     PREFIX ${CMAKE_CURRENT_BINARY_DIR}
     UPDATE_DISCONNECTED 1
     CMAKE_ARGS
     ${MY_CMAKE_ARGS}
     #BUILD_IN_SOURCE 1
     INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}

     )
 # 需要自己为find_package 配置号参数
 #set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

 # 只需要指定OPENSSL_ROOT_DIR
 set(OPENSSL_ROOT_DIR ${CMAKE_BINARY_DIR})
 set(OPENSSL_USE_STATIC_LIBS TRUE)

 externalproject_add(
     curl
     # 当前目录的压缩包，可以变成远程链接
     #URL ${CMAKE_SOURCE_DIR}/vendor/curl-7.65.0.tar.gz
     GIT_REPOSITORY https://github.com/curl/curl.git
     #DEPENDS ${MY_DEPENDS}
     DEPENDS openssl
     #SOURCE_DIR ${CMAKE_BINARY_DIR}/curl-prefix/src/curl/lib
     #CONFIGURE_COMMAND  cmake -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR} -DBUILD_SHARED_LIBS=OFF
     CMAKE_ARGS
     ${MY_CMAKE_ARGS}
     -DOPENSSL_USE_STATIC_LIBS=TRUE
     -DOPENSSL_ROOT_DIR=${CMAKE_BINARY_DIR}
     -DBUILD_SHARED_LIBS=OFF
     -DBUILD_CURL_EXE=OFF
     -DBUILD_TESTING=OFF
     BUILD_COMMAND make
     )
 if(OPENSSL_FOUND)
     message(STATUS "find it " openssl)
 endif()
 add_executable(t11 main.c)
 include_directories(${OPENSSL_INCLUDE_DIR})
 target_link_libraries(t11 ${OPENSSL_LIBRARIES})
 add_dependencies(t11 curl)
 message(STATUS "----------------->>>${CMAKE_SYSROOT}")
 link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib
     ${CMAKE_SYSROOT}/usr/lib
     )
 add_library(mytest1 SHARED a.c)
 target_link_libraries(mytest1
     -Wl,--whole-archive
     -Wl,-Bstatic
     ssl
     crypto
     -Wl,-Bdynamic
     -Wl,--no-whole-archive
     )
