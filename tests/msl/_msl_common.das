options gen2
options indenting = 4

// Shared fixture for the dasMetal MSL test suite (mirrors tests/spirv/_spirv_common.das): the
// [metal_kernel] fixture classes whose emitted text the shape, golden, and census tests read,
// plus the declared census — the gate-B contract the fixture union must equal in both directions.
// [marker(no_coverage)] keeps daslib/coverage from instrumenting the kernel bodies: they are
// compile-time MSL source, not runtime code, and injected coverage nodes are not lowerable.

module _msl_common shared public

require metal/msl_shader public
require math

class MulAB {
    @ssbo @binding = 0 a : array<float>
    @ssbo @binding = 1 b : array<float>
    @ssbo @binding = 2 c : array<float>

    [metal_kernel(name="mul_ab_msl"), marker(no_coverage)]
    def mul_ab {
        let i = gl_GlobalInvocationID.x
        c[i] = a[i] * b[i]
    }
}

// Phase-2 arithmetic: one class per scalar class so every opcode flavor is asserted per type
class UArith {
    @ssbo @binding = 0 a : array<uint>
    @ssbo @binding = 1 b : array<uint>
    @ssbo @binding = 2 c : array<uint>
    @ssbo @binding = 3 d : array<uint>

    [metal_kernel(name="uarith_msl"), marker(no_coverage)]
    def uarith {
        let i = gl_GlobalInvocationID.x
        let x = a[i]
        let y = b[i] | 1u
        c[i] = ((x + y) - (x * y)) + ((x / y) + (x % y))
        d[i] = ((x & y) | (x ^ y)) + ((~x) + ((x << 2u) >> 1u))
    }
}

class IArith {
    @ssbo @binding = 0 a : array<int>
    @ssbo @binding = 1 b : array<int>
    @ssbo @binding = 2 c : array<int>
    @ssbo @binding = 3 d : array<int>

    [metal_kernel(name="iarith_msl"), marker(no_coverage)]
    def iarith {
        let i = gl_GlobalInvocationID.x
        let x = a[i]
        let y = b[i] | 1
        c[i] = (((x + y) - (x * y)) + ((x / y) + (x % y))) + (-x)
        d[i] = ((x & y) | (x ^ y)) + ((~x) + ((x << 2) >> 1))
    }
}

class FArith {
    @ssbo @binding = 0 a : array<float>
    @ssbo @binding = 1 b : array<float>
    @ssbo @binding = 2 c : array<float>

    [metal_kernel(name="farith_msl"), marker(no_coverage)]
    def farith {
        let i = gl_GlobalInvocationID.x
        let x = a[i]
        let y = b[i] + 1.5
        c[i] = ((((x + y) - (x * y)) + (x / y)) + (x % y)) + (-x)
    }
}

// Phase-2 vector arithmetic: componentwise float4/int2 infix, vector<op>scalar broadcast, vector
// negate, and das's scalar-bool vector ==/!= (lowered via all()/any())
class VecArith {
    @ssbo @binding = 0 fa : array<float4>
    @ssbo @binding = 1 fb : array<float4>
    @ssbo @binding = 2 fc : array<float4>
    @ssbo @binding = 3 ia : array<int2>
    @ssbo @binding = 4 ib : array<int2>
    @ssbo @binding = 5 flags : array<int>

    [metal_kernel(name="vecarith_msl"), marker(no_coverage)]
    def vecarith {
        let i = gl_GlobalInvocationID.x
        let u = fa[i]
        let v = fb[i] * 0.5
        fc[i] = (((u + v) - (u * v)) + (u / v)) + ((u * 2.0) + (-u))
        let p = ia[i]
        let q = ib[i]
        flags[i] = (p == q) ? 1 : ((((p + q) - (p * q)) != q) ? 2 : 3)
    }
}

// Phase-2 control flow: mutable + zero-init locals, compound assignment (arithmetic + bitwise),
// ++/--, every comparison flavor per scalar class, logical &&/||/!, if/elif/else, ternary
class Control {
    @ssbo @binding = 0 a : array<uint>
    @ssbo @binding = 1 b : array<uint>
    @ssbo @binding = 2 p : array<int>
    @ssbo @binding = 3 q : array<int>
    @ssbo @binding = 4 f : array<float>
    @ssbo @binding = 5 g : array<float>
    @ssbo @binding = 6 out1 : array<uint>
    @ssbo @binding = 7 out2 : array<int>
    @ssbo @binding = 8 out3 : array<float>

