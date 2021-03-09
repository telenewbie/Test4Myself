include(${CMAKE_SOURCE_DIR}/cmake/hello.cmake)
message(STATUS "child CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "child CMAKE_SOURCE_DIR:${CMAKE_SOURCE_DIR}")

file(GLOB_RECURSE LIB_SRCS "src/*.h" "src/*.cpp" "src/*.c" "src/*.qrc")

message(STATUS "GLOB_RECURSE:${LIB_SRCS}")