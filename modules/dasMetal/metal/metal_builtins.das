options gen2
options indenting = 4

// Metal-only builtin surface on top of the shared shader lingua franca (the spirv_builtins
// pattern: re-export the shared compute IDs + barrier(), add what only this backend speaks).
// All declarations are AST markers: the emitter lowers references/calls by NAME; the stub
// bodies exist so the same kernel method compiles and runs on the CPU. The simd stubs are
// IDENTITY functions — that is the degenerate-but-valid width-1 simdgroup semantics, so a CPU
// replay that sets gl_SubgroupSize = 1u and gl_SubgroupInvocationID = 0u stays exact.

module metal_builtins shared public

require daslib/shader_lingua_franca public
require daslib/math_bits   // uint_bits_to_float — the tensor-op CPU stubs decode bf16/f16 halfwords

// ===== compute builtins (Metal-only additions) =====
// simdgroup IDs, GLSL subgroup spellings (KHR_shader_subgroup names — portable if dasSpirv grows
// subgroup support later); the emitter lowers each to Metal's simdgroup attribute.
var gl_SubgroupInvocationID : uint      // lane within the simdgroup   -> [[thread_index_in_simdgroup]]
var gl_SubgroupSize : uint              // simdgroup execution width   -> [[threads_per_simdgroup]]
var gl_NumSubgroups : uint              // simdgroups per threadgroup  -> [[simdgroups_per_threadgroup]]
var gl_SubgroupID : uint                // simdgroup within the group  -> [[simdgroup_index_in_threadgroup]]

// ===== loop shaping =====
// ===== simdgroup reductions + shuffles (Metal spellings — the Metal value-add) =====
// simd_sum(v): sum of v across the simdgroup's active lanes.
[unused_argument(v), sideeffects] def public simd_sum(v : float) : float { return v }
[unused_argument(v), sideeffects] def public simd_sum(v : int) : int { return v }
[unused_argument(v), sideeffects] def public simd_sum(v : uint) : uint { return v }

// simd_shuffle(v, lane): v from the given absolute lane. Reading an inactive lane is undefined —
// keep shuffles in simdgroup-uniform control flow and select the edge cases on the VALUE after.
[unused_argument(lane), sideeffects] def public simd_shuffle(v : float; lane : uint) : float { return v }
[unused_argument(lane), sideeffects] def public simd_shuffle(v : int; lane : uint) : int { return v }
[unused_argument(lane), sideeffects] def public simd_shuffle(v : uint; lane : uint) : uint { return v }

// simd_shuffle_up/_down(v, delta): v from lane - delta / lane + delta (NOT wrapping — the result
// is undefined for lanes whose source falls outside the simdgroup).
[unused_argument(delta), sideeffects] def public simd_shuffle_up(v : float; delta : uint) : float { return v }
[unused_argument(delta), sideeffects] def public simd_shuffle_up(v : int; delta : uint) : int { return v }
[unused_argument(delta), sideeffects] def public simd_shuffle_up(v : uint; delta : uint) : uint { return v }

[unused_argument(delta), sideeffects] def public simd_shuffle_down(v : float; delta : uint) : float { return v }
[unused_argument(delta), sideeffects] def public simd_shuffle_down(v : int; delta : uint) : int { return v }
[unused_argument(delta), sideeffects] def public simd_shuffle_down(v : uint; delta : uint) : uint { return v }

// simd_shuffle_xor(v, mask): v from lane ^ mask (the butterfly primitive; always in range).
[unused_argument(mask), sideeffects] def public simd_shuffle_xor(v : float; mask : uint) : float { return v }
[unused_argument(mask), sideeffects] def public simd_shuffle_xor(v : int; mask : uint) : int { return v }
[unused_argument(mask), sideeffects] def public simd_shuffle_xor(v : uint; mask : uint) : uint { return v }

