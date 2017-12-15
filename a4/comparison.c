#include <stdio.h>
#include <stdlib.h>

int cmp_ptr(const void *p, const void *q)
{
    return (const char *)p - (const char *)q;
}

int cmp_int(const void *p, const void *q)
{
    return *(const int *)p - *(const int *)q;
}

int cmp_char(const void *p, const void *q)
{
    return *(const unsigned char *)p - *(const unsigned char *)q;
}

int cmp_first_char(const void *p, const void *q)
{
    return **(const char **)p - **(const char **)q;
}

int cmp_asymmetric(const void *p, const void *q)
{
    return *(const char *)p - **(const char **)q;
}

void test_bsearch_old(char *arr[], int n)
{
    char ch = 'z';
    qsort(arr, n, sizeof(arr[0]), cmp_first_char);
    char **found = bsearch(&ch, arr, n, sizeof(arr[0]), cmp_asymmetric);
    printf("\n%c matches? %s\n", ch, found ? *found : "none");
}

void test_bsearch(char *arr[], int n)
{
      char *ch = "z";
      qsort(arr, n, sizeof(arr[0]), cmp_first_char);
      char **found = bsearch(&ch, arr, n, sizeof(arr[0]), cmp_first_char);
      printf("\n%s matches? %s\n", ch, found ? *found : "none");
}

void *gfind_max(void *arr, int n, size_t elemsz, int (*compar)(const void *, const void *))
{
    void *pmax = arr;
    for (int i = 1; i < n; i++) {
        void *ith = (char *)arr + i*elemsz;
        if (compar(ith, pmax) > 0)
            pmax = ith;
    }
    return pmax;
}

void test_max()
{
    int nums[] = {0x55443322, 0xfffff, 0x900000};
    int n = sizeof(nums)/sizeof(nums[0]);

    int max = *(int *)gfind_max(nums, n, sizeof(nums[0]), cmp_int);
    printf("correct call max = %#x\n", max);

    max = *(int *)gfind_max(nums, n, sizeof(nums[0]), cmp_char);
    printf("incorrect call #1 max = %#x\n", max);
    char ch = *(char *)gfind_max(nums, n, 1, cmp_char);
    printf("incorrect call #2 max = %#hhx\n", ch);
    max = *(int *)gfind_max(nums, n, sizeof(nums[0]), cmp_ptr);
    printf("incorrect call #3 max = %#x\n", max);
}

int main(int argc, char *argv[])
{
    test_max();
    test_bsearch(argv+1, argc-1);
    return 0;
}
