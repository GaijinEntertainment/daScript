//! an [inline] call with an early return inside a ternary right after
//! a goto target label cannot splice, so the demanded inline does not
//! compile
options gen2
expect 30100:1

[inline]
def twice(a : int) : int {
    var r = a
    if (a > 100) {
        return 0
    }
    r += a
    return r
}

[export]
def main() {
    var i = 0
    var s = 0
    label 0:
    let x = i > 1 ? 7 : twice(i)
    s += x
    i ++
    if (i < 3) {
        goto label 0
    }
    print("{s}\n")
}
