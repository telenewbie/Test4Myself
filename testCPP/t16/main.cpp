//
// Created by Elevoc on 2020/10/26 0026.
//
#include <string>
#include <iostream>
#include <cstring>

void test(char *p) {
    char tmp[10] = "aabc";
    strcpy(p, tmp);
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
    return 0;
}