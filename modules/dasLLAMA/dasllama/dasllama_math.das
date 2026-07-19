options gen2
options _comment_hygiene = true

module dasllama_math shared public

require math
require strings  // to_int: DASLLAMA_CALLER_PRIO env override in setup_dasllama_jobque_
require daslib/fio  // has_env_variable: DAS_JOBQUE_TEAM_RANK_GATE suppresses the profile's team_rank_gate knob
require daslib/jobque_boost public  // matmul's threaded path expands parallel_for; consumers need its symbols
require daslib/jobque_profile  // named trace categories + unit markers (set_trace_tags registers them)
require dasllama/dasllama_par  // maybe_parallel_for: thread-or-inline a kernel body on a runtime size flag
require dasllama/dasllama_tune  // [tuned]: reconstitute dot from dot_template with per-box loop hints
require dasllama/dasllama_gemm_schema  // kq_qsb/kq_ssb — the fmt-id kq plane strides
require llvm/daslib/f16_cvt  // f16<->f32 converts: JIT half intrinsics on aarch64 / x64+F16C, exact bit-twiddle elsewhere

// dasLLAMA numeric primitives — naive fp32 baseline (curriculum step 1).
// Correctness first; optimization (blocking, float4, threading) comes later.
//
// Every array param is `array<float> | #` (owning array OR borrowed view), so weights
// can be passed zero-copy as views into one big model blob and outputs can be written
// straight into a view (e.g. the KV cache). Dims/indices are int64 for large models.

//! Dot product of two n-length float buffers, auto-vectorized by the LLVM JIT
//! (width-8, no bounds checks, a/b asserted non-aliasing). Beats hand-rolled float4 ~1.8x.
//! `dot_template` is the single source of truth (hot loop marked `[tune=1]`); the emitted
//! `dot` below reconstitutes it via `[tuned]` with the per-box permutation — default `vec8_u2`
//! is the original `[vectorize, vectorize_width=8, unroll_count=2]`, so this is bit-identical.
[hint(unsafe_range_check, noalias = a, noalias = b)]
def template dot_template(a, b : float const ?; n : int64) : float {
    var acc = 0.0
    for [tune = 1] (j in range64(n)) {
        acc = mad(unsafe(a[j]), unsafe(b[j]), acc)   // fused multiply-add (vectorizes to fmla)
    }
    return acc
}

[hint(unsafe_range_check, noalias = a, noalias = b), tuned]
def dot(a, b : float const?; n : int64) : float {}

// The elementwise kernels below share dot's tuning rig: each `<name>_template` holds the `[tune=1]`
// hot loop and `[tuned]` reconstitutes `<name>` with the per-box permutation from the app tune
// sidecar — default `vec8_u2` is bit-identical until the profiler picks a different perm.

//! AXPY: d[j] += scale * s[j] over n elements, auto-vectorized. Used single-threaded AND as the
//! V-accumulate in threaded prefill attention — `d`/`s` never overlap, so noalias holds inside
//! fork workers too (a captured `var d : float?` stays a writable pointer — capture freezes the binding, not the pointee).
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template axpy_template(var d : float ?; s : float const ?; scale : float; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] = mad(scale, s[j], d[j])
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def axpy(var d : float?; s : float const?; scale : float; n : int64) {}

// The f16 KV-codec kernels: the cache side is IEEE binary16 held in uint16, converted per element
// inside the hot loop (ggml's lesson: convert the O(head_size) side in-loop, keep the bandwidth
// win — f16_to_f32 lowers to vcvtph2ps/fcvtl inside the vectorized loop under the JIT).

//! Dot of an f32 query row against an f16 (binary16-in-uint16) cache row.
[hint(unsafe_range_check, noalias = a, noalias = b)]
def template dot_f16_template(a : float const ?; b : uint16 const ?; n : int64) : float {
    var acc = 0.0
    for [tune = 1] (j in range64(n)) {
        acc = mad(unsafe(a[j]), f16_to_f32(uint(unsafe(b[j]))), acc)
    }
    return acc
}

[hint(unsafe_range_check, noalias = a, noalias = b), tuned]
def dot_f16(a : float const?; b : uint16 const?; n : int64) : float {}

//! AXPY from an f16 cache row: d[j] += scale * f16(s[j]) — the V-accumulate under the f16 codec.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template axpy_f16_template(var d : float ?; s : uint16 const ?; scale : float; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] = mad(scale, f16_to_f32(uint(s[j])), d[j])
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def axpy_f16(var d : float?; s : uint16 const?; scale : float; n : int64) {}

//! Dot of an f32 row against a bfloat16 (bf16-in-uint16) row. The widen is an exact high-half
//! bit shift, so dots are bit-identical to pre-expanding the weight to fp32 at half the bandwidth
//! (llama.cpp's AVX2 ggml_vec_dot_bf16 also truncates the ACTIVATION to bf16; ours stays f32).
[hint(unsafe_range_check, noalias = a, noalias = b)]
def template dot_bf16_template(a : float const ?; b : uint16 const ?; n : int64) : float {
    var acc = 0.0
    for [tune = 1] (j in range64(n)) {
        acc = mad(unsafe(a[j]), unsafe(reinterpret<float>(uint(unsafe(b[j])) << 16u)), acc)
    }
    return acc
}

[hint(unsafe_range_check, noalias = a, noalias = b), tuned]
def dot_bf16(a : float const?; b : uint16 const?; n : int64) : float {}

//! f32 -> f16 row convert (the codec store). Clamps to ±65504 so inf can NEVER enter the cache —
//! O(kv_dim) per token, free next to the reads.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template cvt_f32_to_f16_template(var d : uint16 ?; s : float const ?; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] = uint16(f32_to_f16(clamp(s[j], -65504.0, 65504.0)))
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def cvt_f32_to_f16(var d : uint16?; s : float const?; n : int64) {}

//! f16 -> f32 row convert (bulk dequant into f32 scratch).
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template cvt_f16_to_f32_template(var d : float ?; s : uint16 const ?; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] = f16_to_f32(uint(s[j]))
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def cvt_f16_to_f32(var d : float?; s : uint16 const?; n : int64) {}

// The q8_0 KV-codec kernels: cache side is ggml block_q8_0 — 32 int8 quants sharing one f16 scale,
// INTERLEAVED {f16 d; int8 qs[32]} = 34 bytes/block (llama.cpp -ctk/-ctv layout, so oracle
// cross-checks stay honest). Rows are whole blocks (kv_dim % 32 == 0); n always a multiple of 32.

//! Dot of an f32 query row against a q8_0 cache row: the block scale factors out —
//! d * Σ a[j]·q[j] per 32-block (the prefill read; decode uses the int8×int8 twin below).
[hint(unsafe_range_check, noalias = a, noalias = b)]
def template dot_q8kv_template(a : float const ?; b : uint8 const ?; n : int64) : float {
    var acc = 0.0
    unsafe {
        let b16 = reinterpret<uint16 const?>(b)
        let bq = reinterpret<int8 const?>(b)
        for (bi in range64(n / 32l)) {
            let base = bi * 32l
            let qb = bi * 34l + 2l
            var facc = 0.0
            for [tune = 1] (k in range64(32l)) {
                facc = mad(a[base + k], float(bq[qb + k]), facc)
            }
            acc = mad(facc, f16_to_f32(uint(b16[bi * 17l])), acc)
        }
    }
    return acc
}
[hint(unsafe_range_check, noalias = a, noalias = b), tuned]
def dot_q8kv(a : float const?; b : uint8 const?; n : int64) : float {}

//! THE q8_0 decode hot path: a Q8-quantized query (int8 quants + f32 scales per 32) against a
//! q8_0 cache row. Inner loop is pure int8·int8 multiply-accumulate — the same SDOT/VNNI shape
//! as dot_q8q8 — with both block scales multiplying in once per 32-block.
[hint(unsafe_range_check, noalias = q, noalias = qs, noalias = b)]
def template dot_q8q8kv_template(q : int8 const ?; qs : float const ?; b : uint8 const ?; n : int64) : float {
    var acc = 0.0
    unsafe {
        let b16 = reinterpret<uint16 const?>(b)
        let bq = reinterpret<int8 const?>(b)
        for (bi in range64(n / 32l)) {
            let base = bi * 32l
            let qb = bi * 34l + 2l
            var iacc = 0
            for [tune = 1] (k in range64(32l)) {
                iacc += int(q[base + k]) * int(bq[qb + k])
            }
            acc += float(iacc) * qs[bi] * f16_to_f32(uint(b16[bi * 17l]))
        }
    }
    return acc
}
[hint(unsafe_range_check, noalias = q, noalias = qs, noalias = b), tuned(fallback = "vec16")]
def dot_q8q8kv(q : int8 const?; qs : float const?; b : uint8 const?; n : int64) : float {}

