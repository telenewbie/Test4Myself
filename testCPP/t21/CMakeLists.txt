
set(SRC main.cpp MyWriter.cpp MyWriter.h MyReader.cpp MyReader.h)
add_executable(t20 ${SRC})
target_link_libraries(t20 PUBLIC pthread)