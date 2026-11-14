//! a const table returned by value cannot be moved into a generator
//! variable, so a yielding generator loop over it does not compile
expect 30941

options gen2

def make_const_squares(n : int) : table<int; int> const {
    return <- {for (i in range(n)); i => i * i}
}

[export]
def main {
    var total = 0
    var gen <- generator<int> {
        for (s in values(make_const_squares(6))) {
            yield s
        }
        return false
    }
    for (v in gen) {
        total += v
    }
    print("{total}\n")
}
