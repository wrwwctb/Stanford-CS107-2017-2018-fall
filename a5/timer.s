# File: timer.s
# ---------------
# Returns a 64-bit value for the number of milliseconds since
# the computer was restarted 

.section .text

	.type	milliseconds, @function
	.globl milliseconds 

THOUSAND:
        .int 1000 # 32-bit int

milliseconds:
        # your code here
        rdtsc
        # Move edx to the upper 32 bits.
        shlq    $32,    %rdx
        # Ensure the upper 32 bits of rax are 0s.
        shlq    $32,    %rax
        shrq    $32,    %rax
        # Effectively concatenate
        addq    %rdx,   %rax
        # Make way for mulq, divq
        movq    $0x0,   %rdx
        # arithmetic
        movq    $1000,  %rsi
        mulq    %rsi
        divq    %rdi
        ret
