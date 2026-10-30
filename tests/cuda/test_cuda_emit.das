options gen2
options indenting = 4

// The text layer: the emitted shape of every construct family - the pointer struct and its
// unpacking, the by-value args struct, the builtin prologue, methods and free functions as
// __device__ functions with their hidden parameters, the prelude pieces a text carries only
// when it names them - and the compile gate: every fixture's text through NVRTC where a
// device exists.

require dastest/testing_boost public
require _cuda_common
require daslib/fio
require strings

def private has(t : T?; text, needle, what : string) {
    t |> success(find(text, needle) >= 0, "{what}: emitted CUDA carries `{needle}`")
    if (find(text, needle) < 0) {
        to_log(LOG_ERROR, "MISSING `{needle}` in:\n{text}\n")
    }
}

def private lacks(t : T?; text, needle, what : string) {
    t |> success(find(text, needle) < 0, "{what}: emitted CUDA does not carry `{needle}`")
}

[test]
def test_cuda_signature(t : T?) {
    t |> run("the pointer struct, the entry point, the launch bounds, the unpacking") <| @(t : T?) {
        has(t, mul_ab_cuda, "struct MulAB_mul_ab_bufs \{\n    const float* a;\n    const float* b;\n    float* c;\n\};", "bufs")
        has(t, mul_ab_cuda, "extern \"C\" __global__ void __launch_bounds__(64) MulAB_mul_ab(const MulAB_mul_ab_bufs __das_b)", "entry")
        has(t, mul_ab_cuda, "const float* __restrict__ a = __das_b.a;", "read-only unpack")
        has(t, mul_ab_cuda, "float* c = __das_b.c;", "written unpack")
        has(t, mul_ab_cuda, "const uint3 gl_GlobalInvocationID = make_uint3(blockIdx.x * blockDim.x + threadIdx.x", "builtin prologue")
        has(t, mul_ab_cuda, "typedef unsigned int uint;", "prelude head")
        t |> equal(mul_ab_cuda_entry, "MulAB_mul_ab")
        t |> equal(mul_ab_cuda_block, uint3(64u, 1u, 1u))
        t |> equal(mul_ab_cuda_smem, 0ul)
        lacks(t, mul_ab_cuda, "__shared__", "no shared memory")
        lacks(t, mul_ab_cuda, "struct half", "no half prelude")
    }
    t |> run("the args struct rides by value, a method is a __device__ function with hidden parameters") <| @(t : T?) {
        has(t, axpy_cuda, "struct __align__(8) AxpyArgs \{\n    uint nelem;\n    float scale;\n    uint zero;\n\};", "kargs struct")
        has(t, axpy_cuda, "Axpy_run(const Axpy_run_bufs __das_b, const AxpyArgs ka)", "kargs param")
        has(t, axpy_cuda, "__device__ __forceinline__ void axpy_body(uint3 gl_GlobalInvocationID, const AxpyArgs& ka, float* o, const float* __restrict__ y)", "method signature")
        has(t, axpy_cuda, "axpy_body(gl_GlobalInvocationID, ka, o, y);", "method call")
        has(t, axpy_cuda, "((ka.zero != 0u) ? 0.0f : o[i])", "kargs field + ternary")
        has(t, axpy_cuda, "    if ((i >= ka.nelem)) \{\n        return;\n    \}", "a bare return inside the method")
        has(t, axpy_cuda, "#if __CUDA_ARCH__ >= 900\n    asm volatile(\"griddepcontrol.wait;\" ::: \"memory\");", "the programmatic dependent launch wait at the kernel's top")
        t |> equal(axpy_cuda_block, uint3(256u, 1u, 1u))
    }
    t |> run("precise: every scalar f32 sum, product and quotient through the correctly rounded intrinsic") <| @(t : T?) {
        has(t, precise_cuda, "float acc = __fadd_rn(__fmul_rn(x, y), __fsub_rn(x, y));", "the operators")
        has(t, precise_cuda, "acc = __fadd_rn(acc, __fdiv_rn(x, y));", "the compound add over a quotient")
        has(t, precise_cuda, "acc = __fdiv_rn(acc, 1.5f);", "the compound divide")
        lacks(t, farith_cuda, "__fadd_rn", "a plain kernel's sums stay operators")
    }
}

