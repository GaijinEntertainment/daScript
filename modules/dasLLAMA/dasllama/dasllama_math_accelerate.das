options gen2
options _comment_hygiene = true

module dasllama_math_accelerate shared public

require dasllama/dasllama_math
require dasllama/dasllama_par
require das_accelerate
require llvm/daslib/f16_cvt
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
//   DASLLAMA_ACCEL_F16=1        bf16 planes via BNNS f16 matmul instead of widen+sgemm: the
//                               weight plane converts bf16->f16 ONCE (exact for every bf16
//                               value inside f16 normal range), activations convert f32->f16
//                               per call (clamped, the one rounding step), accumulate is f32
//                               (bnns_hgemm_probe-proven). M1: ~12ms vs ~16.6ms/pass at the
//                               E4B proj shape; the real target is M4/M5 SME f16 throughput.
// Numerics: sgemm accumulates f32 in AMX order — a tolerance path vs the portable kernels (same
// envelope class as the metal donor), never bit-exact. Parity gates pin the raw-CPU tier. The
// f16 lane adds one f32->f16 rounding on activations — the metal f16-tile envelope class.

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

// ===== BNNS f16 lane (bf16 planes) =====

struct private BnnsF16Plane {
    total : int64
    fp : uint64
    data : array<uint16>
}

var private g_accel_f16 = false
var private g_hw_planes : table<uint64; BnnsF16Plane>
var private g_hx_scratch : array<uint16>

// Sampled content fingerprint: the plane cache is keyed by pointer, and mmap addresses recycle
// across model loads — 16 spread samples + size catch address reuse with different weights.
def private plane_fp(wp : uint16 const?; total : int64) : uint64 {
    var h = 0xcbf29ce484222325ul
    for (i in range(16)) {
        unsafe {
            h = (h ^ uint64(wp[int64(i) * total / 16l])) * 0x100000001b3ul
        }
    }
    return h ^ uint64(total)
}

// bf16 -> f16 converted copy of the weight plane, converted once and cached. Exact for every
// bf16 value inside f16 normal range (7 mantissa bits into 10); out-of-range clamps to +-65504
// like the f16 KV codec, so inf can never enter the GEMM.
def private f16_plane_ptr(wp : uint16 const?; total : int64) : uint16 const? {
    let key = unsafe(reinterpret<uint64>(wp))
    let fpv = plane_fp(wp, total)
    var hit : uint16 const?
    g_hw_planes |> get(key) $(pl) {
        if (pl.total == total && pl.fp == fpv) {
            unsafe {
                hit = addr(pl.data[0])
            }
        }
    }
    if (hit != null) {
        return hit
    }
    var pl = BnnsF16Plane(total = total, fp = fpv)
    pl.data |> resize(int(total))
    unsafe {
        var hp = addr(pl.data[0])
        maybe_parallel_for(0, int(total), 32) $(cb, ce) {
            unsafe {
                for (i in range64(int64(cb), int64(ce))) {
                    hp[i] = uint16(f32_to_f16(clamp(int_bits_to_float(int(uint(wp[i]) << 16u)), -65504.0, 65504.0)))
                }
            }
        }
    }
    to_log(LOG_INFO, "dasLLAMA: bnns f16 plane converted ({total} halfwords, {total * 2l / 1048576l} MB)\n")
    g_hw_planes[key] <- pl
    unsafe {
        return addr(g_hw_planes[key].data[0])
    }
}

// f16-lane strip count: 0 = the shared accel_strips default. The BNNS f16 kernel saturates the
// AMX blocks at a different feeder count than sgemm (C rig: 6 strips ~12ms vs 8 ~15ms at the
// E4B proj shape) — tune-crownable per box via DASLLAMA_ACCEL_F16_STRIPS.
var private g_accel_f16_strips = 0

def dasllama_accelerate_set_f16_strips(v : int64) {
    g_accel_f16_strips = int(clamp(v, 0l, 64l))
}

// Strip-dispatch the single-threaded BNNS f16 matmul over the jobque — same discipline as
// accel_sgemm_strips. false = a lane's filter refused (caller falls back; y is fully rewritten
// by the portable path, so partial writes are harmless).
def private accel_bnns_strips(var yp : float?; hwp : uint16 const?; hxp : uint16 const?; n, d, ntok : int64) : bool {
    // flag written through a captured POINTER — lane jobs clone captures, a scalar write is lost
    var bad = 0
    unsafe {
        var badp = addr(bad)
        maybe_parallel_for(0, int(d), g_accel_f16_strips > 0 ? g_accel_f16_strips : accel_strips(d)) $(rb, re) {
            unsafe {
                if (accel_bnns_hgemm_nt(int(ntok), re - rb, int(n), hxp, int(n), hwp + int64(rb) * n, int(n), yp + rb, int(d)) != 0) {
                    badp[0] = 1
                }
            }
        }
    }
    return bad == 0
}

def private accel_bf16_f16_lane(var yp : float?; wp : uint16 const?; xp : float const?; n, d, ntok : int64) : bool {
    let hwp = f16_plane_ptr(wp, d * n)
    let xcnt = ntok * n
    if (int64(length(g_hx_scratch)) < xcnt) {
        g_hx_scratch |> resize(int(xcnt))
    }
    unsafe {
        var hxp = addr(g_hx_scratch[0])
        maybe_parallel_for(0, int(ntok), int(min(ntok, 8l))) $(tb, te) {
            unsafe {
                cvt_f32_to_f16(hxp + int64(tb) * n, xp + int64(tb) * n, (int64(te) - int64(tb)) * n)
            }
        }
        return accel_bnns_strips(yp, hwp, hxp, n, d, ntok)
    }
}

// 1x1x1 smoke: BNNS present on this OS and computing — gates arming the f16 lane.
def private accel_bnns_probe() : bool {
    var a = uint16(f32_to_f16(1.0))
    var b = uint16(f32_to_f16(2.0))
    var c = 0.0
    unsafe {
        if (accel_bnns_hgemm_nt(1, 1, 1, addr(a), 1, addr(b), 1, addr(c), 1) != 0) {
            return false
        }
    }
    return abs(c - 2.0) < 1e-3
}

//! Arm the BNNS f16 lane for bf16 planes (DASLLAMA_ACCEL_F16=1 arms it at init). Returns
//! whether it armed — the probe fails where BNNS is unavailable (pre-macOS-11) and the
//! sgemm widen path keeps serving bf16.
def dasllama_accelerate_set_f16(on : bool) : bool {
    g_accel_f16 = on && accel_bnns_probe()
    return g_accel_f16
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
    if (g_accel_f16 && accel_bf16_f16_lane(yp, wp, xp, n, d, ntok)) {
        return true
    }
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
    dasllama_accelerate_set_f16_strips(env_int64("DASLLAMA_ACCEL_F16_STRIPS", 0l, 0l))
    if (get_env_variable("DASLLAMA_ACCEL_F16") == "1" && !dasllama_accelerate_set_f16(true)) {
        to_log(LOG_WARNING, "dasLLAMA: DASLLAMA_ACCEL_F16=1 but the BNNS probe failed — f16 lane stays off\n")
    }
    dasllama_accelerate_enable(env_int64("DASLLAMA_ACCEL_MIN_NTOK", 32l, 1l))
}
