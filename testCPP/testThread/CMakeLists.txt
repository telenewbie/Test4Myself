
set(SRC main.cpp resample.c)

set(CMAKE_CXX_STANDARD 11)
add_executable(resample ${SRC})
target_compile_definitions(resample PUBLIC -DOUTSIDE_SPEEX -DFLOATING_POINT)