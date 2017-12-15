/* floats.c
 * --------
 * A program to explore the behavior of floating point types for lab.
 */
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


// Just a little helper to convert bool to string value when printing
static char *boolstr(bool b)
{
    return b ? "true" : "false";
}


// helper to display raw float bits
static void print_float_bits(float f, const char *name, const char *comment)
{
    unsigned int bits = *(unsigned int *)&f; // trick compiler into giving us access to raw bits
    unsigned int mask = 1 << 31;
    printf(" %d ", (mask & bits) != 0); // msb is sign bit
    for (int i = 0; i < 8; i++)         // next 8 bits are exponent
        printf("%d", ((mask >>= 1) & bits) != 0);
    printf(" ");
    for (int i = 0; i < 23; i++)        // remaining 23 bits are significand
        printf("%d", ((mask >>= 1) & bits) != 0); 
    printf(" raw bits of %s ", name);
    if (!comment) printf("(float value = %g)\n", f);
    else printf("(%s)\n", comment);
}



/* Function: float_behavior
 * ------------------------
 * Tries various floating point operations and prints results so you can observe beahvior.
 */
static void float_behavior()
{
    printf("sizeof(float) = %zu  FLT_MIN = %g  FLT_MAX = %g  FLT_DIG = %d (num decimal digits of precision)\n",
           sizeof(float), FLT_MIN, FLT_MAX, FLT_DIG);
    printf("sizeof(double) = %zu  DBL_MIN = %g  DBL_MAX = %g  DBL_DIG = %d\n\n",
          sizeof(double), DBL_MIN, DBL_MAX, DBL_DIG);

    printf("max * 2 = %g\n", DBL_MAX*2);
    printf("1.0/0.0 = %g\n", 2.0/0.0);
    printf("0.0/0.4 = %g\n", 0.0/0.4);

    double infinity = 1.0/0.0;
    printf("inf * 2 = %g\n", infinity * 2);
    printf("inf - inf = %g\n", infinity - infinity);
    printf("does inf == inf? %s, does inf == -inf? %s\n", boolstr(infinity == infinity), boolstr(infinity == -infinity));

    double nan = 0.0/0.0;
    printf("nan * 2 = %g\n", nan * 2);
    printf("nan - nan = %g\n", nan - nan);
    printf("does nan == inf? %s, does nan == nan? %s\n", boolstr(nan == infinity), boolstr(nan == nan));
}


/* Function: float_bits
 * --------------------
 * Prints bit patterns for various float values.
 */
static void float_bits()
{
    unsigned int rawbits = 1;
    float f = *(float *)&rawbits;

    printf("\nFloat bit patterns:\n");
    print_float_bits(FLT_MAX, "FLT_MAX", NULL);
    print_float_bits(FLT_MIN, "FLT_MIN", NULL);
    print_float_bits(f, "smallest non-zero denorm", NULL);
    print_float_bits(1.0/0.0, "Infinity", NULL);
    print_float_bits(0.0/0.0, "nan", NULL);
    print_float_bits(1.0, "1.0", NULL);
    print_float_bits(1.5, "1.5", "how did float bits change from 1.0 to 1.5?");
    print_float_bits(1.25, "1.25", "how did float bits change from 1.5 to 1.25?");
    print_float_bits(-1.25, "-1.25", "how did negation change float bits?");
    print_float_bits(127.0, "127.0", "note bit pattern for integer 127 appears within float bits!");
    print_float_bits(126.0, "126.0", "how did subtract 1 change float bits?");
    print_float_bits(126.5, "126.5", "how did add 0.5 change float bits?");
    print_float_bits(253.0, "253.0", "how did multiply by 2 change float bits of norm?");
    print_float_bits(f, "smallest non-zero denorm", NULL);
    print_float_bits(f*2, "denorm*2", "how did multiply by 2 change float bits of min denorm?");
    print_float_bits(.5, ".5", "2 to power -1");
    print_float_bits(8, "8", "2 to power 3");
    print_float_bits(1024, "1024", "2 to power 10");
    print_float_bits(32768, "32768", "what do normalized powers of two have in common?");
}

int main(int argc, char *argv[])
{
    float_behavior();
    float_bits();
    return 0;
}

