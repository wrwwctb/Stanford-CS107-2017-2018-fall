#include "samples/prototypes.h"
#include <string.h>

bool scan_token(const char **p_input, const char *delimiters, char buf[], size_t buflen)
{
/*
 1    char *strtok(char *s, const char *sep)
 2    {
 3         static char *p = NULL;
 4
 5         if (s == NULL && ((s = p) == NULL))
 6             return NULL;
 7         s += strspn(s, sep);
 8         if (!*s)
 9             return p = NULL;
10         p = s + strcspn(s, sep);
11         if (*p)
12             *p++ = '\0';
13         else 
14              p = NULL;
15         return s;
16     }
*/
  size_t dellen = strspn(*p_input, delimiters);
  *p_input += dellen;
  size_t toklen = strcspn(*p_input, delimiters);
  if (toklen > 0){
    if (toklen >= buflen)
      toklen = buflen-1;
    strncpy(buf, *p_input, toklen);
    buf[toklen] = '\0';
    *p_input += toklen;
    return true;
  }else
    return false;
}
