options gen2
expect 30341, 50503

require daslib/lpipe

//! a two-block call returned with one trailing block and its
//! lpipe() stranded after the return reports the unmatched call
//! and the stranded pipe, not a compiler crash
def two_blocks(a : block<() : int>; b : block<() : int>) : int {
    return invoke(a) + invoke(b)
}

def go() : int {
    return two_blocks() {
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
