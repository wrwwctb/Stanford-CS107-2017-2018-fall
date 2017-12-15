#include <dirent.h>
#include <errno.h>
#include <error.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int musl_scandir(const char *path, struct dirent ***res,
                 int (*sel)(const struct dirent *),
                 int (*cmp)(const struct dirent **, const struct dirent **))
{
    DIR *d = opendir(path);
    struct dirent *de, **names = NULL, **tmp;
    size_t cnt = 0, len = 0;

    if (!d) return -1;

    while ((de = readdir(d))) {
        if (sel && !sel(de)) continue;
        if (cnt >= len) {
            len = 2*len+1;
            if (len > SIZE_MAX/sizeof(*names)) break;
            tmp = realloc(names, len * sizeof(*names));
            if (!tmp) break;
            names = tmp;
        }
        names[cnt] = malloc(de->d_reclen);
        if (!names[cnt]) break;
        memcpy(names[cnt++], de, de->d_reclen);
    }

    closedir(d);

    if (errno) {
        if (names) while (cnt-- > 0) free(names[cnt]);
        free(names);
        return -1;
    }
    if (cmp) qsort(names, cnt, sizeof *names, (int (*)(const void *, const void *))cmp);
    *res = names;
    return cnt;
}

void *apple_bsearch(const void *key, const void *base, size_t nmemb, size_t width,
                    int (*compar)(const void *, const void *))
{
    for (size_t nremain = nmemb; nremain != 0; nremain >>= 1) {
        void *p = (char *)base + (nremain >> 1) * width;
        int sign = compar(key, p);
        if (sign == 0)
            return p;
        if (sign > 0) {  /* key > p: move right */
            base = (char *)p + width;
            nremain--;
        }       /* else move left */
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    return 0;
}
