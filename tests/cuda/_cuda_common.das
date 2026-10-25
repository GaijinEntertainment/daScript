options gen2
options indenting = 4

// Shared fixture for the dasCUDA emitter suite (the tests/msl/_msl_common shape): the
// [cuda_kernel] classes whose emitted text the shape, compile-gate, census and oracle tests
// read, plus the declared census - the contract the fixture union must equal in both
// directions. [marker(no_coverage)] keeps daslib/coverage off the kernel bodies: they are
// compile-time CUDA source, not runtime code.

module _cuda_common shared public

require cuda/cuda_kernel public
require cuda/cuda_intrinsics public
require cuda/das_cuda_boost public
require math
require daslib/math_bits
require daslib/templates
require spirv/spirv_shader

class MulAB {
    @ssbo @binding = 0 a : array<float>
    @ssbo @binding = 1 b : array<float>
    @ssbo @binding = 2 c : array<float>

    [cuda_kernel(name="mul_ab_cuda", local_size_x = 64), marker(no_coverage)]
    def mul_ab {
        let i = gl_GlobalInvocationID.x
        c[i] = a[i] * b[i]
    }
}

struct AxpyArgs {
    nelem : uint
    scale : float
    zero : uint
}

let AXPY_ZERO_FLOOR = 0.0   //! a module `let` the kernel reads: the emitter folds it to its constant

//! the @kargs struct, a method the kernel calls, the bounds guard, a module constant
class Axpy {
    @ssbo @binding = 0 o : array<float>
    @ssbo @binding = 1 y : array<float>
    @kargs ka : AxpyArgs

    def axpy_body {
        let i = gl_GlobalInvocationID.x
        return if (i >= ka.nelem)
        let base = ka.zero != 0u ? AXPY_ZERO_FLOOR : o[i]
        o[i] = base + ka.scale * y[i]
    }

    [cuda_kernel(name="axpy_cuda", local_size_x = 256), marker(no_coverage)]
    def run {
        axpy_body()
    }
}

class UArith {
    @ssbo @binding = 0 a : array<uint>
    @ssbo @binding = 1 b : array<uint>
    @ssbo @binding = 2 c : array<uint>
    @ssbo @binding = 3 d : array<uint>

    [cuda_kernel(name="uarith_cuda", local_size_x = 64), marker(no_coverage)]
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

    [cuda_kernel(name="iarith_cuda", local_size_x = 64), marker(no_coverage)]
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

    [cuda_kernel(name="farith_cuda", local_size_x = 64), marker(no_coverage)]
    def farith {
        let i = gl_GlobalInvocationID.x
        let x = a[i]
        let y = b[i] + 1.5
        c[i] = ((((x + y) - (x * y)) + (x / y)) + (x % y)) + (-x)
    }
}

//! `precise = true`: every scalar f32 add, subtract, multiply and divide, the compound forms included, through the
//! correctly rounded intrinsics - the stored bytes are the CPU's
class Precise {
    @ssbo @binding = 0 a : array<float>
    @ssbo @binding = 1 b : array<float>
    @ssbo @binding = 2 c : array<float>

    [cuda_kernel(name="precise_cuda", local_size_x = 64, precise = true), marker(no_coverage)]
    def precise {
        let i = gl_GlobalInvocationID.x
        let x = a[i]
        let y = b[i]
        var acc = (x * y) + (x - y)
        acc += x / y
        acc -= y
        acc *= x
        acc /= 1.5
        c[i] = acc
    }
}

//! componentwise float4/int2 infix, vector-scalar broadcast, vector negate, das's scalar-bool
//! vector ==/!=, swizzles, the ctor, splat and convert forms
class VecArith {
    @ssbo @binding = 0 fa : array<float4>
    @ssbo @binding = 1 fb : array<float4>
    @ssbo @binding = 2 fc : array<float4>
    @ssbo @binding = 3 ia : array<int2>
    @ssbo @binding = 4 ic : array<int2>
    @ssbo @binding = 5 fl : array<float>
    @ssbo @binding = 6 f2 : array<float2>

    [cuda_kernel(name="vecarith_cuda", local_size_x = 64), marker(no_coverage)]
    def vecarith {
        let i = gl_GlobalInvocationID.x
        let x = fa[i]
        let y = fb[i]
        var v = ((x + y) - (x * y)) + (x / (y + float4(1.5)))
        v = v * 2.0 + (-x)
        v += float4(x.x, y.y, x.z, y.w) + float4(y.w)
        fc[i] = v
        let m = ia[i]
        let n = int2(3, 5)
        var w = ((m + n) - (m * n)) + (m / n)
        w = (w & n) | (w ^ n)
        w = w << 1
        w = -w
        let wv = w
        ic[i] = int2(float2(w)) + int2(wv.yx)
        fl[i] = (x == y ? 1.0 : 0.0) + (x != y ? 2.0 : 0.0) + x.w + x.xyz.z
        f2[i] = x.xy + y.zw
    }
}

//! mutable and zero-init locals, every compound assignment, pre/post increment, every comparison per
//! class, logical ops, if/elif/else chains, while, the ternary
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

    [cuda_kernel(name="control_cuda", local_size_x = 64), marker(no_coverage)]
    def control {   // nolint:STYLE037,STYLE038 - one kernel per construct family: every branch and loop shape in one text the shape test reads
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
        let snap = acc++
        let dnap = acc--
        zed += acc + snap + dnap
        out1[i] = hot ? zed : zed + 7u
        var m = p[i]
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
        var k = 0
        while (k < 3 && !(m > 100)) {
            iacc += k
            k++
            m++
        }
        out2[i] = iacc
        var fv = f[i]
        let gv = g[i]
        var facc = 0.0
        if (fv > gv) {
            facc += 1.0   // nolint:PERF013 - the compound add-assign on a float is the construct under test
        }
        if (fv < gv) {
            facc += 2.0
        }
        if (fv >= gv) {
            facc += 4.0
        }
        if (fv <= gv) {
            facc += 8.0
        }
        if (fv == gv) {
            facc += 16.0
        }
        if (fv != gv) {
            facc += 32.0
        }
        fv += 1.0   // nolint:PERF013 - the compound add-assign on a float is the construct under test
        fv -= 0.5
        fv *= 2.0
        fv /= 4.0
        fv %= 3.0
        for (j in range(3)) {
            facc += float(j)
            if (j == 1) {
                continue
            }
            if (j == 2) {
                break
            }
        }
        for (j in range(1, int(x & 3u) + 2)) {   // nolint:LINT002 - the hoisted-bound loop; the shape test names j
            facc += 0.5
        }
        for [unroll] (j in range(2)) {
            facc += float(j) * 0.25
        }
        out3[i] = facc + fv
    }
}

//! the math whitelist on float, the int twins, the lane-wise float4 forms, the bitcasts
class MathCalls {
    @ssbo @binding = 0 a : array<float>
    @ssbo @binding = 1 b : array<float>
    @ssbo @binding = 2 c : array<float>
    @ssbo @binding = 3 ia : array<int>
    @ssbo @binding = 4 ic : array<int>
    @ssbo @binding = 5 va : array<float4>
    @ssbo @binding = 6 vc : array<float4>
    @ssbo @binding = 7 ua : array<uint>
    @ssbo @binding = 8 uc : array<uint>

    [cuda_kernel(name="mathcalls_cuda", local_size_x = 64), marker(no_coverage)]
    def mathcalls {
        let i = gl_GlobalInvocationID.x
        let x = abs(a[i]) + 1.0
        let y = b[i]
        var r = exp(x) + log(x) + exp2(x) + log2(x) + sqrt(x) + rsqrt(x) + sin(x) + cos(x) + tanh(x)
        r += saturate(y) + round(y) + floor(y) + ceil(y) + atan(y) + atan2(y, x) + pow(x, 2.0) + fmod(y, 1.5)
        r += min(x, y) + max(x, y) + clamp(y, 0.0, 1.0) + mad(x, y, 1.0)
        c[i] = r
        let m = ia[i]
        ic[i] = abs(m) + min(m, 3) + max(m, -3) + clamp(m, -2, 2)
        let v = va[i]
        vc[i] = exp(v) + sqrt(abs(v)) + min(v, float4(1.0)) + clamp(v, float4(0.0), float4(1.0)) + mad(v, v, v)
        uc[i] = float_bits_to_uint(a[i]) + uint(int_bits_to_float(ia[i])) + ua[i] + float_bits_to_uint(uint_bits_to_float(ua[i]))
    }
}

