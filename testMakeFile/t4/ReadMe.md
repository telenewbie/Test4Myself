这篇主要是为了 

## 尝试 头文件 修改 如何才能 使源文件 跟随重新编译

Makefile 里面要指定：

```
%.o:%.c
${CC} -c $< -o %@ -MD -MF"%@.d" -MT $@
```
大致的意思就是 我们需要未每一个 test.o 关联所有的头文件以及源文件 例如
```
test.o:defs.h
test.o:test.c
test.o:stdio.h
```
如此我们才能在某一个文件改变后，导致整个的文件都重新编译

#### cmake
在cmake中是怎么指定的呢？

经过实验发现，原来 cmake 已经自动做好这些关联，不需要额外指定 任何的参数即可导致重新编译。

## 2. 如何让某个文件一致重新编译
在Makefile 里面 和cmake 的原理都是一样的，都是让某个目标项依赖鱼另一个文件，而这个文件却又不生成，导致每次都需要重新编译

Makefile
```
test.c:FORCE
FORCE:
```
CMakeLists.txt
```
add_custome_COMMAND(OUTPUT A COMMAND echo "")
add_executable(${PROJECT_NAME} test.c)
add_dependencies(${PROJECT_NAME} A)
```
如此即可保证每次都重新编译指定的文件

