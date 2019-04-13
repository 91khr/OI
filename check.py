#! python3
import random as rand
import subprocess as sp
import os

# Data generator, return a dict of 'input' and 'output'
# return 'input' as a list to use SPJ, then it will be passed to SPJ
def gendata():
    import functools as ft

    n, m = rand.randint(0, 10), rand.randint(3, 10)
    arr = [ rand.randint(-10, 10) for i in range(n) ]
    origin = arr

    def genlen():
        beg = rand.randint(0, len(arr) - 1)
        return [ beg + 1, rand.randint(beg, len(arr) - 1) - beg + 1 ]
    cmdlist = [
            ('INSERT', lambda: [rand.randint(0, len(arr)),
                *(lambda a: (a, *[ rand.randint(-10, 10) for i in range(a) ]))(rand.randint(1, 5))]),
            ('DELETE', lambda: genlen()),
            ('MAKE-SAME', lambda: [*genlen(), rand.randint(-10, 10)]),
            ('REVERSE', lambda: genlen()),
            ('GET-SUM', lambda: genlen()),
            ('MAX-SUM', lambda: []),
            ]
    opers = []
    output = []
    print('origin:', arr)
    for i in range(m):
        if len(arr) > 0:
            op =  (lambda i: (i, cmdlist[i][1]()))(rand.randint(0, len(cmdlist) - 1))
        else:
            op = (0, cmdlist[0][1]())
        opers.append(op)
        if op[0] == 0:
            pos, val = op[1][0], op[1][2:]
            arr = arr[:pos] + val + arr[pos:]
            print('insert ', pos, val, arr)
        elif op[0] == 1:
            pos, fwd = op[1]
            arr = arr[:pos - 1] + arr[pos + fwd - 1:]
            print('delete', pos, fwd, arr)
        elif op[0] == 2:
            pos, fwd, val = op[1]
            arr = arr[:pos - 1] + [ val for i in range(fwd) ] + arr[pos + fwd - 1:]
            print('fill', pos, fwd, val, arr)
        elif op[0] == 3:
            pos, fwd = op[1]
            mid = arr[pos - 1 : pos + fwd - 1]
            arr = arr[:pos - 1] + [ mid[-i - 1] for i in range(len(mid)) ] + arr[pos + fwd - 1:]
            print('reverse', pos, fwd, arr)
        elif op[0] == 4:
            pos, fwd = op[1]
            output.append(ft.reduce(lambda a, b: a + b, arr[pos - 1: pos + fwd - 1], 0))
            print('query', pos, fwd, output[-1])
        elif op[0] == 5:
            ans = arr[0]
            prev = 0
            for now in arr:
                prev = max([prev + now, now])
                ans = max([prev, ans])
            print('max', ans)
            output.append(ans)

    return {
            'input': "{} {}\n".format(n, m) + ' '.join([ str(i) for i in origin ]) + '\n'
                + '\n'.join([ cmdlist[it[0]][0] + (' ' if len(it[1]) else '') + ' '.join([ str(i) for i in it[1] ]) for it in opers ])
                + '\n',
            'output': ''.join([ str(i) + '\n' for i in output ]),
            }

# SPJ, data is input('input' is used by python)
# std is output returned by gendata
# output is program output
# return a dict of 'info' and 'result'
def SPJ(data, std, output):
    pass

# {{{ Functional utils
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
        with sp.Popen(self.prog, stdin = sp.PIPE, stdout = sp.PIPE, stderr = sp.PIPE,
                universal_newlines = True) as proc:
            self.output, self.log = proc.communicate(self.input, limit)
            if self.SPJ:
                info = SPJ(self.input, self.std, self.output)
                self.SPJinfo = info.info
                return info.result
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
            res += { "SPJinfo": self.SPJinfo }
        return res

def printData(data):
    print("input:\n", data['input'])
    print("output:\n\"{}\"".format(data['output']))
    print("log:\n", data['log'])
    print("std:\n\"{}\"".format(data['std']))
    if data['SPJ']:
        print("Special Judge info:\n", data['SPJinfo'])

def checkUntilPass(prog = "./a.out", limit = None):
    now = 1
    while True:
        print("data", now, end = "... ", flush = True)
        now = now + 1
        handle = Check(prog)
        if not handle.run(limit):
            print("failed, data:")
            printData(handle.data())
            break
        else:
            print("passed")

def checkByCount(count, prog = "./a.out", limit = None):
    for now in range(1, count):
        print("data", now, end = "... ", flush = True)
        handle = Check(prog)
        if not handle.run(limit):
            print("failed, data:")
            printData(handle.data())
            break
        else:
            print("passed")
# }}}

if __name__ == "__main__":
    # Modify this to use different check method
    #checkUntilPass()
    print(gendata())

# vim: fdm=marker
