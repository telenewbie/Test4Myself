#include <stdio.h>
#include <stdlib.h>

#ifndef _WIN32
#error 123
#endif
#ifndef ABC
#error abc
#endif

#include "curl.h"
#include "easy.h"

int main(){
	CURL* curl;
	printf("hello try_compile\n");
	return 0;
}