#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int testopen(){
    int filedesc = open("testfile.txt", O_WRONLY | O_APPEND|O_CREAT);
    if (filedesc < 0)
    {
        write(2,"error open \n",strlen("error open \n"));
        return 1;
    }
    //判断文件是否存在
    if(access("testfile.txt",F_OK)==0){
        printf("存在\n");
    }

    if (write(filedesc, "This will be output to testfile.txt\n", 36) != 36)
    {
        write(2, "There was an error writing to testfile.txt\n", strlen("There was an error writing to testfile.txt")); // strictly not an error, it is allowable for fewer characters than requested to be written.
        return 1;
    }
    close(filedesc);
}

void testfopen(){
    FILE* file = fopen("a.txt","w+");
    if(!file){
        printf("error\n");
    }
    //判断文件是否存在
    if(access("a.txt",F_OK)==0){
        printf("存在\n");
    }
    // 写入文件
    char buf[20]="hello world\0";
    fwrite(buf,1,strlen(buf),file);
    fclose(file);
}

int main(int argc, char *argv)
{
    //testfopen();
    testopen();
}