//! AXPY from a q8_0 cache row: d[j] += scale * (q[j] * d_block) — the V-accumulate under the
//! q8_0 codec; scale * d_block hoists per 32-block.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template axpy_q8kv_template(var d : float ?; s : uint8 const ?; scale : float; n : int64) : void {
    unsafe {
        let s16 = reinterpret<uint16 const?>(s)
        let sq = reinterpret<int8 const?>(s)
        for (bi in range64(n / 32l)) {
            let base = bi * 32l
            let qb = bi * 34l + 2l
            let c = scale * f16_to_f32(uint(s16[bi * 17l]))
            for [tune = 1] (k in range64(32l)) {
                d[base + k] = mad(c, float(sq[qb + k]), d[base + k])
            }
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def axpy_q8kv(var d : float?; s : uint8 const?; scale : float; n : int64) {}

//! q8_0 -> f32 row convert (the flash pack's bulk dequant): d[j] = q[j] * d_block.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template cvt_q8kv_to_f32_template(var d : float ?; s : uint8 const ?; n : int64) : void {
    unsafe {
        let s16 = reinterpret<uint16 const?>(s)
        let sq = reinterpret<int8 const?>(s)
        for (bi in range64(n / 32l)) {
            let base = bi * 32l
            let qb = bi * 34l + 2l
            let db = f16_to_f32(uint(s16[bi * 17l]))
            for [tune = 1] (k in range64(32l)) {
                d[base + k] = float(sq[qb + k]) * db
            }
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def cvt_q8kv_to_f32(var d : float?; s : uint8 const?; n : int64) {}

//! Quantize n f32s (whole 32-blocks) into q8_0 blocks at dst — the codec store, ggml's exact
//! quantize_row_q8_0 semantics: per block d = amax/127 as f16, quants round with UN-rounded f32
//! 1/d — idempotent (re-quantizing a dequantized row is byte-identical, max element hits ±127).
[hint(unsafe_range_check, noalias = dst, noalias = src)]
def template quantize_q8kv_row_template(var dst : uint8 ?; src : float const ?; n : int64) : void {
    unsafe {
        let p4 = reinterpret<float4 const?>(src)
        var d16 = reinterpret<uint16?>(dst)
        var dq = reinterpret<int8?>(dst)
        for (bi in range64(n / 32l)) {
            let base = bi * 32l
            let f4 = base / 4l
            var m0 = abs(p4[f4])
            var m1 = abs(p4[f4 + 1l])
            for (k in range64(1l, 4l)) {
                m0 = max(m0, abs(p4[f4 + 2l * k]))
                m1 = max(m1, abs(p4[f4 + 2l * k + 1l]))
            }
            let m = max(m0, m1)
            let amax = max(max(m.x, m.y), max(m.z, m.w))
            let d = amax / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            d16[bi * 17l] = uint16(f32_to_f16(d))
            let qb = bi * 34l + 2l
            for [tune = 1] (i in range64(32l)) {
                dq[qb + i] = int8(round(src[base + i] * id))
            }
        }
    }
}
[hint(unsafe_range_check, noalias = dst, noalias = src), tuned(fallback = "plain")]
def quantize_q8kv_row(var dst : uint8?; src : float const?; n : int64) {}

// The tq4 KV-codec kernels: TurboQuant-style rotated 4-bit cache — a seeded-sign FWHT basis
// change at the store/query seams (dasllama_common) over a q4_0-shaped block: 32 4-bit quants
// sharing one f16 scale, {f16 d; uint8 qs[16]} = 18B/block. Rows whole blocks; head_size a power of two for the FWHT.

//! In-place per-head rotation: v *= signs, then the orthonormal FWHT (n a power of two).
//! ⟨Hx, Hy⟩ = ⟨x, y⟩, so scores/AV run entirely in the rotated basis.
def fwht_signs_row(var v : float?; signs : float const?; n : int64) {
    unsafe {
        for (i in range64(n)) {
            v[i] *= signs[i]
        }
        fwht_core(v, n)
    }
}

//! Inverse of fwht_signs_row: FWHT first (self-inverse), then the signs.
def fwht_unsign_row(var v : float?; signs : float const?; n : int64) {
    unsafe {
        fwht_core(v, n)
        for (i in range64(n)) {
            v[i] *= signs[i]
        }
    }
}

def private fwht_core(var v : float?; n : int64) {
    unsafe {
        var h = 1l
        while (h < n) {
            var i = 0l
            while (i < n) {
                for (j in range64(i, i + h)) {
                    let a = v[j]
                    let b = v[j + h]
                    v[j] = a + b
                    v[j + h] = a - b
                }
                i += h * 2l
            }
            h *= 2l
        }
        let inv = 1.0 / sqrt(float(n))
        for (i in range64(n)) {
            v[i] *= inv
        }
    }
}

//! Fill the tq4 rotation sign vector (±1), deterministic — sessions and pools sharing cache
//! bytes MUST agree, so the seed is a fixed constant and layers with a smaller head_size use
//! a prefix of the same vector.
def tq4_fill_signs(var dst : array<float>; n : int64) {
    dst |> resize(int(n))
    var s = 0xC0FFEEu
    for (x in dst) {
        s = s * 1664525u + 1013904223u
        x = (s & 0x10000u) != 0u ? -1.0 : 1.0
    }
}

//! THE tq4 decode hot path: a Q8-quantized (rotated) query against a tq4 cache row — int8×int4
//! multiply-accumulate per 32-block, both block scales in once. The nibble unpack is two MACs
//! per cache byte.
[hint(unsafe_range_check, noalias = q, noalias = qs, noalias = b)]
def template dot_q8tq4kv_template(q : int8 const ?; qs : float const ?; b : uint8 const ?; n : int64) : float {
    var acc = 0.0
    unsafe {
        let b16 = reinterpret<uint16 const?>(b)
        for (bi in range64(n / 32l)) {
            let base = bi * 32l
            let qb = bi * 18l + 2l
            var iacc = 0
            for [tune = 1] (k in range64(16l)) {
                let by = int(b[qb + k])
                iacc += int(q[base + 2l * k]) * ((by & 15) - 8)
                iacc += int(q[base + 2l * k + 1l]) * ((by >> 4) - 8)
            }
            acc += float(iacc) * qs[bi] * f16_to_f32(uint(b16[bi * 9l]))
        }
    }
    return acc
}
[hint(unsafe_range_check, noalias = q, noalias = qs, noalias = b), tuned(fallback = "vec16")]
def dot_q8tq4kv(q : int8 const?; qs : float const?; b : uint8 const?; n : int64) : float {}

//! AXPY from a tq4 cache row: d[j] += scale * ((nib - 8) * d_block) — the V-accumulate under
//! the tq4 codec (in the rotated basis; the driver un-rotates the head's output once).
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template axpy_tq4kv_template(var d : float ?; s : uint8 const ?; scale : float; n : int64) : void {
    unsafe {
        let s16 = reinterpret<uint16 const?>(s)
        for (bi in range64(n / 32l)) {
            let base = bi * 32l
            let qb = bi * 18l + 2l
            let c = scale * f16_to_f32(uint(s16[bi * 9l]))
            for [tune = 1] (k in range64(16l)) {
                let by = int(s[qb + k])
                d[base + 2l * k] = mad(c, float((by & 15) - 8), d[base + 2l * k])
                d[base + 2l * k + 1l] = mad(c, float((by >> 4) - 8), d[base + 2l * k + 1l])
            }
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def axpy_tq4kv(var d : float?; s : uint8 const?; scale : float; n : int64) {}

//! tq4 -> f32 row convert (the flash pack's bulk dequant): d[j] = (nib - 8) * d_block.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template cvt_tq4kv_to_f32_template(var d : float ?; s : uint8 const ?; n : int64) : void {
    unsafe {
        let s16 = reinterpret<uint16 const?>(s)
        for (bi in range64(n / 32l)) {
            let base = bi * 32l
            let qb = bi * 18l + 2l
            let db = f16_to_f32(uint(s16[bi * 9l]))
            for [tune = 1] (k in range64(16l)) {
                let by = int(s[qb + k])
                d[base + 2l * k] = float((by & 15) - 8) * db
                d[base + 2l * k + 1l] = float((by >> 4) - 8) * db
            }
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def cvt_tq4kv_to_f32(var d : float?; s : uint8 const?; n : int64) {}

//! Quantize n (rotated) f32s into tq4 blocks at dst — the codec store. ggml quantize_row_q4_0
//! semantics: d = signed-max / -8 stored as f16, quants truncate x*id + 8.5 (never negative:
//! |x*id| <= 8 by construction), capped at 15.
[hint(unsafe_range_check, noalias = dst, noalias = src)]
def template quantize_tq4kv_row_template(var dst : uint8 ?; src : float const ?; n : int64) : void {
    unsafe {
        var d16 = reinterpret<uint16?>(dst)
        for (bi in range64(n / 32l)) {
            let base = bi * 32l
            var amax = 0.0
            var mval = 0.0
            for (i in range64(32l)) {
                let v = src[base + i]
                if (abs(v) > amax) {
                    amax = abs(v)
                    mval = v
                }
            }
            let d = mval / -8.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            d16[bi * 9l] = uint16(f32_to_f16(d))
            let qb = bi * 18l + 2l
            for [tune = 1] (k in range64(16l)) {
                let q0 = min(15, int(src[base + 2l * k] * id + 8.5))
                let q1 = min(15, int(src[base + 2l * k + 1l] * id + 8.5))
                dst[qb + k] = uint8(q0 | (q1 << 4))
            }
        }
    }
}
[hint(unsafe_range_check, noalias = dst, noalias = src), tuned(fallback = "plain")]
def quantize_tq4kv_row(var dst : uint8?; src : float const?; n : int64) {}

//! d[j] += s[j], width-8 vectorized (the residual-stream update). The plain `for k; a[k]+=b[k]`
//! over bounds-checked arrays does NOT vectorize under the JIT — this pointer form is ~5x faster.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template add_inplace_template(var d : float ?; s : float const ?; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] += s[j]
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def add_inplace(var d : float?; s : float const?; n : int64) {}

//! d[j] = (d[j] + s[j]) * scale in ONE pass — the residual add fused with the per-layer output
//! scale (gemma4 layer_out_scale, the PLE 1/sqrt(2) fold). Bit-identical to add_inplace followed
//! by scale_inplace (same two fp ops per element, same order) at one RMW walk instead of two.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template add_scale_inplace_template(var d : float ?; s : float const ?; scale : float; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] = (d[j] + s[j]) * scale
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def add_scale_inplace(var d : float?; s : float const?; scale : float; n : int64) {}

//! d[j] *= s[j], width-8 vectorized — the SwiGLU/GeGLU gate multiply (act(W1·x) * (W3·x)).
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template mul_inplace_template(var d : float ?; s : float const ?; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] *= s[j]
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def mul_inplace(var d : float?; s : float const?; n : int64) {}

//! d[j] *= scale (broadcast scalar), width-8 vectorized — ggml_vec_scale_f32. Flash attention
//! multiplies the whole Q×KV score tile by 1/sqrt(head_size) in one sweep, replacing the old scalar
//! per-element multiply buried in the mask branch.
[hint(unsafe_range_check, noalias = d)]
def template scale_inplace_template(var d : float ?; scale : float; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] *= scale
        }
    }
}
[hint(unsafe_range_check, noalias = d), tuned]
def scale_inplace(var d : float?; scale : float; n : int64) {}

//! d[j] = s[j], width-8 vectorized contiguous copy (the KV-cache store). Same reason as add_inplace:
//! a bounds-checked array copy does NOT vectorize under the JIT; this pointer form does. d (cache) and
//! s (the k_b/v_b scratch) are always distinct arrays, so noalias is correct and free.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def template copy_floats_template(var d : float ?; s : float const ?; n : int64) : void {
    for [tune = 1] (j in range64(n)) {
        unsafe {
            d[j] = s[j]
        }
    }
}
[hint(unsafe_range_check, noalias = d, noalias = s), tuned]
def copy_floats(var d : float?; s : float const?; n : int64) {}

// Spin-before-park window for the jobque workers (see setup_dasllama_jobque). Runtime knob: the
// useful window tracks the box's serial-gap lengths, not the kernel. 30ms covers the between-call
// gaps of a decode/prefill loop, keeping workers hot through a whole token instead of parking.
var g_jobque_spin_us = 30000l
def public set_jobque_spin_us(v : int64) { g_jobque_spin_us = max(v, 0l) }
def public get_jobque_spin_us() : int64 { return g_jobque_spin_us }

// Join-poll level for fork/join joins (see setup_dasllama_jobque) — ggml's --poll denomination:
// level*1024*128 relax-rounds on the joining thread before it parks (0 = park immediately).
// 50 is ggml's shipped default; the poll burns only the calling thread and is bounded (~100ms).
var g_jobque_join_poll = 50l
def public set_jobque_join_poll(v : int64) { g_jobque_join_poll = max(v, 0l) }
def public get_jobque_join_poll() : int64 { return g_jobque_join_poll }

// Worker-pool limit (JobQue::setWorkerLimit — workers above the limit sleep dormant): the
// tiny-model dial. An inline-dominated decode token runs FASTER with fewer awake workers (zen2
// 135M: T=8 176 t/s vs T=48 149 — T-1 pollers interfere with main-thread weight streaming). 0 = no limit.
var g_dispatch_worker_limit = 0
def public set_dispatch_worker_limit(v : int) { g_dispatch_worker_limit = max(v, 0) }
def public get_dispatch_worker_limit() : int { return g_dispatch_worker_limit }

// Per-op team rank gate (JobQue::setTeamRankGate — each team op admits only as many workers as
// its widest stage published chunks). Server-core boxes want it ON (135M T=48: +10% zen2/+48% SPR,
// >=1B neutral); heterogeneous P/E (M1) keep it off. Tri-state: -1 = que default, 0/1 = force.
var g_team_rank_gate = -1
def public set_team_rank_gate(v : int) { g_team_rank_gate = clamp(v, -1, 1) }
def public get_team_rank_gate() : int { return g_team_rank_gate }

// Trace op tags (jobque_trace_tag): stamped before dispatches so the lane-timeline viewer colors
// by OP rather than stage-count. Gated — set_trace_tags(true) rides with jobque_trace_start in
// the profiling harness; production dispatches never pay the extern call.
var g_trace_tags = false
def public set_trace_tags(on : bool) {
    g_trace_tags = on
    if (on && is_job_que_available()) {   // registries live on the que; stay que-independent like trace_tag
        register_trace_categories()
    }
}
let public TRACE_TAG_ATTN_CHAIN = 1
let public TRACE_TAG_ATTN_STD = 2
let public TRACE_TAG_FFN_CHAIN = 3
let public TRACE_TAG_FFN_STD = 4
let public TRACE_TAG_CLS = 5
let public TRACE_TAG_MOE = 6
let public TRACE_TAG_SHEXP = 7
let public TRACE_TAG_PLE = 8
let public TRACE_TAG_ROUTE = 9   // MoE router GEMM + select/bucket (prefill section split)
let public TRACE_TAG_PARK = 10   // MoE park/reduce bookkeeping around the expert GEMMs
let public TRACE_TAG_DN = 11   // Gated DeltaNet layers (qwen35 hybrid) — separates dn from gated attn
def public trace_tag(t : int) {
    if (g_trace_tags) {
        jobque_trace_tag(t)
    }
}
def public trace_marker(name : string; arg : int) {
    // instant unit boundary ("token", "layer", "step") on the caller lane; same gate as trace_tag
    if (g_trace_tags && is_job_que_available()) {
        profile_marker(name, arg)
    }
}
// names/colors match the lane-timeline viewer's dark-theme palette, so saved traces carry
// the exact legend the artifact hardcoded
def private register_trace_categories() {
    profile_category(TRACE_TAG_ATTN_CHAIN, "attn chain", 0x5E96DEu)
    profile_category(TRACE_TAG_ATTN_STD, "attn std", 0x5E96DEu)
    profile_category(TRACE_TAG_FFN_CHAIN, "ffn chain", 0x43B08Au)
    profile_category(TRACE_TAG_FFN_STD, "ffn per-op", 0x43B08Au)
    profile_category(TRACE_TAG_CLS, "classifier", 0xAC82E4u)
    profile_category(TRACE_TAG_MOE, "moe experts", 0xE08563u)
    profile_category(TRACE_TAG_SHEXP, "shexp", 0xC39536u)
    profile_category(TRACE_TAG_PLE, "ple", 0xC39536u)
    profile_category(TRACE_TAG_ROUTE, "router", 0x4FC3D0u)
    profile_category(TRACE_TAG_PARK, "park/reduce", 0xD678B4u)
}

//! Configure the job queue for dasLLAMA's pure fork/join matmul dispatch: pooled fork contexts,
//! batched dispatch, spin-before-park window, join-poll level. Call INSIDE `with_job_que` (or
//! after `create_job_que`) — the queue must exist. Engine spelling of `setup_dasllama_jobque`.
def setup_dasllama_jobque_() {
    set_jobque_fork_pool(true, true)
    set_jobque_batch_dispatch(true)
    // team dispatch (#3368) as the runner default: fifo fork/join costs ~10us PER JOB, so a
    // 96-lane dispatch is ~1ms x ~160 dispatches/decoded token — EPYC 9654 measured Llama-1B
    // decode 7.6 -> 43 t/s (5.7x) and prefill-512 2x from this one switch (epyc9654_measurements.md)
    set_jobque_team_mode(true)
    set_jobque_worker_spin(int(min(g_jobque_spin_us, 2147483647l)))
    set_jobque_join_spin(int(min(g_jobque_join_poll, 2147483647l)))
    set_jobque_worker_limit(g_dispatch_worker_limit > 0 ? g_dispatch_worker_limit : -1)   // -1 = unlimited
    if (g_team_rank_gate >= 0 && !has_env_variable("DAS_JOBQUE_TEAM_RANK_GATE")) {
        set_jobque_team_rank_gate(g_team_rank_gate != 0)
    }
    // The dispatch caller is load-bearing in team mode — only it publishes chains, so a
    // descheduled caller idles every lane (4B Q4 32L trace: 13 all-lane holes/token, 1.7% worst).
    // Que creation ran this thread at High; claim the top notch. DASLLAMA_CALLER_PRIO=<-2..2> is the A/B rail.
    var prio = 2
    if (has_env_variable("DASLLAMA_CALLER_PRIO")) {
        prio = to_int(get_env_variable("DASLLAMA_CALLER_PRIO"))
    }
    set_current_thread_priority(clamp(prio, -2, 2))
}

// dasLLAMA matmuls are lockstep linear algebra: SMT siblings share the FMA/load ports, so the
// default jobque gets physical cores - 1 workers (ggml's -t default; EPYC 9654 confirmed 192 SMT
// lanes lose to 96). Cap min()s with the stock rule; DAS_JOBQUE_THREADS overrides.
[init]
def dasllama_jobque_threads_cap() {
    set_jobque_threads_cap(max(get_total_hw_cores() - 1, 1))
}

// Chunks-per-hw-job for the quantized matmuls' row split (the fp32 kernels use 1). Q8's
// dequant-in-dot is compute-bound and balances better at 2 chunks/job; Q4's heavy nibble
// unpack at 4 (both measured M1 Max). Per-box knobs; clamped to >= 1.
var g_q8_chunks_per_job = 2
var g_q4_chunks_per_job = 4
def public set_q8_chunks_per_job(v : int) { g_q8_chunks_per_job = max(v, 1) }
def public get_q8_chunks_per_job() : int { return g_q8_chunks_per_job }
def public set_q4_chunks_per_job(v : int) { g_q4_chunks_per_job = max(v, 1) }
def public get_q4_chunks_per_job() : int { return g_q4_chunks_per_job }

// Oversplit factor for the quantized BATCH matmuls (prefill GEMM): chunks = hw_jobs * this.
// More chunks than workers lets awake workers pop a late-woken worker's remaining chunks off the
// fifo (3990X @32 workers: fork/join bill -41% -> -15% at x4; x8 is past the per-chunk-overhead knee).
var g_q8_batch_chunks_per_job = 4
def public set_q8_batch_chunks_per_job(v : int) { g_q8_batch_chunks_per_job = max(v, 1) }
def public get_q8_batch_chunks_per_job() : int { return g_q8_batch_chunks_per_job }

// Dispatch lanes for a fork/join chunk split: worker threads PLUS the calling thread (team mode
// by design). Sizing chunk counts to workers alone leaves the last wave short (7 workers x4 = 28
// chunks/8 lanes = 87.5%); sizing to lanes took decode GEMV ~109 -> ~118 GB/s on M1 Max.
var g_dispatch_caller_lane = true
def public set_dispatch_caller_lane(v : bool) { g_dispatch_caller_lane = v }
def public get_dispatch_caller_lane() : bool { return g_dispatch_caller_lane }
def public get_dispatch_lanes() : int {
    // Que-less process: the calling thread is the only lane, so every shaper answers "inline".
    // (The work-proportional shapers evaluate eagerly at the dispatch sites — the old bool-gated
    // form only read this in its parallel arm, so que-less small-matmul callers never noticed.)
    if (!is_job_que_available()) {
        return 1
    }
    // the live pool: dormant over-limit workers (set_jobque_worker_limit) never serve chunks,
    // so the shapers must size waves to the awake workers only
    let live = min(get_total_hw_jobs(), get_jobque_worker_limit())
    return live + (g_dispatch_caller_lane ? 1 : 0)
}

def public get_dispatch_slot_bound() : int {
    // exclusive upper bound on the slot maybe_parallel_for_indexed hands its block — size
    // per-slot scratch to this. Caller participates as the LAST slot; a worker limit only shrinks
    // the live set, so dormant workers keep their high slot numbers (bound stays pool-sized).
    return is_job_que_available() ? get_total_hw_jobs() + 1 : 1
}

// Min grain for matmul row splits (ggml-cpu.c mul_mat chunk_size = 16): without it a fixed
// lanes×oversplit chunk count shatters a small matmul into sub-grain work items on many-core
// boxes (EPYC 9654: kv 2048x512 GEMM 2.5x slower at 192 lanes than at 48). 0 disables.
var g_matmul_min_chunk_rows = 16
def public set_matmul_min_chunk_rows(v : int) { g_matmul_min_chunk_rows = max(v, 0) }
def public get_matmul_min_chunk_rows() : int { return g_matmul_min_chunk_rows }

// The dispatch invariant that killed the prefill regression: a chunk count > lanes MUST be a whole
// multiple of dispatch lanes, so no wave is ragged (a partial final wave makes the join wait on
// stragglers). Fires in debug/test builds so a future chunk-math change can't silently reintroduce it.
def private assert_wave_aligned(chunks, lanes : int) {
    assert(chunks <= lanes || chunks % lanes == 0, "matmul dispatch chunk count must be a whole number of dispatch-lane waves")
}

// ===== work-proportional dispatch (lanes sized to the matmul, not the box) =====

// MACs of work that justify one dispatch chunk: lanes_for_work invites one lane per this much
// work (grain floors still keep tiny matmuls inline). Default 1 = dispatch everything the grains
// admit — the old 1M default left every per-layer attn matmul of a <=4B model single-lane (-5..-17% tok/s).
var g_target_chunk_work = 1l
def public set_target_chunk_work(v : int64) { g_target_chunk_work = max(v, 1l) }
def public get_target_chunk_work() : int64 { return g_target_chunk_work }

// Per-regime lane caps (0 = uncapped). GEMV rides memory bandwidth — past the box's DRAM knee
// extra lanes just queue at the memory door (team_probe's streaming ladder finds the knee).
// Batch (GEMM) is compute-bound and caps at the overhead knee instead.
var g_gemv_lane_cap = 0
def public set_gemv_lane_cap(v : int) { g_gemv_lane_cap = max(v, 0) }
def public get_gemv_lane_cap() : int { return g_gemv_lane_cap }

var g_batch_lane_cap = 0
def public set_batch_lane_cap(v : int) { g_batch_lane_cap = max(v, 0) }
def public get_batch_lane_cap() : int { return g_batch_lane_cap }

//! Dispatch lanes proportional to work: clamp(work / target_chunk_work, 1,
//! min(get_dispatch_lanes(), lane_cap)). 1 means "not worth a dispatch" — the matmul_chunks
//! shapers return 1 and the num_jobs form of maybe_parallel_for runs inline. lane_cap 0 = uncapped.
def public lanes_for_work(work : int64; lane_cap : int) : int {
    let lanes = max(get_dispatch_lanes(), 1)
    let cap = lane_cap > 0 ? min(lanes, lane_cap) : lanes
    return int(clamp(work / g_target_chunk_work, 1l, int64(cap)))
}

//! Work-aware chunk count for a batch (GEMM) row split: L = lanes_for_work(work, batch cap);
//! 1 => inline; else chunks rounded DOWN to a whole number of L-waves so no ragged final wave
//! makes the join wait on stragglers. `rows` is the split range's total row count; `work` = MAC count.
def public matmul_chunks(rows : int; oversplit : int; work : int64) : int {
    let L = lanes_for_work(work, g_batch_lane_cap)
    if (L <= 1) {
        return 1
    }
    let want = L * max(oversplit, 1)
    if (g_matmul_min_chunk_rows <= 0) {
        return want
    }
    let cap = clamp(rows / g_matmul_min_chunk_rows, 1, want)   // grain-limited ceiling (<= want = L × oversplit)
    // the grain may only limit OVERSPLIT — never cut chunks below the admitted lanes (underfilling
    // L idles lanes for the whole dispatch, always worse than sub-grain chunks; rows < L is the
    // only natural floor)
    if (cap < L) {
        let chunks = clamp(rows, 1, L)
        assert_wave_aligned(chunks, L)
        return chunks
    }
    let chunks = (cap / L) * L                                 // whole L-waves within the grain cap
    assert_wave_aligned(chunks, L)
    return chunks
}

// ===== 2-D batch chunk grid (row-units × token-blocks) — the chunk-starved-shape remedy =====

// The per-box 2-D grid pin (box_profile "batch_grid_2d"): 0 = off (1-D always); 1/2 arm the
// per-dispatch auto-gate — 2-D engages only when the 1-D grain-capped chunk count can't fill one
// wave (a starved shape idles lanes); big GEMMs keep the weight-stationary 1-D walk regardless.
var g_batch_grid_2d = 0
def public set_batch_grid_2d(v : int) { g_batch_grid_2d = clamp(v, 0, 2) }
def public get_batch_grid_2d() : int { return g_batch_grid_2d }

def private gcd_int(a, b : int) : int {
    var x = max(a, 1)
    var y = max(b, 1)
    while (y != 0) {
        let t = y
        y = x % y
        x = t
    }
    return x
}

//! Chunk grid for the token-blocked batch GEMM: (row chunks, token chunks). y == 1 => stay 1-D.
//! With batch_grid_2d armed, a starved shape (1-D chunks < admitted lanes) goes 2-D: the token
//! axis supplies missing parallelism at the same row grain (cells decode rows-fastest, sharing LLC).
def public matmul_grid(units : int; unit_rows : int64; oversplit : int; work : int64; ntok : int64; tokq : int) : int2 {
    let rows = units * int(unit_rows)
    let c1 = matmul_chunks(rows, oversplit, work)
    if (g_batch_grid_2d == 0 || c1 <= 1) {
        return int2(c1, 1)
    }
    let L = lanes_for_work(work, g_batch_lane_cap)
    let ntile = int(ntok / int64(tokq))
    // grain-starved = the 1-D count only reaches L by going sub-grain (matmul_chunks fills to L);
    // the token axis supplies the missing parallelism at FULL row grain instead — weight rows
    // stream in grain-sized spans, re-read once per token cell
    let gcap = g_matmul_min_chunk_rows > 0 ? clamp(rows / g_matmul_min_chunk_rows, 1, c1) : c1
    if (gcap >= L || ntile < 2) {   // not grain-starved, or no token axis to split
        return int2(c1, 1)
    }
    let rc = clamp(gcap, 1, max(units, 1))   // row cells finer than one unit are empty claims
    var ntc = 1
    if (g_batch_grid_2d == 1) {
        ntc = min((int(ntok) + 15) / 16, ntile)
    } else {
        // smallest whole-wave ntc covering the oversplit target; alignment that can't fit the
        // available tiles stays 1-D (a clamped ntc would be unaligned = strategy 1 in disguise)
        let step = L / gcd_int(rc, L)
        let need = (L * max(oversplit, 1) + rc - 1) / rc
        ntc = ((need + step - 1) / step) * step
        if (ntc > ntile) {
            return int2(c1, 1)
        }
    }
    return ntc > 1 ? int2(rc, ntc) : int2(c1, 1)
}

// Coarser grain for GEMV-shaped splits (ggml mul_mat: chunk_size 16 -> 64 when nr0==1||nr1==1):
// a decode matvec row is far less work than a GEMM row reused across ntok tokens (EPYC 9654:
// grain 64 = +13-15% decode at default threads). 0 disables.
var g_matmul_min_chunk_rows_gemv = 64
def public set_matmul_min_chunk_rows_gemv(v : int) { g_matmul_min_chunk_rows_gemv = max(v, 0) }
def public get_matmul_min_chunk_rows_gemv() : int { return g_matmul_min_chunk_rows_gemv }

// GEMV tail: the one-chunk-per-lane equal split leaves the join waiting out the SLOWEST lane's
// whole chunk (median lane idles 14-24% of every GEMV stage, per-lane trace 3990X). >1 publishes
// wave-aligned chunks per lane so finished lanes keep serving; default 8; 1 = historical equal split.
var g_gemv_chunks_per_lane = 8
def public set_gemv_chunks_per_lane(v : int) { g_gemv_chunks_per_lane = max(v, 1) }
def public get_gemv_chunks_per_lane() : int { return g_gemv_chunks_per_lane }

// Wave fill: when the grain-derived count lands below admitted lanes, round UP to one full wave —
// underfilling always idles more than going sub-grain (30B decode router: 128 rows/grain 64 = 2
// chunks on 16 lanes, 14 idle, ~3.3% of token). rows < lanes is the only natural floor.
var g_gemv_wave_fill = true
def public set_gemv_wave_fill(v : bool) { g_gemv_wave_fill = v }
def public get_gemv_wave_fill() : bool { return g_gemv_wave_fill }

//! matmul_chunks for GEMV-shaped dispatches (single-token mm/group3/groupn): L = lanes_for_work
//! (work, gemv cap); 1 => inline; else the coarser GEMV grain, capped at ONE chunk per lane (a
//! decode chunk is one bandwidth stream — EPYC T=24: last lane's +1 chunk = 37% of op wall).
def public matmul_chunks_gemv(rows : int; oversplit : int; work : int64) : int {
    let L = lanes_for_work(work, g_gemv_lane_cap)
    if (L <= 1) {
        return 1
    }
    if (g_matmul_min_chunk_rows_gemv <= 0) {
        return L * max(oversplit, 1)
    }
    if (g_gemv_chunks_per_lane > 1) {
        let waves = min(rows / (g_matmul_min_chunk_rows_gemv * L), g_gemv_chunks_per_lane)
        if (waves >= 1) {
            let chunks = waves * L
            assert_wave_aligned(chunks, L)
            return chunks
        }
        // fewer than one grain-sized chunk per lane — the grain-limited split below applies
    }
    let n = clamp(rows / g_matmul_min_chunk_rows_gemv, 1, L)
    let chunks = (g_gemv_wave_fill && n < L) ? clamp(rows, 1, L) : n
    assert_wave_aligned(chunks, L)
    return chunks
}

//! LPT (longest-processing-time-first) claim order for a region-list dispatch with known unequal
//! per-region costs: sort (id, cost) biggest-cost first (ties: lowest id) so the self-serve chunk
//! queue front-loads expensive regions and drains cheap ones last. Use whenever cost is known at build time.
def public lpt_order(var ord : array<int2>) {
    ord |> sort() $(a, b) => a.y != b.y ? a.y > b.y : a.x < b.x
}

//! Chunk count for a fused-chain stage over `groups` 32-row quant groups: the GEMV shaper's count,
//! except fine-grain mode (gemv_chunks_per_lane > 1) splits at GROUP granularity (capped at
//! lanes×fine) so a ragged last wave costs one 32-row chunk, not a whole lane's share (~25% join tail fix, 3990X).
def public chain_stage_chunks(groups : int; rows : int; work : int64) : int {
    if (g_gemv_chunks_per_lane > 1) {
        let L = lanes_for_work(work, g_gemv_lane_cap)
        if (L > 1) {
            return min(groups, L * g_gemv_chunks_per_lane)
        }
    }
    return matmul_chunks_gemv(rows, 1, work)
}

//! y = W·x — matrix-vector product, the dominant LLM compute kernel. W is row-major [d x n],
//! x is [n], y is [d]; each y[i] is one dot of W's row i with x (matches llama2.c: n=input, d=output).
//! Large matmuls thread output rows (requires an enclosing with_job_que); small stay single-thread.
def matmul(var y : array<float>; w : array<float> | #; x : array<float>; n, d : int64) {
    unsafe {
        var yp = addr(y[0])                              // var: stays a writable pointer when captured
        let wp = addr<float const?>(w[0])   // strip the `#` if w is a view
        let xp = addr(x[0])
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), 1, n * d)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot(wp + int64(i) * n, xp, n)
                }
            }
        }
    }
}

