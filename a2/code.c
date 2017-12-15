/*
 * File: code.c
 * ------------
 * Code to read/analyze, CS107 assign2
 * Refer to assignment writeup for more information.
 */
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>


int my_atoi(const char *s)
{
    int n=0, neg=0;
    while (isspace(*s)) s++;
    switch (*s) {
        case '-': neg=1;
        case '+': s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
        n = 10*n - (*s++ - '0');
    return neg ? n : -n;
}


int my_atoi_(const char *s)
{
    int n=0, neg=0;
    while (isspace(*s)) s++;
    switch (*s) {
        case '-': neg=1;
        case '+': s++;
    }
    while (isdigit(*s))
        n = 10*n + (*s++ - '0');
    return neg ? -n : n;
}


char *my_strtok(char * s, const char * sep)
{
    static char *p = NULL;

    if (s == NULL && ((s = p) == NULL))
        return NULL;
    s += strspn(s, sep);
    if (!*s) 
        return p = NULL;
    p = s + strcspn(s, sep);
    if (*p) 
        *p++ = '\0';
    else 
        p = NULL;
    return s;
}


void list_filenames(const char *dirpath)
{
    DIR *dp = opendir(dirpath);
    if (dp == NULL) return;

                   // loop through directory entries
    struct dirent *entry;
    while ((entry = readdir(dp)) != NULL) {
        printf("%s\n",entry->d_name);
    }

    //closedir(dp);
}


void invalid_atoi_calls(void)
{
     printf("my_atoi(\"%s\") = %d\n", "$5", my_atoi("$5"));
     printf("my_atoi(\"%s\") = %d\n", "12:34", my_atoi("12:34"));
     printf("my_atoi(\"%s\") = %d\n", "-2147483649", my_atoi("-2147483649"));
     #warning below is from invalid my_atoi calls that have type mismatch
     int num = 50;
     printf("my_atoi(&num) = %d\n", my_atoi(&num));
     printf("Did I survive here?\n");
     printf("my_atoi(num) = %d\n", my_atoi(num));
     printf("Did I survive here?\n");
}


int main(int argc, char *argv[])
{   
    //invalid_atoi_calls();
    list_filenames(".");
    return 0;
}
