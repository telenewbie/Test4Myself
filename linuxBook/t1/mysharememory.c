#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define SHM_MODE 0600
#define SHM_SIZE 4096

#define PATHNAME "telenewbie3"
#define PROJ_ID 0x6669
#define WIRTE_COUNT 10

int main(){
	key_t key = ftok(PATHNAME,PROJ_ID);
	int shmid = shmget(key,SHM_SIZE,IPC_CREAT);
	if(shmid<0) 
	{
		perror("shmget error");
		exit(-1);
	}
	printf("client shmget shmid: %d\n",shmid);
	char * shmptr = shmat(shmid,0,0);
	printf("attached from %p to %p\n",(void*) shmptr,(void*)shmptr+SHM_SIZE);
	// 客户端 负责写
	int i =0;
	while(i < WIRTE_COUNT)
	{
		shmptr[i] = 'A' + i;
		i++;
		shmptr[i] = 0;
		sleep(1);
	}
	shmdt(shmptr);
	return 0;
}