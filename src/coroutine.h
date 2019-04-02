#ifndef _COROUTINE_H
#define _COROUTINE_H
#include <stdbool.h>

#define MAX_COROUTINES 64
#define STACK_SIZE 4096

enum co_state
{
    Running = 1,
//    Waiting,
    Finished
};

struct context
{
    unsigned long r15, r14, r13, r12, rbp, rbx;
    unsigned long rip;
};


struct coroutine
{
    int id;
    int state;
    struct context* ctx;
};

struct scheduler
{
    struct coroutine* corts[MAX_COROUTINES];
    //int f, p;
    int cur, num;
    struct context* ctx;
    //Coroutine* cur;
};




typedef void (*co_func_t)(void* arg);

extern void switch_to(struct context** from, struct context** to);
//extern void start(struct scheduler* sched, co_func_t func, void* arg);
extern void start_entry();
extern bool schedule(struct scheduler* sched);

extern struct scheduler* co_new();
//extern void co_del(struct scheduler* sched);
extern void co_run(struct scheduler* sched);
extern bool co_add(struct scheduler* sched, co_func_t func, void* arg);
extern void co_yield(struct scheduler* sched);
extern void co_exit(struct scheduler* sched);

#endif