#include "samples/prototypes.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

// You wrote this as an exercise in lab2! Grab the code from your lab checkoff sheet
// at https://cs107.stanford.edu/cgi-bin/lab2
const char *get_env_value(const char *envp[], const char *varname)
{
  for (int i = 0; envp[i] != NULL; i++){
    //printf("%s\n", envp[i]);
    char *result = strstr(envp[i], varname);
    if (result == envp[i]){
    //if result is null, don't do anything
    //envp[i] can be null but will be blocked by for
      result += strlen(varname);
      if (*result == '=')
        return ++result;
    }
  }
  return NULL;    // TODO
}


// This main is incomplete. It sketches the expected behavior for the case when
// mywhich is invoked with no arguments. You are to first read and understand 
// this code, work out how to change/extend to suit your needs, and finally add
// comments to document your strategy.

int main(int argc, char *argv[], const char *envp[])
{
  const char *searchpaths = get_env_value(envp, "MYPATH");
  if (searchpaths == NULL)
    searchpaths = get_env_value(envp, "PATH");

  if (argc == 1){
    char dir[PATH_MAX+1];
    const char *remaining = searchpaths;

    printf("Directories in search path:\n");
    while (scan_token(&remaining, ":", dir, sizeof(dir))){
      printf("%s\n", dir);
    }
  }else{
    argc--;
    argv++;
    char dir[PATH_MAX+1];

    //loop through arguments
    for (int i = 0; i < argc; i++){
      //check argument
      if (!strlen(argv[i]))
        continue;
      unsigned char ifwild = 0;
      if (argv[i][0] == '+'){
        argv[i]++;
        if (!strlen(argv[i]))
          continue;
        ifwild = 1;
      }
      const char *remaining = searchpaths;
      //loop through dirs in path
      while (scan_token(&remaining, ":", dir, sizeof(dir))){
        DIR *dp = opendir(dir);
        if (dp == NULL)
          continue;

        struct dirent *entry;
        unsigned char ifbreak = 0;
        //loop through files in dir
        while ((entry = readdir(dp)) != NULL){
          if (!ifwild){
            if (!strcmp(argv[i], entry->d_name)){
              printf("%s/%s\n", dir, entry->d_name);
              ifbreak = 1;
              break;
            }
          }else{
            char *find = strstr(entry->d_name, argv[i]);
            if (find)
              printf("%s/%s\n", dir, entry->d_name);
          }
        }
        closedir(dp);
        if (ifbreak)
          break;
      }
    }
  }
  return 0;
}