//! Blob+offset form: the weight is rows [d x n] at element offset `woff` inside `w` — no
//! array_view (the view's block invoke is an LLVM optimization boundary; the kernel only ever
//! wanted the base pointer).
def matmul(var y : array<float>; w : array<float>; woff : int64; x : array<float>; n, d : int64) {
    unsafe {
        var yp = addr(y[0])
        let wp = addr<float const?>(w[woff])
        let xp = addr(x[0])
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), 1, n * d)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot(wp + int64(i) * n, xp, n)
                }
            }
        }
    }
}

//! Batched fp32 GEMM for prefill: Y [ntok x d] = X [ntok x n]·Wᵀ, W row-major [d x n]. Both
//! token-major. Weight-stationary nest (rows outer) — each weight row streams from DRAM once,
//! reused across all ntok tokens (vs ntok× matmul re-reading W). Bit-for-bit ntok× matmul.
def matmul_batch(var y : array<float>; w : array<float> | #; x : array<float>; n, d, ntok : int64) {
    unsafe {
        matmul_batch_core(addr(y[0]), addr<float const?>(w[0]), addr<float const?>(x[0]), n, d, ntok)
    }
}

// The fp32 batch GEMM core: 1-D row split, except ROW-STARVED shapes (MoE router: d=n_expert=128,
// grain 16 => 8 chunks idling half a 16-lane box) split tokens too — a flat (row-chunk × token-block)
// domain. Disjoint stores, same dot per (row, token) — bit-identical either way.
def private matmul_batch_core(var yp : float?; wp : float const?; xp : float const?; n, d, ntok : int64) {
    var myp = yp
    let rch = matmul_chunks(int(d), get_q8_batch_chunks_per_job(), n * d * ntok)
    let L = lanes_for_work(n * d * ntok, g_batch_lane_cap)
    let tb = (rch < L && ntok >= 128l) ? min(int64((2 * L + rch - 1) / rch), ntok / 64l) : 1l
    if (tb <= 1l) {
        maybe_parallel_for(0, int(d), rch) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    let wrow = wp + int64(i) * n
                    for (tk in range64(ntok)) {
                        myp[tk * d + int64(i)] = dot(wrow, xp + tk * n, n)
                    }
                }
            }
        }
        return
    }
    let rows_per = (d + int64(rch) - 1l) / int64(rch)
    let tok_per = (ntok + tb - 1l) / tb
    maybe_parallel_for(0, int(int64(rch) * tb), int(int64(rch) * tb)) $(cb, ce) {
        unsafe {
            for (c in range(cb, ce)) {
                let rc = int64(c) / tb
                let tc = int64(c) % tb
                let r1 = min(rc * rows_per + rows_per, d)
                let t1 = min(tc * tok_per + tok_per, ntok)
                for (i in range64(rc * rows_per, r1)) {
                    let wrow = wp + i * n
                    for (tk in range64(tc * tok_per, t1)) {
                        myp[tk * d + i] = dot(wrow, xp + tk * n, n)
                    }
                }
            }
        }
    }
}

//! Blob+offset form of matmul_batch (see the matmul woff overload).
def matmul_batch(var y : array<float>; w : array<float>; woff : int64; x : array<float>; n, d, ntok : int64) {
    unsafe {
        matmul_batch_core(addr(y[0]), addr<float const?>(w[woff]), addr<float const?>(x[0]), n, d, ntok)
    }
}

//! Batched matvec against a native-BF16 weight [d x n] at halfword offset woff: Y [ntok x d]
//! token-major. Bit-for-bit matmul_batch's fp32 expand at half the weight read (dot_bf16's widen
//! is exact). Decode (ntok == 1) takes the GEMV chunk shaper.
def matmul_bf16_batch(var y : array<float>; w : array<uint16>; woff : int64; x : array<float>; n, d, ntok : int64) {
    unsafe {
        var yp = addr(y[0])
        let wp = addr<uint16 const?>(w[woff])
        let xp = addr<float const?>(x[0])
        let nch = ntok == 1l ? matmul_chunks_gemv(int(d), 1, n * d) : matmul_chunks(int(d), get_q8_batch_chunks_per_job(), n * d * ntok)
        maybe_parallel_for(0, int(d), nch) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    let wrow = wp + int64(i) * n
                    for (tk in range64(ntok)) {
                        yp[tk * d + int64(i)] = dot_bf16(xp + tk * n, wrow, n)
                    }
                }
            }
        }
    }
}

// ===== fp32 micro-GEMM: C[M x N] += A[M x K] * B[K x N] (B's N contiguous = SIMD dim) =====
// Broadcast-A FMA register-tile form (zero horizontal reduce) that makes flash attention's QKᵀ
// and P·V fast; single-thread, pointer-based (fork-callable). Tolerance-equal to scalar (FMA reorders low bits).

// Scalar C += A·B over rows [i0,i1) × cols [j0,j1) — the L-shaped remainder the 4×16 tile leaves.
[hint(noalias = cp, noalias = ap, noalias = bp)]
def private gemm_f32_region(var cp : float?; ap, bp : float const?; i0, i1, j0, j1, K, N : int64) {
    unsafe {
        for (i in range64(i0, i1)) {
            let crow = i * N
            for (kk in range64(K)) {
                let a = ap[i * K + kk]
                let brow = kk * N
                for (j in range64(j0, j1)) {
                    cp[crow + j] += a * bp[brow + j]
                }
            }
        }
    }
}

// 4-row × 16-col tile: 16 float4 accumulators, broadcast-A FMA, no horizontal reduce. C += (cols
// [j0,j0+16) of rows [i0,i0+4)). N is the row stride of C/B; K the row stride / inner dim of A/B.
// Template public (not private): the tuner harness's cross-module [dasllama_grid] scan must see it.
[hint(noalias = cp, noalias = ap, noalias = bp)]
def template gemm_f32_uk_4x16_template(var cp : float ?; ap, bp : float const ?; i0, j0, K, N : int64) : void {
    unsafe {
        let o0 = (i0 + 0l) * N + j0; let o1 = (i0 + 1l) * N + j0
        let o2 = (i0 + 2l) * N + j0; let o3 = (i0 + 3l) * N + j0
        var r0a = *reinterpret<float4?>(cp + o0);       var r0b = *reinterpret<float4?>(cp + o0 + 4l)
        var r0c = *reinterpret<float4?>(cp + o0 + 8l);  var r0d = *reinterpret<float4?>(cp + o0 + 12l)
        var r1a = *reinterpret<float4?>(cp + o1);       var r1b = *reinterpret<float4?>(cp + o1 + 4l)
        var r1c = *reinterpret<float4?>(cp + o1 + 8l);  var r1d = *reinterpret<float4?>(cp + o1 + 12l)
        var r2a = *reinterpret<float4?>(cp + o2);       var r2b = *reinterpret<float4?>(cp + o2 + 4l)
        var r2c = *reinterpret<float4?>(cp + o2 + 8l);  var r2d = *reinterpret<float4?>(cp + o2 + 12l)
        var r3a = *reinterpret<float4?>(cp + o3);       var r3b = *reinterpret<float4?>(cp + o3 + 4l)
        var r3c = *reinterpret<float4?>(cp + o3 + 8l);  var r3d = *reinterpret<float4?>(cp + o3 + 12l)
        for [tune = 1] (kk in range64(K)) {             // fallback u2: ~3% over the rolled loop; same accumulation order
            let bo = kk * N + j0
            let b0 = *reinterpret<float4 const?>(bp + bo);      let b1 = *reinterpret<float4 const?>(bp + bo + 4l)
            let b2 = *reinterpret<float4 const?>(bp + bo + 8l); let b3 = *reinterpret<float4 const?>(bp + bo + 12l)
            let a0 = ap[(i0 + 0l) * K + kk]; let v0 = float4(a0, a0, a0, a0)
            r0a = mad(b0, v0, r0a); r0b = mad(b1, v0, r0b); r0c = mad(b2, v0, r0c); r0d = mad(b3, v0, r0d)
            let a1 = ap[(i0 + 1l) * K + kk]; let v1 = float4(a1, a1, a1, a1)
            r1a = mad(b0, v1, r1a); r1b = mad(b1, v1, r1b); r1c = mad(b2, v1, r1c); r1d = mad(b3, v1, r1d)
            let a2 = ap[(i0 + 2l) * K + kk]; let v2 = float4(a2, a2, a2, a2)
            r2a = mad(b0, v2, r2a); r2b = mad(b1, v2, r2b); r2c = mad(b2, v2, r2c); r2d = mad(b3, v2, r2d)
            let a3 = ap[(i0 + 3l) * K + kk]; let v3 = float4(a3, a3, a3, a3)
            r3a = mad(b0, v3, r3a); r3b = mad(b1, v3, r3b); r3c = mad(b2, v3, r3c); r3d = mad(b3, v3, r3d)
        }
        *reinterpret<float4?>(cp + o0) = r0a;       *reinterpret<float4?>(cp + o0 + 4l) = r0b
        *reinterpret<float4?>(cp + o0 + 8l) = r0c;  *reinterpret<float4?>(cp + o0 + 12l) = r0d
        *reinterpret<float4?>(cp + o1) = r1a;       *reinterpret<float4?>(cp + o1 + 4l) = r1b
        *reinterpret<float4?>(cp + o1 + 8l) = r1c;  *reinterpret<float4?>(cp + o1 + 12l) = r1d
        *reinterpret<float4?>(cp + o2) = r2a;       *reinterpret<float4?>(cp + o2 + 4l) = r2b
        *reinterpret<float4?>(cp + o2 + 8l) = r2c;  *reinterpret<float4?>(cp + o2 + 12l) = r2d
        *reinterpret<float4?>(cp + o3) = r3a;       *reinterpret<float4?>(cp + o3 + 4l) = r3b
        *reinterpret<float4?>(cp + o3 + 8l) = r3c;  *reinterpret<float4?>(cp + o3 + 12l) = r3d
    }
}

[hint(noalias = cp, noalias = ap, noalias = bp), tuned(fallback = "u2")]
def private gemm_f32_uk_4x16(var cp : float?; ap, bp : float const?; i0, j0, K, N : int64) {}

//! Register-tiled fp32 GEMM, C[M x N] += A[M x K] · B[K x N]. 4×16 tile over the bulk; the
//! L-shaped remainder (cols N%16, rows M%4) falls back to scalar. General over M/K/N. C must be
//! pre-initialized (the kernel accumulates). 2-3.5× the scalar form on the attention tile shapes.
[hint(noalias = cp, noalias = ap, noalias = bp)]
def gemm_f32(var cp : float?; ap, bp : float const?; M, K, N : int64) {
    let m4 = M - M % 4l
    let n16 = N - N % 16l
    var ii = 0l
    while (ii < m4) {
        var jj = 0l
        while (jj < n16) {
            gemm_f32_uk_4x16(cp, ap, bp, ii, jj, K, N)
            jj += 16l
        }
        ii += 4l
    }
    if (n16 < N) {                                   // right strip: rows [0,m4) × cols [n16,N)
        gemm_f32_region(cp, ap, bp, 0l, m4, n16, N, K, N)
    }
    if (m4 < M) {                                    // bottom strip: rows [m4,M) × all cols
        gemm_f32_region(cp, ap, bp, m4, M, 0l, N, K, N)
    }
}

//! gemm_f32 with the tile walk COLUMN-BLOCKS OUTER (jj outer, ii inner): the K×16 B strip stays
//! L1-hot across row blocks, so big-M streams B once instead of M/4 times — numerically identical
//! to gemm_f32. Wins on big-B multi-thread shapes (parakeet attn −20%); small/reused B keeps gemm_f32.
[hint(noalias = cp, noalias = ap, noalias = bp)]
def gemm_f32_jo(var cp : float?; ap, bp : float const?; M, K, N : int64) {
    let m4 = M - M % 4l
    let n16 = N - N % 16l
    var jj = 0l
    while (jj < n16) {
        var ii = 0l
        while (ii < m4) {
            gemm_f32_uk_4x16(cp, ap, bp, ii, jj, K, N)
            ii += 4l
        }
        jj += 16l
    }
    if (n16 < N) {                                   // right strip: rows [0,m4) × cols [n16,N)
        gemm_f32_region(cp, ap, bp, 0l, m4, n16, N, K, N)
    }
    if (m4 < M) {                                    // bottom strip: rows [m4,M) × all cols
        gemm_f32_region(cp, ap, bp, m4, M, 0l, N, K, N)
    }
}

//! Q8_0 dequant-in-dot: `wq` int8 quants + fp32 scale `ws` per 32-block, `x` fp32. float4
//! accumulator carried across blocks (scale applied per block as broadcast fma), single
//! horizontal reduce at the end — won the kernel bench (see matmul_variants). n % 32 == 0.
def dot_q8(wq : int8 const?; ws : float const?; x : float const?; n : int64) : float {
    let nb = n / 32l
    var acc = float4(0.0, 0.0, 0.0, 0.0)
    for (bi in range64(nb)) {
        let base = bi * 32l
        let s = unsafe(ws[bi])
        var bacc = float4(0.0, 0.0, 0.0, 0.0)
        var c = 0l
        while (c < 32l) {
            let off = base + c
            unsafe {
                let q4 = float4(float(wq[off]), float(wq[off + 1l]), float(wq[off + 2l]), float(wq[off + 3l]))
                let x4 = *addr<float4?>(x[off])
                bacc += q4 * x4
            }
            c += 4l
        }
        acc += float4(s, s, s, s) * bacc
    }
    return acc.x + acc.y + acc.z + acc.w
}

//! Q8_0 matmul: y = W·x, W as int8 quants `wq` [d x n] + per-block scales `ws` [d x n/32]. Large
//! matmuls thread output rows at 2x worker count (Q8 is compute-bound, threads better than fp32).
//! n % 32 == 0. Blob+offset: woff in elements (scales at woff/32).
def matmul_q8(var y : array<float>; wq : array<int8>; ws : array<float>; woff : int64; x : array<float>; n, d : int64) {
    let nb = n / 32l
    unsafe {
        var yp = addr(y[0])
        let wp = addr<int8 const?>(wq[woff])
        let sp = addr<float const?>(ws[woff / 32l])
        let xp = addr(x[0])
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), g_q8_chunks_per_job, n * d)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot_q8(wp + int64(i) * n, sp + int64(i) * nb, xp, n)
                }
            }
        }
    }
}

def matmul_q8(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; x : array<float>; n, d : int64) {
    let nb = n / 32l
    unsafe {
        var yp = addr(y[0])
        let wp = addr<int8 const?>(wq[0])
        let sp = addr<float const?>(ws[0])
        let xp = addr(x[0])
        // default 2 chunks per hw job (knob): Q8 dequant-in-dot is compute-bound and threads better than fp32
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), g_q8_chunks_per_job, n * d)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot_q8(wp + int64(i) * n, sp + int64(i) * nb, xp, n)
                }
            }
        }
    }
}

// ===== Token-block width (shared, ISA-agnostic) =====
// A token-blocked batch kernel (arm64 laneq, x64 grp4) tiles tokens into TB-sized blocks so a
// TB×n activation slice stays L2-resident. Tuned default 128 (Llama-3.2-1B, M1 Max); clamped >= 1.
var g_q8_token_block = 128l
def public set_q8_token_block(tb : int64) { g_q8_token_block = max(tb, 1l) }
def public get_q8_token_block() : int64 { return g_q8_token_block }

// ===== Token-block L2 budget (shared, ISA-agnostic) =====
// A token-blocked Q8·Q8 GEMM keeps TB×n L2-resident; past L2 the slice spills and prefill falls
// off a cliff (M1 Max: knee at TB ≈ L2/n). TB clamped to this budget; default 4MB is PROVISIONAL.
var g_q8_l2_budget = 4l * 1024l * 1024l
def public set_q8_l2_budget(bytes : int64) { g_q8_l2_budget = max(bytes, 1l) }
def public get_q8_l2_budget() : int64 { return g_q8_l2_budget }

//! Clamp a requested token-block width so the TB×n int8 activation slice stays within the per-box L2
//! budget. n = contraction dim (one int8 per activation element). Returns >= 1.
def public effective_token_block(tb, n : int64) : int64 {
    return clamp(tb, 1l, max(1l, g_q8_l2_budget / max(n, 1l)))   // floor 1 (a 0 block width infinite-loops the kernel)
}

// ===== dispatch observability (jobs-per-section profiling) =====
// maybe_parallel_for's macro emits these on every decision, so a side-by-side vs llama.cpp can
// compare dispatch counts and chunk granularity, not just time. Counters live on the MAIN context.
var g_disp_count = 0l    // parallel_for dispatches taken
var g_disp_chunks = 0l   // chunks (jobs) pushed across those dispatches
var g_disp_inline = 0l   // maybe_parallel_for calls that ran inline instead
def count_parallel_dispatch(njobs : int) {
    g_disp_count++
    g_disp_chunks += int64(njobs)
}
// pass-through flavor for maybe_parallel_for: profiles the exact value the dispatch receives
// while keeping num_jobs a single evaluation (callers pass shaper calls)
def count_parallel_dispatch_thru(njobs : int) : int {
    count_parallel_dispatch(njobs)
    return njobs
}
def count_inline_run() {
    g_disp_inline++
}
def get_disp_count() : int64 => g_disp_count
def get_disp_chunks() : int64 => g_disp_chunks
def get_disp_inline() : int64 => g_disp_inline

