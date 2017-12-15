#!/usr/bin/python

import re

# used as postfilter for all assign1 tests
# remap live/empty automata cell from inverted to # and .
# BUT you have to make filter be no-op for the other programs
# that's where the "sniffing" part comes in

def invert_to_plus(text):
    reset_sequence = chr(27) + "\[0m"
    open_sequence = chr(27) + "\[[0-9;]*m"
    regex = "%s[ ]*%s" % (open_sequence, reset_sequence)
    # sniff whether this is automata output being filtered
    # if contains inverted box or only is whitespace and plus chars
    if re.search(regex, text) is not None or re.match("^[ \n\t+]+$", text) is not None:
        text = re.sub(regex, "#", text)  # convert inverted box to #
        text = text.replace('+', '#')    # convert + char to #
        return text.replace(' ', '.')    # convert space to .
    else:
        return text
