options gen2

let N = 64
var xs : double[N]
var ys : double[N]

//! four accesses to each of two globals in one function: the emitter resolves each global once per function
[export, sideeffects]
def four_sums() : double {
    var s0 = 0.0lf
    var s1 = 0.0lf
    var s2 = 0.0lf
    var s3 = 0.0lf
    var j = 0
    while (j < N) {
        s0 += xs[j] / ys[j]
        s1 += xs[j + 1] / ys[j + 1]
        s2 += xs[j + 2] / ys[j + 2]
        s3 += xs[j + 3] / ys[j + 3]
        j += 4
    }
    return (s0 + s1) + (s2 + s3)
}

var ga = 0
var gb = 0

//! one global per branch: each lookup sits in its own branch, none in the entry block
[export, sideeffects]
def branchy(foo : bool) {
    if (foo) {
        ga = 1
    } else {
        gb = 1
    }
}

var lut : int[256]
var scale = 3

//! two globals read on one arm of a branch inside a loop: each
//! lookup leaves the loop
[export, sideeffects]
def sum_scaled(n : int) : int {
    var s = 0
    for (i in range(n)) {
        if ((i & 3) != 0) {
            s += lut[i & 255] * scale
        }
    }
    return s
}

let shared shared_scale <- [3]

//! a shared global read on one arm of a branch inside a loop: its
//! lookup leaves the loop too
[export, sideeffects]
def sum_shared_scaled(n : int) : int {
    var s = 0
    for (i in range(n)) {
        if ((i & 3) != 0) {
            s += i * shared_scale[0]
        }
    }
    return s
}

[export]
def main {
    for (j in range(N)) {
        xs[j] = 1.0lf + double(j % 7)
        ys[j] = 3.0lf + double(j % 11)
    }
    print("FOUR_SUMS {four_sums()}\n")
    branchy(true)
    branchy(false)
    print("BRANCHY {ga} {gb}\n")
    for (i in range(256)) {
        lut[i] = i * 7
    }
    print("SUM_SCALED {sum_scaled(1024)}\n")
    print("SUM_SHARED_SCALED {sum_shared_scaled(1024)}\n")
}
