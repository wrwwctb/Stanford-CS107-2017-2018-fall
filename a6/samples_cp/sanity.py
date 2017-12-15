#!/usr/bin/python

from testing import *
import results

class CountLines(BaseTest):

    nlevels = 4

    def score(self, student_ex, context):
        nlines = int(student_ex.output)
        return results.Points(score=(nlines >= self.nlevels),
            short="Counted %d lines in your input.txt file, expected %d" % (nlines, self.nlevels))

class ATM(BaseTest):

    def score(self, student_ex, context):
        if student_ex.exitcode == 0:
            if "Cash dispersed" in student_ex.output:
                return results.Correct(short="Mischief managed! $200 to you.")
            else:
                return results.Incorrect(short="You're close! Successful login, but cash not dispensed?")
        elif student_ex.exitcode == 2:
            return results.Incorrect(short="Successful login, but withdrawal denied.")
        else:
            return results.Incorrect(short=student_ex.output)
