#include <error.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *apple_gets(char *buf)
{
    int c;
    static int warned;
    static const char w[] = "warning: this program uses gets(), which is unsafe.\n";

    if (!warned) {
        fprintf(stderr, "%s", w);
        warned = 1;
    }
    char *s = buf;
    while ((c = getc(stdin)) != '\n') {
        if (c == EOF) {
            if (s == buf)
                return NULL;
            else
                break;
        } else {
            *s++ = c;
        }
    }
    *s = '\0';
    return buf;
}


char *musl_gets(char *s)
{
    char *ret = fgets(s, INT_MAX, stdin);
    if (ret && s[strlen(s)-1] == '\n') s[strlen(s)-1] = '\0';
    return ret;
}


void *calloc(size_t m, size_t n)
{
    if (n && m > SIZE_MAX/n) {
        return NULL;
    }
    size_t sz = n * m;
    void *p = malloc(sz);
    if (p != NULL) {
        size_t *wp;
        size_t nw = (sz + sizeof(*wp) - 1)/sizeof(*wp);
        for (wp = p; nw != 0; nw--, wp++) 
            if (*wp) *wp = 0;
    }
    return p;
}


size_t available_size_at_address(void *addr)
{
    // The internal record-keeping of the heap allocator
    // can determine the available space for a given heap address
    // but we are not privy to that information (yet! :-)
    error(1, 0, "Privileged operation for the heap allocator only!");
    return 0;
}

void *realloc(void *oldptr, size_t newsz)
{
    size_t oldsz = available_size_at_address(oldptr);

    if (newsz == 0) {
        free(oldptr);
        return NULL;
    }
    if (oldsz >= newsz) {
        return oldptr;
    }
    void *newptr = malloc(newsz);
    if (newptr != NULL) {
        if (oldptr != NULL) memcpy(newptr, oldptr, oldsz);
        free(oldptr);
    }
    return newptr;
}

int main(int argc, char *argv[])
{
    char buf[16];

    printf("This is the assign3 code program.\nWhat is your name? ");
    if (gets(buf))   // this calls myth's version of gets
        printf("Buffer is size %zu, your name is size %zu.\n", sizeof(buf), strlen(buf));
    return 0;
}
