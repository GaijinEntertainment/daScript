options gen2
options _dasllama_internal = true

// NAX register-fragment GEMM probe (M5 arc): is the per-simdgroup 16x32x16 matmul2d with
// DEVICE->REGISTER operands (no threadgroup staging, no barriers) faster than the crowned
// q8u tg-staged form at the 1B's dominant GEMM shapes? Hand-MSL arms raced against the
// production PSO on one queue; numerics gated against a CPU dequant dot. Decides whether
// the emitter grows a nax rail — measurement only, nothing here ships.

require math
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
