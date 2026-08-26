options gen2
options _dasllama_internal = true

// NAX register-fragment GEMM probe (M5 arc): is the per-simdgroup 16x32x16 matmul2d with
// DEVICE->REGISTER operands (no threadgroup staging, no barriers) faster than the crowned
// q8u tg-staged form at the 1B's dominant GEMM shapes? Hand-MSL arms raced against the
// production PSO on one queue; numerics gated against a CPU dequant dot. Decides whether
// the emitter grows a nax rail — measurement only, nothing here ships.

require math
require strings
require metal/msl_shader
require metal/das_metal_boost
require ?das_metal das_metal
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_prefill
require dasllama/dasllama_metal_lens

// the reference lane map (vllm-metal pagedattention_nax / MLX steel BaseNAXFrag::get_coord):
// lane owns rows {fm, fm+8} x cols {fn..fn+3} of a 16x16 fragment, element i*4+j
let NAX_COMMON = ("#include <metal_stdlib>\n#include <MetalPerformancePrimitives/MetalPerformancePrimitives.h>\nusing namespace metal;\n" +
    "static inline short2 nax_coord() \{\n" +
    "  const ushort lane = __metal_get_thread_index_in_simdgroup(ushort());\n" +
    "  const short qid = lane >> 2;\n" +
    "  const short fm = (qid & 4) | ((lane >> 1) & 3);\n" +
    "  const short fn = ((qid & 2) | (lane & 1)) * 4;\n" +
    "  return short2\{fn, fm\};\n" +
    "\}\n" +
    "template <bool TB>\n" +
    "METAL_FUNC void nax_mma(thread vec<float,8>& C0, thread vec<float,8>& C1,\n" +
    "                        const thread vec<half,8>& A,\n" +
    "                        const thread vec<half,8>& B0, const thread vec<half,8>& B1) \{\n" +
    "  constexpr auto desc = mpp::tensor_ops::matmul2d_descriptor(16, 32, 16, false, TB, true,\n" +
    "      mpp::tensor_ops::matmul2d_descriptor::mode::multiply_accumulate);\n" +
    "  mpp::tensor_ops::matmul2d<desc, metal::execution_simdgroup> op;\n" +
    "  auto ca = op.template get_left_input_cooperative_tensor<half, half, float>();\n" +
    "  auto cb = op.template get_right_input_cooperative_tensor<half, half, float>();\n" +
    "  auto cc = op.template get_destination_cooperative_tensor<decltype(ca), decltype(cb), float>();\n" +
    "  _Pragma(\"clang loop unroll(full)\") for (short i = 0; i < 8; i++) \{\n" +
    "    ca[i] = A[i]; cb[i] = B0[i]; cb[8 + i] = B1[i]; cc[i] = C0[i]; cc[8 + i] = C1[i]; \}\n" +
    "  op.run(ca, cb, cc);\n" +
    "  _Pragma(\"clang loop unroll(full)\") for (short i = 0; i < 8; i++) \{ C0[i] = cc[i]; C1[i] = cc[8 + i]; \}\n" +
    "\}\n")

// arm A — the register-blocked q8 GEMM: one simdgroup owns 16(m) x 4*32(n) of C (8 frag
// pairs), tg = 4 sgs stacked on m (64 x 128 C per tg). Per 16-K step: ONE A fragment
// (f32 -> f16 at load), 8 dequant B fragments read straight from the 34B q8 blob, 4 mmas.
// No threadgroup memory, no barriers.
let NAX_Q8_GEMM = (NAX_COMMON +
    "kernel void nax_q8_gemm(\n" +
    "    device const half* wsh [[buffer(0)]], device const char* wqb [[buffer(1)]],\n" +
    "    device const float* x [[buffer(2)]], device float* y [[buffer(3)]],\n" +
    "    constant uint& kdim [[buffer(4)]], constant uint& ndim [[buffer(5)]],\n" +
    "    uint3 tgp [[threadgroup_position_in_grid]], ushort sgi [[simdgroup_index_in_threadgroup]])\n" +
    "\{\n" +
    "  const uint mBase = (tgp.x * 4u + sgi) * 16u;\n" +
    "  const uint nBase = tgp.y * 256u;\n" +
    "  const short2 sc = nax_coord();\n" +
    "  const uint fm = uint(sc.y); const uint fn = uint(sc.x);\n" +
    "  vec<float,8> C0[8], C1[8];\n" +
    "  _Pragma(\"clang loop unroll(full)\") for (short t = 0; t < 8; t++) \{ C0[t] = 0.0f; C1[t] = 0.0f; \}\n" +
    "  const uint nb = kdim / 32u;\n" +
    "  const uint r0 = mBase + fm;\n" +
    "  for (uint kb = 0u; kb < kdim; kb += 16u) \{\n" +
    "    vec<half,8> A;\n" +
    "    _Pragma(\"clang loop unroll(full)\") for (short i = 0; i < 2; i++) \{\n" +
    "      const float4 av = *(device const float4*)(x + (r0 + uint(i) * 8u) * kdim + kb + fn);\n" +
    "      A[i * 4 + 0] = half(av.x); A[i * 4 + 1] = half(av.y); A[i * 4 + 2] = half(av.z); A[i * 4 + 3] = half(av.w);\n" +
    "    \}\n" +
    "    const uint blkk = kb / 32u; const uint qoff = (kb % 32u) + fn;\n" +
    "    _Pragma(\"clang loop unroll(full)\") for (short t = 0; t < 8; t++) \{\n" +
    "      vec<half,8> B0, B1;\n" +
    "      _Pragma(\"clang loop unroll(full)\") for (short i = 0; i < 2; i++) \{\n" +
    "        const uint nr0 = nBase + uint(t) * 32u + fm + uint(i) * 8u;\n" +
    "        const uint blk0 = nr0 * nb + blkk;\n" +
    "        const half d0 = wsh[blk0 * 17u];\n" +
    "        const uint blk1 = (nr0 + 16u) * nb + blkk;\n" +
    "        const half d1 = wsh[blk1 * 17u];\n" +
    "        const char4 q0 = *(device const char4*)(wqb + blk0 * 34u + 2u + qoff);\n" +
    "        const char4 q1 = *(device const char4*)(wqb + blk1 * 34u + 2u + qoff);\n" +
    "        B0[i * 4 + 0] = d0 * half(q0.x); B0[i * 4 + 1] = d0 * half(q0.y);\n" +
    "        B0[i * 4 + 2] = d0 * half(q0.z); B0[i * 4 + 3] = d0 * half(q0.w);\n" +
    "        B1[i * 4 + 0] = d1 * half(q1.x); B1[i * 4 + 1] = d1 * half(q1.y);\n" +
    "        B1[i * 4 + 2] = d1 * half(q1.z); B1[i * 4 + 3] = d1 * half(q1.w);\n" +
    "      \}\n" +
    "      nax_mma<true>(C0[t], C1[t], A, B0, B1);\n" +
    "    \}\n" +
    "  \}\n" +
    "  _Pragma(\"clang loop unroll(full)\") for (short t = 0; t < 8; t++)\n" +
    "    _Pragma(\"clang loop unroll(full)\") for (short i = 0; i < 2; i++)\n" +
    "      _Pragma(\"clang loop unroll(full)\") for (short j = 0; j < 4; j++) \{\n" +
    "        y[(r0 + uint(i) * 8u) * ndim + nBase + uint(t) * 32u + fn + uint(j)] = C0[t][i * 4 + j];\n" +
    "        y[(r0 + uint(i) * 8u) * ndim + nBase + uint(t) * 32u + 16u + fn + uint(j)] = C1[t][i * 4 + j];\n" +
    "      \}\n" +
    "\}\n")


// arm B — the tg-protocol f16(A-stream) x RAW-int8(W) form: W stages as quant BYTES (half the
// threadgroup traffic of the dequant-half stage, no multiply at staging); the per-32-block
// q8 scale folds on a second cooperative C after each run (float x int8 -> float is in the
// MPP mixed-operand table). A streams from device f32 exactly like the crowned q8u.
let TGI8_Q8_GEMM = (NAX_COMMON +
    "static void tmm_q8i(threadgroup int8_t * wt, threadgroup half * tsc, device const half * ws, device const char * wq, device const float * a, uint lda, device float * c, uint ldc, uint kk, uint ldwb, uint tid) \{\n" +
    "    constexpr auto d = mpp::tensor_ops::matmul2d_descriptor(32, 64, 32, false, true, true, mpp::tensor_ops::matmul2d_descriptor::mode::multiply_accumulate);\n" +
    "    mpp::tensor_ops::matmul2d<d, execution_simdgroups<4>> op;\n" +
    "    using AT = tensor<device float, dextents<int32_t, 2>, tensor_inline>;\n" +
    "    using WT = tensor<threadgroup int8_t, dextents<int32_t, 2>, tensor_inline>;\n" +
    "    auto cB = op.get_destination_cooperative_tensor<AT, WT, float>();\n" +
    "    auto cT = op.get_destination_cooperative_tensor<AT, WT, float>();\n" +
    "    short colx[64]; ushort ne = 0;\n" +
    "    for (uint16_t i = 0; i < cT.get_capacity(); ++i) \{ if (cT.is_valid_element(i)) \{ cT[i] = 0.0f; auto ids = cT.get_multidimensional_index(i); colx[i] = short(ids[0]); ne = i + 1; \} \}\n" +
    "    const uint nb = kk / 32u;\n" +
    "    for (uint b = 0; b < nb; ++b) \{\n" +
    "        for (uint work = tid; work < 64u * 2u; work += 128u) \{\n" +
    "            const uint col = work >> 1;\n" +
    "            const uint kh = (work & 1u) * 16u;\n" +
    "            const uint blk = col * ldwb + b;\n" +
    "            if (kh == 0u) \{ tsc[col] = ws[blk * 17u]; \}\n" +
    "            device const char * q = wq + blk * 34u + kh;\n" +
    "            threadgroup int8_t * dstp = wt + col * 32u + kh;\n" +
    "            _Pragma(\"clang loop unroll(full)\") for (short i = 0; i < 16; ++i) \{ dstp[i] = int8_t(q[i]); \}\n" +
    "        \}\n" +
    "        threadgroup_barrier(mem_flags::mem_threadgroup);\n" +
    "        for (uint16_t i = 0; i < ne; ++i) \{ if (cB.is_valid_element(i)) cB[i] = 0.0f; \}\n" +
    "        auto A = AT((device float *)(a + b * 32u), dextents<int32_t, 2>(32, 32), metal::array<int32_t, 2>\{1, int(lda)\});\n" +
    "        auto W = WT((threadgroup int8_t *)wt, dextents<int32_t, 2>(32, 64));\n" +
    "        op.run(A, W, cB);\n" +
    "        for (uint16_t i = 0; i < ne; ++i) \{ if (cB.is_valid_element(i)) cT[i] += cB[i] * float(tsc[colx[i]]); \}\n" +
    "        threadgroup_barrier(mem_flags::mem_threadgroup);\n" +
    "    \}\n" +
    "    auto C = tensor<device float, dextents<int32_t, 2>, tensor_inline>(c, dextents<int32_t, 2>(64, 32), metal::array<int32_t, 2>\{1, int(ldc)\});\n" +
    "    cT.store(C);\n" +
    "\}\n" +
    "kernel void tgi8_q8_gemm(device const half* wsh [[buffer(0)]],\n" +
    "        device const char* wqb [[buffer(1)]],\n" +
    "        device const float* xf [[buffer(2)]],\n" +
    "        device float* y [[buffer(3)]],\n" +
    "        constant uint& kdim [[buffer(4)]],\n" +
    "        constant uint& ndim [[buffer(5)]],\n" +
    "        uint tid [[thread_index_in_threadgroup]],\n" +
    "        uint3 wg [[threadgroup_position_in_grid]]) \{\n" +
    "    threadgroup int8_t wt[2048];\n" +
    "    threadgroup half tsc[64];\n" +
    "    uint mBase = wg.x * 32u;\n" +
    "    uint nBase = wg.y * 64u;\n" +
    "    uint blk0 = nBase * (kdim / 32u);\n" +
    "    tmm_q8i(wt, tsc, wsh + blk0 * 17u, wqb + blk0 * 34u + 2u, xf + mBase * kdim, kdim, y + mBase * ndim + nBase, ndim, kdim, kdim / 32u, tid);\n" +
    "\}\n")


