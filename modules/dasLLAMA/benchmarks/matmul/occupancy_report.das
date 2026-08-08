options gen2
options persistent_heap

// Per-kernel occupancy report over every production Metal kernel: maxTotalThreadsPerThreadgroup
// is the register-pressure oracle — a reg-limited PSO reports < 1024 and runs fewer resident
// simdgroups (the Phase-7 GEMM chase root cause). Run after ANY kernel change:
//   bin/daslang modules/dasLLAMA/benchmarks/matmul/occupancy_report.das
// Known tiers: q8_gemm64 = 704 (16 accumulators = 32 regs/lane of C — the 64x64 geometry's
// floor); attn_qk/av_mm = 896 (f32 source values live across the staging barrier).
require metal/das_metal_boost
require dasllama/dasllama_metal_prefill
require dasllama/dasllama_metal_kernels
require dasllama/dasllama_metal_gemm
require strings

def check(dev; name, src, entry : string; fastmath : bool) {
    var err : string
    var pso = pipeline_from_source(dev, src, entry, fastmath, err)
    if (pso == null) {
        print("{name}: COMPILE FAIL {err}\n")
        return
    }
    let mt = int(metal_pipeline_max_total_threads(pso))
    print("{name}: max_threads={mt}{mt < 1024 ? "  <-- REG-LIMITED" : ""}\n")
    metal_release(pso)
}

[export]
def main {
    with_metal_device() $(dev : MetalDevice?) {
        if (dev == null) {
            return
        }
        check(dev, "q8_mulmm (prod GEMM)", metal_q8_mulmm_msl, metal_q8_mulmm_msl_entry, metal_q8_mulmm_msl_fastmath)
        check(dev, "q8_gemm 32x32", metal_q8_gemm_msl, metal_q8_gemm_msl_entry, metal_q8_gemm_msl_fastmath)
        check(dev, "q8_gemm64 64x64", metal_q8_gemm64_msl, metal_q8_gemm64_msl_entry, metal_q8_gemm64_msl_fastmath)
        check(dev, "attn_qk_mm", metal_attn_qk_mm_msl, metal_attn_qk_mm_msl_entry, metal_attn_qk_mm_msl_fastmath)
        check(dev, "attn_av_mm", metal_attn_av_mm_msl, metal_attn_av_mm_msl_entry, metal_attn_av_mm_msl_fastmath)
        check(dev, "attn_qk (classic)", metal_attn_qk_msl, metal_attn_qk_msl_entry, metal_attn_qk_msl_fastmath)
        check(dev, "attn_av (classic)", metal_attn_av_msl, metal_attn_av_msl_entry, metal_attn_av_msl_fastmath)
        check(dev, "attn_softmax", metal_attn_softmax_msl, metal_attn_softmax_msl_entry, metal_attn_softmax_msl_fastmath)
        check(dev, "rmsnorm", metal_rmsnorm_msl, metal_rmsnorm_msl_entry, metal_rmsnorm_msl_fastmath)
        check(dev, "gemv (logits)", metal_q8_gemv_msl, metal_q8_gemv_msl_entry, metal_q8_gemv_msl_fastmath)
        check(dev, "swiglu", metal_swiglu_msl, metal_swiglu_msl_entry, metal_swiglu_msl_fastmath)
        check(dev, "add", metal_add_msl, metal_add_msl_entry, metal_add_msl_fastmath)
        check(dev, "rope", metal_rope_msl, metal_rope_msl_entry, metal_rope_msl_fastmath)
        check(dev, "kq_mulmm_k4", metal_kq_mulmm_k4_msl, metal_kq_mulmm_k4_msl_entry, metal_kq_mulmm_k4_msl_fastmath)
        check(dev, "kq_mulmm_k5", metal_kq_mulmm_k5_msl, metal_kq_mulmm_k5_msl_entry, metal_kq_mulmm_k5_msl_fastmath)
        check(dev, "kq_mulmm_k6", metal_kq_mulmm_k6_msl, metal_kq_mulmm_k6_msl_entry, metal_kq_mulmm_k6_msl_fastmath)
        check(dev, "kq_gemv_k4", metal_kq_gemv_k4_msl, metal_kq_gemv_k4_msl_entry, metal_kq_gemv_k4_msl_fastmath)
        check(dev, "kq_gemv_k5", metal_kq_gemv_k5_msl, metal_kq_gemv_k5_msl_entry, metal_kq_gemv_k5_msl_fastmath)
        check(dev, "kq_gemv_k6", metal_kq_gemv_k6_msl, metal_kq_gemv_k6_msl_entry, metal_kq_gemv_k6_msl_fastmath)
        check(dev, "kq_mvb2_k4", MetalKqMvB2K4_metal_kq_mv_k4_msl, MetalKqMvB2K4_metal_kq_mv_k4_msl_entry, MetalKqMvB2K4_metal_kq_mv_k4_msl_fastmath)
        check(dev, "kq_mvb4_k4", MetalKqMvB4K4_metal_kq_mv_k4_msl, MetalKqMvB4K4_metal_kq_mv_k4_msl_entry, MetalKqMvB4K4_metal_kq_mv_k4_msl_fastmath)
        check(dev, "kq_mvb2_k5", MetalKqMvB2K5_metal_kq_mv_k5_msl, MetalKqMvB2K5_metal_kq_mv_k5_msl_entry, MetalKqMvB2K5_metal_kq_mv_k5_msl_fastmath)
        check(dev, "kq_mvb4_k5", MetalKqMvB4K5_metal_kq_mv_k5_msl, MetalKqMvB4K5_metal_kq_mv_k5_msl_entry, MetalKqMvB4K5_metal_kq_mv_k5_msl_fastmath)
        check(dev, "kq_mvb2_k6", MetalKqMvB2K6_metal_kq_mv_k6_msl, MetalKqMvB2K6_metal_kq_mv_k6_msl_entry, MetalKqMvB2K6_metal_kq_mv_k6_msl_fastmath)
        check(dev, "kq_mvb4_k6", MetalKqMvB4K6_metal_kq_mv_k6_msl, MetalKqMvB4K6_metal_kq_mv_k6_msl_entry, MetalKqMvB4K6_metal_kq_mv_k6_msl_fastmath)
    }
}
