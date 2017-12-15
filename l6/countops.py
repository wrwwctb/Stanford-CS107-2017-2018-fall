#!/usr/bin/env python

"""
Quick little python script to count opcodes on disassembled ojbect
file and report top 10 most often used instructions with percentage.

jzelenski Sun Oct 30 14:56:27 PDT 2011
"""

import sys
import re
import commands

def count_opcodes(output):
    counts = {}
    for line in output.splitlines():
        found = re.match('\s*[0-9a-f]+:\s*([a-z]+)', line)
        if found:
            opcode = found.group(1)
            if opcode[-1] in 'bwlq': opcode = opcode[:-1]  # cheesy elide of width suffix (not quite right, but close enough)
            if opcode in counts:
                counts[opcode] += 1
            else:
                counts[opcode] = 1
    return counts


def print_histogram(counts):
    if not counts.items():
        print "No instructions to count. (Was object file invalid?)"
    total = sum(counts.values())
    for e in sorted(counts.items(), key=lambda x: (-x[1], x[0]))[:10]:
        print "   %-6s %4d (%d%%)" % (e[0], e[1], (e[1]*100)/total)


if __name__ == '__main__':
    if len(sys.argv) != 2:  # expects filename argument
        print "usage: countops object-file"
        sys.exit(1)
    status, output = commands.getstatusoutput("objdump -d --no-show-raw-insn  %s" % sys.argv[1])
    print_histogram(count_opcodes(output))