struct ReduceArgs {
    n : uint
}

//! @workgroup memory, barrier(), the subgroup set, the subgroup and block IDs
class Reduce {
    @ssbo @binding = 0 x : array<float>
    @ssbo @binding = 1 o : array<float>
    @ssbo @binding = 2 oi : array<int>
    @workgroup part : float[8]
    @workgroup flag : uint
    @kargs ka : ReduceArgs

    def lane_value : float {
        let i = gl_GlobalInvocationID.x
        return i < ka.n ? x[i] * 2.0 : 0.0
    }

    def stash(v : float) {
        part[gl_SubgroupID] = v
    }

    [cuda_kernel(name="reduce_cuda", local_size_x = 256), marker(no_coverage)]
    def reduce {
        let v = lane_value()
        let s = subgroupAdd(v)
        let mx = subgroupMax(v)
        let mn = subgroupMin(v)
        let first = subgroupBroadcastFirst(v)
        let bc = subgroupBroadcast(v, 3u)
        let sh = subgroupShuffle(v, gl_SubgroupInvocationID ^ 1u)
        let shx = subgroupShuffleXor(v, 16u)
        let cl = subgroupClusteredAdd(v, 4u)
        let any = subgroupAny(v > 0.0)
        let all = subgroupAll(v >= 0.0)
        if (subgroupElect()) {
            stash(s + mx + mn + first + bc + sh + shx + cl + (any ? 1.0 : 0.0) + (all ? 2.0 : 0.0))
        }
        if (gl_LocalInvocationIndex == 0u) {
            flag = gl_NumSubgroups + gl_SubgroupSize
        }
        barrier()
        if (gl_LocalInvocationID.x == 0u) {
            var tot = 0.0
            for (w in range(8)) {
                tot += part[w]
            }
            o[gl_WorkGroupID.x] = tot + float(gl_WorkGroupSize.x) + float(gl_NumWorkGroups.x)
            oi[gl_WorkGroupID.x] = int(flag)
        }
        memoryBarrierShared()
        memoryBarrierBuffer()
    }
}

struct DivergeArgs {
    n : uint   //! the live lanes of every warp: the rest skip the guarded block
}

//! a lane-pair shuffle, the first broadcast and the elect inside a guard the upper lanes of every warp skip - each reads
//! the lanes executing the call (the active mask), never the whole warp
class Diverge {
    @ssbo @binding = 0 x : array<float>
    @ssbo @binding = 1 o : array<float>
    @kargs ka : DivergeArgs

    [cuda_kernel(name="diverge_cuda", local_size_x = 64), marker(no_coverage)]
    def run {
        let lane = gl_SubgroupInvocationID
        let i = gl_GlobalInvocationID.x
        if (lane < ka.n) {
            let v = x[i]
            let pair = v + subgroupShuffleXor(v, 1u)
            let first = subgroupBroadcastFirst(v)
            let lead = subgroupElect() ? 100.0 : 0.0
            o[i] = pair + first + lead
        }
    }
}

//! float16 buffers through the prelude's half: load, arithmetic, compare, store, the half4 vector
class HalfArith {
    @ssbo @binding = 0 a : array<float16>
    @ssbo @binding = 1 b : array<float16>
    @ssbo @binding = 2 c : array<float16>
    @ssbo @binding = 3 f : array<float>
    @ssbo @binding = 4 va : array<half4>
    @ssbo @binding = 5 vf : array<float4>

    [cuda_kernel(name="halfarith_cuda", local_size_x = 64), marker(no_coverage)]
    def halfarith {
        let i = gl_GlobalInvocationID.x
        let x = a[i]
        let y = b[i]
        var h = (x + y) * y - x / float16(2.0)
        h += float16(0.5)
        h = -h
        c[i] = x > y ? h : y
        f[i] = float(h) + float(x < y ? float16(1.0) : float16(0.0))
        let v = float4(va[i])
        vf[i] = v * 2.0
        va[i] = half4(v + float4(1.0))
    }
}

//! free functions: buffer, shared (written and read-only), ref, pointer (mutable and const pointee) and value parameters,
//! a value return, the builtin reach, a generic twin pair
def ufx_scale(d : float) : float => d * 3.0 + 1.0

def ufx_lane : uint => gl_GlobalInvocationID.x

def ufx_splat(var yo : array<float>; i : uint; v : float) {
    yo[i] = v
}

def ufx_bump(var acc : float&; by : float) {
    acc += by
}

def ufx_pick(src : array<auto(TT)>; i : uint) : float => float(src[i])

def ufx_part(var sm : float[64]; k : uint; v : float) {
    sm[k] = v
}

def ufx_read(sm : float[64]; k : uint) : float => sm[k]

def ufx_at(var p : float?; v : float) {
    *p += v
}

def ufx_peek(p : float const?) : float => *p

class UserFns {
    @ssbo @binding = 0 xs : array<float>
    @ssbo @binding = 1 hs : array<float16>
    @ssbo @binding = 2 yo : array<float>
    @workgroup sm : float[64]

    [cuda_kernel(name="userfns_cuda", local_size_x = 64), marker(no_coverage)]
    def userfns {
        let i = ufx_lane()
        var acc = ufx_scale(xs[i])
        ufx_bump(acc, ufx_pick(xs, i) + ufx_pick(hs, i))
        ufx_part(sm, gl_LocalInvocationID.x, acc)
        barrier()
        ufx_splat(yo, i, ufx_read(sm, gl_LocalInvocationID.x ^ 1u))
        ufx_at(unsafe(addr(yo[i])), ufx_peek(unsafe(addr(xs[i]))))
    }
}

//! atomics on uint, int and float memory
class Atomics {
    @ssbo @binding = 0 cnt : array<uint>
    @ssbo @binding = 1 sum : array<float>
    @ssbo @binding = 2 ext : array<int>

    [cuda_kernel(name="atomics_cuda", local_size_x = 64), marker(no_coverage)]
    def atomics {
        let i = gl_GlobalInvocationID.x
        atomicAdd(cnt[0], 1u)
        let old = atomicAdd(sum[0], 1.5)
        atomicMax(ext[0], int(i))
        atomicMin(ext[1], int(i))
        atomicOr(cnt[1], 1u << (i & 31u))
        atomicAnd(cnt[2], ~(1u << (i & 31u)))
        atomicXor(cnt[3], 1u)
        atomicExchange(cnt[4], i)
        atomicCompSwap(cnt[5], 0u, i)
        if (old < 0.0) {
            cnt[6] = 1u
        }
    }
}

//! a second `tab` of other values: the kernel's own takes a suffixed table name
def ctab_pick(k : uint) : float {
    let tab = fixed_array<float>(2.0, 4.0, 6.0, 8.0)
    return tab[k & 3u]
}

//! a let fixed-array of literals hoists to a __constant__ table; a var fixed array stays a local
class ConstTab {
    @ssbo @binding = 0 idx : array<uint>
    @ssbo @binding = 1 o : array<float>

    [cuda_kernel(name="consttab_cuda", local_size_x = 64), marker(no_coverage)]
    def consttab {
        let i = gl_GlobalInvocationID.x
        let tab = fixed_array<float>(0.5, 1.5, 2.5, 3.5)
        let itab = fixed_array<int>(1, 2, 3, 4, 5, 6, 7, 8, 9)
        var scratch : float[4]
        scratch[idx[i] & 3u] = tab[idx[i] & 3u]
        o[i] = scratch[0] + scratch[1] + scratch[2] + scratch[3] + float(itab[idx[i] % 9u]) + ctab_pick(idx[i])
    }
}

//! the 8/16-bit integer lattice: buffer elements, widening, the saturating narrows, the packed dots
class Lattice {
    @ssbo @binding = 0 bs : array<int8>
    @ssbo @binding = 1 us : array<uint16>
    @ssbo @binding = 2 b4 : array<byte4>
    @ssbo @binding = 3 u2 : array<ubyte2>
    @ssbo @binding = 4 ob : array<int8>
    @ssbo @binding = 5 o4 : array<byte4>
    @ssbo @binding = 6 s2 : array<ushort2>
    @ssbo @binding = 7 pk : array<uint>
    @ssbo @binding = 8 od : array<int>

