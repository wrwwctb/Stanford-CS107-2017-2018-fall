#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int cmp_int(const void *p, const void *q)
{
    int first = *(const int *)p;
    int second = *(const int *)q;
    return first - second;
}

int nobetter_cmp_int(const void *p, const void *q)
{
    long first = *(const int *)p;
    long second = *(const int *)q;
    return first - second;
}

int main(int argc, char *argv[])
{
    int nums[12];
    int count = sizeof(nums)/sizeof(nums[0]);

    nums[0] = INT_MAX;
    nums[1] = INT_MIN;
    srand(time(0));
    for (int i = 2; i < count; i++)
        nums[i] = 50 - (rand() % 100);

    qsort(nums, count, sizeof(int), nobetter_cmp_int);
    for (int i = 0; i < count; i++)
        printf(" %4d", nums[i]);
    printf("\n");    
    return 0;
}
