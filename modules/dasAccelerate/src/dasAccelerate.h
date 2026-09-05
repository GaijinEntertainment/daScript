#pragma once

#include "daScript/misc/platform.h"

// the das_accelerate binding surface, declared for AOT-generated C++ (Module::aotRequire)

namespace das {
    DAS_MOD_API void accel_sgemm_nt(int32_t m, int32_t n, int32_t k,
                                    const float * a, int32_t lda,
                                    const float * b, int32_t ldb,
                                    float * c, int32_t ldc);
    DAS_MOD_API void accel_sgemv_n(int32_t n, int32_t k,
                                   const float * w, int32_t ldw,
                                   const float * x, float * y);
    DAS_MOD_API int32_t accel_bnns_hgemm_nt(int32_t m, int32_t n, int32_t k,
                                            const uint16_t * a, int32_t lda,
                                            const uint16_t * b, int32_t ldb,
                                            float * c, int32_t ldc);
    DAS_MOD_API int32_t accel_threading_mode();
}
