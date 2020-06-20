`b.cpp`  和`c.cpp` 编译出来 用 `g++ -S x.cpp -o x.s` 可以生成 `b.s`和`c.s` 可以从生成的汇编文件中看出石油区别的，如果只是简单的 调用 `a++` 和 `++a` 经过编译器优化是没有区别的。结论：`a++` 会多拷贝两次，如果有使用到返回值的话，但是如果没有返回值的话，会被编译器优化，我们最后不要写出一堆依赖编译器的行为的代码，所以尽量写成 `++a`。
必须要写`a++` 的场景：
1. 如果是从集合中剔除一项
2. 指针的偏移 `*(pa++)`

`a.cpp` 表明 类的继承前修饰符，是由区别的，可以看出单独编译 `a.cpp` 是编译不过的，结论：不改变封装性。