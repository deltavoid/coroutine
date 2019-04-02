#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coroutine.h"
//#

// pub unsafe extern "C" fn switch(&mut self, _target: &mut Self) {
//         asm!(
//         "
//         // push rip (by caller)
//         // Save old callee-save registers
//         push rbx
//         push rbp
//         push r12
//         push r13
//         push r14
//         push r15
//         mov r15, cr3
//         push r15
//         // Switch stacks
//         mov [rdi], rsp      // rdi = from_rsp
//         mov rsp, [rsi]      // rsi = to_rsp
//         // Save old callee-save registers
//         pop r15
//         mov cr3, r15
//         pop r15
//         pop r14
//         pop r13
//         pop r12
//         pop rbp
//         pop rbx
//         // pop rip
//         ret"
//         : : : : "intel" "volatile" )
//     }

// .text
// .globl switch_to
// switch_to:                      # switch_to(from, to)
//     # save from's registers
//     movq %rdi, %rax             # rax point to from
//     popq 0(%rax)                # save rip
//     movq %rsp, 0x08(%rax)
//     movq %rdi, 0x10(%rax)
//     movq %rsi, 0x18(%rax)
//     movq %rdx, 0x20(%rax)
//     movq %rcx, 0x28(%rax)
//     movq %r8,  0x30(%rax)
//     movq %r9,  0x38(%rax)
//     movq %r10, 0x40(%rax)
//     movq %r11, 0x48(%rax)
//     movq %rbx, 0x50(%rax)
//     movq %rbp, 0x58(%rax)
//     movq %r12, 0x60(%rax)
//     movq %r13, 0x68(%rax)
//     movq %r14, 0x70(%rax)
//     movq %r15, 0x78(%rax)

//     # restore to's registers
//     movq %rsi, %rax

//     movq 0x78(%rax), %r15
//     movq 0x70(%rax), %r14
//     movq 0x68(%rax), %r13
//     movq 0x60(%rax), %r12
//     movq 0x58(%rax), %rbp
//     movq 0x50(%rax), %rbx
//     movq 0x48(%rax), %r11
//     movq 0x40(%rax), %r10
//     movq 0x38(%rax), %r9
//     movq 0x30(%rax), %r8
//     movq 0x28(%rax), %rcx
//     movq 0x20(%rax), %rdx
//     movq 0x18(%rax), %rsi
//     movq 0x10(%rax), %rdi
//     movq 0x08(%rax), %rsp

//     pushq 0(%rax)               # push rip

//     ret

// .text
// .globl kernel_thread_entry
// kernel_thread_entry:        # void kernel_thread(void)

// 	# kern_enter(0)
// #if 0
// 	push %rdi
// 	push %rsi
// 	movq $0x0, %rdi
// 	call kern_enter
// 	pop  %rsi
// 	pop  %rdi
// #endif
	
//     movq %rdi, %rax         # mov fn to rax
//     movq %rsi, %rdi         # prepare arg
//     callq *%rax             # call fn

//     movq %rax, %rdi         # save the return value of fn(arg)
//     call do_exit            # call do_exit to terminate current thread

// void switch_to(struct context** from, struct context** to)
// {


// }


// void start(struct scheduler* sched, co_func_t func, void* arg)
// {
//     (*func)(arg);
//     co_yield(sched);
// }


bool schedule(struct scheduler* sched)
{
    int i;
    for (i = (sched->cur + 1)%sched->num; i != sched->cur; i += 1, i %= sched->num)
        if  (sched->corts[i]->state == Running)  break;
    
    printf("schedule: %d, %d\n", i, sched->corts[i]->state);
    if  (sched->corts[i]->state == Running)  {  sched->cur = i;  return true;}
    else return false;
}

void output_context(struct context* ctx)
{
    printf("%rip: %x\n", ctx->rip);
}

void output_register(unsigned long reg)
{
    printf("%x\n", reg);
}




struct scheduler* co_new()
{
    struct scheduler* sched = malloc(sizeof(struct scheduler));
    //sched->corts = (struct coroutine**)malloc(MAX_COROUTINES * sizeof(struct coroutine*));
    sched->cur = sched->num = 0;

    return sched;
}


void co_run(struct scheduler* sched)
{
    sched->cur = 0;
    
    while (schedule(sched))
    {
        printf("co_run: cur: %d\n", sched->cur);

        output_context(sched->corts[sched->cur]->ctx);
        printf("%x %x\n", &sched->ctx, &sched->corts[sched->cur]->ctx);
        switch_to(&sched->ctx, &sched->corts[sched->cur]->ctx);

    }

    // for (int i = 0; i < sched->num; i++)
    //     free((void*)sched->corts[i]);
    // free(sched);
}

bool co_add(struct scheduler* sched, co_func_t func, void* arg)
{
    unsigned char* stack = (unsigned char*)malloc(STACK_SIZE);
    struct coroutine* co = (struct coroutine*)stack;
    if  (sched->num >= MAX_COROUTINES)  return false;
    co->id = sched->num++;
    co->state = Running;

    unsigned long* args = (unsigned long*)(stack + STACK_SIZE - 4 * sizeof(unsigned long));
    args[1] = (unsigned long)sched;
    args[2] = (unsigned long)func;
    args[3] = (unsigned long)arg;
    printf("args: %x %x %x\n", args[1], args[2], args[3]);

    struct context* ctx = (struct context*)(((unsigned char*)args) - sizeof(struct context));
    memset(ctx, 0, sizeof(struct context));
    ctx->rip = (unsigned long)start_entry;
    printf("start_entry: %x\n", start_entry);
    
    co->ctx = ctx;
    sched->corts[co->id] = co;
    return true;
}

void co_yield(struct scheduler* sched)
{
    printf("co_yield\n");
    switch_to(&sched->corts[sched->cur]->ctx, &sched->ctx);
}



void co_exit(struct scheduler* sched)
{
    printf("co_exit\n");
    sched->corts[sched->cur]->state = Finished;
    co_yield(sched);
}