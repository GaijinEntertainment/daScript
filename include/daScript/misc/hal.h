#pragma once

#if defined(_MSC_VER) || __APPLE__
    #ifdef _MSC_VER
        #include <intrin.h>
    #endif
    #include <xmmintrin.h>
#elif _TARGET_SIMD_NEON // PSP2, iOS
    #include <arm_neon.h>
#else
    #error "unsupported target"
#endif

namespace das {
    
#if defined(_MSC_VER) || __APPLE__
    typedef __m128 vec4f;
    typedef __m128i vec4i;
// casts
    __forceinline vec4f vec_cast_esi_ps ( const vec4i a ) {
        return _mm_castsi128_ps(a);
    }
    __forceinline vec4i vec_cast_ps_esi ( const vec4f a ) {
        return _mm_castps_si128(a);
    }
    __forceinline float vec_cast_ps_ss ( vec4f a ) {
        return _mm_cvtss_f32(a);
    }
    __forceinline vec4f vec_cast_ss_ps ( float a ) {
        return _mm_set_ss(a);
    }
    __forceinline int32_t vec_cast_esi_int32 ( vec4i a ) {
        return _mm_cvtsi128_si32(a);
    }
    __forceinline vec4i vec_cast_int32_esi ( int32_t a ) {
        return _mm_set1_epi32(a);
    }
    __forceinline int64_t vec_cast_esi_int64 ( vec4i a ) {
#if defined(_MSC_VER)
		return a.m128i_i64[0];
#else
        int64_t t; _mm_storel_epi64((__m128i*)&t, a); return t;
#endif
    }
    __forceinline vec4i vec_cast_int64_esi ( int64_t a ) {
        return _mm_loadl_epi64((__m128i const*)&a);
    }
    __forceinline void * vec_cast_esi_ptr ( vec4i a ) {
#if INTPTR_MAX == INT32_MAX
		return (void*) _mm_cvtsi128_si32(a);
#else
#if defined(_MSC_VER)
		return (void*)a.m128i_u64[0];
#else
		void * t; _mm_storel_epi64((__m128i*)&t, a); return t;
#endif
#endif

    }
    __forceinline vec4i vec_cast_ptr_esi ( void * a ) {
        #if INTPTR_MAX == INT32_MAX
        return _mm_set1_epi32((uint32_t)a);
        #else
        return _mm_loadl_epi64((__m128i const*)&a);
        #endif
    }
// vec4
    __forceinline vec4f vec_set_ps_xyzw ( float x, float y, float z, float w ) {
        return _mm_setr_ps(x,y,z,w);
    }
    __forceinline vec4f vec_setzero_ps () {
        return _mm_setzero_ps();
    }
    __forceinline vec4f vec_loadu_ps ( const float * src ) {
        return _mm_loadu_ps(src);
    }
    __forceinline vec4f vec_neg_ps ( vec4f a ) {
        return _mm_sub_ps(_mm_setzero_ps(), a);
    }
    __forceinline vec4f vec_add_ps ( vec4f a, vec4f b ) {
        return _mm_add_ps(a,b);
    }
    __forceinline vec4f vec_sub_ps ( vec4f a, vec4f b ) {
        return _mm_sub_ps(a,b);
    }
    __forceinline vec4f vec_div_ps ( vec4f a, vec4f b ) {
        return _mm_div_ps(a,b);
    }
    __forceinline vec4f vec_mul_ps ( vec4f a, vec4f b ) {
        return _mm_mul_ps(a,b);
    }
    __forceinline vec4f vec_shuffle_ps_xxxx ( vec4f a ) {
        return _mm_shuffle_ps(a,a,_MM_SHUFFLE(0,0,0,0));
    }
    __forceinline bool vec_equ_ps ( vec4f a, vec4f b, int mask ) {
        return (_mm_movemask_ps(_mm_cmpeq_ps(a,b)) & mask)==mask;
    }
    __forceinline bool vec_nequ_ps ( vec4f a, vec4f b, int mask ) {
        return (_mm_movemask_ps(_mm_cmpeq_ps(a,b)) & mask)!=mask;
    }
    __forceinline vec4f vec_mod_ps(vec4f  a, vec4f aDiv) {
        vec4f c = _mm_div_ps(a,aDiv);
        vec4i i = _mm_cvttps_epi32(c);
        vec4f cTrunc = _mm_cvtepi32_ps(i);
        vec4f base = _mm_mul_ps(cTrunc, aDiv);
        vec4f r = _mm_sub_ps(a, base);
        return r;
    }
// vec4i
    __forceinline vec4i vec_set_pi_xyzw ( int32_t x, int32_t y, int32_t z, int32_t w ) {
        return _mm_setr_epi32(x,y,z,w);
    }
    __forceinline vec4i vec_setzero_epi () {
        return _mm_setzero_si128();
    }
    __forceinline vec4i vec_neg_epi ( vec4i a ) {
        return _mm_sub_epi32(_mm_setzero_si128(), a);
    }
    __forceinline vec4i vec_add_epi ( vec4i a, vec4i b ) {
        return _mm_add_epi32(a,b);
    }
    __forceinline vec4i vec_sub_epi ( vec4i a, vec4i b ) {
        return _mm_sub_epi32(a,b);
    }
    __forceinline vec4i vec_shuffle_epi_xxxx ( vec4i a ) {
        return _mm_shuffle_epi32(a,_MM_SHUFFLE(0,0,0,0));
    }
    __forceinline bool vec_equ_epi ( vec4i a, vec4i b, int mask ) {
        return (_mm_movemask_ps(_mm_castsi128_ps(_mm_cmpeq_epi32(a,b))) & mask)==mask;
    }
    __forceinline bool vec_nequ_epi  ( vec4i a, vec4i b, int mask ) {
        return (_mm_movemask_ps(_mm_castsi128_ps(_mm_cmpeq_epi32(a,b))) & mask)!=mask;
    }
    __forceinline vec4i vec_mul_epi ( vec4i a, vec4i b ) {
        vec4i c;
        int32_t * A = (int32_t *)&a;
        int32_t * B = (int32_t *)&b;
        int32_t * C = (int32_t *)&c;
        C[0] = A[0]*B[0];   C[1] = A[1]*B[1];   C[2] = A[2]*B[2];   C[3] = A[3]*B[3];
        return c;
    }
    __forceinline vec4i vec_div_epi ( vec4i a, vec4i b ) {
        vec4i c;
        uint32_t * A = (uint32_t *)&a;
        uint32_t * B = (uint32_t *)&b;
        uint32_t * C = (uint32_t *)&c;
        C[0] = A[0]/B[0];   C[1] = A[1]/B[1];   C[2] = A[2]/B[2];   C[3] = A[3]/B[3];
        return c;
    }
    __forceinline vec4i vec_mod_epi ( vec4i a, vec4i b ) {
        vec4i c;
        int32_t * A = (int32_t *)&a;
        int32_t * B = (int32_t *)&b;
        int32_t * C = (int32_t *)&c;
        C[0] = A[0]%B[0];   C[1] = A[1]%B[1];   C[2] = A[2]%B[2];   C[3] = A[3]%B[3];
        return c;
    }
// vec4i unsigned
    __forceinline vec4i vec_loadu_esi ( const vec4i * t ) {
        return _mm_loadu_si128(t);
    }
    
    __forceinline vec4i vec_mul_epu ( vec4i a, vec4i b ) {
        return _mm_mul_epu32(a,b);
    }
    __forceinline vec4i vec_div_epu ( vec4i a, vec4i b) {
        vec4i c;
        uint32_t * A = (uint32_t *)&a;
        uint32_t * B = (uint32_t *)&b;
        uint32_t * C = (uint32_t *)&c;
        C[0] = A[0]/B[0];   C[1] = A[1]/B[1];   C[2] = A[2]/B[2];   C[3] = A[3]/B[3];
        return c;
    }
    __forceinline vec4i vec_mod_epu ( vec4i a, vec4i b ) {
        vec4i c;
        uint32_t * A = (uint32_t *)&a;
        uint32_t * B = (uint32_t *)&b;
        uint32_t * C = (uint32_t *)&c;
        C[0] = A[0]%B[0];   C[1] = A[1]%B[1];   C[2] = A[2]%B[2];   C[3] = A[3]%B[3];
        return c;
    }
    
#elif _TARGET_SIMD_NEON // PSP2, iOS
#include <arm_neon.h>
    typedef float32x4_t vec4f;
    typedef int32x4_t   vec4i;
#else
    #error "unsupported target"
#endif
}
