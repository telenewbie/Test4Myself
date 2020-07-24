Windows 的命令行窗口的编译 一开始以为很难，

在看了 [这篇文章](https://www.cnblogs.com/Stultz-Lee/p/10012366.html)之后，突然觉得，配合 cmake 简直不要太简单。

需要配置的环境变量 cmake 以及 vcvarsall 的路径

在我的环境是
```
C:\Users\telenewbie>cmake --version
cmake version 3.18.0-rc2

CMake suite maintained and supported by Kitware (kitware.com/cmake).

C:\Users\telenewbie>echo %VC_HOME%
D:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build
```
主要的 环境配置就是这个上面

关于脚本 还要多说一句，之前也想写个 vcvarsall 的脚本，但是 每次直接调用vcvarsall之后 会导致 其他的语句不执行，在这里看到这位大神的做法 `call vcvarsall x86`就可以 也是很牛的啊