    [metal_kernel(name="control_msl"), marker(no_coverage)]
    def control {   // nolint:STYLE037,STYLE038 — conformance fixture; flat one-statement-per-opcode enumeration is the design
        let i = gl_GlobalInvocationID.x
        let x = a[i]
        let y = b[i]
        var acc = 0u
        var zed : uint
        var hot = false
        if (x > y && x != 0u) {
            acc += x
            hot = true
        } elif (x < y || y >= 8u) {
            acc -= x
        } else {
            acc *= 3u
        }
        if (x <= y) {
            acc /= 2u
            acc %= 101u
        }
        if (x == y) {
            acc |= 1u
            acc &= 255u
            acc ^= 129u
            acc <<= 1u
            acc >>= 1u
        }
        acc++
        ++acc
        acc--
        --acc
        let snap = acc++     // value-level postfix survives as postfix (statement-level arrives prefix)
        let dnap = acc--
        zed += (acc + snap) + dnap
        out1[i] = hot ? zed : (zed + 7u)
        // const-cond selects (the stamped @template_constant shape) fold to the taken side
        out1[i] += true ? zed : 12345u
        out1[i] += false ? 54321u : (zed + 1u)
        let m = p[i]
        let w = q[i]
        var iacc = 0
        if (m > w) {
            iacc += 9
        }
        if (m < w) {
            iacc += 2
        }
        if (m >= w) {
            iacc += 3
        }
        if (m <= w) {
            iacc += 4
        }
        if (m == w) {
            iacc += 5
        }
        if (m != w) {
            iacc += 6
        }
        out2[i] = (!(m > w)) ? iacc : (-iacc)
        let s = f[i]
        let z = g[i]
        var facc = 1.0
        if (s > z) {
            facc += 2.0
        }
        if (s < z) {
            facc += 4.0
        }
        if (s >= z) {
            facc *= 2.0
        }
        if (s <= z) {
            facc += 8.0
        }
        if (s == z) {
            facc += 16.0
        }
        if (s != z) {
            facc += 32.0
        }
        out3[i] = facc
    }
}

// Phase-2 loops + the non-exact-grid bounds guard: @uniform scalar, early return, while with
// break/continue, constant range-for, dynamic-end range-for (das evaluates range endpoints once —
// the emitter hoists the end into a loop-scoped local, proven here by mutating `s` in the body),
// and a urange loop over the uniform
class Loops {
    @ssbo @binding = 0 src : array<int>
    @ssbo @binding = 1 dst : array<int>
    @uniform @binding = 2 n : uint
    @ssbo @binding = 3 udst : array<uint>

    [metal_kernel(name="loops_msl"), marker(no_coverage)]
    def loops {
        let i = gl_GlobalInvocationID.x
        if (i >= n) {
            return
        }
        var s = 0
        var j = 0
        while (j < 100) {
            j++
            if (j == 3) {
                continue
            }
            if (j >= 7) {
                break
            }
            s += j
        }
        for (k in range(4)) {
            s += k * src[i]
        }
        for (_k in range(0, s)) {
            s++
        }
        dst[i] = s
        var usum = 0u
        for (ku in urange(n)) {
            usum += ku
        }
        udst[i] = usum
    }
}

// Phase-3 threadgroup memory + barriers: @workgroup members (fixed array + scalar) lower to
// threadgroup declarations at the top of the kernel body; barrier() and memoryBarrierShared()
// both lower to threadgroup_barrier(mem_flags::mem_threadgroup) — MSL has no execution-free
// threadgroup fence; and the five workgroup-grid builtins lower to attributed parameters
class Reduce {
    @ssbo @binding = 0 src : array<uint>
    @ssbo @binding = 1 dst : array<uint>
    @workgroup tile : uint[64]
    @workgroup total : uint

    [metal_kernel(name="reduce_msl"), marker(no_coverage)]
    def reduce {
        let gid = gl_GlobalInvocationID.x
        let lid = gl_LocalInvocationID.x
        tile[lid] = src[gid]
        barrier()
        var s = gl_WorkGroupSize.x / 2u
        while (s > 0u) {
            if (lid < s) {
                tile[lid] += tile[lid + s]
            }
            barrier()
            s /= 2u
        }
        if (gl_LocalInvocationIndex == 0u) {
            total = tile[0] + (gl_NumWorkGroups.x * 0u)
        }
        memoryBarrierShared()   // uniform control flow — a threadgroup_barrier under a lane test is MSL UB
        if (gl_LocalInvocationIndex == 0u) {
            dst[gl_WorkGroupID.x] = total
        }
    }
}

// Phase-3 simdgroup intrinsics: the four simdgroup builtins and all fifteen simd_* overloads
// (sum + the four shuffles, each for float/int/uint)
class Simd {
    @ssbo @binding = 0 fa : array<float>
    @ssbo @binding = 1 ia : array<int>
    @ssbo @binding = 2 ua : array<uint>
    @ssbo @binding = 3 fo : array<float>
    @ssbo @binding = 4 io : array<int>
    @ssbo @binding = 5 uo : array<uint>
    @ssbo @binding = 6 meta : array<uint>

    [metal_kernel(name="simd_msl"), marker(no_coverage)]
    def simd_all {
        let i = gl_GlobalInvocationID.x
        let lane = gl_SubgroupInvocationID
        fo[i] = ((simd_sum(fa[i]) + simd_shuffle(fa[i], lane)) +
            (simd_shuffle_up(fa[i], 1u) + simd_shuffle_down(fa[i], 1u))) + simd_shuffle_xor(fa[i], 1u)
        io[i] = ((simd_sum(ia[i]) + simd_shuffle(ia[i], lane)) +
            (simd_shuffle_up(ia[i], 1u) + simd_shuffle_down(ia[i], 1u))) + simd_shuffle_xor(ia[i], 1u)
        uo[i] = ((simd_sum(ua[i]) + simd_shuffle(ua[i], lane)) +
            (simd_shuffle_up(ua[i], 1u) + simd_shuffle_down(ua[i], 1u))) + simd_shuffle_xor(ua[i], 1u)
        meta[i] = (lane + gl_SubgroupSize) + (gl_NumSubgroups + gl_SubgroupID)
    }
}

