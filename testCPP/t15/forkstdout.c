#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __WIN32
#else
	 #include <sys/types.h>
       #include <unistd.h>
#endif

int main(){
	int fd[2] = {0};
	pipe(fd);
	int ret =fork();
	if(ret == 0){
		//child
		close(fd[0]);
		dup2(fd[1], 1);
		// 执行命令
		system("./main.exe");
		printf("child over\n");
	}else{
		// parent
		close(fd[1]);//close write
		char buf[8]= {0};
		int readSize = 0;
		
		while((readSize = read(fd[0],buf,sizeof(buf)))>0){
			printf("parent: %.*s\n",readSize,buf);
		}
		printf("parent over\n");
	}
	return 0;
}
