//
// Created by Elevoc on 2020/11/2 0002.
//
#include <iostream>
#include <algorithm>    // std::find_first_of
#include <cstdarg>
#include <string>
#include <sstream>

using namespace std;
#ifndef ELEVOC_REQUEST_BASE_URL
#define ELEVOC_REQUEST_BASE_URL "sdk-auth.elevoc.com"
#endif
#define ONLINE_OAUTH_URL "https://" ELEVOC_REQUEST_BASE_URL "/serviceEnv/oauth_device.php"

std::string test(int num_args,...){
    va_list ap;
    std::stringstream ss;
    int  i ;
    va_start(ap, num_args);
    for(i = 0; i < num_args; i++) {
        ss << va_arg(ap,char*);
    }
    va_end(ap);
    return ss.str();
}

int main() {
    std::cout << "a" << std::endl;
    std::string path(ONLINE_OAUTH_URL);
    int i = path.find("//");
    std::cout << i << endl;

    path  = path.replace(i+2,std::string("sdk-auth.elevoc.com").length(),"sdk-auth.pocradio.com:20333");
    cout<<path<<endl;
    std::cout<<test(3,"a","b","c")<<endl;
    return 0;
}