[test]
def test_cuda_constructs(t : T?) {
    t |> run("vectors: make_, operators from the prelude, swizzles, splat, convert, scalar-bool compares") <| @(t : T?) {
        has(t, vecarith_cuda, "__device__ __forceinline__ float4 operator+(float4 a, float4 b)", "vec ops prelude")
        has(t, vecarith_cuda, "__device__ __forceinline__ float4 operator*(float4 a, float b)", "vec-scalar op")
        has(t, vecarith_cuda, "make_float4(1.5f, 1.5f, 1.5f, 1.5f)", "folded constant splat")
        has(t, vecarith_cuda, "__das_splat_float4(y.w)", "splat")
        has(t, vecarith_cuda, "make_float4(x.x, y.y, x.z, y.w)", "ctor from lanes")
        has(t, vecarith_cuda, "__das_cvt_int2_float2(", "convert")
        has(t, vecarith_cuda, "__das_swz_int2_yx(", "multi-lane swizzle")
        has(t, vecarith_cuda, "__das_all_eq_float4(x, y)", "vector ==")
        has(t, vecarith_cuda, "__das_any_ne_float4(x, y)", "vector !=")
        has(t, vecarith_cuda, "__das_swz_float4_xyz(x).z", "chained swizzle")
        lacks(t, vecarith_cuda, "x.xyz.z", "a chained swizzle lowers through the helper")
    }
    t |> run("control flow, compound assignment, loops and hints") <| @(t : T?) {
        has(t, control_cuda, "\} else if (((x < y) || (y >= 8u))) \{", "elif")
        has(t, control_cuda, "acc %= 101u;", "compound mod")
        has(t, control_cuda, "fv = fmodf(fv, 3.0f);", "float mod assign")
        has(t, control_cuda, "uint snap = (acc++);", "postfix value")
        has(t, control_cuda, "for (int j = 0; j < 3; ++j) \{", "const range")
        has(t, control_cuda, "for (int j = 1, j_end = (int((x & 3u)) + 2); j < j_end; ++j) \{", "hoisted bound")
        has(t, control_cuda, "#pragma unroll\n", "unroll hint")
        has(t, control_cuda, "while (((k < 3) && (!(m > 100)))) \{", "while")
        has(t, control_cuda, "uint zed = \{\};", "zero init")
    }
    t |> run("math: the float names, the int twins, lane-wise vector forms, bitcasts") <| @(t : T?) {
        has(t, mathcalls_cuda, "expf(x)", "")
        has(t, mathcalls_cuda, "rsqrtf(x)", "")
        has(t, mathcalls_cuda, "__saturatef(y)", "")
        has(t, mathcalls_cuda, "fmaf(x, y, 1.0f)", "mad")
        has(t, mathcalls_cuda, "__das_clamp_float(y, 0.0f, 1.0f)", "clamp")
        has(t, mathcalls_cuda, "fminf(x, y)", "")
        has(t, mathcalls_cuda, "abs(m)", "int abs")
        has(t, mathcalls_cuda, "__das_clamp_int(m, -2, 2)", "int clamp")
        has(t, mathcalls_cuda, "__das_lw_expf_float4(v)", "lane-wise exp")
        has(t, mathcalls_cuda, "__das_lw_fmaf_float4(v, v, v)", "lane-wise mad")
        has(t, mathcalls_cuda, "__float_as_uint(a[i])", "bitcast")
        has(t, mathcalls_cuda, "__int_as_float((ia[i] & 1073741823))", "bitcast")
    }
}

