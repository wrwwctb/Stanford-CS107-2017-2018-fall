#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int sum_A(int n)
{
    int result = 0;

    for (int cur = 1; cur <= n; cur++)
        result += cur;
    return result;
}

int sum_B(int n)
{
    int result = 0;
    int cur = 1;

    while (cur <= n)
        result += cur++;
    return result;
}

int sum_C(int n)
{
    int result = 0;
    int cur = 0;

    do {
        result += cur++;
    } while (cur <= n);
    return result;
}

int sum_D(int n)
{
    int result = 0;
    int cur;

    for (cur = 1; cur + 3 <= n; cur += 4) {
        result += cur + cur + 1 + cur + 2 + cur + 3;
    }
    for (; cur <= n; cur++)
        result += cur;
    return result;
}

double time_in_msecs(int (*fn)(int), int val)
{
    struct timeval start, end;
    gettimeofday(&start, 0);
    fn(val);
    gettimeofday(&end, 0);
    return (end.tv_sec-start.tv_sec)*1e3 + (end.tv_usec-start.tv_usec)/1e3;
}

int main(int argc, char *argv[])
{
    int val = 1e6;
    printf("This program will report time spent executing each call to sum()\n");;

    printf("%1.4f msecs executing sum_A(%d)\n", time_in_msecs(sum_A, val), val);
    printf("%1.4f msecs executing sum_B(%d)\n", time_in_msecs(sum_B, val), val);
    printf("%1.4f msecs executing sum_C(%d)\n", time_in_msecs(sum_C, val), val);
    printf("%1.4f msecs executing sum_D(%d)\n", time_in_msecs(sum_D, val), val);
    return 0;
}
