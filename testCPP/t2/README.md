如果不加 `-z defs` 则会把一些编译器可以发现的问题，给隐藏了。

`a.cpp` 由 libb.so 进行调用

`b.cpp` 生成libb.so 

`c.cpp` 生成a.out 

由`c.cpp` 去`dlopen`打开 一个库并且调用，就会报运行时找不到符号的问题了。
