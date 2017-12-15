#include "samples/prototypes.h"
#include <error.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[])
{
    if (argc < 3) error(1, 0, "Missing argument(s). Please supply delimiters and string to tokenize.");

    const char *delim = argv[1];
    const char *input = argv[2];

    char copy[strlen(input)+1];
    strcpy(copy, input);       // make copy, strtok is going to destroy it

    printf("Using strtok:     { ");
    for (char *token = strtok(copy, delim); token != NULL; token = strtok(NULL, delim)) {
        printf("\"%s\" ", token);
    }
    printf("}\n");

    char buf[10];
    const char *remaining = input;
    printf("Using scan_token: { ");
    while (scan_token(&remaining, delim, buf, sizeof(buf))) {
        printf("\"%s\" ", buf);
    }
    printf("}\n");

    return 0;
}