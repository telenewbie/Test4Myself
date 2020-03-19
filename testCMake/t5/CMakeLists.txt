cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(t5)

# 通过STRINGS 来取值，可以组成list，如果是READ则是一陀一起返回
file(STRINGS tmp.txt out)

foreach(item ${out})
    message(STATUS "xxx" ${item})
endforeach()

file(READ tmp.txt out_read)
foreach(item ${out_read})
    message(STATUS "-----" ${item})
endforeach()

## 再次测试

execute_process(COMMAND find ${CMAKE_SOURCE_DIR}  -name *.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_FILE tmp1.txt
    )
file(STRINGS tmp1.txt PROTO_HEADERS)
foreach(item ${PROTO_HEADERS})
    message(STATUS ">>>>>>>>>>>>" ${item})
endforeach()
file(READ tmp1.txt read_file_content)
foreach(item ${read_file_content})
    message(STATUS "<<<<<<<<<<<" ${item})
endforeach()

# 输出多行文本并输出
execute_process(COMMAND find ${CMAKE_SOURCE_DIR}  -name *.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE output_content
    )

foreach(item ${output_content})
    message(STATUS "++++++++" ${item})
endforeach()

# 测试aux_source_dir 这种在不修改 CMakeLists的情况下是否会重新编译

message(status "CMAKE_COMMAND:${CMAKE_COMMAND}")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} source)

message(STATUS "souce:" ${source})

add_executable(t5 ${source})

file(GLOB file_sources *.cpp)

add_executable(tmy EXCLUDE_FROM_ALL ${file_sources})
LIST(APPEND SOURCE app_head.pb.c)
add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/${SOURCE}
    COMMAND ls -al
    COMMENT "h -----------------------"
    )
add_executable(test_add_custom_command EXCLUDE_FROM_ALL ${SOURCE})
# 如果关联一个不存在的target会怎样
add_custom_target(dep_${PROJECT_NAME}
    COMMAND ls -l
    COMMENT "******************"
    )
macro(dep_install)
    add_dependencies(${PROJECT_NAME} dep_${PROJECT_NAME})
endmacro()
dep_install()
