/* nearest.c
 * ---------
 * Lab exercises to explore the limits of finite representation and use of nearest representable float.
 */
#include <float.h>
#include <stdio.h>


static void float_rounding()
{
    float f;

    // rounding on assignment
    f = 0.4;
    printf("constant 0.4 rounded on assignment to float? = %.9f\n", f);

    f = 9.25;
    printf("constant 9.25 rounded on assignment to float? = %.9f\n", f);
    f = 1000.25;
    printf("constant 1000.25 rounded on assignment to float? = %.9f\n", f);
    f = 1000000000.25;
    printf("constant 1000000000.25 rounded on assignment to float? = %.9f\n", f);

}

#include <stdbool.h>

static bool int_to_float(int before)
{
    float as_float = before; // round trip int->float->int
    int after = as_float;
    printf("int = %d, converted to float = %f, converted back to int = %d\n", before, as_float, after);
    return before == after;
}

static bool float_to_int(float before)
{
    int as_int = before;   // round trip float->int->float
    float after = as_int;
    printf("float = %f, converted to int = %d, converted back to float = %f\n", before, as_int, after);
    return !(before < after || before > after);
}

// Some code to experiment with converting between int <=> float
// Try choosing your own int/floats and seeing how they fares during conversion
static void float_conversions()
{
    int_to_float(-4);
    int_to_float(123456789);

    float_to_int(107.75);
    float_to_int(FLT_MAX);
}


int main(int argc, char *argv[])
{
    printf("Examples of rounding on assignment and during operations:\n");
    float_rounding();
    printf("\nConversions between int <-> float:\n");
    float_conversions();
    return 0;
}

