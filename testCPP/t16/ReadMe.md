测试c++ 的函数之间的区别

# st::string resize vs reverse
```
s[reserve]:1000,size:1000
s1[resize]:0,size:0
```

# 获取时间戳
C语言
```
struct timeval time;
gettimeofday(&time, NULL);
uint64_t b = ((uint64_t) time.tv_sec * 1000 + time.tv_usec / 1000); // 记得先强转才可以，要不然可能出现越界的情况
```
c++
```
uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
```
