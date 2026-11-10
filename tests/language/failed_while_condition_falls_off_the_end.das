//! a function with a return type falls off its end when its only
//! return sits in a while whose condition can be false
expect 30310

options gen2

def f(x : int) : int {
    while (x > 0) {
        return 1
    }
}

[export]
def main {
    print("{f(0)}\n")
}