// arm C — the q8u form with a HALF A stream: X pre-converted to f16 once (upstream feeds f16
// activations to its tensor mul_mm); probes whether float-A keeps the op off the NAX-native
// half x half path and halves the A read traffic besides.
let HALFA_Q8_GEMM = (NAX_COMMON +
    "static void tmm_q8h(threadgroup half * wt, device const half * ws, device const char * wq, device const half * a, uint lda, device float * c, uint ldc, uint kk, uint ldwb, uint tid) \{\n" +
    "    constexpr auto d = mpp::tensor_ops::matmul2d_descriptor(32, 64, static_cast<int>(dynamic_extent), false, true, true, mpp::tensor_ops::matmul2d_descriptor::mode::multiply_accumulate);\n" +
    "    mpp::tensor_ops::matmul2d<d, execution_simdgroups<4>> op;\n" +
    "    using AT = tensor<device half, dextents<int32_t, 2>, tensor_inline>;\n" +
    "    using WT = tensor<threadgroup half, dextents<int32_t, 2>, tensor_inline>;\n" +
    "    auto cT = op.get_destination_cooperative_tensor<AT, WT, float>();\n" +
    "    for (uint16_t i = 0; i < cT.get_capacity(); ++i) \{ if (cT.is_valid_element(i)) cT[i] = 0.0f; \}\n" +
    "    const uint nb = kk / 32u;\n" +
    "    for (uint b = 0; b < nb; ++b) \{\n" +
    "        for (uint work = tid; work < 64u * 2u; work += 128u) \{\n" +
    "            const uint col = work >> 1;\n" +
    "            const uint kh = (work & 1u) * 16u;\n" +
    "            const uint blk = col * ldwb + b;\n" +
    "            const float sc = float(ws[blk * 17u]);\n" +
    "            device const char * q = wq + blk * 34u + kh;\n" +
    "            threadgroup half * dstp = wt + col * 32u + kh;\n" +
    "            _Pragma(\"clang loop unroll(full)\") for (short i = 0; i < 16; ++i) \{ dstp[i] = half(float(q[i]) * sc); \}\n" +
    "        \}\n" +
    "        threadgroup_barrier(mem_flags::mem_threadgroup);\n" +
    "        auto A = AT((device half *)(a + b * 32u), dextents<int32_t, 2>(32, 32), metal::array<int32_t, 2>\{1, int(lda)\});\n" +
    "        auto W = WT((threadgroup half *)wt, dextents<int32_t, 2>(32, 64));\n" +
    "        op.run(A, W, cT);\n" +
    "        threadgroup_barrier(mem_flags::mem_threadgroup);\n" +
    "    \}\n" +
    "    auto C = tensor<device float, dextents<int32_t, 2>, tensor_inline>(c, dextents<int32_t, 2>(64, 32), metal::array<int32_t, 2>\{1, int(ldc)\});\n" +
    "    cT.store(C);\n" +
    "\}\n" +
    "kernel void halfa_q8_gemm(device const half* wsh [[buffer(0)]],\n" +
    "        device const char* wqb [[buffer(1)]],\n" +
    "        device const half* xh [[buffer(2)]],\n" +
    "        device float* y [[buffer(3)]],\n" +
    "        constant uint& kdim [[buffer(4)]],\n" +
    "        constant uint& ndim [[buffer(5)]],\n" +
    "        uint tid [[thread_index_in_threadgroup]],\n" +
    "        uint3 wg [[threadgroup_position_in_grid]]) \{\n" +
    "    threadgroup half wt[2048];\n" +
    "    uint mBase = wg.x * 32u;\n" +
    "    uint nBase = wg.y * 64u;\n" +
    "    uint blk0 = nBase * (kdim / 32u);\n" +
    "    tmm_q8h(wt, wsh + blk0 * 17u, wqb + blk0 * 34u + 2u, xh + mBase * kdim, kdim, y + mBase * ndim + nBase, ndim, kdim, kdim / 32u, tid);\n" +
    "\}\n")

// arm D — dequant W to a DEVICE f16 panel (bandwidth pass), then the all-device half x half
// matmul2d (the bf16 twin's form, W as device half): probes whether paying the W panel
// traffic per forward escapes the tg-staging tax (the ~60% law) on the GEMM itself.
// Timed COMBINED (dequant + gemm in one encoder window) — that is the per-forward bill.
let DEVW_Q8_GEMM = (NAX_COMMON +
    "kernel void dequant_q8_wh(device const half* wsh [[buffer(0)]],\n" +
    "        device const char* wqb [[buffer(1)]],\n" +
    "        device half* wh [[buffer(2)]],\n" +
    "        constant uint& nblk [[buffer(3)]],\n" +
    "        uint gid [[thread_position_in_grid]]) \{\n" +
    "    if (gid >= nblk) return;\n" +
    "    const half dscale = wsh[gid * 17u];\n" +
    "    device const char * q = wqb + gid * 34u + 2u;\n" +
    "    device half * o = wh + gid * 32u;\n" +
    "    _Pragma(\"clang loop unroll(full)\") for (short i = 0; i < 32; ++i) \{ o[i] = dscale * half(q[i]); \}\n" +
    "\}\n" +
    "static void tmm_devw(device const half * a, uint lda, device const half * wv, uint ldw, device float * c, uint ldc, uint kk) \{\n" +
    "    auto A = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half *)a, dextents<int32_t, 2>(int(kk), 32), metal::array<int32_t, 2>\{1, int(lda)\});\n" +
    "    auto W = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half *)wv, dextents<int32_t, 2>(int(kk), 64), metal::array<int32_t, 2>\{1, int(ldw)\});\n" +
    "    auto C = tensor<device float, dextents<int32_t, 2>, tensor_inline>(c, dextents<int32_t, 2>(64, 32), metal::array<int32_t, 2>\{1, int(ldc)\});\n" +
    "    constexpr auto d = mpp::tensor_ops::matmul2d_descriptor(32, 64, static_cast<int>(dynamic_extent), false, true, true);\n" +
    "    mpp::tensor_ops::matmul2d<d, execution_simdgroups<4>> op;\n" +
    "    auto cT = op.get_destination_cooperative_tensor<decltype(A), decltype(W), float>();\n" +
    "    for (uint16_t i = 0; i < cT.get_capacity(); ++i) \{ if (cT.is_valid_element(i)) cT[i] = 0.0f; \}\n" +
    "    op.run(A, W, cT);\n" +
    "    cT.store(C);\n" +
    "\}\n" +
    "kernel void devw_q8_gemm(device const half* wh [[buffer(0)]],\n" +
    "        device const half* xh [[buffer(1)]],\n" +
    "        device float* y [[buffer(2)]],\n" +
    "        constant uint& kdim [[buffer(3)]],\n" +
    "        constant uint& ndim [[buffer(4)]],\n" +
    "        uint3 wg [[threadgroup_position_in_grid]]) \{\n" +
    "    uint mBase = wg.x * 32u;\n" +
    "    uint nBase = wg.y * 64u;\n" +
    "    tmm_devw(xh + mBase * kdim, kdim, wh + nBase * kdim, kdim, y + mBase * ndim + nBase, ndim, kdim);\n" +
    "\}\n")

