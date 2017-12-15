# A simple makefile for building a program composed of C source files.
# Refer to CS107 guide to Make for background info on makefiles

PROGRAMS = trials copy isort

all:: $(PROGRAMS)

CC = gcc
CFLAGS = -g -std=gnu99 -Wall $$warnflags
export warnflags = -Wfloat-equal -Wtype-limits -Wpointer-arith -Wlogical-op -Wshadow -Winit-self -fno-diagnostics-show-option
LDFLAGS =
LDLIBS = -lm

$(PROGRAMS): %:%.c fcyc.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

trials: CFLAGS += -O2

copy: CFLAGS += -O0

# no-stack-protector because char buf used for tmp
isort: CFLAGS += -Og -fno-stack-protector -fno-builtin

clean::
	rm -f $(PROGRAMS) *.o callgrind.out.*

.PHONY: clean all