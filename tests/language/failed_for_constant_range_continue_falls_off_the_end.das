//! a function with a return type falls off its end when a continue
//! can skip the return in every pass of its constant-range for
expect 30310

options gen2

def f(x : int) : int {
    for (i in range(3)) {
        if (i != x) {
            continue
        }
        return i
    }
}

[export]
def main {
    print("{f(7)}\n")
}
