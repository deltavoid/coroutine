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
#    movq %rsp, %rdi
#    call output_register
    movq %rsp, 0x0(%rdi)
#    call output_register

    # restore to's registers
    movq 0x0(%rsi), %rsp
#    call output_register

    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    popq %rbx
    # pop rip
#    call output_register
    
    ret


.globl start_entry
start_entry:          
    
#    call output_register
    movq 0x10(%rsp), %rax
    movq 0x18(%rsp), %rdi
    callq *%rax

#    call output_register
    movq 0x8(%rsp), %rdi
    call co_exit


