#include "samples/prototypes.h"
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define MAX_NLINES_ON_STACK 10000

void print_last_n(FILE *fp, int n)
{
  char *lines[MAX_NLINES_ON_STACK];
  char **p;
  if (n > MAX_NLINES_ON_STACK){
    p = calloc(n, sizeof(char*));
    assert(p != NULL);
  }else
    p = lines;

  for(size_t i = 0; i < n; i++)
    p[i] = NULL;

  size_t i = 0;
  while (ungetc(getc(fp), fp) != EOF){
    if (p[i%n] != NULL)
      free(p[i%n]);
    p[i%n] = read_line(fp);
    i++;
  }

  size_t s = (i>n)?i%n:0;

  for (size_t j = 0; j < ((i>n)?n:i); j++)
                      // < precedes ?..
    printf("%s\n", p[(s+j)%n]);


  for(size_t j = 0; j < n; j++)
    if (p[j] != NULL)
      free(p[j]);
  if (n > MAX_NLINES_ON_STACK)
    free(p);
}

int convert_arg(const char *str)
{
    char *end;
    long n = strtol(str, &end, 10);
    if (*end != '\0') 
        error(1, 0, "Invalid number '%s'", str);
    if (n < 1 || n > INT_MAX) 
        error(1, 0, "%s is not within the acceptable range [%d, %d]", str, 1, INT_MAX);
    return n;
}

int main(int argc, char *argv[])
{
    int num = 10;

    if (argc > 1 && argv[1][0] == '-') {
        num = convert_arg(argv[1] + 1);
        argv++;
        argc--;
    }

    FILE *fp;
    if (argc == 1) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) error(1, 0, "%s: no such file", argv[1]);
    }
    print_last_n(fp, num);
    fclose(fp);
    return 0;
}
