// 测试CPP的捕获
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <signal.h>
#include <string>
#include <unistd.h>

#define BT_BUF_SIZE 100

void (*SIGHANDLE)(int) = nullptr;

void SignalExceptionHandler(int code)
{
    int j, nptrs;
    void *buffer[BT_BUF_SIZE];
    char **strings;
    nptrs = backtrace(buffer, BT_BUF_SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL)
    {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
        return;
    }

    for (j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);

    free(strings);
    printf("over\n");
    SIGHANDLE(code);
}

int main(int argc, char const *argv[])
{
    signal(SIGHUP, SignalExceptionHandler);
    signal(SIGINT, SignalExceptionHandler);
    signal(SIGQUIT, SignalExceptionHandler);
    signal(SIGABRT, SignalExceptionHandler);
    signal(SIGILL, SignalExceptionHandler);
     signal(SIGSEGV, SignalExceptionHandler);
     signal(SIGFPE, SignalExceptionHandler);
    signal(SIGBUS, SignalExceptionHandler);
    signal(SIGPIPE, SignalExceptionHandler);
    int16_t

#if 0
    int a = 1 / 0;
#else
    int *a;
    *a = 1;
#endif
    while (1)
    {
        sleep(1);
    }
    return 0;
}
