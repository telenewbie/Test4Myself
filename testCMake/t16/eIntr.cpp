// 测试 EINTR 的作用？

#include <stdio.h>
// 可以查看 man errno 来获取帮助
#include <errno.h>

int main(){
    printf("hell \n");
    int n = EINTR;
    printf("EINTR:%d\n",n);
    return 0 ;
}