/*
 * Dagor Engine 5
 * Copyright (C) 2003-2021  Gaijin Entertainment.  All rights reserved
 *
 * (for conditions of distribution and use, see License)
*/

#ifndef _DAGOR_PUBLIC_MATH_DAG_VECMATH_PC_SSE_H_
#define _DAGOR_PUBLIC_MATH_DAG_VECMATH_PC_SSE_H_
#pragma once

#if !defined(_TARGET_PC_LINUX) && !defined(_TARGET_PC_MACOSX) && !defined(_TARGET_PC_WIN)\
 && !defined(_TARGET_PS4) && !defined(_TARGET_PS5)  && !defined(_TARGET_XBOX) && !defined(_TARGET_PC) && !defined(_TARGET_ANDROID)
  #if __linux__ || __unix__
    #define _TARGET_PC_LINUX 1
  #elif __APPLE__
    #define _TARGET_PC_MACOSX 1
  #elif _WIN32
    #define _TARGET_PC_WIN 1
    #if _WIN64
      #define TARGET_64BIT 1
    #endif
  #endif
#endif

#include <math.h> //for fabsf, which is used once, and not wise
#if _TARGET_PC_LINUX
#include <x86intrin.h> // MAC doesn't have it in GCC frontend, but it exist in CLANG one
#elif _TARGET_SIMD_SSE >= 4 || defined(_DAGOR_PROJECT_OPTIONAL_SSE4)
#include <smmintrin.h>
#else
#include <emmintrin.h>
#include <pmmintrin.h>
#include <tmmintrin.h>
#endif

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable: 4714) //function marked as __forceinline not inlined

  #if _MSC_VER < 1500
  VECMATH_FINLINE __m128i VECTORCALL _mm_castps_si128(__m128  v) { return *(__m128i*)&v; }
  VECMATH_FINLINE __m128 VECTORCALL  _mm_castsi128_ps(__m128i v) { return *(__m128 *)&v; }
  #endif
#endif

VECMATH_FINLINE vec4f VECTORCALL v_zero() { return _mm_setzero_ps(); }
VECMATH_FINLINE vec4i VECTORCALL v_zeroi() { return _mm_setzero_si128(); }
VECMATH_FINLINE vec4f VECTORCALL v_msbit() { return (vec4f&)V_CI_SIGN_MASK; }
VECMATH_FINLINE vec4f VECTORCALL v_splat4(const float *a) { return _mm_load1_ps(a); }
VECMATH_FINLINE vec4f VECTORCALL v_splats(float a) {return _mm_set1_ps(a);}//_mm_set_ps1(a) is slower...
VECMATH_FINLINE vec4i VECTORCALL v_splatsi(int a) {return _mm_set1_epi32(a);}
VECMATH_FINLINE vec4f VECTORCALL v_set_x(float a) {return _mm_load_ss(&a);} // set x, zero others
VECMATH_FINLINE vec4i VECTORCALL v_seti_x(int a) {return _mm_cvtsi32_si128(a);} // set x, zero others


#if defined(DAGOR_ASAN_ENABLED) && defined(__GNUC__) && __GNUC__ >= 7
NO_ASAN_INLINE vec4f v_ld(const float *m) { return  *(__m128 *)m; }
NO_ASAN_INLINE vec4f v_ldu(const float *m) { return *(__m128_u *)m; }
NO_ASAN_INLINE vec4i v_ldi(const int *m) { return  *(__m128i *)m; }
NO_ASAN_INLINE vec4i v_ldui(const int *m) { return *(__m128i_u *)m; }
NO_ASAN_INLINE vec4f v_ldu_x(const float *m) { union { float x; vec4f vec; } mm{}; mm.x = *m; return mm.vec; } // load x, zero others
#else
NO_ASAN_INLINE vec4f v_ld(const float *m) { return _mm_load_ps(m); }
NO_ASAN_INLINE vec4f v_ldu(const float *m) { return _mm_loadu_ps(m); }
NO_ASAN_INLINE vec4i v_ldi(const int *m) { return  _mm_load_si128((const vec4i*)m); }
NO_ASAN_INLINE vec4i v_ldui(const int *m) { return _mm_loadu_si128((const vec4i*)m); }
NO_ASAN_INLINE vec4f v_ldu_x(const float *m) { return _mm_load_ss(m); } // load x, zero others
#endif

VECMATH_FINLINE vec4i VECTORCALL v_ldush(const signed short *m)
{
  vec4i h = _mm_loadl_epi64((__m128i const*)m);
  vec4i sx = _mm_cmplt_epi16(h, _mm_setzero_si128());
  return _mm_unpacklo_epi16(h, sx);
}
VECMATH_FINLINE vec4i VECTORCALL v_lduush(const unsigned short *m)
{
  vec4i h = _mm_loadl_epi64((__m128i const*)m);
  return _mm_unpacklo_epi16(h, _mm_setzero_si128());
}

VECMATH_FINLINE vec4i VECTORCALL v_ldui_half(const void *m) { return _mm_loadl_epi64((__m128i const*)m); }
VECMATH_FINLINE vec4f VECTORCALL v_ldu_half(const void *m) { return v_cast_vec4f(v_ldui_half(m)); }
VECMATH_FINLINE void v_prefetch(const void *m) { _mm_prefetch((const char *)m, _MM_HINT_T0); }

VECMATH_FINLINE vec4i VECTORCALL v_cvt_ush_vec4i(vec4i a) { return _mm_unpacklo_epi16(a, _mm_setzero_si128()); }
VECMATH_FINLINE vec4i VECTORCALL
  v_cvt_ssh_vec4i(vec4i a) { vec4i sx = _mm_cmplt_epi16(a, _mm_setzero_si128()); return _mm_unpacklo_epi16(a, sx); }

VECMATH_FINLINE vec4i VECTORCALL v_cvt_byte_vec4i(vec4i a) { return _mm_unpacklo_epi8(a, _mm_setzero_si128()); }

VECMATH_FINLINE vec4f VECTORCALL v_make_vec4f(float x, float y, float z, float w)
{ return _mm_setr_ps(x, y, z, w); }

VECMATH_FINLINE vec4i VECTORCALL v_make_vec4i(int x, int y, int z, int w)
{ return _mm_setr_epi32(x, y, z, w); }

//VECMATH_FINLINE vec4f VECTORCALL v_perm_mask(){ _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(a), mask)); }
#define V_SHUFFLE(v, mask) _mm_shuffle_ps(v, v, mask)
#define V_SHUFFLE_REV(v, maskW, maskZ, maskY, maskX) V_SHUFFLE(v, _MM_SHUFFLE(maskW, maskZ, maskY, maskX))
#define V_SHUFFLE_FWD(v, maskX, maskY, maskZ, maskW) V_SHUFFLE(v, _MM_SHUFFLE(maskW, maskZ, maskY, maskX))

#if _TARGET_SIMD_SSE >=3
VECMATH_FINLINE vec4f VECTORCALL v_perm_xxzz(vec4f b){ return _mm_moveldup_ps(b); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xyxy(vec4f b){ return _mm_castpd_ps(_mm_movedup_pd(_mm_castps_pd(b))); }
#else
VECMATH_FINLINE vec4f VECTORCALL v_perm_xxzz(vec4f b){ return V_SHUFFLE_FWD(b, 0,0, 2,2); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xyxy(vec4f b){ return V_SHUFFLE_FWD(b, 0,1, 0,1); }
#endif
VECMATH_FINLINE vec4f VECTORCALL v_perm_yyww(vec4f b){ return V_SHUFFLE_FWD(b, 1,1, 3,3); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xzxz(vec4f b){ return V_SHUFFLE_FWD(b, 0,2,0,2); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_zwzw(vec4f b){ return V_SHUFFLE_FWD(b, 2,3, 2,3); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_ywyw(vec4f b){ return V_SHUFFLE_FWD(b, 1,3,1,3); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xyzz(vec4f b){ return V_SHUFFLE_FWD(b, 0,1,2,2); }


VECMATH_FINLINE vec4f VECTORCALL v_splat_x(vec4f a)
  { return _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 0));  }
VECMATH_FINLINE vec4f VECTORCALL v_splat_y(vec4f a)
  { return _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1));  }
VECMATH_FINLINE vec4f VECTORCALL v_splat_z(vec4f a)
  { return _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2));  }
VECMATH_FINLINE vec4f VECTORCALL v_splat_w(vec4f a)
  { return _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 3, 3, 3));  }

