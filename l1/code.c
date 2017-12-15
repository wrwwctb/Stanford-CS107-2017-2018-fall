/* File: code.c
 * ------------
 * Code passages to read as part of lab1.
 * Read lab1 writeup for more information.
 */

#include <limits.h>
#include <stdio.h>

size_t roundup(size_t sz, size_t mult)
{
    return (sz + mult-1) & ~(mult-1);
}

size_t roundup_alt(size_t sz, size_t mult)
{
    return (sz + mult) & -mult;
}


int abs_val(int x)
{
    return (x < 0 ? -x : x);
}

int abs_val_alt(int x)
{
    int sn = x >> (sizeof(int)*CHAR_BIT -1);
    return (x ^ sn) - sn;
}


int min(int x, int y)
{
    int diff = x-y;
    return y + (diff & (diff>>31));
}

int min_alt(int x, int y)
{
    return y ^ ((x ^ y) & -(x < y));
}


int main(int argc, char *argv[])
{
    printf("roundup(%d, %d) = %zu\n", 9, 2, roundup(9, 2));
    printf("roundup(%d, %d) = %zu\n", 33, 32, roundup(33, 32));
    printf("roundup(%d, %d) = %zu\n", 4, 4, roundup(4, 4));
    printf("roundup(%d, %d) = %zu\n", 7, 6, roundup(7, 6)); // mult is not a power of 2!
    printf("roundup_alt(%d, %d) = %zu\n\n", 32, 32, roundup_alt(33, 32));

    printf("abs_val(%d) = %d\n", 27, abs_val(27));
    printf("abs_val(%d) = %d\n", -27, abs_val(-27));
    printf("abs_val_alt(%d) = %d\n", 27, abs_val_alt(27));
    printf("abs_val_alt(%d) = %d\n\n", -27, abs_val_alt(-27));

    printf("min(%d, %d) = %d\n", 7, 0, min(7, 0));
    printf("min(%d, %d) = %d\n", 5, 2, min(5, 2));
    printf("min(%d, %d) = %d\n", -1, 3, min(-1, 3));
    printf("min(%d, %d) = %d\n", INT_MIN, 1, min(INT_MIN, 1));
    printf("min(%d, %d) = %d\n", INT_MAX, INT_MIN, min(INT_MAX, INT_MIN));
    printf("min(%d, %d) = %d\n", INT_MAX, -1, min(INT_MAX, -1));
    printf("min_alt(%d, %d) = %d\n", INT_MIN, 1, min_alt(INT_MIN, 1));
    printf("min_alt(%d, %d) = %d\n", INT_MAX, INT_MIN, min_alt(INT_MAX, INT_MIN));
    printf("min_alt(%d, %d) = %d\n", INT_MAX, -1, min_alt(INT_MAX, -1));
    return 0;
}