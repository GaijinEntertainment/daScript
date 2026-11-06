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
    label 0:
    let x = twice(i)
    i ++
    if (i < 3) {
        goto label 0
    }
    print("{x}\n")
}
