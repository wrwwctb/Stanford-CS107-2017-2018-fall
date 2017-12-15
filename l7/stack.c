#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int leaf(int arr[], int unused)
{
    int val = arr[0];
    return val;
}

int locals(int a, int b)
{
    // single local variables likely to end up registers
    // array will be placed on stack
    int arr[] = {1, 2, 3};

    int x = a * 14;
    int y = b - x;
    int z = y * b;
    int q = 10 + z + a;
    int p = q + b;
    return leaf(arr, p);
}

int calls(const char *param)
{
    return strspn(param, "aeiou") +  atoi(param);
}

int main(int argc, char *argv[])
{
    printf("This program demonstrates various behaviors of the runtime stack.\n");
    int val = calls("Stanford");
    val += locals(7, 1);
    printf("All done, goodbye!\n");
    return val;
}
