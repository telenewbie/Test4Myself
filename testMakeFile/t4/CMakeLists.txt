cmake_minimum_required(VERSION 3.7)

project (main)

set(TEST_FILE app.h)
add_custom_command(OUTPUT A COMMAND echo >/dev/null )
add_custom_command(OUTPUT ${TEST_FILE} 
	COMMAND echo "\\#ifndef _TEST" > ${TEST_FILE} 
	COMMAND echo "\\#define _TEST" >> ${TEST_FILE}
	#COMMAND echo "\\#define APP_TIME \\\\" >> ${TEST_FILE}
	COMMAND echo -n "\\#define APP_TIME \\\"" >> ${TEST_FILE}
	COMMAND echo -n ` date "+%Y-%m-%d %H:%M:%S" ` >> ${TEST_FILE}
	COMMAND echo "\\\"" >> ${TEST_FILE}
	COMMAND echo "\\#endif" >> ${TEST_FILE}
	COMMAND ${CMAKE_COMMAND} -E copy ${TEST_FILE} ${CMAKE_SOURCE_DIR}
	DEPENDS A
	COMMENT "This is a test1"
)

add_executable(${PROJECT_NAME} main.c test.c ${TEST_FILE})