    [cuda_kernel(name="lattice_cuda", local_size_x = 64), marker(no_coverage)]
    def lattice {
        let i = gl_GlobalInvocationID.x
        let w = int(bs[i]) * 3 + int(us[i])
        ob[i] = int8(w & 127)
        let w4 = int4(b4[i])
        o4[i] = byte4_sat((w4 * 40) + w4)
        let uw = uint2(u2[i])
        s2[i] = ushort2_sat(uw * 1000u)
        od[i] = sdot4(pk[i], pk[i] ^ 0x01020304u) + usdot4(pk[i], pk[i])
    }
}

//! pointer locals over members and pointer arithmetic
class Ptrs {
    @ssbo @binding = 0 src : array<float>
    @ssbo @binding = 1 dst : array<float>

    [cuda_kernel(name="ptrs_cuda", local_size_x = 64), marker(no_coverage)]
    def ptrs {
        let i = gl_GlobalInvocationID.x
        var p = unsafe(addr(src[i * 2u]))
        var q = unsafe(addr(dst[i]))
        let a = *p
        p = unsafe(p + 1)
        *q = a + *p
    }
}

//! two members viewing one binding through their own element types: every view read-only, the
//! owner written, the alias written - and a method reaching an aliased view; the float2 view reads
//! from element 128 up, the upper half of the buffer the float view writes below (a kernel reads no module global)
class Alias {
    @ssbo @binding = 0 wq : array<uint>
    @ssbo @binding = 0 wq4 : array<uint4>
    @ssbo @binding = 1 o : array<float>
    @ssbo @binding = 1 o2 : array<float2>
    @ssbo @binding = 2 p : array<uint>
    @ssbo @binding = 2 p4 : array<uint4>

    def pair_sum(i : uint) : float {
        let v = wq4[i / 4u]
        let t = o2[128u + i / 2u]
        return float(v.x + v.y + v.z + v.w) + t.x + t.y
    }

    [cuda_kernel(name="alias_cuda", local_size_x = 64), marker(no_coverage)]
    def alias {
        let i = gl_GlobalInvocationID.x
        o[i] = pair_sum(i) + float(wq[i])
        p4[i] = uint4(p[4u * i], p[4u * i + 1u], p[4u * i + 2u], p[4u * i + 3u]) + uint4(1u, 2u, 3u, 4u)
    }
}

struct Blk {
    qs : int16[8]
    sc : float
}

struct Meta {
    pos : uint
    cnt : uint
    blk : Blk
}

struct TagA {
    unused : int
}

[template(fmt), unused_argument(fmt)]
def tf_scale(fmt : TagA; x : uint) : uint => x * 3u

//! struct buffer elements (a fixed-array field, a nested struct), a struct local, a lane of a
//! vector by index read and written, a loop kept rolled, a [template] instance called by its base name
class Blocks {
    @ssbo @binding = 0 blk : array<Blk>
    @ssbo @binding = 1 meta : array<Meta>
    @ssbo @binding = 2 w4 : array<uint4>
    @ssbo @binding = 3 o : array<float>

    [cuda_kernel(name="blocks_cuda", local_size_x = 64), marker(no_coverage)]
    def blocks {
        let i = gl_GlobalInvocationID.x
        let b = blk[i]
        var acc = 0.0
        for [dont_unroll] (k in range(8)) {
            acc += float(b.qs[k])
        }
        let m = meta[i].blk
        let lane = w4[i][int(meta[i].pos & 3u)]
        var v = w4[i]
        v[int(meta[i].cnt & 3u)] = tf_scale(type<TagA>, lane)
        o[i] = acc * blk[i].sc + m.sc + float(meta[i].cnt) + float(v.x + v.y + v.z + v.w)
    }
}

//! the pack family: packHalf2x16 / unpackHalf2x16, unpack8 at both widths and signs, pack32
class Packs {
    @ssbo @binding = 0 f : array<float2>
    @ssbo @binding = 1 u : array<uint>
    @ssbo @binding = 2 o : array<float2>
    @ssbo @binding = 3 ob : array<int>
    @ssbo @binding = 4 ou : array<uint>

    [cuda_kernel(name="packs_cuda", local_size_x = 64), marker(no_coverage)]
    def packs {
        let i = gl_GlobalInvocationID.x
        let h = packHalf2x16(f[i])
        o[i] = unpackHalf2x16(h)
        let b4 = unpack8(int(u[i]))
        let ub4 = unpack8(u[i])
        let b2 = unpack8(int16(u[i] & 0xFFFFu))
        let ub2 = unpack8(uint16(u[i] >> 16u))
        ob[i] = pack32(b4) + int(b2.x) + int(b2.y)
        ou[i] = pack32(ub4) + uint(ub2.x) + uint(ub2.y)
    }
}

//! the KHR 16x16 tiles: the f16 loads in both layouts, the s8 pair in both, the three accumulators, the
//! f16 -> f32 convert, the stores in both layouts and the f16 store into pair staging
class Coop {
    @ssbo @binding = 0 a : array<float16>    //! A 16x16 row-major: (r, k) at r * 16 + k
    @ssbo @binding = 1 b : array<float16>    //! B 16x16 column-major: (k, n) at n * 16 + k
    @ssbo @binding = 2 o : array<float>      //! four 16x16 f32 tiles
    @ssbo @binding = 3 a8 : array<int8>      //! A 16x32 row-major
    @ssbo @binding = 4 b8 : array<int8>      //! B 32x16 column-major: (k, n) at n * 32 + k
    @ssbo @binding = 5 o32 : array<int>      //! two 16x16 s32 tiles
    @ssbo @binding = 6 a8t : array<int8>     //! A transposed: (c, r) at c * 16 + r - the column-major A read
    @ssbo @binding = 7 b8r : array<int8>     //! B row-major: (k, n) at k * 16 + n
    @workgroup sh : uint[128]                //! a 16x16 f16 tile as pairs, row stride 8

    [cuda_kernel(name="coop_cuda", local_size_x = 32), marker(no_coverage)]
    def coop {
        var ar, ac : coopmatA_f16_16x16
        var br, bc : coopmatB_f16_16x16
        coopmatLoad(ar, a, 0u, 16, 0)
        coopmatLoad(ac, a, 0u, 16, 1)
        coopmatLoad(bc, b, 0u, 16, 1)
        coopmatLoad(br, b, 0u, 16, 0)
        var c0 : coopmatAcc_f32_16x16
        c0 = coopmatMulAdd(ar, bc, c0)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
        coopmatStore(c0, o, 0, 16, 0)
        var c1 : coopmatAcc_f32_16x16
        c1 = coopmatMulAdd(ac, br, c1)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
        coopmatStore(c1, o, 256, 16, 1)
        var c2 : coopmatAcc_f16_16x16
        c2 = coopmatMulAdd(ar, bc, c2)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
        var c2f : coopmatAcc_f32_16x16
        coopmatConvert(c2f, c2)
        coopmatStore(c2f, o, 512, 16, 0)
        coopmatStore(c2, sh, 0, 8, 0)
        var a8t_ : coopmatA_s8_16x32
        var b8t_ : coopmatB_s8_32x16
        coopmatLoad(a8t_, a8, 0u, 32, 0)
        coopmatLoad(b8t_, b8, 0u, 32, 1)
        var s0 : coopmatAcc_s32_16x16
        s0 = coopmatMulAdd(a8t_, b8t_, s0)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
        coopmatStore(s0, o32, 0, 16, 0)
        var a8c : coopmatA_s8_16x32
        var b8r_ : coopmatB_s8_32x16
        coopmatLoad(a8c, a8t, 0u, 16, 1)
        coopmatLoad(b8r_, b8r, 0u, 16, 0)
        var s1 : coopmatAcc_s32_16x16
        s1 = coopmatMulAdd(a8c, b8r_, s1)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
        coopmatStore(s1, o32, 256, 16, 1)
        barrier()
        let lane = gl_SubgroupInvocationID
        for (j in range(4)) {
            let u = lane * 4u + uint(j)
            let v = unpackHalf2x16(sh[u])
            o[768u + 2u * u] = v.x
            o[768u + 2u * u + 1u] = v.y
        }
    }
}

[spirv_combine]
def cm2_max(a, b : float) : float => max(a, b)

[spirv_combine]
def cm2_add(a, b : float) : float => a + b

//! v + r - c x bias: the row and column reach the callback
[spirv_per_element]
def cm2_bias(r, c : uint; v : float; bias : float) : float => v + float(r) - float(c) * bias

