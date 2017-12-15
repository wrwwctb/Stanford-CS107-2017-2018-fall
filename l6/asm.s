# File: asm.s
# -----------
# These fuctions are implemented in assembly and linked with C code

.section .text

    .type  sample, @function
    .globl sample
    
sample:
    cmpl    %edi, %esi
    je      .L1
    movl    %esi, %eax
    addl    %edi, %eax
    ret
.L1:
    movl    $22, %eax
    ret




    .type  mine, @function
    .globl mine
            
mine:
    # this function currently empty, yours to fill in!
    #mov $0, %eax

    #make sure upper bits are 0s or 1s depending on sign
    salq $32, %rdi
    salq $32, %rsi
    sarq $32, %rdi
    sarq $32, %rsi

    subq %rsi, %rdi
    movq %rdi, %rax
    cmpq $0, %rax
    jns  .pos
    negl %eax
.pos:
    ret