// ===== Q8·Q8 matmul: pluggable kernel-backend registry =====
// A *backend* (KernelBackend) is a named kernel-set + weight repack, registered per ISA at [init];
// row-major backends auto-activate, repack backends (arm64 laneq) activate only at load (pin_kernel_backend forces one for A/B).
typedef MatmulQ8Q8Fn = function<(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64) : void>
typedef MatmulQ8Q8BatchFn = function<(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64; ntok : int64) : void>
typedef MatmulQ8Q8Group3Fn = function<(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?; woff0 : int64; woff1 : int64; woff2 : int64; xqp : int8 const?; xsp : float const?; n : int64; d0 : int64; d1 : int64; d2 : int64) : void>
// In-place repack of one Q8 weight region [d rows x n cols] (raw pointers at the region start) into
// the active backend's preferred layout. Default = no-op (row-major). n % 32 == 0.
typedef RepackQ8Q8Fn = function<(var wp : int8?; var sp : float?; n : int64; d : int64) : void>
// MXFP4·Q8 GEMV: W kept in its native 4-bit form as two planes — `wn` packed nibbles (16 bytes per
// 32-block, row stride n/2) + `we` RAW E8M0 scale bytes (1 per block, row stride n/32) — against a
// pre-quantized Q8 activation. Native half-traffic path for MoE expert stacks (gpt-oss decode).
typedef MatmulMx4Q8Fn = function<(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64) : void>
// Batched MXFP4·Q8 GEMM — the batch analog of MatmulMx4Q8Fn (Y token-major [ntok x d], activations
// pre-quantized per token like MatmulQ8Q8BatchFn). Grouped MoE prefill dispatches here INSTEAD of
// expanding the expert to Q8 scratch when the backend sets mx4_native_batch.
typedef MatmulMx4Q8BatchFn = function<(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64; ntok : int64) : void>
// In-place repack of one MXFP4 region [d rows x n cols] (raw plane pointers at the region start)
// into the active backend's preferred layout. Default = no-op (row-major). n % 32 == 0.
typedef RepackMx4Fn = function<(var np : uint8?; var ep : uint8?; n : int64; d : int64) : void>
// Region-list GEMV, ONE dispatch for N same-shape [d x n] projections (MoE decode fuse: all k
// routed experts' gates/ups/downs in one fork/join). `offs` packs PAIRS [weight offset, activation
// offset] per region; region r -> yp+r*d. bp folds a per-region bias at the store (~36us/layer saved).
typedef MatmulQ8Q8GroupNFn = function<(var yp : float?; wp : int8 const?; sp : float const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n : int64; d : int64) : void>
// Row-range GEMV core: compute y rows [rb,re) of ONE [d x n] region with NO internal dispatch —
// the caller owns threading, letting a fused multi-stage decode chain serve matmul rows from
// inside its own stage lambdas. rb/re must be multiples of 32; bit-identical to the mm slot.
typedef MatmulQ8Q8RowsFn = function<(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n : int64; rb : int64; re : int64) : void>
typedef MatmulMx4GroupNFn = function<(var yp : float?; wn : uint8 const?; we : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n : int64; d : int64) : void>
// f16-weight-scale twins (the wscale_f16 rail): identical contracts to the Fn above them, except
// the per-32-block WEIGHT scale plane is raw binary16 halfwords (Model.qscales16) instead of f32 —
// llama.cpp byte parity, 34 vs 36 B per 32 weights. Activation scales (xsp) stay f32 everywhere.
typedef MatmulQ8Q8FnS16 = function<(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64) : void>
typedef MatmulQ8Q8BatchFnS16 = function<(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n : int64; d : int64; ntok : int64) : void>
typedef MatmulQ8Q8Group3FnS16 = function<(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : uint16 const?; woff0 : int64; woff1 : int64; woff2 : int64; xqp : int8 const?; xsp : float const?; n : int64; d0 : int64; d1 : int64; d2 : int64) : void>
typedef MatmulQ8Q8GroupNFnS16 = function<(var yp : float?; wp : int8 const?; sp : uint16 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n : int64; d : int64) : void>
typedef MatmulQ8Q8RowsFnS16 = function<(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n : int64; rb : int64; re : int64) : void>
// Is this backend selectable RIGHT NOW? Evaluated at (load-)select/pin time — NOT at registration
// ([init]s run before the JIT installs generated code, so a predicate reading a JIT-stamped value
// answers with its reference tier there). Lets a module register several runtime-dependent flavors.
typedef BackendAvailableFn = function<() : bool>
// Region-list batched MXFP4·Q8 GEMM — batch analog of MatmulMx4GroupNFn, llama.cpp's mul_mat_id
// shape: ONE dispatch runs every routed expert's [d x n] GEMM over its own row range of a shared
// token-row image. `offs` packs TRIPLES [weight offset, row0, cnt]; bit-exact vs a per-region loop.
typedef MatmulMx4Q8BatchGroupNFn = function<(var yp : float?; wn : uint8 const?; we : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n : int64; d : int64) : void>
// Native K-quant rows-range GEMV core (kq stage 4): rows [rb, re) of one plane-region pair
// (kqp/ksp = the format's quant/scale plane bases). One typedef per format; a backend's cores
// read ITS repacked kq layout — mixing backends across a load is as illegal as for the Q8 plane.
typedef MatmulKqRowsFn = function<(var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; rb : int64; re : int64) : void>
// The fmt-branched full K-quant GEMV (fmt 4/5/6; internal fork over d rows) and plane repack.
typedef MatmulKqFn = function<(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; d : int64) : void>
typedef RepackKqFn = function<(fmt : int; var kq : uint8?; var ks : uint8?; n : int64; d : int64) : void>
// Batched K-quant GEMM (the kq prefill tile): activations an [ntok x n] image with per-token
// scale (n/32) and bsum (n/16) rows; Y token-major [ntok x d]. Bit-for-bit ntok x the fmt's
// per-position GEMV over the same repacked planes.
typedef MatmulKqBatchFn = function<(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; d : int64; ntok : int64) : void>
// Batched region-list K-quant GEMM (kq batch groupn — fused MoE prefill, llama.cpp's mul_mat_id
// shape): `offs` packs TRIPLES [weight offset, row0, cnt]; region r consumes/produces activation
// rows row0..row0+cnt (token-major). Plane bases pass whole; no bias form (biased arches are mx4).
typedef MatmulKqBatchGroupNFn = function<(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; d : int64) : void>
// Region-list K-quant GEMV (kq groupn — MoE routed-expert decode dispatch): `offs` packs
// (weight, activation) element-offset PAIRS, region r's rows land at y[r*d .. r*d+d); kqp/ksp are
// plane bases (each region computes its own superblock offset). No bias form: gpt-oss carries mx4, never kq.
typedef MatmulKqGroupNFn = function<(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; d : int64) : void>

//! One kernel backend: the three Q8·Q8 matmul kernels + the weight repack, plus selection metadata.
//! needs_repack = kernels read an interleaved layout, so the loader repacks first (mm_mx4/repack_mx4
//! follow the same flag). priority: higher wins auto-select (portable 0; arm64 sdot 10/laneq 20).
struct KernelBackend {
    name : string
    mm : MatmulQ8Q8Fn
    batch : MatmulQ8Q8BatchFn
    group3 : MatmulQ8Q8Group3Fn
    repack : RepackQ8Q8Fn
    mm_mx4 : MatmulMx4Q8Fn = @@mx4q8_unset_kernel
    // The mx4 BATCH kernels are optional (unlike mm_mx4): only dispatched when mx4_native_batch is
    // set, so backends without them keep the panic stubs (a row-major fallback over an interleaved
    // plane would read garbage, not just run slow) and use the expansion path instead.
    mx4_batch : MatmulMx4Q8BatchFn = @@mx4q8_unset_batch
    mx4_batch_groupn : MatmulMx4Q8BatchGroupNFn = @@mx4q8_unset_batch_groupn
    repack_mx4 : RepackMx4Fn = @@repack_mx4_identity
    groupn : MatmulQ8Q8GroupNFn = @@q8q8_unset_groupn
    groupn_mx4 : MatmulMx4GroupNFn = @@mx4q8_unset_groupn
    // Optional row-range GEMV core (see MatmulQ8Q8RowsFn) — backends without one keep the panic
    // stub, and the fused multi-stage decode falls back to the per-op dispatch path.
    mm_rows : MatmulQ8Q8RowsFn = @@q8q8_unset_rows
    // Optional f16-scale twins (the wscale_f16 rail). All-or-nothing per backend: the loader's
    // plane convert gates on kernel_backend_has_wscale16(), keyed off mm_s16 alone.
    mm_s16 : MatmulQ8Q8FnS16 = @@q8q8_unset_kernel_s16
    batch_s16 : MatmulQ8Q8BatchFnS16 = @@q8q8_unset_batch_s16
    group3_s16 : MatmulQ8Q8Group3FnS16 = @@q8q8_unset_group3_s16
    groupn_s16 : MatmulQ8Q8GroupNFnS16 = @@q8q8_unset_groupn_s16
    mm_rows_s16 : MatmulQ8Q8RowsFnS16 = @@q8q8_unset_rows_s16
    // Optional native K-quant slots (kq stage 4), all-or-nothing per backend, keyed off mm_kq
    // (kernel_backend_has_kq): once the loader repacks kq planes via repack_kq, EVERY kq matmul
    // must run these cores — the portable disk-order kernels would read garbage off the interleaved planes.
    mm_kq : MatmulKqFn = @@kq_unset_kernel
    kq_rows_k4 : MatmulKqRowsFn = @@kq_unset_rows
    kq_rows_k5 : MatmulKqRowsFn = @@kq_unset_rows
    kq_rows_k6 : MatmulKqRowsFn = @@kq_unset_rows
    kq_rows_q40 : MatmulKqRowsFn = @@kq_unset_rows
    kq_batch : MatmulKqBatchFn = @@kq_unset_batch
    kq_groupn : MatmulKqGroupNFn = @@kq_unset_groupn
    kq_batch_groupn : MatmulKqBatchGroupNFn = @@kq_unset_batch_groupn
    repack_kq : RepackKqFn = @@repack_kq_identity
    needs_repack : bool
    // mx4_expand_interleaved: true when repack_mx4/repack Q8 layouts satisfy the byte-for-byte
    // interleave map (expand_mx4_region_q8's direct branch) — nibble byte `off` of group bg maps
    // positionally onto Q8 bytes. False on a row-major mx4 backend: prefill expands + repacks via `repack`.
    mx4_expand_interleaved : bool = false
    // mx4_native_batch: the backend's mx4_batch beats expand-to-Q8 + Q8 batch (x64: the expansion's
    // 3·d·n Q8 scratch round-trip through DRAM was 87% of gpt-oss prefill wall), so grouped MoE
    // prefill dispatches matmul_mx4q8_batch instead. Slot may be set without the flag (probe/A-B ref).
    mx4_native_batch : bool = false
    priority : int
    // see BackendAvailableFn — selection-time availability; default = always selectable
    available : BackendAvailableFn = @@backend_always_available
    // The Q8 interleave (mr) the backend's repack writes and its kernels read. Evaluated at
    // ACTIVATION time, never [init] (a JIT-stamped layout still answers with its reference tier then).
    // The mx4 expand's positional map and probes read the cached value via active_q8_layout_mr().
    q8_layout : function<() : int> = @@q8_layout_grp4
    // per-FORMAT kq plane interleave (kq v2: the k4/k5/k6 families tune separately, so their
    // grp layouts can differ from the q8 family's and from each other)
    kq_layout : function<(fmt : int) : int> = @@kq_layout_grp4_fmt
    // The additive bias the backend's repack bakes into GROUP-row weight bytes (128 for a
    // bias128 gen stamp, 0 everywhere else). Same activation-time contract as q8_layout;
    // the mx4->Q8 expand reads the cached value via active_q8_wbias().
    q8_wbias : function<() : int> = @@q8_wbias_zero
    // K-bytes per row per interleave group of the backend's Q8 plane (4 everywhere; 8 on an
    // smmla gen stamp — the row-pair × 8-k layout). Same activation-time contract; the mx4->Q8
    // expand map and the token-tail generic read the cached value via active_q8_kgroup().
    q8_kgroup : function<() : int> = @@q8_kgroup_four
}

// Active kernels — the hot path reads these directly. A backend must register before any Q8·Q8 matmul
// runs (require dasllama/dasllama_math_default for the portable fallback); until then the stubs panic.
var g_mm_q8q8 = @@q8q8_unset_kernel
var g_mm_q8q8_batch = @@q8q8_unset_batch
var g_mm_q8q8_group3 = @@q8q8_unset_group3
var g_mm_mx4q8 = @@mx4q8_unset_kernel
var g_mm_mx4q8_batch = @@mx4q8_unset_batch
var g_mm_mx4q8_batch_groupn = @@mx4q8_unset_batch_groupn
var g_mm_q8q8_groupn = @@q8q8_unset_groupn
var g_mm_mx4q8_groupn = @@mx4q8_unset_groupn
var g_mm_q8q8_rows = @@q8q8_unset_rows
var g_active_has_rows = false       // does the active backend carry a row-range GEMV core?
var g_mm_q8q8_s16 = @@q8q8_unset_kernel_s16
var g_mm_q8q8_batch_s16 = @@q8q8_unset_batch_s16
var g_mm_q8q8_group3_s16 = @@q8q8_unset_group3_s16
var g_mm_q8q8_groupn_s16 = @@q8q8_unset_groupn_s16
var g_mm_q8q8_rows_s16 = @@q8q8_unset_rows_s16
var g_active_has_wscale16 = false   // does the active backend carry the f16-scale twins?
var g_active_has_rows16 = false     // ... including the f16-scale row-range GEMV core?
var g_mm_kq = @@kq_unset_kernel
var g_kq_rows_k4 = @@kq_unset_rows
var g_kq_rows_k5 = @@kq_unset_rows
var g_kq_rows_k6 = @@kq_unset_rows
var g_kq_rows_q40 = @@kq_unset_rows
var g_mm_kq_batch = @@kq_unset_batch
var g_mm_kq_groupn = @@kq_unset_groupn
var g_mm_kq_batch_groupn = @@kq_unset_batch_groupn
var g_active_has_kq = false         // does the active backend carry the native K-quant slots?
var g_active_has_kq_batch = false   // ... including the batched kq GEMM (the prefill tile)?
var g_active_has_kq_groupn = false  // ... and the region-list kq GEMV (MoE expert dispatch)?
var g_active_has_kq_batch_groupn = false   // ... and the fused batched region-list GEMM (MoE prefill)?
var g_repack_q8q8 = @@repack_q8q8_identity   // no-op until a repack backend is selected
var g_repack_mx4 = @@repack_mx4_identity     // no-op until a repack backend is selected
var g_repack_kq = @@repack_kq_identity       // no-op until a kq-capable backend is selected

// ===== MoE GPU tier (per-layer expert offload) =====
// A GPU tier (dasllama_math_vulkan) installs FFN/attention/dn overrides + a weight-stack upload
// hook; hooks live HERE (not dasllama_common) so the engine never names a GPU symbol directly.
typedef MoeGpuFfnFn = function<(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n : int64; nfe : int64; nrows : int64; npos : int64; f1 : int; f3 : int; f2 : int; is_gelu : bool) : void>
typedef MoeGpuUploadFn = function<(wq : uint8 const?; ws : uint8 const?; woff : int64; n : int64; rows : int64; fmt : int; stream : bool) : bool>
typedef MoeGpuClsFn = function<(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n : int64; d : int64; fmt : int) : void>
typedef MoeGpuDenseFn = function<(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n : int64; d : int64; nrows : int64; fmt : int) : void>
typedef MoeGpuDropFn = function<(n : int; stream : bool) : void>
// The whole deltanet attention block of one recurrent layer as ONE device chain: qkv + z-gate
// GEMMs (Q8_0, always q8 plane), depthwise conv + silu + q/k L2-norm, chunked delta-rule scan,
// gated out-norm, out projection. histp feeds conv history channel-major; zeros = dn_reset's fresh state.
typedef MoeGpuDnFn = function<(var yp : float?; woq : int64; woz : int64; woo : int64;
                               xqp : int8 const?; xsp : float const?; bp : float const?; gp : float const?;
                               convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                               var stp : float?; var tailp : float?; histp : float const?;
                               npos : int64; dim : int64; cd : int64; kd : int64; di : int64;
                               nvh : int64; nkh : int64; ds : int64; dconv : int64; eps : float) : void>
// The whole softmax attention block of one full-attention layer as ONE device chain per window
// (qkv GEMMs, qk-RMSNorm, partial NEOX RoPE, flash-style causal GQA attention, out-gate, wo proj).
// Start-of-sequence ONLY (start_pos == 0) — continuation windows fall back to the CPU path (fail-closed).
typedef MoeGpuAttnFn = function<(var yp : float?; var kp : float?; var vp : float?;
                                 woq : int64; wok : int64; wov : int64; woo : int64;
                                 fq : int; fk : int; fv : int; fo : int;
                                 xqp : int8 const?; xsp : float const?;
                                 rmsqp : float const?; rmskp : float const?;
                                 cosp : float const?; sinp : float const?;
                                 npos : int64; dim : int64; qd : int64; kv_dim : int64;
                                 hs : int64; rot : int64; n_heads : int64; kv_mul : int64;
                                 scale : float; eps : float; gated : bool; qknorm : bool) : void>

[unused_argument(goutp, offs1, offs3, offs2, nregions, xqp, xsp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)]
def private moe_gpu_unset_ffn(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    panic("dasLLAMA: MoE GPU FFN dispatch hit without an installed GPU tier")
}

[unused_argument(wq, ws, woff, n, rows, fmt, stream)]
def private moe_gpu_unset_upload(wq : uint8 const?; ws : uint8 const?; woff : int64; n : int64; rows : int64; fmt : int; stream : bool) : bool {
    return false
}

[unused_argument(yp, woff, xqp, xsp, n, d, fmt)]
def private moe_gpu_unset_cls(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d : int64; fmt : int) {
    panic("dasLLAMA: GPU classifier dispatch hit without an installed GPU tier")
}

[unused_argument(yp, woff, xqp, xsp, n, d, nrows, fmt)]
def private moe_gpu_unset_dense(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d, nrows : int64; fmt : int) {
    panic("dasLLAMA: GPU dense GEMM dispatch hit without an installed GPU tier")
}

[unused_argument(n, stream)]
def private moe_gpu_unset_drop(n : int; stream : bool) {
    panic("dasLLAMA: GPU stack drop hit without an installed GPU tier")
}

[unused_argument(yp, woq, woz, woo, xqp, xsp, bp, gp, convw, wnormp, ap, dtp, stp, tailp, histp, npos, dim, cd, kd, di, nvh, nkh, ds, dconv, eps)]
def private moe_gpu_unset_dn(var yp : float?; woq, woz, woo : int64;
                             xqp : int8 const?; xsp : float const?; bp : float const?; gp : float const?;
                             convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                             var stp : float?; var tailp : float?; histp : float const?;
                             npos, dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    panic("dasLLAMA: GPU deltanet dispatch hit without an installed GPU tier")
}

[unused_argument(yp, kp, vp, woq, wok, wov, woo, fq, fk, fv, fo, xqp, xsp, rmsqp, rmskp, cosp, sinp, npos, dim, qd, kv_dim, hs, rot, n_heads, kv_mul, scale, eps, gated, qknorm)]
def private moe_gpu_unset_attn(var yp : float?; var kp : float?; var vp : float?;
                               woq, wok, wov, woo : int64; fq, fk, fv, fo : int;
                               xqp : int8 const?; xsp : float const?;
                               rmsqp : float const?; rmskp : float const?;
                               cosp : float const?; sinp : float const?;
                               npos, dim, qd, kv_dim, hs, rot, n_heads, kv_mul : int64;
                               scale : float; eps : float; gated : bool; qknorm : bool) {
    panic("dasLLAMA: GPU attention dispatch hit without an installed GPU tier")
}

var g_moe_gpu_ffn = @@moe_gpu_unset_ffn
var g_moe_gpu_upload = @@moe_gpu_unset_upload
var g_moe_gpu_cls = @@moe_gpu_unset_cls
var g_moe_gpu_dense = @@moe_gpu_unset_dense
var g_moe_gpu_drop = @@moe_gpu_unset_drop
var g_moe_gpu_dn = @@moe_gpu_unset_dn
var g_moe_gpu_attn = @@moe_gpu_unset_attn
var g_moe_gpu_installed = false
var g_moe_gpu_layer_count = 0l         // K = layers offloaded from the END (env-driven, set by the tier)
var g_moe_gpu_from_layer = 1000000l    // first offloaded layer (n_layers - K, set by the loader)
var g_moe_gpu_cls_off = -1l            // element offset of the resident classifier plane (loader contract)
var g_moe_gpu_stream_from = 1000000l   // first STREAMED layer (streamed range = [stream_from, from_layer))
var g_moe_gpu_dense_offs : table<int64>   // resident dense planes, keyed woff * 8 + fmt (per-format offset spaces)
var g_moe_gpu_dense_route = true       // runtime routing gate over the marks (in-process A/B lever)
var g_moe_gpu_dn_layers : table<int64>    // recurrent layers whose deltanet triple is tier-resident
var g_moe_gpu_dn_route = true          // runtime routing gate over the deltanet marks (--dn-ab lever)
var g_moe_gpu_attn_layers : table<int64>  // full-attention layers whose q/k/v/o quad is tier-resident
var g_moe_gpu_attn_route = true        // runtime routing gate over the attention marks (--attn-ab lever)

//! Install the GPU MoE tier (a GPU backend calls this at [init] when its device probe passes).
def public install_moe_gpu_tier(ffn : MoeGpuFfnFn; upload : MoeGpuUploadFn; cls : MoeGpuClsFn; dense : MoeGpuDenseFn; drop : MoeGpuDropFn; dn : MoeGpuDnFn; attn : MoeGpuAttnFn) {
    g_moe_gpu_ffn = ffn
    g_moe_gpu_upload = upload
    g_moe_gpu_cls = cls
    g_moe_gpu_dense = dense
    g_moe_gpu_drop = drop
    g_moe_gpu_dn = dn
    g_moe_gpu_attn = attn
    g_moe_gpu_installed = true
}

def public set_moe_gpu_layer_count(k : int64) {
    g_moe_gpu_layer_count = k
}

def public moe_gpu_layer_count : int64 {
    return g_moe_gpu_installed ? g_moe_gpu_layer_count : 0l
}

//! Loader contract: called once per load after the resident set is known (layers - K .. layers).
def public set_moe_gpu_from_layer(from_layer : int64) {
    g_moe_gpu_from_layer = from_layer
}

def public moe_gpu_layer_on_gpu(l : int64) : bool {
    return g_moe_gpu_installed && l >= g_moe_gpu_from_layer
}

//! Loader contract: first streamed layer (streamed range = [from, the resident set); reset per
//! load). Streamed layers keep their CPU planes — only PREFILL batches dispatch on the GPU,
//! the expert stacks DMA-streamed from pinned host mirrors; decode stays on the CPU.
def public set_moe_gpu_stream_from(from_layer : int64) {
    g_moe_gpu_stream_from = from_layer
}

def public moe_gpu_layer_streamed(l : int64) : bool {
    return g_moe_gpu_installed && l >= g_moe_gpu_stream_from && l < g_moe_gpu_from_layer
}

