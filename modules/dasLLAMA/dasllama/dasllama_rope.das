options gen2
options _comment_hygiene = true

module dasllama_rope shared public

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
    cos_tab |> resize(npos * half)   // nolint:PERF026 — session scratch; same size every step, so this allocates once
    sin_tab |> resize(npos * half)   // nolint:PERF026 — see above
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
    cos_tab |> resize(nrows * half)   // nolint:PERF026 — session scratch; same size every step, so this allocates once
    sin_tab |> resize(nrows * half)   // nolint:PERF026 — see above
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
