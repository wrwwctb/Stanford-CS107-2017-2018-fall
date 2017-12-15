/*
 * File: bump.c
 * ------------
 * A very simple "bump" allocator.
 * An allocation request is serviced by tacking on the requested
 * space to the end of the heap thus far. 
 * Free is a no-op: blocks are never coalesced or reused.
 * Realloc is implemented using malloc/memcpy/free. Operations
 * are fast, but utilization is terrible.
 *
 * Shows the very simplest of approaches; there are better options!
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "allocator.h"

#define ALIGNMENT 8

static void *next_avail, *heap_max;

// Efficient bitwise round up to nearest multiple (you saw this code in lab1)
// NOTE: mult has to be power of 2 for the bitwise trick to work!
size_t roundup(size_t sz, size_t mult)
{
    return (sz + mult-1) & ~(mult-1);
}

// initialize global variables based on segment boundaries
bool myinit(void *segment_start, size_t segment_size)
{
    next_avail = segment_start;
    heap_max = (char *)segment_start + segment_size;
    return true;
}

// place block at end of heap
// no search means fast but no recycle makes for terrible utilization
void *mymalloc(size_t requestedsz)
{
    size_t needed = roundup(requestedsz, ALIGNMENT);
    if ((char *)next_avail + needed > (char *)heap_max)
        return NULL;
    void *alloc = next_avail;
    next_avail = (char *)next_avail + needed;
    return alloc;
}

// free does nothing.  fast!... but lame :(
void myfree(void *ptr)
{
}

// realloc built on malloc/memcpy/free is easy to write but not particularly efficient
void *myrealloc(void *oldptr, size_t newsz)
{
    void *newptr = mymalloc(newsz);
    if (!newptr) return NULL;
    memcpy(newptr, oldptr, newsz);
    myfree(oldptr);
    return newptr;
}

// debugging routine to detect/report inconsistencies in heap data structures
bool validate_heap()
{
    if (next_avail > heap_max) {
        printf("Oops! Next available spot is beyond heap end!!\n");
        return false;
    }
    return true;
}
