options gen2
options fast_math = true

var g_a = 1.5lf
var g_b = 2.5lf
var g_c = 0.25lf

[never_fast_math, hint(noinline)]
def precise_mac(a, b, c : double) : double {
    return a * b + c
}

[never_inline, hint(noinline)]
def fast_mac(a, b, c : double) : double {
    return a * b + c
}

[never_inline, hint(noinline)]
def run_block(x : double; blk : block<(v : double) : double>) : double {
    return invoke(blk, x)
}

[never_fast_math, hint(noinline)]
def precise_block_mac(a, b, c : double) : double {
    return run_block(a) $ [never_inline] (v : double) : double {
        return v * b + c
    }
}

[export]
def main() {
    print("MACS {precise_mac(g_a, g_b, g_c)} {fast_mac(g_a, g_b, g_c)} {precise_block_mac(g_a, g_b, g_c)}\n")
}
