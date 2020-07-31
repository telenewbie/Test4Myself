CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
project(test)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(include)

file(GLOB MY_HEADER1 ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB MY_HEADER2 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB MY_HEADER3 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/build_vs/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/build_vs/*.h)
file(GLOB MY_HEADER4 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/build_vs/*.cc)
file(GLOB_RECURSE DATABASE_SRCS
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/build_vs/*.cc
         ${CMAKE_CURRENT_SOURCE_DIR}/build_vs/*.h)
source_group(test2    FILES ${DATABASE_SRCS})

source_group(header1 ${MY_HEADER1})
source_group(header2 ${MY_HEADER2})
source_group(header3 ${MY_HEADER3})
source_group(header4 ${MY_HEADER4})
message(STATUS "MY_HEADER2:${MY_HEADER2}")
message(STATUS "DATABASE_SRCS:${DATABASE_SRCS}")

set(LIB_NAME "A")
add_library(${LIB_NAME} SHARED testA.cpp)

add_executable(${PROJECT_NAME} test.c)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIB_NAME})

