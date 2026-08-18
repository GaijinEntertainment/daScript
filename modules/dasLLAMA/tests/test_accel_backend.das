options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_math
require daslib/jobque_boost
require daslib/math_bits
require math
require ?das_accelerate dasllama/dasllama_math_accelerate

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

// static_if shims: every driver symbol reference lives behind builtin_module_exists so the
// FILE compiles off-Apple (the [test] arm feints there, but helper bodies still typecheck)
def private t_accel_enable() {
    static_if (typeinfo builtin_module_exists(das_accelerate)) {
        dasllama_accelerate_enable(32l)
    }
}

def private t_accel_disable() {
    static_if (typeinfo builtin_module_exists(das_accelerate)) {
        dasllama_accelerate_disable()
    }
}

def private t_accel_pin_floor() {
    static_if (typeinfo builtin_module_exists(das_accelerate)) {
        dasllama_accelerate_set_min_mmac(0l)
    }
}

[unused_argument(on)]  // off-Apple the else arm never reads it
def private t_accel_set_f16(on : bool) : bool {
    static_if (typeinfo builtin_module_exists(das_accelerate)) {
        return dasllama_accelerate_set_f16(on)
    } else {
        return false
    }
}

// run the same mm twice — portable (override off) into yref, accelerate-armed into yacc
def private ref_and_accel(ntok : int; run_mm : block<(var y : array<float>) : void>) : tuple<yref : array<float>; yacc : array<float>> {
    var res : tuple<yref : array<float>; yacc : array<float>>
    res.yref |> resize(ntok * D)
    res.yacc |> resize(ntok * D)
    clear_float_batch_override()
    invoke(run_mm, res.yref)
    t_accel_enable()
    invoke(run_mm, res.yacc)
    t_accel_disable()
    return <- res
}

def private check_fp32(t : T?; ntok : int; expect_exact : bool; f16_lane : bool = false) {
    var w : array<float>
    w |> resize(D * N)
    fill01(w, 3u)
    var x : array<float>
    x |> resize(ntok * N)
    fill01(x, 7u)
    if (f16_lane && !t_accel_set_f16(true)) {
        to_log(LOG_WARNING, "BNNS probe failed on this OS - f16 lane arm feints\n")
        t |> success(true)
        return
    }
    let r <- ref_and_accel(ntok) $(var y) {
        matmul_batch(y, w, 0l, x, int64(N), int64(D), int64(ntok))
    }
    if (f16_lane) {
        t_accel_set_f16(false)
    }
    if (expect_exact) {
        for (va, vb in r.yref, r.yacc) {
            t |> equal(va, vb)
        }
    } else {
        t |> success(max_rel_err(r.yref, r.yacc) < (f16_lane ? 1e-2 : 1e-4))
    }
}

// f16_lane routes the accelerate half through the BNNS f16 lane (weights bf16->f16 exact,
// activations f32->f16 rounded, f32 accumulate) — one x rounding step, so a looser envelope.
def private check_bf16(t : T?; ntok : int; f16_lane : bool) {
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
    if (f16_lane && !t_accel_set_f16(true)) {
        to_log(LOG_WARNING, "BNNS probe failed on this OS - f16 lane arm feints\n")
        t |> success(true)
        return
    }
    let r <- ref_and_accel(ntok) $(var y) {
        matmul_bf16_batch(y, wbf, 0l, x, int64(N), int64(D), int64(ntok))
    }
    t_accel_set_f16(false)
    t |> success(max_rel_err(r.yref, r.yacc) < (f16_lane ? 1e-2 : 1e-4))
}

[test, unused_argument(t)]  // off-Apple the feint arm never reads t
def test_accel_float_batch(t : T?) {
    static_if (typeinfo builtin_module_exists(das_accelerate)) {
        with_job_que() {
            setup_dasllama_jobque_()
            t_accel_pin_floor()  // tiny test shapes must engage, not fall through
            t_accel_set_f16(false)  // pin: env DASLLAMA_ACCEL_F16 must not leak into the sgemm arms
            t |> run("fp32 batch vs portable (tolerance)") @(t : T?) {
                check_fp32(t, NTOK, false)
            }
            t |> run("bf16 batch vs portable (tolerance)") @(t : T?) {
                check_bf16(t, NTOK, false)
            }
            t |> run("bf16 BNNS f16 lane vs portable (tolerance)") @(t : T?) {
                check_bf16(t, NTOK, true)
            }
            t |> run("below-gate ntok falls through bit-exact") @(t : T?) {
                check_fp32(t, SMALL_NTOK, true)
            }
            t |> run("fp32 BNNS f16 lane vs portable (tolerance)") @(t : T?) {
                check_fp32(t, NTOK, false, true)
            }
            t |> run("strips override serves correctly, incl. rows < strips") @(t : T?) {
                dasllama_accelerate_set_strips(8l)
                check_fp32(t, NTOK, false)
                var w : array<float>
                w |> resize(4 * N)
                fill01(w, 5u)
                var x : array<float>
                x |> resize(NTOK * N)
                fill01(x, 9u)
                var yref : array<float>
                yref |> resize(NTOK * 4)
                var yacc : array<float>
                yacc |> resize(NTOK * 4)
                clear_float_batch_override()
                matmul_batch(yref, w, 0l, x, int64(N), 4l, int64(NTOK))
                t_accel_enable()
                matmul_batch(yacc, w, 0l, x, int64(N), 4l, int64(NTOK))
                t_accel_disable()
                dasllama_accelerate_set_strips(0l)
                t |> success(max_rel_err(yref, yacc) < 1e-4)
            }
        }
    } else {
        feint("das_accelerate is not built on this platform; nothing to compare\n")
    }
}
