#include "samples/prototypes.h"
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
void print_uniq_lines(FILE *fp){
  char *prev = NULL, *curr = NULL;
  size_t count = 1;
  while (1){
    if (ungetc(getc(fp), fp) == EOF){
      if (prev != NULL){
        printf("%lu %s\n", count, prev);
        free(prev);
      }
      return;
    }
    curr = read_line(fp);
    if (prev != NULL){
      if (strcmp(prev, curr)){
        printf("%lu %s\n", count, prev);
        count = 1;
      }else
        count++;
      free(prev);
    }
    prev = curr;
  }
}
/*old version without numbers
void print_uniq_lines(FILE *fp){
  char *prev = NULL, *curr = NULL;
  while(ungetc(getc(fp), fp) != EOF){
    curr = read_line(fp);
    if (prev == NULL || strcmp(prev, curr) != 0){
      printf("%s\n", curr);
      prev = realloc(prev, strlen(curr)+1);
      assert(prev != NULL);
      strcpy(prev, curr);
    }
    free(curr);
  }
  if (prev != NULL)
    free(prev);
}
*/

int main(int argc, char *argv[])
{
    FILE *fp;

    if (argc == 1) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) error(1, 0, "%s: no such file", argv[1]);
    }
    print_uniq_lines(fp);
    fclose(fp);
    return 0;
}
