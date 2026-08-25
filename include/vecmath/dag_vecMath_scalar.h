//
// Dagor Engine 6.5 - 1st party libs
// Copyright (C) Gaijin Games KFT.  All rights reserved.
//
#pragma once

// Scalar per-lane backend: implements the same primitive contract as dag_vecMath_pc_sse.h /
// dag_vecMath_neon.h for targets with no SIMD ISA (Cortex-M, RISC-V without V, forced via
// _TARGET_SIMD_SCALAR=1 for testing). Semantics follow the SSE backend: compare results are
// per-lane all-bits/zero masks, v_sel/v_seli and the v_check_*/v_signmask family read only
// the lane sign bit, v_min/v_max return the second operand on unordered, float->int
// conversions mirror cvttps/cvtps out-of-range behavior (INT_MIN), and the Cramer's-rule
// matrix inverse/determinant are transcribed from the SSE backend op for op so rounding matches.

#include <stdint.h>
#include <string.h>
#include <math.h>

VECMATH_FINLINE float scalar_u2f(uint32_t u) { float r; memcpy(&r, &u, 4); return r; }
VECMATH_FINLINE uint32_t scalar_f2u(float f) { uint32_t r; memcpy(&r, &f, 4); return r; }
VECMATH_FINLINE float scalar_fmask(bool c) { return scalar_u2f(c ? 0xFFFFFFFFu : 0u); }
VECMATH_FINLINE int scalar_sign(float f) { return int(scalar_f2u(f) >> 31); }

VECMATH_FINLINE int scalar_trunc_to_int(float f)
{
  if (!(f >= -2147483648.0f && f < 2147483648.0f))
    return INT32_MIN;
  return (int)f;
}
VECMATH_FINLINE int scalar_round_to_int(float f)
{
  float r = nearbyintf(f);
  if (!(r >= -2147483648.0f && r < 2147483648.0f))
    return INT32_MIN;
  return (int)r;
}

VECMATH_FINLINE vec4f scalar_perm(vec4f a, int i0, int i1, int i2, int i3)
{
  vec4f r; r.f[0] = a.f[i0]; r.f[1] = a.f[i1]; r.f[2] = a.f[i2]; r.f[3] = a.f[i3]; return r;
}
VECMATH_FINLINE vec4f scalar_shuffle(vec4f a, vec4f b, int i0, int i1, int i2, int i3)
{
  vec4f r; r.f[0] = a.f[i0]; r.f[1] = a.f[i1]; r.f[2] = b.f[i2]; r.f[3] = b.f[i3]; return r;
}

VECMATH_FINLINE vec4f v_zero() { vec4f r = {0.f, 0.f, 0.f, 0.f}; return r; }
VECMATH_FINLINE vec4i v_zeroi() { vec4i r = {0, 0, 0, 0}; return r; }
VECMATH_FINLINE vec4i v_set_all_bitsi() { vec4i r = {-1, -1, -1, -1}; return r; }
VECMATH_FINLINE vec4f v_set_all_bits() { return v_cast_vec4f(v_set_all_bitsi()); }
VECMATH_FINLINE vec4f v_msbit() { return (const vec4f &)V_CI_SIGN_MASK; }
VECMATH_FINLINE vec4f v_splats(float a) { vec4f r = {a, a, a, a}; return r; }
VECMATH_FINLINE vec4i v_splatsi(int a) { vec4i r = {a, a, a, a}; return r; }
VECMATH_FINLINE vec4f v_set_x(float a) { vec4f r = {a, 0.f, 0.f, 0.f}; return r; }
VECMATH_FINLINE vec4i v_seti_x(int a) { vec4i r = {a, 0, 0, 0}; return r; }

NO_ASAN_INLINE vec4f v_ld(const float *m) { vec4f r; memcpy(&r, m, 16); return r; }
NO_ASAN_INLINE vec4f v_ldu(const float *m) { vec4f r; memcpy(&r, m, 16); return r; }
NO_ASAN_INLINE vec4i v_ldi(const int *m) { vec4i r; memcpy(&r, m, 16); return r; }
NO_ASAN_INLINE vec4i v_ldui(const int *m) { vec4i r; memcpy(&r, m, 16); return r; }
NO_ASAN_INLINE vec4f v_ldu_x(const float *m) { vec4f r = {*m, 0.f, 0.f, 0.f}; return r; }

VECMATH_FINLINE vec4i v_ldui_half(const void *m) { vec4i r = {0, 0, 0, 0}; memcpy(&r, m, 8); return r; }
VECMATH_FINLINE vec4f v_ldu_half(const void *m) { return v_cast_vec4f(v_ldui_half(m)); }
VECMATH_FINLINE void v_prefetch(const void *m) { (void)m; }

NO_ASAN_INLINE vec3f v_ldu_p3_safe(const float *m) { vec4f r = {m[0], m[1], m[2], 0.f}; return r; }
NO_ASAN_INLINE vec4i v_ldui_p3_safe(const int *m) { vec4i r = {m[0], m[1], m[2], 0}; return r; }

VECMATH_FINLINE vec4i v_ldush(const signed short *m)
{
  vec4i r; r.i[0] = m[0]; r.i[1] = m[1]; r.i[2] = m[2]; r.i[3] = m[3]; return r;
}
VECMATH_FINLINE vec4i v_lduush(const unsigned short *m)
{
  vec4i r; r.i[0] = m[0]; r.i[1] = m[1]; r.i[2] = m[2]; r.i[3] = m[3]; return r;
}

VECMATH_FINLINE vec4i v_cvt_lo_ush_vec4i(vec4i a)
{
  uint16_t h[8]; memcpy(h, &a, 16);
  vec4i r; r.i[0] = h[0]; r.i[1] = h[1]; r.i[2] = h[2]; r.i[3] = h[3]; return r;
}
VECMATH_FINLINE vec4i v_cvt_hi_ush_vec4i(vec4i a)
{
  uint16_t h[8]; memcpy(h, &a, 16);
  vec4i r; r.i[0] = h[4]; r.i[1] = h[5]; r.i[2] = h[6]; r.i[3] = h[7]; return r;
}
VECMATH_FINLINE vec4i v_cvt_lo_ssh_vec4i(vec4i a)
{
  int16_t h[8]; memcpy(h, &a, 16);
  vec4i r; r.i[0] = h[0]; r.i[1] = h[1]; r.i[2] = h[2]; r.i[3] = h[3]; return r;
}
VECMATH_FINLINE vec4i v_cvt_hi_ssh_vec4i(vec4i a)
{
  int16_t h[8]; memcpy(h, &a, 16);
  vec4i r; r.i[0] = h[4]; r.i[1] = h[5]; r.i[2] = h[6]; r.i[3] = h[7]; return r;
}

VECMATH_FINLINE vec4i v_cvt_byte_vec4i(uint32_t a)
{
  vec4i r; r.i[0] = a & 0xFF; r.i[1] = (a >> 8) & 0xFF; r.i[2] = (a >> 16) & 0xFF; r.i[3] = (a >> 24) & 0xFF; return r;
}

VECMATH_FINLINE vec4f v_make_vec4f(float x, float y, float z, float w) { vec4f r = {x, y, z, w}; return r; }
VECMATH_FINLINE vec4i v_make_vec4i(int x, int y, int z, int w) { vec4i r = {x, y, z, w}; return r; }
VECMATH_FINLINE vec4f v_make_vec3f(float x, float y, float z) { vec4f r = {x, y, z, z}; return r; }
VECMATH_FINLINE vec4i v_make_vec3i(int x, int y, int z) { vec4i r = {x, y, z, z}; return r; }
VECMATH_FINLINE vec4f v_make_vec3f(vec4f x, vec4f y, vec4f z) { vec4f r = {x.f[0], y.f[0], z.f[0], z.f[0]}; return r; }

