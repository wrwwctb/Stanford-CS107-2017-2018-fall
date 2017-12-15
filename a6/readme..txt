File: readme.txt
Author: YOUR NAME HERE
----------------------

Atm attack
---
1a)
The input can always be longer than the allocated size of the buffer.

1b)
Top 8 bytes: Content of %r12
Next 8         "        %rbp
Next 8         "        %rbx
Next 7 bytes: Not used
Next 9 bytes: buf

%rsp points at buf right before gets.

One char too long: Program works.
30        "        %rbp and %rbx are corrupted.
60        "        The saved %rip before fast_cash, as well as another 12
                   bytes above, is corrupted.

1c)
Using account "kfo" the password is:
"kfo\nofk" (8 bytes),
followed by 0x00 repeated for 32 times,
followed by 0x0000000000400b37,
totaling 48, bytes.

1d)
Call the program by
./atm kfo < password.txt

Stack protector
--------------
2a)
*** stack smashing detected ***: samples/protected_atm_soln terminated
Abort (core dumped)

2b)
prolog:

  400aef:       64 48 8b 04 25 28 00    mov    %fs:0x28,%rax
  400af6:       00 00
  400af8:       48 89 44 24 18          mov    %rax,0x18(%rsp)

epilog:

  400b75:       48 8b 4c 24 18          mov    0x18(%rsp),%rcx
  400b7a:       64 48 33 0c 25 28 00    xor    %fs:0x28,%rcx
  400b81:       00 00
  400b83:       74 05                   je     400b8a <fast_cash+0xa8>
  400b85:       e8 16 fc ff ff          callq  4007a0 <__stack_chk_fail@plt>

2c)
8 bytes. Immediately below saved registers. To safeguard saved registers and the rest of the stack.

2d)
Such that deliberate tampering can be detected. If the canary is known, we can feed that value, along with malicious codes, onto the stack without being detected.

Bomb
----
3a)
b explode_bomb

3b)
0x4029f0
It is the pointer to the password of level 1. It is fed to strcmp via %edi.
Because the address is short and mov can be interpreted as movl which populates the upper 4 bytes with zeros.

3c)
xor sets flags which affect jle.
Jump happens if ZF=1 or SF!=OF.
In this context, since (1) the two numbers need to differ by 1471, and (2) xor cannot overflow, we must have SF=1.

3d)
winky exists when the input string (without its first char) matches one of the words given by read_64_words.

3e)
char**, 8 bytes
Use:
  display/1gx $rsp+8
  display/s $rax
After 0x401c75:
  mov    0x8(%rsp),%rax
rax is shown to contain an address pointing to characters.

The memory, whose address is 8 + the content of rsp, is written in strtoul, to be an address in the heap, where the first character of the input string, past what's interpreted by strtoul, is stored. 

3f) 
It has two 4-byte arguments. For each argument, the function counts the number of zero bits from the least significant bit, until it reaches an one or the end. The function then compares the counts of the two arguments. The function is different from the cmp instruction.
