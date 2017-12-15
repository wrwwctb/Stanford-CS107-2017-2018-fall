#include "allocator.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define ALIGNMENT 8
#define SIZE_T 8
#define MINBLK 32
#define PTRSZ 16
#define HEADFOOT_NOPTR 16
#define ONE     0x0000000000000001
#define MULT8   0xFFFFFFFFFFFFFFF8
#define NOT_ONE 0xFFFFFFFFFFFFFFFE

//head of free list
static char *head;
//min acceptable ptr: seg min rounded up
static char *heap_min;
//max acceptable ptr: (seg min + seg sz) rounded down - min blk sz
static char *heap_max;

/*
assumptions
- when given a block pointer char*ptr, it's ok to access its header and footer
*/
size_t rounddown(size_t, size_t);
inline size_t rounddown(size_t sz, size_t mult){
  return ~(mult-1) & sz;
}

size_t roundup(size_t, size_t);
inline size_t roundup(size_t sz, size_t mult){
  return (sz + mult-1) & ~(mult-1);
}

char *getsizeptr1(char *);
inline char *getsizeptr1(char *ptr){
  return ptr;
}

size_t ptr2size(char *);
inline size_t ptr2size(char *ptr){
  return *(size_t*)ptr & MULT8;
}

size_t getsize(char *);
inline size_t getsize(char *ptr){
  return ptr2size(getsizeptr1(ptr));
}

char *getsizeptr2(char *);
inline char *getsizeptr2(char *ptr){
  ptr += getsize(ptr) - SIZE_T;
  return ptr;
}

void setsize_setfree(char *, size_t);
inline void setsize_setfree(char *ptr, size_t sz){
  *(size_t*)ptr = sz;
  ptr += sz - SIZE_T;
  *(size_t*)ptr = sz;
}

void setsize_setalloc(char *, size_t);
inline void setsize_setalloc(char *ptr, size_t sz){
  *(size_t*)ptr = sz | ONE;
  ptr += sz - SIZE_T;
  *(size_t*)ptr = sz | ONE;
}

bool isalloc(char *);
inline bool isalloc(char *ptr){
  return *getsizeptr1(ptr) & 0x01;
}

bool setalloc(char *);
inline bool setalloc(char *ptr){
  char *sizeptr1 = getsizeptr1(ptr);
  char *sizeptr2 = getsizeptr2(ptr);
  bool orig = *sizeptr1 & 0x01;
  *sizeptr1 = *sizeptr1 | 0x01;
  *sizeptr2 = *sizeptr1;
  return orig;
}

bool setfree(char *);
inline bool setfree(char *ptr){
  char *sizeptr1 = getsizeptr1(ptr);
  char *sizeptr2 = getsizeptr2(ptr);
  bool orig = *sizeptr1 & 0x01;
  *sizeptr1 = *sizeptr1 & 0xFE;
  *sizeptr2 = *sizeptr1;
  return orig;
}

char *getprevptr(char *ptr);
inline char *getprevptr(char *ptr){
  return (char *)*(size_t*)(ptr + SIZE_T);
}

void setprevptr(char *, char *);
inline void setprevptr(char *ptr, char *prev){
  ptr += SIZE_T;
  *(size_t*)ptr = (size_t)prev;
}

char *getnextptr(char *ptr);
inline char *getnextptr(char *ptr){
  return (char *)*(size_t*)(ptr + 2*SIZE_T);;
}

void setnextptr(char *, char *);
inline void setnextptr(char *ptr, char *next){
  ptr += 2*SIZE_T;
  *(size_t*)ptr = (size_t)next;
}

//given ptr of a free blk, remove blk from free list
void bypasslist(char *);
inline void bypasslist(char *ptr){
  char *prev = getprevptr(ptr);
  char *next = getnextptr(ptr);
  if (prev != NULL)
    setnextptr(prev, next);
  if (next != NULL)
    setprevptr(next, prev);
  if (head == ptr)
    head = next;
}

//sz must be getsize(ptr)
char *upstairs2(char *, size_t);
inline char *upstairs2(char *ptr, size_t sz){
  ptr += sz;
  if (ptr > heap_max)
    return NULL;
  return ptr;
}

char *upstairs1(char *);
inline char *upstairs1(char *ptr){
  return upstairs2(ptr, getsize(ptr));
}

char *downstairs(char *);
inline char *downstairs(char *ptr){
  if (ptr < heap_min + MINBLK)
    return NULL;
  return ptr - ptr2size(ptr - SIZE_T);
}

//how about a while loop
//sz must be &(getsize(ptr))
void check_bypass_up(char *, size_t *);
inline void check_bypass_up(char *ptr, size_t *sz){
  ptr = upstairs2(ptr, *sz);
  if (ptr != NULL && !isalloc(ptr)){
    *sz += getsize(ptr);
    bypasslist(ptr);
  }
}

void check_bypass_down(char **, size_t *);
inline void check_bypass_down(char **ptr, size_t *sz){
  char *down = downstairs(*ptr);
  if (down != NULL && !isalloc(down)){
    *sz += getsize(down);
    bypasslist(down);
    *ptr = down;
  }
}

//given ptr and sz of a blk, initialize blk to free
//attach blk to front of free list
void initutil(char *, size_t);
inline void initutil(char *ptr, size_t sz){
  check_bypass_up(ptr, &sz);
  check_bypass_down(&ptr, &sz);
  if (head != NULL){
    setprevptr(head, ptr);
  }
  setnextptr(ptr, head);
  setprevptr(ptr, NULL);
  setsize_setfree(ptr, sz);
  head = ptr;
}

