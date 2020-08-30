#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#if 1
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>
#endif

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

int main(){
	// 创建 socket
	struct addrinfo *ailist,*aip;
	struct addrinfo hint;
	int  err,n;
	char * host;
	if((n = sysconf(_SC_HOST_NAME_MAX))<0) n = HOST_NAME_MAX;
	printf("n=%d\n",n);
	if((host = malloc(n))<0) perror("malloc error\n");
	if(gethostname(host,n)<0) perror("gethostname\n");
	printf("host = %s\n",host);
	struct hostent* h =  gethostent();
	if(h == NULL) perror("gethostent error\n");
	printf("hostent->h_name:%s\n",h->h_name);
	printf("hostent->h_aliases:%s\n",*(h->h_aliases));
	memset(&hint,0,sizeof(hint));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_socktype = SOCK_DGRAM;
	if((err= getaddrinfo(host,"ruptime",&hint,&ailist))!=0){
		syslog(LOG_ERR,"ruptime:getaddrinfo error:%s",gai_strerror(err));
		exit(1);
	}
	for(aip= ailist;aip != NULL; aip = aip->ai_next){
		printf("addrinfo:%d\n",aip->ai_addr->sa_family);
	}
	return 0;
}