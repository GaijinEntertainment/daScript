#pragma once

#define FPE_DISABLE FpeDisable  __no_fp_##__LINE__;

#ifdef _MSC_VER
#else
#include <fenv.h>
#endif

namespace das {
    class FpeDisable {
    public:
        __forceinline FpeDisable() {
#ifdef _MSC_VER
            _controlfp_s(&excepts, 0, 0);
            _controlfp_s(0, _MCW_EM, _MCW_EM);
#else
            fegetexceptflag(&excepts,FE_ALL_EXCEPT);
            feclearexcept(FE_ALL_EXCEPT);
#endif

        }
        __forceinline ~FpeDisable() {
#ifdef _MSCVER
            _clearfp();
            _controlfp_s(0, excepts, _MCW_EM);
#else
            fesetexceptflag(&excepts,FE_ALL_EXCEPT);
#endif
        }
    private:
#ifdef _MSCVER
        unsigned int excepts;
#else
        fexcept_t excepts;
#endif
    };
}
