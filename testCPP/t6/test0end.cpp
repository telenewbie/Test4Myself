#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hello(void**a,int len){
    *a = malloc(len);
    
}

int main(int argc, char **argv)
{
    printf("hello world\n");
    char a[] = "hello0123";
    a[5] = '\0';

    printf("%s\n", a);
    printf("strlen = %lu, sizeof = %lu\n", strlen(a), sizeof(a));

    // char b[5] = "hello";

    char a1[6] = "hello";
    char b[sizeof(a1)] = {0};
    memcpy(b, a1, sizeof(a1));
    printf("%s\n", b);

    // strcpy
    char a2[6] = "world";
    char b2[6] = {0};
    strcpy(b2, a2);
    printf("%s\n", b2);

    const char *b3 = "bcd";
    strcpy(a2, b3);
    printf("%s\n", a2);

    char str[3] = "ab";
    printf("%02x%02x\n", str[0], str[1]);
    for (size_t i = 0; i < strlen(str); ++i)
    {
        printf("%02x", str[i]);
    }
    printf("\n");
    void * dupP = NULL;
    printf("void* sizeof=%d\n",dupP==NULL);
    hello(&dupP,3);
    printf("void* sizeof=%d\n",dupP==NULL);
    return 0;
}