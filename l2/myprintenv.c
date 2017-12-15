/* 
 * File: myprintenv.c
 * ------------------
 * Read lab2 writeup for more information.
 */

#include <stdio.h>
#include <string.h>

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


/* Function: main
 * --------------
 * Ordinarily main() has just two arguments (argc/argv); this alternate
 * prototype includes an optional third argument which is an array of
 * environment variables (like argc/argv, the values passed to this third
 * arg are set automatically by the operating system when the program is 
 * launched). Programs can use either form for main. The 2-argument 
 * version is more common, but this program will use the main with
 * 3rd argument to get access to the environment variables.
 * (In fact, many programmers don't even know about this optional 3rd
 * argument, so now you're already elite! :-) )
 *
 * Each array entry in this third argument is a string (pointer char*), 
 * where the string pointed to is of the form "NAME=value", for different
 * environment variable NAMEs and configuration values. The array's last 
 * entry is a NULL pointer, which acts as a sentinel to identify the
 * end of the array (which is why we can get away with not including an 
 * accompanying array size argument, as we would normally do when passing 
 * arrays in C). 
 */
int main(int argc, char *argv[], const char *envp[])
{
    if (argc == 1) {
        for (int i = 0; envp[i] != NULL; i++)
            printf("%s\n", envp[i]);
    } else {
        for (int i = 1; i < argc; i++) {
            const char *value = get_env_value(envp, argv[i]);
            if (value != NULL)  // if not found, don't print anything
                printf("%s\n", value);
        }
    }
    return 0;
}
