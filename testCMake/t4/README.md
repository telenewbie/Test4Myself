##  查找指定文件在指定的目录
``find_path()``
## 查找库
``find_library()``
## 查找包
``find_package()``
## 通过`CMAKE_MODULE_PATH` 设置*.cmake 的查找路径
```
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
```
如果找不到则``<VAR>_NOTFOUND``会被设置

*.cmake 的命名规则是：`Find<package>.cmake` 区分大小写

*.cmake 会定义如下的变量
```
<NAME>_FOUND
<NAME>_INCLUDE_DIRS or <NAME>_INCLUDES
<NAME>_LIBRARIES or <NAME>_LIBRARIES or <NAME>_LIBS
<NAME>_DEFINITIONS
```

