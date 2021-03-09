
aux_source_directory(B BBB)
aux_source_directory(A AAA)
set(SRC main.cpp  ${AAA} ${BBB})

include_directories(A B)

add_executable(t22 ${SRC})