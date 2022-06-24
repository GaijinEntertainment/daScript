/*
 * Dagor Engine 5
 * Copyright (C) 2003-2021  Gaijin Entertainment.  All rights reserved
 *
 * (for conditions of distribution and use, see License)
*/

#ifndef _DAGOR_PUBLIC_MATH_DAG_VECMATH_NEON_H_
#define _DAGOR_PUBLIC_MATH_DAG_VECMATH_NEON_H_
#pragma once

#include <math.h> //for fabsf, which is used once, and not wise

#define VECMATH_NEON_SQRT_MIN_THRESHOLD_VALUE 1e-16

VECMATH_FINLINE vec4f VECTORCALL v_zero() { return vdupq_n_f32(0); }
VECMATH_FINLINE vec4i VECTORCALL v_zeroi() { return vdupq_n_u32(0); }
VECMATH_FINLINE vec4f VECTORCALL v_msbit() { return (vec4f)vdupq_n_u32(0x80000000); }
VECMATH_FINLINE vec4f VECTORCALL v_splat4(const float *a) { return vld1q_dup_f32(a); }
VECMATH_FINLINE vec4f VECTORCALL v_ld(const float *m) { return vld1q_f32(m); }
VECMATH_FINLINE vec4f VECTORCALL v_ldu(const float *m) { return vld1q_f32(m); }
VECMATH_FINLINE vec4f VECTORCALL v_ldu_x(const float *m) { return vsetq_lane_f32(*m, v_zero(), 0); } // load x, zero others
VECMATH_FINLINE vec4i VECTORCALL v_ldi(const int *m) { return vld1q_s32(m); }
VECMATH_FINLINE vec4i VECTORCALL v_ldui(const int *m) { return vld1q_s32(m); }
VECMATH_FINLINE vec4i VECTORCALL v_ldush(const signed short *m) { return vmovl_s16(vld1_s16(m)); }
VECMATH_FINLINE vec4i VECTORCALL v_lduush(const unsigned short *m) { return (vec4i)vmovl_u16(vld1_u16(m)); }
VECMATH_FINLINE vec4f VECTORCALL v_splat_x(vec4f a) { return vdupq_lane_f32(vget_low_f32(a), 0); }
VECMATH_FINLINE vec4f VECTORCALL v_splat_y(vec4f a) { return vdupq_lane_f32(vget_low_f32(a), 1); }
VECMATH_FINLINE vec4f VECTORCALL v_splat_z(vec4f a) { return vdupq_lane_f32(vget_high_f32(a), 0); }
VECMATH_FINLINE vec4f VECTORCALL v_splat_w(vec4f a) { return vdupq_lane_f32(vget_high_f32(a), 1); }
VECMATH_FINLINE vec4i VECTORCALL v_splat_xi(vec4i a) { return vdupq_lane_s32(vget_low_s32(a), 0); }
VECMATH_FINLINE vec4i VECTORCALL v_splat_yi(vec4i a) { return vdupq_lane_s32(vget_low_s32(a), 1); }
VECMATH_FINLINE vec4i VECTORCALL v_splat_zi(vec4i a) { return vdupq_lane_s32(vget_high_s32(a), 0); }
VECMATH_FINLINE vec4i VECTORCALL v_splat_wi(vec4i a) { return vdupq_lane_s32(vget_high_s32(a), 1); }

VECMATH_FINLINE vec4f VECTORCALL v_splats(float a) { return vmovq_n_f32(a); }
VECMATH_FINLINE vec4i VECTORCALL v_splatsi(int a) {return vmovq_n_s32(a);}
VECMATH_FINLINE vec4i VECTORCALL v_splatsi64(int64_t a) {return vreinterpretq_s32_s64(vmovq_n_s64(a));}
VECMATH_FINLINE vec4f VECTORCALL v_set_x(float a) { return vsetq_lane_f32(a, v_zero(), 0); } // set x, zero others
VECMATH_FINLINE vec4i VECTORCALL v_seti_x(int a) { return vsetq_lane_s32(a, v_zero(), 0); } // set x, zero others
VECMATH_FINLINE vec4f VECTORCALL v_make_vec4f(float x, float y, float z, float w) { return (vec4f){x, y, z, w}; }
VECMATH_FINLINE vec4i VECTORCALL v_make_vec4i(int x, int y, int z, int w)
{
  alignas(16) int data[4] = { x, y, z, w };
  return vld1q_s32(data);
}

VECMATH_FINLINE void VECTORCALL v_st(void *m, vec4f v)  { vst1q_f32((float*)m, v); }
VECMATH_FINLINE void VECTORCALL v_stu(void *m, vec4f v) { vst1q_f32((float*)m, v); }
VECMATH_FINLINE void VECTORCALL v_sti(void *m, vec4i v)  { vst1q_s32((int*)m, v); }
VECMATH_FINLINE void VECTORCALL v_stui(void *m, vec4i v) { vst1q_s32((int*)m, v); }
VECMATH_FINLINE void VECTORCALL v_stui_half(void *m, vec4i v) { vst1_s32((int*)m, vget_low_s32(v)); }
VECMATH_FINLINE void VECTORCALL v_stu_half(void *m, vec4f v) { vst1_f32((float*)m, vget_low_f32(v)); }
VECMATH_FINLINE void VECTORCALL v_stu_p3(float *p3, vec3f v) { v_stu_half(p3, v); p3[2] = v_extract_z(v); }

VECMATH_FINLINE vec4f VECTORCALL v_merge_hw(vec4f a, vec4f b)
{
  return vzipq_f32(a, b).val[0];
}
VECMATH_FINLINE vec4f VECTORCALL v_merge_lw(vec4f a, vec4f b)
{
  return vzipq_f32(a, b).val[1];
}

VECMATH_FINLINE int VECTORCALL v_signmask(vec4f V)
{
  #if 1 //ifndef __AARCH64_SIMD__  //== since vtbl1q_u8 is not availale!
  static const uint32x4_t movemask = { 1, 2, 4, 8 };
  static const uint32x4_t highbit = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
  uint32x4_t t0 = vreinterpretq_u32_f32(V);
  uint32x4_t t1 = vtstq_u32(t0, highbit);
  uint32x4_t t2 = vandq_u32(t1, movemask);
  uint32x2_t t3 = vorr_u32(vget_low_u32(t2), vget_high_u32(t2));
  return vget_lane_u32(t3, 0) | vget_lane_u32(t3, 1);
  #else
  const uint8x16_t tbl = vcombine_u8((uint8x8_t)vget_low_f32(V), (uint8x8_t)vget_high_f32(V));
  const uint8x8_t idx_sign = { 3, 7, 11, 15 }; // MSB -> LSB = W -> X (0xWWZZYYXX)
  const uint8x8_t sign_bytes = vtbl1q_u8(tbl, idx_sign);
  return vget_lane_u32((uint32x2_t)sign_bytes, 0);
  #endif
}

