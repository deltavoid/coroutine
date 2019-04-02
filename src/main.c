#include <stdio.h>
#include <stdlib.h>
#include "coroutine.h"



void func1(void* arg)
{
    //while (true)
    {
        printf("hello form func1\n");
        sleep(1);

        co_yield(arg);
    }

}


void func2(void* arg)
{
    //while (true)
    {
        printf("hello from func2\n");
        sleep(1);

         co_yield(arg);
    }

}

int main()
{
    //printf("main: 0\n");

    struct scheduler* sched = co_new();
    //printf("main: 1\n");
    
    co_add(sched, func1, sched);
    //printf("main: 2\n");
    
    co_add(sched, func2, sched);
    //printf("main: 3\n");
    
    co_run(sched);
    //printf("main: 4\n");

    return 0;
}