options gen2

// The library the C host in host.c drives. One export per boundary shape the thunks have to
// marshal, plus a panicking one so the host can prove the guard.

struct Pair {
    lo : int
    hi : int
}

var g_calls = 0
var g_torn_down = 0

// The C host proves this ran: a jitted SimFunction carries no shutdown flag, so <P>_destroy has to
// call it explicitly or a host that creates and destroys in a loop leaks whatever it releases.
[finalize]
def teardown() {
    g_torn_down = 1
    print("fixture [finalize] ran\n")
}

[export_c]
def bump() : int {
    g_calls++
    return g_calls
}

[export_c]
def widen(p : Pair) : Pair {
    return Pair(lo = p.lo - 1, hi = p.hi + 1)
}

// Returned by value, and 8-aligned with a hole after `a`: the thunk has to build it in a slot of
// its own alignment, not in a field of the argument frame.
struct Wide {
    a : int8
    b : int64
    c : float
}

[export_c]
def widen_wide(n : int) : Wide {
    return Wide(a = int8(n), b = int64(n) * 100l, c = float(n) * 0.5)
}

[export_c]
def scale3(v : float3; k : float) : float3 {
    return v * k
}

[export_c]
def greet(who : string) : string {
    return "hola {who}"
}

[export_c]
def flip(b : bool) : bool {
    return !b
}

[export_c]
def bigsum(a : int64; b : uint64) : uint64 {
    return uint64(a) + b
}

[export_c(name = "double_or_raise")]
def boom(n : int) : int {
    if (n < 0) {
        panic("negative: {n}")
    }
    return n * 2
}