VECMATH_FINLINE vec4i VECTORCALL v_splat_xi(vec4i a) { return _mm_shuffle_epi32(a, _MM_SHUFFLE(0, 0, 0, 0));  }
VECMATH_FINLINE vec4i VECTORCALL v_splat_yi(vec4i a) { return _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 1, 1, 1));  }
VECMATH_FINLINE vec4i VECTORCALL v_splat_zi(vec4i a) { return _mm_shuffle_epi32(a, _MM_SHUFFLE(2, 2, 2, 2));  }
VECMATH_FINLINE vec4i VECTORCALL v_splat_wi(vec4i a) { return _mm_shuffle_epi32(a, _MM_SHUFFLE(3, 3, 3, 3));  }

VECMATH_FINLINE void VECTORCALL v_st(void *m, vec4f v) { _mm_store_ps((float*)m, v); }
VECMATH_FINLINE void VECTORCALL v_stu(void *m, vec4f v) { _mm_storeu_ps((float*)m, v); }
VECMATH_FINLINE void VECTORCALL v_sti(void *m, vec4i v) { _mm_store_si128((__m128i*)m, v); }
VECMATH_FINLINE void VECTORCALL v_stui(void *m, vec4i v) { _mm_storeu_si128((__m128i*)m, v); }
VECMATH_FINLINE void VECTORCALL v_stui_half(void *m, vec4i v) { _mm_storel_epi64((__m128i*)m, v); }
VECMATH_FINLINE void VECTORCALL v_stu_half(void *m, vec4f v) { _mm_storel_epi64((__m128i*)m, _mm_castps_si128(v)); }
VECMATH_FINLINE void VECTORCALL v_stu_p3(float *p3, vec3f v) { _mm_storel_pi((__m64*)p3, v); p3[2] = v_extract_z(v); } //-V1032

