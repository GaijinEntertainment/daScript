options gen2
options indenting = 4

// Phase-5 simdgroup_matrix gate (text layer): the reference-body 8x8 tile family lowers to
// Metal's simdgroup_matrix surface — zero-init/filled locals via make_filled_simdgroup_matrix,
// device and threadgroup loads as pointer+offset, multiply / multiply-accumulate (all-f32,
// all-f16, and the mixed f32-accumulator/f16-operand combo), and device stores.

require dastest/testing_boost public
require _msl_common
require daslib/strings_boost

let GOLDEN_SGMAT = "#include <metal_stdlib>
using namespace metal;
kernel void sgmat_tile(device const float* fa [[buffer(0)]],
        device const float* fb [[buffer(1)]],
        device float* fc [[buffer(2)]],
        device float* fd [[buffer(3)]],
        device const half* ha [[buffer(4)]],
        device const half* hb [[buffer(5)]],
        device half* hc [[buffer(6)]],
        constant uint& base [[buffer(7)]]) \{
    simdgroup_float8x8 ma = make_filled_simdgroup_matrix<float, 8, 8>(0.0f);
    simdgroup_float8x8 mb = make_filled_simdgroup_matrix<float, 8, 8>(0.0f);
    simdgroup_load(ma, fa + base, 8u);
    simdgroup_load(mb, fb + base, 8u);
    simdgroup_float8x8 mm = make_filled_simdgroup_matrix<float, 8, 8>(0.0f);
    simdgroup_multiply(mm, ma, mb);
    simdgroup_store(mm, fc + base, 8u);
    simdgroup_float8x8 acc = make_filled_simdgroup_matrix<float, 8, 8>(1.0f);
    simdgroup_multiply_accumulate(acc, ma, mb, acc);
    simdgroup_store(acc, fd + base, 8u);
    simdgroup_half8x8 hx = make_filled_simdgroup_matrix<half, 8, 8>(0.0h);
    simdgroup_half8x8 hy = make_filled_simdgroup_matrix<half, 8, 8>(0.0h);
    simdgroup_load(hx, ha + 0u, 8u);
    simdgroup_load(hy, hb + 0u, 8u);
    simdgroup_half8x8 hm = make_filled_simdgroup_matrix<half, 8, 8>(0.5h);
    simdgroup_multiply(hm, hx, hy);
    simdgroup_half8x8 hacc = make_filled_simdgroup_matrix<half, 8, 8>(0.0h);
    simdgroup_multiply_accumulate(hacc, hx, hy, hm);
    simdgroup_store(hacc, hc + 0u, 8u);
\}
"

let GOLDEN_SGMAT_TILED = "#include <metal_stdlib>
using namespace metal;
kernel void sgmat_tiled(device const half* xa [[buffer(0)]],
        device const half* wb [[buffer(1)]],
        device float* yo [[buffer(2)]],
        constant uint& kdim [[buffer(3)]],
        uint3 gl_LocalInvocationID [[thread_position_in_threadgroup]]) \{
    threadgroup half ta[64];
    threadgroup half tb[64];
    uint lid = gl_LocalInvocationID.x;
    simdgroup_float8x8 acc = make_filled_simdgroup_matrix<float, 8, 8>(0.0f);
    uint kk = 0u;
    while ((kk < kdim)) \{
        ta[lid] = xa[(((lid / 8u) * kdim) + (kk + (lid % 8u)))];
        ta[(lid + 32u)] = xa[((((lid + 32u) / 8u) * kdim) + (kk + ((lid + 32u) % 8u)))];
        tb[lid] = wb[((kk * 8u) + lid)];
        tb[(lid + 32u)] = wb[((kk * 8u) + (lid + 32u))];
        threadgroup_barrier(mem_flags::mem_threadgroup);
        simdgroup_half8x8 ma = make_filled_simdgroup_matrix<half, 8, 8>(0.0h);
        simdgroup_half8x8 mb = make_filled_simdgroup_matrix<half, 8, 8>(0.0h);
        simdgroup_load(ma, ta + 0u, 8u);
        simdgroup_load(mb, tb + 0u, 8u);
        simdgroup_multiply_accumulate(acc, ma, mb, acc);
        threadgroup_barrier(mem_flags::mem_threadgroup);
        kk += 8u;
    \}
    simdgroup_store(acc, yo + 0u, 8u);
\}
"

