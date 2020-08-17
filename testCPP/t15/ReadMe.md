环境：linux

主题：多进程 ，捕获子应用的程序输出给到主进程进行展示

重点： `dup2(fd[1],1)` 将输出关闭并且赋值给fd[1] 从而使 printf ("") 类似这些可以正常输出

应为 linux 环境没有 `pause` [windows] 的命令， 所以这里 参考网上的demo写的例子，可用。



```
chmod +x pause.sh
g++ main.c -o main.exe
```

