options gen2
options indenting = 4

// Phase-3 simdgroup gate (text layer): the four simdgroup builtins lower to their attributed
// parameters and all sixteen simd_* overloads (sum + four shuffles, each float/int/uint; max,
// float only) lower to direct Metal intrinsic calls.

require dastest/testing_boost public
require _msl_common
require daslib/strings_boost

let GOLDEN_SIMD = "#include <metal_stdlib>
using namespace metal;
kernel void simd_all(device const float* fa [[buffer(0)]],
        device const int* ia [[buffer(1)]],
        device const uint* ua [[buffer(2)]],
        device float* fo [[buffer(3)]],
        device int* io [[buffer(4)]],
        device uint* uo [[buffer(5)]],
        device uint* meta [[buffer(6)]],
        uint3 gl_GlobalInvocationID [[thread_position_in_grid]],
        uint gl_NumSubgroups [[simdgroups_per_threadgroup]],
        uint gl_SubgroupID [[simdgroup_index_in_threadgroup]],
        uint gl_SubgroupInvocationID [[thread_index_in_simdgroup]],
        uint gl_SubgroupSize [[threads_per_simdgroup]]) \{
    uint i = gl_GlobalInvocationID.x;
    uint lane = gl_SubgroupInvocationID;
    fo[i] = (((simd_sum(fa[i]) + simd_shuffle(fa[i], lane)) + (simd_shuffle_up(fa[i], 1u) + simd_shuffle_down(fa[i], 1u))) + simd_shuffle_xor(fa[i], 1u));
    io[i] = (((simd_sum(ia[i]) + simd_shuffle(ia[i], lane)) + (simd_shuffle_up(ia[i], 1u) + simd_shuffle_down(ia[i], 1u))) + simd_shuffle_xor(ia[i], 1u));
    uo[i] = (((simd_sum(ua[i]) + simd_shuffle(ua[i], lane)) + (simd_shuffle_up(ua[i], 1u) + simd_shuffle_down(ua[i], 1u))) + simd_shuffle_xor(ua[i], 1u));
    fo[i] = (fo[i] + simd_max(fa[i]));
    meta[i] = ((lane + gl_SubgroupSize) + (gl_NumSubgroups + gl_SubgroupID));
\}
"

[test]
def test_simd_shape(t : T?) {
    t |> run("simd: simdgroup builtin parameters") <| @(t : T?) {
        t |> success(contains(simd_msl, "uint gl_SubgroupInvocationID [[thread_index_in_simdgroup]]"), "lane id")
        t |> success(contains(simd_msl, "uint gl_SubgroupSize [[threads_per_simdgroup]]"), "execution width")
        t |> success(contains(simd_msl, "uint gl_NumSubgroups [[simdgroups_per_threadgroup]]"), "simdgroups per threadgroup")
        t |> success(contains(simd_msl, "uint gl_SubgroupID [[simdgroup_index_in_threadgroup]]"), "simdgroup index")
    }
    t |> run("simd: intrinsic calls per scalar class") <| @(t : T?) {
        t |> success(contains(simd_msl, "simd_sum(fa[i])"), "float sum")
        t |> success(contains(simd_msl, "simd_sum(ia[i])"), "int sum")
        t |> success(contains(simd_msl, "simd_sum(ua[i])"), "uint sum")
        t |> success(contains(simd_msl, "simd_max(fa[i])"), "float max")
        t |> success(contains(simd_msl, "simd_shuffle(fa[i], lane)"), "shuffle by computed lane")
        t |> success(contains(simd_msl, "simd_shuffle_up(ua[i], 1u)"), "shuffle up")
        t |> success(contains(simd_msl, "simd_shuffle_down(ua[i], 1u)"), "shuffle down")
        t |> success(contains(simd_msl, "simd_shuffle_xor(ua[i], 1u)"), "shuffle xor")
    }
}

[test]
def test_simd_golden(t : T?) {
    t |> run("simd: golden snapshot") <| @(t : T?) {
        t |> equal(simd_msl, GOLDEN_SIMD)
    }
}
