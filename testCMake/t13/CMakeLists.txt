cmake_minimum_required(VERSION 3.10)
project(t13)
add_subdirectory(out)
add_subdirectory(other)
add_executable(${PROJECT_NAME} 
    inner/Main.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC libhello PUBLIC mid)
target_include_directories(${PROJECT_NAME} PUBLIC inner) 