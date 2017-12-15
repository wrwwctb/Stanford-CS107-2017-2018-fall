/*
 * File: fcyc.c
 * ------------
 * Routines to read x86 cycle counter and do performance
 * measurements based on cycle counts.  This implements the fancy
 * cache-flushing, K-best scheme given in Ch 5
 * Bryant and O'Hallaron.
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "fcyc.h"

/* Default values */
#define K 3                  /* Value of K in K-best scheme */
#define MAXSAMPLES 20        /* Give up after MAXSAMPLES */
#define EPSILON 0.01         /* K samples should be EPSILON of each other*/
#define CLEAR_CACHE 1        /* Clear cache before running test function */
#define CACHE_BYTES (1<<19)  /* Max cache size in bytes */
#define CACHE_BLOCK 32       /* Cache block size in bytes */

static int kbest = K;
static int maxsamples = MAXSAMPLES;
static double epsilon = EPSILON;
static int clear_cache = CLEAR_CACHE;
static int cache_bytes = CACHE_BYTES;
static int cache_block = CACHE_BLOCK;

static int *cache_buf = NULL;

static double *values = NULL;
static int samplecount = 0;

#if defined(__i386__) || defined(__x86_64__)

// last read value of cycle counter stored in static variables
// requires two unsigned, upper & lower
static unsigned cyc_hi = 0;
static unsigned cyc_lo = 0;


/* Set *hi and *lo to the high and low order bits of the cycle counter.  
   Have to use assembly to invoke the rdtsc instruction. */
void read_counter(unsigned *hi, unsigned *lo)
{
    asm volatile("rdtsc; movl %%edx,%0; movl %%eax,%1"
                 : "=r" (*hi), "=r" (*lo)   // two outputs      
                 :                          // no input
                 : "%edx", "%eax");         // clobber list
}

// Record the current value of the cycle counter
void start_counter()
{
    read_counter(&cyc_hi, &cyc_lo);
}

// Return the number of cycles since the last call to start_counter()
double get_counter()
{
    unsigned ncyc_hi, ncyc_lo;
    read_counter(&ncyc_hi, &ncyc_lo);

    /* Do double precision subtraction */
    unsigned lo = ncyc_lo - cyc_lo;
    unsigned borrow = lo > ncyc_lo;
    unsigned hi = ncyc_hi - cyc_hi - borrow;
    double result = (double) hi * (1 << 30) * 4 + lo;
    if (result < 0)
        fprintf(stderr, "Error: counter returns neg value: %.0f\n", result);
    return result;
}
#endif


/* Estimate the clock rate by measuring the cycles that elapse */ 
/* while sleeping for sleeptime seconds */
double mhz()
{
    int sleeptime = 2;
    start_counter();
    sleep(sleeptime);
    return ((double)get_counter())/(1e6*sleeptime);
}



/*
 * fsecs - Return the running time of a function f (in seconds)
 */
double fsecs(void (*fn)(void *), void *argp) 
{
	static double Mhz;  // estimated CPU clock frequency
    static bool init = false;
	if (!init) {
	    Mhz = mhz();
        init = true;
     }
     double cycles = fcyc(fn, argp);
     return cycles/(Mhz*1e6);
}


/* 
 * init_sampler - Start new sampling process 
 */
static void init_sampler()
{
    values = calloc(kbest, sizeof(double));
    samplecount = 0;
}

/* 
 * add_sample - Add new sample  
 */
static void add_sample(double val)
{
    int pos = 0;
    if (samplecount < kbest) {
        pos = samplecount;
        values[pos] = val;
    } else if (val < values[kbest-1]) {
        pos = kbest-1;
        values[pos] = val;
    }
    samplecount++;
    /* Insertion sort */
    while (pos > 0 && values[pos-1] > values[pos]) {
        double temp = values[pos-1];
        values[pos-1] = values[pos];
        values[pos] = temp;
        pos--;
    }
}

/* 
 * has_converged- Have kbest minimum measurements converged within epsilon? 
 */
static int has_converged()
{
    return (samplecount >= kbest) &&
            ((1 + epsilon)*values[0] >= values[kbest-1]);
}

/* 
 * clear - Code to clear cache 
 */
static volatile int sink = 0;

static void cleanup()
{
    free(cache_buf);
    cache_buf = NULL;
}

static void clear()
{
    int x = sink;
    int *cptr, *cend;
    int incr = cache_block/sizeof(int);
    if (!cache_buf) {
        cache_buf = malloc(cache_bytes);
        if (!cache_buf) {
            fprintf(stderr, "Fatal error.  Malloc returned null when trying to clear cache\n");
            exit(1);
        }
       atexit(cleanup);
    }
    cptr = (int *) cache_buf;
    cend = cptr + cache_bytes/sizeof(int);
    while (cptr < cend) {
        x += *cptr;
        cptr += incr;
    }
    sink = x;
}

/*
 * fcyc - Use K-best scheme to estimate the running time of function f
 */
double fcyc(test_funct f, void *argp)
{
    double result;
    init_sampler();
    do {
        double cyc;
        if (clear_cache)
            clear();
        start_counter();
        f(argp);
        cyc = get_counter();
        add_sample(cyc);
    } while (!has_converged() && samplecount < maxsamples);
    result = values[0];
    free(values);
    values = NULL;
    return result;  
}