// tall-M-tile twin of the production q8uh helper: IDENTICAL staging/accumulate protocol,
// only the descriptor M and the per-threadgroup A slice grow (upstream's mul_mm runs 128
// act-rows x 64 wcols — M/128 W re-streams vs our M/32; the old 128-tile loss predates
// accumulate mode)
// upstream's kernel_mul_mm_q8_0 (98c4764b6), literal-specialized: same 128x64xK32 tile,
// W dequanted inline to a tg half tile, X streamed from device (f32 in their production
// prefill, f16 twin for the dtype-matched compare), one accumulate-mode cooperative C
def private their_mm_msl(kdim, ndim, m : int; xf16 : bool; inline_t : bool = false; uintidx : bool = false) : string {
    let xt = xf16 ? "half" : "float"
    let it = uintidx ? "uint" : "int"
    let nm = uintidx ? "their_uidx" : (inline_t ? "their_inl" : (xf16 ? "their_f16" : "their_f32"))
    return build_string() $(var w) {
        w |> write("#include <metal_stdlib>\n#include <MetalPerformancePrimitives/MetalPerformancePrimitives.h>\n")
        w |> write("using namespace metal;\nusing namespace mpp::tensor_ops;\n")
        w |> write("struct block_q8h \{ half d; char qs[32]; \};\n")
        w |> write("kernel void {nm}(\n")
        w |> write("    device const char* srcA [[buffer(0)]], device const {xt}* srcB [[buffer(1)]],\n")
        w |> write("    device float* dst [[buffer(2)]],\n")
        w |> write("    uint3 tgpig [[threadgroup_position_in_grid]], ushort tiitg [[thread_index_in_threadgroup]])\n")
        w |> write("\{\n")
        w |> write("  constexpr int K = {kdim}; constexpr int M = {ndim}; constexpr int N = {m};\n")
        w |> write("  const {it} ra = tgpig.y * 64;\n")
        w |> write("  const {it} rb = tgpig.x * 128;\n")
        w |> write("  threadgroup half sa[2048];\n")
        if (inline_t) {
            w |> write("  auto tA = tensor<threadgroup half, dextents<int32_t, 2>, tensor_inline>(sa, dextents<int32_t, 2>(32, 64));\n")
            w |> write("  auto tB = tensor<device {xt}, dextents<int32_t, 2>, tensor_inline>((device {xt}*)srcB, dextents<int32_t, 2>(K, N), array<int, 2>(\{1, K\}));\n")
        } else {
            w |> write("  auto tA = tensor(sa, dextents<int32_t, 2>(32, 64));\n")
            w |> write("  auto tB = tensor((device {xt}*)srcB, dextents<int32_t, 2>(K, N), array<int, 2>(\{1, K\}));\n")
        }
        w |> write("  matmul2d<matmul2d_descriptor(128, 64, 32, false, true, true, matmul2d_descriptor::mode::multiply_accumulate), execution_simdgroups<4>> mm;\n")
        w |> write("  auto cT = mm.get_destination_cooperative_tensor<decltype(tB), decltype(tA), float>();\n")
        w |> write("  for ({it} loop_k = 0; loop_k < K; loop_k += 32) \{\n")
        w |> write("    for ({it} work = tiitg; work < 128; work += 128) \{\n")
        w |> write("      const {it} row = work / 2;\n")
        w |> write("      const {it} k_chunk = work % 2;\n")
        w |> write("      const {it} k_pos = loop_k + k_chunk * 16;\n")
        w |> write("      const short k_base = k_chunk * 16;\n")
        w |> write("      const {it} block_idx = k_pos / 32;\n")
        w |> write("      const {it} il = (k_pos / 16) % 2;\n")
        w |> write("      device const block_q8h * row_ptr = (device const block_q8h *)(srcA + {kdim / 32 * 34} * (ra + row));\n")
        w |> write("      device const char * qs = row_ptr[block_idx].qs;\n")
        w |> write("      const float d = row_ptr[block_idx].d;\n")
        w |> write("      float4x4 reg_f;\n")
        w |> write("      for (int i = 0; i < 16; i++) \{ reg_f[i/4][i%4] = (qs[i + 16*il] * d); \}\n")
        w |> write("      half4x4 temp_a = (half4x4) reg_f;\n")
        w |> write("      _Pragma(\"clang loop unroll(full)\") for (short i = 0; i < 16; ++i) \{\n")
        w |> write("        sa[row * 32 + (k_base + i)] = temp_a[i/4][i%4];\n")
        w |> write("      \}\n")
        w |> write("    \}\n")
        w |> write("    threadgroup_barrier(mem_flags::mem_threadgroup);\n")
        w |> write("    auto mA = tA.slice(0, 0);\n")
        w |> write("    auto mB = tB.slice(int(loop_k), int(rb));\n")
        w |> write("    mm.run(mB, mA, cT);\n")
        w |> write("    threadgroup_barrier(mem_flags::mem_threadgroup);\n")
        w |> write("  \}\n")
        if (inline_t) {
            w |> write("  auto tD = tensor<device float, dextents<int32_t, 2>, tensor_inline>(dst, dextents<int32_t, 2>(M, N), array<int, 2>(\{1, M\}));\n")
        } else {
            w |> write("  auto tD = tensor(dst, dextents<int32_t, 2>(M, N), array<int, 2>(\{1, M\}));\n")
        }
        w |> write("  cT.store(tD.slice(int(ra), int(rb)));\n")
        w |> write("\}\n")
    }
}

// ours verbatim, but K/N baked as literals (per-shape PSO) — the specialization race
def private tile_q8uh_lit_msl(mt, kdim, ndim : int) : string {
    return build_string() $(var w) {
        w |> write("#include <metal_stdlib>\n#include <MetalPerformancePrimitives/MetalPerformancePrimitives.h>\n")
        w |> write("using namespace metal;\nusing namespace mpp::tensor_ops;\n")
        w |> write("kernel void tile{mt}l_q8uh(\n")
        w |> write("    device const half* wsh [[buffer(0)]], device const char* wqb [[buffer(1)]],\n")
        w |> write("    device const half* xh [[buffer(2)]], device float* y [[buffer(3)]],\n")
        w |> write("    constant uint& kdim_u [[buffer(4)]], constant uint& ndim_u [[buffer(5)]],\n")
        w |> write("    uint3 tgp [[threadgroup_position_in_grid]], ushort tid [[thread_index_in_threadgroup]])\n")
        w |> write("\{\n")
        w |> write("  constexpr uint kdim = {kdim}u; constexpr uint ndim = {ndim}u;\n")
        w |> write("  threadgroup half wt[2048];\n")
        w |> write("  const uint ldwb = kdim / 32u;\n")
        w |> write("  const uint blk0 = tgp.y * 64u * ldwb;\n")
        w |> write("  device const half* ws = wsh + blk0 * 17u;\n")
        w |> write("  device const char* wq = wqb + blk0 * 34u + 2u;\n")
        w |> write("  device const half* a = xh + tgp.x * {mt}u * kdim;\n")
        w |> write("  device float* c = y + tgp.x * {mt}u * ndim + tgp.y * 64u;\n")
        w |> write("  constexpr auto d = matmul2d_descriptor({mt}, 64, static_cast<int>(dynamic_extent), false, true, true, matmul2d_descriptor::mode::multiply_accumulate);\n")
        w |> write("  matmul2d<d, execution_simdgroups<4>> op;\n")
        w |> write("  using AT = tensor<device half, dextents<int32_t, 2>, tensor_inline>;\n")
        w |> write("  using WT = tensor<threadgroup half, dextents<int32_t, 2>, tensor_inline>;\n")
        w |> write("  auto cT = op.get_destination_cooperative_tensor<AT, WT, float>();\n")
        w |> write("  for (uint16_t i = 0; i < cT.get_capacity(); ++i) \{ if (cT.is_valid_element(i)) cT[i] = 0.0f; \}\n")
        w |> write("  const uint nb = kdim / 32u;\n")
        w |> write("  for (uint b = 0; b < nb; ++b) \{\n")
        w |> write("    for (uint work = tid; work < 128u; work += 128u) \{\n")
        w |> write("      const uint col = work >> 1; const uint kh = (work & 1u) * 16u;\n")
        w |> write("      const uint blk = col * ldwb + b;\n")
        w |> write("      const float sc = float(ws[blk * 17u]);\n")
        w |> write("      device const char * q = wq + blk * 34u + kh;\n")
        w |> write("      threadgroup half * dstp = wt + col * 32u + kh;\n")
        w |> write("      _Pragma(\"clang loop unroll(full)\") for (short i = 0; i < 16; ++i) \{ dstp[i] = half(float(q[i]) * sc); \}\n")
        w |> write("    \}\n")
        w |> write("    threadgroup_barrier(mem_flags::mem_threadgroup);\n")
        w |> write("    auto A = AT((device half *)(a + b * 32u), dextents<int32_t, 2>(32, {mt}), metal::array<int32_t, 2>\{1, int(kdim)\});\n")
        w |> write("    auto W = WT((threadgroup half *)wt, dextents<int32_t, 2>(32, 64));\n")
        w |> write("    op.run(A, W, cT);\n")
        w |> write("    threadgroup_barrier(mem_flags::mem_threadgroup);\n")
        w |> write("  \}\n")
        w |> write("  auto C = tensor<device float, dextents<int32_t, 2>, tensor_inline>(c, dextents<int32_t, 2>(64, {mt}), metal::array<int32_t, 2>\{1, int(ndim)\});\n")
        w |> write("  cT.store(C);\n")
        w |> write("\}\n")
    }
}

// hoist=true: upstream's matmul idiom — tensors built ONCE outside the K loop, advanced
// by slice(), static-32 descriptor K; the staging phase and C store stay ours
def private tile_q8uh_hoist_msl(mt : int) : string {
    return build_string() $(var w) {
        w |> write("#include <metal_stdlib>\n#include <MetalPerformancePrimitives/MetalPerformancePrimitives.h>\n")
        w |> write("using namespace metal;\nusing namespace mpp::tensor_ops;\n")
        w |> write("kernel void tile{mt}h_q8uh(\n")
        w |> write("    device const half* wsh [[buffer(0)]], device const char* wqb [[buffer(1)]],\n")
        w |> write("    device const half* xh [[buffer(2)]], device float* y [[buffer(3)]],\n")
        w |> write("    constant uint& kdim [[buffer(4)]], constant uint& ndim [[buffer(5)]],\n")
        w |> write("    uint3 tgp [[threadgroup_position_in_grid]], ushort tid [[thread_index_in_threadgroup]])\n")
        w |> write("\{\n")
        w |> write("  threadgroup half wt[2048];\n")
        w |> write("  const uint ldwb = kdim / 32u;\n")
        w |> write("  const uint blk0 = tgp.y * 64u * ldwb;\n")
        w |> write("  device const half* ws = wsh + blk0 * 17u;\n")
        w |> write("  device const char* wq = wqb + blk0 * 34u + 2u;\n")
        w |> write("  constexpr auto d = matmul2d_descriptor({mt}, 64, 32, false, true, true, matmul2d_descriptor::mode::multiply_accumulate);\n")
        w |> write("  matmul2d<d, execution_simdgroups<4>> op;\n")
        w |> write("  auto tW = tensor(wt, dextents<int32_t, 2>(32, 64));\n")
        w |> write("  device const half* a = xh + tgp.x * {mt}u * kdim;\n")
        w |> write("  auto tX = tensor((device half *)a, dextents<int32_t, 2>(int(kdim), {mt}), array<int, 2>(\{1, int(kdim)\}));\n")
        w |> write("  auto cT = op.get_destination_cooperative_tensor<decltype(tX), decltype(tW), float>();\n")
        w |> write("  for (uint16_t i = 0; i < cT.get_capacity(); ++i) \{ if (cT.is_valid_element(i)) cT[i] = 0.0f; \}\n")
        w |> write("  const uint nb = kdim / 32u;\n")
        w |> write("  for (uint b = 0; b < nb; ++b) \{\n")
        w |> write("    for (uint work = tid; work < 128u; work += 128u) \{\n")
        w |> write("      const uint col = work >> 1; const uint kh = (work & 1u) * 16u;\n")
        w |> write("      const uint blk = col * ldwb + b;\n")
        w |> write("      const float sc = float(ws[blk * 17u]);\n")
        w |> write("      device const char * q = wq + blk * 34u + kh;\n")
        w |> write("      threadgroup half * dstp = wt + col * 32u + kh;\n")
        w |> write("      _Pragma(\"clang loop unroll(full)\") for (short i = 0; i < 16; ++i) \{ dstp[i] = half(float(q[i]) * sc); \}\n")
        w |> write("    \}\n")
        w |> write("    threadgroup_barrier(mem_flags::mem_threadgroup);\n")
        w |> write("    auto mX = tX.slice(int(b) * 32, 0);\n")
        w |> write("    auto mW = tW.slice(0, 0);\n")
        w |> write("    op.run(mX, mW, cT);\n")
        w |> write("    threadgroup_barrier(mem_flags::mem_threadgroup);\n")
        w |> write("  \}\n")
        w |> write("  device float * c = y + tgp.x * {mt}u * ndim + tgp.y * 64u;\n")
        w |> write("  auto C = tensor(c, dextents<int32_t, 2>(64, {mt}), array<int, 2>(\{1, int(ndim)\}));\n")
        w |> write("  cT.store(C);\n")
        w |> write("\}\n")
    }
}

