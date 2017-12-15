#include "samples/prototypes.h"
#include <string.h>
void insert(const void *base, size_t *p_nelem, size_t width, void *p, const void *key){
//shift elements from p onwards, including p
//insert key to where p was
  for (void *curr = (char *)base + (*p_nelem-1)*width;
       (size_t)curr >= (size_t)p;
       curr = (char *)curr - width){
    memcpy((char *)curr + width, curr, width);
  }
  memcpy(p, key, width);
  (*p_nelem)++;
}


void *binsert(const void *key, void *base, size_t *p_nelem, size_t width, int (*compar)(const void *, const void *))
{
  if (*p_nelem == 0){
    memcpy(base, key, width);
    (*p_nelem)++;
    return base;
  }
  void *base_o = base;
  for (size_t nremain = *p_nelem; nremain != 0; nremain >>= 1) {
    void *p = (char *)base + (nremain >> 1) * width;
    int sign = compar(key, p);
    if (sign == 0){
      //don't insert when found
      //void *pnew = (char *)p + width;
      //insert(base_o, p_nelem, width, pnew, key); 
      //return pnew;
      return p;
    }
    else{
      if (sign > 0) {  /* key > p: move right */
        if (nremain <= 2){
          void *pnew = (char *)p + width;
          insert(base_o, p_nelem, width, pnew, key);
          return pnew;
        }
        base = (char *)p + width;
        nremain--;
      }else{       /* else move left */
        if (nremain == 1){
          insert(base_o, p_nelem, width, p, key);
          return p;
        }
      }
    }
  }
  return NULL; // FIXME
}
