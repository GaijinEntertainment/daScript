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
        unsafe {
            freq /= ffp[j]
        }
    }
    return freq
}

//! Positions [start_pos, start_pos+npos) as two cos/sin tables: cos_tab[pi*half + j] =
//! cos((start_pos+pi) * fscale * freq[j]) * mscale, sin likewise. freq[j] is hoisted out of the
//! position loop; mscale folds in here (ggml applies it to NORM and NEOX alike).
def build_rope_tabs(@scratch var cos_tab : array<float>; @scratch var sin_tab : array<float>;
                    theta, fscale, mscale : float; rope_freqs : array<float>; use_ff : bool;
                    start_pos, npos, head_size : int64) {
    let half = head_size / 2l
    let has_ff = use_ff && !empty(rope_freqs)
    cos_tab |> resize(npos * half)
    sin_tab |> resize(npos * half)
    unsafe {
        var cp = addr(cos_tab[0])
        var sp = addr(sin_tab[0])
        let ffp = has_ff ? addr<float const?>(rope_freqs[0]) : null
        for (j in range64(half)) {
            let freq = rope_freq_j(j, head_size, theta, ffp)
            for (pi in range64(npos)) {
                let angle = float(start_pos + pi) * fscale * freq
                cp[pi * half + j] = cos(angle) * mscale
                sp[pi * half + j] = sin(angle) * mscale
            }
        }
    }
}

//! build_rope_tabs' batched-decode twin: row pi's angle uses positions[pi] (batch rows come from
//! different sequences at unrelated positions). Bit-identical per row to the npos=1 table at
//! that position.
def build_rope_tabs_rows(@scratch var cos_tab : array<float>; @scratch var sin_tab : array<float>;
                         theta, fscale, mscale : float; rope_freqs : array<float>; use_ff : bool;
                         positions : array<int64>; nrows, head_size : int64) {
    let half = head_size / 2l
    let has_ff = use_ff && !empty(rope_freqs)
    cos_tab |> resize(nrows * half)
    sin_tab |> resize(nrows * half)
    unsafe {
        var cp = addr(cos_tab[0])
        var sp = addr(sin_tab[0])
        let pp = addr<int64 const?>(positions[0])
        let ffp = has_ff ? addr<float const?>(rope_freqs[0]) : null
        for (j in range64(half)) {
            let freq = rope_freq_j(j, head_size, theta, ffp)
            for (pi in range64(nrows)) {
                let angle = float(pp[pi]) * fscale * freq
                cp[pi * half + j] = cos(angle) * mscale
                sp[pi * half + j] = sin(angle) * mscale
            }
        }
    }
}

//! build_rope_tabs_rows' interleaved-M-RoPE twin (qwen3vl image spans), the ggml
//! ggml_mrope_cache_init sector walk verbatim: half-dim j reads the axis of positions4[pi] =
//! (t, h=grid row, w=column, e) its sector selects; all-equal rows collapse bit-exactly.
def build_rope_tabs_imrope(@scratch var cos_tab : array<float>; @scratch var sin_tab : array<float>;
                           theta, fscale, mscale : float; rope_freqs : array<float>; use_ff : bool;
                           positions4 : array<int4>; sections : int4; nrows, head_size : int64) {
    let half = head_size / 2l
    let has_ff = use_ff && !empty(rope_freqs)
    let sect_dims = sections.x + sections.y + sections.z + sections.w
    cos_tab |> resize(nrows * half)
    sin_tab |> resize(nrows * half)
    unsafe {
        var cp = addr(cos_tab[0])
        var sp = addr(sin_tab[0])
        let pp = addr<int4 const?>(positions4[0])
        let ffp = has_ff ? addr<float const?>(rope_freqs[0]) : null
        for (j in range64(half)) {
            let freq = rope_freq_j(j, head_size, theta, ffp)
            let sector = int(j) % sect_dims
            var axis = 3
            if (sector % 3 == 1 && sector < 3 * sections.y) {
                axis = 1
            } elif (sector % 3 == 2 && sector < 3 * sections.z) {
                axis = 2
            } elif (sector % 3 == 0 && sector < 3 * sections.x) {
                axis = 0
            }
            for (pi in range64(nrows)) {
                let angle = float(pp[pi][axis]) * fscale * freq
                cp[pi * half + j] = cos(angle) * mscale
                sp[pi * half + j] = sin(angle) * mscale
            }
        }
    }
}

//! The mrope position map of one image-span prefill quantum (mtmd's decoder walk verbatim):
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

//! The qwen3vl ViT's VISION-mode rope table (ggml rope VISION): row pi's hs/2 slots = the h
//! ladder (grid row) then the w ladder RESTARTED, freq divisor hs/2 — slot j reads
//! θ^(−2·(j%(hs/4))/(hs/2)). Apply full-head NEOX (rope_scaled_neox_tab); positions2 = (h, w).
def build_rope_tabs_vision(@scratch var cos_tab : array<float>; @scratch var sin_tab : array<float>;
                           theta : float; positions2 : array<int2>; nrows, head_size : int64) {
    let half = head_size / 2l
    let sect = half / 2l
    cos_tab |> resize(nrows * half)
    sin_tab |> resize(nrows * half)
    unsafe {
        var cp = addr(cos_tab[0])
        var sp = addr(sin_tab[0])
        let pp = addr<int2 const?>(positions2[0])
        for (j in range64(half)) {
            let freq = rope_freq_j(j % sect, half, theta, null)
            let is_h = j < sect
            for (pi in range64(nrows)) {
                let angle = float(is_h ? pp[pi].x : pp[pi].y) * freq
                cp[pi * half + j] = cos(angle)
                sp[pi * half + j] = sin(angle)
            }
        }
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
