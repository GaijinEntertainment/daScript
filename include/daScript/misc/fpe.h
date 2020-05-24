#pragma once

#define FPE_DISABLE FpeDisable  __no_fp_##__LINE__;

#include <fenv.h>

namespace das {
    class FpeDisable {
    public:
        __forceinline FpeDisable() {
            fegetexceptflag(&excepts,FE_ALL_EXCEPT);
            feclearexcept(FE_ALL_EXCEPT);
        }
        __forceinline ~FpeDisable() {
            fesetexceptflag(&excepts,FE_ALL_EXCEPT);
        }
    private:
        fexcept_t excepts;
    };

    __forceinline void enable_all_fpe() {
        feraiseexcept(FE_ALL_EXCEPT);
    }
}