[test]
def test_cuda_constructs_device(t : T?) {
    t |> run("shared memory, barriers, the subgroup set over warp shuffles") <| @(t : T?) {
        has(t, reduce_cuda, "extern __shared__ __align__(16) char __das_smem[];", "the dynamic shared block")
        has(t, reduce_cuda, "uint& flag = *(uint*)(__das_smem + 0);", "shared scalar carved first (name order)")
        has(t, reduce_cuda, "float* part = (float*)(__das_smem + 16);", "shared array carved 16-aligned after it")
        has(t, reduce_cuda, "__das_warp_add_float(v)", "subgroupAdd")
        has(t, reduce_cuda, "__das_warp_max_float(v)", "subgroupMax")
        has(t, reduce_cuda, "__shfl_sync(__activemask(), v, __ffs(__activemask()) - 1)", "broadcast first: the first active lane")
        has(t, reduce_cuda, "__shfl_sync(__activemask(), v, 3u)", "broadcast")
        has(t, reduce_cuda, "__shfl_xor_sync(__activemask(), v, 16u)", "shuffle xor over the active mask")
        has(t, reduce_cuda, "(gl_SubgroupInvocationID == (unsigned)(__ffs(__activemask()) - 1))", "elect: the lowest active lane")
        has(t, reduce_cuda, "__das_warp_cadd_float(v, 4u)", "clustered add")
        has(t, reduce_cuda, "__any_sync(__activemask(), (v > 0.0f))", "any")
        has(t, reduce_cuda, "const uint gl_SubgroupID = gl_LocalInvocationIndex >> 5;", "subgroup id after its dependency")
        has(t, reduce_cuda, "__syncthreads();", "barrier")
        has(t, reduce_cuda, "__threadfence();", "buffer barrier")
        has(t, reduce_cuda, "void stash(float v, uint gl_LocalInvocationIndex, uint gl_SubgroupID, float* part)", "a method reaching a shared array takes it as a hidden pointer, after the builtins it reads")
        t |> equal(reduce_cuda_smem, 48ul, "the carve's end: the scalar's 16-aligned slot, then the eight floats")
    }
    t |> run("the half family rides the prelude") <| @(t : T?) {
        has(t, halfarith_cuda, "struct half \{", "half prelude")
        has(t, halfarith_cuda, "cvt.rn.f16.f32", "f32->f16 PTX")
        has(t, halfarith_cuda, "struct half4 \{ half x, y, z, w; \};", "half4 struct")
        has(t, halfarith_cuda, "__das_cvt_float4_half4(va[i])", "half4 -> float4")
        has(t, halfarith_cuda, "__das_cvt_half4_float4(", "float4 -> half4")
        has(t, halfarith_cuda, "half(2.0f)", "half literal")
        has(t, halfarith_cuda, "const half* __restrict__ a = __das_b.a;", "half buffer")
    }
    t |> run("free functions: buffer, shared, ref and value parameters, the builtin reach, generic twins") <| @(t : T?) {
        has(t, userfns_cuda, "float ufx_scale(float d)", "value param")
        has(t, userfns_cuda, "uint ufx_lane(uint3 gl_GlobalInvocationID)", "hidden builtin")
        has(t, userfns_cuda, "void ufx_splat(float* yo, uint i, float v)", "written buffer param")
        has(t, userfns_cuda, "void ufx_bump(float& acc, float by)", "ref param")
        has(t, userfns_cuda, "void ufx_part(float* sm, uint k, float v)", "shared param")
        has(t, userfns_cuda, "float ufx_read(const float* sm, uint k)", "read-only shared param")
        has(t, userfns_cuda, "void ufx_at(float* p, float v)", "pointer param, mutable pointee")
        has(t, userfns_cuda, "float ufx_peek(const float* p)", "pointer param, const pointee")
        has(t, userfns_cuda, "ufx_at((yo + (i)), ufx_peek((xs + (i))));", "addr() of a member element as the pointer argument")
        has(t, userfns_cuda, "float ufx_pick(const float* src, uint i)", "generic twin f32")
        has(t, userfns_cuda, "float ufx_pick_", "generic twin f16 takes a suffixed name")
        has(t, userfns_cuda, "const half* src", "generic twin half")
    }
    t |> run("atomics, constant tables, the lattice narrows, pointer locals") <| @(t : T?) {
        has(t, atomics_cuda, "atomicAdd(&(cnt[0]), 1u);", "atomic statement")
        has(t, atomics_cuda, "float old = atomicAdd(&(sum[0]), 1.5f);", "atomic value")
        has(t, atomics_cuda, "atomicExch(&(cnt[4]), i);", "exchange")
        has(t, atomics_cuda, "atomicCAS(&(cnt[5]), 0u, i);", "compare swap")
        has(t, consttab_cuda, "__constant__ float tab[4] = \{", "const table")
        has(t, consttab_cuda, "__constant__ float tab_c1[4] = \{", "a same-named table of other values takes a suffix")
        has(t, consttab_cuda, "+ ctab_pick(idx[i]));", "the function's own table serves its reads")
        has(t, consttab_cuda, "__constant__ int itab[9] = \{", "const int table")
        has(t, consttab_cuda, "float scratch[4] = \{\};", "local array")
        has(t, lattice_cuda, "__das_sat_char4_int4(", "saturating narrow")
        has(t, lattice_cuda, "__das_sat_ushort2_uint2(", "saturating narrow u16")
        has(t, lattice_cuda, "__dp4a(int(pk[i]), int((pk[i] ^ 16909060u)), 0)", "sdot4")
        has(t, lattice_cuda, "__das_usdot4(pk[i], pk[i])", "usdot4")
        has(t, ptrs_cuda, "const float* p = src + ((i * 2u));", "const pointer local")
        has(t, ptrs_cuda, "float* q = dst + (i);", "pointer local")
        has(t, ptrs_cuda, "p = (p + (1));", "pointer add")
        has(t, ptrs_cuda, "(*q) = (a + (*p));", "deref store")
    }
}

