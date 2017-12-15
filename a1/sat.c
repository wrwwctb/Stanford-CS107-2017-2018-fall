#include "samples/prototypes.h"
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>


long signed_max(int bitwidth){
// return max of 2's complement integer of length bitwidth
  return (1L << (bitwidth-1))-1;
}

long signed_min(int bitwidth){
// return min of 2's complement integer of length bitwidth
  return ~0L << (bitwidth-1);
}

long sat_add(long a, long b, int bitwidth){
// saturation add. get a candidate. detect overflow and underflow
  long c = a + b;// candidate
  long msb = 1L << (bitwidth-1);
  if (!(a & msb) && !(b & msb) && (c & msb)){
    // overflow:
    //  a positive    b positive   c negative
    c = signed_max(bitwidth);
  }else if ((a & msb) && (b & msb) && !(c & msb)){
    // underflow:
    //       a negative   b negative    c positive
    c = signed_min(bitwidth);
  }
  return c;
}




// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------


long convert_arg(const char *str, long low, long high, const char *argname)
{
    char *end;
    long n = strtol(str, &end, 0);
    if (*end != '\0') 
        error(1, 0, "Invalid number '%s'", str);
    if (n < low || n > high || errno == ERANGE) 
        error(1, 0, "Illegal value %s. %s must be in range [%ld, %ld]", str, argname, low, high);
    return n;
}

int main(int argc, char *argv[])
{
    if (argc < 2) error(1, 0, "Missing argument. Please specify the bitwidth.");
    
    int bitwidth = convert_arg(argv[1], 4, sizeof(long)*8, "Bitwidth");
    long min = signed_min(bitwidth);
    long max = signed_max(bitwidth);

    if (argc < 4)  {
        printf("%d-bit signed integer range\n", bitwidth);
        printf("min: %ld   %#018lx\n", min, min);
        printf("max:  %ld   %#018lx\n", max, max);
    } else {
        long a = convert_arg(argv[2], min, max, "Operand");
        long b = convert_arg(argv[3], min, max, "Operand");
        printf("%ld + %ld = %ld\n", a, b, sat_add(a, b, bitwidth));   
    }
    return 0;
}