VECMATH_FINLINE vec4f v_perm_xxzz(vec4f b) { return scalar_perm(b, 0, 0, 2, 2); }
VECMATH_FINLINE vec4f v_perm_xyxy(vec4f b) { return scalar_perm(b, 0, 1, 0, 1); }
VECMATH_FINLINE vec4f v_perm_wwyy(vec4f b) { return scalar_perm(b, 3, 3, 1, 1); }
VECMATH_FINLINE vec4f v_perm_yyww(vec4f b) { return scalar_perm(b, 1, 1, 3, 3); }
VECMATH_FINLINE vec4f v_perm_xzxz(vec4f b) { return scalar_perm(b, 0, 2, 0, 2); }
VECMATH_FINLINE vec4f v_perm_zxzx(vec4f b) { return scalar_perm(b, 2, 0, 2, 0); }
VECMATH_FINLINE vec4f v_perm_zwzw(vec4f b) { return scalar_perm(b, 2, 3, 2, 3); }
VECMATH_FINLINE vec4f v_perm_ywyw(vec4f b) { return scalar_perm(b, 1, 3, 1, 3); }
VECMATH_FINLINE vec4f v_perm_xyzz(vec4f b) { return scalar_perm(b, 0, 1, 2, 2); }

VECMATH_FINLINE vec4f v_splat_x(vec4f a) { return scalar_perm(a, 0, 0, 0, 0); }
VECMATH_FINLINE vec4f v_splat_y(vec4f a) { return scalar_perm(a, 1, 1, 1, 1); }
VECMATH_FINLINE vec4f v_splat_z(vec4f a) { return scalar_perm(a, 2, 2, 2, 2); }
VECMATH_FINLINE vec4f v_splat_w(vec4f a) { return scalar_perm(a, 3, 3, 3, 3); }

VECMATH_FINLINE vec4i v_splat_xi(vec4i a) { vec4i r = {a.i[0], a.i[0], a.i[0], a.i[0]}; return r; }
VECMATH_FINLINE vec4i v_splat_yi(vec4i a) { vec4i r = {a.i[1], a.i[1], a.i[1], a.i[1]}; return r; }
VECMATH_FINLINE vec4i v_splat_zi(vec4i a) { vec4i r = {a.i[2], a.i[2], a.i[2], a.i[2]}; return r; }
VECMATH_FINLINE vec4i v_splat_wi(vec4i a) { vec4i r = {a.i[3], a.i[3], a.i[3], a.i[3]}; return r; }

VECMATH_FINLINE void v_st(void *m, vec4f v) { memcpy(m, &v, 16); }
VECMATH_FINLINE void v_stu(void *m, vec4f v) { memcpy(m, &v, 16); }
VECMATH_FINLINE void v_sti(void *m, vec4i v) { memcpy(m, &v, 16); }
VECMATH_FINLINE void v_stui(void *m, vec4i v) { memcpy(m, &v, 16); }
VECMATH_FINLINE void v_stui_half(void *m, vec4i v) { memcpy(m, &v, 8); }
VECMATH_FINLINE void v_stu_half(void *m, vec4f v) { memcpy(m, &v, 8); }
VECMATH_FINLINE void v_stu_p3(float *p3, vec3f v) { memcpy(p3, &v, 12); }
VECMATH_FINLINE void v_stui_p3(int *p3, vec4i v) { memcpy(p3, &v, 12); }

VECMATH_FINLINE vec4f v_merge_hw(vec4f a, vec4f b) { vec4f r = {a.f[0], b.f[0], a.f[1], b.f[1]}; return r; }
VECMATH_FINLINE vec4f v_merge_lw(vec4f a, vec4f b) { vec4f r = {a.f[2], b.f[2], a.f[3], b.f[3]}; return r; }

VECMATH_FINLINE int v_signmask(vec4f a)
{
  return scalar_sign(a.f[0]) | (scalar_sign(a.f[1]) << 1) | (scalar_sign(a.f[2]) << 2) | (scalar_sign(a.f[3]) << 3);
}

VECMATH_FINLINE bool v_test_all_bits_zeros(vec4f a)
{
  return (scalar_f2u(a.f[0]) | scalar_f2u(a.f[1]) | scalar_f2u(a.f[2]) | scalar_f2u(a.f[3])) == 0;
}
VECMATH_FINLINE bool v_test_all_bits_ones(vec4f a)
{
  return (scalar_f2u(a.f[0]) & scalar_f2u(a.f[1]) & scalar_f2u(a.f[2]) & scalar_f2u(a.f[3])) == 0xFFFFFFFFu;
}
VECMATH_FINLINE bool v_test_any_bit_set(vec4f a) { return !v_test_all_bits_zeros(a); }

VECMATH_FINLINE bool v_check_xyzw_all_true(vec4f a) { return v_signmask(a) == 0b1111; }
VECMATH_FINLINE bool v_check_xyzw_all_false(vec4f a) { return v_signmask(a) == 0; }
VECMATH_FINLINE bool v_check_xyzw_any_true(vec4f a) { return v_signmask(a) != 0; }

VECMATH_FINLINE bool v_check_xyz_all_true(vec4f a) { return (v_signmask(a) & 0b111) == 0b111; }
VECMATH_FINLINE bool v_check_xyz_all_false(vec4f a) { return (v_signmask(a) & 0b111) == 0; }
VECMATH_FINLINE bool v_check_xyz_any_true(vec4f a) { return (v_signmask(a) & 0b111) != 0; }

VECMATH_FINLINE vec4f is_neg_special(vec4f a) { return v_cast_vec4f(v_srai(v_cast_vec4i(a), 31)); }

VECMATH_FINLINE vec4f v_cmp_eq(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = scalar_fmask(a.f[k] == b.f[k]); return r;
}
VECMATH_FINLINE vec4f v_cmp_neq(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = scalar_fmask(!(a.f[k] == b.f[k])); return r;
}
VECMATH_FINLINE vec4i v_cmp_eqi(vec4i a, vec4i b)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = a.i[k] == b.i[k] ? -1 : 0; return r;
}
VECMATH_FINLINE vec4f v_cmp_eqi(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = scalar_fmask(scalar_f2u(a.f[k]) == scalar_f2u(b.f[k])); return r;
}
VECMATH_FINLINE vec4f v_cmp_ge(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = scalar_fmask(a.f[k] >= b.f[k]); return r;
}
VECMATH_FINLINE vec4f v_cmp_gt(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = scalar_fmask(a.f[k] > b.f[k]); return r;
}
VECMATH_FINLINE vec4i v_cmp_lti(vec4i a, vec4i b)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = a.i[k] < b.i[k] ? -1 : 0; return r;
}
VECMATH_FINLINE vec4i v_cmp_gti(vec4i a, vec4i b)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = a.i[k] > b.i[k] ? -1 : 0; return r;
}

VECMATH_FINLINE vec4f v_and(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = scalar_u2f(scalar_f2u(a.f[k]) & scalar_f2u(b.f[k])); return r;
}
VECMATH_FINLINE vec4f v_andnot(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = scalar_u2f(~scalar_f2u(a.f[k]) & scalar_f2u(b.f[k])); return r;
}
VECMATH_FINLINE vec4f v_or(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = scalar_u2f(scalar_f2u(a.f[k]) | scalar_f2u(b.f[k])); return r;
}
VECMATH_FINLINE vec4f v_xor(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = scalar_u2f(scalar_f2u(a.f[k]) ^ scalar_f2u(b.f[k])); return r;
}
VECMATH_FINLINE vec4f v_not(vec4f v) { return v_xor(v, v_set_all_bits()); }
VECMATH_FINLINE vec4f v_btsel(vec4f a, vec4f b, vec4f c)
{
  vec4f r;
  for (int k = 0; k < 4; k++)
    r.f[k] = scalar_u2f((scalar_f2u(c.f[k]) & scalar_f2u(b.f[k])) | (~scalar_f2u(c.f[k]) & scalar_f2u(a.f[k])));
  return r;
}
VECMATH_FINLINE vec4i v_btseli(vec4i a, vec4i b, vec4i c)
{
  vec4i r;
  for (int k = 0; k < 4; k++)
    r.i[k] = (int32_t)(((uint32_t)c.i[k] & (uint32_t)b.i[k]) | (~(uint32_t)c.i[k] & (uint32_t)a.i[k]));
  return r;
}

