cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# 这个测试：测试findpackage这些用法，以及自己创建一个XXX.cmake
project(test4)

# 指定find_package 库的查找路径
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_package(HELLO)

if(HELLO_FOUND)
    add_executable(hello main.cpp)
    include_directories(${HELLO_INCLUDE_DIR})
    target_link_libraries(hello ${HELLO_LIBRARY})
    #message(STATUS "find it")
else()
    message(STATUS "not find it ")
endif()


