#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define BUFLEN 1024
int main(){
	char buf[BUFLEN];
	FILE * fp = popen("/usr/bin/uptime","r");
	if(fp == NULL) {
		perror("error popen\n");
		exit(-1);
	}else{
		if(fgets(buf,BUFLEN,fp) != NULL){
			printf("get %s\n",buf);
		}
	}
	pclose(fp);
	return 0;
}