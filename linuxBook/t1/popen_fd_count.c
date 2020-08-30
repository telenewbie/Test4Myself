#include <stdio.h>
#include <unistd.h>

int main(){
	int fd[2] = {-1,-1};
	int fd1[2] = {-1,-1};
	pipe(fd);
	pipe(fd1);
	// 调用 睡眠进程查看 fd的变化
	// 结论：在 wsl ubuntu 上面 不会 拷贝主进程的所有 fd 而是自己维护增加一个fd
	FILE* fp =popen("./mysleep","r");
	if(!fp) perror("popen error\n");
	while(1)
		usleep(1000);
	return 0;
}