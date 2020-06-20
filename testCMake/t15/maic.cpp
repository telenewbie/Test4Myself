#include <stdio.h>

int main()
{
    int *attr = new int[2];
    attr[0] = 1;
    delete[] attr;
    printf("%d\n", attr[0]);
    attr[0] = 100;
    printf("%d\n", attr[0]);
    return 0;
}