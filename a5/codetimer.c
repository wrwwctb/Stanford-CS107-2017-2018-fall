#include<stdio.h>
#include<stdlib.h>

long milliseconds(long clock_freq_hz);

long fib(int n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fib(n-1) + fib(n-2);
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("usage:\n\t%s fib_n clock_freq_hz\n",argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    long clock_freq = atol(argv[2]);

    long start = milliseconds(clock_freq);
    long f = fib(n);
    long end = milliseconds(clock_freq);

    printf("Fib %d: %lu\n", n, f);
    printf("start: %lu\n", start);
    printf("end:   %lu\n", end);
    printf("elapsed milliseconds: %lu\n",end - start);
    return 0;
}
