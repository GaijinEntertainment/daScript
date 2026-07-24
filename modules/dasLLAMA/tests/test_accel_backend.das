options gen2

require dastest/testing_boost public
require dasllama/dasllama_math
require daslib/jobque_boost  // nolint:STYLE030 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out
require daslib/math_bits  // nolint:STYLE030 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out
require math  // nolint:STYLE030 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out
require ?das_accelerate dasllama/dasllama_math_accelerate  // nolint:STYLE030 — the "+AMX" float-batch override under test; Apple-only C++ module

// The "+AMX" float-batch override (Accelerate sgemm — AMX on M1-M3, SME on M4+) vs the portable
// fp32/bf16 batch matmuls on deterministic data. sgemm accumulates f32 in coprocessor order —
// relative-tolerance compare, never bit-exact. Below the min-ntok gate the override must fall
// through to the portable body and match it exactly. Off-Apple every check feints cleanly.

let N = 96          // K: weight row length
let D = 48          // output rows
let NTOK = 40       // above the armed min-ntok gate (32)
let SMALL_NTOK = 4  // below the gate — must fall through to the portable body

def private fill01(var a : array<float>; salt : uint) {
    for (i in range(length(a))) {
        a[i] = float(int((uint(i) * 2654435761u + salt * 40503u) & 0xFFFFu)) / 65536.0 - 0.5
    }
}

def private max_rel_err(a, b : array<float>) : float {
    var m = 0.0
    for (va, vb in a, b) {
        m = max(m, abs(va - vb) / max(1.0, abs(va)))
    }
    return m
}

// run the same mm twice — portable (override off) into yref, accelerate-armed into yacc
def private ref_and_accel(ntok : int; run_mm : block<(var y : array<float>) : void>) : tuple<yref : array<float>; yacc : array<float>> {
    var res : tuple<yref : array<float>; yacc : array<float>>
    res.yref |> resize(ntok * D)
    res.yacc |> resize(ntok * D)
    clear_float_batch_override()
    invoke(run_mm, res.yref)
    dasllama_accelerate_enable(32l)
    invoke(run_mm, res.yacc)
    dasllama_accelerate_disable()
    return <- res
}

def private check_fp32(t : T?; ntok : int; expect_exact : bool) {
    var w : array<float>
    w |> resize(D * N)
    fill01(w, 3u)
    var x : array<float>
    x |> resize(ntok * N)
    fill01(x, 7u)
    let r <- ref_and_accel(ntok) $(var y) {
        matmul_batch(y, w, 0l, x, int64(N), int64(D), int64(ntok))
    }
    if (expect_exact) {
        for (va, vb in r.yref, r.yacc) {
            t |> equal(va, vb)
        }
    } else {
        t |> success(max_rel_err(r.yref, r.yacc) < 1e-4)
    }
}

def private check_bf16(t : T?; ntok : int) {
    var wbf : array<uint16>
    wbf |> resize(D * N)
    {
        var w : array<float>
        w |> resize(D * N)
        fill01(w, 3u)
        for (dst, src in wbf, w) {
            dst = uint16(uint(float_bits_to_int(src)) >> 16u)
        }
    }
    var x : array<float>
    x |> resize(ntok * N)
    fill01(x, 7u)
    let r <- ref_and_accel(ntok) $(var y) {
        matmul_bf16_batch(y, wbf, 0l, x, int64(N), int64(D), int64(ntok))
    }
    t |> success(max_rel_err(r.yref, r.yacc) < 1e-4)
}

[test]
def test_accel_float_batch(t : T?) {
    static_if (typeinfo builtin_module_exists(das_accelerate)) {
        with_job_que() {
            setup_dasllama_jobque_()
            dasllama_accelerate_set_min_mmac(0l)  // tiny test shapes must engage, not fall through
            t |> run("fp32 batch vs portable (tolerance)") @(t : T?) {
                check_fp32(t, NTOK, false)
            }
            t |> run("bf16 batch vs portable (tolerance)") @(t : T?) {
                check_bf16(t, NTOK)
            }
            t |> run("below-gate ntok falls through bit-exact") @(t : T?) {
                check_fp32(t, SMALL_NTOK, true)
            }
        }
    } else {
        feint("das_accelerate is not built on this platform; nothing to compare\n")
    }
}