VECMATH_FINLINE vec4f VECTORCALL v_merge_hw(vec4f a, vec4f b) { return _mm_unpacklo_ps(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_merge_lw(vec4f a, vec4f b) { return _mm_unpackhi_ps(a, b); }

VECMATH_FINLINE int VECTORCALL v_signmask(vec4f a) { return _mm_movemask_ps(a); }
VECMATH_FINLINE vec4f VECTORCALL v_cmp_eq(vec4f a, vec4f b) { return _mm_cmpeq_ps(a, b); }
VECMATH_FINLINE vec4i VECTORCALL v_cmp_eqi(vec4i a, vec4i b) { return _mm_cmpeq_epi32(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_cmp_eqi(vec4f a, vec4f b)
{
  __m128i m = _mm_cmpeq_epi32(v_cast_vec4i(a), v_cast_vec4i(b));
  return v_cast_vec4f(m);
}
VECMATH_FINLINE vec4f VECTORCALL v_cmp_ge(vec4f a, vec4f b) { return _mm_cmpge_ps(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_cmp_gt(vec4f a, vec4f b) { return _mm_cmpgt_ps(a, b); }
VECMATH_FINLINE vec4i VECTORCALL v_cmp_lti(vec4i a, vec4i b) { return _mm_cmplt_epi32(a, b); }
VECMATH_FINLINE vec4i VECTORCALL v_cmp_gti(vec4i a, vec4i b) { return _mm_cmpgt_epi32(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_and(vec4f a, vec4f b) { return _mm_and_ps(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_andnot(vec4f a, vec4f b) { return _mm_andnot_ps(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_or(vec4f a, vec4f b) { return _mm_or_ps(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_xor(vec4f a, vec4f b) { return _mm_xor_ps(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_sel(vec4f a, vec4f b, vec4f c)
{
  return _mm_or_ps(_mm_and_ps(c, b), _mm_andnot_ps(c, a));
}

VECMATH_FINLINE vec4i VECTORCALL v_seli(vec4i a, vec4i b, vec4i c)
{
  return _mm_or_si128(_mm_and_si128(c, b), _mm_andnot_si128(c, a));
}

VECMATH_FINLINE vec4i VECTORCALL v_cast_vec4i(vec4f a) {return _mm_castps_si128(a);}//no instruction
VECMATH_FINLINE vec4f VECTORCALL v_cast_vec4f(vec4i a) {return _mm_castsi128_ps(a);}//no instruction

VECMATH_FINLINE vec4i VECTORCALL v_cvt_vec4i(vec4f a) { return _mm_cvttps_epi32(a); }
VECMATH_FINLINE vec4f VECTORCALL v_cvt_vec4f(vec4i a) { return _mm_cvtepi32_ps(a); }
VECMATH_FINLINE vec4i VECTORCALL v_cvt_roundi(vec4f a)  { return _mm_cvtps_epi32(a); }

VECMATH_FINLINE vec4i VECTORCALL sse2_cvt_floori(vec4f a)
{
  vec4i a_trunc = v_cvt_vec4i(a);
  vec4f a_truncf = v_cvt_vec4f(a_trunc);
  return v_subi(a_trunc, _mm_andnot_si128(v_cast_vec4i(v_cmp_eq(a_truncf, a)), _mm_srli_epi32(v_cast_vec4i(a), 31)));
}

VECMATH_FINLINE vec4i VECTORCALL sse2_cvt_ceili(vec4f a)
{
  vec4f a_add_half = v_add(a, v_andnot(v_cmp_eq(v_cvt_vec4f(_mm_cvtps_epi32(a)), a), V_C_HALF));
  return _mm_cvtps_epi32(a_add_half);
}
VECMATH_FINLINE vec4f VECTORCALL sse2_floor(vec4f a) { return _mm_cvtepi32_ps(v_cvt_floori(a)); }
VECMATH_FINLINE vec4f VECTORCALL sse2_ceil(vec4f a) { return _mm_cvtepi32_ps(v_cvt_ceili(a)); }
VECMATH_FINLINE vec4f VECTORCALL sse2_round(vec4f a) { return _mm_cvtepi32_ps(_mm_cvtps_epi32(a)); }

#if _TARGET_SIMD_SSE >= 4 || defined(_DAGOR_PROJECT_OPTIONAL_SSE4) || defined(__SSE4_1__)
VECMATH_FINLINE vec4f VECTORCALL sse4_floor(vec4f a) { return _mm_round_ps(a, _MM_FROUND_TO_NEG_INF|_MM_FROUND_NO_EXC); }
VECMATH_FINLINE vec4f VECTORCALL sse4_ceil(vec4f a) { return _mm_round_ps(a, _MM_FROUND_TO_POS_INF|_MM_FROUND_NO_EXC); }
VECMATH_FINLINE vec4f VECTORCALL sse4_round(vec4f a) { return _mm_round_ps(a, _MM_FROUND_RINT); }
VECMATH_FINLINE vec4i VECTORCALL sse4_cvt_floori(vec4f a) { return _mm_cvttps_epi32(sse4_floor(a)); }
VECMATH_FINLINE vec4i VECTORCALL sse4_cvt_ceili(vec4f a)  { return _mm_cvttps_epi32(sse4_ceil(a)); }
#else // fallback to SSE2
VECMATH_FINLINE vec4f VECTORCALL sse4_floor(vec4f a) { return sse2_floor(a); }
VECMATH_FINLINE vec4f VECTORCALL sse4_ceil(vec4f a) { return sse2_ceil(a); }
VECMATH_FINLINE vec4f VECTORCALL sse4_round(vec4f a) { return sse2_round(a); }
VECMATH_FINLINE vec4i VECTORCALL sse4_cvt_floori(vec4f a) { return sse2_cvt_floori(a); }
VECMATH_FINLINE vec4i VECTORCALL sse4_cvt_ceili(vec4f a)  { return sse2_cvt_ceili(a); }
#endif
#if _TARGET_SIMD_SSE >= 4
VECMATH_FINLINE vec4i VECTORCALL v_cvt_floori(vec4f a) {return sse4_cvt_floori(a);}
VECMATH_FINLINE vec4i VECTORCALL v_cvt_ceili(vec4f a) {return sse4_cvt_ceili(a);}
VECMATH_FINLINE vec4f VECTORCALL v_floor(vec4f a) { return sse4_floor(a); }
VECMATH_FINLINE vec4f VECTORCALL v_ceil(vec4f a) { return sse4_ceil(a); }
VECMATH_FINLINE vec4f VECTORCALL v_round(vec4f a) { return sse4_round(a); }
#else
VECMATH_FINLINE vec4i VECTORCALL v_cvt_floori(vec4f a) {return sse2_cvt_floori(a);}
VECMATH_FINLINE vec4i VECTORCALL v_cvt_ceili(vec4f a) {return sse2_cvt_ceili(a);}
VECMATH_FINLINE vec4f VECTORCALL v_floor(vec4f a) { return sse2_floor(a); }
VECMATH_FINLINE vec4f VECTORCALL v_ceil(vec4f a) { return sse2_ceil(a); }
VECMATH_FINLINE vec4f VECTORCALL v_round(vec4f a) { return sse2_round(a); }
#endif


VECMATH_FINLINE vec4f VECTORCALL v_add(vec4f a, vec4f b) { return _mm_add_ps(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_sub(vec4f a, vec4f b) { return _mm_sub_ps(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_mul(vec4f a, vec4f b) { return _mm_mul_ps(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_div(vec4f a, vec4f b) { return _mm_div_ps(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_madd(vec4f a, vec4f b, vec4f c) { return _mm_add_ps(_mm_mul_ps(a, b), c); }
VECMATH_FINLINE vec4f VECTORCALL v_nmsub(vec4f a, vec4f b, vec4f c) { return _mm_sub_ps(c, _mm_mul_ps(a, b)); }
VECMATH_FINLINE vec4f VECTORCALL v_add_x(vec4f a, vec4f b) { return _mm_add_ss(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_sub_x(vec4f a, vec4f b) { return _mm_sub_ss(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_mul_x(vec4f a, vec4f b) { return _mm_mul_ss(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_div_x(vec4f a, vec4f b) { return _mm_div_ss(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_madd_x(vec4f a, vec4f b, vec4f c) { return _mm_add_ss(_mm_mul_ss(a, b), c); }
VECMATH_FINLINE vec4f VECTORCALL v_nmsub_x(vec4f a, vec4f b, vec4f c) { return _mm_sub_ss(c, _mm_mul_ss(a, b)); }
VECMATH_FINLINE vec4i VECTORCALL v_addi(vec4i a, vec4i b) { return _mm_add_epi32(a, b); }
VECMATH_FINLINE vec4i VECTORCALL v_subi(vec4i a, vec4i b) { return _mm_sub_epi32(a, b); }

VECMATH_FINLINE vec4i VECTORCALL v_slli(vec4i v, int bits) {return _mm_slli_epi32(v, bits);}
VECMATH_FINLINE vec4i VECTORCALL v_srli(vec4i v, int bits) {return _mm_srli_epi32(v, bits);}
VECMATH_FINLINE vec4i VECTORCALL v_srai(vec4i v, int bits) {return _mm_srai_epi32(v, bits);}
VECMATH_FINLINE vec4i VECTORCALL v_slli_n(vec4i v, int bits) { return _mm_sll_epi32(v, _mm_cvtsi32_si128(bits)); }
VECMATH_FINLINE vec4i VECTORCALL v_srli_n(vec4i v, int bits) { return _mm_srl_epi32(v, _mm_cvtsi32_si128(bits)); }
VECMATH_FINLINE vec4i VECTORCALL v_srai_n(vec4i v, int bits) { return _mm_sra_epi32(v, _mm_cvtsi32_si128(bits)); }
VECMATH_FINLINE vec4i VECTORCALL v_slli_n(vec4i v, vec4i bits) { return _mm_sll_epi32(v, bits); }
VECMATH_FINLINE vec4i VECTORCALL v_srli_n(vec4i v, vec4i bits) { return _mm_srl_epi32(v, bits); }
VECMATH_FINLINE vec4i VECTORCALL v_srai_n(vec4i v, vec4i bits) { return _mm_sra_epi32(v, bits); }

VECMATH_FINLINE vec4i VECTORCALL v_sll(vec4i v, int bits) {return _mm_slli_epi32(v, bits);}
VECMATH_FINLINE vec4i VECTORCALL v_srl(vec4i v, int bits) {return _mm_srli_epi32(v, bits);}
VECMATH_FINLINE vec4i VECTORCALL v_sra(vec4i v, int bits) {return _mm_srai_epi32(v, bits);}

VECMATH_FINLINE vec4i VECTORCALL v_ori(vec4i a, vec4i b) {return _mm_or_si128(a, b);}
VECMATH_FINLINE vec4i VECTORCALL v_andi(vec4i a, vec4i b) {return _mm_and_si128(a, b);}
VECMATH_FINLINE vec4i VECTORCALL v_andnoti(vec4i a, vec4i b) {return _mm_andnot_si128(a, b);}
VECMATH_FINLINE vec4i VECTORCALL v_xori(vec4i a, vec4i b){return _mm_xor_si128(a, b);}

VECMATH_FINLINE vec4i VECTORCALL v_packs(vec4i a, vec4i b){return _mm_packs_epi32(a, b);}
VECMATH_FINLINE vec4i VECTORCALL v_packs(vec4i a){return _mm_packs_epi32(a, a);}
VECMATH_FINLINE vec4i VECTORCALL sse2_packus(vec4i a, vec4i b)
{
  vec4i z = _mm_setzero_si128();
  a = v_andi(v_ori(v_srai(v_slli(a, 16), 16), _mm_cmpgt_epi32(v_srli(a, 16), z)), _mm_cmpgt_epi32(a, z));
  b = v_andi(v_ori(v_srai(v_slli(b, 16), 16), _mm_cmpgt_epi32(v_srli(b, 16), z)), _mm_cmpgt_epi32(b, z));
  return _mm_packs_epi32(a, b);
}
VECMATH_FINLINE vec4i VECTORCALL sse2_packus(vec4i a)
{
  vec4i z = _mm_setzero_si128();
  a = v_andi(v_ori(v_srai(v_slli(a, 16), 16), _mm_cmpgt_epi32(v_srli(a, 16), z)), _mm_cmpgt_epi32(a, z));
  return _mm_packs_epi32(a, a);
}

//unsigned mul
VECMATH_FINLINE vec4i VECTORCALL sse2_muli(vec4i a, vec4i b)
{
  __m128i tmp1 = _mm_mul_epu32(a,b); /* mul 2,0*/
  __m128i tmp2 = _mm_mul_epu32( _mm_srli_si128(a,4), _mm_srli_si128(b,4)); /* mul 3,1 */
  return _mm_unpacklo_epi32(_mm_shuffle_epi32(tmp1, _MM_SHUFFLE (0,0,2,0)), _mm_shuffle_epi32(tmp2, _MM_SHUFFLE (0,0,2,0))); /* shuffle results to [63..0] and pack */
}

#if _TARGET_SIMD_SSE >= 4 || defined(_DAGOR_PROJECT_OPTIONAL_SSE4) || defined(__SSE4_1__)
VECMATH_FINLINE vec4i VECTORCALL sse4_packus(vec4i a, vec4i b) { return _mm_packus_epi32(a, b); }
VECMATH_FINLINE vec4i VECTORCALL sse4_packus(vec4i a) { return _mm_packus_epi32(a, a); }
VECMATH_FINLINE vec4i VECTORCALL sse4_muli(vec4i a, vec4i b) { return _mm_mullo_epi32(a, b);}
#else // fallback to SSE2
VECMATH_FINLINE vec4i VECTORCALL sse4_packus(vec4i a, vec4i b) { return sse2_packus(a, b); }
VECMATH_FINLINE vec4i VECTORCALL sse4_packus(vec4i a) { return sse2_packus(a); }
VECMATH_FINLINE vec4i VECTORCALL sse4_muli(vec4i a, vec4i b) { return sse2_muli(a, b);}
#endif

#if _TARGET_SIMD_SSE >= 4
VECMATH_FINLINE vec4i VECTORCALL v_packus(vec4i a, vec4i b) { return sse4_packus(a, b); }
VECMATH_FINLINE vec4i VECTORCALL v_packus(vec4i a) { return sse4_packus(a); }
VECMATH_FINLINE vec4i VECTORCALL v_muli(vec4i a, vec4i b) { return sse4_muli(a,b); }
#else
VECMATH_FINLINE vec4i VECTORCALL v_packus(vec4i a, vec4i b) { return sse2_packus(a, b); }
VECMATH_FINLINE vec4i VECTORCALL v_packus(vec4i a) { return sse2_packus(a); }
VECMATH_FINLINE vec4i VECTORCALL v_muli(vec4i a, vec4i b) { return sse2_muli(a,b); }
#endif

VECMATH_FINLINE vec4i VECTORCALL v_packus16(vec4i a, vec4i b) { return _mm_packus_epi16(a,b); }
VECMATH_FINLINE vec4i VECTORCALL v_packus16(vec4i a) { return _mm_packus_epi16(a,a); }

VECMATH_FINLINE vec4f VECTORCALL v_rcp_est(vec4f a) { return _mm_rcp_ps(a); }
VECMATH_FINLINE vec4f VECTORCALL v_rcp(vec4f a)
{
  __m128 y0 = _mm_rcp_ps(a);
  return _mm_sub_ps(_mm_add_ps(y0, y0), _mm_mul_ps(a, _mm_mul_ps(y0, y0)));
}
VECMATH_FINLINE vec4f VECTORCALL v_rcp_est_x(vec4f a) { return _mm_rcp_ss(a); }
VECMATH_FINLINE vec4f VECTORCALL v_rcp_x(vec4f a)
{
  __m128 y0 = _mm_rcp_ss(a);
  return _mm_sub_ss(_mm_add_ss(y0, y0), _mm_mul_ss(a, _mm_mul_ss(y0, y0)));
}

VECMATH_FINLINE vec4f VECTORCALL v_rsqrt4_fast(vec4f a) { return _mm_rsqrt_ps(a); }
VECMATH_FINLINE vec4f VECTORCALL v_rsqrt4(vec4f a) { return v_rcp(_mm_sqrt_ps(a)); }

VECMATH_FINLINE vec4f VECTORCALL v_rsqrt_fast_x(vec4f a) { return _mm_rsqrt_ss(a); }
VECMATH_FINLINE vec4f VECTORCALL v_rsqrt_x(vec4f a) // Reciprocal square root estimate and 1 Newton-Raphson iteration.
{
  vec4f y0, y0x, y0half;
  y0 = v_rsqrt_fast_x(a);
  y0x = v_mul_x(y0, a);
  y0half = v_mul_x(y0, V_C_HALF);
  return v_madd_x(v_nmsub_x(y0, y0x, V_C_ONE), y0half, y0);
}
VECMATH_FINLINE vec4i VECTORCALL sse2_mini(vec4i a, vec4i b)
{
  vec4i cond = v_cmp_gti(a, b);
  return v_ori(v_andnoti(cond, a), v_andi(cond, b));
}
VECMATH_FINLINE vec4i VECTORCALL sse2_maxi(vec4i a, vec4i b)
{
  vec4i cond = v_cmp_gti(b, a);
  return v_ori(v_andnoti(cond, a), v_andi(cond, b));
}

VECMATH_FINLINE vec4i VECTORCALL sse2_absi (vec4i a)
{
  vec4i mask = v_cmp_lti( a, _mm_setzero_si128() ); // FFFF   where a < 0
  a    = v_xori ( a, mask );                         // Invert where a < 0
  mask = v_srli( mask, 31 );                        // 0001   where a < 0
  a = v_addi( a, mask );                             // Add 1  where a < 0
  return a;
}

#if _TARGET_SIMD_SSE >= 3 || defined(_DAGOR_PROJECT_OPTIONAL_SSE4) || defined(__SSE3__)
VECMATH_FINLINE vec4i VECTORCALL sse3_absi(vec4i a) {return _mm_abs_epi32(a);}
#else
VECMATH_FINLINE vec4i VECTORCALL sse3_absi(vec4i a) {return sse2_absi(a);}
#endif

#if _TARGET_SIMD_SSE >= 4 || defined(_DAGOR_PROJECT_OPTIONAL_SSE4) || defined(__SSE4_1__)
VECMATH_FINLINE vec4i VECTORCALL sse4_mini(vec4i a, vec4i b) {return _mm_min_epi32(a, b);}
VECMATH_FINLINE vec4i VECTORCALL sse4_maxi(vec4i a, vec4i b) {return _mm_max_epi32(a, b);}
#else // fallback to SSE2
VECMATH_FINLINE vec4i VECTORCALL sse4_mini(vec4i a, vec4i b) {return sse2_mini(a, b);}
VECMATH_FINLINE vec4i VECTORCALL sse4_maxi(vec4i a, vec4i b) {return sse2_maxi(a, b);}
#endif

VECMATH_FINLINE vec4f VECTORCALL v_min(vec4f a, vec4f b) { return _mm_min_ps(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_max(vec4f a, vec4f b) { return _mm_max_ps(a, b); }
#if _TARGET_SIMD_SSE >= 4
VECMATH_FINLINE vec4i VECTORCALL v_mini(vec4i a, vec4i b) {return sse4_mini(a, b);}
VECMATH_FINLINE vec4i VECTORCALL v_maxi(vec4i a, vec4i b) {return sse4_maxi(a, b);}
VECMATH_FINLINE vec4i VECTORCALL v_absi(vec4i a) {return sse3_absi(a);}
#else
VECMATH_FINLINE vec4i VECTORCALL v_mini(vec4i a, vec4i b) {return sse2_mini(a, b);}
VECMATH_FINLINE vec4i VECTORCALL v_maxi(vec4i a, vec4i b) {return sse2_maxi(a, b);}
VECMATH_FINLINE vec4i VECTORCALL v_absi(vec4i a) {return sse2_absi(a);}
#endif

VECMATH_FINLINE vec4f VECTORCALL v_neg(vec4f a) { return v_sub(v_zero(), a); }
VECMATH_FINLINE vec4i VECTORCALL v_negi(vec4i a){ return v_subi(v_cast_vec4i(v_zero()), a); }
VECMATH_FINLINE vec4f VECTORCALL v_abs(vec4f a)
{
  #if defined(__clang__)
    return v_max(v_neg(a), a);
  #else
    //for this code clang creates one instruction, but uses memory for it.
    //if we think it is good tradeoff, we'd better allocate this constant once
    __m128i absmask = _mm_castps_si128(a);
    absmask = _mm_srli_epi32(_mm_cmpeq_epi32(absmask, absmask), 1);
    return _mm_and_ps(_mm_castsi128_ps(absmask), a);
  #endif
}

VECMATH_FINLINE vec4f VECTORCALL v_sqrt4_fast(vec4f a) { return _mm_sqrt_ps(a); }
VECMATH_FINLINE vec4f VECTORCALL v_sqrt4(vec4f a) { return _mm_sqrt_ps(a); }
VECMATH_FINLINE vec4f VECTORCALL v_sqrt_fast_x(vec4f a) { return _mm_sqrt_ss(a); }
VECMATH_FINLINE vec4f VECTORCALL v_sqrt_x(vec4f a) { return _mm_sqrt_ss(a); }

VECMATH_FINLINE vec4f VECTORCALL v_rot_1(vec4f a) { return V_SHUFFLE_REV(a, 0, 3, 2, 1); }
VECMATH_FINLINE vec4f VECTORCALL v_rot_2(vec4f a) { return V_SHUFFLE_REV(a, 1, 0, 3, 2); }
VECMATH_FINLINE vec4f VECTORCALL v_rot_3(vec4f a) { return V_SHUFFLE_REV(a, 2, 1, 0, 3); }

VECMATH_FINLINE vec4f VECTORCALL v_perm_yzxx(vec4f a) { return V_SHUFFLE_REV(a, 0,0,2,1); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_yzxy(vec4f a) { return V_SHUFFLE_REV(a, 1,0,2,1); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_yzxw(vec4f a) { return V_SHUFFLE_REV(a, 3,0,2,1); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_zxyw(vec4f a) { return V_SHUFFLE_REV(a, 3,1,0,2); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xxyy(vec4f a) { return V_SHUFFLE_REV(a, 1,1,0,0); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_zzww(vec4f a) { return V_SHUFFLE_REV(a, 3,3,2,2); }

VECMATH_FINLINE vec4f VECTORCALL v_perm_xzac(vec4f xyzw, vec4f abcd) { return _mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(2,0,2,0)); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_ywbd(vec4f xyzw, vec4f abcd) { return _mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(3,1,3,1)); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xyab(vec4f xyzw, vec4f abcd) { return _mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(1,0,1,0)); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_zwcd(vec4f xyzw, vec4f abcd) { return _mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(3,2,3,2)); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_bbyx(vec4f xyzw, vec4f abcd) { return _mm_shuffle_ps(abcd, xyzw, _MM_SHUFFLE(0,1,1,1)); }
VECMATH_FINLINE vec4f VECTORCALL
  v_perm_xaxa(vec4f xyzw, vec4f abcd) { return v_perm_yzxw(_mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(0,0,0,0))); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_yybb(vec4f xyzw, vec4f abcd) { return _mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(1,1,1,1)); }

VECMATH_FINLINE vec4f VECTORCALL v_perm_xycd(vec4f xyzw, vec4f abcd) { return _mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(3,2,1,0)); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_ayzw(vec4f xyzw, vec4f abcd) { return _mm_move_ss(xyzw, abcd); }

VECMATH_FINLINE vec4f VECTORCALL v_perm_xzbx(vec4f xyzw, vec4f abcd)
{
  vec4f bbxx =_mm_shuffle_ps(abcd, xyzw, _MM_SHUFFLE(0,0,1,1));
  return _mm_shuffle_ps(xyzw, bbxx, _MM_SHUFFLE(2, 0, 2, 0));
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xzya(vec4f xyzw, vec4f abcd)
{
  vec4f yyaa =_mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(0,0,1,1));
  return _mm_shuffle_ps(xyzw, yyaa, _MM_SHUFFLE(2, 0, 2, 0));
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_yxxc(vec4f xyzw, vec4f abcd)
{
  vec4f xxcc =_mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(2,2,0,0));
  return _mm_shuffle_ps(xyzw, xxcc, _MM_SHUFFLE(2, 0, 0, 1));
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_yaxx(vec4f xyzw, vec4f abcd)
{
  vec4f yyaa =_mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(0,0,1,1));
  return _mm_shuffle_ps(yyaa, xyzw, _MM_SHUFFLE(0, 0, 2, 0));
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_zxxb(vec4f xyzw, vec4f abcd)
{
  vec4f xxbb =_mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(1,1,0,0));
  return _mm_shuffle_ps(xyzw, xxbb, _MM_SHUFFLE(2, 0, 0, 2));
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_zayx(vec4f xyzw, vec4f abcd)
{
  vec4f zzaa =_mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(0,0,2,2));
  return _mm_shuffle_ps(zzaa, xyzw, _MM_SHUFFLE(0, 1, 2, 0));
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_bzxx(vec4f xyzw, vec4f abcd)
{
  vec4f bbzz =_mm_shuffle_ps(abcd, xyzw, _MM_SHUFFLE(2,2,1,1));
  return _mm_shuffle_ps(bbzz, xyzw, _MM_SHUFFLE(0, 0, 2, 0));
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_caxx(vec4f xyzw, vec4f abcd)
{
  return _mm_shuffle_ps(abcd, xyzw, _MM_SHUFFLE(0, 0, 0, 2));
}

#if _TARGET_SIMD_SSE >= 4
VECMATH_FINLINE vec4f VECTORCALL v_perm_xbzw(vec4f xyzw, vec4f abcd) { return _mm_blend_ps(xyzw, abcd, 2); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xycw(vec4f xyzw, vec4f abcd) { return _mm_blend_ps(xyzw, abcd, 4); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xyzd(vec4f xyzw, vec4f abcd) { return _mm_blend_ps(xyzw, abcd, 8); }

#else
VECMATH_FINLINE vec4f VECTORCALL v_perm_xbzw(vec4f xyzw, vec4f abcd)
{
  vec4f xxbb = _mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(1, 1, 0, 0));
  return _mm_shuffle_ps(xxbb, xyzw, _MM_SHUFFLE(3, 2, 3, 0));
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xycw(vec4f xyzw, vec4f abcd)
{
  vec4f wwcc = _mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(2, 2, 3, 3));
  return _mm_shuffle_ps(xyzw, wwcc, _MM_SHUFFLE(0, 3, 1, 0));
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xyzd(vec4f xyzw, vec4f abcd)
{
  vec4f zzdd = _mm_shuffle_ps(xyzw, abcd, _MM_SHUFFLE(3, 3, 2, 2));
  return _mm_shuffle_ps(xyzw, zzdd, _MM_SHUFFLE(3, 0, 1, 0));
}
#endif

VECMATH_FINLINE vec4f VECTORCALL sse2_dot3(vec4f a, vec4f b)
{
  vec4f m = _mm_mul_ps(a, b);

  return _mm_add_ps(
    _mm_add_ps(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2,2,1,0)),
               V_SHUFFLE_REV(m, 1,1,0,2)),
               _mm_shuffle_ps(m, m, _MM_SHUFFLE(0,0,2,1))
  );
}
VECMATH_FINLINE vec4f VECTORCALL sse2_dot4(vec4f a, vec4f b)
{
  vec4f m = _mm_mul_ps(a, b);

  return _mm_add_ps(
    _mm_add_ps(m,
               V_SHUFFLE_REV(m, 2,1,0,3)),
    _mm_add_ps(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1,0,3,2)),
               V_SHUFFLE_REV(m, 0,3,2,1))
  );
}
VECMATH_FINLINE vec4f VECTORCALL sse2_dot3_x(vec4f a, vec4f b)
{
  vec4f m = _mm_mul_ps(a, b);

  return _mm_add_ss(_mm_add_ss(m, v_splat_y(m)), _mm_shuffle_ps(m, m, 2));
}
VECMATH_FINLINE vec4f VECTORCALL sse2_dot4_x(vec4f a, vec4f b)
{
  vec4f m = _mm_mul_ps(a, b);

  return _mm_add_ss(_mm_add_ss(m, v_splat_y(m)),
                    _mm_add_ss(_mm_shuffle_ps(m, m, 2), v_splat_w(m)));
}

VECMATH_FINLINE vec4f VECTORCALL sse2_distance3p_x(plane3f a, vec3f b) { return v_add_x(sse2_dot3_x(a,b), v_splat_w(a)); }

#if _TARGET_SIMD_SSE >= 4 || defined(_DAGOR_PROJECT_OPTIONAL_SSE4) || defined(__SSE4_1__)
VECMATH_FINLINE vec4f VECTORCALL sse4_dot4(vec4f a, vec4f b) { return _mm_dp_ps(a,b, 0xFF); }
VECMATH_FINLINE vec4f VECTORCALL sse4_dot4_x(vec4f a, vec4f b) { return _mm_dp_ps(a,b,0xF1); }
VECMATH_FINLINE vec4f VECTORCALL sse4_dot3(vec4f a, vec4f b) { return _mm_dp_ps(a,b, 0x7F); }
VECMATH_FINLINE vec4f VECTORCALL sse4_dot3_x(vec4f a, vec4f b) { return _mm_dp_ps(a,b,0x71); }
VECMATH_FINLINE vec4f VECTORCALL sse4_distance3p_x(plane3f a, vec3f b) { return v_add_x(sse4_dot3_x(a,b), v_splat_w(a)); }
#else // fallback to SSE2
VECMATH_FINLINE vec4f VECTORCALL sse4_dot4(vec4f a, vec4f b) { return sse2_dot4(a, b); }
VECMATH_FINLINE vec4f VECTORCALL sse4_dot4_x(vec4f a, vec4f b) { return sse2_dot4_x(a, b); }
VECMATH_FINLINE vec4f VECTORCALL sse4_dot3(vec4f a, vec4f b) { return sse2_dot3(a, b); }
VECMATH_FINLINE vec4f VECTORCALL sse4_dot3_x(vec4f a, vec4f b) { return sse2_dot3_x(a, b); }
VECMATH_FINLINE vec4f VECTORCALL sse4_distance3p_x(plane3f a, vec3f b) { return sse2_distance3p_x(a,b); }
#endif

#if _TARGET_SIMD_SSE >= 3 || defined(_DAGOR_PROJECT_OPTIONAL_SSE4) || defined(__SSE3__)
VECMATH_FINLINE vec4f VECTORCALL sse3_dot4(vec4f a, vec4f b)
{
  vec4f m = _mm_mul_ps(a, b);
  m = _mm_hadd_ps(m,m);
  return _mm_hadd_ps(m,m);
}
VECMATH_FINLINE vec4f VECTORCALL sse3_distance3p_x(plane3f a, vec3f b) { return v_add_x(sse2_dot3_x(a,b), v_splat_w(a)); }
#else
VECMATH_FINLINE vec4f VECTORCALL sse3_dot4(vec4f a, vec4f b) { return sse2_dot4(a, b); }
VECMATH_FINLINE vec4f VECTORCALL sse3_distance3p_x(plane3f a, vec3f b) { return sse2_distance3p_x(a,b); }
#endif


#if _TARGET_SIMD_SSE >= 4
VECMATH_FINLINE vec4f VECTORCALL v_dot4(vec4f a, vec4f b) { return sse4_dot4(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_dot4_x(vec4f a, vec4f b) { return sse4_dot4_x(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_dot3(vec4f a, vec4f b) { return sse4_dot3(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_dot3_x(vec4f a, vec4f b) { return sse4_dot3_x(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_distance3p_x(plane3f a, vec3f b) { return sse4_distance3p_x(a,b); }
#elif _TARGET_SIMD_SSE >= 3
VECMATH_FINLINE vec4f VECTORCALL v_dot4(vec4f a, vec4f b) { return sse3_dot4(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_dot4_x(vec4f a, vec4f b) { return sse3_dot4(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_dot3(vec4f a, vec4f b) { return sse2_dot3(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_dot3_x(vec4f a, vec4f b) { return sse2_dot3_x(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_distance3p_x(plane3f a, vec3f b) { return sse3_distance3p_x(a,b); }
#else
VECMATH_FINLINE vec4f VECTORCALL v_dot4(vec4f a, vec4f b) { return sse2_dot4(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_dot4_x(vec4f a, vec4f b) { return sse2_dot4_x(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_dot3(vec4f a, vec4f b) { return sse2_dot3(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_dot3_x(vec4f a, vec4f b) { return sse2_dot3_x(a,b); }
VECMATH_FINLINE vec4f VECTORCALL v_distance3p_x(plane3f a, vec3f b) { return sse2_distance3p_x(a,b); }
#endif

VECMATH_FINLINE vec4f VECTORCALL v_norm4(vec4f a) { return v_div(a, v_splat_x(v_sqrt_x(v_dot4_x(a,a)))); }
VECMATH_FINLINE vec4f VECTORCALL v_norm3(vec4f a) { return v_div(a, v_splat_x(v_sqrt_x(v_dot3_x(a,a)))); }

VECMATH_FINLINE vec4f VECTORCALL v_distance3p(plane3f a, vec3f b)
{
  return v_splat_x(v_distance3p_x(a, b));
}

VECMATH_FINLINE vec3f VECTORCALL v_cross3(vec3f a, vec3f b)
{
  // a.y * b.z - a.z * b.y
  // a.z * b.x - a.x * b.z
  // a.x * b.y - a.y * b.x
  return _mm_sub_ps(
    _mm_mul_ps(V_SHUFFLE_REV(a, 0,0,2,1),
               _mm_shuffle_ps(b, b, _MM_SHUFFLE(0,1,0,2))),
    _mm_mul_ps(V_SHUFFLE_REV(a, 0,1,0,2),
               _mm_shuffle_ps(b, b, _MM_SHUFFLE(0,0,2,1)))
  );
}


VECMATH_FINLINE void VECTORCALL v_mat44_ident(mat44f &dest)
{
  dest.col0 = V_C_UNIT_1000;
  dest.col1 = V_C_UNIT_0100;
  dest.col2 = V_C_UNIT_0010;
  dest.col3 = V_C_UNIT_0001;
}
VECMATH_FINLINE void VECTORCALL v_mat44_ident_swapxz(mat44f &dest)
{
  dest.col0 = V_C_UNIT_0010;
  dest.col1 = V_C_UNIT_0100;
  dest.col2 = V_C_UNIT_1000;
  dest.col3 = V_C_UNIT_0001;
}
VECMATH_FINLINE void VECTORCALL v_mat33_ident(mat33f &dest)
{
  dest.col0 = V_C_UNIT_1000;
  dest.col1 = V_C_UNIT_0100;
  dest.col2 = V_C_UNIT_0010;
}
VECMATH_FINLINE void VECTORCALL v_mat33_ident_swapxz(mat33f &dest)
{
  dest.col0 = V_C_UNIT_0010;
  dest.col1 = V_C_UNIT_0100;
  dest.col2 = V_C_UNIT_1000;
}
VECMATH_FINLINE void VECTORCALL v_mat44_transpose(mat44f &dest, mat44f_cref src)
{
  __m128 tmp3, tmp2, tmp1, tmp0;

  tmp0 = _mm_shuffle_ps(src.col0, src.col1, 0x44);
  tmp2 = _mm_shuffle_ps(src.col0, src.col1, 0xEE);
  tmp1 = _mm_shuffle_ps(src.col2, src.col3, 0x44);
  tmp3 = _mm_shuffle_ps(src.col2, src.col3, 0xEE);

  dest.col0 = _mm_shuffle_ps(tmp0, tmp1, 0x88);
  dest.col1 = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
  dest.col2 = _mm_shuffle_ps(tmp2, tmp3, 0x88);
  dest.col3 = _mm_shuffle_ps(tmp2, tmp3, 0xDD);
}

VECMATH_FINLINE void VECTORCALL v_mat44_transpose(vec4f &r0, vec4f &r1, vec4f &r2, vec4f &r3)
{
   _MM_TRANSPOSE4_PS(r0, r1, r2, r3);
}

VECMATH_FINLINE void VECTORCALL v_mat33_transpose(mat33f &dest, vec3f col0, vec3f col1, vec3f col2)
{
  __m128 tmp3, tmp2, tmp1, tmp0;

  tmp0 = _mm_shuffle_ps(col0, col1, 0x44);
  tmp2 = _mm_shuffle_ps(col0, col1, 0xEE);
  tmp1 = _mm_shuffle_ps(col2, v_zero(), 0x44);//may be v_zero can be replaced with col2
  tmp3 = _mm_shuffle_ps(col2, v_zero(), 0xEE);//may be v_zero can be replaced with col2

  dest.col0 = _mm_shuffle_ps(tmp0, tmp1, 0x88);
  dest.col1 = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
  dest.col2 = _mm_shuffle_ps(tmp2, tmp3, 0x88);
}

VECMATH_FINLINE void VECTORCALL v_mat44_transpose_to_mat33(mat33f &dest, vec3f col0, vec3f col1, vec3f col2, const vec4f& col3)
{
  __m128 tmp3, tmp2, tmp1, tmp0;

  tmp0 = _mm_shuffle_ps(col0, col1, 0x44);
  tmp2 = _mm_shuffle_ps(col0, col1, 0xEE);
  tmp1 = _mm_shuffle_ps(col2, col3, 0x44);
  tmp3 = _mm_shuffle_ps(col2, col3, 0xEE);

  dest.col0 = _mm_shuffle_ps(tmp0, tmp1, 0x88);
  dest.col1 = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
  dest.col2 = _mm_shuffle_ps(tmp2, tmp3, 0x88);
}

VECMATH_FINLINE void VECTORCALL v_mat43_transpose_to_mat44(mat44f &dest, mat43f_cref src)
{
  __m128 tmp3, tmp2, tmp1, tmp0;

  tmp0 = _mm_shuffle_ps(src.row0, src.row1, 0x44);
  tmp2 = _mm_shuffle_ps(src.row0, src.row1, 0xEE);
  tmp1 = _mm_shuffle_ps(src.row2, v_zero(), 0x44);
  tmp3 = _mm_shuffle_ps(src.row2, v_zero(), 0xEE);

  dest.col0 = _mm_shuffle_ps(tmp0, tmp1, 0x88);
  dest.col1 = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
  dest.col2 = _mm_shuffle_ps(tmp2, tmp3, 0x88);
  dest.col3 = _mm_shuffle_ps(tmp2, tmp3, 0xDD);
}
VECMATH_FINLINE void VECTORCALL v_mat44_transpose_to_mat43(mat43f &dest, mat44f_cref src)
{
  __m128 tmp3, tmp2, tmp1, tmp0;

  tmp0 = _mm_shuffle_ps(src.col0, src.col1, 0x44);
  tmp2 = _mm_shuffle_ps(src.col0, src.col1, 0xEE);
  tmp1 = _mm_shuffle_ps(src.col2, src.col3, 0x44);
  tmp3 = _mm_shuffle_ps(src.col2, src.col3, 0xEE);

  dest.row0 = _mm_shuffle_ps(tmp0, tmp1, 0x88);
  dest.row1 = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
  dest.row2 = _mm_shuffle_ps(tmp2, tmp3, 0x88);
}

VECMATH_FINLINE vec4f VECTORCALL v_mat44_mul_vec4(mat44f_cref m, vec4f v)
{
  vec4f xxxx = v_splat_x(v);
  vec4f yyyy = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1,1,1,1));
  vec4f zzzz = v_splat_z(v);
  vec4f wwww = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3,3,3,3));

  return _mm_add_ps(
    _mm_add_ps(_mm_mul_ps(xxxx, m.col0), _mm_mul_ps(yyyy, m.col1)),
    _mm_add_ps(_mm_mul_ps(zzzz, m.col2), _mm_mul_ps(wwww, m.col3))
  );
}
VECMATH_FINLINE vec4f VECTORCALL v_mat44_mul_vec3v(mat44f_cref m, vec3f v)
{
  vec4f xxxx = v_splat_x(v);
  vec4f yyyy = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1,1,1,1));
  vec4f zzzz = v_splat_z(v);

  return _mm_add_ps(_mm_add_ps(_mm_mul_ps(xxxx, m.col0), _mm_mul_ps(yyyy, m.col1)),
                    _mm_mul_ps(zzzz, m.col2));
}
VECMATH_FINLINE vec4f VECTORCALL v_mat44_mul_vec3p(mat44f_cref m, vec3f v)
{
  vec4f xxxx = v_splat_x(v);
  vec4f yyyy = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1,1,1,1));
  vec4f zzzz = v_splat_z(v);

  return _mm_add_ps(
    _mm_add_ps(_mm_mul_ps(xxxx, m.col0), _mm_mul_ps(yyyy, m.col1)),
    _mm_add_ps(_mm_mul_ps(zzzz, m.col2), m.col3)
  );
}

VECMATH_FINLINE vec3f VECTORCALL v_mat33_mul_vec3(mat33f_cref m, vec3f v)
{
  vec4f xxxx = v_splat_x(v);
  vec4f yyyy = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1,1,1,1));
  vec4f zzzz = v_splat_z(v);

  return _mm_add_ps(_mm_add_ps(_mm_mul_ps(xxxx, m.col0), _mm_mul_ps(yyyy, m.col1)),
                    _mm_mul_ps(zzzz, m.col2));
}

VECMATH_FINLINE void VECTORCALL v_mat44_inverse(mat44f &dest, mat44f_cref m)
{
  __m128 minor0, minor1, minor2, minor3;
  __m128 row0, row1, row2, row3;
  __m128 det, tmp0, tmp1, tmp2, tmp3;

  tmp0 = _mm_shuffle_ps(m.col0, m.col1, _MM_SHUFFLE(2,0,2,0));
  tmp1 = _mm_shuffle_ps(m.col2, m.col3, _MM_SHUFFLE(2,0,2,0));
  tmp2 = _mm_shuffle_ps(m.col0, m.col1, _MM_SHUFFLE(3,1,3,1));
  tmp3 = _mm_shuffle_ps(m.col2, m.col3, _MM_SHUFFLE(3,1,3,1));
  row0 = _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(2,0,2,0));
  row1 = _mm_shuffle_ps(tmp3, tmp2, _MM_SHUFFLE(2,0,2,0));
  row2 = _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(3,1,3,1));
  row3 = _mm_shuffle_ps(tmp3, tmp2, _MM_SHUFFLE(3,1,3,1));

  tmp1 = _mm_mul_ps(row2, row3);
  tmp1 = V_SHUFFLE(tmp1, 0xB1);
  minor0 = _mm_mul_ps(row1, tmp1);
  minor1 = _mm_mul_ps(row0, tmp1);
  tmp1 = V_SHUFFLE(tmp1, 0x4E);
  minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
  minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
  minor1 = V_SHUFFLE(minor1, 0x4E);

  tmp1 = _mm_mul_ps(row1, row2);
  tmp1 = V_SHUFFLE(tmp1, 0xB1);
  minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
  minor3 = _mm_mul_ps(row0, tmp1);
  tmp1 = V_SHUFFLE(tmp1, 0x4E);
  minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
  minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
  minor3 = V_SHUFFLE(minor3, 0x4E);

  tmp1 = _mm_mul_ps(V_SHUFFLE(row1, 0x4E), row3);
  tmp1 = V_SHUFFLE(tmp1, 0xB1);
  row2 = V_SHUFFLE(row2, 0x4E);
  minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
  minor2 = _mm_mul_ps(row0, tmp1);
  tmp1 = V_SHUFFLE(tmp1, 0x4E);
  minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
  minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
  minor2 = V_SHUFFLE(minor2, 0x4E);

  tmp1 = _mm_mul_ps(row0, row1);

  tmp1 = V_SHUFFLE(tmp1, 0xB1);
  minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
  minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
  tmp1 = V_SHUFFLE(tmp1, 0x4E);
  minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
  minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));

  tmp1 = _mm_mul_ps(row0, row3);
  tmp1 = V_SHUFFLE(tmp1, 0xB1);
  minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
  minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
  tmp1 = V_SHUFFLE(tmp1, 0x4E);
  minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
  minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));

  tmp1 = _mm_mul_ps(row0, row2);
  tmp1 = V_SHUFFLE(tmp1, 0xB1);
  minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
  minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
  tmp1 = V_SHUFFLE(tmp1, 0x4E);
  minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
  minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);

  det = _mm_mul_ps(row0, minor0);
  det = _mm_add_ps(V_SHUFFLE(det, 0x4E), det);
  det = _mm_add_ss(V_SHUFFLE(det, 0xB1), det);
  tmp1 = _mm_rcp_ss(det);
  det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
  det = v_splat_x(det);
  dest.col0 = _mm_mul_ps(det, minor0);
  dest.col1 = _mm_mul_ps(det, minor1);
  dest.col2 = _mm_mul_ps(det, minor2);
  dest.col3 = _mm_mul_ps(det, minor3);
}

VECMATH_FINLINE void VECTORCALL v_mat33_inverse(mat33f &dest, mat33f_cref m)
{
  vec4f tmp0, tmp1, tmp2, tmp3, tmp4, dot, invdet, inv0, inv1, inv2;

  tmp2 = v_cross3(m.col0, m.col1);
  tmp0 = v_cross3(m.col1, m.col2);
  tmp1 = v_cross3(m.col2, m.col0);
  dot = v_dot3(tmp2, m.col2);
  invdet = v_rcp(dot);

  tmp3 = _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(0,2,0,2));
  tmp4 = _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(1,3,1,3));
  inv0 = _mm_shuffle_ps(tmp3, tmp2, _MM_SHUFFLE(3,0,3,1));
  inv1 = _mm_shuffle_ps(tmp4, tmp2, _MM_SHUFFLE(3,1,3,1));
  inv2 = _mm_shuffle_ps(tmp3, tmp2, _MM_SHUFFLE(3,2,2,0));

  dest.col0 = _mm_mul_ps(inv0, invdet);
  dest.col1 = _mm_mul_ps(inv1, invdet);
  dest.col2 = _mm_mul_ps(inv2, invdet);
}

VECMATH_FINLINE vec4f VECTORCALL v_mat44_det(mat44f_cref m)
{
  __m128 minor0;
  __m128 row0, row1, row2, row3;
  __m128 det, tmp0, tmp1, tmp2, tmp3;

  tmp0 = _mm_shuffle_ps(m.col0, m.col1, _MM_SHUFFLE(2,0,2,0));
  tmp1 = _mm_shuffle_ps(m.col2, m.col3, _MM_SHUFFLE(2,0,2,0));
  tmp2 = _mm_shuffle_ps(m.col0, m.col1, _MM_SHUFFLE(3,1,3,1));
  tmp3 = _mm_shuffle_ps(m.col2, m.col3, _MM_SHUFFLE(3,1,3,1));
  row0 = _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(2,0,2,0));
  row1 = _mm_shuffle_ps(tmp3, tmp2, _MM_SHUFFLE(2,0,2,0));
  row2 = _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(3,1,3,1));
  row3 = _mm_shuffle_ps(tmp3, tmp2, _MM_SHUFFLE(3,1,3,1));

  tmp1 = _mm_mul_ps(row2, row3);
  tmp1 = V_SHUFFLE(tmp1, 0xB1);
  minor0 = _mm_mul_ps(row1, tmp1);
  tmp1 = V_SHUFFLE(tmp1, 0x4E);
  minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);

  tmp1 = _mm_mul_ps(row1, row2);
  tmp1 = V_SHUFFLE(tmp1, 0xB1);
  minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
  tmp1 = V_SHUFFLE(tmp1, 0x4E);
  minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));

  tmp1 = _mm_mul_ps(V_SHUFFLE(row1, 0x4E), row3);
  tmp1 = V_SHUFFLE(tmp1, 0xB1);
  row2 = V_SHUFFLE(row2, 0x4E);
  minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
  tmp1 = V_SHUFFLE(tmp1, 0x4E);
  minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));

  det = _mm_mul_ps(row0, minor0);
  det = _mm_add_ps(V_SHUFFLE(det, 0x4E), det);
  det = _mm_add_ss(V_SHUFFLE(det, 0xB1), det);
  return det;
}

VECMATH_FINLINE vec3f VECTORCALL v_bbox3_center(bbox3f_cref b)
{
  return v_mul(v_add(b.bmin, b.bmax), V_C_HALF);
}
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_outer_rad(vec3f bmin, vec3f bmax)
{
  return v_mul_x(V_C_HALF, v_length3_x(v_sub(bmax, bmin)));
}
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_inner_rad(vec3f bmin, vec3f bmax)
{
  vec4f s = v_sub(bmax, bmin);
  return _mm_mul_ss(V_C_HALF, _mm_min_ss(s,
                    _mm_min_ss(V_SHUFFLE_REV(s, 0, 0, 0, 1),
                               _mm_shuffle_ps(s, s, _MM_SHUFFLE(0,0,0,2)))));
}
VECMATH_FINLINE int VECTORCALL v_bbox3_test_pt_inside_b(bbox3f_cref b, vec3f p)
{
  vec3f m = v_and(v_cmp_ge(p, b.bmin), v_cmp_ge(b.bmax, p));
  return (_mm_movemask_ps(m) & 0x7)==0x7;
}

VECMATH_FINLINE int VECTORCALL v_bbox3_test_pt_inside_b_xz(bbox3f_cref b, vec3f p)
{
  vec3f m = v_and(v_cmp_ge(p, b.bmin), v_cmp_ge(b.bmax, p));
  return (_mm_movemask_ps(m) & 0x5)==0x5;
}

VECMATH_FINLINE int VECTORCALL v_bbox3_test_box_inside_b(bbox3f_cref b, bbox3f_cref b2)
{
  vec3f m1 = v_and(v_cmp_ge(b2.bmin, b.bmin), v_cmp_ge(b.bmax, b2.bmin));
  vec3f m2 = v_and(v_cmp_ge(b2.bmax, b.bmin), v_cmp_ge(b.bmax, b2.bmax));
  vec3f m = v_and(m1, m2);
  return (_mm_movemask_ps(m) & 0x7)==0x7;
}
VECMATH_FINLINE int VECTORCALL v_bbox3_test_box_intersect_b(bbox3f_cref b1, bbox3f_cref b2)
{
  vec3f m = v_and(v_cmp_ge(b2.bmax, b1.bmin), v_cmp_ge(b1.bmax, b2.bmin));
  return (_mm_movemask_ps(m) & 0x7)==0x7;
}
VECMATH_FINLINE int VECTORCALL v_bbox3_test_box_intersect_b_safe(bbox3f_cref b1, bbox3f_cref b2)
{
  vec3f m = v_and(v_and(v_cmp_ge(b1.bmax, b1.bmin), v_cmp_ge(b2.bmax, b2.bmin)),
                  v_and(v_cmp_ge(b2.bmax, b1.bmin), v_cmp_ge(b1.bmax, b2.bmin)));
  return (_mm_movemask_ps(m) & 0x7)==0x7;
}

VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt001(bbox3f_cref b){ return v_perm_xycd(b.bmin, b.bmax); }
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt010(bbox3f_cref b){ return v_perm_xbzw(b.bmin, b.bmax); }
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt011(bbox3f_cref b){ return v_perm_ayzw(b.bmax, b.bmin); }
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt100(bbox3f_cref b){ return v_perm_ayzw(b.bmin, b.bmax); }
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt101(bbox3f_cref b){ return v_perm_xbzw(b.bmax, b.bmin); }
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt110(bbox3f_cref b){ return v_perm_xycd(b.bmax, b.bmin); }

#if defined(_MSC_VER) && (_MSC_VER < 1600 || (_MSC_VER < 1700 && _TARGET_64BIT)) && !defined(__clang__)
//due to compiler bug (vc2008-32 and vc2010-64)
VECMATH_FINLINE float VECTORCALL v_extract_x(vec4f v) { return v.m128_f32[0]; }
VECMATH_FINLINE float VECTORCALL v_extract_y(vec4f v) { return v.m128_f32[1]; }
VECMATH_FINLINE float VECTORCALL v_extract_z(vec4f v) { return v.m128_f32[2]; }
VECMATH_FINLINE float VECTORCALL v_extract_w(vec4f v) { return v.m128_f32[3]; }
#else
VECMATH_FINLINE float VECTORCALL v_extract_x( vec4f a ) { return _mm_cvtss_f32(a); }
VECMATH_FINLINE float VECTORCALL v_extract_y( vec4f a ) { return _mm_cvtss_f32(v_splat_y(a)); }
VECMATH_FINLINE float VECTORCALL v_extract_z( vec4f a ) { return _mm_cvtss_f32(v_splat_z(a)); }
VECMATH_FINLINE float VECTORCALL v_extract_w( vec4f a ) { return _mm_cvtss_f32(v_splat_w(a)); }
#endif

#if defined(__GNUC__) || defined(__clang__)
VECMATH_FINLINE float VECTORCALL v_extract(vec4f v, int i) { alignas(16) float r[4]; _mm_store_ps(r, v); return r[i]; }
VECMATH_FINLINE int VECTORCALL v_extracti(vec4i v, int i) { alignas(16) int r[4]; _mm_store_si128((__m128i*)r, v); return r[i]; }
#else
VECMATH_FINLINE float VECTORCALL v_extract(vec4f v, int i) { return v.m128_f32[i]; }
VECMATH_FINLINE int VECTORCALL v_extracti(vec4i v, int i) { return v.m128i_i32[i]; }
#endif

VECMATH_FINLINE int VECTORCALL v_extract_xi(vec4i v) {return _mm_cvtsi128_si32(v);}
VECMATH_FINLINE int VECTORCALL v_extract_yi(vec4i v) {return _mm_cvtsi128_si32(_mm_shuffle_epi32(v, _MM_SHUFFLE(1,1,1,1)));}
VECMATH_FINLINE int VECTORCALL v_extract_zi(vec4i v) {return _mm_cvtsi128_si32(_mm_shuffle_epi32(v, _MM_SHUFFLE(2,2,2,2)));}
VECMATH_FINLINE int VECTORCALL v_extract_wi(vec4i v) {return _mm_cvtsi128_si32(_mm_shuffle_epi32(v, _MM_SHUFFLE(3,3,3,3)));}

VECMATH_FINLINE int64_t VECTORCALL v_extract_xi64 ( vec4i a )
{
#if defined(_MSC_VER) && !defined(__clang__)//visual studio is not capable of produce reasonable code otherwise!
    return a.m128i_i64[0];
#else
    int64_t t; _mm_storel_epi64((__m128i*)&t, a); return t;
#endif
}
VECMATH_FINLINE vec4i VECTORCALL v_splatsi64(int64_t a) {return _mm_loadl_epi64((const __m128i*)&a);}

VECMATH_FINLINE short VECTORCALL v_extract_xi16(vec4i v) {return (short)_mm_extract_epi16(v, 0);}


VECMATH_FINLINE int VECTORCALL v_test_vec_x_eq(vec3f v, vec3f a)
{
  vec4i eq = _mm_cmpeq_epi32(v_cast_vec4i(v),v_cast_vec4i(a));
  return _mm_movemask_ps(v_cast_vec4f(eq))&1;
}
VECMATH_FINLINE int VECTORCALL v_test_vec_x_gt(vec3f v, vec3f a) { return _mm_comigt_ss(v,a); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_ge(vec3f v, vec3f a) { return _mm_comige_ss(v,a); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_lt(vec3f v, vec3f a) { return _mm_comilt_ss(v,a); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_le(vec3f v, vec3f a) { return _mm_comile_ss(v,a); }

VECMATH_FINLINE int VECTORCALL v_test_vec_x_eq_0(vec3f v) { return v_test_vec_x_eq(v, v_zero()); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_gt_0(vec3f v) { return v_test_vec_x_gt(v, v_zero()); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_ge_0(vec3f v) { return v_test_vec_x_ge(v, v_zero()); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_lt_0(vec3f v) { return v_test_vec_x_lt(v, v_zero()); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_le_0(vec3f v) { return v_test_vec_x_le(v, v_zero()); }

#undef V_SHUFFLE
#undef  V_SHUFFLE_REV
#undef  V_SHUFFLE_FWD

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif
