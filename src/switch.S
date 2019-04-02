.text
.globl switch_to
switch_to:                      # switch_to(from, to)
    # save from's registers
    # push rip (by caller)
    # Save old callee-save registers
    pushq %rbx
    pushq %rbp
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    movq %rsp, 0(%rdi)


    # restore to's registers
    movq 0(%rsi), %rsp

    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    popq %rbx
    # pop rip
    
    ret


.globl start_entry
start_entry:                   
    movq 0x8(%rsp), %rax
    movq 0xc(%rsp), %rdi
    callq *%rax

    movq 0x4(%rsp), %rdi
    call co_yield