//! the larger of the element and its coopmat extra's
[spirv_per_element, unused_argument(r, c)]
def cm2_pick(r, c : uint; v, o : float) : float => max(v, o)

//! the workgroup-scope tiles over four warps: a 64x64 f16 GEMM through the transposed B view, the
//! per-element, copy, scale, clamp, add, sub, Hadamard and reduce forms, the f16 accumulator, the
//! accumulator-to-A convert feeding a second product, a filled B tile, the pad-clamped store
class Cm2 {
    @ssbo @binding = 0 a : array<float16>    //! A 64x64 row-major: (r, k) at r * 64 + k
    @ssbo @binding = 1 bt : array<float16>   //! B transposed: (n, k) at n * 64 + k
    @ssbo @binding = 2 c : array<float>      //! the worked product, rows 60..63 left as uploaded (the pad layout)
    @ssbo @binding = 3 o2 : array<float16>   //! the f16-accumulated product
    @ssbo @binding = 4 red : array<float>    //! [0, 4096) row max of c, [4096, 8192) its column sum, broadcast
    @ssbo @binding = 5 e : array<float>      //! P B, P the worked product as an f16 A tile
    @ssbo @binding = 6 rs : array<float>     //! P x ones: the row sums of P broadcast
    @ssbo @binding = 7 t : array<float>      //! ((c + cc) - cc) x itself

    [cuda_kernel(name="cm2_cuda", local_size_x = 128), marker(no_coverage)]
    def cm2 {
        var tl : tensorLayout2D
        tensorLayoutCreate(tl)
        tensorLayoutSetDimension(tl, 64u, 64u)
        tensorLayoutSetStride(tl, 64u, 1u)
        var tlp : tensorLayout2DPad
        tensorLayoutCreate(tlp)
        tensorLayoutSetDimension(tlp, 60u, 64u)
        tensorLayoutSetStride(tlp, 64u, 1u)
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var tvn : tensorView2D
        tensorViewCreate(tvn)
        var at : coopmatWgA_f16_64x64
        var btile : coopmatWgB_f16_64x64
        coopmatLoadTensor(at, a, 0u, tl, 0u, 64u, 0u, 64u)
        coopmatLoadTensor(btile, bt, 0u, tl, 0u, 64u, 0u, 64u, tv)
        var acc : coopmatWgAcc_f32_64x64
        acc = coopmatMulAdd(at, btile, acc)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
        var acc16 : coopmatWgAcc_f16_64x64
        acc16 = coopmatMulAdd(at, btile, acc16)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
        coopmatStoreTensor(acc16, o2, 0u, tl, 0u, 64u, 0u, 64u)
        coopmatPerElement(acc, acc, @@cm2_bias, 1.0)
        var cc : coopmatWgAcc_f32_64x64
        coopmatCopy(cc, acc)
        coopmatScale(cc, 2.0)
        coopmatPerElement(acc, acc, @@cm2_pick, cc)
        coopmatClamp(acc, -100.0, 100.0)
        var tt : coopmatWgAcc_f32_64x64
        coopmatAdd(tt, acc, cc)
        coopmatSub(tt, tt, cc)
        coopmatMulElem(tt, tt, tt)
        coopmatStoreTensor(tt, t, 0u, tl, 0u, 64u, 0u, 64u)
        var rm : coopmatWgAcc_f32_64x64
        coopmatReduce(rm, acc, COOPMAT_REDUCE_ROW, @@cm2_max)
        coopmatStoreTensor(rm, red, 0u, tl, 0u, 64u, 0u, 64u)
        var cs : coopmatWgAcc_f32_64x64
        coopmatReduce(cs, acc, COOPMAT_REDUCE_COLUMN, @@cm2_add)
        coopmatStoreTensor(cs, red, 4096u, tl, 0u, 64u, 0u, 64u)
        var pa : coopmatWgA_f16_64x64
        coopmatConvert(pa, acc)
        var ev : coopmatWgAcc_f32_64x64
        ev = coopmatMulAdd(pa, btile, ev)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
        var bk : coopmatWgB_f16_64x64
        coopmatLoadTensor(bk, a, 0u, tl, 0u, 64u, 0u, 64u, tvn)   // A's rows as B's k rows: the [k][n] store a plain view reads, the product through the transposing ldmatrix
        ev = coopmatMulAdd(pa, bk, ev)
        coopmatStoreTensor(ev, e, 0u, tl, 0u, 64u, 0u, 64u)
        var one : coopmatWgB_f16_64x64
        coopmatFill(one, 1.0)
        var rsum : coopmatWgAcc_f32_64x64
        rsum = coopmatMulAdd(pa, one, rsum)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
        coopmatStoreTensor(rsum, rs, 0u, tl, 0u, 64u, 0u, 64u)
        coopmatStoreTensor(acc, c, 0u, tlp, 0u, 64u, 0u, 64u)
    }
}

//! the same 64x64 tiles over eight warps: more warps than fragment rows, so the accumulators stay column-owned - the row
//! reduce through its shared scratch and the accumulator-to-A convert under block barriers (the four-warp twin above runs
//! both warp-locally); its `bs` tile is a global view, so every A and shared B tile of this kernel stores k permuted
class Cm2W8 {
    @ssbo @binding = 0 a : array<float16>    //! A 64x64 row-major: (r, k) at r * 64 + k
    @ssbo @binding = 1 bt : array<float16>   //! B transposed: (n, k) at n * 64 + k
    @ssbo @binding = 2 red : array<float>    //! the row max of the product, broadcast
    @ssbo @binding = 3 e : array<float>      //! P B, P the product as an f16 A tile
    @ssbo @binding = 4 e2 : array<float>     //! A (A + 0.5 + 0.25 + B): [k][n] tile, the fills, the global view

    [cuda_kernel(name="cm2w8_cuda", local_size_x = 256, min_blocks = 2), marker(no_coverage)]
    def cm2w8 {
        var tl : tensorLayout2D
        tensorLayoutCreate(tl)
        tensorLayoutSetDimension(tl, 64u, 64u)
        tensorLayoutSetStride(tl, 64u, 1u)
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var at : coopmatWgA_f16_64x64
        var btile : coopmatWgB_f16_64x64
        coopmatLoadTensor(at, a, 0u, tl, 0u, 64u, 0u, 64u)
        coopmatLoadTensor(btile, bt, 0u, tl, 0u, 64u, 0u, 64u, tv)
        var acc : coopmatWgAcc_f32_64x64
        acc = coopmatMulAdd(at, btile, acc)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
        var rm : coopmatWgAcc_f32_64x64
        coopmatReduce(rm, acc, COOPMAT_REDUCE_ROW, @@cm2_max)
        coopmatStoreTensor(rm, red, 0u, tl, 0u, 64u, 0u, 64u)
        var pa : coopmatWgA_f16_64x64
        coopmatConvert(pa, acc)
        var ev : coopmatWgAcc_f32_64x64
        ev = coopmatMulAdd(pa, btile, ev)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
        coopmatStoreTensor(ev, e, 0u, tl, 0u, 64u, 0u, 64u)
        var bk : coopmatWgB_f16_64x64
        coopmatLoadTensor(bk, a, 0u, tl, 0u, 64u, 0u, 64u)   // A's rows as B's k rows: the [k][n] source a plain view reads
        var ev2 : coopmatWgAcc_f32_64x64
        ev2 = coopmatMulAdd(at, bk, ev2)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
        coopmatFill(bk, 0.5)   // a shared fill off 1.0 stays a fill (the ones tile of the four-warp twin is the row sum)
        ev2 = coopmatMulAdd(at, bk, ev2)
        coopmatFill(btile, 0.25)   // the fill keeps the [n][k] tile shared
        ev2 = coopmatMulAdd(at, btile, ev2)
        var bs : coopmatWgB_f16_64x64
        coopmatLoadTensor(bs, bt, 0u, tl, 0u, 64u, 0u, 64u, tv)   // loaded under the transposed view and multiplied alone: a global view
        ev2 = coopmatMulAdd(at, bs, ev2)
        coopmatStoreTensor(ev2, e2, 0u, tl, 0u, 64u, 0u, 64u)
    }
}

struct DecBlk {
    q : int16[4]
}

