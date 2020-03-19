cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# 获取git的信息输入到文件中
# EXEC_PROGRAM vs EXECUTE_PROCESS program 会去掉末尾的回车换行而process不会
##[==[
execute_process(COMMAND git rev-parse --short HEAD
    OUTPUT_VARIABLE GIT_VERSION_COMMIT_SHORT
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
execute_process(COMMAND git rev-parse --abbrev-ref HEAD
    OUTPUT_VARIABLE GIT_BRANCH
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

execute_process(COMMAND git config user.name
    OUTPUT_VARIABLE GIT_USER_NAME
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

execute_process(COMMAND date +%G%m%d%H%M%S
    OUTPUT_VARIABLE GIT_CURRENT_TIME
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
# 如果一定要用execute_process 则需要再使用把文本最后的回车换行去掉，则两者效果相同
string(REGEX REPLACE "\n$" "" GIT_BRANCH "${GIT_BRANCH}")
string(REGEX REPLACE "\n$" "" GIT_VERSION_COMMIT_SHORT "${GIT_VERSION_COMMIT_SHORT}")
string(REGEX REPLACE "\n$" "" GIT_USER_NAME "${GIT_USER_NAME}")
string(REGEX REPLACE "\n$" "" GIT_CURRENT_TIME "${GIT_CURRENT_TIME}")

##]==]
#[==[
exec_program(git
    ARGS rev-parse --short HEAD
    OUTPUT_VARIABLE GIT_VERSION_COMMIT_SHORT
    )

exec_program(git
    ARGS rev-parse --abbrev-ref HEAD
    OUTPUT_VARIABLE GIT_BRANCH
    )
exec_program(git
    ARGS config user.name
    OUTPUT_VARIABLE GIT_USER_NAME
    )
exec_program(date
    ARGS +%G%m%d%H%M%S
    OUTPUT_VARIABLE GIT_CURRENT_TIME
    )
#]==]


file(WRITE tele_version.h "${GIT_CURRENT_TIME}_${GIT_USER_NAME}_${GIT_BRANCH}_${GIT_VERSION_COMMIT_SHORT}")

# file(READ) VS file(STRINGS) strings 会将每一行的回车换行去掉然后再赋值给变量
# 这里用STRINGS + execute_process 和READ + exec_program 读取出来的内容是一样的，虽然写入文件的数据不一样
file(STRINGS tele_version.h content)
message(STATUS "content:" ${content})


# 将会在cmakelists的同级目录生成文件hello
file(WRITE hello "version_id:${GIT_VERSION} \n")
file(APPEND hello "append content\n")
file(READ hello h)
message(STATUS "file_content:" ${h})