VECMATH_FINLINE vec4i v_cast_vec4i(vec4f a) { vec4i r; memcpy(&r, &a, 16); return r; }
VECMATH_FINLINE vec4f v_cast_vec4f(vec4i a) { vec4f r; memcpy(&r, &a, 16); return r; }

VECMATH_FINLINE vec4i v_cvti_vec4i(vec4f a)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = scalar_trunc_to_int(a.f[k]); return r;
}
VECMATH_FINLINE vec4i v_cvtu_vec4i(vec4f a)
{
  vec4i r;
  for (int k = 0; k < 4; k++)
    r.i[k] = (int32_t)(uint32_t)(a.f[k] == a.f[k] ? (int64_t)nearbyintf(a.f[k]) : 0);
  return r;
}
VECMATH_FINLINE vec4i v_cvtu_vec4i_ieee(vec4f a)
{
  vec4i r;
  for (int k = 0; k < 4; k++)
    r.i[k] = (int32_t)(uint32_t)(a.f[k] == a.f[k] ? (int64_t)a.f[k] : 0);
  return r;
}
VECMATH_FINLINE vec4f v_cvti_vec4f(vec4i a)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = (float)a.i[k]; return r;
}
VECMATH_FINLINE vec4f v_cvtu_vec4f(vec4i v)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = (float)(uint32_t)v.i[k]; return r;
}
VECMATH_FINLINE vec4f v_cvtu_vec4f_ieee(vec4i v)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = (float)(uint32_t)v.i[k]; return r;
}
VECMATH_FINLINE vec4i v_cvt_roundi_ieee(vec4f a)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = scalar_round_to_int(a.f[k]); return r;
}

VECMATH_FINLINE vec4i v_cvt_floori(vec4f a)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = scalar_trunc_to_int(floorf(a.f[k])); return r;
}
VECMATH_FINLINE vec4i v_cvt_ceili(vec4f a)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = scalar_trunc_to_int(ceilf(a.f[k])); return r;
}
VECMATH_FINLINE vec4i v_cvt_trunci(vec4f a) { return v_cvti_vec4i(a); }
VECMATH_FINLINE vec4f v_floor(vec4f a)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = floorf(a.f[k]); return r;
}
VECMATH_FINLINE vec4f v_ceil(vec4f a)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = ceilf(a.f[k]); return r;
}
VECMATH_FINLINE vec4f v_round_ieee(vec4f a)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = nearbyintf(a.f[k]); return r;
}
VECMATH_FINLINE vec4f v_trunc(vec4f a)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = truncf(a.f[k]); return r;
}
VECMATH_FINLINE vec4f v_sel(vec4f a, vec4f b, vec4f c)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = (scalar_f2u(c.f[k]) >> 31) ? b.f[k] : a.f[k]; return r;
}
VECMATH_FINLINE vec4i v_seli(vec4i a, vec4i b, vec4i c)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = ((uint32_t)c.i[k] >> 31) ? b.i[k] : a.i[k]; return r;
}

VECMATH_FINLINE vec4f v_add(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = a.f[k] + b.f[k]; return r;
}
VECMATH_FINLINE vec4f v_sub(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = a.f[k] - b.f[k]; return r;
}
VECMATH_FINLINE vec4f v_mul(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = a.f[k] * b.f[k]; return r;
}
VECMATH_FINLINE vec4f v_div(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = a.f[k] / b.f[k]; return r;
}
VECMATH_FINLINE vec4f v_madd(vec4f a, vec4f b, vec4f c)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = a.f[k] * b.f[k] + c.f[k]; return r;
}
VECMATH_FINLINE vec4f v_msub(vec4f a, vec4f b, vec4f c)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = a.f[k] * b.f[k] - c.f[k]; return r;
}
VECMATH_FINLINE vec4f v_nmsub(vec4f a, vec4f b, vec4f c)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = c.f[k] - a.f[k] * b.f[k]; return r;
}
VECMATH_FINLINE vec4f v_add_x(vec4f a, vec4f b) { vec4f r = a; r.f[0] = a.f[0] + b.f[0]; return r; }
VECMATH_FINLINE vec4f v_sub_x(vec4f a, vec4f b) { vec4f r = a; r.f[0] = a.f[0] - b.f[0]; return r; }
VECMATH_FINLINE vec4f v_mul_x(vec4f a, vec4f b) { vec4f r = a; r.f[0] = a.f[0] * b.f[0]; return r; }
VECMATH_FINLINE vec4f v_div_x(vec4f a, vec4f b) { vec4f r = a; r.f[0] = a.f[0] / b.f[0]; return r; }
VECMATH_FINLINE vec4f v_madd_x(vec4f a, vec4f b, vec4f c) { vec4f r = a; r.f[0] = a.f[0] * b.f[0] + c.f[0]; return r; }
VECMATH_FINLINE vec4f v_msub_x(vec4f a, vec4f b, vec4f c) { vec4f r = a; r.f[0] = a.f[0] * b.f[0] - c.f[0]; return r; }
VECMATH_FINLINE vec4f v_nmsub_x(vec4f a, vec4f b, vec4f c) { vec4f r = c; r.f[0] = c.f[0] - a.f[0] * b.f[0]; return r; }
VECMATH_FINLINE vec4i v_addi(vec4i a, vec4i b)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = (int32_t)((uint32_t)a.i[k] + (uint32_t)b.i[k]); return r;
}
VECMATH_FINLINE vec4i v_subi(vec4i a, vec4i b)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = (int32_t)((uint32_t)a.i[k] - (uint32_t)b.i[k]); return r;
}

VECMATH_FINLINE vec4i v_addi16(vec4i a, vec4i b)
{
  uint16_t x[8], y[8]; memcpy(x, &a, 16); memcpy(y, &b, 16);
  for (int k = 0; k < 8; k++) x[k] = (uint16_t)(x[k] + y[k]);
  vec4i r; memcpy(&r, x, 16); return r;
}
VECMATH_FINLINE vec4i v_subi16(vec4i a, vec4i b)
{
  uint16_t x[8], y[8]; memcpy(x, &a, 16); memcpy(y, &b, 16);
  for (int k = 0; k < 8; k++) x[k] = (uint16_t)(x[k] - y[k]);
  vec4i r; memcpy(&r, x, 16); return r;
}
VECMATH_FINLINE vec4i v_muli16(vec4i a, vec4i b)
{
  uint16_t x[8], y[8]; memcpy(x, &a, 16); memcpy(y, &b, 16);
  for (int k = 0; k < 8; k++) x[k] = (uint16_t)((uint32_t)x[k] * y[k]);
  vec4i r; memcpy(&r, x, 16); return r;
}
VECMATH_FINLINE vec4i v_mulhi16(vec4i a, vec4i b)
{
  int16_t x[8], y[8]; memcpy(x, &a, 16); memcpy(y, &b, 16);
  for (int k = 0; k < 8; k++) x[k] = (int16_t)(((int32_t)x[k] * y[k]) >> 16);
  vec4i r; memcpy(&r, x, 16); return r;
}
VECMATH_FINLINE vec4i v_madd_i16(vec4i a, vec4i b)
{
  int16_t x[8], y[8]; memcpy(x, &a, 16); memcpy(y, &b, 16);
  vec4i r;
  //the sum wraps at 0x8000*0x8000*2 like pmaddwd does - keep it defined without -fwrapv
  for (int k = 0; k < 4; k++)
    r.i[k] = (int32_t)((uint32_t)((int32_t)x[2 * k] * y[2 * k]) + (uint32_t)((int32_t)x[2 * k + 1] * y[2 * k + 1]));
  return r;
}
VECMATH_FINLINE vec4i v_splatsi16(int v)
{
  uint16_t x[8]; for (int k = 0; k < 8; k++) x[k] = (uint16_t)v;
  vec4i r; memcpy(&r, x, 16); return r;
}

