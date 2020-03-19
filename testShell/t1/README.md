# 执行
```
./test.sh libhelloOri.so helloB
```
经过对比分析，可知`` frame_dummy`` 这个大小大概是44个字节 所以这里加44[十进制]个字节

这个大小可能跟具体的libc的实现有关，可能后面会报错。

这个脚本配合testCPP/t5里面的例子可以实现加解密