[test]
def test_cuda_lifts(t : T?) {
    t |> run("two members view one binding: one slot, a cast for the alias, __restrict__ only while no view is written") <| @(t : T?) {
        has(t, alias_cuda, "struct Alias_alias_bufs \{\n    const uint* wq;\n    float* o;\n    uint* p;\n\};", "one slot per binding, non-const where any view writes")
        has(t, alias_cuda, "const uint* __restrict__ wq = __das_b.wq;", "owner read-only, group unwritten")
        has(t, alias_cuda, "const uint4* __restrict__ wq4 = (const uint4*)__das_b.wq;", "alias read-only, group unwritten")
        has(t, alias_cuda, "float* o = __das_b.o;", "owner written")
        has(t, alias_cuda, "const float2* o2 = (const float2*)__das_b.o;", "alias read-only in a written group: no __restrict__")
        has(t, alias_cuda, "const uint* p = __das_b.p;", "owner read-only in a written group: no __restrict__")
        has(t, alias_cuda, "uint4* p4 = (uint4*)__das_b.p;", "alias written")
        has(t, alias_cuda, "float pair_sum(uint i, const float2* o2, const uint4* __restrict__ wq4)", "hidden params follow the group rule")
        has(t, alias_cuda, "+ make_uint4(1u, 2u, 3u, 4u))", "a folded uint4 constant")
    }
    t |> run("struct elements: the declaration, field access, a struct local, a vector lane by index, a rolled loop, a [template] instance") <| @(t : T?) {
        has(t, blocks_cuda, "struct Blk \{\n    short qs[8];\n    float sc;\n\};", "element struct")
        has(t, blocks_cuda, "struct Meta \{\n    uint pos;\n    uint cnt;\n    Blk blk;\n\};", "nested struct after its inner")
        has(t, blocks_cuda, "const Blk* __restrict__ blk = __das_b.blk;", "struct buffer")
        has(t, blocks_cuda, "Blk b = blk[i];", "struct local")
        has(t, blocks_cuda, "acc += float(b.qs[k]);", "fixed-array field by index")
        has(t, blocks_cuda, "Blk m = meta[i].blk;", "nested struct field")
        has(t, blocks_cuda, "__das_at_uint4(w4[i], int((meta[i].pos & 3u)))", "vector lane read")
        has(t, blocks_cuda, "__das_at_uint4(v, int((meta[i].cnt & 3u))) = tf_scale(lane);", "vector lane written, template instance by base name")
        has(t, blocks_cuda, "__device__ __forceinline__ uint& __das_at_uint4(uint4& v, int i)", "the written-lane overload")
        has(t, blocks_cuda, "__device__ __forceinline__ const uint& __das_at_uint4(const uint4& v, int i)", "the read-lane overload")
        has(t, blocks_cuda, "#pragma unroll 1\n", "dont_unroll")
        lacks(t, blocks_cuda, "0x", "no hashed name survives")
    }
    t |> run("the KHR tiles: fragment structs, lane gathers per layout, mma.sync per operand pair, the convert, the stores") <| @(t : T?) {
        has(t, coop_cuda, "struct __das_cmA_f16 \{ uint r[4]; \};", "A fragment")
        has(t, coop_cuda, "struct __das_cmC_f32 \{ float r[8]; \};", "f32 accumulator: two n-halves")
        has(t, coop_cuda, "__das_cmA_f16 ar = \{\};", "zero-filled tile local")
        has(t, coop_cuda, "__das_cm_ld_A_f16_row(ar, (const char*)(a + (0u)), int(16) * 2);", "A row load: stride in the buffer's elements")
        has(t, coop_cuda, "__das_cm_ld_A_f16_col(ac, (const char*)(a + (0u)), int(16) * 2);", "A column load")
        has(t, coop_cuda, "__das_cm_ld_B_f16_col(bc, (const char*)(b + (0u)), int(16) * 2);", "B column load")
        has(t, coop_cuda, "m.r[0] = __das_ld32(p + g * sb + t * 4);", "the contiguous pair load")
        has(t, coop_cuda, "m.r[0] = __das_ldh2(p + (2 * t) * sb + g * 2, p + (2 * t + 1) * sb + g * 2);", "the strided pair gather")
        has(t, coop_cuda, "c0 = __das_mma_f16_f32(ar, bc, c0);", "mma value")
        has(t, coop_cuda, "mma.sync.aligned.m16n8k16.row.col.f32.f16.f16.f32", "f16 -> f32 PTX")
        has(t, coop_cuda, "mma.sync.aligned.m16n8k16.row.col.f16.f16.f16.f16", "f16 -> f16 PTX")
        has(t, coop_cuda, "mma.sync.aligned.m16n8k32.row.col.s32.s8.s8.s32", "s8 -> s32 PTX")
        has(t, coop_cuda, "__das_cm_st_C_f32_col(c1, (char*)(o + (256)), int(16) * 4);", "column-major store")
        has(t, coop_cuda, "c2f = __das_cm_cvt_f32_f16(c2);", "convert")
        has(t, coop_cuda, "c0h = __das_cm_cvt_f16_f32(c0);", "the narrowing convert")
        has(t, coop_cuda, "__das_cm_cvt_f16_f32(const __das_cmC_f32& s)", "the narrowing helper is emitted")
        has(t, coop_cuda, "__das_f2h(s.r[4 * h]) | (__das_f2h(s.r[4 * h + 1]) << 16)", "... packing two f16 into a pair word")
        has(t, coop_cuda, "__das_cm_st_C_f16_row(c2, (char*)(sh + (0)), int(8) * 4);", "f16 store into shared pairs")
        has(t, coop_cuda, "__das_cm_ld_A_s8_col(a8c, (const char*)(a8t + (0u)), int(16) * 1);", "s8 column A load")
        has(t, coop_cuda, "float* o = __das_b.o;", "the stored buffer is written")
    }
}