// Phase-4 fp16 arithmetic: closed half-vector/scalar-float16 operators, the f16 broadcast,
// half literals (h suffix), lattice zero-init, lane ctor, swizzle, and das's scalar-bool
// vector ==/!= on half vectors
class HalfArith {
    @ssbo @binding = 0 ha : array<half4>
    @ssbo @binding = 1 hb : array<half4>
    @ssbo @binding = 2 hc : array<half4>
    @ssbo @binding = 3 hs : array<float16>
    @ssbo @binding = 4 hd : array<half2>
    @ssbo @binding = 5 hflags : array<int>

    [metal_kernel(name="halfarith_msl"), marker(no_coverage)]
    def halfarith {
        let i = gl_GlobalInvocationID.x
        let a = ha[i]
        let b = hb[i] * 0.5h
        var acc : half4
        let s = hs[i]
        acc = ((a + b) - (a * b)) + ((a / b) + (-a))
        hc[i] = acc
        let t = (s * 2.0h) + 0.25h
        hd[i] = half2(t, s)
        hflags[i] = (a == b) ? 1 : ((a.wzyx != b) ? 2 : ((s < t) ? 3 : 4))
    }
}

// Phase-4 int-lattice storage + converts: widen to 32-bit, widen within the lattice, truncating
// and saturating narrows, scalar 8/16-bit converts — das's storage-only int families compute in
// 32-bit and narrow back, exactly MSL's own conversion-ctor semantics
class LatticeConvert {
    @ssbo @binding = 0 bi : array<byte4>
    @ssbo @binding = 1 io : array<int4>
    @ssbo @binding = 2 bo : array<byte4>
    @ssbo @binding = 3 bs : array<byte4>
    @ssbo @binding = 4 st : array<short4>
    @ssbo @binding = 5 ui : array<ubyte2>
    @ssbo @binding = 6 uso : array<ushort2>
    @ssbo @binding = 7 uo : array<uint2>
    @ssbo @binding = 8 usat : array<ushort2>
    @ssbo @binding = 9 hi : array<half2>
    @ssbo @binding = 10 fo : array<float2>
    @ssbo @binding = 11 ho : array<half2>
    @ssbo @binding = 12 so : array<int>

    [metal_kernel(name="latconv_msl"), marker(no_coverage)]
    def latconv {
        let i = gl_GlobalInvocationID.x
        let w = int4(bi[i])
        io[i] = (w * w) + w
        bo[i] = byte4((w * 3) - int4(100))
        bs[i] = byte4_sat((w * 40) + w)
        st[i] = short4(bi[i])
        let uw = uint2(ui[i])
        uso[i] = ushort2(ui[i])
        uo[i] = ((uw * 3u) + uw) + uint2(7u, 9u)
        usat[i] = ushort2_sat(uw * 1000u)
        let f = float2(hi[i])
        fo[i] = f * 2.0
        ho[i] = half2(f + float2(0.25, 0.5))
        let sc = int16(w.x)
        so[i] = int(sc) + int(bi[i].s0)
    }
}

// Phase-4 packed 3-lane elements: das packs vectors tightly (float3 = 12B, half3 = 6B,
// byte3 = 3B) and unified memory makes the das array the buffer, so 3-lane buffer/uniform
// elements lower to MSL packed_T3 — loads wrap back into the plain type, stores assign whole
// elements
class PackedVec3 {
    @ssbo @binding = 0 pf : array<float3>
    @ssbo @binding = 1 pfo : array<float3>
    @ssbo @binding = 2 ph : array<half3>
    @ssbo @binding = 3 pho : array<half3>
    @ssbo @binding = 4 pb : array<byte3>
    @ssbo @binding = 5 pio : array<int3>
    @uniform @binding = 6 scale : float16
    @uniform @binding = 7 off : float3

    [metal_kernel(name="packed3_msl"), marker(no_coverage)]
    def packed3 {
        let i = gl_GlobalInvocationID.x
        let v = pf[i]
        pfo[i] = (v * 2.0) + off
        let h = ph[i]
        pho[i] = (h + h) * half3(scale)
        pio[i] = int3(pb[i]) * 3
    }
}

// Phase-5 simdgroup_matrix: the reference-body 8x8 tile family — zero-init + filled locals,
// device loads at runtime offset/stride, multiply and multiply-accumulate at f32 and f16,
// stores back out
class SgMatTile {
    @ssbo @binding = 0 fa : array<float>
    @ssbo @binding = 1 fb : array<float>
    @ssbo @binding = 2 fc : array<float>
    @ssbo @binding = 3 fd : array<float>
    @ssbo @binding = 4 ha : array<float16>
    @ssbo @binding = 5 hb : array<float16>
    @ssbo @binding = 6 hc : array<float16>
    @uniform @binding = 7 base : uint

