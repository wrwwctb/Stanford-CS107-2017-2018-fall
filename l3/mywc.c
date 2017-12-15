/* 
 * File: mywc.c
 * ------------
 * Read lab3 writeup for more information.
 */

#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 128

void count_input(FILE *fp)
{
    int nlines = 0, nchars = 0;
    char line[MAX_LINE_LEN];
    char *longest = strdup("");

    while (fgets(line, sizeof(line), fp) != NULL) {
        nlines++;
        int len = strlen(line);
        nchars += len;
        if (len >= strlen(longest)) {
            //longest = line;
            free(longest);
            longest = strdup(line);
        }
    }
    printf("\nNum lines = %4d\nNum chars = %4d\n" , nlines, nchars);
    printf("Longest = %s\n", longest);
    free(longest);
}


int main(int argc, char *argv[])
{
    FILE *fp;

    if (argc == 1) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) error(1, 0, "%s: no such file", argv[1]);
    }
    count_input(fp);
    fclose(fp);
    return 0;
}