[test]
def test_cuda_wg_tiles(t : T?) {
    t |> run("the workgroup tiles: shared A/B structs, per-warp accumulator slots, the layout object, the lane walks") <| @(t : T?) {
        has(t, cm2_cuda, "struct __das_wgA_f16_64x64 \{ ushort e[4608]; \};", "A tile in shared memory, its rows padded by eight halves")
        has(t, cm2_cuda, "struct __das_wgC_f32_64x64_w4r \{ __das_cmC_f32 fr[4]; \};", "accumulator: 16 fragments over 4 warps, row-owned (the kernel row-reduces it and the warps divide its rows)")
        has(t, cm2_cuda, "__das_wgA_f16_64x64& at = *(__das_wgA_f16_64x64*)(__das_smem + 0);", "shared tile local carved from the block")
        t |> equal(cm2_cuda_smem, 9216ul * 3ul + 1024ul, "the carve: the A, B and P tiles at the padded stride (64 x 72 halves) and the column reduce's scratch - the [k][n] tile reuses the A tile's slot, the ones tile is the row sum and carves nothing, the row reduce stays in the warp")
        has(t, cm2_cuda, "__das_wgC_f32_64x64_w4r acc = \{\};", "register tile local")
        has(t, cm2_cuda, "__das_tlayout tl = \{\};", "layout local")
        has(t, cm2_cuda, "tl.d0 = 64u; tl.d1 = 64u;", "layout dimension")
        has(t, cm2_cuda, "tlp.b0 = 1u; tlp.b1 = 1u; tlp.pad = 1;", "pad layout create")
        has(t, cm2_cuda, "__das_wg_ldt___das_wgB_f16_64x64_half_t(btile, bt + (0u), tl, uint(0u), uint(0u));", "transposed B load: a row-owning warp reads the whole tile, so it stays shared")
        has(t, cm2_cuda, "*(uint4*)&m.e[(c) * 72 + (r)] = *(const uint4*)(src + sr * tl.s0 + sc);", "the 16-byte copy, k in order")
        has(t, cm2_cuda, "tvn.t = 0;", "the plain view")
        has(t, cm2_cuda, "__das_wg_ldt___das_wgB_f16_64x64_kn_half_n(bk, a + (0u), tl, uint(0u), uint(0u));", "a B tile loaded under a plain view: the [k][n] store")
        has(t, cm2_cuda, "acc = __das_wg_mma___das_wgA_f16_64x64___das_wgB_f16_64x64___das_wgC_f32_64x64_w4r(at, btile, acc);", "tile product")
        lacks(t, cm2_cuda, "__das_kp", "no view, no permutation")
        has(t, cm2w8_cuda, "__global__ void __launch_bounds__(256, 2) Cm2W8_cm2w8(", "min_blocks: the register allocation leaves room for two blocks an SM")
        has(t, cm2_cuda, "__global__ void __launch_bounds__(128) Cm2_cm2(", "no min_blocks: the block alone")
        has(t, cm2w8_cuda, "struct __das_wgB_f16_64x64_gv \{ const ushort* p; uint s0, s1, d0, d1, r0, c0; int fast; \};", "a B tile loaded under the transposed view and multiplied alone, into column-owned accumulators, is a view of its source")
        has(t, cm2w8_cuda, "__das_wgB_f16_64x64_gv bs = \{\};", "the view local, in registers")
        has(t, cm2w8_cuda, "__das_wg_view___das_wgB_f16_64x64_gv_half(bs, bt + (0u), tl, uint(0u), uint(0u));", "the view records the window, no barrier")
        has(t, cm2w8_cuda, "m.fast = (tl.s1 == 1u && (tl.s0 & 7u) == 0u && (c0 & 7u) == 0u && ((((size_t)src) & 15) == 0) && (!tl.pad || (r0 + 64u <= tl.d0 && c0 + 64u <= tl.d1))) ? 1 : 0;", "the fast gather: a unit column stride, 16-byte rows and origin, the window inside the bounds")
        has(t, cm2w8_cuda, "uint4 w0 = __ldg((const uint4*)p); uint4 w1 = __ldg((const uint4*)(p + 8 * b.s0)); m[0].r[0] = w0.x; m[0].r[1] = w0.y; m[1].r[0] = w0.z; m[1].r[1] = w0.w;", "the fragment gather: a lane's eight k of a row as one 16-byte load, split over the two k steps")
        has(t, cm2w8_cuda, "uint sr = b.r0 + uint(n), sc = b.c0 + uint(k); return (sr < b.d0 && sc < b.d1) ? uint(b.p[sr * b.s0 + sc * b.s1]) : 0u;", "the clamped element read of the slow gather")
        has(t, cm2w8_cuda, "__das_wg_gld___das_wgB_f16_64x64_gv(fb2, b, kk2 * 32, __j * 16);", "the product gathers its column's B from global memory a 32-k block at a time")
        has(t, cm2w8_cuda, "ev2 = __das_wg_mma___das_wgA_f16_64x64___das_wgB_f16_64x64_gv___das_wgC_f32_64x64_w8(at, bs, ev2);", "the product over the view")
        has(t, cm2w8_cuda, "__device__ __forceinline__ int __das_kp(int k) \{ return (k & ~31) | ((k & 4) << 2) | ((k & 2) << 2) | ((k & 24) >> 2) | (k & 1); \}", "the k permutation a kernel with a view stores its tiles under")
        has(t, cm2w8_cuda, "__das_wg_ldt___das_wgA_f16_64x64_half_n_kp(at, a + (0u), tl, uint(0u), uint(0u));", "the A load under the permutation")
        has(t, cm2w8_cuda, "uint* __d = (uint*)&m.e[(r) * 72 + (((c) & ~31) + (((c) >> 2) & 6))]; __d[0] = __w.x; __d[4] = __w.y; __d[8] = __w.z; __d[12] = __w.w;", "a run of eight k lands as four words eight elements apart")
        has(t, cm2w8_cuda, "__das_wg_ldt___das_wgB_f16_64x64_half_t_kp(btile, bt + (0u), tl, uint(0u), uint(0u));", "a B tile the kernel also fills stays a shared [n][k] tile, permuted")
        has(t, cm2w8_cuda, "__das_wg_ldt___das_wgB_f16_64x64_kn_half_n_kp(bk, a + (0u), tl, uint(0u), uint(0u));", "the [k][n] tile under the permutation")
        has(t, cm2w8_cuda, "*(uint4*)&m.e[(__das_kp(r)) * 72 + (c)] = *(const uint4*)(src + sr * tl.s0 + sc);", "a [k][n] tile's rows permuted, its 16-byte copies whole")
        has(t, cm2dec_cuda, "at.e[__r * 72 + __das_kp(__c + __q)] = __v;", "the decode writes its run at the permuted k")
        has(t, cm2deck_cuda, "__das_wgA_f16_64x32& at = *(__das_wgA_f16_64x32*)(__das_smem + 16);", "the looped decode's A tile, one slot after the shared scalar's 16 bytes")
        has(t, cm2deck_cuda, "acc = __das_wg_mma___das_wgA_f16_64x32___das_wgB_f16_32x32_gv___das_wgC_f32_64x32_w4(at, btile, acc);", "the product over the 32-wide window, its B a view")
        has(t, cm2raw_cuda, "__das_wgA_f16_64x32* __p_at = (__das_wgA_f16_64x32*)(__das_smem + 16);", "the pipelined decode's A tile: two f16 slots behind a pointer")
        has(t, cm2raw_cuda, "__das_cp_async_16(__das_smem + ", "the raw blocks copied by cp.async")
        has(t, cm2raw_cuda, "*(uint*)&(*__p_at).e[__r * 40 + __das_kp(__c + 0)] = __v[0] | (__v[1] << 16);", "the synchronous decode stores packed pairs at the permuted k")
        has(t, cm2raw_cuda, "acc = __das_wg_mma___das_wgA_f16_64x32___das_wgB_f16_32x32_gv___das_wgC_f32_64x32_w4((*__p_at), btile, acc);\n        \{ uint __rb = __p_at_b;", "the next window's decode follows the product")
        has(t, cm2raw_cuda, "*(uint*)&(*__pn).e[__r * 40 + __das_kp(__c + 14)] = __v[14] | (__v[15] << 16);", "the deferred decode into the other slot, a run of sixteen")
        has(t, cm2raw_cuda, "half4 __d4 = decode4(__blk, __bc, make_uint2(__sr % fla.b0, (__sc + uint(__q)) % fla.b1), sc, wg_base); __v[__q + 0] = uint(__d4.x.bits);", "the four-wide twin decodes four elements a call")
        has(t, cm2_cuda, "cm2_bias(uint(__das_cm_row(__i, __g, __k)), uint(__das_cm_col(__j, __t, __k)), __das_cmC_get_f32(acc.fr[__s], __k), 1.0f)", "per-element callback with the element's row and column")
        has(t, cm2_cuda, "__das_cmC_get_f32(cc.fr[__s], __k))", "coopmat extra as its own element")
        has(t, cm2_cuda, "float* __das_red_1 = (float*)(__das_smem + ", "reduce scratch per site, carved from the block")
        has(t, cm2_cuda, "__shfl_xor_sync(0xffffffffu, __a, 1); __a = cm2_max(__a, __b);", "row reduce across the lane group")
        has(t, cm2_cuda, "__shfl_xor_sync(0xffffffffu, __a, 16); __a = cm2_add(__a, __b);", "column reduce across the groups")
        has(t, cm2_cuda, "pa.e[__das_cm_row(__i, __g, __k) * 72 + __das_cm_col(__j, __t, __k)] = (ushort)__das_f2h(", "accumulator into an A tile at the padded stride")
        has(t, cm2w8_cuda, "pa.e[__das_cm_row(__i, __g, __k) * 72 + __das_kp(__das_cm_col(__j, __t, __k))] = (ushort)__das_f2h(", "accumulator into an A tile, the k permuted where the kernel has a view")
        has(t, cm2dec_cuda, "half decode(const DecBlk& blk, uint2 bc, uint2 cib, const half* __restrict__ sc, uint& wg_base)", "decode method: a block by const reference (das's struct parameter, read in place), a shared scalar by reference")
        has(t, cm2dec_cuda, "make_uint2(__sr / fla.b0, __sc / fla.b1), make_uint2(__sr % fla.b0, __sc % fla.b1), sc, wg_base)", "block coordinate, coordinate in block, hidden members")
    }
}