VECMATH_FINLINE vec4i scalar_interleave8(vec4i a, vec4i b, int base)
{
  uint8_t x[16], y[16], o[16]; memcpy(x, &a, 16); memcpy(y, &b, 16);
  for (int k = 0; k < 8; k++) { o[2 * k] = x[base + k]; o[2 * k + 1] = y[base + k]; }
  vec4i r; memcpy(&r, o, 16); return r;
}
VECMATH_FINLINE vec4i scalar_interleave16(vec4i a, vec4i b, int base)
{
  uint16_t x[8], y[8], o[8]; memcpy(x, &a, 16); memcpy(y, &b, 16);
  for (int k = 0; k < 4; k++) { o[2 * k] = x[base + k]; o[2 * k + 1] = y[base + k]; }
  vec4i r; memcpy(&r, o, 16); return r;
}
VECMATH_FINLINE vec4i v_interleave_lo_i8(vec4i a, vec4i b) { return scalar_interleave8(a, b, 0); }
VECMATH_FINLINE vec4i v_interleave_hi_i8(vec4i a, vec4i b) { return scalar_interleave8(a, b, 8); }
VECMATH_FINLINE vec4i v_interleave_lo_i16(vec4i a, vec4i b) { return scalar_interleave16(a, b, 0); }
VECMATH_FINLINE vec4i v_interleave_hi_i16(vec4i a, vec4i b) { return scalar_interleave16(a, b, 4); }
VECMATH_FINLINE vec4i v_interleave_lo_i32(vec4i a, vec4i b) { vec4i r = {a.i[0], b.i[0], a.i[1], b.i[1]}; return r; }
VECMATH_FINLINE vec4i v_interleave_hi_i32(vec4i a, vec4i b) { vec4i r = {a.i[2], b.i[2], a.i[3], b.i[3]}; return r; }
VECMATH_FINLINE vec4i v_interleave_lo_i64(vec4i a, vec4i b) { vec4i r = {a.i[0], a.i[1], b.i[0], b.i[1]}; return r; }
VECMATH_FINLINE vec4i v_interleave_hi_i64(vec4i a, vec4i b) { vec4i r = {a.i[2], a.i[3], b.i[2], b.i[3]}; return r; }

VECMATH_FINLINE vec4f v_hadd4_x(vec4f a)
{
  vec4f s = v_add(a, v_rot_2(a));
  return v_add_x(s, v_splat_y(s));
}
VECMATH_FINLINE vec4f v_hadd3_x(vec3f a)
{
  vec4f r = a;
  r.f[0] = (a.f[0] + a.f[1]) + a.f[2];
  return r;
}

VECMATH_FINLINE vec4i v_slli(vec4i v, int bits)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = bits >= 32 ? 0 : (int32_t)((uint32_t)v.i[k] << bits); return r;
}
VECMATH_FINLINE vec4i v_srli(vec4i v, int bits)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = bits >= 32 ? 0 : (int32_t)((uint32_t)v.i[k] >> bits); return r;
}
VECMATH_FINLINE vec4i v_srai(vec4i v, int bits)
{
  int b = bits > 31 ? 31 : bits;
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = v.i[k] >> b; return r;
}
VECMATH_FINLINE vec4i v_slli_64(vec4i v, int bits)
{
  uint64_t x[2]; memcpy(x, &v, 16);
  x[0] = bits >= 64 ? 0 : x[0] << bits; x[1] = bits >= 64 ? 0 : x[1] << bits;
  vec4i r; memcpy(&r, x, 16); return r;
}
VECMATH_FINLINE vec4i v_srli_64(vec4i v, int bits)
{
  uint64_t x[2]; memcpy(x, &v, 16);
  x[0] = bits >= 64 ? 0 : x[0] >> bits; x[1] = bits >= 64 ? 0 : x[1] >> bits;
  vec4i r; memcpy(&r, x, 16); return r;
}
VECMATH_FINLINE vec4i v_slli_n(vec4i v, int bits) { return v_slli(v, bits); }
VECMATH_FINLINE vec4i v_srli_n(vec4i v, int bits) { return v_srli(v, bits); }
VECMATH_FINLINE vec4i v_srai_n(vec4i v, int bits) { return v_srai(v, bits); }
VECMATH_FINLINE vec4i v_slli_n(vec4i v, vec4i bits) { uint64_t c[2]; memcpy(c, &bits, 16); return v_slli(v, c[0] > 63 ? 64 : (int)c[0]); }
VECMATH_FINLINE vec4i v_srli_n(vec4i v, vec4i bits) { uint64_t c[2]; memcpy(c, &bits, 16); return v_srli(v, c[0] > 63 ? 64 : (int)c[0]); }
VECMATH_FINLINE vec4i v_srai_n(vec4i v, vec4i bits) { uint64_t c[2]; memcpy(c, &bits, 16); return v_srai(v, c[0] > 63 ? 64 : (int)c[0]); }

VECMATH_FINLINE vec4i v_sll(vec4i v, int bits) { return v_slli(v, bits); }
VECMATH_FINLINE vec4i v_srl(vec4i v, int bits) { return v_srli(v, bits); }
VECMATH_FINLINE vec4i v_sra(vec4i v, int bits) { return v_srai(v, bits); }

VECMATH_FINLINE vec4i v_ori(vec4i a, vec4i b)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = (int32_t)((uint32_t)a.i[k] | (uint32_t)b.i[k]); return r;
}
VECMATH_FINLINE vec4i v_andi(vec4i a, vec4i b)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = (int32_t)((uint32_t)a.i[k] & (uint32_t)b.i[k]); return r;
}
VECMATH_FINLINE vec4i v_andnoti(vec4i a, vec4i b)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = (int32_t)(~(uint32_t)a.i[k] & (uint32_t)b.i[k]); return r;
}
VECMATH_FINLINE vec4i v_xori(vec4i a, vec4i b)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = (int32_t)((uint32_t)a.i[k] ^ (uint32_t)b.i[k]); return r;
}

VECMATH_FINLINE int16_t scalar_sat_i16(int32_t v) { return (int16_t)(v < -32768 ? -32768 : (v > 32767 ? 32767 : v)); }
VECMATH_FINLINE uint16_t scalar_sat_u16(int32_t v) { return (uint16_t)(v < 0 ? 0 : (v > 65535 ? 65535 : v)); }
VECMATH_FINLINE uint8_t scalar_sat_u8(int16_t v) { return (uint8_t)(v < 0 ? 0 : (v > 255 ? 255 : v)); }

VECMATH_FINLINE vec4i v_packs(vec4i a, vec4i b)
{
  int16_t o[8];
  for (int k = 0; k < 4; k++) { o[k] = scalar_sat_i16(a.i[k]); o[k + 4] = scalar_sat_i16(b.i[k]); }
  vec4i r; memcpy(&r, o, 16); return r;
}
VECMATH_FINLINE vec4i v_packs(vec4i a) { return v_packs(a, a); }
VECMATH_FINLINE vec4i v_packus(vec4i a, vec4i b)
{
  uint16_t o[8];
  for (int k = 0; k < 4; k++) { o[k] = scalar_sat_u16(a.i[k]); o[k + 4] = scalar_sat_u16(b.i[k]); }
  vec4i r; memcpy(&r, o, 16); return r;
}
VECMATH_FINLINE vec4i v_packus(vec4i a) { return v_packus(a, a); }
VECMATH_FINLINE vec4i v_packus16(vec4i a, vec4i b)
{
  int16_t x[8], y[8]; uint8_t o[16]; memcpy(x, &a, 16); memcpy(y, &b, 16);
  for (int k = 0; k < 8; k++) { o[k] = scalar_sat_u8(x[k]); o[k + 8] = scalar_sat_u8(y[k]); }
  vec4i r; memcpy(&r, o, 16); return r;
}
VECMATH_FINLINE vec4i v_packus16(vec4i a) { return v_packus16(a, a); }

