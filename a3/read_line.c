#include "samples/prototypes.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
// initial allocation will be for min size, if not big enough, doubles
// to 64, then 128, then 256, etc. as needed to accommodate the entire line
#define MINIMUM_SIZE 32


char *read_line(FILE *fp){
  size_t current_size = MINIMUM_SIZE;
  char *line = NULL;
  while (1){
    line = realloc(line, current_size);
    assert(line != NULL);
    if (ungetc(getc(fp), fp) == EOF){
      free(line);
      return NULL;
    }
    fgets(line, current_size, fp);
    char *point = strstr(line, "\n");
    if (point){
      *point = '\0';
      return line;
    }else{
      if (ungetc(getc(fp), fp) == EOF){
        return line;
      }else{
        assert(fseek(fp, 1-current_size, SEEK_CUR) == 0);
        current_size *= 2;
      }
    }
  }
  //return NULL; // replace with your implementation 
}