[test]
def test_cuda_intrinsics(t : T?) {
    t |> run("the CUDA-only tier: cp.async, ldmatrix, the scaled accumulate, the named barrier, byte_perm, clock32, inline PTX") <| @(t : T?) {
        has(t, intrin_cuda, "asm volatile(\"cp.async.cg.shared.global [%0], [%1], 16;\" :: \"r\"((unsigned)__cvta_generic_to_shared(dst)), \"l\"(src) : \"memory\");", "cp.async of 16 bytes, the shared destination converted")
        has(t, intrin_cuda, "uint4* ps = sa + (lane);", "a pointer local over the shared array")
        has(t, intrin_cuda, "__das_cp_async_16(ps, (a8 + (lane)))", "the shared pointer local and the buffer pointer from addr()")
        has(t, intrin_cuda, "__das_cp_async_16((sb + (lane)), (b8 + (lane)))", "the shared and the buffer pointers from addr()")
        has(t, intrin_cuda, "__das_cp_async_commit();", "commit group")
        has(t, intrin_cuda, "asm volatile(\"cp.async.wait_group 0;\" ::: \"memory\");", "wait group with its constant")
        has(t, intrin_cuda, "ldmatrix.sync.aligned.m8n8.x4.shared.b16 \{%0,%1,%2,%3\}, [%4];", "ldmatrix x4")
        has(t, intrin_cuda, "__das_ldm___das_cmA_s8(fa, (sa + (", "the A fragment off the lane's row address")
        has(t, intrin_cuda, "f.r[k] += float(s.r[k]) * r[(k >> 1) & 1] * c[((k >> 2) << 1) | (k & 1)];", "the scaled accumulate's element map")
        has(t, intrin_cuda, "asm volatile(\"bar.sync %0, %1;\" :: \"r\"(id), \"r\"(count) : \"memory\");", "named barrier")
        has(t, intrin_cuda, "__byte_perm(", "byte_perm")
        has(t, intrin_cuda, "(unsigned)clock()", "clock32")
        has(t, intrin_cuda, "asm volatile(\"membar.cta;\" ::  : \"memory\");", "inline PTX, no operands")
        has(t, intrin_cuda, "unsigned r; asm volatile(\"add.u32 %0, %1, %2;\" : \"=r\"(r) : \"r\"(a0), \"r\"(a1) : \"memory\"); return r;", "inline PTX with a result and two operands")
    }
    t |> run("the FP4 tensor rung: e2m1 fragments through ldmatrix, the block-scaled mma, the conversions, the arch marker") <| @(t : T?) {
        has(t, intrin_fp4_cuda, "#define __DAS_ARCH_A__ 1", "the arch-specific target's marker")
        has(t, intrin_fp4_cuda, "__das_ldm___das_cmA_e2m1(fa, (sa + (", "the e2m1 A fragment off the lane's row address")
        has(t, intrin_fp4_cuda, "mma.sync.aligned.kind::mxf4nvf4.block_scale.scale_vec::4X.m16n8k64.row.col.f32.e2m1.e2m1.f32.ue4m3 \{%0,%1,%2,%3\}, \{%4,%5,%6,%7\}, \{%8,%9\}, \{%0,%1,%2,%3\}, %10, \{0,0\}, %11, \{0,0\};", "the block-scaled FP4 mma")
        has(t, intrin_fp4_cuda, "\"+f\"(f.r[4]), \"+f\"(f.r[5]), \"+f\"(f.r[6]), \"+f\"(f.r[7]) : \"r\"(a.r[0]), \"r\"(a.r[1]), \"r\"(a.r[2]), \"r\"(a.r[3]), \"r\"(b.r[2]), \"r\"(b.r[3]), \"r\"(sa), \"r\"(sb1)", "the second n-half under its own B scale word")
        has(t, intrin_fp4_cuda, "f.r[k] *= c[((k >> 2) << 1) | (k & 1)];", "the column scale's element map")
        has(t, intrin_fp4_cuda, "cvt.rn.satfinite.e2m1x2.f32 t, %1, %2; cvt.u16.u8 %0, t; \}\" : \"=h\"(d) : \"f\"(hi), \"f\"(lo)", "the e2m1 pair conversion through a byte register, the low nibble from the first argument")
        has(t, intrin_fp4_cuda, "cvt.rn.satfinite.e4m3x2.f32 %0, %1, %1;\" : \"=h\"(d) : \"f\"(p)", "the UE4M3 conversion off the low byte, no immediate operand")
    }
}