VECMATH_FINLINE vec4i v_muli(vec4i a, vec4i b)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = (int32_t)((uint32_t)a.i[k] * (uint32_t)b.i[k]); return r;
}

VECMATH_FINLINE vec4f v_rcp_unprecise(vec4f a)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = 1.f / a.f[k]; return r;
}
VECMATH_FINLINE vec4f v_rcp_est(vec4f a) { return v_rcp_unprecise(a); }
VECMATH_FINLINE vec4f v_rcp_unprecise_x(vec4f a) { vec4f r = a; r.f[0] = 1.f / a.f[0]; return r; }
VECMATH_FINLINE vec4f v_rcp_est_x(vec4f a) { return v_rcp_unprecise_x(a); }

VECMATH_FINLINE vec4f v_rsqrt_unprecise(vec4f a)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = 1.f / sqrtf(a.f[k]); return r;
}
VECMATH_FINLINE vec4f v_rsqrt_unprecise_x(vec4f a) { vec4f r = a; r.f[0] = 1.f / sqrtf(a.f[0]); return r; }
VECMATH_FINLINE vec4f v_rsqrt_est(vec4f a) { return v_rsqrt_unprecise(a); }
VECMATH_FINLINE vec4f v_rsqrt_est_x(vec4f a) { return v_rsqrt_unprecise_x(a); }
VECMATH_FINLINE vec4f v_rsqrt(vec4f a) { return v_div(v_sqrt(a), a); }
VECMATH_FINLINE vec4f v_rsqrt_x(vec4f a) { return v_div_x(v_sqrt_x(a), a); }

VECMATH_FINLINE vec4f v_min(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = a.f[k] < b.f[k] ? a.f[k] : b.f[k]; return r;
}
VECMATH_FINLINE vec4f v_max(vec4f a, vec4f b)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = a.f[k] > b.f[k] ? a.f[k] : b.f[k]; return r;
}
VECMATH_FINLINE vec4i v_mini(vec4i a, vec4i b)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = a.i[k] < b.i[k] ? a.i[k] : b.i[k]; return r;
}
VECMATH_FINLINE vec4i v_maxi(vec4i a, vec4i b)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = a.i[k] > b.i[k] ? a.i[k] : b.i[k]; return r;
}
VECMATH_FINLINE vec4i v_minu(vec4i a, vec4i b)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = (uint32_t)a.i[k] < (uint32_t)b.i[k] ? a.i[k] : b.i[k]; return r;
}
VECMATH_FINLINE vec4i v_maxu(vec4i a, vec4i b)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = (uint32_t)a.i[k] > (uint32_t)b.i[k] ? a.i[k] : b.i[k]; return r;
}
VECMATH_FINLINE vec4i v_absi(vec4i a)
{
  vec4i r; for (int k = 0; k < 4; k++) r.i[k] = a.i[k] < 0 ? (int32_t)(0u - (uint32_t)a.i[k]) : a.i[k]; return r;
}

VECMATH_FINLINE vec4f v_neg(vec4f a) { return v_xor(a, v_cast_vec4f(v_splatsi(0x80000000))); }
VECMATH_FINLINE vec4i v_negi(vec4i a) { return v_subi(v_cast_vec4i(v_zero()), a); }
VECMATH_FINLINE vec4f v_abs(vec4f a)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = scalar_u2f(scalar_f2u(a.f[k]) & 0x7FFFFFFFu); return r;
}

VECMATH_FINLINE vec4f v_sqrt4_fast(vec4f a)
{
  vec4f r; for (int k = 0; k < 4; k++) r.f[k] = sqrtf(a.f[k]); return r;
}
VECMATH_FINLINE vec4f v_sqrt(vec4f a) { return v_sqrt4_fast(a); }
VECMATH_FINLINE vec4f v_sqrt_fast_x(vec4f a) { vec4f r = a; r.f[0] = sqrtf(a.f[0]); return r; }
VECMATH_FINLINE vec4f v_sqrt_x(vec4f a) { return v_sqrt_fast_x(a); }

VECMATH_FINLINE vec4f v_rot_1(vec4f a) { return scalar_perm(a, 1, 2, 3, 0); }
VECMATH_FINLINE vec4f v_rot_2(vec4f a) { return scalar_perm(a, 2, 3, 0, 1); }
VECMATH_FINLINE vec4f v_rot_3(vec4f a) { return scalar_perm(a, 3, 0, 1, 2); }
VECMATH_FINLINE vec4i v_roti_1(vec4i a) { vec4i r = {a.i[1], a.i[2], a.i[3], a.i[0]}; return r; }
VECMATH_FINLINE vec4i v_roti_2(vec4i a) { vec4i r = {a.i[2], a.i[3], a.i[0], a.i[1]}; return r; }
VECMATH_FINLINE vec4i v_roti_3(vec4i a) { vec4i r = {a.i[3], a.i[0], a.i[1], a.i[2]}; return r; }

VECMATH_FINLINE vec4f v_perm_yzxx(vec4f a) { return scalar_perm(a, 1, 2, 0, 0); }
VECMATH_FINLINE vec4f v_perm_yzxy(vec4f a) { return scalar_perm(a, 1, 2, 0, 1); }
VECMATH_FINLINE vec4f v_perm_yzxw(vec4f a) { return scalar_perm(a, 1, 2, 0, 3); }
VECMATH_FINLINE vec4f v_perm_zxyw(vec4f a) { return scalar_perm(a, 2, 0, 1, 3); }
VECMATH_FINLINE vec4f v_perm_xxyy(vec4f a) { return scalar_perm(a, 0, 0, 1, 1); }
VECMATH_FINLINE vec4f v_perm_zzww(vec4f a) { return scalar_perm(a, 2, 2, 3, 3); }

VECMATH_FINLINE vec4f v_perm_xzac(vec4f xyzw, vec4f abcd) { return scalar_shuffle(xyzw, abcd, 0, 2, 0, 2); }
VECMATH_FINLINE vec4f v_perm_ywbd(vec4f xyzw, vec4f abcd) { return scalar_shuffle(xyzw, abcd, 1, 3, 1, 3); }
VECMATH_FINLINE vec4f v_perm_xyab(vec4f xyzw, vec4f abcd) { return scalar_shuffle(xyzw, abcd, 0, 1, 0, 1); }
VECMATH_FINLINE vec4f v_perm_zwcd(vec4f xyzw, vec4f abcd) { return scalar_shuffle(xyzw, abcd, 2, 3, 2, 3); }
VECMATH_FINLINE vec4f v_perm_bbyx(vec4f xyzw, vec4f abcd) { return scalar_shuffle(abcd, xyzw, 1, 1, 1, 0); }
VECMATH_FINLINE vec4f v_perm_xaxa(vec4f xyzw, vec4f abcd)
{
  vec4f r = {xyzw.f[0], abcd.f[0], xyzw.f[0], abcd.f[0]}; return r;
}
VECMATH_FINLINE vec4f v_perm_yybb(vec4f xyzw, vec4f abcd) { return scalar_shuffle(xyzw, abcd, 1, 1, 1, 1); }
VECMATH_FINLINE vec4f v_perm_xxab(vec4f xyzw, vec4f abcd) { return scalar_shuffle(xyzw, abcd, 0, 0, 0, 1); }
VECMATH_FINLINE vec4f v_perm_yzab(vec4f xyzw, vec4f abcd) { return scalar_shuffle(xyzw, abcd, 1, 2, 0, 1); }

