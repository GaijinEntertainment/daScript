options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_rope shared public

require dasllama/dasllama_lint public
require math

// RoPE ANGLE GENERATION, platform-neutral: the theta schedule (freq[j] = 1/theta^(2j/hs)), the
// NTK rope_freqs divisor, freq scale and mscale, materialized in the layouts the engine consumes —
// two-array cos/sin tables (prefill/decode), the per-row batched table, and the packed cos|sin
// row the resident device kernels take. APPLICATION kernels stay with their backends
// (dasllama_math's rope_scaled_* leaves, the Metal/Vulkan fused rope-store kernels) — the
// specialization there is deliberate hot-loop shape, not duplication. Callers pass plain params;
// Model-facing wrappers live in dasllama_common.
//
// FLOAT ORDER IS CONTRACTUAL: the table builders compute `pos * fscale * freq`, the packed row
// computes `pos * freq * fscale` — each bit-identical to the loop it replaced. Do not "unify"
// the multiplication order; parity fixtures pin both paths.

// freq[j] of the theta schedule, with the optional NTK per-dim divisor (ffp = null for none)
def private rope_freq_j(j, head_size : int64; theta : float; ffp : float const?) : float {
    var freq = 1.0 / pow(theta, float(2l * j) / float(head_size))
    if (ffp != null) {
        freq /= unsafe(ffp[j])
    }
    return freq
}

//! Positions [start, start + nrows): row pi is at start + pi.
struct private RopeSeq {
    start : int64
}

//! Row pi is at positions[pi] (batch rows from different sequences at unrelated positions).
struct private RopeRows {
    pp : int64 const?
}

//! Interleaved M-RoPE (qwen3vl image spans): half-dim j reads the axis of positions4[pi] = (t, h =
//! grid row, w = column, e) that its sector selects.
struct private RopeImrope {
    pp : int4 const?
    sections : int4
}

//! Contiguous M-RoPE (the qwen2vl decoder): j < x reads t, then y reads h, then z reads w, then e,
//! the frequency ladder running continuously across the sections (never restarting).
struct private RopeMrope {
    pp : int4 const?
    sec_h : int64
    sec_w : int64
    sec_e : int64
}

//! The qwen3vl ViT's VISION grid: row pi's hs/2 slots are the h ladder (grid row) then the w ladder
//! RESTARTED, frequency divisor hs/2 - slot j reads θ^(−2·(j%(hs/4))/(hs/2)); positions2 = (h, w).
struct private RopeGrid {
    pp : int2 const?
    sect : int64
}

def private rope_axis(_src : RopeSeq; _j : int64) : int => 0

def private rope_axis(_src : RopeRows; _j : int64) : int => 0

def private rope_axis(src : RopeImrope; j : int64) : int {
    let s = src.sections
    let sector = int(j) % (s.x + s.y + s.z + s.w)
    if (sector % 3 == 1 && sector < 3 * s.y) {
        return 1
    } elif (sector % 3 == 2 && sector < 3 * s.z) {
        return 2
    } elif (sector % 3 == 0 && sector < 3 * s.x) {
        return 0
    }
    return 3
}

def private rope_axis(src : RopeMrope; j : int64) : int {
    if (j >= src.sec_e) {
        return 3
    } elif (j >= src.sec_w) {
        return 2
    } elif (j >= src.sec_h) {
        return 1
    }
    return 0
}

def private rope_axis(src : RopeGrid; j : int64) : int => j < src.sect ? 0 : 1

def private rope_pos(src : RopeSeq; pi : int64; _axis : int) : int64 => src.start + pi

def private rope_pos(src : RopeRows; pi : int64; _axis : int) : int64 => unsafe(src.pp[pi])

def private rope_pos(src : RopeImrope; pi : int64; axis : int) : int64 => int64(unsafe(src.pp[pi])[axis])

def private rope_pos(src : RopeMrope; pi : int64; axis : int) : int64 => int64(unsafe(src.pp[pi])[axis])

