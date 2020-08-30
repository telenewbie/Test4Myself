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
	// shared memory
	key_t key = ftok(PATHNAME,PROJ_ID);
	int shmid = shmget(key,SHM_SIZE,IPC_CREAT | IPC_EXCL | 0666 );
	if(shmid<0) 
	{
		perror("shmget error");
		exit(-1);
	}
	printf("server shmid :%d\n",shmid);
	char *addr = shmat(shmid,NULL,0);
	printf("attached address %p",(void*)addr);
	int i =0;
	while(i++ < WIRTE_COUNT)
	{
		printf("client# %s\n",addr);
		sleep(1);
	}
	shmdt(addr);
	if(shmctl(shmid,IPC_RMID,0)<0) perror("shmctl IPC_RMID error");
	return 0;
}