#include <map>
#include <stdio.h>
#include <string>

static inline ::std::map<int *, ::std::map<uint32_t, std::string *>> &get_mapTimers()
{
    static ::std::map<int *, ::std::map<uint32_t, std::string *>> x;
    return x;
}

int main(int argc, char const *argv[])
{
    int a = 1;
    int* &b = (&a);

    std::string* &value = get_mapTimers()[&a][1];  // 别名
    // printf("%s\n",value.c_str());
    // value ="abc";
    printf("%p\n", &(get_mapTimers()[&a][1]));
    printf("%p\n", &(value));

    value = new std::string("xxxx");
    printf("%p\n", &(get_mapTimers()[&a][1]));
    printf("%p\n", &(value));

    return 0;
}
