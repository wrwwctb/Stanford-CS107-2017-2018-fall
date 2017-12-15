File: readme.txt
Author: YOUR NAME HERE
----------------------

The power of two
1a) One-hot
1b) The minimum eg. INT_MIN
1c) Consider positive numbers. For x=2^n, x-1 flips the most siginificant bit and every bit less significant. So & gives 0.
1d) 1

Finding the middle ground
2a) The subtraction can overflow. Eg. INT_MAX-(-1)
2b) Breaks when low or high is negative.
2c) The first operand of + is the carry bits of low+high and the second is (low+high)/2 without carry bits.

Ground zero
3a)
0x101010101010101
0x1010101
0x0
3b) Left shift by 7 is the same as multiplication by 0x80.
3c) For every byte, flip the most significant bit iff 0. Borrow happens iff a 0 exists.
3d) For every byte, flip the most significant bit. Zero the rest.
3e) Going from the least siginificant end to the most, if a byte is not 0, it evaluates to 0. Once a 0 byte shows up, it evaluates to nonzero, making the return 1.
