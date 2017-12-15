# A simple makefile for building a program composed of C source files.
# Refer to CS107 guide to Make for background info on makefiles

PROGRAMS = create_password atm 

all:: $(PROGRAMS) protected_atm

CC = gcc
CFLAGS = -g -Og -std=gnu99 -Wall $$warnflags $$nofortify
export warnflags = -Wfloat-equal -Wtype-limits -Wpointer-arith -Wlogical-op -Wshadow -Winit-self -fno-diagnostics-show-option
export nofortify = -fno-builtin -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0
LDFLAGS =
LDLIBS =

$(PROGRAMS): %:%.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

atm: CFLAGS += -fno-stack-protector 
protected_atm: CFLAGS += -fstack-protector

protected_atm: atm.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean::
	rm -f $(PROGRAMS) *.o protected_atm

.PHONY: clean all