    [metal_kernel(name="sgmat_msl"), marker(no_coverage)]
    def sgmat_tile {
        var ma : simdgroup_float8x8
        var mb : simdgroup_float8x8
        simdgroup_load(ma, fa, base, 8u)
        simdgroup_load(mb, fb, base, 8u)
        var mm = make_filled_simdgroup_float8x8(0.0)
        simdgroup_multiply(mm, ma, mb)
        simdgroup_store(mm, fc, base, 8u)
        var acc = make_filled_simdgroup_float8x8(1.0)
        simdgroup_multiply_accumulate(acc, ma, mb, acc)
        simdgroup_store(acc, fd, base, 8u)
        var hx : simdgroup_half8x8
        var hy : simdgroup_half8x8
        simdgroup_load(hx, ha, 0u, 8u)
        simdgroup_load(hy, hb, 0u, 8u)
        var hm = make_filled_simdgroup_half8x8(0.5h)
        simdgroup_multiply(hm, hx, hy)
        var hacc : simdgroup_half8x8
        simdgroup_multiply_accumulate(hacc, hx, hy, hm)
        simdgroup_store(hacc, hc, 0u, 8u)
    }
}

// Phase-5 tiled-GEMM shape: one C tile per threadgroup — cooperative threadgroup staging of f16
// tiles + barrier + the MIXED mac (f32 accumulators, f16 operands, the offload kernel's combo)
// walking K in 8-steps; M = N = 8, K = kdim
class SgMatTiled {
    @ssbo @binding = 0 xa : array<float16>
    @ssbo @binding = 1 wb : array<float16>
    @ssbo @binding = 2 yo : array<float>
    @uniform @binding = 3 kdim : uint
    @workgroup ta : float16[64]
    @workgroup tb : float16[64]

    [metal_kernel(name="sgmat_tiled_msl"), marker(no_coverage)]
    def sgmat_tiled {
        let lid = gl_LocalInvocationID.x
        var acc : simdgroup_float8x8
        var kk = 0u
        while (kk < kdim) {
            ta[lid] = xa[(lid / 8u) * kdim + (kk + (lid % 8u))]
            ta[lid + 32u] = xa[((lid + 32u) / 8u) * kdim + (kk + ((lid + 32u) % 8u))]
            tb[lid] = wb[(kk * 8u) + lid]
            tb[lid + 32u] = wb[(kk * 8u) + (lid + 32u)]
            barrier()
            var ma : simdgroup_half8x8
            var mb : simdgroup_half8x8
            simdgroup_load(ma, ta, 0u, 8u)
            simdgroup_load(mb, tb, 0u, 8u)
            simdgroup_multiply_accumulate(acc, ma, mb, acc)
            barrier()
            kk += 8u
        }
        simdgroup_store(acc, yo, 0u, 8u)
    }
}

// Phase-7 rolled-GEMM shape: simdgroup-matrix ARRAYS + [unroll_full] loops + a fixed-array
// staging local — the register-pressure-safe form (loops stay rolled through the Metal
// frontend; the GPU backend unrolls with budget awareness). C(16x8) = A(16xK) x B(Kx8),
// f32 accumulators over f16 operands, one simdgroup (32 threads).
class SgMatRolled {
    @ssbo @binding = 0 xa : array<float16>
    @ssbo @binding = 1 wb : array<float16>
    @ssbo @binding = 2 yo : array<float>
    @uniform @binding = 3 kdim : uint
    @workgroup ta : float16[128]        // A slab: 2 row-tiles of 8x8
    @workgroup tb : float16[64]         // B slab: one 8x8 tile

    [metal_kernel(name="sgmat_rolled_msl"), marker(no_coverage)]
    def sgmat_rolled {
        let lid = gl_LocalInvocationID.x
        var mc : simdgroup_float8x8[2]
        var ma : simdgroup_half8x8[2]
        var va : float16[4]
        var kk = 0u
        while (kk < kdim) {
            // this thread's 4 A elements are contiguous in xa — read through a das device pointer
            let flat0 = lid * 4u
            let xcur = unsafe(addr(xa[((flat0 / 64u) * 8u + (flat0 % 64u) / 8u) * kdim + kk + (flat0 % 8u)]))
            for [unroll_full] (i in range(4)) {
                va[i] = unsafe(xcur[i])
            }
            for [unroll_full] (i in range(4)) {
                ta[lid * 4u + uint(i)] = va[i]
            }
            tb[lid] = wb[(kk + lid / 8u) * 8u + (lid % 8u)]
            tb[lid + 32u] = wb[(kk + (lid + 32u) / 8u) * 8u + ((lid + 32u) % 8u)]
            barrier()
            var mbb : simdgroup_half8x8
            simdgroup_load(mbb, tb, 0u, 8u)
            var acur = unsafe(addr(ta[0]))
            for [unroll_full] (i in range(2)) {
                simdgroup_load(ma[i], acur, 0u, 8u)
                simdgroup_multiply_accumulate(mc[i], ma[i], mbb, mc[i])
                acur = unsafe(acur + 64)
            }
            barrier()
            kk += 8u
        }
        for [unroll_full] (i in range(2)) {
            simdgroup_store(mc[i], yo, uint(i) * 64u, 8u)
        }
    }
}

