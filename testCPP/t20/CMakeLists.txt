
set(SRC main.cpp replace.cpp)

add_library(hook SHARED Hook.cpp)
target_link_libraries(hook PUBLIC z)

set(CMAKE_CXX_STANDARD 11)
add_executable(t19 ${SRC})

target_link_libraries(t19 PUBLIC hook "-Wl,-Bsymbolic" )
