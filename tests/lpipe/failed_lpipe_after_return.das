options gen2
expect 50503

require daslib/lpipe

//! an lpipe() stranded after a return that already took its
//! trailing block is a diagnostic, not a compiler crash: the
//! macro names the return and the assign-first spelling
def one_block(a : block<() : int>) : int {
    return invoke(a)
}

def go() : int {
    return one_block() {
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
