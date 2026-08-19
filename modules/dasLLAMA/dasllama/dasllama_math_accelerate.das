options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_math_accelerate shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_math
require dasllama/dasllama_convert  // cvt_f32_to_f16 — the BNNS f16 batch prep
require dasllama/dasllama_env  // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require dasllama/dasllama_par
require das_accelerate
require llvm/daslib/f16_cvt
require math
require daslib/math_bits

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

var private @scratch g_wf32_scratch : array<float>   // grow-only bf16-widen staging, reused per call

// Strip count for one GEMM: cover both AMX blocks (one per P-cluster) with headroom for load
// balance, without paying per-call sgemm setup on slivers. The trace showed d=1280 shapes
// capped at 5 strips / 3 idle lanes — DASLLAMA_ACCEL_STRIPS overrides for the strips A/B.
var private g_accel_sgemm_strips = 0

def dasllama_accelerate_set_strips(v : int64) {
    g_accel_sgemm_strips = int(clamp(v, 0l, 64l))
}

def private accel_strips(d : int64) : int => int(g_accel_sgemm_strips > 0
    ? clamp(int64(g_accel_sgemm_strips), 1l, max(d, 1l))
    : clamp(d / 256l, 1l, 8l))

// Strip-dispatch a single-threaded sgemm over the jobque: each lane serves rows [rb,re) of W —
// its own sgemm call on its own core, so both AMX blocks engage with no foreign thread pool.
[hot_path]
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
var private @scratch g_hx_scratch : array<uint16>   // grow-only f16-activation staging, reused per call

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
[cold_path]   // first touch of a plane: convert + insert, once per (plane, content)
def private f16_plane_mint(key : uint64; wp : uint16 const?; total : int64; fpv : uint64) : uint16 const? {
    var pl = BnnsF16Plane(total = total, fp = fpv)
    pl.data |> resize(total)
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

[hot_path]
def private f16_plane_ptr(wp : uint16 const?; total : int64) : uint16 const? {
    // key folds the content fingerprint: changed content mints a NEW slot, never a move-over of a live one
    let fpv = plane_fp(wp, total)
    let key = unsafe(reinterpret<uint64>(wp)) ^ fpv
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
    return f16_plane_mint(key, wp, total, fpv)
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
[hot_path]
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

// f32 -> f16 converted copy of a weight plane, converted once and cached — the fp32-tower
// (ASR) twin of f16_plane_ptr. Unlike bf16->f16 this genuinely ROUNDS (23 -> 10 mantissa
// bits): the lane is the metal-f16-tile envelope class, opt-in behind DASLLAMA_ACCEL_F16.
var private g_f32hw_planes : table<uint64; BnnsF16Plane>

[cold_path]   // first touch of an fp32 plane: saturating convert + insert, once per (plane, content)
def private f16_plane_from_f32_mint(key : uint64; wp : float const?; total : int64; fpv : uint64) : uint16 const? {
    var pl = BnnsF16Plane(total = total, fp = fpv)
    pl.data |> resize(total)
    unsafe {
        var hp = addr(pl.data[0])
        maybe_parallel_for(0, int(total), 32) $(cb, ce) {
            unsafe {
                cvt_f32_to_f16_sat(hp + int64(cb), wp + int64(cb), int64(ce) - int64(cb))
            }
        }
    }
    to_log(LOG_INFO, "dasLLAMA: bnns f16 plane converted from f32 ({total} floats, {total * 2l / 1048576l} MB)\n")
    g_f32hw_planes[key] <- pl
    unsafe {
        return addr(g_f32hw_planes[key].data[0])
    }
}

[hot_path]
def private f16_plane_from_f32(wp : float const?; total : int64) : uint16 const? {
    // same never-overwrite key discipline as f16_plane_ptr above
    let fpv = plane_fp(unsafe(reinterpret<uint16 const?>(wp)), total * 2l)
    let key = unsafe(reinterpret<uint64>(wp)) ^ fpv
    var hit : uint16 const?
    g_f32hw_planes |> get(key) $(pl) {
        if (pl.total == total && pl.fp == fpv) {
            unsafe {
                hit = addr(pl.data[0])
            }
        }
    }
    if (hit != null) {
        return hit
    }
    return f16_plane_from_f32_mint(key, wp, total, fpv)
}

// one body for both f16 lanes: size the shared scratch, convert activations, strip-dispatch
[hot_path]
def private accel_f16_serve(var yp : float?; hwp : uint16 const?; xp : float const?; n, d, ntok : int64) : bool {
    let xcnt = ntok * n
    if (long_length(g_hx_scratch) < xcnt) {
        g_hx_scratch |> resize(xcnt)
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

[hot_path]
def private accel_f32_f16_lane(var yp : float?; wp : float const?; xp : float const?; n, d, ntok : int64) : bool => (
    accel_f16_serve(yp, f16_plane_from_f32(wp, d * n), xp, n, d, ntok))

[hot_path]
def private accel_bf16_f16_lane(var yp : float?; wp : uint16 const?; xp : float const?; n, d, ntok : int64) : bool => (
    accel_f16_serve(yp, f16_plane_ptr(wp, d * n), xp, n, d, ntok))

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

[hot_path]
def private accel_fp_batch(var yp : float?; wp : float const?; xp : float const?; n, d, ntok : int64) : bool {
    if (ntok * d * n < g_accel_min_mmac * 1000000l) {
        return false
    }
    if (g_accel_f16 && accel_f32_f16_lane(yp, wp, xp, n, d, ntok)) {
        return true
    }
    accel_sgemm_strips(yp, wp, xp, n, d, ntok)
    return true
}

[hot_path]
def private accel_bf16_batch(var yp : float?; wp : uint16 const?; xp : float const?; n, d, ntok : int64) : bool {
    if (g_accel_f16 && accel_bf16_f16_lane(yp, wp, xp, n, d, ntok)) {
        return true
    }
    // widen bf16 to grow-only f32 scratch once per call (ggml-blas-style), then strip-sgemm
    let total = d * n
    if (long_length(g_wf32_scratch) < total) {
        g_wf32_scratch |> resize(total)
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

// floor guard for the loaded knobs: a SET value below the floor is worth a word (garbage text
// already warned in the env_config loader and reads as unset).
def private floor_or(name : string; v : Option<int64>; def_v : int64; min_v : int64) : int64 {
    return def_v if (is_none(v))
    let got = unwrap(v)
    if (got < min_v) {
        to_log(LOG_WARNING, "dasLLAMA: {name} {got} is below the floor {min_v} - keeping {def_v}\n")
        return def_v
    }
    return got
}

[init]
def private dasllama_math_accelerate_init() {
    if (!g_env_accel.accel) {
        return
    }
    g_accel_min_mmac = floor_or("DASLLAMA_ACCEL_MIN_MMAC", g_env_accel.accel_min_mmac, g_accel_min_mmac, 0l)
    dasllama_accelerate_set_f16_strips(floor_or("DASLLAMA_ACCEL_F16_STRIPS", g_env_accel.accel_f16_strips, 0l, 0l))
    dasllama_accelerate_set_strips(floor_or("DASLLAMA_ACCEL_STRIPS", g_env_accel.accel_strips, 0l, 0l))
    if (g_env_accel.accel_f16 && !dasllama_accelerate_set_f16(true)) {
        to_log(LOG_WARNING, "dasLLAMA: DASLLAMA_ACCEL_F16=1 but the BNNS probe failed - f16 lane stays off\n")
    }
    dasllama_accelerate_enable(floor_or("DASLLAMA_ACCEL_MIN_NTOK", g_env_accel.accel_min_ntok, 32l, 1l))
}
