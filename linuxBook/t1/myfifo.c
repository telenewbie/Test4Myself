#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(){
	// 测试命名管道
	char fifo_name[10]="a";
	int r =mkfifo(fifo_name,O_NONBLOCK|O_APPEND|O_CREAT);
	printf("mkfifo :%d,%s\n",r,strerror(errno));
	int ret = open(fifo_name,O_RDONLY);
	printf("open only read ret = %d,%s\n",ret,strerror(errno));
	
	return 0;
}