#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN 16

void pluto(void)
{
    printf("Whoa! How did I get here??\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    char buf[BUF_LEN];

    printf("What is your name? ");
    if (gets(buf))   // this calls myth's version of gets
        printf("Buffer has space for %zu chars, your name is length %zu.\n", sizeof(buf), strlen(buf));
    return 0;
}
