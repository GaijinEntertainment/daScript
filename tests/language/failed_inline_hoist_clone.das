options gen2
expect 30100:1

[never_inline]
def make_const() : array<int> const {
    var a <- [1, 2, 3]
    return <- a
}

[never_inline]
def count(a : array<int>; b : int) : int {
    return length(a) + b
}

[inline]
def made() : array<int> const {
    var a <- [4, 5]
    if (length(a) > 100) {
        return <- a
    }
    return <- a
}

[export]
def main() {
    let c = true
    print("{count(c ? made() : make_const(), 1)}\n")
}