VECMATH_FINLINE vec4f VECTORCALL v_cmp_eq(vec4f a, vec4f b) { return (vec4f)vceqq_f32(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_cmp_eqi(vec4f a, vec4f b) { return (vec4f)vceqq_s32((vec4i)a, (vec4i)b); }
VECMATH_FINLINE vec4i VECTORCALL v_cmp_eqi(vec4i a, vec4i b) { return (vec4i)vceqq_s32(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_cmp_ge(vec4f a, vec4f b) { return (vec4f)vcgeq_f32(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_cmp_gt(vec4f a, vec4f b) { return (vec4f)vcgtq_f32(a, b); }
VECMATH_FINLINE vec4i VECTORCALL v_cmp_lti(vec4i a, vec4i b) { return (vec4i)vcltq_s32(a, b); }
VECMATH_FINLINE vec4i VECTORCALL v_cmp_gti(vec4i a, vec4i b) { return (vec4i)vcgtq_s32(a, b); }

VECMATH_FINLINE vec4f VECTORCALL v_and(vec4f a, vec4f b) { return (vec4f)vandq_s32((vec4i)a, (vec4i)b); }
VECMATH_FINLINE vec4f VECTORCALL v_andnot(vec4f a, vec4f b) { return (vec4f)vandq_s32(vmvnq_s32((vec4i)a), (vec4i)b); }
VECMATH_FINLINE vec4f VECTORCALL v_or(vec4f a, vec4f b) { return (vec4f)vorrq_s32((vec4i)a, (vec4i)b); }
VECMATH_FINLINE vec4f VECTORCALL v_xor(vec4f a, vec4f b) { return (vec4f)veorq_s32((vec4i)a, (vec4i)b); }
VECMATH_FINLINE vec4f VECTORCALL v_sel(vec4f a, vec4f b, vec4f c) { return vbslq_f32((uint32x4_t)c, b, a); }
VECMATH_FINLINE vec4i VECTORCALL v_seli(vec4i a, vec4i b, vec4i c) { return vbslq_s32((uint32x4_t)c, b, a); }
VECMATH_FINLINE float32x2_t VECTORCALL v_sel2(float32x2_t a, float32x2_t b, int32x2_t c) { return vbsl_f32((uint32x2_t)c, b, a); }

VECMATH_FINLINE int VECTORCALL v_check_xyz_non_zero(vec4f a)
{
  const uint32x2_t xy = vget_low_u32((uint32x4_t)a);
  const uint32x2_t zz = vdup_lane_u32(vget_high_u32((uint32x4_t)a), 0);
  uint32x2_t m_and = vand_u32(xy, zz);
  m_and = vand_u32(m_and, vdup_lane_u32(xy, 1));
  return vget_lane_u32(m_and, 0);
}

VECMATH_FINLINE vec4i VECTORCALL v_cvt_vec4i(vec4f a) { return vcvtq_s32_f32(a); }
VECMATH_FINLINE vec4f VECTORCALL v_cvt_vec4f(vec4i a) { return vcvtq_f32_s32(a); }

VECMATH_FINLINE vec4i VECTORCALL v_cast_vec4i(vec4f a) { return vreinterpretq_s32_f32(a); }
VECMATH_FINLINE vec4f VECTORCALL v_cast_vec4f(vec4i a) { return vreinterpretq_f32_s32(a); }

VECMATH_FINLINE vec4f VECTORCALL v_floor(vec4f a)
{
  vec4i xi, xi1;
  vec4f inrange;
  vec4f truncated, truncated1;

  // Find truncated value and one less.
  inrange = v_cmp_gt((vec4f)vdupq_n_u32(0x4b000000), v_abs(a));

  xi = v_cvt_vec4i(a);
  xi1 = vsubq_s32(xi, vdupq_n_s32(1));

  truncated = v_sel(a, v_cvt_vec4f(xi), inrange);
  truncated1 = v_sel(a, v_cvt_vec4f(xi1), inrange);

  // If truncated value is greater than input, subtract one.
  return v_sel(truncated, truncated1, v_cmp_gt(truncated, a));
}
VECMATH_FINLINE vec4f VECTORCALL v_ceil(vec4f a)
{
  vec4i xi, xi1;
  vec4f inrange;
  vec4f truncated, truncated1;

  // Find truncated value and one less.
  inrange = v_cmp_gt((vec4f)vdupq_n_u32(0x4b000000), v_abs(a));

  xi = v_cvt_vec4i(a);
  xi1 = vaddq_s32(xi, vdupq_n_s32(1));

  truncated = v_sel(a, v_cvt_vec4f(xi), inrange);
  truncated1 = v_sel(a, v_cvt_vec4f(xi1), inrange);

  // If truncated value is greater than input, subtract one.
  return v_sel(truncated, truncated1, v_cmp_gt(a, truncated));
}
VECMATH_FINLINE vec4i VECTORCALL v_cvt_floori(vec4f a)
{
  vec4i xi = v_cvt_vec4i(a);
  vec4i xi1 = vsubq_s32(xi, vdupq_n_s32(1));
  return (vec4i)v_sel((vec4f)xi, (vec4f)xi1, v_cmp_gt(v_cvt_vec4f(xi), a));
}

VECMATH_FINLINE vec4f VECTORCALL sse4_floor(vec4f a) { return v_floor(a); }
VECMATH_FINLINE vec4f VECTORCALL sse4_ceil(vec4f a) { return v_ceil(a); }
VECMATH_FINLINE vec4f VECTORCALL sse4_round(vec4f a) { return v_round(a); }
VECMATH_FINLINE vec4i VECTORCALL sse4_cvt_floori(vec4f a) { return v_cvt_floori(a); }
VECMATH_FINLINE vec4i VECTORCALL sse4_cvt_ceili(vec4f a)  { return v_cvt_ceili(a); }

VECMATH_FINLINE vec4i VECTORCALL v_cvt_ceili(vec4f a)
{
  vec4i xi = v_cvt_vec4i(a);
  vec4i xi1 = vaddq_s32(xi, vdupq_n_s32(1));
  return (vec4i)v_sel((vec4f)xi, (vec4f)xi1, v_cmp_gt(a, v_cvt_vec4f(xi)));
}

VECMATH_FINLINE vec4f VECTORCALL v_round(vec4f a) { return v_floor(v_add(a, V_C_HALF)); }
VECMATH_FINLINE vec4i VECTORCALL v_cvt_roundi(vec4f a) { return v_cvt_floori(v_add(a, V_C_HALF)); }

VECMATH_FINLINE vec4f VECTORCALL v_add(vec4f a, vec4f b) { return vaddq_f32(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_sub(vec4f a, vec4f b) { return vsubq_f32(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_mul(vec4f a, vec4f b) { return vmulq_f32(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_madd(vec4f a, vec4f b, vec4f c) { return vmlaq_f32(c, a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_nmsub(vec4f a, vec4f b, vec4f c) { return vmlsq_f32(c, a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_add_x(vec4f a, vec4f b) { return vaddq_f32(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_sub_x(vec4f a, vec4f b) { return vsubq_f32(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_mul_x(vec4f a, vec4f b) { return vmulq_f32(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_madd_x(vec4f a, vec4f b, vec4f c) { return vmlaq_f32(c, a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_nmsub_x(vec4f a, vec4f b, vec4f c) { return vmlsq_f32(c, a, b); }

VECMATH_FINLINE vec4i VECTORCALL v_addi(vec4i a, vec4i b) { return vaddq_s32(a, b); }
VECMATH_FINLINE vec4i VECTORCALL v_subi(vec4i a, vec4i b) { return vsubq_s32(a, b); }
VECMATH_FINLINE vec4i VECTORCALL v_muli(vec4i a, vec4i b) { return vmulq_s32(a, b); }

VECMATH_FINLINE vec4f VECTORCALL v_rcp_est(vec4f a) { return vrecpeq_f32(a); }
VECMATH_FINLINE vec4f VECTORCALL v_rcp_iter(vec4f a, vec4f est)
{
  return vmulq_f32(est, vrecpsq_f32(est, a));
}
VECMATH_FINLINE vec4f VECTORCALL v_rcp(vec4f a)
{
  return v_rcp_iter(a, v_rcp_iter(a, vrecpeq_f32(a)));
}
VECMATH_FINLINE vec4f VECTORCALL v_rcp_est_x(vec4f a) { return vrecpeq_f32(a); }
VECMATH_FINLINE vec4f VECTORCALL v_rcp_x(vec4f a) { return v_rcp(a); }
VECMATH_FINLINE vec4f VECTORCALL v_div(vec4f a, vec4f b) { return v_mul(a, v_rcp(b)); }
VECMATH_FINLINE vec4f VECTORCALL v_div_x(vec4f a, vec4f b) { return v_mul(a, v_rcp(b)); }
VECMATH_FINLINE vec4f VECTORCALL v_min(vec4f a, vec4f b) { return vminq_f32(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_max(vec4f a, vec4f b) { return vmaxq_f32(a, b); }
VECMATH_FINLINE vec4f VECTORCALL v_neg(vec4f a) { return vnegq_f32(a); }
VECMATH_FINLINE vec4i VECTORCALL v_negi(vec4i a){ return vnegq_s32(a); }
VECMATH_FINLINE vec4f VECTORCALL v_abs(vec4f a) { return vabsq_f32(a); }
VECMATH_FINLINE vec4i VECTORCALL v_absi(vec4i a) { return vabsq_s32(a); }
VECMATH_FINLINE vec4i VECTORCALL v_maxi(vec4i a, vec4i b) { return vmaxq_s32(a, b); }
VECMATH_FINLINE vec4i VECTORCALL v_mini(vec4i a, vec4i b) { return vminq_s32(a, b); }

VECMATH_FINLINE vec4f VECTORCALL v_rsqrt4_fast(vec4f a) { return vrsqrteq_f32(a); }
VECMATH_FINLINE vec4f VECTORCALL v_rsqrt4(vec4f a)
{
  vec4f e = vrsqrteq_f32(a);
  e = vmulq_f32(e , vrsqrtsq_f32(vmulq_f32(e, e), a));
  return vmulq_f32(e , vrsqrtsq_f32(vmulq_f32(e, e), a));
}

VECMATH_FINLINE vec4f VECTORCALL v_rsqrt_fast_x(vec4f a)
{
  float32x2_t e = vrsqrte_f32(vget_low_f32(a));
  return vcombine_f32(e, e);
}
VECMATH_FINLINE float32x2_t VECTORCALL v_rsqrt_x(float32x2_t a)
{
  float32x2_t e = vrsqrte_f32(a);
  e = vmul_f32(e, vrsqrts_f32(vmul_f32(e, e), a));
  return vmul_f32(e, vrsqrts_f32(vmul_f32(e, e), a));
}
VECMATH_FINLINE vec4f VECTORCALL v_rsqrt_x(vec4f a) { float32x2_t r = v_rsqrt_x(vget_low_f32(a)); return vcombine_f32(r, r); }

//sqrt of very small number (~1e-22) returns INF instead of zero, so use bigger threshold (VECMATH_NEON_SQRT_MIN_THRESHOLD_VALUE) then 0
//added for compatibility with SSE implementation
VECMATH_FINLINE vec4f VECTORCALL v_sqrt4_fast(vec4f a)
{
  return v_and(v_mul(a, vrsqrteq_f32(a)), v_cmp_gt(a, vdupq_n_f32(VECMATH_NEON_SQRT_MIN_THRESHOLD_VALUE)));
}
VECMATH_FINLINE vec4f VECTORCALL v_sqrt4(vec4f a)
{
  return v_and(v_mul(a, v_rsqrt4(a)), v_cmp_gt(a, vdupq_n_f32(VECMATH_NEON_SQRT_MIN_THRESHOLD_VALUE)));
}
VECMATH_FINLINE vec4f VECTORCALL v_sqrt_fast_x(vec4f _a)
{
  float32x2_t a = vget_low_f32(_a);
  float32x2_t r = (float32x2_t)vand_s32((int32x2_t)vmul_f32(a, vrsqrte_f32(a)),
    vcgt_f32(a, vdup_n_f32(VECMATH_NEON_SQRT_MIN_THRESHOLD_VALUE)));
  return vcombine_f32(r, r);
}
VECMATH_FINLINE vec4f VECTORCALL v_sqrt_x(vec4f _a)
{
  float32x2_t a = vget_low_f32(_a);
  float32x2_t r = (float32x2_t)vand_s32(vmul_f32(a, v_rsqrt_x(a)), vcgt_f32(a, vdup_n_f32(VECMATH_NEON_SQRT_MIN_THRESHOLD_VALUE)));
  return vcombine_f32(r, r);
}

VECMATH_FINLINE vec4f VECTORCALL v_rot_1(vec4f a) { return vextq_f32(a, a, 1); }
VECMATH_FINLINE vec4f VECTORCALL v_rot_2(vec4f a) { return vextq_f32(a, a, 2); }
VECMATH_FINLINE vec4f VECTORCALL v_rot_3(vec4f a) { return vextq_f32(a, a, 3); }

VECMATH_FINLINE vec4f VECTORCALL v_perm_yxwz(vec4f a)
{
  return __builtin_shufflevector(a, a, 1, 0, 3, 2);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_yzxy(vec4f a)
{
  return __builtin_shufflevector(a, a, 1, 2, 0, 1);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_yzxx(vec4f a)
{
  return __builtin_shufflevector(a, a, 1, 2, 0, 0);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_yzxw(vec4f a)
{
  return __builtin_shufflevector(a, a, 1, 2, 0, 3);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_zxyz(vec4f a)
{
  return __builtin_shufflevector(a, a, 2, 0, 1, 2);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_zxyw(vec4f a)
{
  return vuzpq_f32(vextq_f32(a, a, 1), a).val[1];
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xxzz(vec4f a)
{
  return __builtin_shufflevector(a, a, 0, 0, 2, 2);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_yyww(vec4f a)
{
  return __builtin_shufflevector(a, a, 1, 1, 3, 3);
}

VECMATH_FINLINE vec4f VECTORCALL v_perm_xyxy(vec4f a)
{
  return __builtin_shufflevector(a, a, 0, 1, 0, 1);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_zwzw(vec4f a)
{
  return __builtin_shufflevector(a, a, 2, 3, 2, 3);
}

VECMATH_FINLINE vec4f VECTORCALL v_perm_xaxa(vec4f xyzw, vec4f abcd)
{
  float32x2_t xy = vget_low_f32(xyzw);
  float32x2_t XY = vget_low_f32(abcd);
  float32x2x2_t xX_yY = vtrn_f32(xy, XY);
  return vcombine_f32(xX_yY.val[0], xX_yY.val[0]);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_yybb(vec4f xyzw, vec4f abcd)
{
  float32x2_t yy = vdup_lane_f32(vget_low_f32(xyzw), 1);
  float32x2_t bb = vdup_lane_f32(vget_low_f32(abcd), 1);
  return vcombine_f32(yy, bb);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xyab(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 0, 1, 4, 5);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xycd(vec4f xyzw, vec4f abcd)
{
  return vcombine_f32(vget_low_f32(xyzw), vget_high_f32(abcd));
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xxaa(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 0, 0, 4, 4);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_wwdd(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 3, 3, 7, 7);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_yxba(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 1, 0, 5, 4);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_wzdc(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 3, 2, 7, 6);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_zwcd(vec4f xyzw, vec4f abcd)
{
  return vcombine_f32(vget_high_f32(xyzw), vget_high_f32(abcd));
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_zwba(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 2, 3, 5, 4);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_zbwa(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 2, 5, 3, 4);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xzac(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 0, 2, 4, 6);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_ywbd(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 1, 3, 5, 7);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xycx(vec4f xyzw, vec4f abcd)
{
  float32x2_t xy = vget_low_f32(xyzw);
  float32x2x2_t cx_dy = vzip_f32(vget_high_f32(abcd), xy);
  float32x2_t cx = cx_dy.val[0];
  return vcombine_f32(xy, cx);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_ycxy(vec4f xyzw, vec4f abcd)
{
  float32x2_t xy = vget_low_f32(xyzw);
  float32x2_t cd = vget_high_f32(abcd);
  float32x2_t yc = vext_f32(xy, cd, 1);
  return vcombine_f32(yc, xy);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_cxyc(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 6, 0, 1, 6);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xycc(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 0, 1, 6, 6);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xbzz(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 0, 5, 2, 2);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xbcc(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 0, 5, 6, 6);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xbzw(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 0, 5, 2, 3);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_zayx(vec4f xyzw, vec4f abcd)
{
  float32x2x2_t za_wb = vtrn_f32(vget_high_f32(xyzw), vget_low_f32(abcd));
  float32x2_t za = za_wb.val[0];
  float32x2_t yx = vrev64_f32(vget_low_f32(xyzw));
  return vcombine_f32(za, yx);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_zxxb(vec4f xyzw, vec4f abcd)
{
  float32x2_t xy = vget_low_f32(xyzw);
  float32x2_t zw = vget_high_f32(xyzw);
  float32x2_t ba = vrev64_f32(vget_low_f32(abcd));
  float32x2x2_t zx_wy = vzip_f32(zw, xy);
  float32x2x2_t xb_ya = vzip_f32(xy, ba);
  return vcombine_f32(zx_wy.val[0], xb_ya.val[0]);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_yxxc(vec4f xyzw, vec4f abcd)
{
  float32x2_t xy = vget_low_f32(xyzw);
  float32x2_t cd = vget_high_f32(abcd);
  return vcombine_f32(vrev64_f32(xy), vzip_f32(xy, cd).val[0]);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_yxac(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 1, 0, 4, 6);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xyzd(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 0, 1, 2, 7);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xycw(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 0, 1, 6, 3);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_bzxx(vec4f xyzw, vec4f abcd)
{
  float32x2_t bz = vext_f32(vget_low_f32(abcd), vget_high_f32(xyzw), 1);
  float32x2_t xx = vdup_lane_f32(vget_low_f32(xyzw), 0);
  return vcombine_f32(bz, xx);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xzya(vec4f xyzw, vec4f abcd)
{
  float32x2_t xy = vget_low_f32(xyzw);
  float32x2_t yx = vrev64_f32(xy);
  float32x2x2_t xz_yw = vzip_f32(xy, vget_high_f32(xyzw));
  float32x2x2_t ya_xb = vzip_f32(yx, vget_low_f32(abcd));
  return vcombine_f32(xz_yw.val[0], ya_xb.val[0]);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_ayzw(vec4f xyzw, vec4f abcd)
{
  return __builtin_shufflevector(xyzw, abcd, 4, 1, 2, 3);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_caxx(vec4f xyzw, vec4f abcd)
{
  float32x2_t dc = vrev64_f32(vget_high_f32(abcd));
  float32x2_t xx = vdup_lane_f32(vget_low_f32(xyzw), 0);
  float32x2_t ca = vext_f32(dc, vget_low_f32(abcd), 1);
  return vcombine_f32(ca, xx);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_yaxx(vec4f xyzw, vec4f abcd)
{
  float32x2_t xy = vget_low_f32(xyzw);
  float32x2_t ya = vext_f32(xy, vget_low_f32(abcd), 1);
  float32x2_t xx = vdup_lane_f32(xy, 0);
  return vcombine_f32(ya, xx);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_bbyx(vec4f xyzw, vec4f abcd)
{
  float32x2_t bb = vdup_lane_f32(vget_low_f32(abcd), 1);
  float32x2_t yx = vrev64_f32(vget_low_f32(xyzw));
  return vcombine_f32(bb, yx);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xzbx(vec4f xyzw, vec4f abcd)
{
  float32x2_t xy = vget_low_f32(xyzw);
  float32x2_t xz = vzip_f32(xy, vget_high_f32(xyzw)).val[0];
  float32x2_t bx = vext_f32(vget_low_f32(abcd), xy, 1);
  return vcombine_f32(xz, bx);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_xzxz(vec4f xyzw)
{
  return __builtin_shufflevector(xyzw, xyzw, 0, 2, 0, 2);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_ywyw(vec4f xyzw)
{
  return __builtin_shufflevector(xyzw, xyzw, 1, 3, 1, 3);
}

VECMATH_FINLINE vec4f VECTORCALL v_perm_xxyy(vec4f xyzw)
{
  return __builtin_shufflevector(xyzw, xyzw, 0, 0, 1, 1);
}
VECMATH_FINLINE vec4f VECTORCALL v_perm_zzww(vec4f xyzw)
{
  return __builtin_shufflevector(xyzw, xyzw, 2, 2, 3, 3);
}


VECMATH_FINLINE vec4f VECTORCALL v_transpose3x(vec4f a, vec4f b, vec4f c)
{
  float32x2_t a0b0 = vtrn_f32(vget_low_f32(a), vget_low_f32(b)).val[0];
  float32x2_t c0c0 = vdup_lane_f32(vget_low_f32(c), 0);
  return vcombine_f32(a0b0, c0c0);
}
VECMATH_FINLINE vec4f VECTORCALL v_transpose3y(vec4f a, vec4f b, vec4f c)
{
  float32x2_t a1b1 = vtrn_f32(vget_low_f32(a), vget_low_f32(b)).val[1];
  float32x2_t c1c1 = vdup_lane_f32(vget_low_f32(c), 1);
  return vcombine_f32(a1b1, c1c1);
}
VECMATH_FINLINE vec4f VECTORCALL v_transpose3z(vec4f a, vec4f b, vec4f c)
{
  float32x2_t a2b2 = vtrn_f32(vget_high_f32(a), vget_high_f32(b)).val[0];
  float32x2_t c2c2 = vdup_lane_f32(vget_high_f32(c), 0);
  return vcombine_f32(a2b2, c2c2);
}
VECMATH_FINLINE vec4f VECTORCALL v_transpose3w(vec4f a, vec4f b, vec4f c)
{
  float32x2_t a3b3 = vtrn_f32(vget_high_f32(a), vget_high_f32(b)).val[1];
  float32x2_t c3c3 = vdup_lane_f32(vget_high_f32(c), 1);
  return vcombine_f32(a3b3, c3c3);
}

VECMATH_FINLINE vec4f VECTORCALL v_transpose4x(vec4f a, vec4f b, vec4f c, vec4f d)
{
  float32x2_t a0b0 = vtrn_f32(vget_low_f32(a), vget_low_f32(b)).val[0];
  float32x2_t c0d0 = vtrn_f32(vget_low_f32(c), vget_low_f32(d)).val[0];
  return vcombine_f32(a0b0, c0d0);
}
VECMATH_FINLINE vec4f VECTORCALL v_transpose4y(vec4f a, vec4f b, vec4f c, vec4f d)
{
  float32x2_t a1b1 = vtrn_f32(vget_low_f32(a), vget_low_f32(b)).val[1];
  float32x2_t c1d1 = vtrn_f32(vget_low_f32(c), vget_low_f32(d)).val[1];
  return vcombine_f32(a1b1, c1d1);
}
VECMATH_FINLINE vec4f VECTORCALL v_transpose4z(vec4f a, vec4f b, vec4f c, vec4f d)
{
  float32x2_t a2b2 = vtrn_f32(vget_high_f32(a), vget_high_f32(b)).val[0];
  float32x2_t c2d2 = vtrn_f32(vget_high_f32(c), vget_high_f32(d)).val[0];
  return vcombine_f32(a2b2, c2d2);
}
VECMATH_FINLINE vec4f VECTORCALL v_transpose4w(vec4f a, vec4f b, vec4f c, vec4f d)
{
  float32x2_t a3b3 = vtrn_f32(vget_high_f32(a), vget_high_f32(b)).val[1];
  float32x2_t c3d3 = vtrn_f32(vget_high_f32(c), vget_high_f32(d)).val[1];
  return vcombine_f32(a3b3, c3d3);
}

VECMATH_FINLINE vec4f VECTORCALL v_dot3_x(vec4f a, vec4f b)
{
  vec4f m;
  m = v_mul(a, b);
  m = v_madd(v_rot_1(a), v_rot_1(b), m);
  return v_madd(v_rot_2(a), v_rot_2(b), m);
}
VECMATH_FINLINE vec4f VECTORCALL v_dot3(vec4f a, vec4f b) { return v_splat_x(v_dot3_x(a, b)); }

VECMATH_FINLINE vec4f VECTORCALL v_dot4(vec4f a, vec4f b)
{
  vec4f m;
  m = v_mul(a, b);
  m = v_madd(v_rot_1(a), v_rot_1(b), m);
  return v_add(v_rot_2(m), m);
}
VECMATH_FINLINE vec4f VECTORCALL v_dot4_x(vec4f a, vec4f b) { return v_dot4(a,b); }

VECMATH_FINLINE vec4f VECTORCALL v_length4_sq(vec4f a)
{
  vec4f m = v_mul(a, a);
  vec4f ar1 = v_rot_1(a);
  m = v_madd(ar1, ar1, m);
  return v_add(v_rot_2(m), m);
}
VECMATH_FINLINE vec4f VECTORCALL v_length4_sq_x(vec4f a) { return v_length4_sq(a); }

VECMATH_FINLINE float32x2_t VECTORCALL v_length4_sq_xd(vec4f a)
{
  vec4f m = v_mul(a, a);
  float32x2_t m2 = vadd_f32(vget_low_f32(m), vget_high_f32(m));
  return vadd_f32(m2, vext_f32(m2, m2, 1));
}

VECMATH_FINLINE vec3f VECTORCALL v_length3_sq(vec3f a)
{
  vec4f m = v_mul(a, a);
  vec4f ar1 = v_rot_1(a), ar2 = v_rot_2(m);
  m = v_madd(ar1, ar1, m);
  return v_splat_x(v_add(ar2, m));
}
VECMATH_FINLINE vec3f VECTORCALL v_length3_sq_x(vec3f a)
{
  return v_length3_sq(a);
}
VECMATH_FINLINE float32x2_t VECTORCALL v_length3_sq_xd(vec3f a)
{
  vec4f m = v_mul(a, a);
  float32x2_t ar1 = vget_low_f32(v_rot_1(a)), ar2 = vget_high_f32(m);
  float32x2_t m2 = vmla_f32(vget_low_f32(m), ar1, ar1);
  return vadd_f32(ar2, m2);
}

VECMATH_FINLINE vec4f VECTORCALL v_norm4(vec4f a) { return v_mul(a, vdupq_lane_f32(v_rsqrt_x(v_length4_sq_xd(a)), 0)); }
VECMATH_FINLINE vec4f VECTORCALL v_norm3(vec4f a) { return v_mul(a, vdupq_lane_f32(v_rsqrt_x(v_length3_sq_xd(a)), 0)); }

VECMATH_FINLINE vec4f VECTORCALL v_distance3p_x(plane3f a, vec3f b) { return v_add_x(v_dot3_x(a,b), v_rot_3(a)); }
VECMATH_FINLINE vec4f VECTORCALL v_distance3p(plane3f a, vec3f b) { return v_splat_x(v_distance3p_x(a,b)); }

VECMATH_FINLINE vec4f VECTORCALL v_perm_yzxz(vec4f a) { return __builtin_shufflevector(a, a, 1, 2, 0, 2); }

VECMATH_FINLINE vec3f VECTORCALL v_cross3(vec3f a, vec3f b)
{
  vec4f tmp0, tmp1, tmp2, tmp3, result;
  tmp0 = v_perm_yzxz(a);
  tmp1 = v_perm_zxyz(b);
  tmp2 = v_perm_zxyz(a);
  tmp3 = v_perm_yzxz(b);
  result = v_mul(tmp0, tmp1);
  return v_nmsub(tmp2, tmp3, result);
}

VECMATH_FINLINE void VECTORCALL v_mat44_ident(mat44f &dest)
{
  dest.col3 = V_C_UNIT_0001;
  dest.col2 = v_rot_1(dest.col3);
  dest.col1 = v_rot_1(dest.col2);
  dest.col0 = v_rot_1(dest.col1);
}
VECMATH_FINLINE void VECTORCALL v_mat44_ident_swapxz(mat44f &dest)
{
  dest.col3 = V_C_UNIT_0001;
  dest.col0 = v_rot_1(dest.col3);
  dest.col1 = v_rot_1(dest.col0);
  dest.col2 = v_rot_1(dest.col1);
}
VECMATH_FINLINE void VECTORCALL v_mat33_ident(mat33f &dest)
{
  dest.col2 = V_C_UNIT_0010;
  dest.col1 = v_rot_1(dest.col2);
  dest.col0 = v_rot_1(dest.col1);
}
VECMATH_FINLINE void VECTORCALL v_mat33_ident_swapxz(mat33f &dest)
{
  dest.col0 = V_C_UNIT_0010;
  dest.col1 = v_rot_1(dest.col0);
  dest.col2 = v_rot_1(dest.col1);
}

VECMATH_FINLINE void VECTORCALL v_mat44_transpose(vec4f &r0, vec4f &r1, vec4f &r2, vec4f &r3)
{
  vec4f tmp0, tmp1, tmp2, tmp3;
  tmp0 = v_merge_hw(r0, r2);
  tmp1 = v_merge_hw(r1, r3);
  tmp2 = v_merge_lw(r0, r2);
  tmp3 = v_merge_lw(r1, r3);
  r0 = v_merge_hw(tmp0, tmp1);
  r1 = v_merge_lw(tmp0, tmp1);
  r2 = v_merge_hw(tmp2, tmp3);
  r3 = v_merge_lw(tmp2, tmp3);
}

VECMATH_FINLINE void VECTORCALL v_mat44_transpose(mat44f &dest, mat44f_cref src)
{
  vec4f tmp0, tmp1, tmp2, tmp3;
  tmp0 = v_merge_hw(src.col0, src.col2);
  tmp1 = v_merge_hw(src.col1, src.col3);
  tmp2 = v_merge_lw(src.col0, src.col2);
  tmp3 = v_merge_lw(src.col1, src.col3);
  dest.col0 = v_merge_hw(tmp0, tmp1);
  dest.col1 = v_merge_lw(tmp0, tmp1);
  dest.col2 = v_merge_hw(tmp2, tmp3);
  dest.col3 = v_merge_lw(tmp2, tmp3);
}

VECMATH_FINLINE void VECTORCALL v_mat43_transpose_to_mat44(mat44f &dest, mat43f_cref src)
{
  vec4f tmp0, tmp1, tmp2, tmp3;
  tmp0 = v_merge_hw(src.row0, src.row2);
  tmp1 = v_merge_hw(src.row1, v_zero());
  tmp2 = v_merge_lw(src.row0, src.row2);
  tmp3 = v_merge_lw(src.row1, v_zero());
  dest.col0 = v_merge_hw(tmp0, tmp1);
  dest.col1 = v_merge_lw(tmp0, tmp1);
  dest.col2 = v_merge_hw(tmp2, tmp3);
  dest.col3 = v_merge_lw(tmp2, tmp3);
}
VECMATH_FINLINE void VECTORCALL v_mat44_transpose_to_mat43(mat43f &dest, mat44f_cref src)
{
  vec4f tmp0, tmp1, tmp2, tmp3;
  tmp0 = v_merge_hw(src.col0, src.col2);
  tmp1 = v_merge_hw(src.col1, src.col3);
  tmp2 = v_merge_lw(src.col0, src.col2);
  tmp3 = v_merge_lw(src.col1, src.col3);
  dest.row0 = v_merge_hw(tmp0, tmp1);
  dest.row1 = v_merge_lw(tmp0, tmp1);
  dest.row2 = v_merge_hw(tmp2, tmp3);
}

VECMATH_FINLINE vec4f VECTORCALL v_mat44_mul_vec4(mat44f_cref m, vec4f v)
{
  vec4f xxxx = v_splat_x(v);
  vec4f yyyy = v_splat_y(v);
  vec4f zzzz = v_splat_z(v);
  vec4f wwww = v_splat_w(v);
  vec4f tmp0, tmp1;

  tmp0 = v_mul(m.col0, xxxx);
  tmp1 = v_mul(m.col1, yyyy);
  tmp0 = v_madd(m.col2, zzzz, tmp0);
  tmp1 = v_madd(m.col3, wwww, tmp1);
  return v_add(tmp0, tmp1);
}

VECMATH_FINLINE vec4f VECTORCALL v_mat44_mul_vec3v(mat44f_cref m, vec4f v)
{
  vec4f xxxx = v_splat_x(v);
  vec4f yyyy = v_splat_y(v);
  vec4f zzzz = v_splat_z(v);
  vec4f res;

  res = v_mul(m.col0, xxxx);
  res = v_madd(m.col1, yyyy, res);
  return v_madd(m.col2, zzzz, res);
}

VECMATH_FINLINE vec4f VECTORCALL v_mat44_mul_vec3p(mat44f_cref m, vec4f v)
{
  vec4f xxxx = v_splat_x(v);
  vec4f yyyy = v_splat_y(v);
  vec4f zzzz = v_splat_z(v);
  vec4f tmp0, tmp1;

  tmp0 = v_mul(m.col0, xxxx);
  tmp1 = v_mul(m.col1, yyyy);
  tmp0 = v_madd(m.col2, zzzz, tmp0);
  tmp1 = v_add(m.col3, tmp1);
  return v_add(tmp0, tmp1);
}

VECMATH_FINLINE vec3f VECTORCALL v_mat33_mul_vec3(mat33f_cref m, vec3f v)
{
  vec4f xxxx = v_splat_x(v);
  vec4f yyyy = v_splat_y(v);
  vec4f zzzz = v_splat_z(v);
  vec4f res;

  res = v_mul(m.col0, xxxx);
  res = v_madd(m.col1, yyyy, res);
  return v_madd(m.col2, zzzz, res);
}

VECMATH_FINLINE void VECTORCALL v_mat44_inverse(mat44f &dest, mat44f_cref m)
{
  vec4f in0 = m.col0; // AEIM
  vec4f in1 = m.col1; // BFJN
  vec4f in2 = m.col2; // CGKO
  vec4f in3 = m.col3; // DHLP

  vec4f inA0 = v_merge_hw(in0, in1); // ABEF
  vec4f inA1 = v_merge_hw(in2, in3); // CDGH
  vec4f inA2 = v_merge_lw(in0, in1); // IJMN
  vec4f inA3 = v_merge_lw(in2, in3); // KLOP

  // XYZW, ABCD
  vec4f KKII = v_perm_xxaa(inA3, inA2); // KLOP, IJMN
  vec4f KIIK = v_rot_1(KKII);
  vec4f PPNN = v_perm_wwdd(in3, in1);   // DHLP, BFJN
  vec4f NPPN = v_rot_3(PPNN);
  vec4f OOMM = v_perm_wwdd(in2, in0);   // CGKO, AEIM
  vec4f OMMO = v_rot_1(OOMM);
  vec4f JILK = v_perm_yxba(inA2, inA3); // IJMN, KLOP
  vec4f LKJI = v_rot_2(JILK);
  vec4f PONM = v_perm_wzdc(inA3, inA2); // KLOP, IJMN
  vec4f NMPO = v_rot_2(PONM);
  vec4f LLJJ = v_perm_yybb(inA3, inA2); // KLOP, IJMN
  vec4f JLLJ = v_rot_3(LLJJ);
  vec4f CCAA = v_perm_xxaa(in2, in0);   // CGKO, AEIM
  vec4f CAAC = v_rot_1(CCAA);
  vec4f GGEE = v_perm_yybb(in2, in0);   // CGKO, AEIM
  vec4f GEEG = v_rot_1(GGEE);
  vec4f BADC = v_perm_yxba(inA0, inA1); // ABEF, CDGH
  vec4f DCBA = v_perm_yxba(inA1, inA0); // CDGH, ABEF
  vec4f HGFE = v_perm_wzdc(inA1, inA0); // CDGH, ABEF
  vec4f FEHG = v_perm_wzdc(inA0, inA1); // ABEF, CDGH
  vec4f HHFF = v_perm_wwdd(inA1, inA0); // CDGH, ABEF
  vec4f FHHF = v_rot_3(HHFF);
  vec4f DDBB = v_perm_xxaa(in3, in1);   // DHLP, BFJN
  vec4f BDDB = v_rot_3(DDBB);

  vec4f KP_IP_IN_KN = v_mul(KIIK, PPNN);
  vec4f JP_IO_LN_KM = v_mul(JILK, PONM);
  vec4f KN_KP_IP_IN = v_mul(KKII, NPPN);
  vec4f CH_AH_AF_CF = v_mul(CAAC, HHFF);
  vec4f BH_AG_DF_CE = v_mul(BADC, HGFE);
  vec4f CF_CH_AH_AF = v_mul(CCAA, FHHF);

  vec4f GHEF = v_perm_zwcd(inA1, inA0); // CDGH, ABEF
  vec4f FGHE = v_rot_3(GHEF);
  vec4f HEFG = v_rot_1(GHEF);
  vec4f CDAB = v_perm_xyab(inA1, inA0); // CDGH, ABEF
  vec4f BCDA = v_rot_3(CDAB);
  vec4f DABC = v_rot_1(CDAB);
  vec4f OPMN = v_perm_zwcd(inA3, inA2); // KLOP, IJMN
  vec4f NOPM = v_rot_3(OPMN);
  vec4f PMNO = v_rot_1(OPMN);

  vec4f KP_LO_IP_LM_IN_JM_KN_JO = v_nmsub(LLJJ, OMMO, KP_IP_IN_KN);
  vec4f JP_LN_IO_KM_LN_JP_KM_IO = v_nmsub(LKJI, NMPO, JP_IO_LN_KM);
  vec4f KN_JO_KP_LO_IP_LM_IN_JM = v_nmsub(JLLJ, OOMM, KN_KP_IP_IN);
  vec4f CH_DG_AH_DE_AF_BE_CF_BG = v_nmsub(DDBB, GEEG, CH_AH_AF_CF);
  vec4f BH_DF_AG_CE_DF_BH_CE_AG = v_nmsub(DCBA, FEHG, BH_AG_DF_CE);
  vec4f CF_BG_CH_DG_AH_DE_AF_BE = v_nmsub(BDDB, GGEE, CF_CH_AH_AF);

  vec4f tmpA1 = v_mul(FGHE, KP_LO_IP_LM_IN_JM_KN_JO);
  vec4f tmpB1 = v_nmsub(GHEF, JP_LN_IO_KM_LN_JP_KM_IO, tmpA1);
  vec4f out1 = v_nmsub(HEFG, KN_JO_KP_LO_IP_LM_IN_JM, tmpB1);
  vec4f tmpB2 = v_mul(CDAB, JP_LN_IO_KM_LN_JP_KM_IO);
  vec4f tmpA2 = v_nmsub(BCDA, KP_LO_IP_LM_IN_JM_KN_JO, tmpB2);
  vec4f out2 = v_madd(DABC, KN_JO_KP_LO_IP_LM_IN_JM, tmpA2);
  vec4f tmpA3 = v_mul(NOPM, CH_DG_AH_DE_AF_BE_CF_BG);
  vec4f tmpB3 = v_nmsub(OPMN, BH_DF_AG_CE_DF_BH_CE_AG, tmpA3);
  vec4f out3 = v_nmsub(PMNO, CF_BG_CH_DG_AH_DE_AF_BE, tmpB3);

  //det
  vec4f AF_BE_AG_CE_CF_BG_BH_DF = v_perm_zbwa(CH_DG_AH_DE_AF_BE_CF_BG, BH_DF_AG_CE_DF_BH_CE_AG);
  vec4f KP_LO_JP_LN_IP_LM_IO_KM = v_merge_hw(KP_LO_IP_LM_IN_JM_KN_JO, JP_LN_IO_KM_LN_JP_KM_IO);
  vec4f CH_DG_AH_DE_BH_DF_AG_CE = v_perm_xyab(CH_DG_AH_DE_AF_BE_CF_BG, BH_DF_AG_CE_DF_BH_CE_AG);
  vec4f IN_JM_KN_JO_IO_KM_JP_LN = v_perm_zwba(KP_LO_IP_LM_IN_JM_KN_JO, JP_LN_IO_KM_LN_JP_KM_IO);
  vec4f det0 = v_mul(AF_BE_AG_CE_CF_BG_BH_DF, KP_LO_JP_LN_IP_LM_IO_KM);
  vec4f det1 = v_madd(CH_DG_AH_DE_BH_DF_AG_CE, IN_JM_KN_JO_IO_KM_JP_LN, det0);
  vec4f detA0 = v_splat_x(det1);
  vec4f detA1 = v_splat_y(det1);
  vec4f detA2 = v_splat_z(det1);
  vec4f detB0 = v_sub(detA0, detA1);
  vec4f det = v_sub(detB0, detA2);
  vec4f invdet = v_rcp(det);
  /* Multiply the cofactors by the reciprocal of the determinant.
  */
  dest.col0 = v_mul(out1, invdet);
  dest.col1 = v_mul(out1, invdet);
  dest.col2 = v_mul(out2, invdet);
  dest.col3 = v_mul(out3, invdet);
}
VECMATH_FINLINE void VECTORCALL v_mat33_inverse(mat33f &dest, mat33f_cref m)
{
  vec4f tmp0, tmp1, tmp2, dot, invdet, inv0, inv1, inv2;
    tmp2 = v_cross3(m.col0, m.col1);
  tmp0 = v_cross3(m.col1, m.col2);
  tmp1 = v_cross3(m.col2, m.col0);
  dot = v_dot3(tmp2, m.col2);
  invdet = v_rcp(dot);
  inv0 = v_transpose3x(tmp0, tmp1, tmp2);
  inv1 = v_transpose3y(tmp0, tmp1, tmp2);
  inv2 = v_transpose3z(tmp0, tmp1, tmp2);
  dest.col0 = v_mul(inv0, invdet);
  dest.col1 = v_mul(inv1, invdet);
  dest.col2 = v_mul(inv2, invdet);
}
VECMATH_FINLINE vec4f VECTORCALL v_mat44_det(mat44f_cref m)
{
  vec4f tmp0, tmp1, tmp2, tmp3;
  vec4f cof0;
  vec4f t0, t1, t2, t3;
  vec4f t12, t23;
  vec4f t1r, t2r;
  vec4f t12r, t23r;
  vec4f t1r3, t1r3r;

  tmp0 = v_perm_xzac(m.col0, m.col1);
  tmp1 = v_perm_xzac(m.col2, m.col3);
  tmp2 = v_perm_ywbd(m.col0, m.col1);
  tmp3 = v_perm_ywbd(m.col2, m.col3);
  t0 = v_perm_xzac(tmp0, tmp1);
  t1 = v_perm_xzac(tmp3, tmp2);
  t2 = v_perm_ywbd(tmp0, tmp1);
  t3 = v_perm_ywbd(tmp3, tmp2);

  t23 = v_mul(t2, t3);
  t23 = v_perm_yxwz(t23);
  cof0 = v_mul(t1, t23);
  t23r = v_rot_2(t23);
  cof0 = v_neg(v_nmsub(t1, t23r, cof0));
  t12 = v_mul(t1, t2);
  t12 = v_perm_yxwz(t12);
  cof0 = v_madd(t3, t12, cof0);
  t12r = v_rot_2(t12);
  cof0 = v_nmsub(t3, t12r, cof0);
  t1r = v_rot_2(t1);
  t2r = v_rot_2(t2);
  t1r3 = v_mul(t1r, t3);
  t1r3 = v_perm_yxwz(t1r3);
  cof0 = v_madd(t2r, t1r3, cof0);
  t1r3r = v_rot_2(t1r3);
  cof0 = v_nmsub(t2r, t1r3r, cof0);
  return v_dot4(t0, cof0);
}

VECMATH_FINLINE vec3f VECTORCALL v_bbox3_center(bbox3f_cref b)
{
  return v_mul(v_add(b.bmin, b.bmax), V_C_HALF);
}
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_outer_rad(vec3f bmin, vec3f bmax)
{
  return v_mul(V_C_HALF, v_length3(v_sub(bmax, bmin)));
}
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_inner_rad(vec3f bmin, vec3f bmax)
{
  vec4f s = v_sub(bmax, bmin);
  return v_mul(V_C_HALF, v_min(s, v_min(v_rot_1(s), v_rot_2(s))));
}

VECMATH_FINLINE int VECTORCALL v_check_xz_non_zero(vec4f a)
{
  const uint32x2_t xy = vget_low_u32((uint32x4_t)a);
  const uint32x2_t zw = vget_high_u32((uint32x4_t)a);
  uint32x2_t m_and = vand_u32(xy, zw);
  return vget_lane_u32(m_and, 0);
}

VECMATH_FINLINE int VECTORCALL v_bbox3_test_pt_inside_b_xz(bbox3f_cref b, vec3f p)
{
  vec3f m = v_and(v_cmp_ge(p, b.bmin), v_cmp_ge(b.bmax, p));
  return v_check_xz_non_zero(m);
}
VECMATH_FINLINE int VECTORCALL v_bbox3_test_pt_inside_b(bbox3f_cref b, vec3f p)
{
  vec3f m = v_and(v_cmp_ge(p, b.bmin), v_cmp_ge(b.bmax, p));
  return v_check_xyz_non_zero(m);
}
VECMATH_FINLINE int VECTORCALL v_bbox3_test_box_inside_b(bbox3f_cref b, bbox3f_cref b2)
{
  vec3f m1 = v_and(v_cmp_ge(b2.bmin, b.bmin), v_cmp_ge(b.bmax, b2.bmin));
  vec3f m2 = v_and(v_cmp_ge(b2.bmax, b.bmin), v_cmp_ge(b.bmax, b2.bmax));
  vec3f m = v_and(m1, m2);
  return v_check_xyz_non_zero(m);
}
VECMATH_FINLINE int VECTORCALL v_bbox3_test_box_intersect_b(bbox3f_cref b1, bbox3f_cref b2)
{
  vec3f m = v_and(v_cmp_ge(b2.bmax, b1.bmin), v_cmp_ge(b1.bmax, b2.bmin));
  return v_check_xyz_non_zero(m);
}
VECMATH_FINLINE int VECTORCALL v_bbox3_test_box_intersect_b_safe(bbox3f_cref b1, bbox3f_cref b2)
{
  vec3f m = v_and(v_and(v_cmp_ge(b1.bmax, b1.bmin), v_cmp_ge(b2.bmax, b2.bmin)),
                  v_and(v_cmp_ge(b2.bmax, b1.bmin), v_cmp_ge(b1.bmax, b2.bmin)));
  return v_check_xyz_non_zero(m);
}

VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt001(bbox3f_cref b){ return v_perm_xycc(b.bmin, b.bmax); }
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt010(bbox3f_cref b){ return v_perm_xbzz(b.bmin, b.bmax); }
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt011(bbox3f_cref b){ return v_perm_xbcc(b.bmin, b.bmax); }
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt100(bbox3f_cref b){ return v_perm_xbcc(b.bmax, b.bmin); }
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt101(bbox3f_cref b){ return v_perm_xbzz(b.bmax, b.bmin); }
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt110(bbox3f_cref b){ return v_perm_xycc(b.bmax, b.bmin); }

VECMATH_FINLINE vec4f VECTORCALL v_insert(float s, vec4f v, int i)
{
  if (i == 0)
    return v_sel(v, v_splats(s), (vec4f)V_CI_MASK0001);
  if (i == 1)
    return v_sel(v, v_splats(s), (vec4f)vextq_s32(V_CI_MASK0001, V_CI_MASK0001, 1));
  if (i == 2)
    return v_sel(v, v_splats(s), (vec4f)vextq_s32(V_CI_MASK0001, V_CI_MASK0001, 2));
  if (i == 3)
    return v_sel(v, v_splats(s), (vec4f)vextq_s32(V_CI_MASK0001, V_CI_MASK0001, 3));
  return v;
}

VECMATH_FINLINE vec4f VECTORCALL v_promote(float s, int /*i*/)
{
  return vmovq_n_f32(s);
}

VECMATH_FINLINE quat4f VECTORCALL v_un_quat_from_mat(vec3f col0, vec3f col1, vec3f col2)
{
  /* compute quaternion for each case */
  const vec4f xx_yy = v_perm_xbzw(col0, col1);
  const vec4f xx_yy_zz_xx = v_perm_xycx(xx_yy, col2);
  const vec4f yy_zz_xx_yy = v_perm_ycxy(xx_yy, col2);
  const vec4f zz_xx_yy_zz = v_perm_cxyc(xx_yy, col2);

  const vec4f diagSum = v_add(v_add(xx_yy_zz_xx, yy_zz_xx_yy), zz_xx_yy_zz);
  const vec4f diagDiff = v_sub(v_sub(xx_yy_zz_xx, yy_zz_xx_yy), zz_xx_yy_zz);
  const vec4f radicand = v_add(v_perm_xyzd(diagDiff, diagSum), V_C_ONE);
  const vec4f invSqrt = v_rsqrt4(v_sel(radicand, V_C_ONE, v_cmp_ge(v_zero(), radicand)));

  const vec4f zy_xz_yx = v_perm_zayx(v_perm_xycw(col0, col1), col2);
  const vec4f yz_zx_xy = v_perm_bzxx(v_perm_ayzw(col0, col1), col2);

  const vec4f sum = v_add(zy_xz_yx, yz_zx_xy);
  const vec4f diff = v_sub(zy_xz_yx, yz_zx_xy);

  const vec4f scale = v_mul(invSqrt, V_C_HALF);
  const vec4f res0 = v_mul(v_perm_ayzw(v_perm_xzya(sum, diff), radicand), v_splat_x(scale));
  const vec4f res1 = v_mul(v_perm_xbzw(v_perm_zxxb(sum, diff), radicand), v_splat_y(scale));
  const vec4f res2 = v_mul(v_perm_xycw(v_perm_yxac(sum, diff), radicand), v_splat_z(scale));
  const vec4f res3 = v_mul(v_perm_xyzd(diff, radicand), v_splat_w(scale));

  /* determine case and select answer */
  vec4f xx = v_splat_x(col0);
  vec4f yy = v_splat_y(col1);
  vec4f zz = v_splat_z(col2);

  vec4f q = v_sel(res0, res1, v_cmp_gt(yy, xx));
  q = v_sel(q, res2, v_and(v_cmp_gt(zz, xx), v_cmp_gt(zz, yy)));
  return v_sel(q, res3, v_cmp_gt(v_splat_x(diagSum), v_zero()));
}

VECMATH_FINLINE short VECTORCALL v_extract_xi16(vec4i v) { return vgetq_lane_s16(vreinterpretq_s16_s32(v), 0); }

VECMATH_FINLINE float VECTORCALL v_extract(vec4f v, int i)
{
  alignas(16) float a[4];
  v_st(a, v);
  return a[i];
}

VECMATH_FINLINE int VECTORCALL v_extracti(vec4i v, int i)
{
  alignas(16) int a[4];
  v_sti(a, v);
  return a[i];
}

VECMATH_FINLINE float VECTORCALL v_extract_x(vec4f v) { return vgetq_lane_f32(v, 0); }
VECMATH_FINLINE float VECTORCALL v_extract_y(vec4f v) { return vgetq_lane_f32(v, 1); }
VECMATH_FINLINE float VECTORCALL v_extract_z(vec4f v) { return vgetq_lane_f32(v, 2); }
VECMATH_FINLINE float VECTORCALL v_extract_w(vec4f v) { return vgetq_lane_f32(v, 3); }

VECMATH_FINLINE int VECTORCALL v_extract_xi(vec4i v) {return vgetq_lane_s32(v, 0);}
VECMATH_FINLINE int VECTORCALL v_extract_yi(vec4i v) {return vgetq_lane_s32(v, 1);}
VECMATH_FINLINE int VECTORCALL v_extract_zi(vec4i v) {return vgetq_lane_s32(v, 2);}
VECMATH_FINLINE int VECTORCALL v_extract_wi(vec4i v) {return vgetq_lane_s32(v, 3);}

VECMATH_FINLINE int64_t VECTORCALL v_extract_xi64(vec4i v) {return vgetq_lane_s64(vreinterpretq_s64_s32(v), 0);}

#define V_TEST_VEC_X_BIT0(v) (vget_lane_u32(v, 0) & 1)

VECMATH_FINLINE int VECTORCALL v_test_vec_x_eq(vec3f v, vec3f a) { return V_TEST_VEC_X_BIT0(vceq_f32(vget_low_f32(v), vget_low_f32(a))); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_gt(vec3f v, vec3f a) { return V_TEST_VEC_X_BIT0(vcgt_f32(vget_low_f32(v), vget_low_f32(a))); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_ge(vec3f v, vec3f a) { return V_TEST_VEC_X_BIT0(vcge_f32(vget_low_f32(v), vget_low_f32(a))); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_lt(vec3f v, vec3f a) { return V_TEST_VEC_X_BIT0(vclt_f32(vget_low_f32(v), vget_low_f32(a))); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_le(vec3f v, vec3f a) { return V_TEST_VEC_X_BIT0(vcle_f32(vget_low_f32(v), vget_low_f32(a))); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_eq_0(vec3f v) { return V_TEST_VEC_X_BIT0(vceq_s32(vget_low_s32((vec4i)v), vmov_n_s32(0))); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_gt_0(vec3f v) { return V_TEST_VEC_X_BIT0(vcgt_f32(vget_low_f32(v), vmov_n_f32(0))); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_ge_0(vec3f v) { return V_TEST_VEC_X_BIT0(vcge_f32(vget_low_f32(v), vmov_n_f32(0))); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_lt_0(vec3f v) { return V_TEST_VEC_X_BIT0(vclt_f32(vget_low_f32(v), vmov_n_f32(0))); }
VECMATH_FINLINE int VECTORCALL v_test_vec_x_le_0(vec3f v) { return V_TEST_VEC_X_BIT0(vcle_f32(vget_low_f32(v), vmov_n_f32(0))); }

#undef V_TEST_VEC_X_BIT0

VECMATH_FINLINE vec4i VECTORCALL v_ldui_half(const void *m)
{
  return vcombine_s32(vld1_s32((int32_t const*)m), vcreate_s32(0));
}
VECMATH_FINLINE vec4f VECTORCALL v_ldu_half(const void *m)
{
  return vreinterpretq_f32_s32(v_ldui_half(m));
}
VECMATH_FINLINE void v_prefetch(const void *m)
{
  __builtin_prefetch(m);
}

VECMATH_FINLINE vec4i VECTORCALL v_cvt_ush_vec4i(vec4i a) { return (int32x4_t)vmovl_u16(vget_low_u16(vreinterpretq_u16_s32(a))); }
VECMATH_FINLINE vec4i VECTORCALL v_cvt_ssh_vec4i(vec4i a) { return vmovl_s16(vget_low_s16(vreinterpretq_s16_s32(a))); }

VECMATH_FINLINE vec4i v_cvt_byte_vec4i(vec4i a)
{
  int8x8_t a1 = vreinterpret_s8_s16(vget_low_s16(vreinterpretq_s16_s32(a)));
  int8x8_t b1 = vdup_n_s8(0);
  int8x8x2_t result = vzip_s8(a1, b1);
  return vreinterpretq_s32_s8(vcombine_s8(result.val[0], result.val[1]));
}

#if __APPLE__ || defined(__clang__) // fix error 'bits is not imm'
#define v_slli(v, bits) (int32x4_t)vshlq_n_u32((uint32x4_t)(v), bits)
#define v_srli(v, bits) (int32x4_t)vshrq_n_u32((uint32x4_t)(v), bits)
#define v_srai(v, bits) vshrq_n_s32(v, bits)

#else
VECMATH_FINLINE vec4i VECTORCALL v_slli(vec4i v, int bits) { return (int32x4_t)vshlq_n_u32((uint32x4_t)v, bits); }
VECMATH_FINLINE vec4i VECTORCALL v_srli(vec4i v, int bits) { return (int32x4_t)vshrq_n_u32((uint32x4_t)v, bits); }
VECMATH_FINLINE vec4i VECTORCALL v_srai(vec4i v, int bits) { return vshrq_n_s32(v, bits); }
#endif
VECMATH_FINLINE vec4i VECTORCALL v_slli_n(vec4i v, int bits)
{
  if (bits & ~31)
    return v_zero();
  return vreinterpretq_s64_s32(vshlq_s32(vreinterpretq_s32_s64(v), vdupq_n_s32(bits)));
}
VECMATH_FINLINE vec4i VECTORCALL v_srli_n(vec4i v, int bits)
{
  if (bits & ~31)
    return v_zero();
  return vreinterpretq_s64_s32(vshlq_u32(vreinterpretq_s32_s64(v), vdupq_n_s32(-bits)));
}
VECMATH_FINLINE vec4i VECTORCALL v_srai_n(vec4i v, int bits)
{
  if (bits & ~31)
    return v_cmp_lti(v, v_zeroi());
  return vreinterpretq_s64_s32(vshlq_s32(vreinterpretq_s32_s64(v), vdupq_n_s32(-bits)));
}
VECMATH_FINLINE vec4i VECTORCALL v_slli_n(vec4i v, vec4i bits)
{
  int64_t c = (int64_t)vget_low_s64(bits);
  if (c & ~31)
    return v_zero();
  return vreinterpretq_s64_s32(vshlq_s32(vreinterpretq_s32_s64(v), vdupq_n_s32(c)));
}
VECMATH_FINLINE vec4i VECTORCALL v_srli_n(vec4i v, vec4i bits)
{
  int64_t c = (int64_t)vget_low_s64(bits);
  if (c & ~31)
    return v_zero();
  return vreinterpretq_s64_s32(vshlq_u32(vreinterpretq_s32_s64(v), vdupq_n_s32(-c)));
}
VECMATH_FINLINE vec4i VECTORCALL v_srai_n(vec4i v, vec4i bits)
{
  int64_t c = (int64_t)vget_low_s64(bits);
  if (c & ~31)
    return v_cmp_lti(v, v_zeroi());
  return vreinterpretq_s64_s32(vshlq_s32(vreinterpretq_s32_s64(v), vdupq_n_s32(-c)));
}

VECMATH_FINLINE vec4i VECTORCALL v_sll(vec4i v, int bits) { return (int32x4_t)vshlq_u32((uint32x4_t)v, (int32x4_t)v_splatsi(bits)); }
VECMATH_FINLINE vec4i VECTORCALL v_srl(vec4i v, int bits) { return (int32x4_t)vshlq_u32((uint32x4_t)v, (int32x4_t)v_splatsi(-bits)); }
VECMATH_FINLINE vec4i VECTORCALL v_sra(vec4i v, int bits) { return vshlq_s32(v, (int32x4_t)v_splatsi(-bits)); }

VECMATH_FINLINE vec4i VECTORCALL v_ori(vec4i a, vec4i b) { return vorrq_s32(a, b); }
VECMATH_FINLINE vec4i VECTORCALL v_andi(vec4i a, vec4i b) { return vandq_s32(a, b); }
VECMATH_FINLINE vec4i VECTORCALL v_andnoti(vec4i a, vec4i b) { return vandq_s32(vmvnq_s32(a), b); }
VECMATH_FINLINE vec4i VECTORCALL v_xori(vec4i a, vec4i b) { return veorq_s32(a, b); }

VECMATH_FINLINE vec4i VECTORCALL v_packs(vec4i a, vec4i b) { return vreinterpretq_s32_s16(vcombine_s16(vqmovn_s32(a), vqmovn_s32(b))); }
VECMATH_FINLINE vec4i VECTORCALL v_packs(vec4i a) { int16x4_t w = vqmovn_s32(a); return vreinterpretq_s32_s16(vcombine_s16(w, w)); }

VECMATH_FINLINE vec4i VECTORCALL v_packus(vec4i a, vec4i b)
{
  return vreinterpretq_s32_u16(vcombine_u16(vqmovun_s32(a), vqmovun_s32(b)));
}
VECMATH_FINLINE vec4i VECTORCALL v_packus(vec4i a)
{
  uint16x4_t w = vqmovun_s32(a);
  return vreinterpretq_s32_u16(vcombine_u16(w, w));
}
VECMATH_FINLINE vec4i VECTORCALL v_packus16(vec4i a, vec4i b)
{
  return vreinterpretq_s32_u8(
    vcombine_u8(vqmovun_s16(vreinterpretq_s16_s32(a)),
      vqmovun_s16(vreinterpretq_s16_s32(b))));
}
VECMATH_FINLINE vec4i VECTORCALL v_packus16(vec4i a)
{
  uint8x8_t t = vqmovun_s16(vreinterpretq_s16_s32(a));
  return vreinterpretq_s32_u8(vcombine_u8(t,t));
}

#endif
