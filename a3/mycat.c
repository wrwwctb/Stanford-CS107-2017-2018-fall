#include "samples/prototypes.h"
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *fp;

    if (argc == 1) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) error(1, 0, "%s: no such file", argv[1]);
    }

    char *line;
    for (int i = 1; (line = read_line(fp)) != NULL; i++) {
        printf("%6d  %s\n", i, line);
        free(line);
    }
    fclose(fp);
    return 0;
}
