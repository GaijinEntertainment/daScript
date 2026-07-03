options gen2
// STRICT FP on purpose (no _jit_fast_math): the gate is bit-equality between the emitted
// dot32_acc8 and its per-lane identity acc[j] += float(dot of byte quad j) * s — single calls
// (extremes included) AND a kernel-shaped block loop (the LICM-promoted accumulator path).

require llvm/daslib/x64_avx

var g_w : array<int8>
var g_x : array<int8>

def lcg(var st : uint&) : uint {
    st = st * 1664525u + 1013904223u
    return st
}

def quad_ref(wp, xp : int8 const?; off : int64; j : int) : int {
    var q = 0
    unsafe {
        for (e in range(4)) {
            q += int(wp[off + int64(j * 4 + e)]) * int(xp[off + int64(j * 4 + e)])
        }
    }
    return q
}

[export]
def main {
    print("jit: {jit_enabled()}\n")
    let N = 320
    g_w |> resize(N)
    g_x |> resize(N)
    var st = 12345u
    for (i in range(N)) {
        g_w[i] = int8(int(lcg(st) % 256u) - 128)          // w: any int8 incl -128
        g_x[i] = int8(int(lcg(st) % 255u) - 127)          // x: [-127, 127] (the quantizer contract)
    }
    // force the extremes into the first block
    g_w[0] = int8(-128)
    g_w[1] = int8(127)
    g_x[0] = int8(-127)
    g_x[1] = int8(127)
    var bad = 0
    unsafe {
        let wp = reinterpret<int8 const?>(addr(g_w[0]))
        let xp = reinterpret<int8 const?>(addr(g_x[0]))
        // phase 1: single calls, bit-equal per lane
        for (bi in range(N / 32)) {
            let off = int64(bi) * 32l
            for (t in range(7)) {
                let s = float(t * 13 - 31) * 0.00714 + 0.001
                var got : float[8]
                var want : float[8]
                for (j in range(8)) {
                    got[j] = float(j) * 0.5 - 1.75 + float(bi) * 0.25
                    want[j] = got[j]
                }
                dot32_acc8(addr(got[0]), wp + off, xp + off, s)
                for (j in range(8)) {
                    want[j] += float(quad_ref(wp, xp, off, j)) * s
                }
                for (j in range(8)) {
                    if (got[j] != want[j]) {
                        bad++
                        if (bad < 9) {
                            print("MISMATCH bi={bi} t={t} j={j}: got={got[j]} want={want[j]}\n")
                        }
                    }
                }
            }
        }
        // phase 2: kernel-shaped block loop into one accumulator (the LICM promotion path)
        var acc : float[8]
        var ref : float[8]
        var ap = addr(acc[0])
        for (bi in range64(int64(N / 32))) {
            let s = float(bi) * 0.033 + 0.005
            dot32_acc8(ap, wp + bi * 32l, xp + bi * 32l, s)
        }
        for (bi in range(N / 32)) {
            let s = float(bi) * 0.033 + 0.005
            for (j in range(8)) {
                ref[j] += float(quad_ref(wp, xp, int64(bi) * 32l, j)) * s
            }
        }
        for (j in range(8)) {
            if (acc[j] != ref[j]) {
                bad++
                print("LOOP MISMATCH j={j}: got={acc[j]} want={ref[j]}\n")
            }
        }
    }
    if (bad == 0) {
        print("dot32_acc8 probe: ALL OK ({N / 32} blocks x 7 scales + block loop, extremes included)\n")
    } else {
        panic("dot32_acc8 probe FAILED: {bad} mismatches")
    }
}
