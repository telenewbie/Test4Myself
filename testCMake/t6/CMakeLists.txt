project(demo)

cmake_minimum_required(VERSION 3.0)

message("----------> CMAKE_SOURCE_DIR:${CMAKE_SOURCE_DIR}")
message("----------> CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR}")

message("----------> CMAKE_BINARY_DIR:${CMAKE_BINARY_DIR}")
message("----------> CMAKE_CURRENT_BINARY_DIR:${CMAKE_CURRENT_BINARY_DIR}")
add_subdirectory(src)