[test]
def test_cuda_packs(t : T?) {
    t |> run("the pack family") <| @(t : T?) {
        has(t, packs_cuda, "cvt.rn.f16x2.f32 %0, %1, %2;", "packHalf2x16 PTX")
        has(t, packs_cuda, "uint h = __das_pack_half2(f[i]);", "packHalf2x16")
        has(t, packs_cuda, "o[i] = __das_unpack_half2(h);", "unpackHalf2x16")
        has(t, packs_cuda, "__device__ __forceinline__ char4 __das_unpack8_char4(int x) \{ return make_char4(char(x), char(x >> 8), char(x >> 16), char(x >> 24)); \}", "unpack8 signed")
        has(t, packs_cuda, "__device__ __forceinline__ uchar2 __das_unpack8_uchar2(ushort x) \{ return make_uchar2(uchar(x), uchar(x >> 8)); \}", "unpack8 unsigned 16")
        has(t, packs_cuda, "__device__ __forceinline__ int __das_pack32_char4(char4 v) \{ return int(uchar(v.x)) | (int(uchar(v.y)) << 8) | (int(uchar(v.z)) << 16) | (int(uchar(v.w)) << 24); \}", "pack32 signed")
        has(t, packs_cuda, "__das_pack32_uchar4(ub4)", "pack32 unsigned")
    }
}

