/* 
Example program from
https://www.gnu.org/software/libc/manual/html_node/Search_002fSort-Example.html#Search_002fSort-Example

9.4 Searching and Sorting Example

Here is an example showing the use of qsort and bsearch with an array of structures. 
The objects in the array are sorted by comparing their name fields with the strcmp function. 
Then, we can look up individual objects based on their names.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Define an array of critters to sort. */

struct critter
  {
    const char *name;
    const char *species;
  };

struct critter muppets[] =
  {
    {"Kermit", "frog"},
    {"Piggy", "pig"},
    {"Gonzo", "whatever"},
    {"Fozzie", "bear"},
    {"Sam", "eagle"},
    {"Robin", "frog"},
    {"Animal", "animal"},
    {"Camilla", "chicken"},
    {"Sweetums", "monster"},
    {"Dr. Strangepork", "pig"},
    {"Link Hogthrob", "pig"},
    {"Zoot", "human"},
    {"Robin", "bird"},
    {"Dr. Bunsen Honeydew", "human"},
    {"Beaker", "human"},
    {"Swedish Chef", "human"},
    {"Robin", "sidekick of batman"}
  };

int count = sizeof (muppets) / sizeof (struct critter);



/* This is the comparison function used for sorting and searching. */

int critter_cmp(const void *v1, const void *v2)
{
    const struct critter *c1 = v1;
    const struct critter *c2 = v2;
    //return strcmp(c1->name, c2->name);
    int name = strcmp(c1->name, c2->name);
    if (name)
        return name;
    else
        return strcmp(c1->species, c2->species);
}


/* Print information about a critter. */

void print_critter(const struct critter *c)
{
    printf("%s, the %s\n", c->name, c->species);
}


/* Do the lookup into the sorted array. */

void find_critter(const char *name)
{
    struct critter target, *result;
    target.name = name;
    result = bsearch(&target, muppets, count, sizeof (struct critter), critter_cmp);
    if (result)
        print_critter(result);
    else
        printf("Couldn't find %s.\n", name);
}


int main(int argc, char *argv[])
{
    for (int i = 0; i < count; i++)
        print_critter(&muppets[i]);
    printf("\n");

    qsort(muppets, count, sizeof (struct critter), critter_cmp);

    for (int i = 0; i < count; i++)
        print_critter(&muppets[i]);
    printf("\n");

    find_critter("Kermit");
    find_critter("Gonzo");
    find_critter("Janice");

    return 0;
}
