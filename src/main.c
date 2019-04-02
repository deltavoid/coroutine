#include <stdio.h>
#include <stdlib.h>
#include "coroutine.h"



void func1(void* arg)
{
    printf("hello form func1\n");
    sleep(1);

    co_yield(arg);
}


void func2(void* arg)
{
    printf("hello from func2\n");
    sleep(1);

    co_yield(arg);
}

int main()
{
    
    struct scheduler* sched = co_new();
    co_add(sched, func1, sched);
    co_add(sched, func2, sched);
    co_run(sched);

    return 0;
}