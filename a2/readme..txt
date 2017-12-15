File: readme.txt
Author: YOUR NAME HERE
----------------------

atoi
1a) Distance from '0'
1b) case '+': s++;
1c)
If n is computed as a positive number, 2147483648 gives -2147483648 due to overflow
                                      -2147483648 gives -2147483648 due to overflow and then negation
If n is computed as a negative number, 2147483648 gives -2147483648 due to negation
                                      -2147483648 gives -2147483648 without issue
1d)
0          : No digit before $
12         : ':' stops operation.
2147483647 : Overflow
2          : 50 in ascii is 2.
seg fault  : Accessing address 50 is seg fault.

strtok
2a) To keep track of progress of tokenization.
2b) initializer element is not constant. Static variables need constant initializers.
2c) At the last call.
2d) s is moved to the end '\0'.
2e) The end delimiter of a token is changed to a null. Then p is moved one char further.

3) (scan_token goes in scan_token.c, tokenize test cases in custom_tests)

opendir/readdir
4a) 255
4b) 32,816
