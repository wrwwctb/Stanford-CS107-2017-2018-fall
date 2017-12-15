#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
int main(){
  printf("(long)NULL: %lx\n", (long)NULL);
  printf("LONG_MAX, LONG_MIN: %lx %lx\n", LONG_MAX, LONG_MIN);
  long ar[11];
  long *pt;
  printf("%lx %lx\n", (long)ar, (long)&ar);
  printf("%lx %lx\n", (long)pt, (long)&pt);

  pt = malloc(11);
  printf("%lx %lx\n", ar[12], pt[12]);
  return 0;
}