//input requested size
//output aligned block size
size_t pumpsz(size_t);
inline size_t pumpsz(size_t sz){
  if (sz < PTRSZ)
    return MINBLK;
  sz = roundup(sz, ALIGNMENT);
  return sz + HEADFOOT_NOPTR;
}

////////////////////////////////////////

bool myinit(void *, size_t);
inline bool myinit(void *reqstart, size_t reqsz){
  if (reqstart == NULL)
    return false;
  char *reqend = (char *)reqstart + reqsz;
  // are these aligned already?
  char *start = (char*)roundup((size_t)reqstart, ALIGNMENT);
  char *end = (char*)rounddown((size_t)reqend, ALIGNMENT);
  size_t sz = end - start;
  heap_min = (char*)start;
  heap_max = (char*)start + sz - MINBLK;
  head = NULL;
  initutil(start, sz);
  return true;
}

void *mymalloc(size_t sz){
  sz = pumpsz(sz);
  char *ptr = head;
  //first fit
  while (ptr != NULL){
    size_t blocksz = getsize(ptr);

    //has needed space +  min free block in bytes. split
    if (blocksz >= sz + MINBLK){
      bypasslist(ptr);
      setsize_setalloc(ptr, sz);
      initutil(ptr + sz, blocksz - sz);
      return ptr + SIZE_T;//internal -> client ptr
    }
    //not enough to split. allocate whole
    if (blocksz >= sz){
      bypasslist(ptr);
      setalloc(ptr);
      return ptr + SIZE_T;
    }
    ptr = getnextptr(ptr);
  }
  return NULL;
}

void myfree(void *);
inline void myfree(void *ptrv){
  char *ptr = (char *)ptrv - SIZE_T;//client -> internal ptr
  if (ptr < heap_min ||
      ptr > heap_max ||
      !isalloc(ptr)){
    //printf("\ninvalid free\nheap_min %lx\nheap_max %lx\nptr %lx\n",
    //       (size_t)heap_min, (size_t)heap_max, (size_t)ptr);
    //exit(1346347);
  }else{
    initutil(ptr, getsize(ptr));
  }
}

void *myrealloc(void *oldptrv, size_t newsz){
  char *oldptr = (char *)oldptrv - SIZE_T;//client -> internal ptr
  newsz = pumpsz(newsz);
  size_t oldsz = getsize(oldptr);
  if (oldsz >= newsz + MINBLK){
    setsize_setalloc(oldptr, newsz);
    initutil(oldptr + newsz, oldsz - newsz);
    return oldptrv;//client ptr
  }
  if (oldsz >= newsz)
    return oldptrv;//client ptr

  char *up = upstairs2(oldptr, oldsz);
  char *down = downstairs(oldptr);
  size_t szup = (up != NULL && !isalloc(up))?
                getsize(up):0;
  size_t szdown = (down != NULL && !isalloc(down))?
                getsize(down):0;

  if (oldsz + szup >= newsz + MINBLK){
    bypasslist(up);
    setsize_setalloc(oldptr, newsz);
    initutil(oldptr + newsz, oldsz + szup - newsz);
    return oldptr + SIZE_T;//internal -> client ptr
  }
  if (oldsz + szup >= newsz){
    bypasslist(up);
    setsize_setalloc(oldptr, oldsz + szup);
    return oldptr + SIZE_T;//internal -> client ptr
  }
  if (oldsz + szdown >= newsz + MINBLK){
    bypasslist(down);
    char *newptr = oldptr + oldsz - newsz;
    memmove(newptr, oldptr, oldsz);
    setsize_setalloc(newptr, newsz);
    initutil(down, oldsz + szdown - newsz);
    return newptr + SIZE_T;//internal -> client ptr
  }
  if (oldsz + szdown >= newsz){
    bypasslist(down);
    memmove(down, oldptr, oldsz);
    setsize_setalloc(down, oldsz + szdown);
    return down + SIZE_T;//internal -> client ptr
  }
  if (oldsz + szdown + szup >= newsz + MINBLK){
    bypasslist(up);
    bypasslist(down);
    char *newptr = oldptr + oldsz + szup - newsz;
    memmove(newptr, oldptr, oldsz);
    setsize_setalloc(newptr, newsz);
    initutil(down, oldsz + szup + szdown - newsz);
    return newptr + SIZE_T;//internal -> client ptr
  }
  if (oldsz + szdown + szup >= newsz){
    bypasslist(up);
    bypasslist(down);
    memmove(down, oldptr, oldsz);
    setsize_setalloc(down, oldsz + szdown + szup);
    return down + SIZE_T;//internal -> client ptr
  }
  char *ptr = head;
  while (ptr != NULL){
    size_t ptrsz = getsize(ptr);
    if (ptrsz >= newsz + MINBLK){
      bypasslist(ptr);
      memcpy(ptr, oldptr, oldsz);
      //what if newsz real small? no way. newsz > oldsz here
      setsize_setalloc(ptr, newsz);
      initutil(ptr + newsz, ptrsz - newsz);
      initutil(oldptr, oldsz);
      return ptr + SIZE_T;//internal -> client ptr
    }
    if (ptrsz >= newsz){
      bypasslist(ptr);
      memcpy(ptr, oldptr, oldsz);
      setsize_setalloc(ptr, ptrsz);
      initutil(oldptr, oldsz);
      return ptr + SIZE_T;//internal -> client ptr
    }
    ptr = getnextptr(ptr);
  }
  return NULL;
}

bool validate_heap(){
  return true;
}
