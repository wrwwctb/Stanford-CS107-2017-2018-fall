#!/usr/bin/python

from testing import *
import results


class Allocator(BaseTest):

    def score(self, student_ex, context):
        if student_ex.exitcode == 0 and "successfully" in student_ex.output:
            return results.Correct(short=student_ex.output.strip())
        else:
            return results.Incorrect(short=student_ex.output.strip())

class InstructionCount(BaseTest):

    core_cmd_expansion = "core_cmd /usr/bin/valgrind --tool=callgrind --toggle-collect=my* --callgrind-out-file=/dev/null"

    def score(self, student_ex, context):
        if "ALLOCATOR FAILURE" in student_ex.output:
            return results.Incorrect(short="No instruction count, test_harness reported ALLOCATOR FAILURE")
        try:
            args = self.command.split(' ',1)[1]
            nrequests = int(util.system("grep '^[afr]' %s | wc -l" % args))
        except:
            return results.Incorrect(short="Unable to determine count of requests in scripts")
        try:
            refs = util.match_regex("I\s+refs:\s+((\d|,)+)", student_ex.output)
            ninstr = int(ui.without_commas(refs))
        except:
            return results.Incorrect(short="Unable to determine callgrind count of instructions (scrape failed)")
        return results.Correct(short="Counted %s instructions for %d requests. Average %d instructions/request" % (refs, nrequests, ninstr/nrequests))


class CustomCount(InstructionCount):
    is_custom_template = True

    def init_from_string(self, line, num):
        self.name = "Custom-%d" % num
        command_str = line.strip()  # remove leading/trailing whitespace
        # if exec_name begins with ./  just discard (will allow either name or ./name as convenience)
        command_str = command_str[2:] if command_str.startswith("./") else command_str
        exec_name = command_str.split()[0]  # break off first token
        assert(exec_name in self.executables), "%s is not a valid executable choice, instead use one of %s" % (exec_name, ui.pretty_list(self.executables))
        self.command = "$" + command_str
        return self

    def execute_solution(self, path):
        pass
