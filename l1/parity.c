/* File: parity.c
 * --------------
 * Read lab1 writeup for more information.
 */

#include <error.h>
#include <stdio.h>
#include <stdlib.h>



/* Function: parityA
 * -----------------
 * Simon says: "I used a simple loop to mask out bits one by one,
 * invert parity for each "on" bit. What could be simpler?
 * Sadly, it doesn't work and I don't know why.
 * Leaving it for the next programmer to work it out...""
 */
int parityA(int x)
{
    int parity = 0;
    for (int i = 0; i < 32; i++) {
        if (x & (1 << i)) 
            parity ^= 1;
    }
    return parity;
}


/* Function: parityB
 * -----------------
 * Simon says: "My second attempt, changed up my loop to shift
 * value down each iteration and test bit at position of least
 * significance. When I xor parity against lsb, this inverts
 * parity if lsb is 'on', no change if lsb 'off'. Perfect,
 * just what I wanted. I tested this code and it works great."
 *
 * You say: but did you test on negative inputs? I don't think so...
 */
int parityB(int x)
{
    int parity = x & 1;
    //while ((x = (x >> 1) & 0x7fffffff)) {
    while ((x = (unsigned int)x >> 1)) {
        parity ^= (x & 1);
    }
    return parity;
}


int main(int argc, char *argv[])
{
    if (argc < 2) error(1, 0, "missing argument");
    int val = atoi(argv[1]);
    printf("%d has %s parity\n", val, parityB(val) ? "odd" : "even");   
    return 0;
}
