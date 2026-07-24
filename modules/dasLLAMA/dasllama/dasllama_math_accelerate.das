options gen2
options _comment_hygiene = true

module dasllama_math_accelerate shared public

require dasllama/dasllama_math
require dasllama/dasllama_par
require das_accelerate
require math
require daslib/fio
require daslib/math_bits
require daslib/strings_convert

// The "+AMX" tier: Accelerate BLAS (AMX on M1-M3, SME on M4+) serving the FLOAT-plane batch
// matmuls only. The AMX arc P0 ceilings put the quant crossover out of reach on M1 (dequant tax
// on top of NEON tiers already at ~800 GMAC/s aggregate), while the fp32/bf16 planes are where
// stock's AMX advantage genuinely lives (E4B per_layer_model_proj: NEON-bf16 1676ms/pass vs AMX
// 19.7ms at pp512). Opt-in per the three-tier product gating — raw CPU (default) / +AMX (this
// driver) / Metal:
//   DASLLAMA_ACCEL=1            arm the float-batch override (min ntok 32)
//   DASLLAMA_ACCEL_MIN_NTOK=N   override the gate (1 = the decode GEMV probe lane, M4/M5 sweep)
// Numerics: sgemm accumulates f32 in AMX order — a tolerance path vs the portable kernels (same
// envelope class as the metal donor), never bit-exact. Parity gates pin the raw-CPU tier.

var private g_wf32_scratch : array<float>

// Strip count for one GEMM: cover both AMX blocks (one per P-cluster) with headroom for load
// balance, without paying per-call sgemm setup on slivers.
def private accel_strips(d : int64) : int => int(clamp(d / 256l, 1l, 8l))

// Strip-dispatch a single-threaded sgemm over the jobque: each lane serves rows [rb,re) of W —
// its own sgemm call on its own core, so both AMX blocks engage with no foreign thread pool.
def private accel_sgemm_strips(var yp : float?; wp : float const?; xp : float const?; n, d, ntok : int64) {
    maybe_parallel_for(0, int(d), accel_strips(d)) $(rb, re) {
        unsafe {
            accel_sgemm_nt(int(ntok), re - rb, int(n), xp, int(n), wp + int64(rb) * n, int(n), yp + rb, int(d))
        }
    }
}

// Work floor in MMAC: per-call AMX engagement carries a fixed tax, so tiny GEMMs (MoE routers,
// ~130-180 MMAC — 26B/30B warm A/Bs net-lost engaged) fall through to NEON. Tune-crownable per box.
var private g_accel_min_mmac = 512l

//! Override the per-call work floor (MMAC) below which fp32 batch calls fall through to NEON.
//! 0 = engage always (tests / M4-M5 probing).
def dasllama_accelerate_set_min_mmac(v : int64) {
    g_accel_min_mmac = max(v, 0l)
}

def private accel_fp_batch(var yp : float?; wp : float const?; xp : float const?; n, d, ntok : int64) : bool {
    if (ntok * d * n < g_accel_min_mmac * 1000000l) {
        return false
    }
    accel_sgemm_strips(yp, wp, xp, n, d, ntok)
    return true
}

def private accel_bf16_batch(var yp : float?; wp : uint16 const?; xp : float const?; n, d, ntok : int64) : bool {
    // widen bf16 to grow-only f32 scratch once per call (ggml-blas-style), then strip-sgemm
    let total = d * n
    if (int64(length(g_wf32_scratch)) < total) {
        g_wf32_scratch |> resize(int(total))
    }
    unsafe {
        var sp = addr(g_wf32_scratch[0])
        maybe_parallel_for(0, int(d), int(min(d, 32l))) $(rb, re) {
            unsafe {
                for (i in range64(int64(rb) * n, int64(re) * n)) {
                    sp[i] = int_bits_to_float(int(uint(wp[i]) << 16u))
                }
            }
        }
        accel_sgemm_strips(yp, addr<float const?>(g_wf32_scratch[0]), xp, n, d, ntok)
    }
    return true
}

//! Arm the accelerate float-batch tier (DASLLAMA_ACCEL=1 arms it at init; tests/hosts call this
//! directly). min_ntok gates the donor to batch shapes — 1 extends it to decode GEMV.
def dasllama_accelerate_enable(min_ntok : int64) {
    set_float_batch_override(@@accel_fp_batch, @@accel_bf16_batch, min_ntok)
    to_log(LOG_INFO, "dasLLAMA: accelerate float-batch tier armed (min ntok {min_ntok})\n")
}

def dasllama_accelerate_disable() {
    clear_float_batch_override()
}

def private env_int64(name : string; def_v : int64; min_v : int64) : int64 {
    let raw : string = get_env_variable(name)
    if (empty(raw)) {
        return def_v
    }
    let parsed = try_to_int64(raw)
    if (parsed._is_ok && parsed._value >= min_v) {
        return parsed._value
    }
    to_log(LOG_WARNING, "dasLLAMA: {name} `{raw}` is not an integer >= {min_v} — keeping {def_v}\n")
    return def_v
}

[init]
def private dasllama_math_accelerate_init() {
    if (get_env_variable("DASLLAMA_ACCEL") != "1") {
        return
    }
    g_accel_min_mmac = env_int64("DASLLAMA_ACCEL_MIN_MMAC", g_accel_min_mmac, 0l)
    dasllama_accelerate_enable(env_int64("DASLLAMA_ACCEL_MIN_NTOK", 32l, 1l))
}
