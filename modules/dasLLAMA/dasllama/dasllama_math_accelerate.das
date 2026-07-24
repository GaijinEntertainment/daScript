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

def private accel_fp_batch(var yp : float?; wp : float const?; xp : float const?; n, d, ntok : int64) : bool {
    unsafe {
        accel_sgemm_nt(int(ntok), int(d), int(n), xp, int(n), wp, int(n), yp, int(d))
    }
    return true
}

def private accel_bf16_batch(var yp : float?; wp : uint16 const?; xp : float const?; n, d, ntok : int64) : bool {
    // widen bf16 to grow-only f32 scratch once per call (ggml-blas-style), then one sgemm over all tokens
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
        accel_sgemm_nt(int(ntok), int(d), int(n), xp, int(n), addr<float const?>(g_wf32_scratch[0]), int(n), yp, int(d))
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

[init]
def private dasllama_math_accelerate_init() {
    if (get_env_variable("DASLLAMA_ACCEL") != "1") {
        return
    }
    var mntok = 32l
    let mbe : string = get_env_variable("DASLLAMA_ACCEL_MIN_NTOK")
    if (!empty(mbe)) {
        let parsed = try_to_int64(mbe)
        if (parsed._is_ok && parsed._value >= 1l) {
            mntok = parsed._value
        } else {
            to_log(LOG_WARNING, "dasLLAMA: DASLLAMA_ACCEL_MIN_NTOK `{mbe}` is not a positive integer — keeping {mntok}\n")
        }
    }
    dasllama_accelerate_enable(mntok)
}
