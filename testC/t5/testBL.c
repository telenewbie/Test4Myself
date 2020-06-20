#include <stdio.h>
#ifdef __WIN32__
#include <WinSock2.h>
#error 123
#else
// #include <arpa/inet.h>
//#include<netinet/in.h>
#include <arpa/inet.h>
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#endif
#include <string.h>
void process (char con[2]){
    printf("0x%x%x\n",con[0],con[1]);
}

char alignData ='\0';
void align(char * data ,size_t len){
    if(alignData !='\0'){
        char temp [2] ={0};
        memcpy(temp,&alignData,1);
        memcpy(temp+1,data,1);
        process(temp);
        data +=1;
        --len;
    }
    for(int i = len-1;i>=1;i=i-2){
        process(data);
        data = data+2;
    }
    if(len%2==1){
        alignData = (data[len-1]);
    }else{
        alignData ='\0';
    }
}

int main(int argc,char * argv[])
{
#if 0
    char test[2] ={0};
    test[0] = 0x10;
    test[1] = 0x24;
    short* tests = (short*)test;
    printf("0x%x\n",*tests);
#endif
#if 1
    short test[2] ={0};
    test[0] = 0x1234;//
    test[1] = 0x5678;//
    char* tests = (char*)test;//12345678 
    printf("0x%x%x%x%x\n",*tests ,*(tests+1),*(tests+2),*(tests+3));
#endif

uint16_t result =htons(0x1234);
uint16_t result1 =ntohs(0x1234);
printf("0x%x\n",result);
printf("0x%x\n",result1);
printf("0x%x\n",0x1234);

printf("-------------\n");
int testalign = 0x12345678;
char * test4int = (char*)(&testalign);
printf("%x%x%x%x\n",*(test4int++),*(test4int++),*(test4int++),*(test4int++));
align(((char*)&testalign),1);
align(((char*)&testalign)+1,3);


return 0;
}