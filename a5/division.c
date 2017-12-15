#include <stdio.h>


void divide(int numer, int denom, int *p_quot, int *p_mod)
{
    *p_quot = numer / denom;
    *p_mod = numer % denom;
}

unsigned int udiv_by_two(unsigned int n)
{
    return n / 2;
}

int div_by_two(int n)
{
    return n / 2;
}

unsigned int udiv_by_ten(unsigned int n)
{
    return n / 10;
}

int main(int argc, char *argv[])
{
    int numer = 22, denom = 5;
    int q, m;
    divide(numer, denom, &q, &m);
    printf("%d divided by %d is %d mod %d\n", numer, denom, q, m);
    return 0;
}