VECMATH_FINLINE vec4f v_perm_xycd(vec4f xyzw, vec4f abcd) { return scalar_shuffle(xyzw, abcd, 0, 1, 2, 3); }
VECMATH_FINLINE vec4f v_perm_ayzw(vec4f xyzw, vec4f abcd)
{
  vec4f r = xyzw; r.f[0] = abcd.f[0]; return r;
}

VECMATH_FINLINE vec4f v_perm_xzbx(vec4f xyzw, vec4f abcd)
{
  vec4f r = {xyzw.f[0], xyzw.f[2], abcd.f[1], xyzw.f[0]}; return r;
}
VECMATH_FINLINE vec4f v_perm_xzya(vec4f xyzw, vec4f abcd)
{
  vec4f r = {xyzw.f[0], xyzw.f[2], xyzw.f[1], abcd.f[0]}; return r;
}
VECMATH_FINLINE vec4f v_perm_yxxc(vec4f xyzw, vec4f abcd)
{
  vec4f r = {xyzw.f[1], xyzw.f[0], xyzw.f[0], abcd.f[2]}; return r;
}
VECMATH_FINLINE vec4f v_perm_yaxx(vec4f xyzw, vec4f abcd)
{
  vec4f r = {xyzw.f[1], abcd.f[0], xyzw.f[0], xyzw.f[0]}; return r;
}
VECMATH_FINLINE vec4f v_perm_zxxb(vec4f xyzw, vec4f abcd)
{
  vec4f r = {xyzw.f[2], xyzw.f[0], xyzw.f[0], abcd.f[1]}; return r;
}
VECMATH_FINLINE vec4f v_perm_zayx(vec4f xyzw, vec4f abcd)
{
  vec4f r = {xyzw.f[2], abcd.f[0], xyzw.f[1], xyzw.f[0]}; return r;
}
VECMATH_FINLINE vec4f v_perm_bzxx(vec4f xyzw, vec4f abcd)
{
  vec4f r = {abcd.f[1], xyzw.f[2], xyzw.f[0], xyzw.f[0]}; return r;
}
VECMATH_FINLINE vec4f v_perm_caxx(vec4f xyzw, vec4f abcd)
{
  vec4f r = {abcd.f[2], abcd.f[0], xyzw.f[0], xyzw.f[0]}; return r;
}

VECMATH_FINLINE vec3f v_mat43_extract_pos(mat43f_cref mat)
{
  vec4f r = {mat.row0.f[3], mat.row1.f[3], mat.row2.f[3], mat.row2.f[0]}; return r;
}

VECMATH_FINLINE vec4f v_perm_xbzw(vec4f xyzw, vec4f abcd)
{
  vec4f r = xyzw; r.f[1] = abcd.f[1]; return r;
}
VECMATH_FINLINE vec4f v_perm_xycw(vec4f xyzw, vec4f abcd)
{
  vec4f r = xyzw; r.f[2] = abcd.f[2]; return r;
}
VECMATH_FINLINE vec4f v_perm_xyzd(vec4f xyzw, vec4f abcd)
{
  vec4f r = xyzw; r.f[3] = abcd.f[3]; return r;
}

VECMATH_FINLINE vec4f v_dot4(vec4f a, vec4f b)
{
  float d = (a.f[0] * b.f[0] + a.f[1] * b.f[1]) + (a.f[2] * b.f[2] + a.f[3] * b.f[3]);
  return v_splats(d);
}
VECMATH_FINLINE vec4f v_dot4_x(vec4f a, vec4f b)
{
  vec4f r = {(a.f[0] * b.f[0] + a.f[1] * b.f[1]) + (a.f[2] * b.f[2] + a.f[3] * b.f[3]), 0.f, 0.f, 0.f}; return r;
}
VECMATH_FINLINE vec4f v_dot3(vec4f a, vec4f b)
{
  float d = (a.f[0] * b.f[0] + a.f[1] * b.f[1]) + a.f[2] * b.f[2];
  return v_splats(d);
}
VECMATH_FINLINE vec4f v_dot3_x(vec4f a, vec4f b)
{
  vec4f r = {(a.f[0] * b.f[0] + a.f[1] * b.f[1]) + a.f[2] * b.f[2], 0.f, 0.f, 0.f}; return r;
}
VECMATH_FINLINE vec4f v_dot2(vec4f a, vec4f b)
{
  return v_splats(a.f[0] * b.f[0] + a.f[1] * b.f[1]);
}
VECMATH_FINLINE vec4f v_dot2_x(vec4f a, vec4f b)
{
  vec4f r = {a.f[0] * b.f[0] + a.f[1] * b.f[1], 0.f, 0.f, 0.f}; return r;
}
VECMATH_FINLINE vec4f v_plane_dist_x(plane3f a, vec3f b) { return v_add_x(v_dot3_x(a, b), v_splat_w(a)); }

VECMATH_FINLINE vec4f v_length4_sq(vec4f a) { return v_dot4(a, a); }
VECMATH_FINLINE vec3f v_length3_sq(vec3f a) { return v_dot3(a, a); }
VECMATH_FINLINE vec4f v_length2_sq(vec4f a) { return v_dot2(a, a); }
VECMATH_FINLINE vec4f v_length4_sq_x(vec4f a) { return v_dot4_x(a, a); }
VECMATH_FINLINE vec3f v_length3_sq_x(vec3f a) { return v_dot3_x(a, a); }
VECMATH_FINLINE vec4f v_length2_sq_x(vec4f a) { return v_dot2_x(a, a); }

VECMATH_FINLINE vec4f v_norm4(vec4f a) { return v_div(a, v_splat_x(v_sqrt_x(v_dot4_x(a, a)))); }
VECMATH_FINLINE vec4f v_norm3(vec4f a) { return v_div(a, v_splat_x(v_sqrt_x(v_dot3_x(a, a)))); }
VECMATH_FINLINE vec4f v_norm2(vec4f a) { return v_div(a, v_splat_x(v_sqrt_x(v_dot2_x(a, a)))); }

VECMATH_FINLINE vec4f v_plane_dist(plane3f a, vec3f b)
{
  return v_splat_x(v_plane_dist_x(a, b));
}

VECMATH_FINLINE void v_mat_33cu_from_mat33(float * __restrict m33, const mat33f &tm)
{
  v_stu_p3(m33 + 0, tm.col0);
  v_stu_p3(m33 + 3, tm.col1);
  v_stu_p3(m33 + 6, tm.col2);
}

VECMATH_FINLINE void v_mat44_make_from_43cu(mat44f &tm, const float *const __restrict m43)
{
  vec4f c0 = {m43[0], m43[1], m43[2], 0.f};
  vec4f c1 = {m43[3], m43[4], m43[5], 0.f};
  vec4f c2 = {m43[6], m43[7], m43[8], 0.f};
  vec4f c3 = {m43[9], m43[10], m43[11], 1.f};
  tm.col0 = c0; tm.col1 = c1; tm.col2 = c2; tm.col3 = c3;
}
VECMATH_FINLINE void v_mat44_make_from_43ca(mat44f &tm, const float *const __restrict m43)
{
  v_mat44_make_from_43cu(tm, m43);
}
VECMATH_FINLINE void v_mat44_make_from_43cu_unsafe(mat44f &tmV, const float *const __restrict m43)
{
  v_mat44_make_from_43cu(tmV, m43);
}

VECMATH_FINLINE void v_mat_43cu_from_mat44(float * __restrict m43, const mat44f &tm)
{
  v_stu_p3(m43 + 0, tm.col0);
  v_stu_p3(m43 + 3, tm.col1);
  v_stu_p3(m43 + 6, tm.col2);
  v_stu_p3(m43 + 9, tm.col3);
}
VECMATH_FINLINE void v_mat_43ca_from_mat44(float * __restrict m43, const mat44f &tm)
{
  v_mat_43cu_from_mat44(m43, tm);
}