// Phase-6 math builtins: the value-call whitelist with 1:1 MSL spellings — float scalar +
// vector classes throughout (abs also signed int; min/max any numeric class)
class MathCalls {
    @ssbo @binding = 0 a : array<float>
    @ssbo @binding = 1 b : array<float>
    @ssbo @binding = 2 c : array<float>
    @ssbo @binding = 3 va : array<float4>
    @ssbo @binding = 4 vb : array<float4>
    @ssbo @binding = 5 vc : array<float4>
    @ssbo @binding = 6 ia : array<int>
    @ssbo @binding = 7 ib : array<int>
    @ssbo @binding = 8 ic : array<int>
    @ssbo @binding = 9 ua : array<uint>
    @ssbo @binding = 10 ub : array<uint>
    @ssbo @binding = 11 uc : array<uint>
    @ssbo @binding = 12 qa : array<int8>
    @ssbo @binding = 13 qc : array<int8>

    [metal_kernel(name="mathcalls_msl"), marker(no_coverage)]
    def mathcalls {
        let i = gl_GlobalInvocationID.x
        let x = a[i]
        let y = b[i]
        c[i] = (((exp(min(x, 4.0)) + sqrt(abs(x))) + (rsqrt(abs(y) + 1.0) + tanh(x))) +
            ((sin(x) + cos(y)) + (saturate(x) + max(x, y))))
        c[i] += float(qa[i])
        c[i] += log(abs(x) + 1.0)
        let u = va[i]
        let v = vb[i]
        vc[i] = (((exp(min(u, float4(4.0))) + sqrt(abs(u))) + (rsqrt(abs(v) + float4(1.0)) + tanh(u))) +
            ((sin(u) + cos(v)) + (saturate(u) + max(u, v))))
        vc[i] = vc[i] + round(v)
        ic[i] = min(abs(ia[i]), ib[i]) + max(ia[i], ib[i])
        uc[i] = min(ua[i], ub[i]) + max(ua[i], ub[i])
        qc[i] = int8(round(b[i] * 12.5))
    }
}

// ===== Phase 0: user-function lowering (free functions lower 1:1 as MSL functions) =====

def private ufx_sky(d : float) : float => 0.5 * d + 0.5

def private ufx_shade(d : float; bounce : int) : float {
    var acc = ufx_sky(d)     // nested user call
    for (_i in range(bounce)) {
        acc *= 0.5
    }
    return acc
}

def private ufx_lane : uint => gl_GlobalInvocationID.x   // hidden-builtin threading

def private ufx_splat(var yo : array<float>; i : uint; v : float) {   // write-through param
    yo[i] = v
}

def private ufx_twice(src : array<auto(TT)>; i : uint) : TT {   // generic: 2 MSL instances
    return src[i] + src[i]
}

def private ufx_sum_tg(part : float[32]; n : uint) : float {   // threadgroup-pointer param
    var s = 0.0
    var k = 0u
    while (k < n) {
        s += part[k]
        k++
    }
    return s
}

// a WRITABLE threadgroup slab param that also rendezvouses inside the helper — the shape a
// staged kernel (dasLLAMA's attention softmax) hands its @workgroup scratch to
def private ufx_scale_tg(var part : float[32]; k : float; lid, n : uint) {
    var i = lid
    while (i < n) {
        part[i] *= k
        i += n
    }
    barrier()
}

def private ufx_accum(var acc : float&; v : float) {   // thread-ref param
    acc += v
}

// Pointer parameters. das `T?` names no address space, so the parameter declares it — unmarked
// is `device`, `@threadgroup` opts in — and every call site is checked against the provenance
// of the pointer it hands over. The pointee's const is the MSL const, exactly as for a buffer.
def private ufx_ptr_walk(var p : float const?; n : uint) : float {
    var s = 0.0
    var k = 0u
    while (k < n) {
        s += unsafe(p[0])
        p = unsafe(p + 1)   // the advancing-pointer shape the split-K attention kernels stream
        k++
    }
    return s
}

def private ufx_ptr_fwd(var p : float const?; n : uint) : float => ufx_ptr_walk(p, n)   // space propagates

def private ufx_ptr_store(var o : float?; i : uint; v : float) {
    unsafe(o[i]) = v
}

def private ufx_ptr_tg(@threadgroup p : float const?; n : uint) : float {
    var s = 0.0
    var k = 0u
    while (k < n) {
        s += unsafe(p[k])
        k++
    }
    return s
}

class UserFns {
    @ssbo @binding = 0 src : array<float>
    @ssbo @binding = 1 srch : array<float16>
    @ssbo @binding = 2 yo : array<float>
    @ssbo @binding = 3 yoh : array<float16>
    @ssbo @binding = 4 yp : array<float>
    @workgroup part : float[32]

    [metal_kernel(name="userfns_msl"), marker(no_coverage)]
    def userfns {
        let i = ufx_lane()
        let lid = gl_LocalInvocationID.x
        part[lid] = ufx_twice(src, i)
        yoh[i] = ufx_twice(srch, i)
        barrier()
        ufx_scale_tg(part, 0.5, lid, gl_WorkGroupSize.x)   // writes the slab, barriers inside
        var acc = 0.0
        ufx_accum(acc, ufx_shade(src[i], 3))
        if (lid == 0u) {
            acc += ufx_sum_tg(part, gl_WorkGroupSize.x)
            acc += ufx_ptr_tg(unsafe(addr(part[0])), gl_WorkGroupSize.x)
        }
        var sp = unsafe(addr(src[0]))
        acc += ufx_ptr_walk(sp, 2u) + ufx_ptr_fwd(unsafe(sp + i), 2u)
        ufx_ptr_store(unsafe(addr(yp[0])), i, acc)
        ufx_splat(yo, i, acc)
    }
}

