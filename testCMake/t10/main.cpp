#include <stdio.h>
#include "curl/curl.h"
#include "a.h"

int main(){
    curl_global_init(CURL_GLOBAL_ALL);
testA();
    curl_version_info_data * data =curl_version_info(CURLVERSION_NOW);
    printf("hello main\n");
    return 0;
}
