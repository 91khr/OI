#! python3
import random as rand
import subprocess as sp
import os
import sys

# {{{ Data generation and verification
def runprog(progname, input, limit = None):
    output, log = None, None
    with sp.Popen(progname, stdin = sp.PIPE, stdout = sp.PIPE, stderr = sp.PIPE,
            universal_newlines = True) as proc:
        output, log = proc.communicate(input, limit)
    return output, log
# }}}

# Data generator, return a dict of 'input' and 'output'
# return 'input' as a list to use SPJ, then it will be passed to SPJ
def gendata():
    import functools as ft

    n, l = rand.randint(5, 10), rand.randint(3, 12)
    nums = [ rand.randint(2, 7) for i in range(n) ]
    data = "{} {}\n".format(n, l) + '\n'.join((str(i) for i in nums))

    return {
        'input': data,
        'output': runprog('./std.out', data)[0]
    }

# SPJ, data is input('input' is used by python)
# std is output returned by gendata
# output is program output
# return a dict of 'info' as SPJ message, and 'result' as passed
def SPJ(data, std, output):
    pass

# {{{ Checking utils
class Check:
    def __init__(self, prog):
        self.prog = prog
        data = gendata()
        self.input = data['input']
        if type(self.input) == type([]):
            self.SPJ = True
        else:
            self.SPJ = False
        self.std = data['output']

    def run(self, limit = None):
        self.output, self.log = runprog(self.prog, self.input[0] if self.SPJ else self.input, limit)
        if self.SPJ:
            info = SPJ(self.input, self.std, self.output)
            self.SPJinfo = info['info']
            return info['result']
        else:
            return self.output == self.std

    def data(self):
        res = {
                "input": self.input,
                "output": self.output,
                "log": self.log,
                "std": self.std,
                "SPJ": self.SPJ,
                }
        if self.SPJ:
            res["SPJinfo"] = self.SPJinfo
        return res

def printData(data):
    print("input:\n", data['input'][0] if data['SPJ'] else data['input'])
    print("output:\n\"{}\"".format(data['output']))
    print("log:\n", data['log'])
    print("std:\n\"{}\"".format(data['std']))
    if data['SPJ']:
        print("Special Judge info:\n", data['SPJinfo'])

def checkUntilPass(prog = "./a.out", limit = None):
    now = 1
    try:
        while True:
            print("\rdata", now, end = "... ", flush = True, file = sys.stderr)
            now = now + 1
            handle = Check(prog)
            if not handle.run(limit):
                print("failed, data:")
                printData(handle.data())
                break
            else:
                print("passed", end='', file = sys.stderr)
    except KeyboardInterrupt:
        print("\nend", file = sys.stderr)

def checkByCount(count, prog = "./a.out", limit = None):
    for now in range(1, count):
        print("\rdata", now, end = "... ", flush = True, file = sys.stderr)
        handle = Check(prog)
        if not handle.run(limit):
            print("failed, data:")
            printData(handle.data())
            break
        else:
            print("passed", end='', file = sys.stderr)
# }}}

if __name__ == "__main__":
    # Modify this to use different check method
    checkUntilPass()
    #print(gendata())

# vim: fdm=marker
