options gen2
expect 30100:1

[never_inline]
def make_const() : array<int> const {
    var a <- [1, 2, 3]
    return <- a
}

[inline]
def len_of(a : array<int>) : int {
    var n = length(a)
    if (n > 100) {
        return 0
    }
    n += length(a)
    return n
}

[export]
def main() {
    let c = true
    print("{len_of(c ? make_const() : make_const())}\n")
}