//! the decode-in-load form: a block of four quants along k, scaled by the row's scale through a method
//! that reads a member buffer - the callback takes the block, its coordinate and the coordinate in it
class Cm2Dec {
    @ssbo @binding = 0 wq : array<DecBlk>    //! 64 rows x 16 blocks
    @ssbo @binding = 1 sc : array<float16>   //! a scale per row
    @ssbo @binding = 2 bt : array<float16>   //! B transposed, as Cm2's
    @ssbo @binding = 3 c : array<float>
    @workgroup wg_base : uint                //! the scale rows' base: a shared scalar the method reaches

    [spirv_decode]
    def decode(blk : DecBlk; bc, cib : uint2) : float16 => sc[wg_base + bc.x] * float16(float(int(blk.q[int(cib.y)])))

    [cuda_kernel(name="cm2dec_cuda", local_size_x = 128), marker(no_coverage)]
    def cm2dec {
        if (gl_LocalInvocationID.x == 0u) {
            wg_base = 0u
        }
        barrier()
        var fla : tensorLayout2D
        tensorLayoutCreate(fla)
        tensorLayoutSetBlockSize(fla, 1u, 4u)
        tensorLayoutSetDimension(fla, 64u, 64u)
        tensorLayoutSetStride(fla, 16u, 1u)
        var tl : tensorLayout2D
        tensorLayoutCreate(tl)
        tensorLayoutSetDimension(tl, 64u, 64u)
        tensorLayoutSetStride(tl, 64u, 1u)
        var tlp : tensorLayout2DPad   // B's last four rows (n 60..63) sit past the bound: the view's clamped gather reads them as zero
        tensorLayoutCreate(tlp)
        tensorLayoutSetDimension(tlp, 60u, 64u)
        tensorLayoutSetStride(tlp, 64u, 1u)
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var at : coopmatWgA_f16_64x64
        coopmatLoadTensorDecode(at, wq, 0u, fla, 0u, 64u, 0u, 64u, self.decode, true)
        var btile : coopmatWgB_f16_64x64
        coopmatLoadTensor(btile, bt, 0u, tlp, 0u, 64u, 0u, 64u, tv)
        var acc : coopmatWgAcc_f32_64x64
        acc = coopmatMulAdd(at, btile, acc)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
        coopmatStoreTensor(acc, c, 0u, tl, 0u, 64u, 0u, 64u)
    }
}

//! the product's left 32 columns over two k windows of 32: the decode-in-load inside a k loop, the view B tile reloaded
//! a window at a time
class Cm2DecK {
    @ssbo @binding = 0 wq : array<DecBlk>    //! 64 rows x 16 blocks
    @ssbo @binding = 1 sc : array<float16>   //! a scale per row
    @ssbo @binding = 2 bt : array<float16>   //! B transposed, as Cm2's
    @ssbo @binding = 3 c : array<float>
    @workgroup wg_base : uint

    [spirv_decode]
    def decode(blk : DecBlk; bc, cib : uint2) : float16 => sc[wg_base + bc.x] * float16(float(int(blk.q[int(cib.y)])))

    [cuda_kernel(name="cm2deck_cuda", local_size_x = 128), marker(no_coverage)]
    def cm2deck {
        if (gl_LocalInvocationID.x == 0u) {
            wg_base = 0u
        }
        barrier()
        var fla : tensorLayout2D
        tensorLayoutCreate(fla)
        tensorLayoutSetBlockSize(fla, 1u, 4u)
        tensorLayoutSetDimension(fla, 64u, 64u)
        tensorLayoutSetStride(fla, 16u, 1u)
        var tl : tensorLayout2D
        tensorLayoutCreate(tl)
        tensorLayoutSetDimension(tl, 64u, 64u)
        tensorLayoutSetStride(tl, 64u, 1u)
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var at : coopmatWgA_f16_64x32
        var btile : coopmatWgB_f16_32x32
        var acc : coopmatWgAcc_f32_64x32
        var k = 0u
        while (k < 64u) {
            coopmatLoadTensorDecode(at, wq, 0u, fla, 0u, 64u, k, 32u, self.decode)
            coopmatLoadTensor(btile, bt, 0u, tl, 0u, 32u, k, 32u, tv)
            acc = coopmatMulAdd(at, btile, acc)
            k += 32u
        }
        coopmatStoreTensor(acc, c, 0u, tl, 0u, 64u, 0u, 32u)   // the product's 32 columns into the 64-wide plane's left half
    }
}

struct RawBlk {
    q : int8[32]
}

//! the decode-in-load's pipelined form: a block of 32 bytes, one an element, two k windows of 32 over the 64-wide product -
//! the first window copies and decodes synchronously, the second finds its tile decoded past the first product; the
//! four-wide twin decodes a run four elements a call
class Cm2Raw {
    @ssbo @binding = 0 wq : array<RawBlk>    //! 64 rows x 2 blocks
    @ssbo @binding = 1 sc : array<float16>   //! a scale per row
    @ssbo @binding = 2 bt : array<float16>   //! B transposed, as Cm2's
    @ssbo @binding = 3 c : array<float>
    @workgroup wg_base : uint

    [spirv_decode]
    def decode(blk : RawBlk; bc, cib : uint2) : float16 => sc[wg_base + bc.x] * float16(float(int(blk.q[int(cib.y)])))

    [spirv_decode]
    def decode4(blk : RawBlk; bc, cib : uint2) : half4 {
        let s = float(sc[wg_base + bc.x])
        let k = int(cib.y)
        return half4(float4(s * float(int(blk.q[k])), s * float(int(blk.q[k + 1])), s * float(int(blk.q[k + 2])), s * float(int(blk.q[k + 3]))))
    }

    [cuda_kernel(name="cm2raw_cuda", local_size_x = 128), marker(no_coverage)]
    def cm2raw {
        if (gl_LocalInvocationID.x == 0u) {
            wg_base = 0u
        }
        barrier()
        var fla : tensorLayout2D
        tensorLayoutCreate(fla)
        tensorLayoutSetBlockSize(fla, 1u, 32u)
        tensorLayoutSetDimension(fla, 64u, 64u)
        tensorLayoutSetStride(fla, 2u, 1u)
        var tl : tensorLayout2D
        tensorLayoutCreate(tl)
        tensorLayoutSetDimension(tl, 64u, 64u)
        tensorLayoutSetStride(tl, 64u, 1u)
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var at : coopmatWgA_f16_64x32
        var btile : coopmatWgB_f16_32x32
        var acc : coopmatWgAcc_f32_64x32
        var k = 0u
        while (k < 64u) {
            coopmatLoadTensorDecode(at, wq, 0u, fla, 0u, 64u, k, 32u, self.decode, self.decode4)
            coopmatLoadTensor(btile, bt, 0u, tl, 0u, 32u, k, 32u, tv)
            acc = coopmatMulAdd(at, btile, acc)
            k += 32u
        }
        coopmatStoreTensor(acc, c, 0u, tl, 0u, 64u, 0u, 32u)
    }
}

//! the int8 workgroup tiles: an A tile of s8 rows and a transposed s8 B tile off int8 planes, the s32 product scaled per row
//! and column off the shared scale arrays into the f32 accumulator (coopmatScaleAcc), over two k windows with scales of
//! their own - the block-scaled int8 GEMM's step
class Cm2I8 {
    @ssbo @binding = 0 a8 : array<int8>     //! A 64x64 row-major s8
    @ssbo @binding = 1 b8 : array<int8>     //! B transposed: (n, k) at n * 64 + k
    @ssbo @binding = 2 rs : array<float>    //! two windows x 64 row scales
    @ssbo @binding = 3 cs : array<float>    //! two windows x 64 column scales
    @ssbo @binding = 4 c : array<float>
    @workgroup srs : float[128]
    @workgroup scs : float[128]

    [cuda_kernel(name="cm2i8_cuda", local_size_x = 128), marker(no_coverage)]
    def cm2i8 {
        let tid = gl_LocalInvocationID.x
        srs[tid] = rs[tid]
        scs[tid] = cs[tid]
        barrier()
        var tl : tensorLayout2D
        tensorLayoutCreate(tl)
        tensorLayoutSetDimension(tl, 64u, 64u)
        tensorLayoutSetStride(tl, 64u, 1u)
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var facc : coopmatWgAcc_f32_64x64
        var k = 0u
        while (k < 64u) {
            var at : coopmatWgA_s8_64x32
            var bt : coopmatWgB_s8_32x64
            coopmatLoadTensor(at, a8, 0u, tl, 0u, 64u, k, 32u)
            coopmatLoadTensor(bt, b8, 0u, tl, 0u, 64u, k, 32u, tv)
            var sacc : coopmatWgAcc_s32_64x64
            sacc = coopmatMulAdd(at, bt, sacc)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
            coopmatScaleAcc(facc, sacc, srs, (k / 32u) * 64u, scs, (k / 32u) * 64u)
            k += 32u
        }
        coopmatStoreTensor(facc, c, 0u, tl, 0u, 64u, 0u, 64u)
    }
}