// ===== simdgroup_matrix 8x8 tiles (Metal spellings; reference bodies = CPU semantics) =====
// The reference structs hold a row-major 8x8 tile; every op below is the FULL cooperative tile
// operation per call (on GPU all lanes of a simdgroup execute it together; the width-1 CPU replay
// does the whole tile each call — idempotent, so sequential replay stays a valid oracle for
// kernels whose sgmat calls sit in simdgroup-uniform flow). The emitter lowers by NAME; on GPU
// these are the opaque hardware tile types — reaching into `.data` inside a kernel fails closed.

struct public simdgroup_float8x8 {
    data : float[64]
}

struct public simdgroup_half8x8 {
    data : float16[64]
}

def public make_filled_simdgroup_float8x8(v : float) : simdgroup_float8x8 {
    var m : simdgroup_float8x8
    for (k in range(64)) {
        m.data[k] = v
    }
    return m
}

def public make_filled_simdgroup_half8x8(v : float16) : simdgroup_half8x8 {
    var m : simdgroup_half8x8
    for (k in range(64)) {
        m.data[k] = v
    }
    return m
}

// simdgroup_load(m, src, offset, stride): the 8x8 tile whose (r, c) element is
// src[offset + r*stride + c]. `src` is untyped so one spelling covers @ssbo array<T> (device)
// and @workgroup T[N] (threadgroup) sources; the element type must match the matrix.
def public simdgroup_load(var m : simdgroup_float8x8; src; offset : uint; stride : uint) {
    for (r in range(8)) {
        for (c in range(8)) {
            m.data[r * 8 + c] = unsafe(src[int(offset) + r * int(stride) + c])   // pointer-source instantiations index through a das pointer
        }
    }
}

def public simdgroup_load(var m : simdgroup_half8x8; src; offset : uint; stride : uint) {
    for (r in range(8)) {
        for (c in range(8)) {
            m.data[r * 8 + c] = unsafe(src[int(offset) + r * int(stride) + c])   // pointer-source instantiations index through a das pointer
        }
    }
}

// The transpose-flag overloads: with transpose == true the (r, c) element is
// src[offset + c*stride + r] — MSL's transpose_matrix argument. Lets a GEMM stage its B tile
// ROW-major (contiguous threadgroup writes) and transpose at the simdgroup load instead of
// paying strided staging stores.
def public simdgroup_load(var m : simdgroup_float8x8; src; offset : uint; stride : uint; transpose : bool) {
    for (r in range(8)) {
        for (c in range(8)) {
            m.data[r * 8 + c] = unsafe(src[int(offset) + (transpose ? c * int(stride) + r : r * int(stride) + c)])
        }
    }
}

def public simdgroup_load(var m : simdgroup_half8x8; src; offset : uint; stride : uint; transpose : bool) {
    for (r in range(8)) {
        for (c in range(8)) {
            m.data[r * 8 + c] = unsafe(src[int(offset) + (transpose ? c * int(stride) + r : r * int(stride) + c)])
        }
    }
}

// tg_store_half4(dst, idx, v): dst[idx..idx+3] = half4(v) as ONE aligned vector store into a
// float16 @workgroup array (idx % 4 == 0 — 8-byte alignment). The staging-loop sugar: four
// scalar threadgroup stores collapse to one instruction on the GPU; the reference body is the
// exact elementwise equivalent.
def public tg_store_half4(var dst; idx : uint; v : float4) {
    dst[int(idx)] = float16(v.x)
    dst[int(idx) + 1] = float16(v.y)
    dst[int(idx) + 2] = float16(v.z)
    dst[int(idx) + 3] = float16(v.w)
}

// tg_store_float4(dst, idx, v): the float twin — one aligned float4 vector store into a float
// @workgroup array (idx % 4 == 0 — 16-byte alignment).
def public tg_store_float4(var dst; idx : uint; v : float4) {
    dst[int(idx)] = v.x
    dst[int(idx) + 1] = v.y
    dst[int(idx) + 2] = v.z
    dst[int(idx) + 3] = v.w
}

// simdgroup_store(m, dst, offset, stride): the tile back out — @ssbo destinations only.
def public simdgroup_store(m : simdgroup_float8x8; var dst; offset : uint; stride : uint) {
    for (r in range(8)) {
        for (c in range(8)) {
            dst[int(offset) + r * int(stride) + c] = m.data[r * 8 + c]
        }
    }
}