//! Loader contract: the resident classifier plane's element offset (-1 = none; reset per load).
def public set_moe_gpu_cls_off(woff : int64) {
    g_moe_gpu_cls_off = woff
}

def public moe_gpu_cls_on_gpu(woff : int64) : bool {
    return g_moe_gpu_installed && woff == g_moe_gpu_cls_off
}

//! Loader contract: mark one dense (attention-side) plane resident on the tier. Keyed by
//! (fmt, element offset) — the same identity the tier resolves stacks by, so the batch GEMM
//! funnels route any marked weight without knowing layers or arches. Reset per load.
def public set_moe_gpu_dense_off(fmt : int; woff : int64) {
    g_moe_gpu_dense_offs |> insert(woff * 8l + int64(fmt))
}

def public reset_moe_gpu_dense_offs {
    g_moe_gpu_dense_offs |> clear()
}

def public moe_gpu_dense_on_gpu(fmt : int; woff : int64) : bool {
    return g_moe_gpu_installed && g_moe_gpu_dense_route && key_exists(g_moe_gpu_dense_offs, woff * 8l + int64(fmt))
}

//! Runtime routing gate over the dense marks: uploads stay resident, only the funnels' routing
//! flips. The in-process A/B lever — both arms share one process, one residency, one budget
//! knee, so the measured delta is the dense rail alone (cross-run pp drift measured ±12).
def public set_moe_gpu_dense_route(on : bool) {
    g_moe_gpu_dense_route = on
}

//! Loader contract: mark one recurrent layer's deltanet triple (qkv / z-gate / out-proj planes)
//! resident on the tier — the prefill deltanet block then runs as one device chain. Reset per load.
def public set_moe_gpu_dn_layer(l : int64) {
    g_moe_gpu_dn_layers |> insert(l)
}

def public reset_moe_gpu_dn_layers {
    g_moe_gpu_dn_layers |> clear()
}

def public moe_gpu_dn_on_gpu(l : int64) : bool {
    return g_moe_gpu_installed && g_moe_gpu_dn_route && key_exists(g_moe_gpu_dn_layers, l)
}

//! Runtime routing gate over the deltanet marks (the --dn-ab in-process lever; uploads stay).
def public set_moe_gpu_dn_route(on : bool) {
    g_moe_gpu_dn_route = on
}

//! Loader contract: mark one full-attention layer's q/k/v/o plane quad resident on the tier —
//! the prefill attention block then runs as one device chain. Reset per load.
def public set_moe_gpu_attn_layer(l : int64) {
    g_moe_gpu_attn_layers |> insert(l)
}

def public reset_moe_gpu_attn_layers {
    g_moe_gpu_attn_layers |> clear()
}

def public moe_gpu_attn_on_gpu(l : int64) : bool {
    return g_moe_gpu_installed && g_moe_gpu_attn_route && key_exists(g_moe_gpu_attn_layers, l)
}

//! Runtime routing gate over the attention marks (the --attn-ab in-process lever; uploads stay).
def public set_moe_gpu_attn_route(on : bool) {
    g_moe_gpu_attn_route = on
}

//! Upload one weight stack ([rows x n] at woff) as the GPU tier's row-major planes — q8 (fmt 0):
//! int8 quants + f16 scales; native K-quant (k4/k5/k6): superblock payloads + decoded scale rows.
//! stream = pinned host mirror for DMA streaming vs resident VRAM; false = tier absent/budget hit.
def public moe_gpu_upload_stack(wq : array<uint8> | #; ws : array<uint8> | #; woff : int64; n, rows : int64; fmt : int; stream : bool = false) : bool {
    if (!g_moe_gpu_installed) {
        return false
    }
    unsafe {
        return invoke(g_moe_gpu_upload, addr<uint8 const?>(wq[0]), addr<uint8 const?>(ws[0]), woff, n, rows, fmt, stream)
    }
}

//! Fused MoE FFN on the GPU tier — the whole gate+up -> act -> requant -> down chain of the
//! routed experts in one device-side dispatch (decode and prefill alike; the tier chunks
//! internally). gout is nrows x n floats; xq/xs is the gathered activation in the layer's own quant form.
def public matmul_moe_gpu_ffn(var gout : array<float>; offs1 : array<int64>; offs3 : array<int64>; offs2 : array<int64>;
                              nregions : int64; xq : array<int8>; xs : array<float>; n, nfe, nrows : int64;
                              f1, f3, f2 : int; is_gelu : bool) {
    unsafe {
        invoke(g_moe_gpu_ffn, addr(gout[0]),
            addr<int64 const?>(offs1[0]), addr<int64 const?>(offs3[0]), addr<int64 const?>(offs2[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), null, null, n, nfe, nrows, 0l, f1, f3, f2, is_gelu)
    }
}

//! The combined prefill form: the routed weighted-sum reduce ALSO runs on the device, so gout
//! returns npos x n combined rows instead of nrows x n bucket rows (down rows never cross PCIe —
//! readback shrinks k-fold). w is the routing weight plane (npos x k), inv the grid->bucket row map.
def public matmul_moe_gpu_ffn_combined(var gout : array<float>; offs1 : array<int64>; offs3 : array<int64>; offs2 : array<int64>;
                                       nregions : int64; xq : array<int8>; xs : array<float>; w : array<float>; inv : array<uint>;
                                       n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    unsafe {
        invoke(g_moe_gpu_ffn, addr(gout[0]),
            addr<int64 const?>(offs1[0]), addr<int64 const?>(offs3[0]), addr<int64 const?>(offs2[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), addr<float const?>(w[0]), addr<uint const?>(inv[0]),
            n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    }
}

//! Dense batched GEMM on the GPU tier — y [nrows x d] = X·Wᵀ over the resident plane ([d x n]
//! at woff). xq/xs is the row-major pre-quantized activation image in the plane's own quant form
//! (q8_0 per-32 for q8, Q8_K per-256 for kq); per-16 block sums are recomputed in-kernel.
def public matmul_gpu_dense(var y : array<float>; woff : int64; xq : array<int8>; xs : array<float>; n, d, nrows : int64; fmt : int) {
    unsafe {
        invoke(g_moe_gpu_dense, addr(y[0]), woff, addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, nrows, fmt)
    }
}

//! One recurrent layer's whole deltanet attention block on the GPU tier — see MoeGpuDnFn for
//! the contract. y gets npos x dim out-proj rows, state/tail update in place; beta/alpha arrive
//! RAW (transforms run in-kernel), conv taps in the fblob disk layout (cd x dconv).
def public matmul_moe_gpu_dn(var y : array<float>; woq, woz, woo : int64;
                             xq : array<int8>; xs : array<float>; b : array<float>; g : array<float>;
                             convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                             var stp : float?; var tail : array<float>; histp : float const?;
                             npos, dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    unsafe {
        invoke(g_moe_gpu_dn, addr(y[0]), woq, woz, woo,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), addr<float const?>(b[0]), addr<float const?>(g[0]),
            convw, wnormp, ap, dtp, stp, addr(tail[0]), histp,
            npos, dim, cd, kd, di, nvh, nkh, ds, dconv, eps)
    }
}

//! One full-attention layer's whole softmax attention block on the GPU tier — see MoeGpuAttnFn
//! for the contract. y gets npos x dim wo rows; k/v get npos x kv_dim roped-k / raw-v rows.
//! cos/sin are npos x rot/2 NEOX rope rows; rms weight rows may be null when qknorm is off.
def public matmul_moe_gpu_attn(var y : array<float>; var k : array<float>; var v : array<float>;
                               woq, wok, wov, woo : int64; fq, fk, fv, fo : int;
                               xq : array<int8>; xs : array<float>;
                               rmsqp : float const?; rmskp : float const?;
                               cosp : float const?; sinp : float const?;
                               npos, dim, qd, kv_dim, hs, rot, n_heads, kv_mul : int64;
                               scale : float; eps : float; gated : bool; qknorm : bool) {
    unsafe {
        invoke(g_moe_gpu_attn, addr(y[0]), addr(k[0]), addr(v[0]),
            woq, wok, wov, woo, fq, fk, fv, fo,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            rmsqp, rmskp, cosp, sinp,
            npos, dim, qd, kv_dim, hs, rot, n_heads, kv_mul, scale, eps, gated, qknorm)
    }
}

//! Roll back the last n weight-stack uploads (a partial layer triple the VRAM budget cut
//! mid-way) — a dangling plane would hijack the residency check when its layer re-enters as
//! streamed or CPU. stream selects the pinned-mirror registry instead of the resident one.
def public moe_gpu_drop_stacks(n : int; stream : bool) {
    invoke(g_moe_gpu_drop, n, stream)
}

//! Classifier GEMV on the GPU tier — y[d] = W·x over the resident plane at woff (one region
//! covering the whole vocab). xq/xs is the activation vector in the plane's own quant form (q8_0
//! per-32 for q8, Q8_K per-256 for kq — the tier resolves which from the uploaded stack format).
def public matmul_moe_gpu_cls(var y : array<float>; woff : int64; xq : array<int8>; xs : array<float>; n, d : int64; fmt : int) {
    unsafe {
        invoke(g_moe_gpu_cls, addr(y[0]), woff, addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, fmt)
    }
}

var g_kernel_backends : table<string; KernelBackend>
var g_active_backend = ""           // name of the active backend ("" = the unset stub)
var g_active_priority = -1          // priority of the active row-major default (auto-activate compare)
var g_active_needs_repack = false   // does the active backend need repacked weights?
var g_active_mx4_expand_interleaved = false   // see KernelBackend.mx4_expand_interleaved
var g_active_mx4_native_batch = false         // see KernelBackend.mx4_native_batch
var g_active_q8_layout_mr = 4l                // see KernelBackend.q8_layout (activation-time cache)
var g_active_kq_layout = @@kq_layout_grp4_fmt // see KernelBackend.kq_layout (activation-time cache)
var g_active_q8_wbias = 0l                    // see KernelBackend.q8_wbias (activation-time cache)
var g_active_q8_kgroup = 4l                   // see KernelBackend.q8_kgroup (activation-time cache)
var g_backend_pin = ""              // A/B: force this backend by name; "" = auto-select
var g_batch_pin = ""                // hybrid: donor backend for the BATCH-shaped slots; "" = whole-backend
var g_active_batch_backend = ""     // == g_active_backend unless a batch pin overrode the batch slots

//! Default repack: no-op — weights stay row-major for the portable / sdot4x4 kernels.
[unused_argument(wp, sp, n, d)]
def repack_q8q8_identity(var wp : int8?; var sp : float?; n, d : int64) {}

//! Default MXFP4 repack: no-op — the planes stay row-major for the portable / row-major kernels.
[unused_argument(np, ep, n, d)]
def repack_mx4_identity(var np : uint8?; var ep : uint8?; n, d : int64) {}

//! Default K-quant repack: no-op — the kq planes stay disk-order for the portable kernels.
[unused_argument(fmt, kq, ks, n, d)]
def repack_kq_identity(fmt : int; var kq : uint8?; var ks : uint8?; n, d : int64) {}

def private no_backend() {
    panic("dasLLAMA: no Q8·Q8 kernel backend registered — require dasllama/dasllama_math_default")
}
def private backend_always_available() : bool => true
def private q8_layout_grp4() : int => 4

[unused_argument(fmt)]
def private kq_layout_grp4_fmt(fmt : int) : int => 4
def private q8_wbias_zero() : int => 0
def private q8_kgroup_four() : int => 4

//! The Q8 interleave (mr) of the ACTIVE backend's weight layout — cached at activation from
//! KernelBackend.q8_layout (4 for every hand tier; the stamped mr on the gen backend).
def active_q8_layout_mr() : int64 => g_active_q8_layout_mr

//! The active backend's kq plane interleave for `fmt` (4/5/6) — per-format since the kq v2
//! family split; the loader freezes these onto the Model at repack time.
def active_kq_layout_mr(fmt : int) : int64 => int64(invoke(g_active_kq_layout, fmt))

//! The weight-plane bias of the ACTIVE backend's Q8 layout — cached at activation from
//! KernelBackend.q8_wbias (0 for every hand tier; 128 on a bias128 gen stamp). The mx4->Q8
//! expand biases its LUT writes to match.
def active_q8_wbias() : int64 => g_active_q8_wbias

//! The plane kgroup of the ACTIVE backend's Q8 layout — cached at activation from
//! KernelBackend.q8_kgroup (4 for every hand tier; 8 on an smmla gen stamp). The mx4->Q8
//! expand's positional map and the batch token-tail generic read it.
def active_q8_kgroup() : int64 => g_active_q8_kgroup
[unused_argument(yp, wp, sp, xqp, xsp, n, d)]
def private q8q8_unset_kernel(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) { no_backend() }
[unused_argument(yp, wp, sp, xqp, xsp, n, d, ntok)]
def private q8q8_unset_batch(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) { no_backend() }
[unused_argument(y0p, y1p, y2p, wp, sp, woff0, woff1, woff2, xqp, xsp, n, d0, d1, d2)]
def private q8q8_unset_group3(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?; woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) { no_backend() }
[unused_argument(yp, wn, we, xqp, xsp, n, d)]
def private mx4q8_unset_kernel(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, d : int64) { no_backend() }
[unused_argument(yp, wn, we, xqp, xsp, n, d, ntok)]
def private mx4q8_unset_batch(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    // reachable with a registered backend (slot is optional — see KernelBackend.mx4_batch)
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' provides no MXFP4·Q8 batch kernel (mx4_batch)")
}
[unused_argument(yp, wn, we, offs, nregions, xqp, xsp, bp, boffs, n, d)]
def private mx4q8_unset_batch_groupn(var yp : float?; wn : uint8 const?; we : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' provides no MXFP4·Q8 batch groupN kernel (mx4_batch_groupn)")
}
[unused_argument(yp, wp, sp, offs, nregions, xqp, xsp, bp, boffs, n, d)]
def private q8q8_unset_groupn(var yp : float?; wp : int8 const?; sp : float const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) { no_backend() }
[unused_argument(yp, wp, sp, xqp, xsp, n, rb, re)]
def private q8q8_unset_rows(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) {
    // reachable with a registered backend: the slot is optional — gate on kernel_backend_has_rows()
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' provides no row-range GEMV core (mm_rows)")
}
[unused_argument(yp, wn, we, offs, nregions, xqp, xsp, bp, boffs, n, d)]
def private mx4q8_unset_groupn(var yp : float?; wn : uint8 const?; we : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) { no_backend() }
// The f16-scale stubs are reachable with a registered backend (the twins are optional) — a
// wscale_f16 model must never activate a backend without them; the loader gates the convert.
[unused_argument(yp, wp, sp, xqp, xsp, n, d)]
def private q8q8_unset_kernel_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no f16-scale kernels (mm_s16)")
}
[unused_argument(yp, wp, sp, xqp, xsp, n, d, ntok)]
def private q8q8_unset_batch_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no f16-scale batch kernel (batch_s16)")
}
[unused_argument(y0p, y1p, y2p, wp, sp, woff0, woff1, woff2, xqp, xsp, n, d0, d1, d2)]
def private q8q8_unset_group3_s16(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : uint16 const?; woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no f16-scale group3 kernel (group3_s16)")
}
[unused_argument(yp, wp, sp, offs, nregions, xqp, xsp, bp, boffs, n, d)]
def private q8q8_unset_groupn_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no f16-scale groupN kernel (groupn_s16)")
}
[unused_argument(yp, wp, sp, xqp, xsp, n, rb, re)]
def private q8q8_unset_rows_s16(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no f16-scale row-range GEMV core (mm_rows_s16)")
}
// The kq stubs are reachable with a registered backend (the slots are optional) — dispatch
// gates on kernel_backend_has_kq(); when it is down the disk-order portable kernels serve.
[unused_argument(fmt, yp, kqp, ksp, xqp, xsp, xbsp, n, d)]
def private kq_unset_kernel(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no native K-quant kernels (mm_kq)")
}
[unused_argument(yp, kqp, ksp, xqp, xsp, xbsp, n, rb, re)]
def private kq_unset_rows(var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no K-quant row-range GEMV cores (kq_rows_*)")
}
[unused_argument(fmt, yp, kqp, ksp, offs, nregions, xqp, xsp, xbsp, n, d)]
def private kq_unset_groupn(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no K-quant region-list kernel (kq_groupn)")
}
[unused_argument(fmt, yp, kqp, ksp, offs, nregions, xqp, xsp, xbsp, n, d)]
def private kq_unset_batch_groupn(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no K-quant batched region-list kernel (kq_batch_groupn)")
}
[unused_argument(fmt, yp, kqp, ksp, xqp, xsp, xbsp, n, d, ntok)]
def private kq_unset_batch(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, ntok : int64) {
    panic("dasLLAMA: the active kernel backend '{g_active_backend}' has no K-quant batch kernel (kq_batch)")
}

def private activate(be : KernelBackend) {
    g_mm_q8q8 = be.mm
    g_mm_q8q8_batch = be.batch
    g_mm_q8q8_group3 = be.group3
    g_mm_mx4q8 = be.mm_mx4
    g_mm_mx4q8_batch = be.mx4_batch
    g_mm_mx4q8_batch_groupn = be.mx4_batch_groupn
    g_mm_q8q8_groupn = be.groupn
    g_mm_mx4q8_groupn = be.groupn_mx4
    g_mm_q8q8_rows = be.mm_rows
    g_active_has_rows = !(be.mm_rows == @@q8q8_unset_rows)
    g_mm_q8q8_s16 = be.mm_s16
    g_mm_q8q8_batch_s16 = be.batch_s16
    g_mm_q8q8_group3_s16 = be.group3_s16
    g_mm_q8q8_groupn_s16 = be.groupn_s16
    g_mm_q8q8_rows_s16 = be.mm_rows_s16
    g_active_has_wscale16 = !(be.mm_s16 == @@q8q8_unset_kernel_s16)
    g_active_has_rows16 = !(be.mm_rows_s16 == @@q8q8_unset_rows_s16)
    g_mm_kq = be.mm_kq
    g_kq_rows_k4 = be.kq_rows_k4
    g_kq_rows_k5 = be.kq_rows_k5
    g_kq_rows_k6 = be.kq_rows_k6
    g_kq_rows_q40 = be.kq_rows_q40
    g_mm_kq_batch = be.kq_batch
    g_mm_kq_groupn = be.kq_groupn
    g_mm_kq_batch_groupn = be.kq_batch_groupn
    g_active_has_kq = !(be.mm_kq == @@kq_unset_kernel)
    g_active_has_kq_batch = !(be.kq_batch == @@kq_unset_batch)
    g_active_has_kq_groupn = !(be.kq_groupn == @@kq_unset_groupn)
    g_active_has_kq_batch_groupn = !(be.kq_batch_groupn == @@kq_unset_batch_groupn)
    g_repack_q8q8 = be.repack
    g_repack_mx4 = be.repack_mx4
    g_repack_kq = be.repack_kq
    g_active_backend = be.name
    g_active_needs_repack = be.needs_repack
    g_active_mx4_expand_interleaved = be.mx4_expand_interleaved
    g_active_mx4_native_batch = be.mx4_native_batch
    g_active_q8_layout_mr = int64(invoke(be.q8_layout))
    g_active_kq_layout = be.kq_layout
    g_active_q8_wbias = int64(invoke(be.q8_wbias))
    g_active_q8_kgroup = int64(invoke(be.q8_kgroup))
    g_active_batch_backend = be.name
    // a standing batch pin survives re-activation (auto-select, load-time select, explicit select)
    if (g_batch_pin != "") {
        apply_batch_override(g_batch_pin)
    }
}

// The batch half of a hybrid: overwrites only BATCH-shaped slots (prefill GEMM + mx4 batch) from
// the donor; GEMV-shaped slots stay with the active backend. Donor must read the same weight layout
// (same needs_repack, and for repack backends the same repack fn — laneq/grp4 are mutually unreadable).
def private apply_batch_override(name : string) {
    if (!key_exists(g_kernel_backends, name)) {
        to_log(LOG_WARNING, "dasLLAMA: batch backend '{name}' — no such backend registered; keeping '{g_active_backend}' batch kernels\n")
        return
    }
    let be = g_kernel_backends[name]
    if (be.needs_repack != g_active_needs_repack || (g_active_needs_repack && !(be.repack == g_repack_q8q8))) {
        to_log(LOG_WARNING, "dasLLAMA: batch backend '{name}' reads a different weight layout than active '{g_active_backend}'; keeping '{g_active_backend}' batch kernels\n")
        return
    }
    g_mm_q8q8_batch = be.batch
    g_mm_mx4q8_batch = be.mx4_batch
    g_mm_mx4q8_batch_groupn = be.mx4_batch_groupn
    g_active_mx4_native_batch = be.mx4_native_batch
    g_active_batch_backend = be.name
}

