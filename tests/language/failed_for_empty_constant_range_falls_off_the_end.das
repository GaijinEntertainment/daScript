//! a function with a return type falls off its end when its only
//! return sits in a for over an empty constant range
expect 30310

options gen2

def f() : int {
    for (i in range(0)) {
        return i
    }
}

[export]
def main {
    print("{f()}\n")
}
