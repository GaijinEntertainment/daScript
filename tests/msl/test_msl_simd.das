options gen2

require dastest/testing_boost public
require _msl_common
require daslib/strings_boost

// Phase-3 simdgroup gate (text layer): the four simdgroup builtins lower to their attributed
// parameters and all sixteen simd_* overloads (sum + four shuffles, each float/int/uint; max,
// float only) lower to direct Metal intrinsic calls.
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

[test(description = "Simdgroup builtin parameters and the per-scalar-class intrinsic calls emit the expected MSL.")]
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

[test(description = "The simd kernel's emitted MSL matches its golden snapshot.")]
def test_simd_golden(t : T?) {
    t |> run("simd: golden snapshot") <| @(t : T?) {
        t |> equal(simd_msl, GOLDEN_SIMD)
    }
}

// Phase-3 threadgroup gate (text layer): @workgroup fixed-array + scalar members lower to
// threadgroup declarations at the top of the kernel body (name-sorted, never parameters);
// barrier() and memoryBarrierShared() both lower to threadgroup_barrier(mem_flags::mem_threadgroup)
// (MSL has no execution-free threadgroup fence — the SPIRV-Cross strengthening); and the five
// workgroup-grid builtins lower to their attributed parameters.
let GOLDEN_REDUCE = "#include <metal_stdlib>
using namespace metal;
kernel void reduce(device const uint* src [[buffer(0)]],
        device uint* dst [[buffer(1)]],
        uint3 gl_GlobalInvocationID [[thread_position_in_grid]],
        uint3 gl_LocalInvocationID [[thread_position_in_threadgroup]],
        uint gl_LocalInvocationIndex [[thread_index_in_threadgroup]],
        uint3 gl_NumWorkGroups [[threadgroups_per_grid]],
        uint3 gl_WorkGroupID [[threadgroup_position_in_grid]],
        uint3 gl_WorkGroupSize [[threads_per_threadgroup]],
        threadgroup char * shmem [[threadgroup(0)]]) \{
    threadgroup uint * tile = (threadgroup uint *)(shmem);
    threadgroup uint * total_wg = (threadgroup uint *)(shmem + 256);
    uint gid = gl_GlobalInvocationID.x;
    uint lid = gl_LocalInvocationID.x;
    tile[lid] = src[gid];
    threadgroup_barrier(mem_flags::mem_threadgroup);
    uint s = (gl_WorkGroupSize.x / 2u);
    while ((s > 0u)) \{
        if ((lid < s)) \{
            tile[lid] += tile[(lid + s)];
        \}
        threadgroup_barrier(mem_flags::mem_threadgroup);
        s /= 2u;
    \}
    if ((gl_LocalInvocationIndex == 0u)) \{
        (*total_wg) = (tile[0] + (gl_NumWorkGroups.x * 0u));
    \}
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if ((gl_LocalInvocationIndex == 0u)) \{
        dst[gl_WorkGroupID.x] = (*total_wg);
    \}
\}
"

[test(description = "@workgroup members become body-top threadgroup declarations, barriers lower, and the workgroup-grid builtins become parameters.")]
def test_threadgroup_shape(t : T?) {
    t |> run("reduce: @workgroup members -> body-top threadgroup declarations") <| @(t : T?) {
        t |> success(contains(reduce_msl, "    threadgroup uint * tile = (threadgroup uint *)(shmem);"), "fixed array -> shmem-derived pointer")
        t |> success(contains(reduce_msl, "    threadgroup uint * total_wg = (threadgroup uint *)(shmem + 256);"), "scalar -> shmem-derived pointer, accesses dereference")
        t |> success(!contains(reduce_msl, "tile [[buffer"), "threadgroup memory is never a buffer parameter")
    }
    t |> run("reduce: barrier lowering") <| @(t : T?) {
        t |> success(contains(reduce_msl, "threadgroup_barrier(mem_flags::mem_threadgroup);"), "barrier() -> threadgroup rendezvous+fence")
    }
    t |> run("reduce: workgroup-grid builtin parameters") <| @(t : T?) {
        t |> success(contains(reduce_msl, "uint3 gl_LocalInvocationID [[thread_position_in_threadgroup]]"), "local invocation id")
        t |> success(contains(reduce_msl, "uint3 gl_WorkGroupID [[threadgroup_position_in_grid]]"), "workgroup id")
        t |> success(contains(reduce_msl, "uint3 gl_NumWorkGroups [[threadgroups_per_grid]]"), "workgroup count")
        t |> success(contains(reduce_msl, "uint3 gl_WorkGroupSize [[threads_per_threadgroup]]"), "workgroup size")
        t |> success(contains(reduce_msl, "uint gl_LocalInvocationIndex [[thread_index_in_threadgroup]]"), "flat local index")
    }
}

[test(description = "The threadgroup reduce kernel's emitted MSL matches its golden snapshot.")]
def test_threadgroup_golden(t : T?) {
    t |> run("reduce: golden snapshot") <| @(t : T?) {
        t |> equal(reduce_msl, GOLDEN_REDUCE)
    }
}
