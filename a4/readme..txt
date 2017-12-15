File: readme.txt
Author: YOUR NAME HERE
----------------------

scandir
1a) Continue causes the remaining portion of while to be skipped.
1b) **names is initialized to NULL, so the first time realloc is called, it functions like malloc.
1c) If realloc fails, it returns NULL, but names still needs to be freed.
1d) Nonzero. ENOMEM=12.
1e) warning: passing argument 4 of ‘qsort’ from incompatible pointer type
1f) sel works on a pointer to a single dirent. qsort works on an array of pointers, each of which points to a dirent.

bsearch
3a) No. If base, nmemb, and width are correct, p cannot go beyond the extent of the array, so no overflow.
3b) (found-arr)/width
3c) The search goes astray but a match is still valid should it find one. So no crash, no false positive, but false negative is possible.

comparison functions
4a)
Output:
correct call max = 0x55443322
incorrect call #1 max = 0xfffff
incorrect call #2 max = 0x44
incorrect call #3 max = 0x900000
Reasons:
incorrect call #1: Only the least significant byte is compared.
incorrect call #2: Only the 3 least significant bytes of the first integer are compared.
incorrect call #3: The address is compared so the third integer comes out on top.
4b) The first parameter of bsearch should be the address of a string instead of the address of a char.
4c) When comparing the key to an element of the array, the former is fed to compar as the first parameter, the latter the second.
4d) See comparison.c.



