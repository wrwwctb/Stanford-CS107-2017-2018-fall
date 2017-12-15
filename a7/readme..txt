File: readme.txt
Author: YOUR NAME HERE
----------------------

test harness
---
1a) Alignment should be in 8 bytes. Tested in verify_block.
1b) By comparing with existing blocks one by one.
1c) After init and after each request.
1d)
// Fill new block with the low-order byte of new id
// can be used later to verify data copied when realloc'ing
memset(p, id & 0xFF, requested_size);

bump allocator
---
2a) 0x107000000. 600.
In parse_script:
  script->segment_size = 2*script->peak;
It will not pass verify_block in eval_correctness.

2b) roundup.
2c) There is no way to decrement next_avail.
2d)
Here memcpy is ok because blocks never overlap.
newsz bytes.
The specification of the standard realloc says: If the area is expanded, the contents of the new part of the array are undefined.
2e) roundup

implicit
---
3a) isalloc, upstairs1
3b)

Robustness: Both are error-free.
Throughput: When bump.c can service a sequence of requests successfully, it is faster. For trace-firefox.script, implicit.c totals 8,274,041,468 instructions, compared with 1,133,931 for bump.c.
Utilization: implicit.c can service all requests. bump.c fails most.

explicit
---
4a) When testing with all scripts, which include a total of 80492 requests, explicit.c uses 34,250,004 instructions, averaging 425.5 per request, compared with 12,146,417,757 instructions and 150.9k on average for implicit.c - a reduction of 99.7%.



Tell us about your quarter in CS107!
---



