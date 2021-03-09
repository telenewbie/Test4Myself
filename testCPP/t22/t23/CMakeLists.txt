
#aux_source_directory(B BBB)
#aux_source_directory(A AAA)

add_subdirectory(A)
add_subdirectory(B)

set(SRC main.cpp  #[[${AAA} ${BBB}]])

include_directories(A B)

add_executable(t22 ${SRC})
target_link_libraries(t22 b)

