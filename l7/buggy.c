#include <stdio.h>

#define N_ELEMS 5

long clear_array(void)
{
    long i, arr[N_ELEMS];

    for (i = 1; i <= N_ELEMS; i++) {  // BUGGY! Writes one past end of array
        arr[i] = 0;
    }
    printf("Finished clear\n");
    return arr[0];
}

int main(int argc, char *argv[])
{   
    clear_array();
    printf("All done, goodbye!\n");
    return 0;
}
