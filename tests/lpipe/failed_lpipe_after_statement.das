options gen2
expect 50503:2

require daslib/lpipe

//! an lpipe() after a statement that is no call at all - an if, a let
//! with no initializer - reports the shape it cannot pipe into, not a
//! compiler crash
def go() : int {
    var x : int
    lpipe() {
        return 2
    }
    if (x > 0) {
        x = 1
    }
    lpipe() {
        return 3
    }
    return x
}

[export]
def main() {
    print("{go()}\n")
}