//! Register a kernel backend under its name. Auto-activates it as the row-major default when it is the
//! highest-priority backend that needs no repack — so direct callers get the best un-repacked kernel with
//! no load step. Repack backends are activated only by select_matmul_backend_for_load (at model load).
def register_kernel_backend(be : KernelBackend) {
    g_kernel_backends[be.name] = be
    if (g_backend_pin == "" && !be.needs_repack && be.priority > g_active_priority) {
        g_active_priority = be.priority
        activate(be)
    }
}

//! Activate a registered backend by name (explicit control / A/B benchmarking). No-op if unknown.
//! Runs the availability witness first — for some backends it doubles as the runtime enable
//! (amx = XTILEDATA arch_prctl); activating past it would SIGILL on the first tile op.
def select_kernel_backend(name : string) {
    if (!key_exists(g_kernel_backends, name)) {
        return
    }
    if (!invoke(g_kernel_backends[name].available)) {
        to_log(LOG_WARNING, "dasLLAMA: select_kernel_backend('{name}') — backend reports itself unavailable in this configuration; keeping '{g_active_backend}'\n")
        return
    }
    activate(g_kernel_backends[name])
}

//! Pin the active backend to `name` (forces it; suppresses auto-select on later registrations). For A/B
//! benchmarking on one binary — e.g. pin "arm64-sdot" to measure the row-major baseline against the gen tier.
def pin_kernel_backend(name : string) {
    // Reject an unknown name rather than sticking an invalid pin: a bad pin would suppress
    // auto-activation AND keep select_matmul_backend_for_load from picking the best (repack) backend,
    // silently degrading to whatever is currently active. Keep the current backend and warn instead.
    if (!key_exists(g_kernel_backends, name)) {
        to_log(LOG_WARNING, "dasLLAMA: pin_kernel_backend('{name}') — no such backend registered; keeping '{g_active_backend}'\n")
        return
    }
    if (!invoke(g_kernel_backends[name].available)) {
        to_log(LOG_WARNING, "dasLLAMA: pin_kernel_backend('{name}') — backend reports itself unavailable in this configuration (stamped layout mismatch?); keeping '{g_active_backend}'\n")
        return
    }
    g_backend_pin = name
    select_kernel_backend(name)
}
def clear_kernel_backend_pin() {
    g_backend_pin = ""
}

//! Pin the BATCH-shaped kernel slots (prefill GEMM + mx4 batch) to backend `name`, keeping the
//! active backend's GEMV slots — the hybrid for boxes where decode/prefill prefer different backends.
//! Survives later reselection; `""` clears it. Layout-incompatible/unknown donor warns and no-ops.
def select_batch_backend(name : string) {
    g_batch_pin = name
    if (name != "") {
        apply_batch_override(name)
    }
}

//! The backend serving the BATCH-shaped slots (== active_kernel_backend() unless a batch pin
//! overrode them — diagnostics / tests).
def active_batch_backend() : string {
    return g_active_batch_backend
}

//! Select the best backend for a model load — INCLUDING repack backends (arm64 laneq). Honors a pin.
//! Returns true if the selected backend needs the loader to repack every weight tensor (via
//! repack_q8q8_weight) into its interleaved layout before any matmul runs.
def select_matmul_backend_for_load() : bool {
    if (g_backend_pin != "") {
        select_kernel_backend(g_backend_pin)
        return g_active_needs_repack
    }
    var best = ""
    var bestpri = -1
    for (be in values(g_kernel_backends)) {
        if (be.priority > bestpri && invoke(be.available)) {
            bestpri = be.priority
            best = be.name
        }
    }
    if (best != "") {
        select_kernel_backend(best)
    }
    return g_active_needs_repack
}

//! The active backend's name (diagnostics / tests). "" until a backend registers.
def active_kernel_backend() : string {
    return g_active_backend
}

//! Does the active backend carry a row-range GEMV core (mm_rows)? The fused multi-stage decode
//! chains gate on this and fall back to the per-op dispatch path when false.
def kernel_backend_has_rows() : bool {
    return g_active_has_rows
}

//! The active backend's row-range GEMV core (see MatmulQ8Q8RowsFn). Read it into a local in the
//! MAIN context and capture that in worker lambdas — same rule as every kernel-pointer read.
def mm_q8q8_rows_fn() : MatmulQ8Q8RowsFn {
    return g_mm_q8q8_rows
}

//! Does the active backend carry the f16-scale kernel twins (the wscale_f16 rail)? The loader's
//! scale-plane convert gates on this — without them the model keeps the f32 plane (logged).
def kernel_backend_has_wscale16() : bool {
    return g_active_has_wscale16
}

//! The active backend's f16-scale row-range GEMV core (see MatmulQ8Q8RowsFnS16) — the chains'
//! twin of mm_q8q8_rows_fn. Same read-into-a-local rule.
def mm_q8q8_rows16_fn() : MatmulQ8Q8RowsFnS16 {
    return g_mm_q8q8_rows_s16
}

//! Does the active backend carry the f16-scale row-range GEMV core? The chains' twin of
//! kernel_backend_has_rows — a wscale_f16 model's fused chains gate on it.
def kernel_backend_has_rows16() : bool {
    return g_active_has_rows16
}

//! Does the active backend carry the native K-quant slots (mm_kq + kq_rows_* + repack_kq)?
//! Decides at LOAD whether the kq planes get repacked; a Model records the answer
//! (kq_repacked) and every later dispatch routes off that record, not this live query.
def kernel_backend_has_kq() : bool {
    return g_active_has_kq
}

//! The active backend's K-quant rows core for `fmt` (4/5/6/40) — the chains' kq twin of
//! mm_q8q8_rows_fn. Same read-into-a-local-then-capture rule. Any other id (kq_fi's q8
//! poison 0 included) gets the panic stub — never a silently-wrong format's core.
def kq_rows_fn(fmt : int) : MatmulKqRowsFn {
    if (fmt == 4) return g_kq_rows_k4
    if (fmt == 5) return g_kq_rows_k5
    if (fmt == 6) return g_kq_rows_k6
    if (fmt == 40) return g_kq_rows_q40
    return @@kq_unset_rows
}

//! Does the active backend carry the batched kq GEMM (kq_batch — the prefill tile)? The
//! batched dispatch gates on it and falls back to serial per-position GEMVs when down.
def kernel_backend_has_kq_batch() : bool {
    return g_active_has_kq_batch
}

//! A/B override for the grouped-MoE-prefill mx4 route: native mx4 batch GEMMs vs expand-to-Q8 +
//! Q8 batch. Safe to flip after load (both routes read the same repacked planes); forcing `true`
//! on a backend without mx4 batch slots hits their panic stubs — benchmark knob only.
def set_mx4_native_batch(on : bool) {
    g_active_mx4_native_batch = on
}

//! Does the active backend carry the region-list kq GEMV (kq_groupn — MoE expert dispatch)?
//! The MoE dispatch gates on it and falls back to per-region GEMVs when down.
def kernel_backend_has_kq_groupn() : bool {
    return g_active_has_kq_groupn
}

//! Does the active backend carry the fused batched region-list kq GEMM (kq_batch_groupn — the
//! fused MoE prefill)? The grouped prefill gates on it and keeps per-expert tiles when down.
def kernel_backend_has_kq_batch_groupn() : bool {
    return g_active_has_kq_batch_groupn
}

