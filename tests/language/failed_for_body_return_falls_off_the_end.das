//! a function with a return type falls off its end when its only
//! return sits in a for body and the source is empty
expect 30310

options gen2

def f(a : array<int>) : int {
    for (x in a) {
        return x
    }
}

[export]
def main {
    var a : array<int>
    print("{f(a)}\n")
}