[test]
def test_sgmat_shape(t : T?) {
    t |> run("sgmat: locals lower to simdgroup types with zero fills") <| @(t : T?) {
        t |> success(contains(sgmat_msl, "simdgroup_float8x8 ma = make_filled_simdgroup_matrix<float, 8, 8>(0.0f);"), "f32 zero-init local")
        t |> success(contains(sgmat_msl, "simdgroup_half8x8 hx = make_filled_simdgroup_matrix<half, 8, 8>(0.0h);"), "f16 zero-init local")
        t |> success(contains(sgmat_msl, "= make_filled_simdgroup_matrix<float, 8, 8>(1.0f);"), "explicit f32 fill")
        t |> success(contains(sgmat_msl, "= make_filled_simdgroup_matrix<half, 8, 8>(0.5h);"), "explicit f16 fill")
    }
    t |> run("sgmat: loads/stores lower as pointer + offset") <| @(t : T?) {
        t |> success(contains(sgmat_msl, "simdgroup_load(ma, fa + base, 8u);"), "device load at uniform offset")
        t |> success(contains(sgmat_msl, "simdgroup_load(hx, ha + 0u, 8u);"), "f16 device load")
        t |> success(contains(sgmat_msl, "simdgroup_store(mm, fc + base, 8u);"), "f32 store")
        t |> success(contains(sgmat_msl, "simdgroup_store(hacc, hc + 0u, 8u);"), "f16 store")
        t |> success(contains(sgmat_msl, "device float* fc [[buffer(2)]]"), "stored buffer is non-const")
        t |> success(contains(sgmat_msl, "device const float* fa [[buffer(0)]]"), "loaded buffer stays const")
    }
    t |> run("sgmat: multiply + multiply-accumulate per combo") <| @(t : T?) {
        t |> success(contains(sgmat_msl, "simdgroup_multiply(mm, ma, mb);"), "f32 multiply")
        t |> success(contains(sgmat_msl, "simdgroup_multiply(hm, hx, hy);"), "f16 multiply")
        t |> success(contains(sgmat_msl, "simdgroup_multiply_accumulate(acc, ma, mb, acc);"), "f32 mac")
        t |> success(contains(sgmat_msl, "simdgroup_multiply_accumulate(hacc, hx, hy, hm);"), "f16 mac")
    }
    t |> run("sgmat tiled: threadgroup staging + mixed mac") <| @(t : T?) {
        t |> success(contains(sgmat_tiled_msl, "threadgroup half ta[64];"), "staging tile declared")
        t |> success(contains(sgmat_tiled_msl, "simdgroup_load(ma, ta + 0u, 8u);"), "load from threadgroup tile")
        t |> success(contains(sgmat_tiled_msl, "simdgroup_multiply_accumulate(acc, ma, mb, acc);"), "mixed mac: f32 acc, f16 operands")
        t |> success(contains(sgmat_tiled_msl, "simdgroup_float8x8 acc"), "accumulator is f32")
        t |> success(contains(sgmat_tiled_msl, "simdgroup_half8x8 ma"), "operands are f16")
        t |> success(contains(sgmat_tiled_msl, "threadgroup_barrier(mem_flags::mem_threadgroup);"), "staging barrier")
    }
}

