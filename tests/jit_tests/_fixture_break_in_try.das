options gen2
options no_aot

//! a child for jit_artifacts.das: a loop whose try arm breaks out and
//! whose recover arm can panic - the JIT must leave the loop, cold
//! cache and warm alike
def f(n : int) : int {
    var s = 0
    for (i in range(n)) {
        try {
            if (i == 2) {
                break
            }
            s++
        } recover {
            panic("again")
        }
    }
    return s
}

[export]
def main {
    print("BREAK_IN_TRY {f(5)}\n")
}
