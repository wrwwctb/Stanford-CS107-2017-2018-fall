/* File: buggy.c
 * --------------
 * Intentionally buggy cdoe to observe under Valgrind.
 * Read lab3 writeup for more information.
 */

#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void make_error_1(const char *arg)
{
    printf("--- Making error 1: write past end of stack-allocated array\n\n");
   
    char buf[6];
    strcpy(buf, arg);
    printf("address %p of stack array size 6, contents '%s' uses size %zu\n", buf, buf, strlen(buf)+1);
}

void make_error_2(const char *arg)
{
    printf("--- Making error 2: write past end of heap-allocated memory\n\n");
   
    char *ptr = malloc(6);
    strcpy(ptr, arg);
    printf("ptr %p to heap memory size 6, contents '%s' uses size %zu\n", ptr, ptr, strlen(ptr)+1);
    free(ptr);
}

void make_error_3(void)
{
    printf("--- Making error 3: use pointer to freed heap memory\n\n");

    char *ptr = malloc(10);
    strcpy(ptr, "Junior");
    free(ptr);
    printf("ptr %p to freed heap memory, contents='%s'\n", ptr, ptr);
}

void make_error_4(void)
{
    printf("--- Making error 4: double free heap memory\n\n");

    char *ptr = strdup("University");   // strdup does malloc+strcpy
    free(ptr);
    free(ptr);
}

int main(int argc, char *argv[])
{
    int num = argc > 1 ? atoi(argv[1]) : 0;
    const char *argument = argc > 2 ? argv[2] : "eliza";

    switch(num) {
        case 1: make_error_1(argument); return 0;
        case 2: make_error_2(argument); return 0;
        case 3: make_error_3(); return 0;
        case 4: make_error_4(); return 0;
        default: error(1, 0, "required argument is number 1, 2, 3, or 4");
    }
}
