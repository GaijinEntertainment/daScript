options gen2
options _comment_hygiene = true

module dasllama_kv_codec shared public

require math
require llvm/daslib/f16_cvt  // per-element f16 scale reads/writes inside the hot loops
require dasllama/dasllama_math  // dot/axpy/copy_floats — the f32 arms of the dispatch below
require dasllama/dasllama_convert  // cvt_f16_to_f32 — the bulk f16 row dequant
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
    dst |> resize(n)
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

// ===== per-dtype dispatch over the codec kernels above =====

// tq4 cache-pointer dispatch tag: the read seam overloads on the cache pointer's element type, and
// q8_0 already owns `uint8 const?` — a distinct newtype over uint8 gives tq4 its own overload
// identity at zero runtime cost (byte-strided arithmetic, ABI identical to uint8).
typedef distinct TQ4B = uint8

// Read-side glue: overload per cache element type, resolved inside the monomorphized workers.
def private kv_dot(a : float const?; b : float const?; n : int64) : float => dot(a, b, n)
def kv_axpy(var d : float?; s : float const?; scale : float; n : int64) {
    axpy(d, s, scale, n)
}
def kv_load(p : float const?; base, i : int64) : float => unsafe(p[base + i])
def private kv_dot(a : float const?; b : uint16 const?; n : int64) : float => dot_f16(a, b, n)
def kv_axpy(var d : float?; s : uint16 const?; scale : float; n : int64) {
    axpy_f16(d, s, scale, n)
}
def kv_load(p : uint16 const?; base, i : int64) : float => f16_to_f32(uint(unsafe(p[base + i])))
// q8_0: the cache pointer stays uint8 (byte units — the _d wrappers prescale stride/offset via
// kv_side_off), so `base` here is a whole-block BYTE offset and `i` an element index within it
def private kv_dot(a : float const?; b : uint8 const?; n : int64) : float => dot_q8kv(a, b, n)
def kv_axpy(var d : float?; s : uint8 const?; scale : float; n : int64) {
    axpy_q8kv(d, s, scale, n)
}
def kv_load(p : uint8 const?; base, i : int64) : float {
    unsafe {
        let blk = p + base + i / 32l * 34l
        let b16 = reinterpret<uint16 const?>(blk)   // bind first: reinterpret would swallow the index
        let bq = reinterpret<int8 const?>(blk)
        return float(bq[2l + i % 32l]) * f16_to_f32(uint(b16[0]))
    }
}
// tq4: dispatch rides the TQ4B distinct-pointer tag (q8_0 owns the raw uint8 overloads); the
// kernels themselves are byte-typed, so each arm re-tags back to uint8. Values come out in the
// ROTATED basis — kv_axpy accumulates rotated and the drivers un-rotate each head's output once.
def kv_axpy(var d : float?; s : TQ4B const?; scale : float; n : int64) {
    axpy_tq4kv(d, unsafe(reinterpret<uint8 const?>(s)), scale, n)
}
def kv_load(p : TQ4B const?; base, i : int64) : float {
    unsafe {
        let blk = reinterpret<uint8 const?>(p) + base + i / 32l * 18l
        let b16 = reinterpret<uint16 const?>(blk)
        let by = int(blk[2l + (i % 32l) / 2l])
        let q = (i & 1l) == 0l ? (by & 15) : (by >> 4)
        return float(q - 8) * f16_to_f32(uint(b16[0]))
    }
}
// kv_side_off's pointer-typed twin for code that is generic over the cache element type (the
// flash-decode witness): element offset -> the side's own addressing units
[unused_argument(p)]
def kv_head_off(p : float const?; elems : int64) : int64 => elems
[unused_argument(p)]
def kv_head_off(p : uint16 const?; elems : int64) : int64 => elems
[unused_argument(p)]
def kv_head_off(p : uint8 const?; elems : int64) : int64 => elems / 32l * 34l
[unused_argument(p)]
def kv_head_off(p : TQ4B const?; elems : int64) : int64 => elems / 32l * 18l

// Bulk one-row dequant into f32 scratch — the flash pack's per-row primitive (and any future
// codec's bulk-dequant seam: q8_0 adds a cvt overload here, never a new pack shape).
def kv_row_to_f32(var d : float?; p : float const?; base, n : int64) {
    unsafe {
        copy_floats(d, p + base, n)
    }
}
def kv_row_to_f32(var d : float?; p : uint16 const?; base, n : int64) {
    unsafe {
        cvt_f16_to_f32(d, p + base, n)
    }
}
def kv_row_to_f32(var d : float?; p : uint8 const?; base, n : int64) {
    unsafe {
        cvt_q8kv_to_f32(d, p + base, n)
    }
}
def kv_row_to_f32(var d : float?; p : TQ4B const?; base, n : int64) {
    unsafe {
        cvt_tq4kv_to_f32(d, reinterpret<uint8 const?>(p) + base, n)
    }
}

// Decode-read score dot, resolved on the K-cache pointer type: scalar codecs dot the f32 query;
// q8_0 dots the PRE-QUANTIZED query planes (qq/qs — filled once per token after RoPE) through the
// int8×int8 kernel. Lets the flash-decode witness recompute scores exactly as the sliced path did.
[unused_argument(qq, qs)]
def kv_score(qp : float const?; qq : int8 const?; qs : float const?; kcp : float const?;
             koff, qoff, head_size : int64) : float {
    return kv_dot(unsafe(qp + qoff), unsafe(kcp + koff), head_size)
}
[unused_argument(qq, qs)]
def kv_score(qp : float const?; qq : int8 const?; qs : float const?; kcp : uint16 const?;
             koff, qoff, head_size : int64) : float {
    return kv_dot(unsafe(qp + qoff), unsafe(kcp + koff), head_size)
}
[unused_argument(qp)]
def kv_score(qp : float const?; qq : int8 const?; qs : float const?; kcp : uint8 const?;
             koff, qoff, head_size : int64) : float {
    return dot_q8q8kv(unsafe(qq + qoff), unsafe(qs + qoff / 32l), unsafe(kcp + koff), head_size)
}
// tq4 K: same pre-quantized-query planes as q8_0 — the drivers fill them from the ROTATED query,
// so the int8×int4 dot runs entirely in the rotated basis
[unused_argument(qp)]
def kv_score(qp : float const?; qq : int8 const?; qs : float const?; kcp : TQ4B const?;
             koff, qoff, head_size : int64) : float {
    unsafe {
        let kb = reinterpret<uint8 const?>(kcp)
        return dot_q8tq4kv(qq + qoff, qs + qoff / 32l, kb + koff, head_size)
    }
}
