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
