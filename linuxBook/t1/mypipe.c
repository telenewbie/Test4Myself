#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>

#define TEST_READ_PIPE

void myPro(int sig){
	printf("sig\n");
}
int main(){
	int fd[2] = {-1,-1};
	pipe(fd);
	printf("PIPE_BUF:%d\n",PIPE_BUF);
	printf("MAXLINE:%d\n",MAXLINE);
#if TEST_WRITE_PIPE
	// 测试关闭读端，对于写端的影响
	close(fd[0]);
	// 捕捉信号
	signal(SIGPIPE,myPro);
	char content [PIPE_BUF] = "hello world";
	int ret = write(fd[1],content,sizeof(content));
	if(ret<0) perror("write error");
	else printf("write success %d\n",ret);
#endif
#ifdef TEST_READ_PIPE
	char content [12] = "hello world";
	write(fd[1],content,sizeof(content));
	close(fd[1]);
	char buf[256] ={0};
	int ret = 0;
	while((ret = read(fd[0],buf,sizeof(buf)))>0){
		printf("read data: %s\n",buf);
	}
	printf("read over\n");
#endif
	return 0;
}