def public simdgroup_store(m : simdgroup_half8x8; var dst; offset : uint; stride : uint) {
    for (r in range(8)) {
        for (c in range(8)) {
            dst[int(offset) + r * int(stride) + c] = m.data[r * 8 + c]
        }
    }
}

// simdgroup_multiply_accumulate(d, a, b, c): d = a*b + c. d may alias c (each element is read
// before its slot is written). The mixed overload is the GEMM workhorse: f16 operand tiles into
// f32 accumulators — an f16 product is exact in f32, matching the hardware's widened MAC.
def public simdgroup_multiply_accumulate(var d : simdgroup_float8x8; a, b, c : simdgroup_float8x8) {
    for (r in range(8)) {
        for (cc in range(8)) {
            var acc = c.data[r * 8 + cc]
            for (k in range(8)) {
                acc += a.data[r * 8 + k] * b.data[k * 8 + cc]
            }
            d.data[r * 8 + cc] = acc
        }
    }
}

def public simdgroup_multiply_accumulate(var d : simdgroup_half8x8; a, b, c : simdgroup_half8x8) {
    for (r in range(8)) {
        for (cc in range(8)) {
            var acc = c.data[r * 8 + cc]
            for (k in range(8)) {
                acc += a.data[r * 8 + k] * b.data[k * 8 + cc]
            }
            d.data[r * 8 + cc] = acc
        }
    }
}

def public simdgroup_multiply_accumulate(var d : simdgroup_float8x8; a, b : simdgroup_half8x8; c : simdgroup_float8x8) {
    for (r in range(8)) {
        for (cc in range(8)) {
            var acc = c.data[r * 8 + cc]
            for (k in range(8)) {
                acc += float(a.data[r * 8 + k]) * float(b.data[k * 8 + cc])
            }
            d.data[r * 8 + cc] = acc
        }
    }
}

// the f32-B/f16-A operand mix (llama.cpp's quant mul_mm shape: float activation tiles against
// half dequantized-weight tiles) — MSL accepts mixed operand element types on Apple silicon
def public simdgroup_multiply_accumulate(var d : simdgroup_float8x8; a : simdgroup_float8x8; b : simdgroup_half8x8; c : simdgroup_float8x8) {
    for (r in range(8)) {
        for (cc in range(8)) {
            var acc = c.data[r * 8 + cc]
            for (k in range(8)) {
                acc += a.data[r * 8 + k] * float(b.data[k * 8 + cc])
            }
            d.data[r * 8 + cc] = acc
        }
    }
}

def public simdgroup_multiply_accumulate(var d : simdgroup_float8x8; a : simdgroup_half8x8; b : simdgroup_float8x8; c : simdgroup_float8x8) {
    for (r in range(8)) {
        for (cc in range(8)) {
            var acc = c.data[r * 8 + cc]
            for (k in range(8)) {
                acc += float(a.data[r * 8 + k]) * b.data[k * 8 + cc]
            }
            d.data[r * 8 + cc] = acc
        }
    }
}

// simdgroup_multiply(d, a, b): d = a*b.
def public simdgroup_multiply(var d : simdgroup_float8x8; a, b : simdgroup_float8x8) {
    for (r in range(8)) {
        for (cc in range(8)) {
            var acc = 0.0
            for (k in range(8)) {
                acc += a.data[r * 8 + k] * b.data[k * 8 + cc]
            }
            d.data[r * 8 + cc] = acc
        }
    }
}

def public simdgroup_multiply(var d : simdgroup_half8x8; a, b : simdgroup_half8x8) {
    for (r in range(8)) {
        for (cc in range(8)) {
            var acc = 0.0h
            for (k in range(8)) {
                acc += a.data[r * 8 + k] * b.data[k * 8 + cc]
            }
            d.data[r * 8 + cc] = acc
        }
    }
}

