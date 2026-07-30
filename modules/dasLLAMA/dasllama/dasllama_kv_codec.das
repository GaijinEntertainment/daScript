options gen2
options _comment_hygiene = true

module dasllama_kv_codec shared public

require math
require llvm/daslib/f16_cvt  // per-element f16 scale reads/writes inside the hot loops
require dasllama/dasllama_tune  // [tuned]: reconstitute kernels from templates with per-box loop hints

// The KV-cache codec — one family per cache format, kept WHOLE: store (quantize), read (cvt
// back to f32), the attention score dot, and the V-accumulate axpy share the format's block
// byte geometry, so a layout change can never span modules. These run per token per layer
// (store) and per token per cached position (dot/axpy) — [tuned] hot kernels, not load-time
// converters. Codec dispatch (KVDtype -> family) lives at dasllama_common's kv_store_row /
// kv_load_row / kv_dot / kv_axpy seam. The f16 family's row converts are the generic pair in
// dasllama_convert (cvt_f32_to_f16 / cvt_f16_to_f32 — dual-use: weight/scale planes at load,
// activation prep, and this codec).

// ===== f16 codec: IEEE binary16 held in uint16, converted per element inside the hot loop =====
// (ggml's lesson: convert the O(head_size) side in-loop, keep the bandwidth win — f16_to_f32
// lowers to vcvtph2ps/fcvtl inside the vectorized loop under the JIT).

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

// ===== q8_0 codec: ggml block_q8_0 — 32 int8 quants sharing one f16 scale =====
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

// ===== tq4 codec: TurboQuant-style rotated 4-bit cache =====
// A seeded-sign FWHT basis change at the store/query seams (dasllama_common) over a q4_0-shaped
// block: 32 4-bit quants + one f16 scale = 18B/block. Rows whole blocks; head_size a power of two (FWHT).

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
