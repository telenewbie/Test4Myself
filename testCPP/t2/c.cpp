#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc,char * argv []){
    void * handle =  dlopen("./libb.so",RTLD_LAZY);
    if(handle == nullptr){
        printf("error \n ");
        printf("%s\n",dlerror());
        fflush(stdout);
        exit(0);
    }
    void (*pfun)(void)=(void(*)(void))dlsym(handle,"testB");
    if( pfun == nullptr )
    {
        
        printf("%s\n",dlerror());
        return 0;
    }
        pfun();
       dlclose(handle);
    return 0;
}
