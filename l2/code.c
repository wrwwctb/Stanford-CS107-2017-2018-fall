/* File: code.c
 * ------------
 * Code passages to read as part of lab.
 * Read lab2 writeup for more information.
 */

#include <ctype.h>
#include <stdio.h>

int my_isxdigit(int c)
{
    return isdigit(c) || ((unsigned)c|32)-'a' < 6;
}

char *my_strcpy(char *dst, const char *src)
{
    char *result = dst;
    while ((*dst++ = *src++)) ;
    return result;
}

// implementation taken from the strncpy man page
char *my_strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++)
         dest[i] = src[i];
    for ( ; i < n; i++)
        dest[i] = '\0';
    return dest;
}


int main(int argc, char *argv[])
{
    printf("my_isxdigit(%c) = %d\n", 'e', my_isxdigit('e'));
    printf("my_isxdigit(%c) = %d\n", '#', my_isxdigit('#'));

    char buf[8];
    printf("PROTIP:\tThe gdb command 'x/8bc buf' shows the contents of buf.\n"
        "\tStep execution with next and use x to see how contents change.\n");

    my_strcpy(buf, "abc"); 
    my_strncpy(buf, "defghijklmnop", 4);
    buf[4] = '\0';
    my_strncpy(buf, "yz", 8);
}