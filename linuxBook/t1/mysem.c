#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define SEM_NAMED
//#define SEM_UN_NAMED //未命名信号量

int main(){
#ifdef SEM_NAMED
	char sem_name[20] = "/telenewbie123";
	int ret =0;
	sem_t* sem =  sem_open(sem_name,O_CREAT|O_EXCL,S_IRWXU,0);
	if(!sem)perror("sem_open error");
	else printf("sem_open success\n");
	if (sem == SEM_FAILED &&errno == EEXIST) printf("aleady exists\n");
	sem_post(sem);// 计数+1
	sem_post(sem);// 计数+1
	ret = sem_trywait(sem);// 尝试等待  -1 EAGAIN
	if(ret)perror("sem_trywait error");
	sem_wait(sem);//等待信号 如果信号量计数为0 则会阻塞 否则信号量 -1
	sem_close(sem); // 释放信号量相关资源，不会影响信号量值的状态
	ret = sem_unlink(sem_name);// 销毁一个命名信号量
	if(ret !=0) printf("sem_unlink error:%d\n",ret);
#endif
#ifdef SEM_UN_NAMED
	sem_t sem ;
	int ret =sem_init(&sem,0,2);
	if(ret) perror("sem_init error");
	else printf("sem_init sucess \n");
	int value =0;
	ret = sem_getvalue(&sem,&value);
	printf("sem_getvalue:%d\n",value);
	ret = sem_trywait(&sem);// 尝试等待  -1 EAGAIN
	if(ret)perror("sem_trywait error");
	sem_wait(&sem);//等待信号 如果信号量计数为0 则会阻塞 否则信号量 -1
	sem_destroy(&sem);
#endif
	return 0;
}