// Shape stays compile-time (dasLLAMA REVIEW "shape is compile-time"): the per-type overload does the indexing
// ITSELF, so the stride is a literal inside each instance and never crosses a call boundary as a
// value. The array argument is the codec discriminator, not data — hence the unused annotation.
[unused_argument(sel)]
def private ufx_pick(sel : array<int8>; xs : array<float>; blk : uint) : float => xs[blk * 34u]

[unused_argument(sel)]
def private ufx_pick(sel : array<uint8>; xs : array<float>; blk : uint) : float => xs[blk * 18u]

class UkStride {
    @ssbo @binding = 0 xs : array<float>
    @ssbo @binding = 1 sa : array<int8>
    @ssbo @binding = 2 sb : array<uint8>
    @ssbo @binding = 3 ys : array<float>

    [metal_kernel(name="ukstride_msl"), marker(no_coverage)]
    def ukstride {
        let i = gl_GlobalInvocationID.x
        ys[i] = ufx_pick(sa, xs, i) + ufx_pick(sb, xs, i)
    }
}

// ===== @uniform structs (the kargs form) =====
// A kernel with a dozen scalars costs a dozen binds; one struct costs one. Fields are 4-byte
// scalars only, so the das layout IS the MSL layout and the host writes its struct straight
// through setBytes.

struct UkArgs {
    n : uint
    gain : float
    bias : int
}

class UkKern {
    @ssbo @binding = 0 xs : array<float>
    @ssbo @binding = 1 ys : array<float>
    @uniform @binding = 2 ka : UkArgs

    [metal_kernel(name="ukargs_msl"), marker(no_coverage)]
    def ukargs {
        let i = gl_GlobalInvocationID.x
        if (i >= ka.n) {
            return
        }
        ys[i] = xs[i] * ka.gain + float(ka.bias)
    }
}

// ===== Phase 0: class methods + inheritance (FLATTENED — bodies splice at call sites) =====

class UfxBase {
    @ssbo @binding = 0 xs : array<float>
    @ssbo @binding = 1 ys : array<float>
    @uniform @binding = 2 gain : float

    def stage(v : float) : float => v * gain
    def pick(i : uint) : float => xs[i]
    def combo(i : uint) : float => stage(pick(i))   // base skeleton calling the overridable stage
}

class UfxKid : UfxBase {
    @ssbo @binding = 3 zs : array<float>
    @workgroup tmp : float[16]
    cur : float const?      // plain member = STATE: carried-pointer rider (the Q8 walk shape)
    boost : float = 0.25    // scalar state with a constant initializer

    def override pick(i : uint) : float => xs[i] + zs[i]   // what combo must actually call

    def store(i : uint; v : float) {   // member write through a method body
        ys[i] = v
    }

    def seed(i : uint) {    // the stage_init shape: bind the carried pointer once
        cur = unsafe(addr(xs[i]))
    }

    def advance {           // per-iteration advance — state lives ACROSS splices
        cur = unsafe(cur + 1)
        boost += 0.5
    }

    def peek : float => unsafe(cur[0]) * boost

    [metal_kernel(name="ufxcls_msl"), marker(no_coverage)]
    def ufxcls {
        let i = gl_GlobalInvocationID.x
        let lid = gl_LocalInvocationID.x
        tmp[lid] = combo(i)
        barrier()
        seed(i)
        advance()
        store(i, (tmp[lid] + ufx_sky(xs[i])) + peek())
    }
}

// The remaining state-member kinds: a fixed-ARRAY state member (a kernel-entry thread-local
// array — the per-thread scratch a rider carries across splices) and a MUTABLE-pointee pointer
// rider. Taking a writable pointer into a buffer IS the write declaration (no deref tracking),
// so `out_` lowers non-const from the binding alone.
class StateArr {
    @ssbo @binding = 0 xs : array<float>
    @ssbo @binding = 1 out_ : array<float>
    hist : float[4]
    wcur : float?

    def load(i : uint) {
        for (k in range(4)) {
            hist[k] = xs[i + uint(k)]
        }
    }

    def bind_out(i : uint) {
        wcur = unsafe(addr(out_[i]))
    }

    def flush {
        unsafe(wcur[0]) = ((hist[0] + hist[1]) + hist[2]) + hist[3]
    }

    [metal_kernel(name="statearr_msl"), marker(no_coverage)]
    def statearr {
        let i = gl_GlobalInvocationID.x
        load(i)
        bind_out(i)
        flush()
    }
}

// The DERIVED companion-name form: no `name=` on [metal_kernel], so the globals are
// <Class>_<method>_msl and the MSL entry point is the qualified <Class>_<method> — which is what
// keeps a stamped class's twins distinguishable in a GPU capture. Every other fixture here
// passes name=, so this is the only one covering the derived path.
class DerivedName {
    @ssbo @binding = 0 xs : array<float>
    @ssbo @binding = 1 ys : array<float>

    [metal_kernel, marker(no_coverage)]
    def dkern {
        let i = gl_GlobalInvocationID.x
        ys[i] = xs[i] * 2.0
    }
}

