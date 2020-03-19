# 续
说在前面，编译Linux版本真的很简单而编译Android 版本真的有点不简单

# 需求
使用`externalproject_add` 导入``openssl`` 和`curl` 进行编译，编译过程中发现

1. openssl 总是编译成为动态库，

>    解决方案：查看源码，发现原来tar包中，强制指定android版本为动态库，结果下载源码进行编译发现里面默认也会编译动态库，但是没有指定`shared`的参数，而是通过`BUILD_SHARED_LIBS`修改了默认值。这里进行了修改`-DBUILD_SHARED_LIBS=OFF`进行了关闭

2. ``externalproject_add`` 使用这个导入外部项目进行编译，那么使用`-DCMAKE_TOOLCHAIN_FILE=/cross/github/jni/android-ndk-r14b/build/cmake/android.toolchain.cmake` 进行指定的变量，就不能传递给到外部项目里面

> 解决方案：``openssl``本来不支持CMAKE的方式进行编译的。后面github上面发现有人写好了openssl的cmake的方式，这里就用他的进行编译，这时候就出现了其他的问题，`externalproject_add`不会用导致的一些问题。

3. `externalproject_add` 不会使用

> 这里简单说下：这个函数有两种编译方式，外部编译和内部编译：`BUILD_IN_SOURCE 1`如果用这个的话，可能导致一些问题，比方说``write()`` 就会有问题
>
> 可以通过``GIT``，``SVN``， ``CVS``，`HG`，`本地路径` 的方式进行下载
>
> 如果是网络请求可以通过 `UPDATE_DISCONNECTED 1` 来禁止更新，这个选项必须依赖网络的资源，如果是本地路径则报错
>
> 如果指定了`CONFIGURE_COMMAND` 则`CMAKE_ARGS` 不起作用，所以如果要使用cmake进行编译，则不能指定``CONFIGURE_COMMAND``这个参数，否则`CMAKE_ARGS`不起作用
>
> 需要使用`-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}` 来指定安装的路径
>
>可以通过DEPENDS 来指定依赖关系

4. `find_*` 路径正确，却找不到。

> 原因： android-ndk-r14b 默认是将下面的值设置未ONLY导致，find_*找不到，必须强制指定参数`NO_CMAKE_FIND_ROOT_PATH`才可以，或者
> ```
>      set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
>           set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
> ```
> 就可以了

5. 在编译`curl`的时候，curl需要依赖``openssl``，但是可以指定`-DOPENSSL_USE_STATIC_LIBS=TRUE` 和 `-DOPENSSL_ROOT_DIR=${CMAKE_BINARY_DIR}` 来表明静态库的位置和查找的路径，这里要说一下，默认FINDOpenSSL.cmake 会增加include和lib进行查找头文件和静态库文件

6. 在我使用 `cmake  -DCMAKE_TOOLCHAIN_FILE=/cross/github/jni/android-ndk-r14b/build/cmake/android.toolchain.cmake -DANDROID_TOOLCHAIN=clang -DANDROID_ABI=armeabi-v7a -DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang ..` 进行编译的时候，会报错：
```
/home/txz/test/testCMake/t11/build/src/openssl/crypto/threads_pthread.c:183: error: undefined reference to 'pthread_atfork'
```
经过研究发现，原来这个方法定义是在libc里面，但是不是所有版本的libc都有这个定义，只有从``PLATFORM=11`` 开始 才有这个定义的，我直接就改成了``19``

7. 再次编译的时候又有头文件找不到的。
```
/cross/github/jni/android-ndk-r14b/toolchains/llvm/prebuilt/linux-x86_64/bin/../lib64/clang/3.8.275480/include/stdatomic.h:150:11: fatal error: 
      'uchar.h' file not found
```
经过分析发现：原来这个头文件只有在android-21之上才有，所以干脆就直接指定21进行编译,最终编译的命令行
```
cmake  -DCMAKE_TOOLCHAIN_FILE=/cross/github/jni/android-ndk-r14b/build/cmake/android.toolchain.cmake -DANDROID_TOOLCHAIN=clang -DANDROID_ABI=armeabi-v7a -DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang -DANDROID_PLATFORM=android-21   ..
```
可以成功编译上面的东西了

8. 遗留的问题，如何保证在`externalproject_add`中的代码没有改变的情况下，不会重新编译