//! the CUDA-only tier over one warp: a 16x32 s8 A tile and a [n][k] 16x32 s8 B tile copied into shared memory by
//! cp.async (one through a pointer local over the shared array), their fragments through ldmatrix, the int8 product
//! scaled by the row and column scales into an f32 accumulator, byte_perm, a named barrier under clock32, and the
//! inline-PTX floor both ways
class Intrin {
    @ssbo @binding = 0 a8 : array<uint4>     //! A 16x32 s8 row-major: row r at uint4 2r, 2r + 1
    @ssbo @binding = 1 b8 : array<uint4>     //! B as [n][k]: column n at uint4 2n, 2n + 1
    @ssbo @binding = 2 row_scales : array<float>
    @ssbo @binding = 3 col_scales : array<float>
    @ssbo @binding = 4 o : array<float>
    @ssbo @binding = 5 misc : array<uint>    //! 32 byte_perm results, 32 clock monotonic flags, 32 asm results
    @workgroup sa : uint4[32]
    @workgroup sb : uint4[32]

    [cuda_kernel(name="intrin_cuda", local_size_x = 32), marker(no_coverage)]
    def intrin {
        let lane = gl_SubgroupInvocationID
        var ps = unsafe(addr(sa[lane]))
        cp_async_16(ps, unsafe(addr(a8[lane])))
        cp_async_16(unsafe(addr(sb[lane])), unsafe(addr(b8[lane])))
        cp_async_commit()
        cp_async_wait(0)
        barrier()
        let mt = lane >> 3u
        let r = lane & 7u
        var fa : coopmatA_s8_16x32
        var fb : coopmatB_s8_32x16
        ldmatrix(fa, unsafe(addr(sa[(r + 8u * (mt & 1u)) * 2u + (mt >> 1u)])))   // (rows lo, k lo), (rows hi, k lo), (rows lo, k hi), (rows hi, k hi)
        ldmatrix(fb, unsafe(addr(sb[(r + 8u * (mt >> 1u)) * 2u + (mt & 1u)])))   // (n lo, k lo), (n lo, k hi), (n hi, k lo), (n hi, k hi)
        var s : coopmatAcc_s32_16x16
        s = coopmatMulAdd(fa, fb, s)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
        let g = lane >> 2u
        let t = lane & 3u
        var f : coopmatAcc_f32_16x16
        mma_scale_acc(f, s, float2(row_scales[g], row_scales[g + 8u]),
            float4(col_scales[2u * t], col_scales[2u * t + 1u], col_scales[2u * t + 8u], col_scales[2u * t + 9u]))
        coopmatStore(f, o, 0, 16, 0)
        misc[lane] = byte_perm(0x03020100u + lane, 0x8f0e0d0cu, 0x7654u + lane + (lane & 1u) * 0x8000u)
        let t0 = clock32()
        bar_sync(1u, 32u)
        let t1 = clock32()
        misc[32u + lane] = t1 - t0 < 0x80000000u ? 1u : 0u   // the counter moved forward (modulo its wrap)
        cuda_asm("membar.cta;")
        misc[64u + lane] = cuda_asm_value("add.u32 %0, %1, %2;", lane, 5u)
    }
}

//! the FP4 tensor rung over one warp: a 16x64 e2m1 A tile and a [n][k] 16x64 e2m1 B tile through cp.async and ldmatrix, the
//! block-scaled product under a UE4M3 scale word a row and a column (the k16 strips' four codes), and the two conversions
//! over a sweep the CPU bodies answer
class IntrinFp4 {
    @ssbo @binding = 0 a4 : array<uint4>      //! A 16x64 e2m1 row-major: row r at uint4 2r, 2r + 1, nibble k of the row's 32 bytes element k
    @ssbo @binding = 1 b4 : array<uint4>      //! B as [n][k]: column n at uint4 2n, 2n + 1
    @ssbo @binding = 2 saw : array<uint>      //! a row's four UE4M3 scale codes, byte i the k16 strip i
    @ssbo @binding = 3 sbw : array<uint>      //! a column's
    @ssbo @binding = 4 o : array<float>       //! the 16x16 product
    @ssbo @binding = 5 xin : array<float>     //! 128 e2m1 inputs as 64 pairs, 64 UE4M3 inputs, then the product's sixteen column factors
    @ssbo @binding = 6 cv : array<uint>       //! 64 e2m1 pair bytes, then 64 UE4M3 codes
    @workgroup sa : uint4[32]
    @workgroup sb : uint4[32]

    [cuda_kernel(name="intrin_fp4_cuda", local_size_x = 32), marker(no_coverage)]
    def intrin_fp4 {
        let lane = gl_SubgroupInvocationID
        cp_async_16(unsafe(addr(sa[lane])), unsafe(addr(a4[lane])))
        cp_async_16(unsafe(addr(sb[lane])), unsafe(addr(b4[lane])))
        cp_async_commit()
        cp_async_wait(0)
        barrier()
        let mt = lane >> 3u
        let r = lane & 7u
        var fa : coopmatA_e2m1_16x64
        var fb : coopmatB_e2m1_64x16
        ldmatrix(fa, unsafe(addr(sa[(r + 8u * (mt & 1u)) * 2u + (mt >> 1u)])))
        ldmatrix(fb, unsafe(addr(sb[(r + 8u * (mt >> 1u)) * 2u + (mt & 1u)])))
        let g = lane >> 2u
        var f : coopmatAcc_f32_16x16
        mma_block_scaled(f, fa, fb, saw[g + 8u * (lane & 1u)], sbw[g], sbw[8u + g])
        let t = lane & 3u
        mma_scale_cols(f, float4(xin[192u + 2u * t], xin[193u + 2u * t], xin[200u + 2u * t], xin[201u + 2u * t]))
        coopmatStore(f, o, 0, 16, 0)
        cv[lane] = cvt_e2m1x2(xin[2u * lane], xin[2u * lane + 1u])
        cv[32u + lane] = cvt_e2m1x2(xin[64u + 2u * lane], xin[64u + 2u * lane + 1u])
        cv[64u + lane] = cvt_ue4m3(xin[128u + lane])
        cv[96u + lane] = cvt_ue4m3(xin[160u + lane])
    }
}

//! the float exchange the kernel vocabulary does not declare: the emitter lowers every atomic by name
def private atomicExchange(var mem : float&; value : float) : float {
    let old = mem
    mem = value
    return old
}

def pin_peek(p : float const?) : float => *p

//! the entry override, a float exchange, the unroll count hint, a constant urange loop, a member written only through a
//! swizzle, a member read only through a helper's const-pointee parameter
class Pins {
    @ssbo @binding = 0 f : array<float>
    @ssbo @binding = 1 v4 : array<float4>
    @ssbo @binding = 2 src : array<float>

    [cuda_kernel(name="pins_cuda", entry="pins_entry", local_size_x = 64), marker(no_coverage)]
    def pins {
        let i = gl_GlobalInvocationID.x
        let old = atomicExchange(f[i], 2.5)
        var acc = old + pin_peek(unsafe(addr(src[i])))
        for [unroll_count=5] (j in range(int(i & 15u))) {
            acc += float(j)
        }
        for (k in urange(2u, 9u)) {
            acc += float(k)
        }
        v4[i].x = acc
    }
}