def private rope_pos(src : RopeGrid; pi : int64; axis : int) : int64 => int64(axis == 0 ? unsafe(src.pp[pi]).x : unsafe(src.pp[pi]).y)

def private rope_freq_of(_src : RopeSeq; j, head_size : int64; theta : float; ffp : float const?) : float => rope_freq_j(j, head_size, theta, ffp)

def private rope_freq_of(_src : RopeRows; j, head_size : int64; theta : float; ffp : float const?) : float => rope_freq_j(j, head_size, theta, ffp)

def private rope_freq_of(_src : RopeImrope; j, head_size : int64; theta : float; ffp : float const?) : float => rope_freq_j(j, head_size, theta, ffp)

def private rope_freq_of(_src : RopeMrope; j, head_size : int64; theta : float; ffp : float const?) : float => rope_freq_j(j, head_size, theta, ffp)

def private rope_freq_of(src : RopeGrid; j, head_size : int64; theta : float; _ffp : float const?) : float => rope_freq_j(j % src.sect, head_size / 2l, theta, null)

//! cos_tab[pi*half + j] = cos(pos(pi, axis(j)) * fscale * freq[j]) * mscale, sin likewise: freq[j]
//! and the axis are hoisted out of the row loop; mscale folds in here (it applies to NORM and NEOX alike).
def private rope_tabs_fill(@scratch var cos_tab : array<float>; @scratch var sin_tab : array<float>; src : auto(TS);
                           theta, fscale, mscale : float; ffp : float const?; nrows, head_size : int64) {
    let half = head_size / 2l
    cos_tab |> resize(nrows * half)
    sin_tab |> resize(nrows * half)
    unsafe {
        var cp = addr(cos_tab[0])
        var sp = addr(sin_tab[0])
        for (j in range64(half)) {
            let freq = rope_freq_of(src, j, head_size, theta, ffp)
            let axis = rope_axis(src, j)
            for (pi in range64(nrows)) {
                let angle = float(rope_pos(src, pi, axis)) * fscale * freq
                cp[pi * half + j] = cos(angle) * mscale
                sp[pi * half + j] = sin(angle) * mscale
            }
        }
    }
}

def private rope_ffp(rope_freqs : array<float>; use_ff : bool) : float const? => use_ff && !empty(rope_freqs) ? unsafe(addr<float const?>(rope_freqs[0])) : null

//! Positions [start_pos, start_pos+npos) as two cos/sin tables.
[arch(at = "../ARCHITECTURE_ENGINE_FORMATS.md#rope-one-fill")]
def build_rope_tabs(@scratch var cos_tab : array<float>; @scratch var sin_tab : array<float>;
                    theta, fscale, mscale : float; rope_freqs : array<float>; use_ff : bool;
                    start_pos, npos, head_size : int64) {
    rope_tabs_fill(cos_tab, sin_tab, RopeSeq(start = start_pos), theta, fscale, mscale, rope_ffp(rope_freqs, use_ff), npos, head_size)
}

//! build_rope_tabs' batched-decode twin: row pi's angle uses positions[pi]. Bit-identical per row
//! to the npos=1 table at that position.
def build_rope_tabs_rows(@scratch var cos_tab : array<float>; @scratch var sin_tab : array<float>;
                         theta, fscale, mscale : float; rope_freqs : array<float>; use_ff : bool;
                         positions : array<int64>; nrows, head_size : int64) {
    unsafe {
        rope_tabs_fill(cos_tab, sin_tab, RopeRows(pp = addr<int64 const?>(positions[0])), theta, fscale, mscale, rope_ffp(rope_freqs, use_ff), nrows, head_size)
    }
}

