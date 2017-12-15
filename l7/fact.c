#include <error.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int factorial(unsigned int n)
{
    if (n <= 1) return 1;
    return n*factorial(n-1);
}

int main(int argc, char *argv[])
{
    if (argc == 1) error(1, 0, "missing argument. Please supply one or more numbers.");
    for (int i = 1; i <  argc; i++) {
        int num = atoi(argv[i]);
        printf("factorial(%d) = %u\n", num, factorial(num));       
    }
    return 0;
}
