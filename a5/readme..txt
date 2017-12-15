File: readme.txt
Author: YOUR NAME HERE
----------------------

division
1a)
idiv gives the quotient in %rax and the modulus om %rdx.

1b)
Before: shr    %eax
After : shr    $0x5,%eax

1c)
Unsigned division: shr
Signed division: shr by 31 bits, add the result to the original operand and then sar
When an operand x is negative and odd, sar alone gives x/2-1. shr 0x1f and add can compensate this, but don't affect the result when x is positive or even.

1d)
cmovns is "conditionally move if not sign." If %edi is nonnegative, move it to %eax which is to be shifted. If %edi is negative, the same trick as 1c) is used.

timer
2a)
18446744073709551615/(3158610000*60*60*24) ~ 67594 days

2b)
n >= 88

Ran experiments on an Intel Xeon X3480 with throttling turned off so the frequency was fixed at 3060000000Hz.

Results:
n,  t=elapsed time (ms), t2=log2(elapsed clock ticks)
------------------------------------------------
40, 713,                 31.02
42, 1812,                32.37
45, 7705,                34.46
47, 19966,               35.83
50, 84716,               37.92
52, 221387,              39.30
55, 938937,              41.39

Affine fit betwee n and t2 gives
  n ~ 3.333 + .6917 t2
with the correlation coefficient being 0.99998823.
For the clock ticks to overflow, we need
  t2 > log2(18446744073709551615-3060000000*60) ~ 64.
which corresponds to
  n >= 88

loops
3a)
The only difference is how cur is incremented.
for  : add    $0x1,%edx
while: lea    0x1(%rdx),%edx

3b)
A do-while loop is executed for one time before the exit condition is checked. So, unlike for and while, there is no jmp before result += cur.

3c)
for: 50%
  4005fc:       01 d0                   add    %edx,%eax
  4005fe:       83 c2 01                add    $0x1,%edx
  400601:       39 fa                   cmp    %edi,%edx
  400603:       7e f7                   jle    4005fc <sum_A+0xc>
while: 50%
  400613:       01 d0                   add    %edx,%eax
  400615:       8d 52 01                lea    0x1(%rdx),%edx
  400618:       39 fa                   cmp    %edi,%edx
  40061a:       7e f7                   jle    400613 <sum_B+0xc>
do-while: 40%, but the difference from the above two is due to how cur is incremented.
  400628:       8d 4a 01                lea    0x1(%rdx),%ecx
  40062b:       01 d0                   add    %edx,%eax
  40062d:       89 ca                   mov    %ecx,%edx
  40062f:       39 f9                   cmp    %edi,%ecx
  400631:       7e f5                   jle    400628 <sum_C+0xa>

3d)
jmp. One less jmp in the loop is faster.

3e)
It's possible because of the utilization of:
  lea    0x6(%rax,%rdx,4),%eax
which does what previously required 4 iterations. The comparision mechanism is longer by one instruction because cur + 3 is tested.

3f)
Results:
This program will report time spent executing each call to sum()
0.5400 msecs executing sum_A(1000000)
0.4600 msecs executing sum_B(1000000)
0.9290 msecs executing sum_C(1000000)
0.1580 msecs executing sum_D(1000000)

sum_A vs sum_B: lea is a bit faster than add for increment.
sum_C: mov is roughly as slow as all others (lea, add, cmp, jle) combined.
sum_D: A bit more than 25% of the time sum_B used, as expected.

bbsort
4a)
        -n      Sort numbers
        -r      Reverse sort order
        -u      Suppress duplicate lines

4b)
-z: '\0' is used as the delimiter, so everything is concatenated into one entry.
-c: No effect. A sorted list is printed.

4c)
compare_keys. 44

4d)
Arguments are stored in registers. char**.
p *(char**) $rdi

4e)
0x201d3a(%rip) i.e. 0x603180. x/1xg 0x603180 gives
0x0000000000000101 when -n -r is specified
0x0000000000000100      -r

4f)
Print the array with p *(char**)0x604270@24
With -r, the array is sorted, not just printed, in reverse.
With -u, duplicates are removed after sorting.

