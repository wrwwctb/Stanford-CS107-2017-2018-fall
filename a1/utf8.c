#include "samples/prototypes.h"
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define US (unsigned short)

int to_utf8(unsigned short cp, unsigned char seq[]){
// takes in a utf-8 code point
// returns a utf-8 encoded sequence

  //printf("%hu", (short)0x0800);
  unsigned short edges[4] = {US 0x0000,
                             US 0x0080,
                             US 0x0800};
  if (edges[0] <= cp && cp < edges[1]){
    seq[0] = cp;
    return 1;
  } else if (edges[1] <= cp && cp < edges[2]){
    seq[0] = '\xC0' + (char)((cp & US 0x07C0) >> 6);
    seq[1] = '\x80' + (char) (cp & US 0x003F);
    return 2;
  } else if (edges[2] <= cp){
    seq[0] = '\xE0' + (char)((cp & US 0xF000) >> 12);
    seq[1] = '\x80' + (char)((cp & US 0x0FC0) >> 6);
    seq[2] = '\x80' + (char) (cp & US 0x003F);
    return 3;
  } else
    exit(1);
  return 0;
}




// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------

void print_utf8(unsigned short cp, unsigned char seq[], int len)
{
    int i;
    printf("U+%04X   Hex:", cp);
    for (i = 0; i < len; i++)
       printf(" %02x", seq[i]);
    for ( ; i < 4; i++) printf("   ");
    seq[len] = '\0';
    printf("Character: %s  \n", seq);  
}

unsigned short convert_arg(const char *str, unsigned short low, unsigned short high)
{
    char *end;
    long n = strtol(str, &end, 0);
    if (*end != '\0')
        error(1, 0, "Invalid code point '%s'", str);
    if (n < low || n > high) 
        error(1, 0, "Code point %s not in range [%u, %u]", str, low, high);
    return n;
}

int main(int argc, char *argv[])
{
    if (argc < 2) 
        error(1, 0, "Missing argument. Please supply one or more unicode code points in decimal or hex.");
    
    for (int i = 1; i < argc; i++) {
        unsigned short cp = convert_arg(argv[i], 0, USHRT_MAX);
        unsigned char seq[4];
        int len = to_utf8(cp, seq);
        print_utf8(cp, seq, len);  
    }
    return 0;
}
