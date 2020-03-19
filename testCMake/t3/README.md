通过：

联想：
通过这个可以自动生成好多的东西，比方说在cmake中获取的一些变量可以直接输出到某个文件中
通过 `configure_file`

通过``@ONLY`` 使输入的文件只认@VAR@ 否则也会替换${VAR} 

<input>输入文件 需要 #define 或者#cmakedefine 来声明
```C
#define TUTORIAL_VARSION_MAJOR @TUTORIAL_VARSION_MAJOR@
#define TUTORIAL_VERSION_MAJOR @TUTORIAL_VERSION_MAJOR@
// ${VAR} 这样的必须要用 #cmakedefine 才会替换
#define TUTORIAL_VERSION_MINOR ${TUTORIAL_VERSION_MINOR}
// 如果加了 @ONLY 则这里不会进行替换
#cmakedefine TEST_VAR ${TEST_VAR}
// 这里可以证明，宏名必须和需要替换的名称一致，否则有问题
#cmakedefine TTT @TEST_VAR@
// 如果是define 则宏名和替换的名称可以不一致
#define TTTDefine @TEST_VAR@

```

# 测试


测试 `set` 和 `option` 同时存在的情况下，变量会使用哪个
```
set(PCRE_BUILD_PCRECPP OFF)
OPTION(PCRE_BUILD_PCRECPP "Build the PCRE C++ library (pcrecpp)."    ON)
```
在这个例子里面会显示 `ON`

```
set(PCRE_BUILD_PCRECPP OFF)
 set(PCRE_BUILD_PCRECPP ON CACHE BOOL "xxx")
```
那这个例子呢？

结果是：``ON``
```
 set(PCRE_BUILD_PCRECPP OFF CACHE BOOL "xxx")
  OPTION(PCRE_BUILD_PCRECPP "Build the PCRE C++ library (pcrecpp)."    ON)
```
那这个例子呢？

结果是：``OFF``
