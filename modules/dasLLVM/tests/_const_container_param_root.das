options gen2

//! a const array parameter the callee iterates: `for` locks it, which
//! writes through the parameter, so it must not be readonly
[never_inline, hint(noinline, noalias = a)]
def sum_for(a : array<int>) : int {
    var n = 0
    for (x in a) {
        n += x
    }
    return n
}

//! the same through a const pointer to the array
[never_inline, hint(noinline, noalias = p)]
def sum_ptr(p : array<int> const?) : int {
    var n = 0
    for (x in *p) {
        n += x
    }
    return n
}

[export]
def main {
    let a <- [1, 2, 3]
    var total = 0
    for (_p in range(3)) {
        total += sum_for(a)
        total += sum_ptr(unsafe(addr(a)))
    }
    print("SUM {total}\n")
}
