File: readme.txt
Author: YOUR NAME HERE
----------------------

gets
1a)
code.c: In function ‘main’:
code.c:92:5: warning: ‘gets’ is deprecated (declared at /usr/include/stdio.h:638)
     if (gets(buf))   // this calls myth's version of gets
     ^
/tmp/ccBEV284.o: In function `main':
/afs/ir/users/c/e/cechang/107/a3/code.c:92: warning: the `gets' function is dangerous and should not be used.

1b) w is printed only during the first time apple_gets is called.
1c) No the buffer's length is still not considered.
1d) 25 total

calloc/realloc
2a) Overflow
2b) # of bytes allocated, rounded up to the nearest multiple of 8
2c) No. If *wp is 0, not spending time on assignment is faster.
2d) Speed
2e) Pointers themselves are passed by value.
2f)
If newsz==0, oldptr is freed. newptr is not malloc'd.
If newsz>0 and return is NULL, oldptr is not freed. newptr is not malloc'd
If newsz>0 and return is not NULL, oldptr is freed. newptr is malloc'd.


