options gen2
options solid_context
options no_aot

//! a child for the solid array base test: a global array and a
//! shared global array a loop indexes, and scalar globals none does

var private solid_limit : int = 1000
var private solid_seen : int
var private solid_flags : uint8[1001]

def solid_squares() : int[64] {
    var t : int[64]
    for (i in range(64)) {
        t[i] = i * i
    }
    return t
}

let shared private solid_shared_bias = solid_squares()
let shared private solid_shared = solid_squares()

[hint(noinline)]
def solid_sieve() : int {
    let n = solid_limit
    var count = 0
    for (i in range(n + 1)) {
        solid_flags[i] = uint8(1)
    }
    var i = 2
    while (i <= n) {
        if (solid_flags[i] != 0u8) {
            var k = i + i
            while (k <= n) {
                solid_flags[k] = uint8(0)
                k += i
            }
            count++
        }
        i++
    }
    solid_seen += count
    return count
}

[hint(noinline)]
def solid_shared_sum(n : int) : int {
    var sum = 0
    for (i in range(n)) {
        sum += solid_shared[i] + solid_shared_bias[i]
    }
    return sum
}

[export]
def main {
    print("SOLID_SIEVE {solid_sieve()} {solid_seen} {solid_shared_sum(64)}\n")
}
