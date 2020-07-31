#include <stdio.h>


#include "curl.h"
#include "easy.h"
int main(){
	CURL* curl=curl_easy_init();
	printf("hello world\n");
	return 0;
}