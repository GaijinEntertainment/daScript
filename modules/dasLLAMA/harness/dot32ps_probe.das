options gen2
// STRICT FP on purpose (no _jit_fast_math): the gate is bit-equality between the emitted
// dot32_ps and its identity expression acc + float4(dot32(0,w,x)) * s.

require llvm/daslib/x64_avx

var g_w : array<int8>
var g_x : array<int8>

def lcg(var st : uint&) : uint {
    st = st * 1664525u + 1013904223u
    return st
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
        for (bi in range(N / 32)) {
            let off = int64(bi) * 32l
            for (t in range(7)) {
                let s = float(t * 13 - 31) * 0.00714 + 0.001
                let acc = float4(float(t) * 0.5 - 1.0, 0.25, -3.75, float(bi))
                let got = dot32_ps(acc, wp + off, xp + off, s)
                let d = dot32(int4(0, 0, 0, 0), wp + off, xp + off)
                let want = acc + float4(d) * float4(s, s, s, s)
                if (got.x != want.x || got.y != want.y || got.z != want.z || got.w != want.w) {
                    bad++
                    if (bad < 4) {
                        print("MISMATCH bi={bi} t={t}: got={got} want={want}\n")
                    }
                }
            }
        }
    }
    if (bad == 0) {
        print("dot32_ps probe: ALL OK ({N / 32} blocks x 7 scales, extremes included)\n")
    } else {
        panic("dot32_ps probe FAILED: {bad} mismatches")
    }
}