def private tile_q8uh_msl(mt : int; kstatic : bool = false; reg_stage : bool = false; nozero : bool = false) : string {
    let kd = kstatic ? "32" : "static_cast<int>(dynamic_extent)"
    return build_string() $(var w) {
        w |> write("#include <metal_stdlib>\n#include <MetalPerformancePrimitives/MetalPerformancePrimitives.h>\n")
        w |> write("using namespace metal;\nusing namespace mpp::tensor_ops;\n")
        w |> write("kernel void tile{mt}{kstatic ? "s" : ""}{reg_stage ? "r" : ""}{nozero ? "z" : ""}_q8uh(\n")
        w |> write("    device const half* wsh [[buffer(0)]], device const char* wqb [[buffer(1)]],\n")
        w |> write("    device const half* xh [[buffer(2)]], device float* y [[buffer(3)]],\n")
        w |> write("    constant uint& kdim [[buffer(4)]], constant uint& ndim [[buffer(5)]],\n")
        w |> write("    uint3 tgp [[threadgroup_position_in_grid]], ushort tid [[thread_index_in_threadgroup]])\n")
        w |> write("\{\n")
        w |> write("  threadgroup half wt[2048];\n")
        w |> write("  const uint ldwb = kdim / 32u;\n")
        w |> write("  const uint blk0 = tgp.y * 64u * ldwb;\n")
        w |> write("  device const half* ws = wsh + blk0 * 17u;\n")
        w |> write("  device const char* wq = wqb + blk0 * 34u + 2u;\n")
        w |> write("  device const half* a = xh + tgp.x * {mt}u * kdim;\n")
        w |> write("  device float* c = y + tgp.x * {mt}u * ndim + tgp.y * 64u;\n")
        w |> write("  constexpr auto d = matmul2d_descriptor({mt}, 64, {kd}, false, true, true, matmul2d_descriptor::mode::multiply_accumulate);\n")
        w |> write("  matmul2d<d, execution_simdgroups<4>> op;\n")
        w |> write("  using AT = tensor<device half, dextents<int32_t, 2>, tensor_inline>;\n")
        w |> write("  using WT = tensor<threadgroup half, dextents<int32_t, 2>, tensor_inline>;\n")
        w |> write("  auto cT = op.get_destination_cooperative_tensor<AT, WT, float>();\n")
        if (!nozero) {
            w |> write("  for (uint16_t i = 0; i < cT.get_capacity(); ++i) \{ if (cT.is_valid_element(i)) cT[i] = 0.0f; \}\n")
        }
        w |> write("  const uint nb = kdim / 32u;\n")
        w |> write("  for (uint b = 0; b < nb; ++b) \{\n")
        w |> write("    for (uint work = tid; work < 128u; work += 128u) \{\n")
        w |> write("      const uint col = work >> 1; const uint kh = (work & 1u) * 16u;\n")
        w |> write("      const uint blk = col * ldwb + b;\n")
        w |> write("      const float sc = float(ws[blk * 17u]);\n")
        w |> write("      device const char * q = wq + blk * 34u + kh;\n")
        w |> write("      threadgroup half * dstp = wt + col * 32u + kh;\n")
        if (reg_stage) {
            w |> write("      float4x4 reg_f;\n")
            w |> write("      for (int i = 0; i < 16; i++) \{ reg_f[i/4][i%4] = float(q[i]) * sc; \}\n")
            w |> write("      half4x4 tmp = (half4x4) reg_f;\n")
            w |> write("      _Pragma(\"clang loop unroll(full)\") for (short i = 0; i < 16; ++i) \{ dstp[i] = tmp[i/4][i%4]; \}\n")
        } else {
            w |> write("      _Pragma(\"clang loop unroll(full)\") for (short i = 0; i < 16; ++i) \{ dstp[i] = half(float(q[i]) * sc); \}\n")
        }
        w |> write("    \}\n")
        w |> write("    threadgroup_barrier(mem_flags::mem_threadgroup);\n")
        w |> write("    auto A = AT((device half *)(a + b * 32u), dextents<int32_t, 2>(32, {mt}), metal::array<int32_t, 2>\{1, int(kdim)\});\n")
        w |> write("    auto W = WT((threadgroup half *)wt, dextents<int32_t, 2>(32, 64));\n")
        w |> write("    op.run(A, W, cT);\n")
        w |> write("    threadgroup_barrier(mem_flags::mem_threadgroup);\n")
        w |> write("  \}\n")
        w |> write("  auto C = tensor<device float, dextents<int32_t, 2>, tensor_inline>(c, dextents<int32_t, 2>(64, {mt}), metal::array<int32_t, 2>\{1, int(ndim)\});\n")
        w |> write("  cT.store(C);\n")
        w |> write("\}\n")
    }
}

// tall twin of the all-device dev-W GEMM (arm D's devw_q8_gemm at M-tile mt): panel
// re-reads drop from M/32 to M/mt — probes whether the cache knee moves with tile height
def private tile_hw_msl(mt : int) : string {
    return build_string() $(var w) {
        w |> write("#include <metal_stdlib>\n#include <MetalPerformancePrimitives/MetalPerformancePrimitives.h>\n")
        w |> write("using namespace metal;\nusing namespace mpp::tensor_ops;\n")
        w |> write("kernel void tile{mt}_hw(device const half* wh [[buffer(0)]],\n")
        w |> write("        device const half* xh [[buffer(1)]],\n")
        w |> write("        device float* y [[buffer(2)]],\n")
        w |> write("        constant uint& kdim [[buffer(3)]],\n")
        w |> write("        constant uint& ndim [[buffer(4)]],\n")
        w |> write("        uint3 wg [[threadgroup_position_in_grid]]) \{\n")
        w |> write("    device const half* a = xh + wg.x * {mt}u * kdim;\n")
        w |> write("    device const half* wv = wh + wg.y * 64u * kdim;\n")
        w |> write("    device float* c = y + wg.x * {mt}u * ndim + wg.y * 64u;\n")
        w |> write("    auto A = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half *)a, dextents<int32_t, 2>(int(kdim), {mt}), metal::array<int32_t, 2>\{1, int(kdim)\});\n")
        w |> write("    auto W = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half *)wv, dextents<int32_t, 2>(int(kdim), 64), metal::array<int32_t, 2>\{1, int(kdim)\});\n")
        w |> write("    auto C = tensor<device float, dextents<int32_t, 2>, tensor_inline>(c, dextents<int32_t, 2>(64, {mt}), metal::array<int32_t, 2>\{1, int(ndim)\});\n")
        w |> write("    constexpr auto d = matmul2d_descriptor({mt}, 64, static_cast<int>(dynamic_extent), false, true, true);\n")
        w |> write("    matmul2d<d, execution_simdgroups<4>> op;\n")
        w |> write("    auto cT = op.get_destination_cooperative_tensor<decltype(A), decltype(W), float>();\n")
        w |> write("    for (uint16_t i = 0; i < cT.get_capacity(); ++i) \{ if (cT.is_valid_element(i)) cT[i] = 0.0f; \}\n")
        w |> write("    op.run(A, W, cT);\n")
        w |> write("    cT.store(C);\n")
        w |> write("\}\n")
    }
}

def private cpu_ref(blob : array<int8>; xf : array<float>; var want : array<float>; m, kdim, ndim : int) {
    let nb = kdim / 32
    unsafe {
        var p8 = addr(blob[0])
        for (r in range(m)) {
            for (j in range(ndim)) {
                var acc = 0.0lf
                for (b in range(nb)) {
                    let blk = j * nb + b
                    var ph = reinterpret<float16?>(p8 + blk * 34)
                    let d = double(float(ph[0]))
                    for (k in range(32)) {
                        acc += d * double(int(blob[blk * 34 + 2 + k])) * double(xf[r * kdim + b * 32 + k])
                    }
                }
                want[r * ndim + j] = float(acc)
            }
        }
    }
}