//! the gate's contract: every kind the emitter records, one line per construct kind
def public declared_cuda_census : table<string> {   // nolint:STYLE038 - one flat literal per construct kind; the census contract IS this list
    return <- {
        "call.pack32.char4.int",
        "call.pack32.uchar4.uint",
        "call.packHalf2x16",
        "call.unpack8.int.char4",
        "call.unpack8.short.char2",
        "call.unpack8.uint.uchar4",
        "call.unpack8.ushort.uchar2",
        "call.unpackHalf2x16",
        "call.coopmatAdd.wg.f32",
        "call.coopmatClamp.wg.f32",
        "call.coopmatConvert.wg.f32.A",
        "call.coopmatConvert.wg.f32.A.warp",
        "call.coopmatCopy.wg",
        "call.coopmatFill.wg.ones",
        "call.coopmatFill.wg.shared",
        "call.coopmatLoadTensor.A.f16.half",
        "call.coopmatLoadTensor.A.f16.half.kp",
        "call.coopmatLoadTensor.A.s8.char.raw",
        "call.coopmatLoadTensor.B.s8.char.t.raw",
        "call.coopmatLoadTensor.B.f16.half.gv.t",
        "call.coopmatLoadTensor.B.f16.half.kn",
        "call.coopmatLoadTensor.B.f16.half.kn.kp",
        "call.coopmatLoadTensor.B.f16.half.t",
        "call.coopmatLoadTensor.B.f16.half.t.kp",
        "call.coopmatLoadTensorDecode.A",
        "call.coopmatLoadTensorDecode.A.raw",
        "call.coopmatLoadTensorDecode.v4",
        "call.coopmatMulAdd.wg.f16.f16",
        "call.coopmatMulAdd.wg.f16.f32",
        "call.coopmatMulAdd.wg.rowsum",
        "call.coopmatMulAdd.wg.s8.s32",
        "call.coopmatMulElem.wg.f32",
        "call.coopmatPerElement.wg.f32",
        "call.coopmatReduce.wg.col.f32",
        "call.coopmatReduce.wg.row.f32",
        "call.coopmatReduce.wg.row.f32.warp",
        "call.coopmatScale.wg.f32",
        "call.coopmatScaleAcc.wg",
        "call.coopmatStoreTensor.C.f16.half",
        "call.coopmatStoreTensor.C.f32.float",
        "call.coopmatSub.wg.f32",
        "call.tensorLayoutCreate",
        "call.tensorLayoutCreate.pad",
        "call.tensorLayoutSetBlockSize",
        "call.tensorLayoutSetDimension",
        "call.tensorLayoutSetStride",
        "call.tensorViewCreate",
        "call.tensorViewCreate.t",
        "cvt.f32.f16",
        "cvt.i16.i32",
        "prelude.cm2",
        "stmt.var.wg_ones",
        "stmt.var.wg_shared",
        "stmt.var.wg_shared.dbl",
        "stmt.var.wg_shared.ring",
        "stmt.var.wg_shared.reuse",
        "stmt.var.wg_view",
        "ufn.param.hidden_shared_scalar",
        "call.coopmatConvert.f16.f32",
        "call.coopmatLoad.A.f16.col",
        "call.coopmatLoad.A.f16.row",
        "call.coopmatLoad.A.s8.col",
        "call.coopmatLoad.A.s8.row",
        "call.coopmatLoad.B.f16.col",
        "call.coopmatLoad.B.f16.row",
        "call.coopmatLoad.B.s8.col",
        "call.coopmatLoad.B.s8.row",
        "call.coopmatMulAdd.f16.f16",
        "call.coopmatMulAdd.f16.f32",
        "call.coopmatMulAdd.s8.s32",
        "call.coopmatStore.C.f16.row",
        "call.coopmatStore.C.f32.col",
        "call.coopmatStore.C.f32.row",
        "call.coopmatStore.C.s32.col",
        "call.coopmatStore.C.s32.row",
        "prelude.coopmat",
        "ctor.u32v",
        "cvt.i16.f32",
        "cvt.i32.u32",
        "cvt.u32.u16",
        "cvt.u32.i16",
        "cvt.u8.u32",
        "decl.elem_struct",
        "expr.field.struct",
        "expr.index.vector",
        "param.buffer_alias",
        "param.buffer_alias_const",
        "param.buffer_alias_const_aliased",
        "param.buffer_const_aliased",
        "ufn.param.hidden_buffer_const_aliased",
        "call.atomicAdd.f32",
        "call.atomicAdd.u32",
        "call.atomicAnd.u32",
        "call.atomicCompSwap.u32",
        "call.atomicExchange.f32",
        "call.atomicExchange.u32",
        "call.atomicMax.i32",
        "call.atomicMin.i32",
        "call.atomicOr.u32",
        "call.atomicXor.u32",
        "call.barrier",
        "call.bar_sync",
        "call.byte_perm",
        "call.clock32",
        "call.cp_async_16",
        "call.cp_async_commit",
        "call.cp_async_wait",
        "call.cuda_asm.0",
        "call.cuda_asm_value.2",
        "call.ldmatrix.A.s8",
        "call.ldmatrix.B.s8",
        "call.mma_scale_acc",
        "call.ldmatrix.A.e2m1",
        "call.ldmatrix.B.e2m1",
        "call.mma_block_scaled",
        "call.mma_scale_cols",
        "call.cvt_e2m1x2",
        "call.cvt_ue4m3",
        "expr.addr.global",
        "expr.addr.shared",
        "stmt.call_intrinsic",
        "call.bitcast.f32.u32",
        "call.bitcast.i32.f32",
        "call.bitcast.u32.f32",
        "call.math.abs.f32",
        "call.math.abs.f32v",
        "call.math.abs.i32",
        "call.math.atan.f32",
        "call.math.atan2.f32",
        "call.math.ceil.f32",
        "call.math.clamp.f32",
        "call.math.clamp.f32v",
        "call.math.clamp.i32",
        "call.math.cos.f32",
        "call.math.exp.f32",
        "call.math.exp.f32v",
        "call.math.exp2.f32",
        "call.math.floor.f32",
        "call.math.fmod.f32",
        "call.math.log.f32",
        "call.math.log2.f32",
        "call.math.mad.f32",
        "call.math.mad.f32v",
        "call.math.max.f32",
        "call.math.max.i32",
        "call.math.min.f32",
        "call.math.min.f32v",
        "call.math.min.i32",
        "call.math.pow.f32",
        "call.math.round.f32",
        "call.math.rsqrt.f32",
        "call.math.saturate.f32",
        "call.math.sin.f32",
        "call.math.sqrt.f32",
        "call.math.sqrt.f32v",
        "call.math.tanh.f32",
        "call.memory_barrier_buffer",
        "call.memory_barrier_shared",
        "call.method",
        "call.sdot4",
        "call.subgroupAdd.f32",
        "call.subgroupAll",
        "call.subgroupAny",
        "call.subgroupBroadcast.f32",
        "call.subgroupBroadcastFirst.f32",
        "call.subgroupClusteredAdd.f32",
        "call.subgroupElect",
        "call.subgroupMax.f32",
        "call.subgroupMin.f32",
        "call.subgroupShuffle.f32",
        "call.subgroupShuffleXor.f32",
        "call.usdot4",
        "call.user",
        "const.bool",
        "const.f32v",
        "const.float",
        "const.float16",
        "const.i32v",
        "const.int",
        "const.u32v",
        "const.uint",
        "ctor.f32",
        "ctor.f32v",
        "ctor.splat.f32v",
        "cvt.f16.f32",
        "cvt.f16v.f32v",
        "cvt.f32.u32",
        "cvt.f32v.f16v",
        "cvt.f32v.i32v",
        "cvt.i32.f32",
        "cvt.i32.i8",
        "cvt.i32v.f32v",
        "ctor.i32v",
        "cvt.i8.i32",
        "cvt.i8v.i32v",
        "cvt.u16.i32",
        "cvt.u32.f32",
        "cvt.u32.i32",
        "cvt.u8v.u32v",
        "cvt_sat.i32v.i8v",
        "cvt_sat.u32v.u16v",
        "decl.kargs_struct",
        "decl.method",
        "decl.ptr.global",
        "decl.ptr.shared",
        "decl.shared",
        "decl.shared_array",
        "decl.user_fn",
        "expr.builtin.gl_GlobalInvocationID",
        "expr.builtin.gl_LocalInvocationID",
        "expr.builtin.gl_LocalInvocationIndex",
        "expr.builtin.gl_NumSubgroups",
        "expr.builtin.gl_NumWorkGroups",
        "expr.builtin.gl_SubgroupID",
        "expr.builtin.gl_SubgroupInvocationID",
        "expr.builtin.gl_SubgroupSize",
        "expr.builtin.gl_WorkGroupID",
        "expr.builtin.gl_WorkGroupSize",
        "expr.deref",
        "expr.index",
        "expr.member",
        "expr.member.kargs",
        "expr.swizzle",
        "expr.swizzle.multi",
        "expr.var",
        "expr.global_const",
        "kernel_sig",
        "kernel_sig.launch_bounds",
        "kernel_sig.launch_bounds.min_blocks",
        "op.add.f16",
        "op.add.f32",
        "op.add.f32.rn",
        "op.add.f32v",
        "op.add.i32",
        "op.add.i32v",
        "op.add.u32",
        "op.add.u32v",
        "op.add_assign.f16",
        "op.add_assign.f32",
        "op.add_assign.f32.rn",
        "op.add_assign.f32v",
        "op.add_assign.i32",
        "op.add_assign.u32",
        "op.band.i32",
        "op.band.i32v",
        "op.band.u32",
        "op.band_assign.u32",
        "op.bnot.i32",
        "op.bnot.u32",
        "op.bor.i32",
        "op.bor.i32v",
        "op.bor.u32",
        "op.bor_assign.u32",
        "op.bxor.i32",
        "op.bxor.i32v",
        "op.bxor.u32",
        "op.bxor_assign.u32",
        "op.div.f16",
        "op.div.f32",
        "op.div.f32.rn",
        "op.div.f32v",
        "op.div.i32",
        "op.div.i32v",
        "op.div.u32",
        "op.div_assign.f32",
        "op.div_assign.f32.rn",
        "op.div_assign.u32",
        "op.eq.f32",
        "op.eq.f32v",
        "op.eq.i32",
        "op.eq.u32",
        "op.ge.f32",
        "op.ge.i32",
        "op.ge.u32",
        "op.gt.f16",
        "op.gt.f32",
        "op.gt.i32",
        "op.gt.u32",
        "op.land",
        "op.le.f32",
        "op.le.i32",
        "op.le.u32",
        "op.lnot",
        "op.lor",
        "op.lt.f16",
        "op.lt.f32",
        "op.lt.i32",
        "op.lt.u32",
        "op.mod.f32",
        "op.mod.i32",
        "op.mod.u32",
        "op.mod_assign.f32",
        "op.mod_assign.u32",
        "op.mul.f16",
        "op.mul.f32",
        "op.mul.f32.rn",
        "op.mul.f32v",
        "op.mul.i32",
        "op.mul.i32v",
        "op.mul.u32",
        "op.mul.u32v",
        "op.mul_assign.f32",
        "op.mul_assign.f32.rn",
        "op.mul_assign.u32",
        "op.ne.f32",
        "op.ne.f32v",
        "op.ne.i32",
        "op.ne.u32",
        "op.neg.f16",
        "op.neg.f32",
        "op.neg.f32v",
        "op.neg.i32",
        "op.neg.i32v",
        "op.postdec",
        "op.postinc",
        "op.predec",
        "op.preinc",
        "op.ptr_add",
        "op.select",
        "op.shl.i32",
        "op.shl.i32v",
        "op.shl.u32",
        "op.shl_assign.u32",
        "op.shr.i32",
        "op.shr.u32",
        "op.shr_assign.u32",
        "op.sub.f16",
        "op.sub.f32",
        "op.sub.f32.rn",
        "op.sub.f32v",
        "op.sub.i32",
        "op.sub.i32v",
        "op.sub.u32",
        "op.sub_assign.f32",
        "op.sub_assign.f32.rn",
        "op.sub_assign.u32",
        "param.buffer",
        "param.buffer_const",
        "param.builtin.gl_GlobalInvocationID",
        "param.builtin.gl_LocalInvocationID",
        "param.builtin.gl_LocalInvocationIndex",
        "param.builtin.gl_NumSubgroups",
        "param.builtin.gl_NumWorkGroups",
        "param.builtin.gl_SubgroupID",
        "param.builtin.gl_SubgroupInvocationID",
        "param.builtin.gl_SubgroupSize",
        "param.builtin.gl_WorkGroupID",
        "param.builtin.gl_WorkGroupSize",
        "param.kargs",
        "preamble",
        "prelude.half",
        "stmt.assign",
        "stmt.break",
        "stmt.call_atomic",
        "stmt.call_method",
        "stmt.call_user",
        "stmt.continue",
        "stmt.elif",
        "stmt.else",
        "stmt.for",
        "stmt.for.unroll",
        "stmt.if",
        "stmt.let",
        "stmt.let.const_table",
        "stmt.let.const_table.renamed",
        "stmt.pdl_wait",
        "stmt.return",
        "stmt.return_value",
        "stmt.var",
        "stmt.var.array",
        "stmt.var.zero",
        "stmt.while",
        "ufn.param.buffer",
        "ufn.param.buffer_const",
        "ufn.param.hidden_buffer",
        "ufn.param.hidden_buffer_const",
        "ufn.param.hidden_builtin",
        "ufn.param.hidden_kargs",
        "ufn.param.hidden_shared",
        "ufn.param.ptr",
        "ufn.param.ptr_const",
        "ufn.param.ref",
        "ufn.param.shared",
        "ufn.param.shared_const",
        "ufn.param.struct_cref",
        "ufn.param.value"
    }
}

