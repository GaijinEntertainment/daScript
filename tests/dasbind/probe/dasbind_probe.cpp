#include <stdint.h>

#if defined(_WIN32)
#define DASBIND_PROBE_API extern "C" __declspec(dllexport)
#else
#define DASBIND_PROBE_API extern "C" __attribute__((visibility("default")))
#endif

DASBIND_PROBE_API double dasbind_probe_dbl_at45 ( int64_t a0, int64_t a1, int64_t a2, int64_t a3, double a4, double a5 ) {
    return double(a0) + double(a1)*10 + double(a2)*100 + double(a3)*1000 + a4*2 + a5*3;
}

DASBIND_PROBE_API double dasbind_probe_dbl_at6 ( int64_t a0, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, double a6, int64_t a7 ) {
    return double(a0) + double(a1)*10 + double(a2)*100 + double(a3)*1000 + double(a4)*10000 + double(a5)*100000 + a6*2 + double(a7)*1000000;
}

DASBIND_PROBE_API double dasbind_probe_dbl_at67 ( int64_t a0, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, double a6, double a7 ) {
    return double(a0) + double(a1)*10 + double(a2)*100 + double(a3)*1000 + double(a4)*10000 + double(a5)*100000 + a6*2 + a7*3;
}

DASBIND_PROBE_API int64_t dasbind_probe_regs7 ( int8_t a0, int16_t a1, int32_t a2, int64_t a3, float a4, double a5, int32_t a6 ) {
    return a0 + a1*10 + a2*100 + a3*1000 + int64_t(a4*4) + int64_t(a5*8) + int64_t(a6)*7;
}

DASBIND_PROBE_API int64_t dasbind_probe_stack14 ( int8_t a0, int16_t a1, int32_t a2, int64_t a3, float a4, double a5, int32_t a6,
                                                  int8_t a7, int8_t a8, int16_t a9, int32_t a10, int64_t a11, bool a12, int32_t a13 ) {
    return a0 + a1*10 + a2*100 + a3*1000 + int64_t(a4*4) + int64_t(a5*8) + int64_t(a6)*7
         + a7*11 + a8*13 + a9*17 + int64_t(a10)*19 + a11*23 + (a12 ? 29 : 0) + int64_t(a13)*31;
}

DASBIND_PROBE_API float dasbind_probe_stack_narrow ( float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7,
                                                     int64_t a8, int64_t a9, int64_t a10, int64_t a11, int64_t a12, int64_t a13, int64_t a14, int64_t a15,
                                                     int8_t a16, int16_t a17, float a18, int32_t a19 ) {
    return a0*1 + a1*2 + a2*3 + a3*4 + a4*5 + a5*6 + a6*7 + a7*8
         + float(a8*9 + a9*10 + a10*11 + a11*12 + a12*13 + a13*14 + a14*15 + a15*16)
         + float(a16)*17 + float(a17)*18 + a18*19 + float(a19)*20;
}

DASBIND_PROBE_API int64_t dasbind_probe_ref_short ( const float * r0, int64_t a1, const int32_t * r2 ) {
    return int64_t(*r0 * 2) + a1*3 + int64_t(*r2)*5;
}

DASBIND_PROBE_API int64_t dasbind_probe_ref_args ( const float * r0, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6, int64_t a7, const int32_t * r8 ) {
    return int64_t(*r0 * 2) + a1*3 + a2*4 + a3*5 + a4*6 + a5*7 + a6*8 + a7*9 + int64_t(*r8)*10;
}

DASBIND_PROBE_API const float * dasbind_probe_ref_result ( int64_t a0, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6, const float * src ) {
    return (a0 + a1 + a2 + a3 + a4 + a5 + a6) == 28 ? src : nullptr;
}

DASBIND_PROBE_API double dasbind_probe_stack_dbl_result ( double a0, int64_t a1, int64_t a2, int64_t a3, int64_t a4,
                                                      int64_t a5, int64_t a6, int64_t a7, int64_t a8, int32_t a9 ) {
    return a0 + double(a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8) + double(a9)*2;
}