// ===== Metal-4 tensor ops (MTLTensor / mpp::tensor_ops::matmul2d) =====
// Statement-form cooperative GEMM markers: on the GPU the MSL emitter replaces each call with
// tensor_inline views + a matmul2d op (see msl_emit); the bodies below are the CPU-replay
// reference. All threads must reach the call convergently (cooperative op); the stubs compute
// the full tile once (thread 0) since replay threads run independently.
// Layout contract (NT, the engine's panels): A = activations, m rows x kk, row stride lda;
// W = weights, n output cols x kk, row stride ldw; C = m x n, row stride ldc, OVERWRITTEN.
// m/n/sgs (tile rows, tile cols, cooperating simdgroups) must be call-site constants.

//! f32 activations x bf16 weights -> f32 tile (raw bf16 halfwords in `w`).
[unused_argument(sgs), sideeffects]
def public tmm2d_f32_bf16_f32(m, n, sgs : uint; a : float const?; lda : uint; w : uint16 const?; ldw : uint; var c : float?; ldc : uint; kk : uint) {
    if (gl_LocalInvocationID.x != 0u) {
        return
    }
    unsafe {
        for (r in range(int(m))) {
            for (j in range(int(n))) {
                var acc = 0.0
                for (k in range(int(kk))) {
                    acc += a[r * int(lda) + k] * uint_bits_to_float(uint(w[j * int(ldw) + k]) << 16u)
                }
                c[r * int(ldc) + j] = acc
            }
        }
    }
}

//! Q8xQ8 tile with the per-32-block scale fold (exact int32 accumulate per block, then
//! facc += float(iacc) * as[r][b] * ws[j][b] — the dot_q8q8 contract). Scale row strides in
//! ELEMENTS (blocks): as at a + r*lda_s, ws at w + j*ldw_s.
[unused_argument(sgs), sideeffects]
def public tmm2d_q8_f32(m, n, sgs : uint; aq : int8 const?; lda : uint; asf : float const?; lda_s : uint; wq : int8 const?; ldw : uint; wsf : float const?; ldw_s : uint; var c : float?; ldc : uint; kk : uint) {
    if (gl_LocalInvocationID.x != 0u) {
        return
    }
    unsafe {
        let nb = int(kk) / 32
        for (r in range(int(m))) {
            for (j in range(int(n))) {
                var facc = 0.0
                for (b in range(nb)) {
                    var iacc = 0
                    for (k in range(32)) {
                        iacc += int(aq[r * int(lda) + b * 32 + k]) * int(wq[j * int(ldw) + b * 32 + k])
                    }
                    facc += float(iacc) * asf[r * int(lda_s) + b] * wsf[j * int(ldw_s) + b]
                }
                c[r * int(ldc) + j] = facc
            }
        }
    }
}

// binary16 halfword -> f32 for the CPU stubs (exact; subnormals included)
def private f16_bits_to_f32(h : uint) : float {
    let s = (h >> 15u) & 1u
    let e = (h >> 10u) & 31u
    let mant = h & 1023u
    var bits : uint
    if (e == 0u) {
        if (mant == 0u) {
            bits = s << 31u
        } else {
            var m = mant
            var ee = 127u - 15u + 1u
            while ((m & 1024u) == 0u) {
                m <<= 1u
                ee--
            }
            bits = (s << 31u) | (ee << 23u) | ((m & 1023u) << 13u)
        }
    } elif (e == 31u) {
        bits = (s << 31u) | (255u << 23u) | (mant << 13u)
    } else {
        bits = (s << 31u) | ((e + 127u - 15u) << 23u) | (mant << 13u)
    }
    return uint_bits_to_float(bits)
}