[test]
def test_cuda_pins(t : T?) {
    t |> run("the entry override, the float exchange, the unroll count, the constant urange, the swizzled store, the const pointee") <| @(t : T?) {
        t |> equal(pins_cuda_entry, "pins_entry", "entry= names the entry point")
        has(t, pins_cuda, "extern \"C\" __global__ void __launch_bounds__(64) pins_entry(const pins_entry_bufs __das_b)", "the entry and its pointer struct take the override")
        has(t, pins_cuda, "float old = atomicExch(&(f[i]), 2.5f);", "atomicExchange on float memory")
        has(t, pins_cuda, "    #pragma unroll 5\n    for (int j = 0, j_end = int((i & 15u)); j < j_end; ++j) \{", "unroll_count=5 heads its loop")
        has(t, pins_cuda, "for (uint k = 0x2u; k < 0x9u; ++k) \{", "the constant urange's bounds: das interpolates a uint as hex, a valid C literal")
        has(t, pins_cuda, "float4* v4 = __das_b.v4;", "a store through a swizzle writes its member")
        has(t, pins_cuda, "v4[i].x = acc;", "the swizzled store")
        has(t, pins_cuda, "const float* __restrict__ src = __das_b.src;", "a member a helper reads through a const pointee stays const")
        has(t, pins_cuda, "float pin_peek(const float* p)", "the const-pointee parameter")
    }
}

[test]
def test_cuda_no_include(t : T?) {
    t |> run("no emitted text includes a header") <| @(t : T?) {
        for (k in all_cuda_texts()) {
            lacks(t, k.text, "#include", k.name)
        }
    }
}

[test]
def test_cuda_compile_gate(t : T?) {
    t |> run("every fixture's text compiles through NVRTC for this card") <| @(t : T?) {
        if (!cuda_available() || !cuda_nvrtc_available()) {
            feint("no CUDA driver or NVRTC on this box; compile gate skipped\n")
            return
        }
        with_cuda_context() $(cc : CudaContext?) {
            if (cc == null) {
                feint("no CUDA device on this box; compile gate skipped\n")
                return
            }
            let base = cuda_live_object_count()
            let cache_dir = path_join(test_temp_dir(), "cubin")
            for (k in all_cuda_texts()) {
                var err : string
                var mod : CudaModule?
                let fn = function_from_source(cc, k.text, k.name, k.entry, mod, err, cache_dir = cache_dir)
                t |> success(fn != null, "{k.name} compiles and {k.entry} resolves: {err}")
                cuda_release(mod)
            }
            t |> equal(cuda_live_object_count(), base)
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}