[export]
def main {   // nolint:STYLE038,STYLE037 — one-shot probe harness; buffers + PSOs stay live to the release tail
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        var err = ""
        var nax_pso = pipeline_from_source(dev, NAX_Q8_GEMM, "nax_q8_gemm", true, err)
        if (nax_pso == null) {
            print("nax pso: {err}\n")
            return
        }
        var q8u_pso = pipeline_from_source(dev, MetalQ8MulMmT_metal_q8_mulmm_t_msl, MetalQ8MulMmT_metal_q8_mulmm_t_msl_entry, MetalQ8MulMmT_metal_q8_mulmm_t_msl_fastmath, err)
        if (q8u_pso == null) {
            print("q8u pso: {err}\n")
            return
        }
        var i8_pso = pipeline_from_source(dev, TGI8_Q8_GEMM, "tgi8_q8_gemm", true, err)
        if (i8_pso == null) {
            print("tg-i8 pso: {err}\n")
            return
        }
        var ha_pso = pipeline_from_source(dev, HALFA_Q8_GEMM, "halfa_q8_gemm", true, err)
        if (ha_pso == null) {
            print("half-A pso: {err}\n")
            return
        }
        var th_pso = pipeline_from_source(dev, MetalQ8MulMmTH_metal_q8_mulmm_t_msl, MetalQ8MulMmTH_metal_q8_mulmm_t_msl_entry, MetalQ8MulMmTH_metal_q8_mulmm_t_msl_fastmath, err)
        if (th_pso == null) {
            print("q8uh(TH) pso: {err}\n")
            return
        }
        var dq_pso = pipeline_from_source(dev, DEVW_Q8_GEMM, "dequant_q8_wh", true, err)
        if (dq_pso == null) {
            print("dequant pso: {err}\n")
            return
        }
        var dw_pso = pipeline_from_source(dev, DEVW_Q8_GEMM, "devw_q8_gemm", true, err)
        if (dw_pso == null) {
            print("devw pso: {err}\n")
            return
        }
        // the 1B's dominant shapes: gate/up (K=2048 N=8192), down (K=8192 N=2048), qkv-ish
        for (shp in [(m = 2048, k = 2048, n = 8192), (m = 2048, k = 8192, n = 2048), (m = 2048, k = 2048, n = 2048),
                     (m = 512, k = 2048, n = 8192), (m = 512, k = 8192, n = 2048), (m = 128, k = 2048, n = 8192)]) {
            let m = shp.m
            let kdim = shp.k
            let ndim = shp.n
            var blob <- race_q8_blob(ndim * (kdim / 32))
            var xf <- race_x_f32(m * kdim)
            var bw = race_buf(dev, uint64(long_length(blob)), unsafe(addr<void?>(blob[0])))
            var bx = race_buf(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xf[0])))
            var by_a = race_buf(dev, uint64(m * ndim * 4), null)
            var by_b = race_buf(dev, uint64(m * ndim * 4), null)
            var bk = race_uniform_u32(dev, uint(kdim))
            var bn = race_uniform_u32(dev, uint(ndim))
            var t_q8u = -1.0lf
            var t_nax = -1.0lf
            race_pair_ms(queue, 5, t_q8u, t_nax, $(enc) {
                kn_pipeline(enc, q8u_pso)
                kn_tgmem(enc, MetalQ8MulMmT_metal_q8_mulmm_t_msl_tgmem, 0)
                kn_buffer(enc, bw, 0ul, 0)
                kn_buffer(enc, bw, 0ul, 1)
                kn_buffer(enc, bx, 0ul, 2)
                kn_buffer(enc, by_a, 0ul, 3)
                kn_buffer(enc, bk, 0ul, 4)
                kn_buffer(enc, bn, 0ul, 5)
                kn_dispatch(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
            }, $(enc) {
                kn_pipeline(enc, nax_pso)
                kn_buffer(enc, bw, 0ul, 0)
                kn_buffer(enc, bw, 0ul, 1)
                kn_buffer(enc, bx, 0ul, 2)
                kn_buffer(enc, by_b, 0ul, 3)
                kn_buffer(enc, bk, 0ul, 4)
                kn_buffer(enc, bn, 0ul, 5)
                kn_dispatch(enc, uint3(uint(m / 64), uint(ndim / 256), 1u), uint3(128u, 1u, 1u))
            })
            var t_q8u2 = -1.0lf
            var t_i8 = -1.0lf
            race_pair_ms(queue, 5, t_q8u2, t_i8, $(enc) {
                kn_pipeline(enc, q8u_pso)
                kn_tgmem(enc, MetalQ8MulMmT_metal_q8_mulmm_t_msl_tgmem, 0)
                kn_buffer(enc, bw, 0ul, 0)
                kn_buffer(enc, bw, 0ul, 1)
                kn_buffer(enc, bx, 0ul, 2)
                kn_buffer(enc, by_a, 0ul, 3)
                kn_buffer(enc, bk, 0ul, 4)
                kn_buffer(enc, bn, 0ul, 5)
                kn_dispatch(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
            }, $(enc) {
                kn_pipeline(enc, i8_pso)
                kn_buffer(enc, bw, 0ul, 0)
                kn_buffer(enc, bw, 0ul, 1)
                kn_buffer(enc, bx, 0ul, 2)
                kn_buffer(enc, by_b, 0ul, 3)
                kn_buffer(enc, bk, 0ul, 4)
                kn_buffer(enc, bn, 0ul, 5)
                kn_dispatch(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
            })
            var note2 = ""
            let ok2 = race_envelope_ok(by_a, by_b, m * ndim, note2)
            let verdict2 = ok2 ? "outputs-agree" : note2
            print("  tg-i8: q8u={t_q8u2}ms i8={t_i8}ms ratio={t_i8 / t_q8u2} {verdict2}\n")
            var xh : array<float16>
            xh |> resize(m * kdim)
            for (i in range(m * kdim)) {
                xh[i] = float16(xf[i])
            }
            var bxh = race_buf(dev, uint64(m * kdim * 2), unsafe(addr<void?>(xh[0])))
            var t_q8u3 = -1.0lf
            var t_ha = -1.0lf
            race_pair_ms(queue, 5, t_q8u3, t_ha, $(enc) {
                kn_pipeline(enc, q8u_pso)
                kn_tgmem(enc, MetalQ8MulMmT_metal_q8_mulmm_t_msl_tgmem, 0)
                kn_buffer(enc, bw, 0ul, 0)
                kn_buffer(enc, bw, 0ul, 1)
                kn_buffer(enc, bx, 0ul, 2)
                kn_buffer(enc, by_a, 0ul, 3)
                kn_buffer(enc, bk, 0ul, 4)
                kn_buffer(enc, bn, 0ul, 5)
                kn_dispatch(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
            }, $(enc) {
                kn_pipeline(enc, ha_pso)
                kn_buffer(enc, bw, 0ul, 0)
                kn_buffer(enc, bw, 0ul, 1)
                kn_buffer(enc, bxh, 0ul, 2)
                kn_buffer(enc, by_b, 0ul, 3)
                kn_buffer(enc, bk, 0ul, 4)
                kn_buffer(enc, bn, 0ul, 5)
                kn_dispatch(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
            })
            var note3 = ""
            let ok3 = race_envelope_ok(by_a, by_b, m * ndim, note3)
            let verdict3 = ok3 ? "outputs-agree" : note3
            print("  half-A: q8u={t_q8u3}ms halfA={t_ha}ms ratio={t_ha / t_q8u3} {verdict3}\n")
            // arm D vs the PRODUCTION f16-stream twin (the shipping >=256-row form): both
            // read the same f16 activation panel; arm D's window pays dequant + gemm
            let nblk = ndim * (kdim / 32)
            var bwh = race_buf(dev, uint64(ndim * kdim * 2), null)
            var bnb = race_uniform_u32(dev, uint(nblk))
            var t_th = -1.0lf
            var t_dw = -1.0lf
            race_pair_ms(queue, 5, t_th, t_dw, $(enc) {
                kn_pipeline(enc, th_pso)
                kn_tgmem(enc, MetalQ8MulMmTH_metal_q8_mulmm_t_msl_tgmem, 0)
                kn_buffer(enc, bw, 0ul, 0)
                kn_buffer(enc, bw, 0ul, 1)
                kn_buffer(enc, bxh, 0ul, 2)
                kn_buffer(enc, by_a, 0ul, 3)
                kn_buffer(enc, bk, 0ul, 4)
                kn_buffer(enc, bn, 0ul, 5)
                kn_dispatch(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
            }, $(enc) {
                kn_pipeline(enc, dq_pso)
                kn_buffer(enc, bw, 0ul, 0)
                kn_buffer(enc, bw, 0ul, 1)
                kn_buffer(enc, bwh, 0ul, 2)
                kn_buffer(enc, bnb, 0ul, 3)
                kn_dispatch(enc, uint3(uint(nblk / 128), 1u, 1u), uint3(128u, 1u, 1u))
                kn_pipeline(enc, dw_pso)
                kn_buffer(enc, bwh, 0ul, 0)
                kn_buffer(enc, bxh, 0ul, 1)
                kn_buffer(enc, by_b, 0ul, 2)
                kn_buffer(enc, bk, 0ul, 3)
                kn_buffer(enc, bn, 0ul, 4)
                kn_dispatch(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
            })
            let t_dw_gemm = race_time_ms(queue, 5) $(enc) {   // wh stays populated: the GEMM-only split
                kn_pipeline(enc, dw_pso)
                kn_buffer(enc, bwh, 0ul, 0)
                kn_buffer(enc, bxh, 0ul, 1)
                kn_buffer(enc, by_b, 0ul, 2)
                kn_buffer(enc, bk, 0ul, 3)
                kn_buffer(enc, bn, 0ul, 4)
                kn_dispatch(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
            }
            var note4 = ""
            let ok4 = race_envelope_ok(by_a, by_b, m * ndim, note4)
            let verdict4 = ok4 ? "outputs-agree" : note4
            print("  dev-W: q8uh={t_th}ms dequant+devw={t_dw}ms (gemm-only {t_dw_gemm}ms) ratio={t_dw / t_th} {verdict4}\n")
            metal_release(bwh)
            metal_release(bnb)
            metal_release(bxh)
            delete xh
            var note = ""
            let ok = race_envelope_ok(by_a, by_b, m * ndim, note)
            let verdict = ok ? "outputs-agree" : note
            print("K={kdim} N={ndim}: q8u={t_q8u}ms nax={t_nax}ms ratio={t_nax / t_q8u} {verdict}\n")
            // spot CPU truth on a corner slab (full oracle is O(m*k*n) — 64 rows suffice)
            if (kdim == 2048 && ndim == 2048) {
                var want : array<float>
                want |> resize(64 * ndim)
                cpu_ref(blob, xf, want, 64, kdim, ndim)
                var bad = 0
                unsafe {
                    let pg = reinterpret<float?>(metal_buffer_contents(by_b))
                    for (i in range(64 * ndim)) {
                        let d = abs(pg[i] - want[i])
                        if (d > 2e-3 * max(abs(want[i]), 1.0)) {
                            bad++
                        }
                    }
                }
                if (bad == 0) {
                    print("  CPU spot-check (64 rows): within envelope\n")
                } else {
                    print("  CPU spot-check (64 rows): {bad} mismatches\n")
                }
            }
            metal_release(bw)
            metal_release(bx)
            metal_release(by_a)
            metal_release(by_b)
            metal_release(bk)
            metal_release(bn)
            delete blob
            delete xf
        }
        // ===== the M5 dev-W shape grid: production q8uh (tg-staged) vs dequant+devw =====
        // (K, N) pairs are real model sites: E2B kv/up, 1B qkv/up/down, 4B up/down,
        // 8B qkv/up/down; ratio < 1 = dev-W wins the combined window
        print("dev-W shape grid (ratio = (dequant+devw)/q8uh):\n")
        for (g in [(k = 1536, n = 512), (k = 2048, n = 2048), (k = 1536, n = 8192), (k = 2048, n = 8192),
                   (k = 2560, n = 9728), (k = 8192, n = 2048), (k = 9728, n = 2560), (k = 4096, n = 4096),
                   (k = 4096, n = 14336), (k = 14336, n = 4096)]) {
            let kdim = g.k
            let ndim = g.n
            let nblk = ndim * (kdim / 32)
            var blob <- race_q8_blob(nblk)
            var bw = race_buf(dev, uint64(long_length(blob)), unsafe(addr<void?>(blob[0])))
            var bwh = race_buf(dev, uint64(ndim * kdim * 2), null)
            var bk = race_uniform_u32(dev, uint(kdim))
            var bn = race_uniform_u32(dev, uint(ndim))
            var bnb = race_uniform_u32(dev, uint(nblk))
            let mb = ndim * kdim * 2 / (1024 * 1024)
            for (m in [128, 512, 2048]) {
                var xh <- race_x_f16(m * kdim)
                var bxh = race_buf(dev, uint64(m * kdim * 2), unsafe(addr<void?>(xh[0])))
                var by_a = race_buf(dev, uint64(m * ndim * 4), null)
                var by_b = race_buf(dev, uint64(m * ndim * 4), null)
                var t_th = -1.0lf
                var t_dw = -1.0lf
                race_pair_ms(queue, 5, t_th, t_dw, $(enc) {
                    kn_pipeline(enc, th_pso)
                    kn_tgmem(enc, MetalQ8MulMmTH_metal_q8_mulmm_t_msl_tgmem, 0)
                    kn_buffer(enc, bw, 0ul, 0)
                    kn_buffer(enc, bw, 0ul, 1)
                    kn_buffer(enc, bxh, 0ul, 2)
                    kn_buffer(enc, by_a, 0ul, 3)
                    kn_buffer(enc, bk, 0ul, 4)
                    kn_buffer(enc, bn, 0ul, 5)
                    kn_dispatch(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                }, $(enc) {
                    kn_pipeline(enc, dq_pso)
                    kn_buffer(enc, bw, 0ul, 0)
                    kn_buffer(enc, bw, 0ul, 1)
                    kn_buffer(enc, bwh, 0ul, 2)
                    kn_buffer(enc, bnb, 0ul, 3)
                    kn_dispatch(enc, uint3(uint((nblk + 127) / 128), 1u, 1u), uint3(128u, 1u, 1u))
                    kn_pipeline(enc, dw_pso)
                    kn_buffer(enc, bwh, 0ul, 0)
                    kn_buffer(enc, bxh, 0ul, 1)
                    kn_buffer(enc, by_b, 0ul, 2)
                    kn_buffer(enc, bk, 0ul, 3)
                    kn_buffer(enc, bn, 0ul, 4)
                    kn_dispatch(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                })
                var gnote = ""
                let gok = race_envelope_ok(by_a, by_b, m * ndim, gnote)
                print("  K={kdim} N={ndim} M={m} panel={mb}MB: ratio={t_dw / t_th} (q8uh={t_th}ms devw={t_dw}ms) {gok ? "ok" : gnote}\n")
                metal_release(bxh)
                metal_release(by_a)
                metal_release(by_b)
                delete xh
            }
            metal_release(bw)
            metal_release(bwh)
            metal_release(bk)
            metal_release(bn)
            metal_release(bnb)
            delete blob
        }
        // ===== N-tiled dev-W: the map's over-knee losers split into column tiles, each
        // dequant+GEMM pair under the 32MB cache knee (double-buffered panels pipeline) =====
        print("dev-W N-tiled grid (ratio vs q8uh):\n")
        for (gt in [(k = 2560, n = 9728, m = 512, tiles = 2), (k = 2560, n = 9728, m = 2048, tiles = 2),
                    (k = 4096, n = 14336, m = 512, tiles = 4), (k = 4096, n = 14336, m = 2048, tiles = 4),
                    (k = 14336, n = 4096, m = 2048, tiles = 4)]) {
            let kdim = gt.k
            let ndim = gt.n
            let m = gt.m
            let ntile = ndim / gt.tiles
            let nb = kdim / 32
            var blob <- race_q8_blob(ndim * nb)
            var bw = race_buf(dev, uint64(long_length(blob)), unsafe(addr<void?>(blob[0])))
            var bwh1 = race_buf(dev, uint64(ntile * kdim * 2), null)
            var bwh2 = race_buf(dev, uint64(ntile * kdim * 2), null)
            var xh <- race_x_f16(m * kdim)
            var bxh = race_buf(dev, uint64(m * kdim * 2), unsafe(addr<void?>(xh[0])))
            var by_a = race_buf(dev, uint64(m * ndim * 4), null)
            var by_b = race_buf(dev, uint64(m * ndim * 4), null)
            var bk = race_uniform_u32(dev, uint(kdim))
            var bn = race_uniform_u32(dev, uint(ndim))
            var bnb_t = race_uniform_u32(dev, uint(ntile * nb))
            var t_th = -1.0lf
            var t_dw = -1.0lf
            race_pair_ms(queue, 5, t_th, t_dw, $(enc) {
                kn_pipeline(enc, th_pso)
                kn_tgmem(enc, MetalQ8MulMmTH_metal_q8_mulmm_t_msl_tgmem, 0)
                kn_buffer(enc, bw, 0ul, 0)
                kn_buffer(enc, bw, 0ul, 1)
                kn_buffer(enc, bxh, 0ul, 2)
                kn_buffer(enc, by_a, 0ul, 3)
                kn_buffer(enc, bk, 0ul, 4)
                kn_buffer(enc, bn, 0ul, 5)
                kn_dispatch(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
            }, $(enc) {
                for (ti in range(gt.tiles)) {
                    let tbase = ti * ntile
                    var bwh = (ti & 1) == 0 ? bwh1 : bwh2
                    kn_pipeline(enc, dq_pso)
                    kn_buffer(enc, bw, uint64(tbase * nb * 34), 0)
                    kn_buffer(enc, bw, uint64(tbase * nb * 34), 1)
                    kn_buffer(enc, bwh, 0ul, 2)
                    kn_buffer(enc, bnb_t, 0ul, 3)
                    kn_dispatch(enc, uint3(uint((ntile * nb + 127) / 128), 1u, 1u), uint3(128u, 1u, 1u))
                    kn_pipeline(enc, dw_pso)
                    kn_buffer(enc, bwh, 0ul, 0)
                    kn_buffer(enc, bxh, 0ul, 1)
                    kn_buffer(enc, by_b, uint64(tbase * 4), 2)
                    kn_buffer(enc, bk, 0ul, 3)
                    kn_buffer(enc, bn, 0ul, 4)
                    kn_dispatch(enc, uint3(uint(m / 32), uint(ntile / 64), 1u), uint3(128u, 1u, 1u))
                }
            })
            var tnote = ""
            let tok2 = race_envelope_ok(by_a, by_b, m * ndim, tnote)
            let tmb = ntile * kdim * 2 / (1024 * 1024)
            print("  K={kdim} N={ndim} M={m} x{gt.tiles} tiles ({tmb}MB each): ratio={t_dw / t_th} (q8uh={t_th}ms tiled={t_dw}ms) {tok2 ? "ok" : tnote}\n")
            metal_release(bw)
            metal_release(bwh1)
            metal_release(bwh2)
            metal_release(bxh)
            metal_release(by_a)
            metal_release(by_b)
            metal_release(bk)
            metal_release(bn)
            metal_release(bnb_t)
            delete blob
            delete xh
        }
        // ===== tile-height race: the production 32x64 q8uh vs 64/128-tall twins (same
        // protocol, W re-streamed M/mt times instead of M/32) =====
        var t64_pso = pipeline_from_source(dev, tile_q8uh_msl(64), "tile64_q8uh", true, err)
        var t128_pso = pipeline_from_source(dev, tile_q8uh_msl(128), "tile128_q8uh", true, err)
        if (t64_pso == null || t128_pso == null) {
            print("tile pso: {err}\n")
        } else {
            print("tile-height race (ratio = tall/q8uh32; <1 = tall wins):\n")
            for (g in [(k = 2560, n = 9728, m = 512), (k = 9728, n = 2560, m = 512),
                       (k = 2560, n = 9728, m = 2048), (k = 9728, n = 2560, m = 2048),
                       (k = 4096, n = 14336, m = 512), (k = 2048, n = 8192, m = 2048),
                       (k = 2560, n = 9728, m = 128)]) {
                let kdim = g.k
                let ndim = g.n
                let m = g.m
                var blob <- race_q8_blob(ndim * (kdim / 32))
                var bw = race_buf(dev, uint64(long_length(blob)), unsafe(addr<void?>(blob[0])))
                var xh <- race_x_f16(m * kdim)
                var bxh = race_buf(dev, uint64(m * kdim * 2), unsafe(addr<void?>(xh[0])))
                var by_a = race_buf(dev, uint64(m * ndim * 4), null)
                var by_b = race_buf(dev, uint64(m * ndim * 4), null)
                var bk = race_uniform_u32(dev, uint(kdim))
                var bn = race_uniform_u32(dev, uint(ndim))
                for (leg in [(pso = t64_pso, mt = 64, name = "t64"), (pso = t128_pso, mt = 128, name = "t128")]) {
                    if (m % leg.mt != 0) {
                        continue
                    }
                    var t_th = -1.0lf
                    var t_tall = -1.0lf
                    race_pair_ms(queue, 5, t_th, t_tall, $(enc) {
                        kn_pipeline(enc, th_pso)
                        kn_tgmem(enc, MetalQ8MulMmTH_metal_q8_mulmm_t_msl_tgmem, 0)
                        kn_buffer(enc, bw, 0ul, 0)
                        kn_buffer(enc, bw, 0ul, 1)
                        kn_buffer(enc, bxh, 0ul, 2)
                        kn_buffer(enc, by_a, 0ul, 3)
                        kn_buffer(enc, bk, 0ul, 4)
                        kn_buffer(enc, bn, 0ul, 5)
                        kn_dispatch(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                    }, $(enc) {
                        kn_pipeline(enc, leg.pso)
                        kn_buffer(enc, bw, 0ul, 0)
                        kn_buffer(enc, bw, 0ul, 1)
                        kn_buffer(enc, bxh, 0ul, 2)
                        kn_buffer(enc, by_b, 0ul, 3)
                        kn_buffer(enc, bk, 0ul, 4)
                        kn_buffer(enc, bn, 0ul, 5)
                        kn_dispatch(enc, uint3(uint(m / leg.mt), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                    })
                    var hnote = ""
                    let hok = race_envelope_ok(by_a, by_b, m * ndim, hnote)
                    print("  K={kdim} N={ndim} M={m} {leg.name}: ratio={t_tall / t_th} (q8uh32={t_th}ms tall={t_tall}ms) {hok ? "ok" : hnote}\n")
                }
                metal_release(bw)
                metal_release(bxh)
                metal_release(by_a)
                metal_release(by_b)
                metal_release(bk)
                metal_release(bn)
                delete blob
                delete xh
            }
        }
        // ===== descriptor-K race: dynamic_extent vs static 32 (upstream's mul_mm carries
        // static K in the descriptor; every run() here is a fixed 32-chunk, so dynamic is
        // gratuitous — does MPP specialize?) =====
        var t128d_pso = pipeline_from_source(dev, tile_q8uh_msl(128), "tile128_q8uh", true, err)
        var t128s_pso = pipeline_from_source(dev, tile_q8uh_msl(128, kstatic = true), "tile128s_q8uh", true, err)
        if (t128d_pso == null || t128s_pso == null) {
            print("kstatic pso: {err}\n")
        } else {
            print("descriptor-K race (ratio = static32/dynamic; <1 = static wins):\n")
            for (g in [(k = 2560, n = 9728, m = 512), (k = 9728, n = 2560, m = 512),
                       (k = 2560, n = 4096, m = 512), (k = 4096, n = 2560, m = 512),
                       (k = 2560, n = 9728, m = 2048), (k = 9728, n = 2560, m = 2048)]) {
                let kdim = g.k
                let ndim = g.n
                let m = g.m
                var blob <- race_q8_blob(ndim * (kdim / 32))
                var bw = race_buf(dev, uint64(long_length(blob)), unsafe(addr<void?>(blob[0])))
                var xh <- race_x_f16(m * kdim)
                var bxh = race_buf(dev, uint64(m * kdim * 2), unsafe(addr<void?>(xh[0])))
                var by_a = race_buf(dev, uint64(m * ndim * 4), null)
                var by_b = race_buf(dev, uint64(m * ndim * 4), null)
                var bk = race_uniform_u32(dev, uint(kdim))
                var bn = race_uniform_u32(dev, uint(ndim))
                var t_dyn = -1.0lf
                var t_sta = -1.0lf
                race_pair_ms(queue, 5, t_dyn, t_sta, $(enc) {
                    kn_pipeline(enc, t128d_pso)
                    kn_buffer(enc, bw, 0ul, 0)
                    kn_buffer(enc, bw, 0ul, 1)
                    kn_buffer(enc, bxh, 0ul, 2)
                    kn_buffer(enc, by_a, 0ul, 3)
                    kn_buffer(enc, bk, 0ul, 4)
                    kn_buffer(enc, bn, 0ul, 5)
                    kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                }, $(enc) {
                    kn_pipeline(enc, t128s_pso)
                    kn_buffer(enc, bw, 0ul, 0)
                    kn_buffer(enc, bw, 0ul, 1)
                    kn_buffer(enc, bxh, 0ul, 2)
                    kn_buffer(enc, by_b, 0ul, 3)
                    kn_buffer(enc, bk, 0ul, 4)
                    kn_buffer(enc, bn, 0ul, 5)
                    kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                })
                var knote = ""
                let kok = race_envelope_ok(by_a, by_b, m * ndim, knote)
                print("  K={kdim} N={ndim} M={m}: ratio={t_sta / t_dyn} (dyn={t_dyn}ms static={t_sta}ms) {kok ? "ok" : knote}\n")
                metal_release(bw)
                metal_release(bxh)
                metal_release(by_a)
                metal_release(by_b)
                metal_release(bk)
                metal_release(bn)
                delete blob
                delete xh
            }
        }
        // ===== hoist-idiom race: our kernel on their matmul idiom (hoisted tensors +
        // slice + static K; staging/store stay ours) vs our per-chunk-construction form =====
        var t128h_pso = pipeline_from_source(dev, tile_q8uh_hoist_msl(128), "tile128h_q8uh", true, err)
        if (t128h_pso == null) {
            print("hoist pso: {err}\n")
        } else {
            var tnz_pso = pipeline_from_source(dev, tile_q8uh_msl(128, nozero = true), "tile128z_q8uh", true, err)
            if (tnz_pso != null) {
                print("no-zero-init race (ratio = nozero/ours; <1 = dropping the init loop wins):\n")
                for (g in [(k = 2560, n = 9728, m = 512), (k = 9728, n = 2560, m = 512),
                           (k = 2560, n = 4096, m = 512), (k = 2560, n = 9728, m = 2048)]) {
                    let kdim = g.k
                    let ndim = g.n
                    let m = g.m
                    var blob <- race_q8_blob(ndim * (kdim / 32))
                    var bw = race_buf(dev, uint64(long_length(blob)), unsafe(addr<void?>(blob[0])))
                    var xh <- race_x_f16(m * kdim)
                    var bxh = race_buf(dev, uint64(m * kdim * 2), unsafe(addr<void?>(xh[0])))
                    var by_a = race_buf(dev, uint64(m * ndim * 4), null)
                    var by_b = race_buf(dev, uint64(m * ndim * 4), null)
                    var bk = race_uniform_u32(dev, uint(kdim))
                    var bn = race_uniform_u32(dev, uint(ndim))
                    var t_ours = -1.0lf
                    var t_nz = -1.0lf
                    race_pair_ms(queue, 5, t_ours, t_nz, $(enc) {
                        kn_pipeline(enc, t128d_pso)
                        kn_buffer(enc, bw, 0ul, 0)
                        kn_buffer(enc, bw, 0ul, 1)
                        kn_buffer(enc, bxh, 0ul, 2)
                        kn_buffer(enc, by_a, 0ul, 3)
                        kn_buffer(enc, bk, 0ul, 4)
                        kn_buffer(enc, bn, 0ul, 5)
                        kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                    }, $(enc) {
                        kn_pipeline(enc, tnz_pso)
                        kn_buffer(enc, bw, 0ul, 0)
                        kn_buffer(enc, bw, 0ul, 1)
                        kn_buffer(enc, bxh, 0ul, 2)
                        kn_buffer(enc, by_b, 0ul, 3)
                        kn_buffer(enc, bk, 0ul, 4)
                        kn_buffer(enc, bn, 0ul, 5)
                        kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                    })
                    var znote = ""
                    let zok = race_envelope_ok(by_a, by_b, m * ndim, znote)
                    print("  K={kdim} N={ndim} M={m}: ratio={t_nz / t_ours} (ours={t_ours}ms nozero={t_nz}ms) {zok ? "ok" : znote}\n")
                    metal_release(bw)
                    metal_release(bxh)
                    metal_release(by_a)
                    metal_release(by_b)
                    metal_release(bk)
                    metal_release(bn)
                    delete blob
                    delete xh
                }
            }
            var tregs_pso = pipeline_from_source(dev, tile_q8uh_msl(128, reg_stage = true), "tile128r_q8uh", true, err)
            if (tregs_pso != null) {
                print("reg-staging race (ratio = regstage/ours; <1 = their staging wins):\n")
                for (g in [(k = 2560, n = 9728, m = 512), (k = 9728, n = 2560, m = 512),
                           (k = 4096, n = 2560, m = 512), (k = 2560, n = 4096, m = 512),
                           (k = 2560, n = 9728, m = 2048)]) {
                    let kdim = g.k
                    let ndim = g.n
                    let m = g.m
                    var blob <- race_q8_blob(ndim * (kdim / 32))
                    var bw = race_buf(dev, uint64(long_length(blob)), unsafe(addr<void?>(blob[0])))
                    var xh <- race_x_f16(m * kdim)
                    var bxh = race_buf(dev, uint64(m * kdim * 2), unsafe(addr<void?>(xh[0])))
                    var by_a = race_buf(dev, uint64(m * ndim * 4), null)
                    var by_b = race_buf(dev, uint64(m * ndim * 4), null)
                    var bk = race_uniform_u32(dev, uint(kdim))
                    var bn = race_uniform_u32(dev, uint(ndim))
                    var t_ours = -1.0lf
                    var t_reg = -1.0lf
                    race_pair_ms(queue, 5, t_ours, t_reg, $(enc) {
                        kn_pipeline(enc, t128d_pso)
                        kn_buffer(enc, bw, 0ul, 0)
                        kn_buffer(enc, bw, 0ul, 1)
                        kn_buffer(enc, bxh, 0ul, 2)
                        kn_buffer(enc, by_a, 0ul, 3)
                        kn_buffer(enc, bk, 0ul, 4)
                        kn_buffer(enc, bn, 0ul, 5)
                        kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                    }, $(enc) {
                        kn_pipeline(enc, tregs_pso)
                        kn_buffer(enc, bw, 0ul, 0)
                        kn_buffer(enc, bw, 0ul, 1)
                        kn_buffer(enc, bxh, 0ul, 2)
                        kn_buffer(enc, by_b, 0ul, 3)
                        kn_buffer(enc, bk, 0ul, 4)
                        kn_buffer(enc, bn, 0ul, 5)
                        kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                    })
                    var rnote = ""
                    let rok = race_envelope_ok(by_a, by_b, m * ndim, rnote)
                    print("  K={kdim} N={ndim} M={m}: ratio={t_reg / t_ours} (ours={t_ours}ms regstage={t_reg}ms) {rok ? "ok" : rnote}\n")
                    metal_release(bw)
                    metal_release(bxh)
                    metal_release(by_a)
                    metal_release(by_b)
                    metal_release(bk)
                    metal_release(bn)
                    delete blob
                    delete xh
                }
            }
            print("shape-literal race (ratio = literal/ours; <1 = literal wins):\n")
            for (g in [(k = 2560, n = 9728, m = 512), (k = 9728, n = 2560, m = 512),
                       (k = 4096, n = 2560, m = 512), (k = 2560, n = 9728, m = 2048)]) {
                var tlit_pso = pipeline_from_source(dev, tile_q8uh_lit_msl(128, g.k, g.n), "tile128l_q8uh", true, err)
                if (tlit_pso == null) {
                    print("lit pso: {err}\n")
                    continue
                }
                {
                    let kdim = g.k
                    let ndim = g.n
                    let m = g.m
                    var blob <- race_q8_blob(ndim * (kdim / 32))
                    var bw = race_buf(dev, uint64(long_length(blob)), unsafe(addr<void?>(blob[0])))
                    var xh <- race_x_f16(m * kdim)
                    var bxh = race_buf(dev, uint64(m * kdim * 2), unsafe(addr<void?>(xh[0])))
                    var by_a = race_buf(dev, uint64(m * ndim * 4), null)
                    var by_b = race_buf(dev, uint64(m * ndim * 4), null)
                    var bk = race_uniform_u32(dev, uint(kdim))
                    var bn = race_uniform_u32(dev, uint(ndim))
                    var t_ours = -1.0lf
                    var t_lit = -1.0lf
                    race_pair_ms(queue, 5, t_ours, t_lit, $(enc) {
                        kn_pipeline(enc, t128d_pso)
                        kn_buffer(enc, bw, 0ul, 0)
                        kn_buffer(enc, bw, 0ul, 1)
                        kn_buffer(enc, bxh, 0ul, 2)
                        kn_buffer(enc, by_a, 0ul, 3)
                        kn_buffer(enc, bk, 0ul, 4)
                        kn_buffer(enc, bn, 0ul, 5)
                        kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                    }, $(enc) {
                        kn_pipeline(enc, tlit_pso)
                        kn_buffer(enc, bw, 0ul, 0)
                        kn_buffer(enc, bw, 0ul, 1)
                        kn_buffer(enc, bxh, 0ul, 2)
                        kn_buffer(enc, by_b, 0ul, 3)
                        kn_buffer(enc, bk, 0ul, 4)
                        kn_buffer(enc, bn, 0ul, 5)
                        kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                    })
                    var lnote = ""
                    let lok = race_envelope_ok(by_a, by_b, m * ndim, lnote)
                    print("  K={kdim} N={ndim} M={m}: ratio={t_lit / t_ours} (ours={t_ours}ms literal={t_lit}ms) {lok ? "ok" : lnote}\n")
                    metal_release(bw)
                    metal_release(bxh)
                    metal_release(by_a)
                    metal_release(by_b)
                    metal_release(bk)
                    metal_release(bn)
                    delete blob
                    delete xh
                }
                metal_release(tlit_pso)
            }
        }
        if (false) {
            print("hoist-idiom race (ratio = hoist/ours; <1 = hoist wins):\n")
            for (g in [(k = 2560, n = 9728, m = 512), (k = 9728, n = 2560, m = 512),
                       (k = 4096, n = 2560, m = 512), (k = 2560, n = 9728, m = 2048)]) {
                let kdim = g.k
                let ndim = g.n
                let m = g.m
                var blob <- race_q8_blob(ndim * (kdim / 32))
                var bw = race_buf(dev, uint64(long_length(blob)), unsafe(addr<void?>(blob[0])))
                var xh <- race_x_f16(m * kdim)
                var bxh = race_buf(dev, uint64(m * kdim * 2), unsafe(addr<void?>(xh[0])))
                var by_a = race_buf(dev, uint64(m * ndim * 4), null)
                var by_b = race_buf(dev, uint64(m * ndim * 4), null)
                var bk = race_uniform_u32(dev, uint(kdim))
                var bn = race_uniform_u32(dev, uint(ndim))
                var t_ours = -1.0lf
                var t_hoist = -1.0lf
                race_pair_ms(queue, 5, t_ours, t_hoist, $(enc) {
                    kn_pipeline(enc, t128d_pso)
                    kn_buffer(enc, bw, 0ul, 0)
                    kn_buffer(enc, bw, 0ul, 1)
                    kn_buffer(enc, bxh, 0ul, 2)
                    kn_buffer(enc, by_a, 0ul, 3)
                    kn_buffer(enc, bk, 0ul, 4)
                    kn_buffer(enc, bn, 0ul, 5)
                    kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                }, $(enc) {
                    kn_pipeline(enc, t128h_pso)
                    kn_buffer(enc, bw, 0ul, 0)
                    kn_buffer(enc, bw, 0ul, 1)
                    kn_buffer(enc, bxh, 0ul, 2)
                    kn_buffer(enc, by_b, 0ul, 3)
                    kn_buffer(enc, bk, 0ul, 4)
                    kn_buffer(enc, bn, 0ul, 5)
                    kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                })
                var hnote2 = ""
                let hok2 = race_envelope_ok(by_a, by_b, m * ndim, hnote2)
                print("  K={kdim} N={ndim} M={m}: ratio={t_hoist / t_ours} (ours={t_ours}ms hoist={t_hoist}ms) {hok2 ? "ok" : hnote2}\n")
                metal_release(bw)
                metal_release(bxh)
                metal_release(by_a)
                metal_release(by_b)
                metal_release(bk)
                metal_release(bn)
                delete blob
                delete xh
            }
        }
        // ===== upstream-kernel race: their kernel_mul_mm_q8_0 (ported verbatim, literal-
        // specialized) vs our t128 q8uh — the direct per-GEMM comparison; f32 X = their
        // production form (no cvt pass on their side), f16 X = the dtype-matched twin =====
        print("upstream-kernel race (ratio = theirs/ours; <1 = theirs wins):\n")
        for (g in [(k = 2560, n = 9728, m = 512), (k = 9728, n = 2560, m = 512),
                   (k = 4096, n = 2560, m = 512), (k = 2560, n = 4096, m = 512),
                   (k = 2560, n = 9728, m = 2048)]) {
            let kdim = g.k
            let ndim = g.n
            let m = g.m
            var thf32_pso = pipeline_from_source(dev, their_mm_msl(kdim, ndim, m, false), "their_f32", true, err)
            var thf16_pso = pipeline_from_source(dev, their_mm_msl(kdim, ndim, m, true), "their_f16", true, err)
            var thinl_pso = pipeline_from_source(dev, their_mm_msl(kdim, ndim, m, true, inline_t = true), "their_inl", true, err)
            var thuidx_pso = pipeline_from_source(dev, their_mm_msl(kdim, ndim, m, true, uintidx = true), "their_uidx", true, err)
            if (thinl_pso != null) {
                var xh2 <- race_x_f16(m * kdim)
                var bxh2 = race_buf(dev, uint64(m * kdim * 2), unsafe(addr<void?>(xh2[0])))
                var blob2 <- race_q8_blob(ndim * (kdim / 32))
                var bw2 = race_buf(dev, uint64(long_length(blob2)), unsafe(addr<void?>(blob2[0])))
                var bya2 = race_buf(dev, uint64(m * ndim * 4), null)
                var byb2 = race_buf(dev, uint64(m * ndim * 4), null)
                var t_def = -1.0lf
                var t_inl = -1.0lf
                race_pair_ms(queue, 5, t_def, t_inl, $(enc) {
                    kn_pipeline(enc, thf16_pso)
                    kn_buffer(enc, bw2, 0ul, 0)
                    kn_buffer(enc, bxh2, 0ul, 1)
                    kn_buffer(enc, bya2, 0ul, 2)
                    kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                }, $(enc) {
                    kn_pipeline(enc, thinl_pso)
                    kn_buffer(enc, bw2, 0ul, 0)
                    kn_buffer(enc, bxh2, 0ul, 1)
                    kn_buffer(enc, byb2, 0ul, 2)
                    kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                })
                print("  K={kdim} N={ndim} M={m} INLINE-FLAVOR: ratio={t_inl / t_def} (default={t_def}ms tensor_inline={t_inl}ms)\n")
                metal_release(bw2)
                metal_release(bxh2)
                metal_release(bya2)
                metal_release(byb2)
                delete blob2
                delete xh2
            }
            if (thf32_pso == null || thf16_pso == null) {
                print("their pso: {err}\n")
                continue
            }
            print("  PSO caps: ours max={metal_pipeline_max_total_threads(t128d_pso)} theirs max={metal_pipeline_max_total_threads(thf16_pso)}\n")
            var blob <- race_q8_blob(ndim * (kdim / 32))
            var bw = race_buf(dev, uint64(long_length(blob)), unsafe(addr<void?>(blob[0])))
            var xh <- race_x_f16(m * kdim)
            var xf : array<float>
            xf |> resize(m * kdim)
            for (i in range(m * kdim)) {
                xf[i] = float(xh[i])
            }
            var bxh = race_buf(dev, uint64(m * kdim * 2), unsafe(addr<void?>(xh[0])))
            var bxf = race_buf(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xf[0])))
            var by_a = race_buf(dev, uint64(m * ndim * 4), null)
            var by_b = race_buf(dev, uint64(m * ndim * 4), null)
            var bk = race_uniform_u32(dev, uint(kdim))
            var bn = race_uniform_u32(dev, uint(ndim))
            for (leg in [(pso = thf32_pso, bx = bxf, name = "f32X"), (pso = thf16_pso, bx = bxh, name = "f16X"),
                         (pso = thinl_pso, bx = bxh, name = "inlX"), (pso = thuidx_pso, bx = bxh, name = "uidxX")]) {
                if (leg.pso == null) {
                    continue
                }
                var t_ours = -1.0lf
                var t_them = -1.0lf
                race_pair_ms(queue, 5, t_ours, t_them, $(enc) {
                    kn_pipeline(enc, t128d_pso)
                    kn_buffer(enc, bw, 0ul, 0)
                    kn_buffer(enc, bw, 0ul, 1)
                    kn_buffer(enc, bxh, 0ul, 2)
                    kn_buffer(enc, by_a, 0ul, 3)
                    kn_buffer(enc, bk, 0ul, 4)
                    kn_buffer(enc, bn, 0ul, 5)
                    kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                }, $(enc) {
                    kn_pipeline(enc, leg.pso)
                    kn_buffer(enc, bw, 0ul, 0)
                    kn_buffer(enc, leg.bx, 0ul, 1)
                    kn_buffer(enc, by_b, 0ul, 2)
                    kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                })
                var unote = ""
                let uok = race_envelope_ok(by_a, by_b, m * ndim, unote)
                print("  K={kdim} N={ndim} M={m} {leg.name}: ratio={t_them / t_ours} (ours={t_ours}ms theirs={t_them}ms) {uok ? "ok" : unote}\n")
            }
            metal_release(bw)
            metal_release(bxh)
            metal_release(bxf)
            metal_release(by_a)
            metal_release(by_b)
            metal_release(bk)
            metal_release(bn)
            metal_release(thf32_pso)
            metal_release(thf16_pso)
            delete blob
            delete xh
            delete xf
        }
        // ===== tall dev-W GEMM: the all-device form at M-tile 128 vs 32 (GEMM-only over a
        // pre-dequanted panel — does the cache knee move with 4x fewer panel re-reads?) =====
        var h128_pso = pipeline_from_source(dev, tile_hw_msl(128), "tile128_hw", true, err)
        if (h128_pso == null) {
            print("tile hw pso: {err}\n")
        } else {
            print("tall dev-W GEMM race (ratio = hw128/hw32, GEMM-only):\n")
            for (g in [(k = 2560, n = 9728, m = 512), (k = 4096, n = 14336, m = 512),
                       (k = 2560, n = 9728, m = 2048), (k = 4096, n = 14336, m = 2048)]) {
                let kdim = g.k
                let ndim = g.n
                let m = g.m
                let nblk = ndim * (kdim / 32)
                var blob <- race_q8_blob(nblk)
                var bw = race_buf(dev, uint64(long_length(blob)), unsafe(addr<void?>(blob[0])))
                var bwh = race_buf(dev, uint64(ndim * kdim * 2), null)
                var bnb = race_uniform_u32(dev, uint(nblk))
                var xh <- race_x_f16(m * kdim)
                var bxh = race_buf(dev, uint64(m * kdim * 2), unsafe(addr<void?>(xh[0])))
                var by_a = race_buf(dev, uint64(m * ndim * 4), null)
                var by_b = race_buf(dev, uint64(m * ndim * 4), null)
                var bk = race_uniform_u32(dev, uint(kdim))
                var bn = race_uniform_u32(dev, uint(ndim))
                let warm = race_time_ms(queue, 1) $(enc) {   // populate the panel once, untimed
                    kn_pipeline(enc, dq_pso)
                    kn_buffer(enc, bw, 0ul, 0)
                    kn_buffer(enc, bw, 0ul, 1)
                    kn_buffer(enc, bwh, 0ul, 2)
                    kn_buffer(enc, bnb, 0ul, 3)
                    kn_dispatch(enc, uint3(uint((nblk + 127) / 128), 1u, 1u), uint3(128u, 1u, 1u))
                }
                var t_h32 = -1.0lf
                var t_h128 = -1.0lf
                race_pair_ms(queue, 5, t_h32, t_h128, $(enc) {
                    kn_pipeline(enc, dw_pso)
                    kn_buffer(enc, bwh, 0ul, 0)
                    kn_buffer(enc, bxh, 0ul, 1)
                    kn_buffer(enc, by_a, 0ul, 2)
                    kn_buffer(enc, bk, 0ul, 3)
                    kn_buffer(enc, bn, 0ul, 4)
                    kn_dispatch(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                }, $(enc) {
                    kn_pipeline(enc, h128_pso)
                    kn_buffer(enc, bwh, 0ul, 0)
                    kn_buffer(enc, bxh, 0ul, 1)
                    kn_buffer(enc, by_b, 0ul, 2)
                    kn_buffer(enc, bk, 0ul, 3)
                    kn_buffer(enc, bn, 0ul, 4)
                    kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
                })
                var wnote = ""
                let wok = race_envelope_ok(by_a, by_b, m * ndim, wnote)
                let pmb = ndim * kdim * 2 / (1024 * 1024)
                print("  K={kdim} N={ndim} M={m} panel={pmb}MB: ratio={t_h128 / t_h32} (hw32={t_h32}ms hw128={t_h128}ms warm={warm}ms) {wok ? "ok" : wnote}\n")
                metal_release(bw)
                metal_release(bwh)
                metal_release(bnb)
                metal_release(bxh)
                metal_release(by_a)
                metal_release(by_b)
                metal_release(bk)
                metal_release(bn)
                delete blob
                delete xh
            }
            metal_release(h128_pso)
        }
        if (t64_pso != null) {
            metal_release(t64_pso)
        }
        if (t128_pso != null) {
            metal_release(t128_pso)
        }
        metal_release(nax_pso)
        metal_release(i8_pso)
        metal_release(q8u_pso)
        metal_release(th_pso)
        metal_release(dq_pso)
        metal_release(dw_pso)
        metal_release(queue)
        metal_release(dev)
    }
}