//! Gate B: exactly the construct kinds the fixture set must emit — the census meta-test
//! asserts equality with the observed union in both directions.
def public declared_msl_census : table<string> {   // nolint:STYLE038 — one flat literal per construct kind; the gate-B contract IS this list
    return <- {
        // signature + parameters
        "preamble",
        "kernel_sig",
        "param.buffer",
        "param.buffer_const",
        "param.uniform",
        "param.buffer_packed",
        "param.buffer_const_packed",
        "param.uniform_packed",
        "param.builtin.thread_position_in_grid",
        "param.builtin.thread_position_in_threadgroup",
        "param.builtin.threadgroup_position_in_grid",
        "param.builtin.threadgroups_per_grid",
        "param.builtin.threads_per_threadgroup",
        "param.builtin.thread_index_in_threadgroup",
        "param.builtin.thread_index_in_simdgroup",
        "param.builtin.threads_per_simdgroup",
        "param.builtin.simdgroups_per_threadgroup",
        "param.builtin.simdgroup_index_in_threadgroup",
        // threadgroup memory + synchronization
        "decl.shmem_ptr",
        "decl.shmem_ptr_array",
        "param.shmem",
        "call.barrier",
        "call.memory_barrier_shared",
        // simdgroup intrinsics
        "call.simd_sum.f32",
        "call.simd_sum.i32",
        "call.simd_sum.u32",
        "call.simd_shuffle.f32",
        "call.simd_shuffle.i32",
        "call.simd_shuffle.u32",
        "call.simd_shuffle_up.f32",
        "call.simd_shuffle_up.i32",
        "call.simd_shuffle_up.u32",
        "call.simd_shuffle_down.f32",
        "call.simd_shuffle_down.i32",
        "call.simd_shuffle_down.u32",
        "call.simd_shuffle_xor.f32",
        "call.simd_shuffle_xor.i32",
        "call.simd_shuffle_xor.u32",
        // values
        "expr.builtin.thread_position_in_grid",
        "expr.builtin.thread_position_in_threadgroup",
        "expr.builtin.threadgroup_position_in_grid",
        "expr.builtin.threadgroups_per_grid",
        "expr.builtin.threads_per_threadgroup",
        "expr.builtin.thread_index_in_threadgroup",
        "expr.builtin.thread_index_in_simdgroup",
        "expr.builtin.threads_per_simdgroup",
        "expr.builtin.simdgroups_per_threadgroup",
        "expr.builtin.simdgroup_index_in_threadgroup",
        "expr.var",
        "expr.member",
        "expr.member.packed",
        "expr.index",
        "expr.index.packed",
        "expr.swizzle",
        "store.packed",
        "const.int",
        "const.uint",
        "const.float",
        "const.float16",
        "const.bool",
        "const.i32v",
        "const.u32v",
        "const.f32v",
        // uint scalar arithmetic + bitwise
        "op.add.u32",
        "op.sub.u32",
        "op.mul.u32",
        "op.div.u32",
        "op.mod.u32",
        "op.band.u32",
        "op.bor.u32",
        "op.bxor.u32",
        "op.bnot.u32",
        "op.shl.u32",
        "op.shr.u32",
        // int scalar arithmetic + bitwise
        "op.add.i32",
        "op.sub.i32",
        "op.mul.i32",
        "op.div.i32",
        "op.mod.i32",
        "op.band.i32",
        "op.bor.i32",
        "op.bxor.i32",
        "op.bnot.i32",
        "op.shl.i32",
        "op.shr.i32",
        "op.neg.i32",
        // float scalar arithmetic
        "op.add.f32",
        "op.sub.f32",
        "op.mul.f32",
        "op.div.f32",
        "op.mod.f32",
        "op.neg.f32",
        // vector arithmetic
        "op.add.f32v",
        "op.sub.f32v",
        "op.mul.f32v",
        "op.div.f32v",
        "op.neg.f32v",
        "op.add.i32v",
        "op.sub.i32v",
        "op.mul.i32v",
        "op.eq.i32v",
        "op.ne.i32v",
        "op.add.u32v",
        "op.mul.u32v",
        // fp16 closed arithmetic (scalar float16 + half vectors)
        "op.add.f16",
        "op.mul.f16",
        "op.lt.f16",
        "op.add.f16v",
        "op.sub.f16v",
        "op.mul.f16v",
        "op.div.f16v",
        "op.neg.f16v",
        "op.eq.f16v",
        "op.ne.f16v",
        // ctors + lattice converts (widen / truncating narrow / clamp-based saturating narrow)
        "ctor.f16v",
        "ctor.splat.f16v",
        "cvt.f16v.f32v",
        "cvt.f32v.f16v",
        "cvt.i8v.i32v",
        "cvt.i8v.i16v",
        "cvt.i32v.i8v",
        "cvt.u8v.u32v",
        "cvt.u8v.u16v",
        "cvt.i32.i16",
        "cvt.i16.i32",
        "cvt.i8.i32",
        "cvt.i32.f32",
        "cvt.i32.u32",
        "cvt_sat.i32v.i8v",
        "cvt_sat.u32v.u16v",
        // comparisons
        "op.eq.u32",
        "op.ne.u32",
        "op.lt.u32",
        "op.le.u32",
        "op.gt.u32",
        "op.ge.u32",
        "op.eq.i32",
        "op.ne.i32",
        "op.lt.i32",
        "op.le.i32",
        "op.gt.i32",
        "op.ge.i32",
        "op.eq.f32",
        "op.ne.f32",
        "op.lt.f32",
        "op.le.f32",
        "op.gt.f32",
        "op.ge.f32",
        // logical + ternary + increment/decrement
        "op.land",
        "op.lor",
        "op.lnot",
        "op.select",
        "op.preinc",
        "op.postinc",
        "op.predec",
        "op.postdec",
        // compound assignment
        "op.add_assign.u32",
        "op.sub_assign.u32",
        "op.mul_assign.u32",
        "op.div_assign.u32",
        "op.mod_assign.u32",
        "op.band_assign.u32",
        "op.bor_assign.u32",
        "op.bxor_assign.u32",
        "op.shl_assign.u32",
        "op.shr_assign.u32",
        "op.add_assign.i32",
        "op.add_assign.f32",
        "op.mul_assign.f32",
        // simdgroup_matrix (Phase 5): locals, fills, loads (device + threadgroup), stores,
        // multiply, multiply-accumulate (all-f32, all-f16, and the mixed f32-acc/f16-operand combo)
        "stmt.var.sgmat",
        "stmt.var.sgmat.zero",
        "stmt.var.sgmat_array.f32",
        "stmt.var.sgmat_array.f16",
        "stmt.var.array",
        "stmt.for.unroll",
        "decl.uniform_struct",
        "expr.member.ustruct",
        "decl.ptr.tg",
        "decl.ptr.dev",
        "expr.addr.tg",
        "expr.addr.dev",
        "op.ptr_add",
        "call.sgmat_load_ptr.f16",
        "call.sgmat_fill.f32",
        "call.sgmat_fill.f16",
        "call.sgmat_load.f32",
        "call.sgmat_load.f16",
        "call.sgmat_load_tg.f16",
        "call.sgmat_store.f32",
        "call.sgmat_store.f16",
        "call.sgmat_mul.f32",
        "call.sgmat_mul.f16",
        "call.sgmat_mac.f32",
        "call.sgmat_mac.f16",
        "call.sgmat_mac.mixed",
        // math builtins (Phase 6): 1:1 MSL spellings — float scalar + vector classes
        // (abs also signed int; min/max any numeric class)
        "call.math.exp.f32",
        "call.math.log.f32",
        "call.math.sqrt.f32",
        "call.math.rsqrt.f32",
        "call.math.sin.f32",
        "call.math.cos.f32",
        "call.math.tanh.f32",
        "call.math.abs.f32",
        "call.math.saturate.f32",
        "call.math.min.f32",
        "call.math.max.f32",
        "call.math.exp.f32v",
        "call.math.sqrt.f32v",
        "call.math.rsqrt.f32v",
        "call.math.sin.f32v",
        "call.math.cos.f32v",
        "call.math.tanh.f32v",
        "call.math.abs.f32v",
        "call.math.saturate.f32v",
        "call.math.min.f32v",
        "call.math.max.f32v",
        "call.math.abs.i32",
        "call.math.min.i32",
        "call.math.max.i32",
        "call.math.min.u32",
        "call.math.max.u32",
        "call.math.round.f32",
        "call.math.round.f32v",
        "cvt.f32.i8",
        "cvt.i8.f32",
        // statements
        "stmt.let",
        "stmt.var",
        "stmt.var.zero",
        "stmt.assign",
        "stmt.if",
        "stmt.elif",
        "stmt.else",
        "stmt.while",
        "stmt.for",
        "stmt.break",
        "stmt.continue",
        "stmt.return",
        // Phase 0: user-function lowering
        "decl.user_fn",
        "call.user",
        "stmt.call_user",
        "stmt.return_value",
        "ufn.param.buffer",
        "ufn.param.buffer_const",
        "ufn.param.tg",
        "ufn.param.tg_const",
        "ufn.param.ptr.device",
        "ufn.param.ptr.device_const",
        "ufn.param.ptr.threadgroup_const",
        "ufn.param.ref",
        "ufn.param.value",
        "ufn.param.hidden_builtin",
        // Phase 0: class methods + inheritance (flattened — bodies splice at call sites,
        // plain members are kernel-entry thread-locals)
        "splice.method",
        "splice.method.value",
        "splice.param.hoist",
        "decl.state",
        "decl.state.arr",
        "decl.state.ptr"
    }
}

//! Every fixture kernel's census companion, for the census meta-test's union.
def public all_msl_censuses : array<string> {
    return <- [mul_ab_msl_census, uarith_msl_census, iarith_msl_census, farith_msl_census,
        vecarith_msl_census, control_msl_census, loops_msl_census, reduce_msl_census, simd_msl_census,
        halfarith_msl_census, latconv_msl_census, packed3_msl_census, sgmat_msl_census,
        sgmat_tiled_msl_census, sgmat_rolled_msl_census, mathcalls_msl_census, userfns_msl_census,
        ufxcls_msl_census, ukargs_msl_census, ukstride_msl_census, statearr_msl_census,
        DerivedName_dkern_msl_census]
}