//! Batched region-list K-quant GEMM over REPACKED planes (triple offs — see the typedef).
//! Callers guard with kq_repacked && kernel_backend_has_kq_batch_groupn().
def matmul_kq_batch_groupn(fmt : int; var y : array<float> | #; kq : array<uint8> | #; ks : array<uint8> | #; offs : array<int64> | #; nregions : int64; xq : array<int8> | #; xs : array<float> | #; xbs : array<int> | #; n, d : int64) {
    unsafe {
        invoke(g_mm_kq_batch_groupn, fmt, addr(y[0]),
            addr<uint8 const?>(kq[0]), addr<uint8 const?>(ks[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<int const?>(xbs[0]), n, d)
    }
}

//! Region-list K-quant GEMV over REPACKED planes (kq twin of mm_at_q8_groupn): `offs` packs
//! (weight, activation) element-offset pairs, region r's rows land at y[r*d .. r*d+d). Plane bases
//! pass whole; callers guard with kq_repacked && kernel_backend_has_kq_groupn().
def matmul_kq_groupn_active(fmt : int; var y : array<float> | #; kq : array<uint8> | #; ks : array<uint8> | #; offs : array<int64> | #; nregions : int64; xq : array<int8> | #; xs : array<float> | #; xbs : array<int> | #; n, d : int64) {
    unsafe {
        invoke(g_mm_kq_groupn, fmt, addr(y[0]),
            addr<uint8 const?>(kq[0]), addr<uint8 const?>(ks[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<int const?>(xbs[0]), n, d)
    }
}

//! Batched K-quant GEMM over a REPACKED plane pair at plane-local element offset `woff`
//! (blob+offset form): ntok activation rows -> token-major Y [ntok x d]. Bit-for-bit ntok x
//! matmul_kq_active. Callers guard with kq_repacked && kernel_backend_has_kq_batch().
def matmul_kq_batch(fmt : int; var y : array<float> | #; kq : array<uint8> | #; ks : array<uint8> | #; woff : int64; xq : array<int8> | #; xs : array<float> | #; xbs : array<int> | #; n, d, ntok : int64) {
    let sb = woff / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    unsafe {
        invoke(g_mm_kq_batch, fmt, addr(y[0]),
            addr<uint8 const?>(kq[sb * qsb]), addr<uint8 const?>(ks[sb * ssb]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<int const?>(xbs[0]), n, d, ntok)
    }
}

//! Native K-quant GEMV over a REPACKED plane pair at plane-local element offset `woff`
//! (blob+offset form — the repacked twin of dasllama_math_default's matmul_kq). Dispatches
//! to the active backend's mm_kq; callers guard with the model's kq_repacked record.
def matmul_kq_active(fmt : int; var y : array<float> | #; kq : array<uint8> | #; ks : array<uint8> | #; woff : int64; xq : array<int8> | #; xs : array<float> | #; xbs : array<int> | #; n, d : int64; yoff : int64 = 0l) {
    let sb = woff / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    unsafe {
        invoke(g_mm_kq, fmt, addr(y[yoff]),
            addr<uint8 const?>(kq[sb * qsb]), addr<uint8 const?>(ks[sb * ssb]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<int const?>(xbs[0]), n, d)
    }
}

//! Repack one K-quant weight tensor (region at element offset woff in the fmt's planes, shape
//! [d rows x n cols]) into the active backend's kq layout, IN PLACE. No-op unless a kq-capable
//! backend is active. n % 256 == 0 (the ggml K-quant row invariant).
def repack_kq_weight(fmt : int; var kq : array<uint8>; var ks : array<uint8>; woff, n, d : int64) {
    let sb = woff / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    unsafe {
        invoke(g_repack_kq, fmt, addr(kq[sb * qsb]), addr(ks[sb * ssb]), n, d)
    }
}

//! Registered backend names (diagnostics / tests).
def kernel_backend_names() : array<string> {
    return <- [for (k in keys(g_kernel_backends)); k]
}

//! Backend `name`'s f32 batch kernel (a batch-only donor's small-ntok / unsupported-shape
//! fallback rail — the metal backend delegates to "portable" through this). The panic stub
//! when unknown.
def kernel_backend_batch_fn(name : string) : MatmulQ8Q8BatchFn {
    var r = @@q8q8_unset_batch
    g_kernel_backends |> get(name) $(be) {
        r = be.batch
    }
    return r
}

//! Repack one Q8 weight tensor (region at element offset woff in qblob, shape [d rows x n cols]) into
//! the active backend's layout, IN PLACE. No-op unless the laneq backend is enabled. n % 32 == 0.
def repack_q8q8_weight(var qblob : array<int8>; var qscales : array<float>; woff, n, d : int64) {
    unsafe {
        invoke(g_repack_q8q8, addr(qblob[woff]), addr(qscales[woff / 32l]), n, d)
    }
}

//! Repack one native-MXFP4 weight tensor (region at weight-element offset woff in the mxq/mxs
//! planes, shape [d rows x n cols]) into the active backend's layout, IN PLACE. No-op unless a
//! repack backend is enabled. n % 32 == 0.
def repack_mx4_weight(var mxq : array<uint8>; var mxs : array<uint8>; woff, n, d : int64) {
    unsafe {
        invoke(g_repack_mx4, addr(mxq[woff / 2l]), addr(mxs[woff / 32l]), n, d)
    }
}

//! Does backend `name` read an interleaved (repacked) weight layout? false for unknown names.
//! Lets tests / direct callers skip layout-mismatched A/Bs (a repack backend cannot run on
//! row-major buffers).
def kernel_backend_needs_repack(name : string) : bool {
    var r = false
    g_kernel_backends |> get(name) $(be) {
        r = be.needs_repack
    }
    return r
}

//! Q8·Q8 matmul: y = W·x with W as int8 quants `wq` [d x n] + scales `ws` [d x n/32] and the
//! activation PRE-quantized to (xq [n], xs [n/32]) by the caller (it's shared across all d rows, so
//! quantizing once amortizes). Dispatches to the active backend (g_mm_q8q8). n % 32 == 0.
def matmul_q8q8(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8, addr(y[0]),
            addr<int8 const?>(wq[0]), addr<float const?>(ws[0]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d)
    }
}

//! Blob+offset form: the weight region at element offset `woff` in the whole quantized blob
//! (scales at woff/32) — no array_view (see the matmul woff overload).
def matmul_q8q8(var y : array<float>; wq : array<int8>; ws : array<float>; woff : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8, addr(y[0]),
            addr<int8 const?>(wq[woff]), addr<float const?>(ws[woff / 32l]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d)
    }
}

//! Batched Q8·Q8 GEMM for prefill: same weight-stationary nest as matmul_batch over the SDOT kernel.
//! Activations PRE-quantized per token into (xq [ntok x n], xs [ntok x n/32]). Y token-major
//! [ntok x d]. Bit-for-bit ntok× matmul_q8q8. Dispatches to the active backend. n % 32 == 0.
def matmul_q8q8_batch(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; xq : array<int8>; xs : array<float>; n, d, ntok : int64) {
    unsafe {
        invoke(g_mm_q8q8_batch, addr(y[0]),
            addr<int8 const?>(wq[0]), addr<float const?>(ws[0]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, ntok)
    }
}

//! Blob+offset form (weights at woff, scales at woff/32) — no array_view.
def matmul_q8q8_batch(var y : array<float>; wq : array<int8>; ws : array<float>; woff : int64; xq : array<int8>; xs : array<float>; n, d, ntok : int64) {
    unsafe {
        invoke(g_mm_q8q8_batch, addr(y[0]),
            addr<int8 const?>(wq[woff]), addr<float const?>(ws[woff / 32l]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, ntok)
    }
}

//! Fused Q8·Q8 for three independent row-groups sharing one activation (attention Q/K/V): one
//! parallel_for over combined d0+d1+d2 rows so idle workers fill in on small K/V rows instead of
//! running single-thread (~23% faster than sequential Q-parallel + K,V-single on real shapes).
def matmul_q8q8_group3(var y0 : array<float>; var y1 : array<float>; var y2 : array<float>;
                       wq : array<int8>; ws : array<float>; woff0, woff1, woff2 : int64;
                       xq : array<int8>; xs : array<float>; n, d0, d1, d2 : int64) {
    unsafe {
        invoke(g_mm_q8q8_group3, addr(y0[0]), addr(y1[0]), addr(y2[0]),
            addr<int8 const?>(wq[0]), addr<float const?>(ws[0]),
            woff0, woff1, woff2,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d0, d1, d2)
    }
}

// ===== f16-weight-scale overloads (the wscale_f16 rail) — same contracts, ws is the raw
// binary16 plane (Model.qscales16); overload resolution keys on the scale array type =====

//! Blob+offset form over the f16 scale plane — dispatches to the backend's mm_s16 twin.
def matmul_q8q8(var y : array<float>; wq : array<int8>; ws : array<uint16>; woff : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8_s16, addr(y[0]),
            addr<int8 const?>(wq[woff]), addr<uint16 const?>(ws[woff / 32l]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d)
    }
}

//! Blob+offset batch form over the f16 scale plane (batch_s16).
def matmul_q8q8_batch(var y : array<float>; wq : array<int8>; ws : array<uint16>; woff : int64; xq : array<int8>; xs : array<float>; n, d, ntok : int64) {
    unsafe {
        invoke(g_mm_q8q8_batch_s16, addr(y[0]),
            addr<int8 const?>(wq[woff]), addr<uint16 const?>(ws[woff / 32l]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, ntok)
    }
}

//! Q/K/V group form over the f16 scale plane (group3_s16).
def matmul_q8q8_group3(var y0 : array<float>; var y1 : array<float>; var y2 : array<float>;
                       wq : array<int8>; ws : array<uint16>; woff0, woff1, woff2 : int64;
                       xq : array<int8>; xs : array<float>; n, d0, d1, d2 : int64) {
    unsafe {
        invoke(g_mm_q8q8_group3_s16, addr(y0[0]), addr(y1[0]), addr(y2[0]),
            addr<int8 const?>(wq[0]), addr<uint16 const?>(ws[0]),
            woff0, woff1, woff2,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d0, d1, d2)
    }
}

//! Region-list form over the f16 scale plane (groupn_s16).
def matmul_q8q8_groupn(var y : array<float>; wq : array<int8> | #; ws : array<uint16> | #; offs : array<int64>; nregions : int64;
                       xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8_groupn_s16, addr(y[0]),
            addr<int8 const?>(wq[0]), addr<uint16 const?>(ws[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            null, null, n, d)
    }
}

//! Biased region-list form over the f16 scale plane (groupn_s16).
def matmul_q8q8_groupn(var y : array<float>; wq : array<int8> | #; ws : array<uint16> | #; offs : array<int64>; nregions : int64;
                       xq : array<int8>; xs : array<float>; bias : array<float> | #; boffs : array<int64>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8_groupn_s16, addr(y[0]),
            addr<int8 const?>(wq[0]), addr<uint16 const?>(ws[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<float const?>(bias[0]), addr<int64 const?>(boffs[0]), n, d)
    }
}

// ===== MXFP4·Q8: native 4-bit weights (MoE expert stacks) =====
// 32-elem block: 16 packed nibble bytes (low=elem j, high=j+16) + one E8M0 scale byte, in two planes
// (16-byte-regular for TBL); nibbles index the DOUBLED e2m1 table, e8m0_half compensates the doubling.

//! E8M0 scale byte -> fp32 * 0.5 = 2^(e-128), pairing with the DOUBLED e2m1 nibble values in
//! mxfp4_fill_lut. e < 2 lands in fp32-denormal territory -> precomputed bit patterns (mirrors
//! ggml_e8m0_to_fp32_half).
def e8m0_half(e : uint) : float {
    let bits = e < 2u ? (0x00200000u << e) : ((e - 1u) << 23u)
    return unsafe(reinterpret<float>(bits))
}

//! Fill a 16-byte LUT with the MXFP4 e2m1 magnitudes DOUBLED (nibble -> ±{0,.5,1,1.5,2,3,4,6}×2,
//! OCP MX spec via ggml kvalues_mxfp4) as int8 — the table `tbl16_lo`/`tbl16_hi` and the scalar
//! kernels index; the halved E8M0 scale (e8m0_half) compensates.
def mxfp4_fill_lut(var lut : int8[16]) {
    let kv = fixed_array(0, 1, 2, 3, 4, 6, 8, 12, 0, -1, -2, -3, -4, -6, -8, -12)
    for (i in range(16)) {
        lut[i] = int8(kv[i])
    }
}

//! MXFP4·Q8 GEMV: y = W·x with W native 4-bit — `wn` packed nibbles [d x n/2 bytes] + `we` raw E8M0
//! scale bytes [d x n/32] — and the activation PRE-quantized to (xq [n], xs [n/32]) by the caller.
//! Dispatches to the active backend (g_mm_mx4q8). n % 32 == 0.
def matmul_mx4q8(var y : array<float>; wn : array<uint8> | #; we : array<uint8> | #; xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_mx4q8, addr(y[0]),
            addr<uint8 const?>(wn[0]), addr<uint8 const?>(we[0]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d)
    }
}

//! Blob+offset form: weight region at ELEMENT offset `woff` in the whole nibble/exponent planes
//! (nibbles at woff/2, E8M0 at woff/32) — no array_view.
def matmul_mx4q8(var y : array<float>; wn : array<uint8>; we : array<uint8>; woff : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_mx4q8, addr(y[0]),
            addr<uint8 const?>(wn[woff / 2l]), addr<uint8 const?>(we[woff / 32l]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d)
    }
}

//! Batched MXFP4·Q8 GEMM for prefill — batch analog of matmul_mx4q8 (Y token-major [ntok x d],
//! activations pre-quantized per token). Only backends flagged mx4_native_batch provide one (the
//! stub panics — grouped prefill routes through the Q8 expansion elsewhere). n % 32 == 0.
def matmul_mx4q8_batch(var y : array<float>; wn : array<uint8> | #; we : array<uint8> | #; xq : array<int8>; xs : array<float>; n, d, ntok : int64) {
    unsafe {
        invoke(g_mm_mx4q8_batch, addr(y[0]),
            addr<uint8 const?>(wn[0]), addr<uint8 const?>(we[0]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, ntok)
    }
}

//! Blob+offset form (nibbles at woff/2, E8M0 at woff/32) — no array_view.
def matmul_mx4q8_batch(var y : array<float>; wn : array<uint8>; we : array<uint8>; woff : int64; xq : array<int8>; xs : array<float>; n, d, ntok : int64) {
    unsafe {
        invoke(g_mm_mx4q8_batch, addr(y[0]),
            addr<uint8 const?>(wn[woff / 2l]), addr<uint8 const?>(we[woff / 32l]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, ntok)
    }
}

//! Region-list batched MXFP4·Q8 GEMM — every routed expert's [d x n] GEMM over its own row range
//! of one token-row image, in ONE dispatch (see the MatmulMx4Q8BatchGroupNFn doc for the packed
//! `offs` triples). Only backends flagged mx4_native_batch provide one. n % 32 == 0.
def matmul_mx4q8_batch_groupn(var y : array<float>; wn : array<uint8> | #; we : array<uint8> | #; offs : array<int64>; nregions : int64;
                              xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_mx4q8_batch_groupn, addr(y[0]),
            addr<uint8 const?>(wn[0]), addr<uint8 const?>(we[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            null, null, n, d)
    }
}

//! Biased form: folds bias[boffs[r] + row] into every store of region r (per-expert bias vectors).
def matmul_mx4q8_batch_groupn(var y : array<float>; wn : array<uint8> | #; we : array<uint8> | #; offs : array<int64>; nregions : int64;
                              xq : array<int8>; xs : array<float>; bias : array<float> | #; boffs : array<int64>; n, d : int64) {
    unsafe {
        invoke(g_mm_mx4q8_batch_groupn, addr(y[0]),
            addr<uint8 const?>(wn[0]), addr<uint8 const?>(we[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<float const?>(bias[0]), addr<int64 const?>(boffs[0]), n, d)
    }
}

//! Region-list Q8·Q8 GEMV: ONE dispatch for `nregions` same-shape [d x n] projections (see the
//! MatmulQ8Q8GroupNFn doc for the packed `offs` pairs). wq/ws are the WHOLE blob (offsets are
//! absolute); xq/xs hold the activation row(s) the offs pairs index. Region r -> y[r*d .. r*d+d).
def matmul_q8q8_groupn(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; offs : array<int64>; nregions : int64;
                       xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8_groupn, addr(y[0]),
            addr<int8 const?>(wq[0]), addr<float const?>(ws[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            null, null, n, d)
    }
}

//! Biased form: adds bias[boffs[r] + row] at the store (see the MatmulQ8Q8GroupNFn doc).
def matmul_q8q8_groupn(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; offs : array<int64>; nregions : int64;
                       xq : array<int8>; xs : array<float>; bias : array<float> | #; boffs : array<int64>; n, d : int64) {
    unsafe {
        invoke(g_mm_q8q8_groupn, addr(y[0]),
            addr<int8 const?>(wq[0]), addr<float const?>(ws[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<float const?>(bias[0]), addr<int64 const?>(boffs[0]), n, d)
    }
}

//! Region-list MXFP4·Q8 GEMV — the native-4-bit twin of matmul_q8q8_groupn (offsets in weight
//! elements into the whole nibble/scale planes; the kernels derive the /2 and /32 byte positions).
def matmul_mx4q8_groupn(var y : array<float>; wn : array<uint8> | #; we : array<uint8> | #; offs : array<int64>; nregions : int64;
                        xq : array<int8>; xs : array<float>; n, d : int64) {
    unsafe {
        invoke(g_mm_mx4q8_groupn, addr(y[0]),
            addr<uint8 const?>(wn[0]), addr<uint8 const?>(we[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            null, null, n, d)
    }
}

//! Biased form: adds bias[boffs[r] + row] at the store (see the MatmulQ8Q8GroupNFn doc).
def matmul_mx4q8_groupn(var y : array<float>; wn : array<uint8> | #; we : array<uint8> | #; offs : array<int64>; nregions : int64;
                        xq : array<int8>; xs : array<float>; bias : array<float> | #; boffs : array<int64>; n, d : int64) {
    unsafe {
        invoke(g_mm_mx4q8_groupn, addr(y[0]),
            addr<uint8 const?>(wn[0]), addr<uint8 const?>(we[0]),
            addr<int64 const?>(offs[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            addr<float const?>(bias[0]), addr<int64 const?>(boffs[0]), n, d)
    }
}

[hint(unsafe_range_check, noalias = x, noalias = wq, noalias = ws)]
def template dot_q4_template(wq : uint8 const ?; ws : float const ?; x : float const ?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        let bbyte = bi * 16l
        let s = unsafe(ws[bi])
        var bs = 0.0
        for [tune = 1] (j in range64(16l)) {
            let byte = int(unsafe(wq[bbyte + j]))
            bs += float((byte & 0x0F) - 8) * unsafe(x[base + j]) + float((byte >> 4) - 8) * unsafe(x[base + 16l + j])
        }
        acc += s * bs
    }
    return acc
}

//! Q4_0 dequant-in-dot: `wq` is GGML split-half nibble packing (byte j = elem j low / j+16 high) +
//! fp32 scale `ws` per 32-block. Widens nibbles at NEON lane width 4 (width 8 loses ~7% to cross-lane
//! shuffles, hence fallback vec4_u4) — won the kernel bench (see matmul_variants). n % 32 == 0.
[hint(unsafe_range_check, noalias = x, noalias = wq, noalias = ws), tuned(fallback = "vec4_u4")]
def dot_q4(wq : uint8 const?; ws : float const?; x : float const?; n : int64) : float {}

//! Q4_0 matmul: y = W·x, W as 4-bit quants `wq` [d x n/2 bytes] + per-block scales `ws` [d x n/32].
//! Output rows thread at 4 chunks/job (nibble unpack is compute-bound, so finer splitting balances
//! better); small matmuls stay single-threaded. n % 32 == 0. Blob+offset: woff in elements.
def matmul_q4(var y : array<float>; wq : array<uint8>; ws : array<float>; woff : int64; x : array<float>; n, d : int64) {
    let nb = n / 32l
    let nh = n / 2l
    unsafe {
        var yp = addr(y[0])
        let wp = addr<uint8 const?>(wq[woff / 2l])
        let sp = addr<float const?>(ws[woff / 32l])
        let xp = addr(x[0])
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), g_q4_chunks_per_job, n * d)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot_q4(wp + int64(i) * nh, sp + int64(i) * nb, xp, n)
                }
            }
        }
    }
}

def matmul_q4(var y : array<float>; wq : array<uint8> | #; ws : array<float> | #; x : array<float>; n, d : int64) {
    let nb = n / 32l
    let nh = n / 2l
    unsafe {
        var yp = addr(y[0])
        let wp = addr<uint8 const?>(wq[0])
        let sp = addr<float const?>(ws[0])
        let xp = addr(x[0])
        // default 4 chunks per hw job (knob): the heavy nibble unpack makes each row compute-bound, so finer
        // splitting balances better (esp. the classifier) — more than the compute-light Q8 path.
        maybe_parallel_for(0, int(d), matmul_chunks_gemv(int(d), g_q4_chunks_per_job, n * d)) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    yp[i] = dot_q4(wp + int64(i) * nh, sp + int64(i) * nb, xp, n)
                }
            }
        }
    }
}

// noalias only on `weight` — `o` and `x` alias in-place for the Gemma2 post-norms (rms_at(buf,…,buf)),
// so a vectorize perm pays a runtime alias check and falls back to scalar exactly when they alias.
[hint(noalias = weight)]
def template rmsnorm_template(var o : float ?; x : float const ?; weight : float const ?; size : int64; eps : float = 1.0e-5) : void {
    unsafe {
        var ss = 0.0
        for [tune = 1] (j in range64(size)) {
            ss += x[j] * x[j]
        }
        ss /= float(size)
        ss += eps
        ss = 1.0 / sqrt(ss)
        for [tune = 1] (j in range64(size)) {
            o[j] = weight[j] * (x[j] * ss)
        }
    }
}

//! RMSNorm: o[i] = weight[i] * x[i] / sqrt(mean(x^2) + eps). eps defaults to 1e-5 (Llama-2/3);
//! Qwen2/Gemma2 pass 1e-6. `o`/`x` may alias (in-place post-norms) — the sum over x completes
//! before any write to o. Tuned fallback "plain" is byte-identical; a vectorized perm reorders low bits.
[hint(noalias = weight), tuned(fallback = "plain")]
def rmsnorm(var o : float?; x : float const?; weight : float const?; size : int64; eps : float = 1.0e-5) {}
def rmsnorm(var o : array<float> | #; x : array<float> | #; weight : array<float> | #; size : int64; eps : float = 1.0e-5) {
    // reinterpret strips the temporary (#) qualifier — rms_at passes array_view (#) slices. Output `o`
    // keeps a non-const pointer (it's written); the read-only x/weight add const.
    unsafe {
        rmsnorm(addr<float?>(o[0]), addr<float const?>(x[0]),
            addr<float const?>(weight[0]), size, eps)
    }
}

//! Horizontal max of the first `n` floats, hand-vectorized (two float4 accumulators; ~9x over scalar).
//! Reassociated by hand for bit-exactness — LLVM won't auto-vectorize a max-reduction without
//! nnan/nsz fast-math. Safe for n < 8; returns -FLT_MAX for n <= 0.
def hmax(x : float const?; n : int64) : float {
    if (n <= 0l) return -FLT_MAX
    unsafe {
        let pf4 = reinterpret<float4 const?>(x)
        let n8 = n / 8l
        var acc0 = float4(x[0], x[0], x[0], x[0])
        var acc1 = acc0
        for (k in 0l..n8) {
            acc0 = max(acc0, pf4[2l * k])
            acc1 = max(acc1, pf4[2l * k + 1l])
        }
        let acc = max(acc0, acc1)
        var maxv = max(max(acc.x, acc.y), max(acc.z, acc.w))
        for (i in n8 * 8l .. n) {
            maxv = max(maxv, x[i])
        }
        return maxv
    }
}

def template softmax_template(var x : float ?; size : int64) : void {
    if (size <= 0) return   // seeds the reduction with x[0]; size<=0 must be a no-op (same contract as the array overload)
    unsafe {
        let maxv = hmax(x, size)   // hand-vectorized max reduction (see hmax)
        // exp4 fold (like the flash softmax folds): clamp underflow to -80 (exp -> ~0, negligible)
        let m4 = float4(maxv, maxv, maxv, maxv)
        var sum4 = float4(0.0)
        var i = 0l
        while (i + 4l <= size) {
            let e = exp4(max(float4(x[i], x[i + 1l], x[i + 2l], x[i + 3l]) - m4, EXP4_LO))
            x[i] = e.x
            x[i + 1l] = e.y
            x[i + 2l] = e.z
            x[i + 3l] = e.w
            sum4 += e
            i += 4l
        }
        var sum = sum4.x + sum4.y + sum4.z + sum4.w
        while (i < size) {
            let e = exp(max(x[i] - maxv, -80.0))
            x[i] = e
            sum += e
            i++
        }
        let inv_sum = 1.0 / sum   // scale by the reciprocal (matches ggml_vec_scale_f32), not a per-element /=
        for [tune = 1] (j in range64(size)) {
            x[j] *= inv_sum
        }
    }
}

//! Pointer core — the threaded prefill attention runs in fork contexts and can only touch raw
//! addresses; the array overload forwards here, so both are bit-identical. Output `x` is a `var
//! float?` written in place (a hoisted/captured fork pointer binds directly — no const-strip).
[tuned]
def softmax(var x : float?; size : int64) {}
def softmax(var x : array<float> | #; size : int64) {
    if (size <= 0) return   // empty array: addr(x[0]) would index out of range; old loop form no-op'd
    softmax(unsafe(addr(x[0])), size)
}

def template softmax_sink_template(var x : float ?; size : int64; sink : float) : void {
    if (size <= 0) return
    unsafe {
        let maxv = max(hmax(x, size), sink)
        // exp4 fold — same shape (and clamp convention) as softmax_template
        let m4 = float4(maxv, maxv, maxv, maxv)
        var sum4 = float4(0.0)
        var i = 0l
        while (i + 4l <= size) {
            let e = exp4(max(float4(x[i], x[i + 1l], x[i + 2l], x[i + 3l]) - m4, EXP4_LO))
            x[i] = e.x
            x[i + 1l] = e.y
            x[i + 2l] = e.z
            x[i + 3l] = e.w
            sum4 += e
            i += 4l
        }
        var sum = sum4.x + sum4.y + sum4.z + sum4.w
        while (i < size) {
            let e = exp(max(x[i] - maxv, -80.0))
            x[i] = e
            sum += e
            i++
        }
        sum += exp(max(sink - maxv, -80.0))
        let inv_sum = 1.0 / sum
        for [tune = 1] (j in range64(size)) {
            x[j] *= inv_sum
        }
    }
}

//! Softmax with an attention-sink logit (gpt-oss): `sink` joins the max and denominator as one
//! extra virtual score with NO value contribution — x sums to sum/(sum+e^(sink-max)) < 1 by design.
//! Shares softmax's loop shape, so the tuner mirrors softmax's winning perm onto its profile key.
[tuned]
def softmax_sink(var x : float?; size : int64; sink : float) {}
def softmax_sink(var x : array<float> | #; size : int64; sink : float) {
    if (size <= 0) return
    softmax_sink(unsafe(addr(x[0])), size, sink)
}

//! Vectorized expf, 4 lanes (ggml_v_expf, ARM optimized-routines): ~2 ulp vs libm, ~7x scalar exp()
//! (which scalarizes on Darwin-ARM, no vector libm). FAST PATH ONLY (no |n|>126 guard) — valid for
//! x in ~[-87, 88]; callers must clamp (the flash softmax fold clamps to -80 before calling).
def exp4(x : float4) : float4 {
    let z = mad(x, float4(1.4426950216293335), float4(12582912.0))   // x*log2e + 1.5*2^23
    let n = z - float4(12582912.0)                                   // round(x*log2e)
    let b = mad(n, float4(-1.428606765330187e-06), mad(n, float4(-0.693145751953125), x))  // x - n*ln2
    let u = b * b
    let p = mad(mad(mad(float4(0.008247390389442444), b, float4(0.04189976677298546)), u,
        mad(float4(0.16668395698070526), b, float4(0.4999912679195404))), u, float4(0.9999994039535522) * b)
    unsafe {
        let zbits = reinterpret<uint4>(z)
        let k = reinterpret<float4>((zbits << 23) + uint4(0x3f800000u, 0x3f800000u, 0x3f800000u, 0x3f800000u))
        return mad(p, k, k)                                          // 2^n * (1 + poly(b))
    }
}

//! Index of the max element, first-index-wins on ties — the greedy-decode argmax. Chunked team
//! scan (serial 151-262k-vocab scan idled every lane 210-284us/token); combine folds candidates
//! in index order with strict >, so the result is bit-identical to the serial loop for ANY split.
def public parallel_argmax(x : float const?; n : int64) : int64 {
    if (n <= 1l) {
        return 0l
    }
    let nch = int(min(int64(get_dispatch_lanes()) * 2l, n / 4096l))
    unsafe {
        if (nch <= 1) {
            var best = 0l
            for (i in range64(1l, n)) {
                if (x[i] > x[best]) {
                    best = i
                }
            }
            return best
        }
        var cm : array<float>
        var ci : array<int64>
        cm |> resize(nch)
        ci |> resize(nch)
        var cmp = addr(cm[0])
        var cip = addr(ci[0])
        let nn = n
        let nc = int64(nch)
        maybe_parallel_for(0, nch, nch) $(cb, ce) {
            unsafe {
                for (c in range(cb, ce)) {
                    let r0 = int64(c) * nn / nc
                    let r1 = (int64(c) + 1l) * nn / nc
                    var b = r0
                    for (i in range64(r0 + 1l, r1)) {
                        if (x[i] > x[b]) {
                            b = i
                        }
                    }
                    cmp[c] = x[b]
                    cip[c] = b
                }
            }
        }
        var best = ci[0]
        var bm = cm[0]
        for (c in range(1, nch)) {
            if (cm[c] > bm) {
                bm = cm[c]
                best = ci[c]
            }
        }
        delete cm
        delete ci
        return best
    }
}

//! Array overload — forwards to the pointer core.
def public parallel_argmax(x : array<float> | #; n : int64) : int64 {
    if (n <= 0l) {
        return 0l
    }
    return parallel_argmax(unsafe(addr(x[0])), n)
}

//! SiLU (a.k.a. swish) activation: silu(x) = x * sigmoid(x) = x / (1 + exp(-x)), in place.
//! The nonlinearity in the SwiGLU FFN gate; smooth, ~identity for large x, dips slightly negative near zero.
def silu(var x : float?; size : int64) {
    unsafe {
        for (i in range64(size)) {
            let v = x[i]
            x[i] = v / (1.0 + exp(-v))
        }
    }
}
def silu(var x : array<float> | #; size : int64) {
    if (size <= 0) return   // empty array: addr(x[0]) would index out of range; old loop form no-op'd
    silu(unsafe(addr(x[0])), size)
}

//! Softplus, matching ggml's op_softplus exactly: x > 20 → x, else log(1 + exp(x)).
def softplus(x : float) : float => x > 20.0 ? x : log(1.0 + exp(x))

//! Row-wise L2 normalization in place (ggml_l2_norm): scale = 1 / max(sqrt(Σx²), eps), the
//! square-sum accumulated in double like ggml's ggml_float. Used on the deltanet q/k heads.
[hint(noalias = v)]
def l2_norm_rows(var v : float?; rows, row_size : int64; eps : float) {
    unsafe {
        for (r in range64(rows)) {
            let base = r * row_size
            var sum = 0.0lf
            for (i in range64(row_size)) {
                let x = v[base + i]
                sum += double(x * x)
            }
            let scale = 1.0 / max(sqrt(float(sum)), eps)
            for (i in range64(row_size)) {
                v[base + i] *= scale
            }
        }
    }
}

//! GELU, tanh approximation (HF's gelu_pytorch_tanh): the GeGLU gate nonlinearity (Gemma2).
//! gelu(x) = 0.5x(1 + tanh(sqrt(2/pi)(x + 0.044715 x^3))), computed directly rather than via
//! ggml's fp16 LUT approximation — near-ties against the LUT are expected, not bugs.
def gelu(var x : float?; size : int64) {
    let k = 0.7978845608028654  // sqrt(2/pi)
    unsafe {
        for (i in range64(size)) {
            let v = x[i]
            x[i] = 0.5 * v * (1.0 + tanh(k * (v + 0.044715 * v * v * v)))
        }
    }
}
def gelu(var x : array<float> | #; size : int64) {
    if (size <= 0) return   // empty array: addr(x[0]) would index out of range; old loop form no-op'd
    gelu(unsafe(addr(x[0])), size)
}

//! Fused SwiGLU gate: g[i] = silu(g[i]) * u[i] in place (g = W1·x gate, u = W3·x up). One pass
//! instead of silu-then-mul avoids round-tripping the intermediate through memory; bit-exact vs the split path.
def swiglu(var g : float?; u : float const?; size : int64) {
    unsafe {
        for (i in range64(size)) {
            let v = g[i]
            g[i] = (v / (1.0 + exp(-v))) * u[i]
        }
    }
}
def swiglu(var g : array<float> | #; u : array<float> | #; size : int64) {
    if (size <= 0) return
    swiglu(unsafe(addr(g[0])), unsafe(addr(u[0])), size)
}

//! Fused GeGLU gate (Gemma2): g[i] = gelu(g[i]) * u[i] in place, tanh-approx gelu. Same fusion
//! rationale and bit-exactness as swiglu.
def geglu(var g : float?; u : float const?; size : int64) {
    let k = 0.7978845608028654  // sqrt(2/pi)
    unsafe {
        for (i in range64(size)) {
            let v = g[i]
            g[i] = (0.5 * v * (1.0 + tanh(k * (v + 0.044715 * v * v * v)))) * u[i]
        }
    }
}
def geglu(var g : array<float> | #; u : array<float> | #; size : int64) {
    if (size <= 0) return
    geglu(unsafe(addr(g[0])), unsafe(addr(u[0])), size)
}

// gpt-oss clamped-SwiGLU constants (llama.cpp hardcodes both in build_moe_ffn's SWIGLU_OAI arm)
let SWIGLU_OAI_ALPHA = 1.702
let SWIGLU_OAI_LIMIT = 7.0

//! Fused clamped SwiGLU (gpt-oss, ggml_swiglu_oai): x = min(g, 7), y = clamp(u, -7, 7),
//! g = (x · sigmoid(1.702·x)) · (y + 1). The clamps and the +1 on the up branch are part of the
//! trained activation, not an optimization. Same fusion contract as swiglu.
def swiglu_oai(var g : float?; u : float const?; size : int64) {
    unsafe {
        for (i in range64(size)) {
            let x = min(g[i], SWIGLU_OAI_LIMIT)
            let y = clamp(u[i], -SWIGLU_OAI_LIMIT, SWIGLU_OAI_LIMIT)
            g[i] = (x / (1.0 + exp(-SWIGLU_OAI_ALPHA * x))) * (y + 1.0)
        }
    }
}
def swiglu_oai(var g : array<float> | #; u : array<float> | #; size : int64) {
    if (size <= 0) return
    swiglu_oai(unsafe(addr(g[0])), unsafe(addr(u[0])), size)
}

// exp4 valid range (fast-path, no overflow guard) — clamp the exp argument here so an outlier
// gate value can never feed exp4 out of range (silu saturates to 0/v, gelu to ±1 there anyway).
let EXP4_LO = float4(-80.0, -80.0, -80.0, -80.0)
let EXP4_HI = float4(80.0, 80.0, 80.0, 80.0)

//! Vectorized fused SwiGLU gate (default): g[i] = silu(g[i]) * u[i], float4 with exp4. sigmoid =
//! 1/(1+exp4(-v)); ~4.5x the scalar swiglu (which pays a libm expf per element). ~2 ulp, NOT bit-exact
//! (that's the scalar swiglu, kept as the opt-in reference). Tail (<4 elems) runs the scalar form.
def swiglu4(var g : float?; u : float const?; size : int64) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    unsafe {
        var g4 = reinterpret<float4?>(g)
        var u4 = reinterpret<float4 const?>(u)
        let n4 = size / 4l
        for (k in range64(n4)) {
            let v = g4[k]
            let arg = clamp(-v, EXP4_LO, EXP4_HI)
            g4[k] = (v / (one + exp4(arg))) * u4[k]
        }
        for (i in range64(n4 * 4l, size)) {
            let v = g[i]
            g[i] = (v / (1.0 + exp(-v))) * u[i]
        }
    }
}

//! Vectorized fused GeGLU gate (default, Gemma2): g[i] = gelu(g[i]) * u[i], float4 with exp4 via the
//! identity tanh(a) = 1 - 2/(1+exp(2a)); ~4.8x the scalar geglu (which pays a libm tanhf per element —
//! the slowest transcendental, no vector path). ~2 ulp, NOT bit-exact. Tail runs the scalar form.
def geglu4(var g : float?; u : float const?; size : int64) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    let two = float4(2.0, 2.0, 2.0, 2.0)
    let half = float4(0.5, 0.5, 0.5, 0.5)
    let gk = float4(0.7978845608028654)   // sqrt(2/pi)
    let cc = float4(0.044715)
    unsafe {
        var g4 = reinterpret<float4?>(g)
        var u4 = reinterpret<float4 const?>(u)
        let n4 = size / 4l
        for (k in range64(n4)) {
            let v = g4[k]
            let a = gk * (v + cc * v * v * v)
            let arg = clamp(two * a, EXP4_LO, EXP4_HI)
            let th = one - two / (one + exp4(arg))   // tanh(a)
            g4[k] = (half * v * (one + th)) * u4[k]
        }
        let kk = 0.7978845608028654
        for (i in range64(n4 * 4l, size)) {
            let v = g[i]
            g[i] = (0.5 * v * (1.0 + tanh(kk * (v + 0.044715 * v * v * v)))) * u[i]
        }
    }
}

//! Vectorized fused clamped SwiGLU (gpt-oss), float4 with exp4. A very negative gate makes -alpha·x
//! large-positive; the EXP4_HI clamp keeps exp4 finite (exp4(80)≈5.5e34), driving sigmoid to ~0.
//! ~2 ulp, NOT bit-exact (scalar swiglu_oai is the opt-in reference); scalar tail for <4 elems.
def swiglu_oai4(var g : float?; u : float const?; size : int64) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    let lim = float4(SWIGLU_OAI_LIMIT)
    let nalpha = float4(-SWIGLU_OAI_ALPHA)
    unsafe {
        var g4 = reinterpret<float4?>(g)
        var u4 = reinterpret<float4 const?>(u)
        let n4 = size / 4l
        for (k in range64(n4)) {
            let x = min(g4[k], lim)
            let y = clamp(u4[k], -lim, lim)
            let arg = clamp(nalpha * x, EXP4_LO, EXP4_HI)
            g4[k] = (x / (one + exp4(arg))) * (y + one)
        }
        for (i in range64(n4 * 4l, size)) {
            let x = min(g[i], SWIGLU_OAI_LIMIT)
            let y = clamp(u[i], -SWIGLU_OAI_LIMIT, SWIGLU_OAI_LIMIT)
            g[i] = (x / (1.0 + exp(-SWIGLU_OAI_ALPHA * x))) * (y + 1.0)
        }
    }
}

//! Vectorized SiLU: x[i] = x[i] · σ(x[i]) in place, float4 with exp4; ~4x the scalar silu
//! (which pays a libm expf per element). ~2 ulp, NOT bit-exact — scalar silu stays the
//! reference the chat-model oracles pin; ungated conformer FFNs (parakeet) call this form.
def silu4(var x : float?; size : int64) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    unsafe {
        var x4 = reinterpret<float4?>(x)
        let n4 = size / 4l
        for (k in range64(n4)) {
            let v = x4[k]
            let arg = clamp(-v, EXP4_LO, EXP4_HI)
            x4[k] = v / (one + exp4(arg))
        }
        for (i in range64(n4 * 4l, size)) {
            let v = x[i]
            x[i] = v / (1.0 + exp(-v))
        }
    }
}
def silu4(var x : array<float> | #; size : int64) {
    if (size <= 0) return
    silu4(unsafe(addr(x[0])), size)
}

//! Threaded silu4 over a token-major image (npos rows x d columns): rows split across lanes.
//! Row length must be a multiple of 4 (keeps chunk starts 4-aligned so results stay bit-identical
//! at any lane count); measured M1 8 lanes: 3.6x on parakeet jfk ffn, 6.9x on gb1.
def silu4_batch(var x : array<float> | #; d, npos : int64) {
    if (d % 4l != 0l || npos <= 1l) {   // odd row length would shift the float4 grouping per chunk
        silu4(x, d * npos)
        return
    }
    unsafe {
        var xp = addr(x[0])
        maybe_parallel_for(0, int(npos), lanes_for_work(npos * d * 16l, 0)) $(rb, re) {
            silu4(unsafe(xp + int64(rb) * d), int64(re - rb) * d)
        }
    }
}

//! Sigmoid gate: x[i] *= σ(g[i]) over the first `size` elements — the gated-attention out-gate
//! shape (qwen35). Scalar reference form; the exp4 twin below is the production path.
def sigmoid_gate(var x : float?; g : float const?; size : int64) {
    unsafe {
        for (i in range64(size)) {
            x[i] *= 1.0 / (1.0 + exp(-g[i]))
        }
    }
}

//! Vectorized sigmoid gate: float4 with exp4, scalar tail. ~2 ulp, NOT bit-exact vs the scalar form.
def sigmoid_gate4(var x : float?; g : float const?; size : int64) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    unsafe {
        var x4 = reinterpret<float4?>(x)
        let g4 = reinterpret<float4 const?>(g)
        let n4 = size / 4l
        for (k in range64(n4)) {
            let arg = clamp(-g4[k], EXP4_LO, EXP4_HI)
            x4[k] = x4[k] / (one + exp4(arg))
        }
        for (i in range64(n4 * 4l, size)) {
            x[i] *= 1.0 / (1.0 + exp(-g[i]))
        }
    }
}

