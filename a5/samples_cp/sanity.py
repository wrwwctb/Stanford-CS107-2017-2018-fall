#!/usr/bin/python

import datetime, glob

from testing import *
import results

def percent_diff(exp,correct):
    if correct == 0:
        return None
    return (abs(exp-correct)/(float(correct))) * 100.0

def get_clock_freq():
    with open("/proc/cpuinfo") as f:
        for line in f:
            if ("cpu MHz" in line):
                try:
                    return str(int(float(line.split(':')[1]) * 1e6))
                except:
                    return "error converting output to value" # float probably too big...

class TestTimerRealFreq(VersusSolution):
    def command_for_display(self, for_soln=False):
        if self.command:
            # expand command, but delete the "core_cmd" call, what is printed will mostly copy/paste to shell
            env = self.soln_env() if for_soln else self.local_env()
            return self.expanded_command(env).replace("core_cmd ", "") + ' ' + get_clock_freq()

    def execute_local(self, wd):
        solnlen = len(self.cached_soln_ex.output) if self.cached_soln_ex else -1
        command_without_freq = self.expanded_command(self.local_env())
        command_with_freq = command_without_freq + ' ' + get_clock_freq()
        exec_result =  execute_command(wd, command_with_freq, timeout=self.timeout, logged=self.logged, solnlen=solnlen)
        return exec_result
        #return execute_command(wd, command_with_freq, timeout=self.timeout, logged=self.logged, solnlen=solnlen)

    def execute_solution(self, wd):
        # we execute the solution with working dir = submission, not sure if this is a good idea
        # previously always used self.filepath as cwd, but keeping that way forces custom tests to deal with different
        # context for relative paths...
        try:
            command_without_freq = self.expanded_command(self.soln_env())
            command_with_freq = command_without_freq + ' ' + get_clock_freq()
            soln_ex = execute_command(wd, command_with_freq , timeout=self.timeout, logged=self.logged, solnlen=None)
            soln_error = self.simple_fail(soln_ex)
            if soln_error: raise Exception(soln_error.summary_string())
        except Exception as e:
            # repackage as SolutionError in hopes to make it obvious to student this is our problem, not theirs
            cmd = self.command_for_display(for_soln=True)
            raise SolutionError("Unable to run %s\n[%s]" % (cmd, str(e)))
        return soln_ex
  
    def score(self, student_ex, soln_ex, context): 
        MAX_PERC_DIFF = 10 # okay if 10% different
        #print("soln_output:"+soln_ex.output)
        #print("student_output:"+student_ex.output)
        solution_ms = int(soln_ex.output.split('elapsed milliseconds:')[1])
        try:
            student_ms = int(student_ex.output.split('elapsed milliseconds:')[1])
        except:
            student_ms = -1 # problem parsing student solution

        perc_diff = percent_diff(student_ms,solution_ms)
        if perc_diff == None:
            # if 0 for solution, okay if student solution is < 200
            if student_ms < 200:
                return results.Correct()
        elif perc_diff < MAX_PERC_DIFF:
            return results.Correct()
       
        # incorrect
        print("Sample output:")
        print(soln_ex.output)
        print
        print("Your output:")
        print(student_ex.output)

        if perc_diff == None:
            print("Solution had 0ms, and student solution should be less than 200, but was %d\n" % student_ms)
        else:
            print("Percent difference (should be less than %d%%): %0.2f%%" % (MAX_PERC_DIFF,perc_diff))

        return results.Incorrect()

class TestTimer(VersusSolution):
    def score(self, student_ex, soln_ex, context): 
        MAX_PERC_DIFF = 10 # okay if 10% different
        solution_ms = int(soln_ex.output.split('elapsed milliseconds:')[1])
        try:
            student_ms = int(student_ex.output.split('elapsed milliseconds:')[1])
        except:
            student_ms = -1 # something wrong with solution
        perc_diff = percent_diff(student_ms,solution_ms)
    
        if perc_diff < MAX_PERC_DIFF:
            return results.Correct()
        else:
            print("Sample output:")
            print(soln_ex.output)
            print
            print("Your output:")
            print(student_ex.output)

            print
            print("Percent difference (should be less than %d%%): %0.2f%%" % (MAX_PERC_DIFF,perc_diff))
            return results.Incorrect()
