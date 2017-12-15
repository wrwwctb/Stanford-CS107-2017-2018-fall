#include <errno.h>
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define INVERTED_BOX "\e[7m \e[0m"

// each cell is drawn as LIVE_STR or EMPTY_STR
// uncomment the alternate #define below to use + as LIVE_STR
// if your terminal doesn't play nice with inverted box
//#define LIVE_STR INVERTED_BOX
#define LIVE_STR "+"
#define EMPTY_STR " "

#define MSB sizeof(long)*CHAR_BIT-1
const int POW2[3] = {1, 2, 4};

void ruleset2array(unsigned char ruleset, int a[]){
/*
translate the bit pattern of ruleset into a bool array
eg:
ruleset 15=00001110
a[] = {0, 1, 1, 1, 0, 0, 0, 0}
*/
  for (int i = 0; i < CHAR_BIT; i++)
    a[i] = (('\1' << i) & ruleset) > '\0';
}

unsigned long acyclicShiftL(unsigned long num, int shift){
// returns num << shift without the cyclic behavior
  if ((shift <= MSB) && shift >= 0)
    return num << shift;
  else
    return 0UL;
}

int findPatternCode(int i, unsigned long gen){
/*
find pattern code
eg if bits are 110, patternCode=6
i: center bit
gen: bit pattern
*/
  int out = 0;
  for (int j = 1; j >= -1; j--)//j is step bit: 1, 0, -1
    out += ((acyclicShiftL(1L, i+j) & gen) > 0) * POW2[j+1];
  //at the start, look at 64th, 63rd, 62nd bits
  //there is no 64th. ok. gives 0 contribution
  return out;
}

unsigned long advance(unsigned long cur_gen, unsigned char ruleset){
// advance cur_gen
  int ruleArray[CHAR_BIT];
  ruleset2array(ruleset, ruleArray);
  unsigned long out = 0;
  for (int i = MSB; i >= 0; i--){
    // start from the 63rd bit
    int patternCode = findPatternCode(i, cur_gen);
    //at the start, look at 64th, 63rd, 62nd bits
    //there is no 64th. ok. gives 0 contribution
    out += (long)ruleArray[patternCode] << i;
    //at the start, feed result to 63nd bit
  }

  return out;
}

void draw_generation(unsigned long gen){
// draw gen, starting from the MSB
  const char *font[] = { EMPTY_STR, LIVE_STR };

  //printf("%s\n", font[gen != 0]);
  for (int i = MSB; i >= 0; i--){
    long bit = 1L << i;
    printf("%s", font[(gen & bit) > 0]);
  }
  printf("\n");
}




// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------


unsigned long convert_arg(const char *str, unsigned long low, unsigned long high, const char *argname)
{
    char *end;
    unsigned long n = strtoul(str, &end, 0);
    if (*end != '\0') 
        error(1, 0, "Invalid number '%s' for %s", str, argname);
    if (n < low || n > high || errno == ERANGE) 
        error(1, 0, "%s %s is not in range [%#lx, %#lx]", argname, str, low, high);
    return n;
}

int main(int argc, char *argv[])
{
    unsigned long gen_0 = 1L << 32;
    unsigned char ruleset = 30;

    if (argc < 2)
        error(1, 0, "Missing argument. Please supply ruleset and optional initial generation.");

    ruleset = convert_arg(argv[1], 0, UCHAR_MAX, "ruleset");
    if (argc > 2) gen_0 = convert_arg(argv[2], 0, ULONG_MAX, "initial generation");

    if (ruleset == 0) {
        draw_generation(gen_0);
    } else {
        unsigned long gen = gen_0;
        for (int i = 0; i < 32; i++) {
            draw_generation(gen);
            gen = advance(gen, ruleset);
        }
    }
    return 0;
}
