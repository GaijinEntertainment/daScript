// das_accelerate — thin binding over Apple's Accelerate BLAS (AMX on M1-M3, SME on M4+).
// Deliberately minimal: the das side owns all layout, scratch, and dispatch decisions
// (modules/dasLLAMA/dasllama/dasllama_math_accelerate.das); this module is only the calls
// daslang cannot express. Apple-only (see CMakeLists.txt).

#define ACCELERATE_NEW_LAPACK
#include <Accelerate/Accelerate.h>

#include "daScript/daScript.h"

using namespace das;

// BLASSetThreading/BLASGetThreading exist only in the macOS 15 / iOS 18 SDK headers — the
// __builtin_available checks below gate RUNTIME, but an older SDK fails at COMPILE time
// (undeclared identifier on the macOS 14 CI images), so the call sites need this guard too.
#if (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 150000) || \
    (defined(__IPHONE_OS_VERSION_MAX_ALLOWED) && __IPHONE_OS_VERSION_MAX_ALLOWED >= 180000)
    #define DAS_ACCEL_HAS_BLAS_THREADING 1
#else
    #define DAS_ACCEL_HAS_BLAS_THREADING 0
#endif

// Accelerate's internal pool starves against the spinning jobque (~50ms/call), and its threading
// model is PER-THREAD state (thread_api.h: "saved in a thread local variable") — so the pin must
// happen on every calling lane, not at init. Callers parallelize by strip-dispatching over the
// jobque instead.
static thread_local bool t_blas_pinned = false;
static inline void accel_pin_single_thread() {
    if (t_blas_pinned) return;
    t_blas_pinned = true;
#if DAS_ACCEL_HAS_BLAS_THREADING
    if (__builtin_available(macOS 15.0, iOS 18.0, tvOS 18.0, watchOS 11.0, *)) {
        BLASSetThreading(BLAS_THREADING_SINGLE_THREADED);
    }
#endif
}

// C[m x n] = A[m x k] * B[n x k]^T, row-major — the exact call shape ggml-blas.cpp uses for
// mul_mat (activations A stay token-major, weights B stay row-major, no pre-transpose).
static void accel_sgemm_nt(int32_t m, int32_t n, int32_t k,
                           const float * a, int32_t lda,
                           const float * b, int32_t ldb,
                           float * c, int32_t ldc) {
    accel_pin_single_thread();
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans,
                m, n, k,
                1.0f, a, lda,
                      b, ldb,
                0.0f, c, ldc);
}

// y[n] = W[n x k] * x[k], row-major — the decode-lane probe (uncrowned on M1; here for the
// M4/M5-SME applicability sweep, never gated out).
static void accel_sgemv_n(int32_t n, int32_t k,
                          const float * w, int32_t ldw,
                          const float * x, float * y) {
    accel_pin_single_thread();
    cblas_sgemv(CblasRowMajor, CblasNoTrans, n, k, 1.0f, w, ldw, x, 1, 0.0f, y, 1);
}

// C[m x n] f32 (row stride ldc) = A[m x k] f16 * B[n x k] f16^T via a dynamic BNNS
// BroadcastMatMul filter — f16 AMX arithmetic with a PROVEN f32 accumulator, pinned to the
// calling thread (n_threads=1); callers strip-dispatch over the jobque exactly like sgemm.
// Filter create+destroy is ~0.2us (bnns_hgemm_probe), so per-call creation is free and no
// pointer-lifetime coupling exists. rc: 0 = ok, nonzero = BNNS unavailable or refused —
// the caller falls back to the portable path.
static int32_t accel_bnns_hgemm_nt(int32_t m, int32_t n, int32_t k,
                                   const uint16_t * a, int32_t lda,
                                   const uint16_t * b, int32_t ldb,
                                   float * c, int32_t ldc) {
    if (__builtin_available(macOS 11.0, iOS 14.0, tvOS 14.0, watchOS 7.0, *)) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"   // classic BNNS: deprecated 15.0 for BNNSGraph, still the only C-callable dynamic matmul
        BNNSLayerParametersBroadcastMatMul p = {};
        p.alpha = 1.0f;
        p.transB = true;
        auto desc = [](BNNSNDArrayDescriptor & dsc, BNNSDataType t, size_t cols, size_t rows, size_t rowStride) {
            dsc.layout = BNNSDataLayoutRowMajorMatrix;
            dsc.size[0] = cols;
            dsc.size[1] = rows;
            dsc.stride[0] = 1;
            dsc.stride[1] = rowStride;
            dsc.data_type = t;
        };
        desc(p.iA_desc, BNNSDataTypeFloat16, (size_t)k, (size_t)m, (size_t)lda);
        desc(p.iB_desc, BNNSDataTypeFloat16, (size_t)k, (size_t)n, (size_t)ldb);
        desc(p.o_desc,  BNNSDataTypeFloat32, (size_t)n, (size_t)m, (size_t)ldc);
        BNNSFilterParameters fp = {};
        fp.n_threads = 1;
        BNNSFilter f = BNNSFilterCreateLayerBroadcastMatMul(&p, &fp);
        if (!f) return -2;
        int rc = BNNSFilterApplyTwoInput(f, a, b, c);
        BNNSFilterDestroy(f);
#pragma clang diagnostic pop
        return rc;
    }
    return -1;
}

// Calling thread's effective BLAS threading mode after the pin: 0 = multi, 1 = single,
// -1 = BLASSetThreading unavailable (pre-macOS-15). Diagnostics for the contention rig.
static int32_t accel_threading_mode() {
    accel_pin_single_thread();
#if DAS_ACCEL_HAS_BLAS_THREADING
    if (__builtin_available(macOS 15.0, iOS 18.0, tvOS 18.0, watchOS 11.0, *)) {
        return (int32_t)BLASGetThreading();
    }
#endif
    return -1;
}

namespace das {

class Module_DasAccelerate : public Module {
public:
    Module_DasAccelerate() : Module("das_accelerate") {
        // pre-macOS-15 fallback for the per-thread pin above (env read at Accelerate image init,
        // so this only helps when the module loads before first BLAS use; explicit env still wins)
        setenv("VECLIB_MAXIMUM_THREADS", "1", 0);
        ModuleLibrary lib(this);
        lib.addBuiltInModule();
        addExtern<DAS_BIND_FUN(accel_sgemm_nt)>(*this, lib, "accel_sgemm_nt",
            SideEffects::modifyExternal, "accel_sgemm_nt")
                ->args({"m", "n", "k", "a", "lda", "b", "ldb", "c", "ldc"});
        addExtern<DAS_BIND_FUN(accel_sgemv_n)>(*this, lib, "accel_sgemv_n",
            SideEffects::modifyExternal, "accel_sgemv_n")
                ->args({"n", "k", "w", "ldw", "x", "y"});
        addExtern<DAS_BIND_FUN(accel_bnns_hgemm_nt)>(*this, lib, "accel_bnns_hgemm_nt",
            SideEffects::modifyExternal, "accel_bnns_hgemm_nt")
                ->args({"m", "n", "k", "a", "lda", "b", "ldb", "c", "ldc"});
        addExtern<DAS_BIND_FUN(accel_threading_mode)>(*this, lib, "accel_threading_mode",
            SideEffects::modifyExternal, "accel_threading_mode");
    }
};

REGISTER_DYN_MODULE(Module_DasAccelerate, Module_DasAccelerate);

} // namespace das

REGISTER_MODULE_IN_NAMESPACE(Module_DasAccelerate, das);
