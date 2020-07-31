CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
project(test)
message(STATUS "CMAKE_CURRENT_BINARY_DIR:${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "curl_include:${CMAKE_CURRENT_SOURCE_DIR}/curl/")
message(STATUS "cmakeoutputdir:${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log")
#[[
add_definitions(BUILDING_LIBCURL)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/curl/)
link_directories("D:/workspace/github/curl-7.71.1/build_win32_nmake/lib")
add_executable(${PROJECT_NAME} test.c)

target_link_libraries(${PROJECT_NAME} libcurl-d_imp)
#]]

set(TRY_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/curl")

set(TRY_LINK_PATH "D:/workspace/github/curl-7.71.1/build_win32_nmake/lib")
set(TRY_LINK_NAME "libcurl-d_imp")

try_compile(HAVE_TEST_1
	${CMAKE_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/test.c 
	#COMPILE_DEFINITIONS -DABC -DBUILDING_LIBCURL
	
	# 头文件路径
	CMAKE_FLAGS	
	#-DINCLUDE_DIRECTORIES=${TRY_INCLUDE_PATH}
	-DINCLUDE_DIRECTORIES=${CMAKE_CURRENT_SOURCE_DIR}/curl
	-DLINK_DIRECTORIES=${TRY_LINK_PATH}
	-DLINK_LIBRARIES=${TRY_LINK_NAME}
	OUTPUT_VARIABLE OUTPUT
)
message(WARNING "OUTPUT:${OUTPUT}")
if(HAVE_TEST_1)
else()
message(FATAL_ERROR "you are wrong")
endif()
#[[
if(HAVE_TEST_1)
message(STATUS success)
	file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/a.log
	"Performing Curl Test ${HAVE_TEST_1} pass with the following output:/n"
	"${OUTPUT}/n")
else()
	file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/b.log
	"Performing Curl Test ${HAVE_TEST_1} failed with the following output:/n"
	"${OUTPUT}/n")
	message(FATAL_ERROR "you are wrong")
endif()
	  #message(STATUS "OUTPUT:${OUTPUT}")
#]]
      


#try_compile(HAVE_COMPILE 
#${CMAKE_CURRENT_BINARY_DIR} #${CMAKE_CURRENT_SOURCE_DIR}/try_compile.c 
#test1)



