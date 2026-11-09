options gen2
expect 50503:2

require daslib/lpipe

//! an lpipe() after an assignment or a let whose right side is no
//! call reports the shape it cannot pipe into, not a compiler crash
def go() : int {
    var x = 0
    x = 5
    lpipe() {
        return 2
    }
    let y = 7
    lpipe() {
        return 3
    }
    return x + y
}

[export]
def main() {
    print("{go()}\n")
}
