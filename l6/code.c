/* File: trace.c
 * -------------
 * Nonsense program used for studying assembly in lab.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void deref_one(char *ptr, long index) 
{
    *ptr = '\0';
}

void deref_two(int *ptr, long index) 
{
    *ptr = 0;
}


int s_arith(int a, int b) 
{
    return (a - b) * a;
}

unsigned int u_arith(unsigned int a, unsigned int b) 
{
    return (a - b) * a;
}

int combine(int x, int y) 
{
    return x + y;
}

int scale(int x)
{
    return x * 4;
}


int abs_val(int x)
{
    int sn = x >> (sizeof(int)*CHAR_BIT -1);
    return (x ^ sn) - sn;
}

int has_zero_byte(unsigned long val)
{
    unsigned long ones = ~0UL/UCHAR_MAX;
    unsigned long highs = ones << (CHAR_BIT - 1);
    return (((val - ones) & highs) & ~val) != 0;
}

int constant_fold(int x)
{
    return 10 + ((22*45 + 48)*13 - 100)*x - 1;
}


int trace(int param)
{
    int number = 0x4567;
    int arr[] = {1, 2, 3, number, param, INT_MIN, strlen("Stanford University")};
    int count = sizeof(arr)/sizeof(arr[0]);

    int total = 0;
    for (int i = 0; i < count; i++)
        total += arr[i];
    int squared = total * total;
    return squared + arr[3];
}

int sample(int a, int b);
int mine(int a, int b);

int main(int argc, char *argv[])
{
    int code = trace(0x107);
    if (code && argc > 2) {
        int one = atoi(argv[1]), two = atoi(argv[2]);
        int result = sample(one, two);
        int result2 = mine(one, two);
        printf("sample(%d, %d) = %d \n  mine(%d, %d) = %d\n", one, two, result, one, two, result2);
    }
    return 0;
}

