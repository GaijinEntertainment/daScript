options gen2

require daslib/lpipe

//! a fixture for uncategorized_no_aot.das: the lpipe() stranded
//! after a return whose error text the test reads back
def one(a : block<() : int>) : int => invoke(a)

def go() : int {
    return one() {
        return 1
    }
    lpipe() {
        return 2
    }
}

[export]
def main() {
    print("{go()}\n")
}
