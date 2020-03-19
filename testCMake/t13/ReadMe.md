
本用例主要测试 `target_link_libraries` 和 `target_include_directories` 里面的 `<INTERFACE|PUBLIC|PRIVATE>` 的作用

本例的结构： out other 为两个库 inner 为可执行文件 inner 引用了out other这两个库 other 也引用了out库

### out
编译出一个库 `libhello` ,注意使用 `PROJECT_NAME`[大写]
```
target_include_directories(${PROJECT_NAME} INTERFACE outHeader PRIVATE header)
```
表示自己只使用`header` 里面的头文件 ，而 其他导入本库的项目 只能使用到 `outHeader` 

### other
`other` 使用 `libhello` 库 ，需要导入`libhello`库里面的头文件

以前的导入方式时：
```cmake
target_include_directories(${PROJECT_NAME} PUBLIC ../out/outHeader)
```
经过测试应该这样用：
```cmake
target_link_libraries(${PROJECT_NAME} PUBLIC libhello)
```
这样 可以将 `libhello` 里面提供的头文件也导入到新的项目里面，不需要单独在导入头文件

### inner 可执行

`PUBLIC`: 将东西给自己用，别人也能用

`PRIVATE`: 将东西自己用，别人不能用

`INTERFACE`: 将东西自己不用，别人能用

`target_link_libraries(${PROJECT_NAME} PUBLIC libhello)` 
表示 连接 `libhello` 库 并且可以将`libhello`提供的头文件给到使用自己的人。

`target_include_directories(${PROJECT_NAME} INTERFACE outHeader PRIVATE header)` 
表示头文件的查找路径在`header`中查找 不在`outheader`中查找 但是提供给`outHeader` 。