//! Fused SiLU gate into a destination: o[i] = a[i] · silu(z[i]), float4 with exp4, scalar tail.
//! ~2 ulp, NOT bit-exact. The deltanet chunked z-gated out-norm shape (o must not alias a/z).
def silu_mul4(var o : float?; a, z : float const?; size : int64) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    unsafe {
        var o4 = reinterpret<float4?>(o)
        let a4 = reinterpret<float4 const?>(a)
        let z4 = reinterpret<float4 const?>(z)
        let n4 = size / 4l
        for (k in range64(n4)) {
            let zv = z4[k]
            let arg = clamp(-zv, EXP4_LO, EXP4_HI)
            o4[k] = a4[k] * (zv / (one + exp4(arg)))
        }
        for (i in range64(n4 * 4l, size)) {
            let zv = z[i]
            o[i] = a[i] * (zv / (1.0 + exp(-zv)))
        }
    }
}

//! Logit soft-capping (Gemma2): x = cap*tanh(x/cap), bounding values to (-cap, cap). Applied to
//! attention scores (cap 50, after the 1/sqrt(head_size) scale) and final classifier logits (cap 30).
def softcap(var x : float?; size : int64; cap : float) {
    let inv = 1.0 / cap
    unsafe {
        for (i in range64(size)) {
            x[i] = cap * tanh(x[i] * inv)
        }
    }
}
def softcap(var x : array<float> | #; size : int64; cap : float) {
    softcap(unsafe(addr(x[0])), size, cap)
}

//! Vectorized logit soft-capping: x = cap*tanh(x/cap), float4 via the tanh(a)=1-2/(1+exp(2a)) identity.
//! ~2 ulp, NOT bit-exact vs scalar softcap. A 256k-vocab scalar-tanh pass cost 2-4ms/token; this
//! is the production path (scalar tail for the remainder).
def softcap4(var x : float?; size : int64; cap : float) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    let two = float4(2.0, 2.0, 2.0, 2.0)
    let cap4 = float4(cap)
    let invcap4 = float4(1.0 / cap)
    unsafe {
        var x4 = reinterpret<float4?>(x)
        let n4 = size / 4l
        for (k in range64(n4)) {
            let arg = clamp(two * (x4[k] * invcap4), EXP4_LO, EXP4_HI)   // 2 * (x/cap)
            let th = one - two / (one + exp4(arg))                       // tanh(x/cap)
            x4[k] = cap4 * th
        }
        let inv = 1.0 / cap
        for (i in range64(n4 * 4l, size)) {
            x[i] = cap * tanh(x[i] * inv)
        }
    }
}
def softcap4(var x : array<float> | #; size : int64; cap : float) {
    softcap4(unsafe(addr(x[0])), size, cap)
}

//! RoPE — rotary position embedding. Rotates each adjacent coordinate pair (2j, 2j+1) within a
//! head by angle pos*freq, freq = 1/10000^(2j/head_size); encodes position as rotation so a later
//! Q·K dot product depends only on relative token offset. In place; matches llama2.c's scheme.
def rope(var vec : array<float> | #; pos, head_size, n : int64) {
    for (p in range64(n / 2l)) {
        let i = p * 2l
        let head_dim = i % head_size
        let freq = 1.0 / pow(10000.0, float(head_dim) / float(head_size))
        let angle = float(pos) * freq
        let fcr = cos(angle)
        let fci = sin(angle)
        let v0 = vec[i]
        let v1 = vec[i + 1]
        vec[i] = v0 * fcr - v1 * fci
        vec[i + 1] = v0 * fci + v1 * fcr
    }
}

//! RoPE with explicit base frequency θ and optional per-pair frequency factors (GGUF `rope_freqs`,
//! Llama-3.1 NTK-by-parts). Empty factors => plain RoPE at θ; (θ=10000, empty) matches rope() bit-for-bit.
//! fscale is linear position scaling (angle = pos*fscale*freq); `has_ff` gates the `ff` pointer (pass null when false).
[hint(noalias = v, noalias = ff)]
def rope_scaled(var v : float?; pos, head_size, n : int64; theta, fscale : float; has_ff : bool; ff : float const?; mscale : float = 1.0) {
    unsafe {
        let posf = float(pos) * fscale
        for (p in range64(n / 2l)) {
            let i = p * 2l
            let head_dim = i % head_size
            var freq = 1.0 / pow(theta, float(head_dim) / float(head_size))
            if (has_ff) {
                freq /= ff[head_dim / 2l]
            }
            let angle = posf * freq
            // mscale (YaRN/LongRoPE attn_factor) scales cos/sin — ggml applies it to NORM and NEOX
            // alike (mistral3 YaRN = 1.416); the 1.0 default multiplies out bit-exactly elsewhere
            let fcr = cos(angle) * mscale
            let fci = sin(angle) * mscale
            let v0 = v[i]
            let v1 = v[i + 1]
            v[i] = v0 * fcr - v1 * fci
            v[i + 1] = v0 * fci + v1 * fcr
        }
    }
}
def rope_scaled(var vec : array<float> | #; pos, head_size, n : int64; theta, fscale : float; freq_factors : array<float> | #; mscale : float = 1.0) {
    let has_ff = !empty(freq_factors)
    unsafe {
        let ff = has_ff ? addr<float const?>(freq_factors[0]) : null
        rope_scaled(addr<float?>(vec[0]), pos, head_size, n, theta, fscale, has_ff, ff, mscale)
    }
}

//! NEOX-style RoPE (Qwen2/Phi3/Gemma2): rotated pair is offset by head_size/2 (vec[base+j] with
//! vec[base+j+half]) instead of the adjacent (2j, 2j+1) pair NORM rope uses; same per-pair frequencies.
[hint(noalias = v, noalias = ff)]
def rope_scaled_neox(var v : float?; pos, head_size, n : int64; theta, fscale : float; has_ff : bool; ff : float const?; mscale : float = 1.0) {
    unsafe {
        let half = head_size / 2l
        let posf = float(pos) * fscale
        for (h in range64(n / head_size)) {
            let base = h * head_size
            for (j in range64(half)) {
                var freq = 1.0 / pow(theta, float(2l * j) / float(head_size))
                if (has_ff) {
                    freq /= ff[j]
                }
                let angle = posf * freq
                // mscale (LongRoPE attn_factor) scales cos/sin — i.e. the rotated magnitude (Phi3 = 1.19)
                let fcr = cos(angle) * mscale
                let fci = sin(angle) * mscale
                let v0 = v[base + j]
                let v1 = v[base + j + half]
                v[base + j] = v0 * fcr - v1 * fci
                v[base + j + half] = v0 * fci + v1 * fcr
            }
        }
    }
}
def rope_scaled_neox(var vec : array<float> | #; pos, head_size, n : int64; theta, fscale : float; freq_factors : array<float> | #; mscale : float = 1.0) {
    let has_ff = !empty(freq_factors)
    unsafe {
        let ff = has_ff ? addr<float const?>(freq_factors[0]) : null
        rope_scaled_neox(addr<float?>(vec[0]), pos, head_size, n, theta, fscale, has_ff, ff, mscale)
    }
}

//! Table-driven RoPE (NORM, adjacent pairs); bit-identical to rope_scaled (cos/sin hoisted into a
//! precomputed table since angle pos*freq is identical across heads/layers).
//! Looped per head, not flat p%half — dropping the per-pair modulo is ~2.3x; NEON vectorize doesn't help (bandwidth-bound).
[hint(noalias = v, noalias = cosrow, noalias = sinrow)]
def rope_scaled_tab(var v : float?; head_size, n : int64; cosrow : float const?; sinrow : float const?) {
    unsafe {
        let half = head_size / 2l
        for (h in range64(n / head_size)) {
            let base = h * head_size
            for (j in range64(half)) {
                let i = base + j * 2l
                let fcr = cosrow[j]
                let fci = sinrow[j]
                let v0 = v[i]
                let v1 = v[i + 1]
                v[i] = v0 * fcr - v1 * fci
                v[i + 1] = v0 * fci + v1 * fcr
            }
        }
    }
}

[hint(noalias = v, noalias = cosrow, noalias = sinrow)]
def template rope_scaled_neox_tab_template(var v : float ?; head_size, n : int64; cosrow : float const ?; sinrow : float const ?) : void {
    unsafe {
        let half = head_size / 2l
        for (h in range64(n / head_size)) {
            let base = h * head_size
            for [tune = 1] (j in range64(half)) {
                let fcr = cosrow[j]
                let fci = sinrow[j]
                let v0 = v[base + j]
                let v1 = v[base + j + half]
                v[base + j] = v0 * fcr - v1 * fci
                v[base + j + half] = v0 * fci + v1 * fcr
            }
        }
    }
}

//! Table-driven NEOX RoPE; bit-identical to rope_scaled_neox (mscale baked into cosrow/sinrow).
//! Contiguous v0/v1 split layout vectorizes width-8 (~5-10% over scalar, bit-exact — no fma contract);
//! the NORM leaf's interleaved stride-2 layout can't do that.
[hint(noalias = v, noalias = cosrow, noalias = sinrow), tuned]
def rope_scaled_neox_tab(var v : float?; head_size, n : int64; cosrow : float const?; sinrow : float const?) {}

//! NEOX RoPE over only the FIRST `rot` dims of each head (partial rotary — qwen35: 64 of 256).
//! Pairs are (base+j, base+j+rot/2), frequencies run over `rot`; heads stride by full head_size,
//! dims >= rot pass through untouched (text-only M-RoPE degenerates to this).
[hint(noalias = v, noalias = ff)]
def rope_scaled_neox_part(var v : float?; pos, head_size, rot, n : int64; theta, fscale : float; has_ff : bool; ff : float const?; mscale : float = 1.0) {
    unsafe {
        let half = rot / 2l
        let posf = float(pos) * fscale
        for (h in range64(n / head_size)) {
            let base = h * head_size
            for (j in range64(half)) {
                var freq = 1.0 / pow(theta, float(2l * j) / float(rot))
                if (has_ff) {
                    freq /= ff[j]
                }
                let angle = posf * freq
                let fcr = cos(angle) * mscale
                let fci = sin(angle) * mscale
                let v0 = v[base + j]
                let v1 = v[base + j + half]
                v[base + j] = v0 * fcr - v1 * fci
                v[base + j + half] = v0 * fci + v1 * fcr
            }
        }
    }
}

//! Table form of rope_scaled_neox_part: cosrow/sinrow are one position's rot/2-wide row of a
//! table built by build_rope_table with head_size = rot. Bit-identical to the direct form.
[hint(noalias = v, noalias = cosrow, noalias = sinrow)]
def rope_scaled_neox_tab_part(var v : float?; head_size, rot, n : int64; cosrow : float const?; sinrow : float const?) {
    unsafe {
        let half = rot / 2l
        for (h in range64(n / head_size)) {
            let base = h * head_size
            for (j in range64(half)) {
                let fcr = cosrow[j]
                let fci = sinrow[j]
                let v0 = v[base + j]
                let v1 = v[base + j + half]
                v[base + j] = v0 * fcr - v1 * fci
                v[base + j + half] = v0 * fci + v1 * fcr
            }
        }
    }
}

//! Dispatch RoPE by the model's convention: NEOX (Qwen2/Phi3/Gemma2) or default NORM (Llama).
//! mscale is the YaRN/LongRoPE magnitude factor; both paths apply it (mistral3 = NORM + YaRN).
def rope_apply(var vec : float?; pos, head_size, n : int64; theta, fscale : float; has_ff : bool; ff : float const?; neox : bool; mscale : float = 1.0) {
    if (neox) {
        rope_scaled_neox(vec, pos, head_size, n, theta, fscale, has_ff, ff, mscale)
    } else {
        rope_scaled(vec, pos, head_size, n, theta, fscale, has_ff, ff, mscale)
    }
}
//! `use_ff = false` drops the freq-factors for this call even when they're loaded — p-RoPE arches
//! (gemma4) apply them to global layers only.
def rope_apply(var vec : array<float> | #; pos, head_size, n : int64; theta, fscale : float; freq_factors : array<float> | #; neox : bool; mscale : float = 1.0; use_ff : bool = true) {
    let has_ff = use_ff && !empty(freq_factors)
    unsafe {
        let ff = has_ff ? addr<float const?>(freq_factors[0]) : null
        rope_apply(addr<float?>(vec[0]), pos, head_size, n, theta, fscale, has_ff, ff, neox, mscale)
    }
}
