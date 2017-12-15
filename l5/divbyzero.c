#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv)
{
    int zero = 0;
    int numerator = 5;
    int result = numerator / zero;
    printf("result: %d\n",result);
    return 0;
}
