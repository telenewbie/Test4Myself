# 这个引用了另外的一个库 cmake 中编译的

cmake_minimum_required(VERSION 3.10)
project(mid)

add_library(${PROJECT_NAME} 
    mid.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC libhello)
target_include_directories(${PROJECT_NAME} 
PUBLIC include
)