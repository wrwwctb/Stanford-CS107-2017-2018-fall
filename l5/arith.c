/* arith.c
 * -------
 * Code for playing with fp operations in lab 4.
 */
#include <math.h>
#include <stdbool.h>
#include <stdio.h>


// Try summing same numbers in different order to compare result
// Does associativity hold? Why or why not?
static void sum_doubles()
{
    double d[] = {1e20, -1e20, 107};
    int n = sizeof(d)/sizeof(*d);
    double sum_forward = 0, sum_backward = 0;

    for (int i = 0; i < n; i++)
        sum_forward += d[i];

    for (int i = n-1; i >= 0; i--)
        sum_backward += d[i];

    printf("Sum array forward = %g, Sum same array backward = %g\n",
           sum_forward, sum_backward);
}

int main(int argc, char *argv[])
{
    sum_doubles();
    return 0;
}