def public all_cuda_censuses : array<string> {
    return <- [mul_ab_cuda_census, axpy_cuda_census, uarith_cuda_census, iarith_cuda_census, farith_cuda_census,
        precise_cuda_census, vecarith_cuda_census, control_cuda_census, mathcalls_cuda_census, reduce_cuda_census, halfarith_cuda_census,
        userfns_cuda_census, atomics_cuda_census, consttab_cuda_census, lattice_cuda_census, ptrs_cuda_census,
        alias_cuda_census, blocks_cuda_census, packs_cuda_census, coop_cuda_census, cm2_cuda_census, cm2w8_cuda_census,
        cm2dec_cuda_census, cm2deck_cuda_census, cm2raw_cuda_census, cm2i8_cuda_census, intrin_cuda_census, intrin_fp4_cuda_census,
        diverge_cuda_census, pins_cuda_census]
}

def public all_cuda_texts : array<tuple<name : string; text : string; entry : string>> {
    return <- [(name = "mul_ab", text = mul_ab_cuda, entry = mul_ab_cuda_entry),
        (name = "axpy", text = axpy_cuda, entry = axpy_cuda_entry),
        (name = "uarith", text = uarith_cuda, entry = uarith_cuda_entry),
        (name = "iarith", text = iarith_cuda, entry = iarith_cuda_entry),
        (name = "farith", text = farith_cuda, entry = farith_cuda_entry),
        (name = "precise", text = precise_cuda, entry = precise_cuda_entry),
        (name = "vecarith", text = vecarith_cuda, entry = vecarith_cuda_entry),
        (name = "control", text = control_cuda, entry = control_cuda_entry),
        (name = "mathcalls", text = mathcalls_cuda, entry = mathcalls_cuda_entry),
        (name = "reduce", text = reduce_cuda, entry = reduce_cuda_entry),
        (name = "halfarith", text = halfarith_cuda, entry = halfarith_cuda_entry),
        (name = "userfns", text = userfns_cuda, entry = userfns_cuda_entry),
        (name = "atomics", text = atomics_cuda, entry = atomics_cuda_entry),
        (name = "consttab", text = consttab_cuda, entry = consttab_cuda_entry),
        (name = "lattice", text = lattice_cuda, entry = lattice_cuda_entry),
        (name = "ptrs", text = ptrs_cuda, entry = ptrs_cuda_entry),
        (name = "alias", text = alias_cuda, entry = alias_cuda_entry),
        (name = "blocks", text = blocks_cuda, entry = blocks_cuda_entry),
        (name = "packs", text = packs_cuda, entry = packs_cuda_entry),
        (name = "coop", text = coop_cuda, entry = coop_cuda_entry),
        (name = "cm2", text = cm2_cuda, entry = cm2_cuda_entry),
        (name = "cm2w8", text = cm2w8_cuda, entry = cm2w8_cuda_entry),
        (name = "cm2dec", text = cm2dec_cuda, entry = cm2dec_cuda_entry),
        (name = "cm2deck", text = cm2deck_cuda, entry = cm2deck_cuda_entry),
        (name = "cm2raw", text = cm2raw_cuda, entry = cm2raw_cuda_entry),
        (name = "cm2i8", text = cm2i8_cuda, entry = cm2i8_cuda_entry),
        (name = "intrin", text = intrin_cuda, entry = intrin_cuda_entry),
        (name = "intrin_fp4", text = intrin_fp4_cuda, entry = intrin_fp4_cuda_entry),
        (name = "diverge", text = diverge_cuda, entry = diverge_cuda_entry),
        (name = "pins", text = pins_cuda, entry = pins_cuda_entry)]
}
