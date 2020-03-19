#[==[
add_subdirectory(proto)
add_executable(demo main.cpp)
target_link_libraries(demo proto)
#]==]

#FILE(GLOB proto_files ${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto)

 # 找出所有的proto文件
  file(GLOB_RECURSE proto_files
           RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
                *.proto)

message(STATUS "----------------------proto_files:" ${proto_files})
FOREACH(proto_file ${proto_files})
    STRING(REGEX REPLACE "[^/]proto" "" proto_file_name ${proto_file})
    LIST(APPEND PROTO_SRCS "${proto_file_name}.pb.cc")
#[==[
    ADD_CUSTOM_COMMAND(
        OUTPUT "${proto_file_name}.pb.h" "${proto_file_name}.pb.cc"
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/protoc_64 --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/proto/ ${CMAKE_CURRENT_SOURCE_DIR}/${proto_file}
        DEPENDS ${proto_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT  "my::::::::::::::${CMAKE_CURRENT_SOURCE_DIR}"
        )
#]==]
ENDFOREACH(proto_file)
message(STATUS ${PROTO_SRCS})
include_directories(./proto)
include_directories(/home/txz/workspace/core/jni/lib3rd/protobuffer)
add_library(demo  ${PROTO_SRCS})
target_link_libraries(demo
    /disk/gitlab/protobuf_lite/build/lib/libprotobuf.a
    pthread)


message("----------> CMAKE_SOURCE_DIR:${CMAKE_SOURCE_DIR}")
message("----------> CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR}")
message("----------> CMAKE_BINARY_DIR:${CMAKE_BINARY_DIR}")
message("----------> CMAKE_CURRENT_BINARY_DIR:${CMAKE_CURRENT_BINARY_DIR}")