VECMATH_FINLINE void v_mat44_ident(mat44f &dest)
{
  dest.col0 = V_C_UNIT_1000;
  dest.col1 = V_C_UNIT_0100;
  dest.col2 = V_C_UNIT_0010;
  dest.col3 = V_C_UNIT_0001;
}
VECMATH_FINLINE void v_mat44_ident_swapxz(mat44f &dest)
{
  dest.col0 = V_C_UNIT_0010;
  dest.col1 = V_C_UNIT_0100;
  dest.col2 = V_C_UNIT_1000;
  dest.col3 = V_C_UNIT_0001;
}
VECMATH_FINLINE void v_mat33_ident(mat33f &dest)
{
  dest.col0 = V_C_UNIT_1000;
  dest.col1 = V_C_UNIT_0100;
  dest.col2 = V_C_UNIT_0010;
}
VECMATH_FINLINE void v_mat33_ident_swapxz(mat33f &dest)
{
  dest.col0 = V_C_UNIT_0010;
  dest.col1 = V_C_UNIT_0100;
  dest.col2 = V_C_UNIT_1000;
}

VECMATH_FINLINE void v_mat44_transpose(mat44f &dest, mat44f_cref src)
{
  vec4f c0 = {src.col0.f[0], src.col1.f[0], src.col2.f[0], src.col3.f[0]};
  vec4f c1 = {src.col0.f[1], src.col1.f[1], src.col2.f[1], src.col3.f[1]};
  vec4f c2 = {src.col0.f[2], src.col1.f[2], src.col2.f[2], src.col3.f[2]};
  vec4f c3 = {src.col0.f[3], src.col1.f[3], src.col2.f[3], src.col3.f[3]};
  dest.col0 = c0; dest.col1 = c1; dest.col2 = c2; dest.col3 = c3;
}
VECMATH_FINLINE void v_mat44_transpose(vec4f &r0, vec4f &r1, vec4f &r2, vec4f &r3)
{
  mat44f m; m.col0 = r0; m.col1 = r1; m.col2 = r2; m.col3 = r3;
  v_mat44_transpose(m, m);
  r0 = m.col0; r1 = m.col1; r2 = m.col2; r3 = m.col3;
}
VECMATH_FINLINE void v_mat43_transpose_to_mat44(mat44f &dest, mat43f_cref src)
{
  vec4f c0 = {src.row0.f[0], src.row1.f[0], src.row2.f[0], 0.f};
  vec4f c1 = {src.row0.f[1], src.row1.f[1], src.row2.f[1], 0.f};
  vec4f c2 = {src.row0.f[2], src.row1.f[2], src.row2.f[2], 0.f};
  vec4f c3 = {src.row0.f[3], src.row1.f[3], src.row2.f[3], 0.f};
  dest.col0 = c0; dest.col1 = c1; dest.col2 = c2; dest.col3 = c3;
}
VECMATH_FINLINE void v_mat44_transpose_to_mat43(mat43f &dest, mat44f_cref src)
{
  vec4f r0 = {src.col0.f[0], src.col1.f[0], src.col2.f[0], src.col3.f[0]};
  vec4f r1 = {src.col0.f[1], src.col1.f[1], src.col2.f[1], src.col3.f[1]};
  vec4f r2 = {src.col0.f[2], src.col1.f[2], src.col2.f[2], src.col3.f[2]};
  dest.row0 = r0; dest.row1 = r1; dest.row2 = r2;
}

VECMATH_FINLINE vec4f v_mat44_mul_vec4(mat44f_cref m, vec4f v)
{
  return v_add(
    v_add(v_mul(v_splat_x(v), m.col0), v_mul(v_splat_y(v), m.col1)),
    v_add(v_mul(v_splat_z(v), m.col2), v_mul(v_splat_w(v), m.col3)));
}
VECMATH_FINLINE vec4f v_mat44_mul_vec3v(mat44f_cref m, vec3f v)
{
  return v_add(v_add(v_mul(v_splat_x(v), m.col0), v_mul(v_splat_y(v), m.col1)),
               v_mul(v_splat_z(v), m.col2));
}
VECMATH_FINLINE vec4f v_mat44_mul_vec3p(mat44f_cref m, vec3f v)
{
  return v_add(
    v_add(v_mul(v_splat_x(v), m.col0), v_mul(v_splat_y(v), m.col1)),
    v_add(v_mul(v_splat_z(v), m.col2), m.col3));
}
VECMATH_FINLINE vec3f v_mat33_mul_vec3(mat33f_cref m, vec3f v)
{
  return v_add(v_add(v_mul(v_splat_x(v), m.col0), v_mul(v_splat_y(v), m.col1)),
               v_mul(v_splat_z(v), m.col2));
}

// transcription key: V_SHUFFLE 0xB1 = yxwz, 0x4E = zwxy
VECMATH_FINLINE void v_mat44_inverse(mat44f &dest, mat44f_cref m)
{
  vec4f minor0, minor1, minor2, minor3;
  vec4f row0, row1, row2, row3;
  vec4f det, tmp0, tmp1, tmp2, tmp3;

  tmp0 = v_perm_xzac(m.col0, m.col1);
  tmp1 = v_perm_xzac(m.col2, m.col3);
  tmp2 = v_perm_ywbd(m.col0, m.col1);
  tmp3 = v_perm_ywbd(m.col2, m.col3);
  row0 = v_perm_xzac(tmp0, tmp1);
  row1 = v_perm_xzac(tmp3, tmp2);
  row2 = v_perm_ywbd(tmp0, tmp1);
  row3 = v_perm_ywbd(tmp3, tmp2);

  tmp1 = v_mul(row2, row3);
  tmp1 = scalar_perm(tmp1, 1, 0, 3, 2);
  minor0 = v_mul(row1, tmp1);
  minor1 = v_mul(row0, tmp1);
  tmp1 = scalar_perm(tmp1, 2, 3, 0, 1);
  minor0 = v_sub(v_mul(row1, tmp1), minor0);
  minor1 = v_sub(v_mul(row0, tmp1), minor1);
  minor1 = scalar_perm(minor1, 2, 3, 0, 1);

  tmp1 = v_mul(row1, row2);
  tmp1 = scalar_perm(tmp1, 1, 0, 3, 2);
  minor0 = v_add(v_mul(row3, tmp1), minor0);
  minor3 = v_mul(row0, tmp1);
  tmp1 = scalar_perm(tmp1, 2, 3, 0, 1);
  minor0 = v_sub(minor0, v_mul(row3, tmp1));
  minor3 = v_sub(v_mul(row0, tmp1), minor3);
  minor3 = scalar_perm(minor3, 2, 3, 0, 1);

  tmp1 = v_mul(scalar_perm(row1, 2, 3, 0, 1), row3);
  tmp1 = scalar_perm(tmp1, 1, 0, 3, 2);
  row2 = scalar_perm(row2, 2, 3, 0, 1);
  minor0 = v_add(v_mul(row2, tmp1), minor0);
  minor2 = v_mul(row0, tmp1);
  tmp1 = scalar_perm(tmp1, 2, 3, 0, 1);
  minor0 = v_sub(minor0, v_mul(row2, tmp1));
  minor2 = v_sub(v_mul(row0, tmp1), minor2);
  minor2 = scalar_perm(minor2, 2, 3, 0, 1);

  tmp1 = v_mul(row0, row1);

  tmp1 = scalar_perm(tmp1, 1, 0, 3, 2);
  minor2 = v_add(v_mul(row3, tmp1), minor2);
  minor3 = v_sub(v_mul(row2, tmp1), minor3);
  tmp1 = scalar_perm(tmp1, 2, 3, 0, 1);
  minor2 = v_sub(v_mul(row3, tmp1), minor2);
  minor3 = v_sub(minor3, v_mul(row2, tmp1));

  tmp1 = v_mul(row0, row3);
  tmp1 = scalar_perm(tmp1, 1, 0, 3, 2);
  minor1 = v_sub(minor1, v_mul(row2, tmp1));
  minor2 = v_add(v_mul(row1, tmp1), minor2);
  tmp1 = scalar_perm(tmp1, 2, 3, 0, 1);
  minor1 = v_add(v_mul(row2, tmp1), minor1);
  minor2 = v_sub(minor2, v_mul(row1, tmp1));

  tmp1 = v_mul(row0, row2);
  tmp1 = scalar_perm(tmp1, 1, 0, 3, 2);
  minor1 = v_add(v_mul(row3, tmp1), minor1);
  minor3 = v_sub(minor3, v_mul(row1, tmp1));
  tmp1 = scalar_perm(tmp1, 2, 3, 0, 1);
  minor1 = v_sub(minor1, v_mul(row3, tmp1));
  minor3 = v_add(v_mul(row1, tmp1), minor3);

  det = v_mul(row0, minor0);
  det = v_add(scalar_perm(det, 2, 3, 0, 1), det);
  det = v_add_x(scalar_perm(det, 1, 0, 3, 2), det);
  tmp1 = v_rcp_safe(det);
  det = v_sub_x(v_add_x(tmp1, tmp1), v_mul_x(det, v_mul_x(tmp1, tmp1)));
  det = v_splat_x(det);
  dest.col0 = v_mul(det, minor0);
  dest.col1 = v_mul(det, minor1);
  dest.col2 = v_mul(det, minor2);
  dest.col3 = v_mul(det, minor3);
}

