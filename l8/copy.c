/* File: copy.c
 * ------------
 * To compare performance of memcpy on large chunk versus calling memcpy in loop on small chunk
 */
#include <stdio.h>
#include <string.h>
#include "fcyc.h"

void *charloop(void *dst, const void *src, size_t nbytes)
{
    char *d = dst; // use char * to copy by char
    const char *s = src;

    for (long i = 0; i < nbytes; i++)
        d[i] = s[i];
    return dst;
}

void *intloop(void *dst, const void *src, size_t nbytes)
{
    int *d = dst; // use int * to copy by int
    const int *s = src;
    size_t niter = nbytes/sizeof(*d);

    for (long i = 0; i < niter; i++)
        d[i] = s[i];
    return dst;
}

void *longloop(void *dst, const void *src, size_t nbytes)
{
    long *d = dst; // use long * to copy by long
    const long *s = src;
    size_t niter = nbytes/sizeof(*d);

    for (long i = 0; i < niter; i++)
        d[i] = s[i];
    return dst;
}

void *memloop(void *dst, const void *src, size_t nbytes)
{
    size_t chunksz = sizeof(long);
    size_t niter = nbytes/chunksz;

    for (long i = 0; i < niter; i++) // memcpy in units of chunksz
        memcpy((char *)dst + i*chunksz, (char *)src + i*chunksz, chunksz);
    return dst;
}


#define DATA_SIZE (1 << 20)

static struct {
   void *(*fn)(void *, const void *, size_t);
   char *name;
   size_t val;
} trials[] = 
    {{charloop, "loop char (%zu iterations, loop body single char assignment dst[i] = src[i], movb)", DATA_SIZE}, 
    {intloop, "loop int (%zu iterations, loop body single int assignment dst[i] = src[i], movl)", DATA_SIZE/sizeof(int)}, 
    {longloop, "loop long (%zu iterations, loop body single long assignment dst[i] = src[i], movq)", DATA_SIZE/sizeof(long)}, 
    {memloop, "loop memcpy (%zu iterations, loop body memcpy(dst,src,8)", DATA_SIZE/sizeof(long)}, 
    {memmove, "no loop, single call on entire block, memmove(dst,src,%zu)", DATA_SIZE}, 
    {memcpy, "no loop, single call on entire block, memcpy(dst,src,%zu)", DATA_SIZE}};
static const int nfns = sizeof(trials)/sizeof(*trials);

int main(int argc, char *argv[])
{
    char src[DATA_SIZE], dst[DATA_SIZE];

    printf("A block of data of total size %zu bytes is being copied from src to dst.\n", sizeof(src));
    printf("What are the relative costs of copying chunk-by-chunk in a loop versus block copy?\n\n");

    for(int i = 0; i < nfns; i++) {
	    start_counter();
        trials[i].fn(dst, src, sizeof(src));
        int ncycles = get_counter();
        printf("%7.2fM total cycles (%4.1f cycles/byte)\t", ncycles/1e6, (double)ncycles/sizeof(src));
        printf(trials[i].name, trials[i].val);
        printf("\n");
   }
   return 0;
}