//! The interleaved-M-RoPE table (qwen3vl image spans); all-equal rows collapse bit-exactly.
def build_rope_tabs_imrope(@scratch var cos_tab : array<float>; @scratch var sin_tab : array<float>;
                           theta, fscale, mscale : float; rope_freqs : array<float>; use_ff : bool;
                           positions4 : array<int4>; sections : int4; nrows, head_size : int64) {
    unsafe {
        rope_tabs_fill(cos_tab, sin_tab, RopeImrope(pp = addr<int4 const?>(positions4[0]), sections = sections), theta, fscale, mscale, rope_ffp(rope_freqs, use_ff), nrows, head_size)
    }
}

//! The contiguous-section MROPE table (the qwen2vl decoder).
def build_rope_tabs_mrope(@scratch var cos_tab : array<float>; @scratch var sin_tab : array<float>;
                          theta, fscale, mscale : float; rope_freqs : array<float>; use_ff : bool;
                          positions4 : array<int4>; sections : int4; nrows, head_size : int64) {
    let src = RopeMrope(pp = unsafe(addr<int4 const?>(positions4[0])), sec_h = int64(sections.x),
        sec_w = int64(sections.x + sections.y), sec_e = int64(sections.x + sections.y + sections.z))
    rope_tabs_fill(cos_tab, sin_tab, src, theta, fscale, mscale, rope_ffp(rope_freqs, use_ff), nrows, head_size)
}

//! The mrope position map of one image-span prefill quantum (the image-grid walk):
//! head rows sequential from ``base``; span row i = (pos0, pos0+i/nx, pos0+i%nx, 0); tail rows
//! sequential from pos0 + max(nx, ny) — an image advances the position by its grid's long side.
def mrope_span_positions(@scratch var out : array<int4>; base, npos, span_lo, span_hi, nx, ny : int64) {
    if (span_hi - span_lo != nx * ny) {
        panic("mrope_span_positions: span [{span_lo},{span_hi}) is {span_hi - span_lo} rows but the grid is {nx}x{ny}")
    }
    out |> resize(npos)
    for (r in range64(span_lo)) {
        let p = int(base + r)
        out[r] = int4(p, p, p, p)
    }
    let pos0 = int(base + span_lo)
    for (i in range64(span_hi - span_lo)) {
        out[span_lo + i] = int4(pos0, pos0 + int(i / nx), pos0 + int(i % nx), 0)
    }
    let tail0 = pos0 + int(max(nx, ny))
    for (r in range64(span_hi, npos)) {
        let p = tail0 + int(r - span_hi)
        out[r] = int4(p, p, p, p)
    }
}

//! The qwen3vl ViT's VISION-mode rope table (RopeGrid); apply full-head NEOX (rope_scaled_neox_tab).
//! No frequency scale and no mscale: the fill's unit factors multiply exactly, so the row is the
//! bare cos(pos * freq) / sin(pos * freq).
def build_rope_tabs_vision(@scratch var cos_tab : array<float>; @scratch var sin_tab : array<float>;
                           theta : float; positions2 : array<int2>; nrows, head_size : int64) {
    unsafe {
        rope_tabs_fill(cos_tab, sin_tab, RopeGrid(pp = addr<int2 const?>(positions2[0]), sect = head_size / 4l), theta, 1.0, 1.0, null, nrows, head_size)
    }
}

//! One position's PACKED row — cos[half] then sin[half] in ONE array at `base` (the layout the
//! resident device kernels take; a different layout from the two-tab builders). Angle order is
//! `pos * freq * fscale` — this row's original loop — NOT the tabs' `pos * fscale * freq`.
def build_rope_row_packed(var dst : array<float>; base, pos, head_size : int64;
                          theta, fscale, mscale : float; rope_freqs : array<float>) {
    let half = head_size / 2l
    let has_ff = !empty(rope_freqs)
    unsafe {
        let ffp = has_ff ? addr<float const?>(rope_freqs[0]) : null
        for (j in range64(half)) {
            let freq = rope_freq_j(j, head_size, theta, ffp)
            let ang = float(pos) * freq * fscale
            dst[int(base + j)] = cos(ang) * mscale
            dst[int(base + half + j)] = sin(ang) * mscale
        }
    }
}