// Phase-7 rolled-GEMM shape: matrix arrays declare + zero-fill in a pragma'd loop; all
// unroll_range loops stay ROLLED with `#pragma clang loop unroll(full)` (the GPU backend
// unrolls late, register-budget-aware — pre-unrolled statements cost an occupancy tier);
// fixed-array locals get the aggregate zero form.
let GOLDEN_SGMAT_ROLLED = "#include <metal_stdlib>
using namespace metal;
kernel void sgmat_rolled(device const half* xa [[buffer(0)]],
        device const half* wb [[buffer(1)]],
        device float* yo [[buffer(2)]],
        constant uint& kdim [[buffer(3)]],
        uint3 gl_LocalInvocationID [[thread_position_in_threadgroup]]) \{
    threadgroup half ta[128];
    threadgroup half tb[64];
    uint lid = gl_LocalInvocationID.x;
    simdgroup_float8x8 mc[2];
    #pragma clang loop unroll(full)
    for (int mc_zi = 0; mc_zi < 2; ++mc_zi) \{
        mc[mc_zi] = make_filled_simdgroup_matrix<float, 8, 8>(0.0f);
    \}
    simdgroup_half8x8 ma[2];
    #pragma clang loop unroll(full)
    for (int ma_zi = 0; ma_zi < 2; ++ma_zi) \{
        ma[ma_zi] = make_filled_simdgroup_matrix<half, 8, 8>(0.0h);
    \}
    half va[4] = \{\};
    uint kk = 0u;
    while ((kk < kdim)) \{
        uint flat0 = (lid * 4u);
        device const half * xcur = xa + (((((((flat0 / 64u) * 8u) + ((flat0 % 64u) / 8u)) * kdim) + kk) + (flat0 % 8u)));
        #pragma clang loop unroll(full)
        for (int i = 0; i < 4; ++i) \{
            va[i] = xcur[(uint(i))];
        \}
        #pragma clang loop unroll(full)
        for (int i = 0; i < 4; ++i) \{
            ta[((lid * 4u) + uint(i))] = va[i];
        \}
        tb[lid] = wb[(((kk + (lid / 8u)) * 8u) + (lid % 8u))];
        tb[(lid + 32u)] = wb[(((kk + ((lid + 32u) / 8u)) * 8u) + ((lid + 32u) % 8u))];
        threadgroup_barrier(mem_flags::mem_threadgroup);
        simdgroup_half8x8 mbb = make_filled_simdgroup_matrix<half, 8, 8>(0.0h);
        simdgroup_load(mbb, tb + 0u, 8u);
        threadgroup const half * acur = ta + (0u);
        #pragma clang loop unroll(full)
        for (int i = 0; i < 2; ++i) \{
            simdgroup_load(ma[i], acur, 8u);
            simdgroup_multiply_accumulate(mc[i], ma[i], mbb, mc[i]);
            acur += 64u;
        \}
        threadgroup_barrier(mem_flags::mem_threadgroup);
        kk += 8u;
    \}
    #pragma clang loop unroll(full)
    for (int i = 0; i < 2; ++i) \{
        simdgroup_store(mc[i], yo + (uint(i) * 64u), 8u);
    \}
\}
"

[test]
def test_sgmat_rolled_shape(t : T?) {
    t |> run("sgmat rolled: arrays + unroll_range keep loops rolled") <| @(t : T?) {
        t |> success(contains(sgmat_rolled_msl, "simdgroup_float8x8 mc[2];"), "f32 matrix array local")
        t |> success(contains(sgmat_rolled_msl, "simdgroup_half8x8 ma[2];"), "f16 matrix array local")
        t |> success(contains(sgmat_rolled_msl, "half va[4] = \{\};"), "fixed-array local zero form")
        t |> success(contains(sgmat_rolled_msl, "#pragma clang loop unroll(full)"), "unroll pragma present")
        t |> success(contains(sgmat_rolled_msl, "for (int i = 0; i < 2; ++i) \{"), "rolled loop form")
        t |> success(contains(sgmat_rolled_msl, "threadgroup const half * acur = ta + (0u);"), "tg_cursor pointer decl")
        t |> success(contains(sgmat_rolled_msl, "device const half * xcur = xa + ("), "dev_cursor pointer decl")
        t |> success(contains(sgmat_rolled_msl, "simdgroup_load(ma[i], acur, 8u);"), "matrix load through cursor")
        t |> success(contains(sgmat_rolled_msl, "va[i] = xcur[(uint(i))];"), "element read through cursor")
        t |> success(contains(sgmat_rolled_msl, "acur += 64u;"), "cursor advance")
        t |> success(contains(sgmat_rolled_msl, "simdgroup_multiply_accumulate(mc[i], ma[i], mbb, mc[i]);"), "indexed mac")
        t |> success(contains(sgmat_rolled_msl, "mc[mc_zi] = make_filled_simdgroup_matrix<float, 8, 8>(0.0f);"), "array zero-fill loop")
    }
}

[test]
def test_sgmat_golden(t : T?) {
    t |> run("sgmat: golden snapshot") <| @(t : T?) {
        t |> equal(sgmat_msl, GOLDEN_SGMAT)
    }
    t |> run("sgmat tiled: golden snapshot") <| @(t : T?) {
        t |> equal(sgmat_tiled_msl, GOLDEN_SGMAT_TILED)
    }
    t |> run("sgmat rolled: golden snapshot") <| @(t : T?) {
        t |> equal(sgmat_rolled_msl, GOLDEN_SGMAT_ROLLED)
    }
}
