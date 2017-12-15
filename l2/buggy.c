/* File: buggy.c
 * --------------
 * Intentionally buggy cdoe to observe under Valgrind.
 * Read lab2 writeup for more information.
 */

#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void make_error_1(void)
{
    printf("--- Making error 1: strlen called on uninitialized pointer\n\n");
    char *uninitialized_ptr = "";//NULL wouldn't do
   
    printf("strlen(uninitialized_ptr) = %lu\n", strlen(uninitialized_ptr));
}

void make_error_2(void)
{
    printf("--- Making error 2: strlen called on string without null terminator\n\n");

    char stackarr[10];
    strncpy(stackarr, "Stanford", 3); 
    printf("strlen(non-terminated) = %lu\n", strlen(stackarr)); 
}

void make_error_3(void)
{
    printf("--- Making error 3: strcpy called on dst that is too small\n\n");

    char *dst = malloc(5);
    strcpy(dst, "Stanford");
    printf("strcpy(toosmall, \"Stanford\") = %s\n", dst);
}

int convert_arg(const char *arg)
{
    int num = 0;
    if (arg) num = atoi(arg);
    return num;
}

int main(int argc, char *argv[])
{
    switch(convert_arg(argv[1])) {
        case 1: make_error_1(); return 0;
        case 2: make_error_2(); return 0;
        case 3: make_error_3(); return 0;
        default: error(1, 0, "required argument is number 1, 2, or 3");
    }
}