VECMATH_FINLINE void v_mat33_inverse(mat33f &dest, mat33f_cref m)
{
  vec4f tmp0, tmp1, tmp2, tmp3, tmp4, dot, invdet, inv0, inv1, inv2;

  tmp2 = v_cross3(m.col0, m.col1);
  tmp0 = v_cross3(m.col1, m.col2);
  tmp1 = v_cross3(m.col2, m.col0);
  dot = v_dot3(tmp2, m.col2);
  invdet = v_rcp_safe(dot);

  tmp3 = scalar_shuffle(tmp0, tmp1, 2, 0, 2, 0);
  tmp4 = scalar_shuffle(tmp0, tmp1, 3, 1, 3, 1);
  inv0 = scalar_shuffle(tmp3, tmp2, 1, 3, 0, 3);
  inv1 = scalar_shuffle(tmp4, tmp2, 1, 3, 1, 3);
  inv2 = scalar_shuffle(tmp3, tmp2, 0, 2, 2, 3);

  dest.col0 = v_mul(inv0, invdet);
  dest.col1 = v_mul(inv1, invdet);
  dest.col2 = v_mul(inv2, invdet);
}

VECMATH_FINLINE vec4f v_mat44_det(mat44f_cref m)
{
  vec4f minor0;
  vec4f row0, row1, row2, row3;
  vec4f det, tmp0, tmp1, tmp2, tmp3;

  tmp0 = v_perm_xzac(m.col0, m.col1);
  tmp1 = v_perm_xzac(m.col2, m.col3);
  tmp2 = v_perm_ywbd(m.col0, m.col1);
  tmp3 = v_perm_ywbd(m.col2, m.col3);
  row0 = v_perm_xzac(tmp0, tmp1);
  row1 = v_perm_xzac(tmp3, tmp2);
  row2 = v_perm_ywbd(tmp0, tmp1);
  row3 = v_perm_ywbd(tmp3, tmp2);

  tmp1 = v_mul(row2, row3);
  tmp1 = scalar_perm(tmp1, 1, 0, 3, 2);
  minor0 = v_mul(row1, tmp1);
  tmp1 = scalar_perm(tmp1, 2, 3, 0, 1);
  minor0 = v_sub(v_mul(row1, tmp1), minor0);

  tmp1 = v_mul(row1, row2);
  tmp1 = scalar_perm(tmp1, 1, 0, 3, 2);
  minor0 = v_add(v_mul(row3, tmp1), minor0);
  tmp1 = scalar_perm(tmp1, 2, 3, 0, 1);
  minor0 = v_sub(minor0, v_mul(row3, tmp1));

  tmp1 = v_mul(scalar_perm(row1, 2, 3, 0, 1), row3);
  tmp1 = scalar_perm(tmp1, 1, 0, 3, 2);
  row2 = scalar_perm(row2, 2, 3, 0, 1);
  minor0 = v_add(v_mul(row2, tmp1), minor0);
  tmp1 = scalar_perm(tmp1, 2, 3, 0, 1);
  minor0 = v_sub(minor0, v_mul(row2, tmp1));

  det = v_mul(row0, minor0);
  det = v_add(scalar_perm(det, 2, 3, 0, 1), det);
  det = v_add_x(scalar_perm(det, 1, 0, 3, 2), det);
  return det;
}

VECMATH_FINLINE float v_extract_x(vec4f v) { return v.f[0]; }
VECMATH_FINLINE float v_extract_y(vec4f v) { return v.f[1]; }
VECMATH_FINLINE float v_extract_z(vec4f v) { return v.f[2]; }
VECMATH_FINLINE float v_extract_w(vec4f v) { return v.f[3]; }

VECMATH_FINLINE int v_extract_xi(vec4i v) { return v.i[0]; }
VECMATH_FINLINE int v_extract_yi(vec4i v) { return v.i[1]; }
VECMATH_FINLINE int v_extract_zi(vec4i v) { return v.i[2]; }
VECMATH_FINLINE int v_extract_wi(vec4i v) { return v.i[3]; }
VECMATH_FINLINE int64_t v_extract_xi64(vec4i v) { int64_t r; memcpy(&r, &v.i[0], 8); return r; }
VECMATH_FINLINE int64_t v_extract_yi64(vec4i v) { int64_t r; memcpy(&r, &v.i[2], 8); return r; }

VECMATH_FINLINE vec4i v_splatsi64(int64_t a)
{
  vec4i r; memcpy(&r.i[0], &a, 8); memcpy(&r.i[2], &a, 8); return r;
}

VECMATH_FINLINE short v_extract_xi16(vec4i v) { return (short)(v.i[0] & 0xFFFF); }

VECMATH_FINLINE int v_test_vec_x_eqi(vec3f v, vec3f a) { return scalar_f2u(v.f[0]) == scalar_f2u(a.f[0]) ? 1 : 0; }
VECMATH_FINLINE int v_test_vec_x_eqi_0(vec3f v) { return scalar_f2u(v.f[0]) == 0 ? 1 : 0; }

VECMATH_FINLINE int v_test_vec_x_eq(vec3f v, vec3f a) { return v.f[0] == a.f[0] ? 1 : 0; }
VECMATH_FINLINE int v_test_vec_x_gt(vec3f v, vec3f a) { return v.f[0] > a.f[0] ? 1 : 0; }
VECMATH_FINLINE int v_test_vec_x_ge(vec3f v, vec3f a) { return v.f[0] >= a.f[0] ? 1 : 0; }
VECMATH_FINLINE int v_test_vec_x_lt(vec3f v, vec3f a) { return v.f[0] < a.f[0] ? 1 : 0; }
VECMATH_FINLINE int v_test_vec_x_le(vec3f v, vec3f a) { return v.f[0] <= a.f[0] ? 1 : 0; }

VECMATH_FINLINE int v_test_vec_x_eq_0(vec3f v) { return v_test_vec_x_eq(v, v_zero()); }
VECMATH_FINLINE int v_test_vec_x_gt_0(vec3f v) { return v_test_vec_x_gt(v, v_zero()); }
VECMATH_FINLINE int v_test_vec_x_ge_0(vec3f v) { return v_test_vec_x_ge(v, v_zero()); }
VECMATH_FINLINE int v_test_vec_x_lt_0(vec3f v) { return v_test_vec_x_lt(v, v_zero()); }
VECMATH_FINLINE int v_test_vec_x_le_0(vec3f v) { return v_test_vec_x_le(v, v_zero()); }
