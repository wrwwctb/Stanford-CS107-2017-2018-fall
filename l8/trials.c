/* File: trials.c
 * --------------
 * Various ways of computing 2^power or is-power-of-2 to be tested for performance
 */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "fcyc.h"

// Five different ways to compute 2^exp, which is best and why?

long two_to_power_A(unsigned int exp)
{
     return pow(2, exp);
}

// is specialized 2^exp any faster than general pow()?
long two_to_power_B(unsigned int exp)
{
    return exp2(exp);
}

// loop to repeatedly multiply by 2, iterates power # of times
long two_to_power_C(unsigned int exp)
{
    long result = 1;
    for (int i = 1; i <= exp; i++)
        result *= 2;
    return result;
}

// base is known to be 2 in this case, so why not a simple bit shift?
long two_to_power_D(unsigned int exp)
{
    return 1L << exp;
}



// Five different ways to report whether a number is a power of 2
// Which is best and why?

bool is_power_A(int num)
{
    // loop that generates successive powers of two, compare to num for equality
    for (int i = 0; i < 31; i++)
        if (num == (1 << i)) return true;
    return false;
}

bool is_power_B(int num)
{
    // convert to float, use math log2 function, then see if whole-numbered
    float val = log2f(num);
    return (int)val == (int)floorf(val);
}

bool is_power_C(int num)
{
    // popcount is an efficient built-in that count "on" bits, powers of 2 have only one bit on
    return __builtin_popcount(num) == 1;
}

bool is_power_D(int num)
{
    // switch statement to match powers of two
    // which kind of assembly does this switch compile to? Why?
    switch (num) {
        case 1: case 2: case 4: case 8:
        case 0x10: case 0x20: case 0x40: case 0x80:
        case 0x100: case 0x200: case 0x400: case 0x800:
        case 0x1000: case 0x2000: case 0x4000: case 0x8000:
        case 0x10000: case 0x20000: case 0x40000: case 0x80000:
        case 0x100000: case 0x200000: case 0x400000: case 0x800000:
        case 0x1000000: case 0x2000000: case 0x4000000: case 0x8000000:
        case 0x10000000: case 0x20000000: case 0x40000000: case 0x80000000:
            return true;
        default:
            return false;
    }
}

bool is_power_E(int num)
{
    // leverage bitwise identities to find numbers with only one bit on
    // Remember where we saw this previously?
    return num && ((num & (num - 1)) == 0);
}


static struct { 
    long (*fn)(unsigned int); 
    char *descr; 
} pfns[] = 
   {{two_to_power_A, "calls pow()"}, 
    {two_to_power_B, "calls exp2()"},
    {two_to_power_C, "multiply loop",}, 
    {two_to_power_D, "bit shift"}};
static const int npfns = sizeof(pfns)/sizeof(*pfns);

static struct { 
    bool (*fn)(int); 
    char *descr; 
} ifns[] =
   {{is_power_A, "multiply loop"}, 
    {is_power_B, "calls log2"},
    {is_power_C, "popcount == 1"}, 
    {is_power_D, "lookup table, switch"},
    {is_power_E, "recognize bitwise/numeric identity"}};
static const int nifns = sizeof(ifns)/sizeof(*ifns);


int main(int argc, char *argv[])
{
    const int ntrials = 1000;
    int arr[ntrials];
    for (int j = 0; j < ntrials; j++) arr[j] = rand(); // set up random array
    long result = 0;

    printf("\nTiming results for two_to_power()\n");
    for (int i = 0; i < npfns; i++) {
        start_counter();
        for (int j = 0; j < ntrials; j++)
            result += pfns[i].fn(arr[j] % 60);
        long totalcycles = get_counter();
        printf("%3ld cycles/call %s [Version %c %s]\n", totalcycles/ntrials, LABEL, i+'A', pfns[i].descr);
    }

    printf("\nTiming results for is_power()\n");
    for (int i = 0; i < nifns; i++) {
        start_counter();
        for (int j = 0; j < ntrials; j++)
            result += ifns[i].fn(arr[j]);
        long totalcycles = get_counter();
        printf("%3ld cycles/call %s [Version %c %s]\n", totalcycles/ntrials, LABEL, i+'A', ifns[i].descr);
    }

    return result == 0;
}
