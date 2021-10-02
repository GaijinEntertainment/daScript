require daslib/algorithm

options log

[export]
def main
    var a : int[10]
    for A,t in a,range(100500)
        A = t
    print("a = {a}\n")
    let lb = lower_bound(a, 5)
    print("lb = {lb}\n")

options debugger
require daslib/debug