//! The wscale_f16 twin of tmm2d_q8_f32: weight scales are raw binary16 halfwords.
[unused_argument(sgs), sideeffects]
def public tmm2d_q8_f16s(m, n, sgs : uint; aq : int8 const?; lda : uint; asf : float const?; lda_s : uint; wq : int8 const?; ldw : uint; wsf : uint16 const?; ldw_s : uint; var c : float?; ldc : uint; kk : uint) {
    if (gl_LocalInvocationID.x != 0u) {
        return
    }
    unsafe {
        let nb = int(kk) / 32
        for (r in range(int(m))) {
            for (j in range(int(n))) {
                var facc = 0.0
                for (b in range(nb)) {
                    var iacc = 0
                    for (k in range(32)) {
                        iacc += int(aq[r * int(lda) + b * 32 + k]) * int(wq[j * int(ldw) + b * 32 + k])
                    }
                    facc += float(iacc) * asf[r * int(lda_s) + b] * f16_bits_to_f32(uint(wsf[j * int(ldw_s) + b]))
                }
                c[r * int(ldc) + j] = facc
            }
        }
    }
}

//! Interleaved-q8_0 W blocks x RAW f32 activations (the production MulMm dataflow). `wsh` is the
//! blob's f16-scale view at the tile's first block (row j chunk b scale at (j*ldwb+b)*17); `wqb`
//! the byte view pre-offset to that block's quants (+2 done by the caller). `ldwb` = W row stride
//! in BLOCKS (kk/32 for a full row; larger under split-K). W scale folded between f32 chunks.
[unused_argument(sgs), sideeffects]
def public tmm2d_q8b_f32(m, n, sgs : uint; wsh : float16 const?; wqb : int8 const?; a : float const?; lda : uint; var c : float?; ldc : uint; kk : uint; ldwb : uint) {
    if (gl_LocalInvocationID.x != 0u) {
        return
    }
    unsafe {
        let nb = int(kk) / 32
        for (r in range(int(m))) {
            for (j in range(int(n))) {
                var facc = 0.0
                for (b in range(nb)) {
                    let blk = j * int(ldwb) + b
                    var ch = 0.0
                    for (k in range(32)) {
                        ch += a[r * int(lda) + b * 32 + k] * float(wqb[blk * 34 + k])
                    }
                    facc += ch * float(wsh[blk * 17])
                }
                c[r * int(ldc) + j] = facc
            }
        }
    }
}

// ===== staged-tile cooperative GEMM protocol (begin / step / store) =====
// For kernels that DEQUANT-stage weight/activation tiles into @workgroup float16 (mx4 LUT,
// k-quant sub-scales): stage a FLAT row-major [rows][kk] tile pair per K-chunk, then step.
// `acc` is an m*n float @workgroup (or local) scratch — the CPU-replay accumulator; on GPU the
// emitter keys a persistent cooperative tensor to the acc VARIABLE and never touches the array.
// run() OVERWRITES a cooperative destination (probe-proven), so step folds explicitly.

//! begin: zero the accumulator. m/n/sgs/kk must be call-site constants (kk = chunk K width).
[unused_argument(sgs, kk), sideeffects]
def public tmm2d_tg_begin(var acc; m, n, sgs, kk : uint) {
    if (gl_LocalInvocationID.x != 0u) {
        return
    }
    for (i in range(int(m * n))) {
        acc[i] = 0.0
    }
}

//! step: acc[r][j] += ta[r][k] * tb[j][k] over one staged chunk — ta flat [m][kk] f16,
//! tb flat [n][kk] f16, both row-major. Call between the same barriers the staging uses.
[sideeffects]
def public tmm2d_tg_step(var acc; ta; tb; m, n, kk : uint) {
    if (gl_LocalInvocationID.x != 0u) {
        return
    }
    for (r in range(int(m))) {
        for (j in range(int(n))) {
            var facc = 0.0
            for (k in range(int(kk))) {
                facc += float(ta[r * int(kk) + k]) * float(tb[j * int(kk) + k])
            }
            acc[r * int(n) + j] += facc
        }
    }
}

//! store: C[r][j] = acc[r][j] (overwrite, row stride ldc).
[sideeffects]
def public tmm2d_tg_store(acc; var c : float?; m, n, ldc : uint) {
    if (gl_LocalInvocationID.x != 0u) {
        return
    }
    unsafe {
        for (r in range(int(m))) {
            for (j in range(int(n))) {
                c[r * int(ldc) + j] = acc[r * int(n) + j]
            }
        }
    }
}
