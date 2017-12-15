#include "samples/prototypes.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


int cmp_int(const void *p, const void *q)
{
    int first = *(const int *)p, second = *(const int *)q;
    return first - second;
}

int main(int argc, char *argv[])
{
    int arr[argc];
    size_t nused = 0;

    for (int i = 1; i < argc; i++) {
        int val = atoi(argv[i]);        
        int *added = binsert(&val, arr, &nused, sizeof(arr[0]), cmp_int);
        assert(val == *added);
    }

    for (int i = 0; i < nused; i++)
        printf(" %d", arr[i]);
    printf("\n");    
    return 0;
}