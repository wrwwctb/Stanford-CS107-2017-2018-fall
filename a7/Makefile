# A simple makefile for building a program composed of C source files.
# Refer to CS107 guide to Make for background info on makefiles

# EDIT HERE to apply different gcc optimization flags (-Ox and -fxxx)
# Initially, the flags are configured for no optimization (to enable better
# debugging) but you can experiment with different compiler settings
# (e.g. different levels and enabling/disabling specific optimizations)
implicit.o: CFLAGS += -O0
explicit.o: CFLAGS += -O0

ALLOCATORS = bump implicit explicit
PROGRAMS = $(ALLOCATORS:%=test_%)

all:: $(PROGRAMS)

CC = gcc
CFLAGS = -g -std=gnu99 -Wall $$warnflags
export warnflags = -Wfloat-equal -Wtype-limits -Wpointer-arith -Wlogical-op -Wshadow -fno-diagnostics-show-option
LDFLAGS =
LDLIBS =

$(PROGRAMS): test_%:%.o segment.c test_harness.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean::
	rm -f $(PROGRAMS) *.o callgrind.out.*

.PHONY: clean all

.INTERMEDIATE: $(ALLOCATORS:%=%.o)
