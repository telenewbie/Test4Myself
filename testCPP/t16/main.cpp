//
// Created by Elevoc on 2020/10/26 0026.
//
#include <string>
#include <iostream>
#include <cstring>
#include <ctime>
#include <chrono>
#include <sys/time.h>


void test(char *p) {
    char tmp[10] = "aabc";
    strcpy(p, tmp);
}


uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main() {
    std::string s;
    s.reserve(1000);
    std::string s1;
    s.resize(1000);
    std::cout << "s[reserve]:" << s.length() << ",size:" << s.size() << std::endl;
    std::cout << "s1[resize]:" << s1.length() << ",size:" << s1.size() << std::endl;
    char p[10] = {0};
    test(p);
    printf("test:%s\n", p);

    std::cout << "current timestampse:" << timeSinceEpochMillisec() << std::endl;
    char *pEnd;
    std::cout << strtoll("1603694369345", &pEnd, 10) << std::endl;
    printf("time:%lld\n", strtoll("1603694369345", &pEnd, 10));
    uint64_t a = 1603694369345;
    printf("a:%lld\n", a);

    // 获取时间
    struct timeval time;
    gettimeofday(&time, NULL);
    uint64_t b = ((uint64_t) time.tv_sec * 1000 + time.tv_usec / 1000);
    printf("%lld\n", (time.tv_sec * 1000 + time.tv_usec / 1000));
    printf("%lld\n", b);
    return 0;
}