cmake_minimum_required(VERSION 3.7)
SET(LIBS "")

# 测试 list append 的用法
list(APPEND VALUES "hello1")
list(APPEND VALUES "hello2")
list(APPEND VALUES "hello3")
# 测试 list 会遍历几次
foreach(item ${VALUES})
    message(STATUS "hello:" ${item})
endforeach()
message(STATUS "list:content:${VALUES}")

# 测试add_custom_command 是否会覆盖 结论是不会的
add_custom_target(hello ALL
    COMMAND echo "target output"
    )
foreach(item ${VALUES})
add_custom_command(TARGET hello
    POST_BUILD
    COMMAND echo ${item}
    COMMENT "start execute ${item}"
    )
endforeach()

# 测试将list的值赋值给变量在foreach会怎样
set(VALUE_ITEM ${VALUES})
message(STATUS "test3:" ${VALUE_ITEM})

foreach(item ${VALUE_ITEM})
    message(STATUS "test3:item:${item}")
endforeach()
## 那如果是宏呢？
macro(testValue values)
    foreach(item ${values})
        message(STATUS "test4:" ${item})
    endforeach()
endmacro(testValue)
testValue("${VALUES}") # 这里要加双引号才可以传递list到marco中

