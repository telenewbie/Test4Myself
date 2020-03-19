## 如何运行

```
./decryption helloso/libhello.so 1
指定so 的路径
指定类型 【1】 解密方式加载 【2】正常加载
```

# 加密

## 编译加密程序

```
g++ soCryption.cpp -o cryption 
```

## 运行加密程序

```
./cryption helloso/libhello.so
```

# 解密

## 编译解密程序

```
g++ soDecryption.cpp mem_operation.cpp -ldl -o decryption
```

## so还原

```
cd - ; cp libhelloOri.so libhello.so ;cd -
```

## 查看方法地址

```shell
$readelf -S helloso/libhello.so
节头：
  [号] 名称              类型             地址              偏移量
       大小              全体大小          旗标   链接   信息   对齐
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .note.gnu.build-i NOTE             00000000000001c8  000001c8
       0000000000000024  0000000000000000   A       0     0     4


$objdump -S ./helloso/libhello.so

elloso/libhello.so：     文件格式 elf64-x86-64
Disassembly of section .init:
00000000000005d8 <_init>:

Disassembly of section .plt:
00000000000005f0 <.plt>:
0000000000000600 <helloB@plt>:
```



```
g++ soDecryption.cpp mem_operation.cpp -ldl -o decryption &&g++ soCryption.cpp -o cryption && cd - ; cp libhelloOri.so libhello.so ;cd - && ./cryption helloso/libhello.so &&./decryption helloso/libhello.so 1
```

```正則
(\S+ <\S+>:)|(Disassembly)

Line 18272: 00014330 <call_weak_fn>:
Line 18283: 00014354 <deregister_tm_clones>:
Line 18305: 00014384 <register_tm_clones>:
Line 18328: 000143b8 <__do_global_dtors_aux>:
Line 18355: 000143f8 <frame_dummy>:
```

经过验证：

如果不使用`dlopen`的方式打开,直接 `-l`的方式参与连接，也是可以的。

连接的方式，不使用`dlopen`

```shell
g++ soDecryption.cpp mem_operation.cpp -ldl -o decryption -lhello -Lhelloso -std=c++11
```

使用`dlopen`

```shell
g++ soDecryption.cpp mem_operation.cpp -ldl -o decryption -DENCYPTION
```

两种方式编译之后，运行

```bash
$ ./decryption helloso/libhello.so 1
```

