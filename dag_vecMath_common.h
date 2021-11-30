/*
 * Dagor Engine 5
 * Copyright (C) 2003-2021  Gaijin Entertainment.  All rights reserved
 *
 * (for conditions of distribution and use, see License)
*/

#ifndef _DAGOR_PUBLIC_MATH_DAG_VECMATH_COMMON_H_
#define _DAGOR_PUBLIC_MATH_DAG_VECMATH_COMMON_H_
#pragma once


#include "dag_vecMath.h"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4800)
//we use direct conversion to bool from int
#endif

VECMATH_FINLINE vec4f VECTORCALL v_cmp_le(vec4f a, vec4f b) { return v_cmp_ge(b, a); }
VECMATH_FINLINE vec4f VECTORCALL v_cmp_lt(vec4f a, vec4f b) { return v_cmp_gt(b, a); }

VECMATH_FINLINE vec4f VECTORCALL v_clamp(vec4f t, vec4f min_val, vec4f max_val)
{
  return v_max(v_min(t, max_val), min_val);
}

VECMATH_FINLINE vec4f VECTORCALL v_safediv(vec4f a, vec4f b)
{
  return v_sel(b, v_div(a, b), v_cmp_gt(a, V_C_EPS_VAL));
}

VECMATH_FINLINE vec4f VECTORCALL v_mod(vec4f a, vec4f aDiv)
{
  vec4f c = v_div(a, aDiv);
  vec4f cTrunc = v_cvt_vec4f(v_cvt_vec4i(c));
  vec4f base = v_mul(cTrunc, aDiv);
  vec4f r = v_sub(a, base);
  return r;
}

VECMATH_FINLINE quat4f VECTORCALL v_lerp_vec4f(vec4f tttt, quat4f a, quat4f b)
{
  return v_madd(v_sub(b, a), tttt, a);
}

VECMATH_FINLINE vec4f VECTORCALL v_perm_xxxx(vec4f a) { return v_splat_x(a); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_yyyy(vec4f a) { return v_splat_y(a); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_zzzz(vec4f a) { return v_splat_z(a); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_wwww(vec4f a) { return v_splat_w(a); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_yzwx(vec4f a) { return v_rot_1(a); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_zwxy(vec4f a) { return v_rot_2(a); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_wxyz(vec4f a) { return v_rot_3(a); }

VECMATH_FINLINE vec4f VECTORCALL v_perm_zcwd(vec4f xyzw, vec4f abcd) { return v_merge_lw(xyzw, abcd); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xayb(vec4f xyzw, vec4f abcd) { return v_merge_hw(xyzw, abcd); }

#if _TARGET_SIMD_VMX
VECMATH_FINLINE vec4f VECTORCALL v_perm_xycx(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_XYCX); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xzya(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_XZYA); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xaxa(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_XAXA); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xbcx(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_XBCX); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xbzx(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_XBZX); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xzac(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_XZAC); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xzbx(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_XZBX); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_yxxc(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_YXXC); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_yxba(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_YXBA); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_ywbd(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_YWBD); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_yaxx(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_YAXX); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_ycxy(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_YCXY); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_zxxb(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_ZXXB); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_zwba(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_ZWBA); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_zayx(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_ZAYX); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_zbwa(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_ZBWA); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_wzdc(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_WZDC); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_bzxx(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_BZXX); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_bbyx(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_BBYX); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_caxx(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_CAXX); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_cxyc(vec4f xyzw, vec4f abcd) { return V_PERM(xyzw, abcd, V_PERM_CXYC); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xzxz(vec4f xyzw) { return V_PERM(xyzw, xyzw, V_PERM_XZAC); }

VECMATH_FINLINE vec4f VECTORCALL v_perm_wabc(vec4f xyzw, vec4f abcd) { return V_SHIFT(xyzw, abcd, 3); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_zwab(vec4f xyzw, vec4f abcd) { return V_SHIFT(xyzw, abcd, 2); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xxaa(vec4f xyzw, vec4f abcd) { return V_SHIFT(v_splat_x(xyzw), v_splat_x(abcd), 2); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_yybb(vec4f xyzw, vec4f abcd) { return V_SHIFT(v_splat_y(xyzw), v_splat_y(abcd), 2); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_wwdd(vec4f xyzw, vec4f abcd) { return V_SHIFT(v_splat_w(xyzw), v_splat_w(abcd), 2); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_xyab(vec4f xyzw, vec4f abcd) { return V_SHIFT(v_rot_2(xyzw), abcd, 2); }
VECMATH_FINLINE vec4f VECTORCALL v_perm_zwcd(vec4f xyzw, vec4f abcd) { return V_SHIFT(xyzw, v_rot_2(abcd), 2); }
VECMATH_FINLINE vec4f VECTORCALL
  v_perm_ayzw(vec4f xyzw, vec4f abcd) { return V_SHIFT(v_splat_x(abcd), v_rot_1(xyzw), 3); }
VECMATH_FINLINE vec4f VECTORCALL
  v_perm_xbzw(vec4f xyzw, vec4f abcd) { return V_SHIFT(v_merge_hw(v_splat_x(xyzw), abcd), v_rot_2(xyzw), 2); }
VECMATH_FINLINE vec4f VECTORCALL
  v_perm_xycw(vec4f xyzw, vec4f abcd) { return V_SHIFT(v_rot_2(xyzw), v_merge_lw(abcd, v_splat_w(xyzw)), 2); }
VECMATH_FINLINE vec4f VECTORCALL
  v_perm_xyzd(vec4f xyzw, vec4f abcd) { return V_SHIFT(v_rot_3(xyzw), v_splat_w(abcd), 1); }
VECMATH_FINLINE vec4f VECTORCALL
  v_perm_xbcd(vec4f xyzw, vec4f abcd) { return V_SHIFT(v_splat_x(xyzw), v_rot_1(abcd), 3); }
VECMATH_FINLINE vec4f VECTORCALL
  v_perm_aycd(vec4f xyzw, vec4f abcd) { return V_SHIFT(v_merge_hw(v_splat_x(abcd), xyzw), v_rot_2(abcd), 2); }
VECMATH_FINLINE vec4f VECTORCALL
  v_perm_abzd(vec4f xyzw, vec4f abcd) { return V_SHIFT(v_rot_2(abcd), v_merge_lw(xyzw, v_splat_w(abcd)), 2); }
VECMATH_FINLINE vec4f VECTORCALL
  v_perm_abcw(vec4f xyzw, vec4f abcd) { return V_SHIFT(v_rot_3(abcd), v_splat_w(xyzw), 1); }
VECMATH_FINLINE vec4f VECTORCALL
  v_perm_xycd(vec4f xyzw, vec4f abcd) { return V_SHIFT(v_rot_2(xyzw), v_rot_2(abcd), 2); }
VECMATH_FINLINE vec4f VECTORCALL
  v_perm_abzw(vec4f xyzw, vec4f abcd) { return V_SHIFT(v_rot_2(abcd), v_rot_2(xyzw), 2); }
VECMATH_FINLINE vec4f VECTORCALL
  v_perm_xbcw(vec4f xyzw, vec4f abcd)
{
  return V_SHIFT(v_merge_hw(v_splat_x(xyzw), abcd), v_merge_lw(abcd, v_splat_w(xyzw)), 2);
}
#endif


#if !_TARGET_SIMD_NEON
VECMATH_FINLINE vec4f VECTORCALL v_length4_sq(vec4f a) { return v_dot4(a,a); }
VECMATH_FINLINE vec3f VECTORCALL v_length3_sq(vec3f a) { return v_dot3(a,a); }
VECMATH_FINLINE vec4f VECTORCALL v_length4_sq_x(vec4f a) { return v_dot4_x(a,a); }
VECMATH_FINLINE vec3f VECTORCALL v_length3_sq_x(vec3f a) { return v_dot3_x(a,a); }
#endif

VECMATH_FINLINE vec4f VECTORCALL v_length4(vec4f a) { return v_sqrt4(v_length4_sq(a)); }
VECMATH_FINLINE vec3f VECTORCALL v_length3(vec3f a) { return v_sqrt4(v_length3_sq(a)); }
VECMATH_FINLINE vec4f VECTORCALL v_length4_est(vec4f a) { return v_sqrt4_fast(v_length4_sq(a)); }
VECMATH_FINLINE vec3f VECTORCALL v_length3_est(vec3f a) { return v_sqrt4_fast(v_length3_sq(a)); }
VECMATH_FINLINE vec4f VECTORCALL v_length4_x(vec4f a) { return v_sqrt_x(v_length4_sq_x(a)); }
VECMATH_FINLINE vec3f VECTORCALL v_length3_x(vec3f a) { return v_sqrt_x(v_length3_sq_x(a)); }
VECMATH_FINLINE vec4f VECTORCALL v_length4_est_x(vec4f a) { return v_sqrt_fast_x(v_length4_sq_x(a)); }
VECMATH_FINLINE vec3f VECTORCALL v_length3_est_x(vec3f a) { return v_sqrt_fast_x(v_length3_sq_x(a)); }

VECMATH_FINLINE vec3f VECTORCALL v_striple3(vec3f a, vec3f b, vec3f c) { return v_dot3(v_cross3(a, b), c); }
VECMATH_FINLINE vec3f VECTORCALL v_vtriple3(vec3f a, vec3f b, vec3f c)
{
  vec3f ac = v_dot3(a, c);
  vec3f ab = v_dot3(a, b);
  return v_nmsub(c, ab, v_mul(b, ac));
}

VECMATH_FINLINE void VECTORCALL v_sincos4(vec4f x, vec4f& s, vec4f& c)
{
  vec4f xl, xl2, xl3;
  vec4i q;
  vec4i offsetSin, offsetCos;
  vec4f vzero = v_zero();

  xl = v_mul(x, v_splats(0.63661977236f));
  xl = v_add(xl, v_sel(V_C_HALF, x, v_msbit()));

  q = v_cvt_vec4i(xl);

  offsetSin = v_andi(q, V_CI_3);
  offsetCos = v_addi(V_CI_1, offsetSin);

  vec4f qf = v_cvt_vec4f(q);
  vec4f p1 = v_nmsub(qf, v_splats(_SINCOS_KC1), x);
  xl  = v_nmsub(qf, v_splats(_SINCOS_KC2), p1);

  xl2 = v_mul(xl, xl);
  xl3 = v_mul(xl2, xl);


  vec4f ct1 = v_madd(v_splats(_SINCOS_CC0), xl2, v_splats(_SINCOS_CC1));
  vec4f st1 = v_madd(v_splats(_SINCOS_SC0), xl2, v_splats(_SINCOS_SC1));

  vec4f ct2 = v_madd(ct1, xl2, v_splats(_SINCOS_CC2));
  vec4f st2 = v_madd(st1, xl2, v_splats(_SINCOS_SC2));

  vec4f cx = v_madd(ct2, xl2, V_C_ONE);
  vec4f sx = v_madd(st2, xl3, xl);

  vec4f sinMask = v_cmp_eq_w(v_cast_vec4f(v_andi(offsetSin, V_CI_1)), vzero);
  vec4f cosMask = v_cmp_eq_w(v_cast_vec4f(v_andi(offsetCos, V_CI_1)), vzero);
  s = v_sel(cx, sx, sinMask);
  c = v_sel(cx, sx, cosMask);

  sinMask = v_cmp_eq_w(v_cast_vec4f(v_andi(offsetSin, V_CI_2)), vzero);
  cosMask = v_cmp_eq_w(v_cast_vec4f(v_andi(offsetCos, V_CI_2)), vzero);

  s = v_sel(v_neg(s), s, sinMask);
  c = v_sel(v_neg(c), c, cosMask);
}
VECMATH_FINLINE void VECTORCALL v_sincos_x(vec4f a, vec4f& s, vec4f& c) { return v_sincos4(a, s, c); }

VECMATH_FINLINE void VECTORCALL v_mat44_make_persp_forward(mat44f &dest, float wk, float hk, float zn, float zf)
{
  float q = zf/(zf-zn);
  dest.col0 = v_make_vec4f(wk, 0, 0, 0);
  dest.col1 = v_make_vec4f(0, hk, 0, 0);
  dest.col2 = v_make_vec4f(0, 0, q, 1.f);
  dest.col3 = v_make_vec4f(0, 0, -q*zn, 0);
}

VECMATH_FINLINE void VECTORCALL v_mat44_make_persp_reverse(mat44f &dest, float wk, float hk, float zn, float zf)
{
  dest.col0 = v_make_vec4f(wk, 0, 0, 0);
  dest.col1 = v_make_vec4f(0, hk, 0, 0);
  dest.col2 = v_make_vec4f(0, 0, zn/(zn-zf), 1.f);
  dest.col3 = v_make_vec4f(0, 0, (zn*zf)/(zf-zn), 0);
}

VECMATH_FINLINE void VECTORCALL v_mat44_make_persp(mat44f &dest, float wk, float hk, float zn, float zf)
{
  return v_mat44_make_persp_reverse(dest, wk, hk, zn, zf);
}


#if !_TARGET_SIMD_SSE
VECMATH_FINLINE void VECTORCALL v_mat33_transpose(mat33f &dest, vec3f col0, vec3f col1, vec3f col2)
{
  vec4f tmp0, tmp1, tmp2, tmp3;
  tmp0 = v_merge_hw(col0, col2);
  tmp1 = v_merge_hw(col1, v_zero());//may be v_zero can be replaced with col2
  tmp2 = v_merge_lw(col0, col2);
  tmp3 = v_merge_lw(col1, v_zero());//may be v_zero can be replaced with col2
  dest.col0 = v_merge_hw(tmp0, tmp1);
  dest.col1 = v_merge_lw(tmp0, tmp1);
  dest.col2 = v_merge_hw(tmp2, tmp3);
}

VECMATH_FINLINE void VECTORCALL v_mat44_transpose_to_mat33(mat33f &dest, vec3f col0, vec3f col1, vec3f col2, vec3f col3)
{
  vec4f tmp0, tmp1, tmp2, tmp3;
  tmp0 = v_merge_hw(col0, col2);
  tmp1 = v_merge_hw(col1, col3);
  tmp2 = v_merge_lw(col0, col2);
  tmp3 = v_merge_lw(col1, col3);
  dest.col0 = v_merge_hw(tmp0, tmp1);
  dest.col1 = v_merge_lw(tmp0, tmp1);
  dest.col2 = v_merge_hw(tmp2, tmp3);
}
#endif

#if defined(__FINITE_MATH_ONLY__) && __FINITE_MATH_ONLY__ > 0 // Clang/GCC
VECMATH_FINLINE vec4f VECTORCALL v_remove_nan(vec4f a)
{
  static vec4i_const nanMask = { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 };
  vec4i ai = v_cast_vec4i(a);
  return v_cast_vec4f(v_andi(ai, v_cmp_lti(v_andi(ai, nanMask), nanMask)));
}
#else
VECMATH_FINLINE vec4f VECTORCALL v_remove_nan(vec4f a) {return v_and(a, v_cmp_eq(a,a)); }
#endif
VECMATH_FINLINE vec4f VECTORCALL v_norm4_safe(vec4f a) {return v_remove_nan(v_norm4(a));}
VECMATH_FINLINE vec4f VECTORCALL v_norm3_safe(vec4f a) {return v_remove_nan(v_norm3(a));}

VECMATH_FINLINE void VECTORCALL v_mat33_transpose(mat33f &dest, mat33f_cref src)
{
  v_mat33_transpose(dest, src.col0, src.col1, src.col2);
}

VECMATH_FINLINE void VECTORCALL v_mat33_make_from_look(mat33f &dest, vec4f look_dir)
{
  vec4f vx = v_cross3(V_C_UNIT_0100, look_dir);
  vec4f vxlsq = v_length3_sq(vx);

#if _TARGET_SIMD_SSE
  if (v_extract_x(vxlsq) < 1.42e-12F)
    vx = v_norm3(v_cross3(v_sel(V_C_UNIT_0010, look_dir, v_msbit()), look_dir));
  else
    vx = v_mul(vx, v_rsqrt4(vxlsq));
#else
  vec4f vx1 = v_norm3(v_cross3(v_sel(V_C_UNIT_0010, look_dir, v_msbit()), look_dir));
  vec4f vx2 = v_mul(vx, v_rsqrt4(vxlsq));
  vx = v_sel(vx1, vx2, v_cmp_gt(vxlsq, v_zero()));
#endif

  dest.col0 = vx;
  dest.col1 = v_norm3(v_cross3(look_dir, vx));
  dest.col2 = look_dir;
}

VECMATH_INLINE void VECTORCALL v_view_matrix_from_tangentZ( vec3f &left, vec3f &up, vec4f vdir )
{
  up = v_sel(V_C_UNIT_0010, V_C_UNIT_1000, v_cmp_gt(v_abs(v_splat_z(vdir)), v_splats(0.999f)));
  left = v_norm3(v_cross3(up, vdir));
  up = v_cross3(vdir, left);
}


VECMATH_FINLINE void VECTORCALL v_mat44_make33_from_look(mat44f &dest, vec4f look_dir)
{
  mat33f m;
  v_mat33_make_from_look(m, look_dir);
  dest.set33(m);
}
VECMATH_FINLINE void VECTORCALL v_mat44_make_look_at(mat44f &dest, vec4f eye, vec4f at, vec4f up)
{
  dest.col2 = v_norm3(v_sub(at, eye));
  dest.col0 = v_norm3(v_cross3(up, dest.col2));
  dest.col1 = v_cross3(dest.col2, dest.col0);
  dest.col3 = eye;
  v_mat44_orthonormal_inverse43(dest, dest);
}
VECMATH_FINLINE void VECTORCALL v_mat44_make_ortho(mat44f &dest, float _w, float _h, float _zn, float _zf)
{
  v_mat44_ident(dest);
  vec4f zn = v_splat4(&_zn);
  vec4f w = v_splat4(&_w);
  vec4f h = v_splat4(&_h);
  vec4f r_zr = v_rcp(v_sub(v_splat4(&_zf), zn));

  dest.col2 = v_mul(dest.col2, r_zr);
  dest.col0 = v_mul(v_add(dest.col0, dest.col0), v_rcp(w));
  dest.col1 = v_mul(v_add(dest.col1, dest.col1), v_rcp(h));
  dest.col3 = v_madd(zn, dest.col2, dest.col3);
}

VECMATH_FINLINE void VECTORCALL v_mat44_add(mat44f &dest, mat44f_cref m1, mat44f_cref m2)
{
  dest.col0 = v_add(m1.col0, m2.col0);
  dest.col1 = v_add(m1.col1, m2.col1);
  dest.col2 = v_add(m1.col2, m2.col2);
  dest.col3 = v_add(m1.col3, m2.col3);
}
VECMATH_FINLINE void VECTORCALL v_mat33_add(mat33f &dest, mat33f_cref m1, mat33f_cref m2)
{
  dest.col0 = v_add(m1.col0, m2.col0);
  dest.col1 = v_add(m1.col1, m2.col1);
  dest.col2 = v_add(m1.col2, m2.col2);
}
VECMATH_FINLINE void VECTORCALL v_mat43_sub(mat43f &dest, mat43f_cref m1, mat43f_cref m2)
{
  dest.row0 = v_sub(m1.row0, m2.row0);
  dest.row1 = v_sub(m1.row1, m2.row1);
  dest.row2 = v_sub(m1.row2, m2.row2);
}
VECMATH_FINLINE void VECTORCALL v_mat44_sub(mat44f &dest, mat44f_cref m1, mat44f_cref m2)
{
  dest.col0 = v_sub(m1.col0, m2.col0);
  dest.col1 = v_sub(m1.col1, m2.col1);
  dest.col2 = v_sub(m1.col2, m2.col2);
  dest.col3 = v_sub(m1.col3, m2.col3);
}
VECMATH_FINLINE void VECTORCALL v_mat33_sub(mat33f &dest, mat33f_cref m1, mat33f_cref m2)
{
  dest.col0 = v_sub(m1.col0, m2.col0);
  dest.col1 = v_sub(m1.col1, m2.col1);
  dest.col2 = v_sub(m1.col2, m2.col2);
}

VECMATH_FINLINE void VECTORCALL v_mat33_from_mat44(mat33f &dest, mat44f_cref m2)
{
  dest.col0 = m2.col0;
  dest.col1 = m2.col1;
  dest.col2 = m2.col2;
}

VECMATH_FINLINE void VECTORCALL v_mat33_neg(mat44f &dest, mat44f_cref m)
{
  vec4f zero = v_zero();
  dest.col0 = v_sub(zero, m.col0);
  dest.col1 = v_sub(zero, m.col1);
  dest.col2 = v_sub(zero, m.col2);
  dest.col3 = v_sub(zero, m.col3);
}
VECMATH_FINLINE void VECTORCALL v_mat33_neg(mat33f &dest, mat33f_cref m)
{
  vec4f zero = v_zero();
  dest.col0 = v_sub(zero, m.col0);
  dest.col1 = v_sub(zero, m.col1);
  dest.col2 = v_sub(zero, m.col2);
}
VECMATH_FINLINE void VECTORCALL v_mat44_mul_elem(mat44f &dest, mat44f_cref m1, mat44f_cref m2)
{
  dest.col0 = v_mul(m1.col0, m2.col0);
  dest.col1 = v_mul(m1.col1, m2.col1);
  dest.col2 = v_mul(m1.col2, m2.col2);
  dest.col3 = v_mul(m1.col3, m2.col3);
}
VECMATH_FINLINE void VECTORCALL v_mat33_mul_elem(mat33f &dest, mat33f_cref m1, mat33f_cref m2)
{
  dest.col0 = v_mul(m1.col0, m2.col0);
  dest.col1 = v_mul(m1.col1, m2.col1);
  dest.col2 = v_mul(m1.col2, m2.col2);
}

VECMATH_FINLINE vec3f VECTORCALL v_mat43_mul_vec3v(mat43f_cref m, vec3f v)
{
  mat44f m44;
  v_mat43_transpose_to_mat44(m44, m);
  return v_mat44_mul_vec3v(m44, v);
}

VECMATH_FINLINE vec3f VECTORCALL v_mat43_mul_vec3p(mat43f_cref m, vec3f v)
{
  mat44f m44;
  v_mat43_transpose_to_mat44(m44, m);
  return v_mat44_mul_vec3p(m44, v);
}

VECMATH_FINLINE void VECTORCALL v_mat44_mul(mat44f &dest, mat44f_cref m1, mat44f_cref m2)
{
  vec4f col0 = v_mat44_mul_vec4(m1, m2.col0);
  vec4f col1 = v_mat44_mul_vec4(m1, m2.col1);
  vec4f col2 = v_mat44_mul_vec4(m1, m2.col2);
  vec4f col3 = v_mat44_mul_vec4(m1, m2.col3);
  dest.col0 = col0;
  dest.col1 = col1;
  dest.col2 = col2;
  dest.col3 = col3;
}
VECMATH_FINLINE void VECTORCALL v_mat44_mul43(mat44f &dest, mat44f_cref m1, mat44f_cref m2)
{
  vec4f col0 = v_mat44_mul_vec3v(m1, m2.col0);
  vec4f col1 = v_mat44_mul_vec3v(m1, m2.col1);
  vec4f col2 = v_mat44_mul_vec3v(m1, m2.col2);
  vec4f col3 = v_mat44_mul_vec3p(m1, m2.col3);
  dest.col0 = col0;
  dest.col1 = col1;
  dest.col2 = col2;
  dest.col3 = col3;
}

VECMATH_FINLINE void VECTORCALL v_mat44_mul43(mat44f &dest, mat44f_cref m1, mat43f_cref m2)
{
  vec4f xxxx, yyyy, zzzz;
  #define SPLAT_MAT_N_COL0(N, COL) \
    xxxx = v_splat_##N(m2.row0); yyyy = v_splat_##N(m2.row1); zzzz = v_splat_##N(m2.row2);\
    vec4f COL = v_add(v_add(v_mul(xxxx, m1.col0), v_mul(yyyy, m1.col1)), v_mul(zzzz, m1.col2));

  #define SPLAT_MAT_N_COL1(N, COL) \
    xxxx = v_splat_##N(m2.row0); yyyy = v_splat_##N(m2.row1); zzzz = v_splat_##N(m2.row2);\
    vec4f COL = v_add(v_add(v_mul(xxxx, m1.col0), v_mul(yyyy, m1.col1)), v_add(v_mul(zzzz, m1.col2), m1.col3));

  SPLAT_MAT_N_COL0(x, col0)
  SPLAT_MAT_N_COL0(y, col1)
  SPLAT_MAT_N_COL0(z, col2)
  SPLAT_MAT_N_COL1(w, col3)
  dest.col0 = col0;
  dest.col1 = col1;
  dest.col2 = col2;
  dest.col3 = col3;
  #undef SPLAT_MAT_N_COL0
  #undef SPLAT_MAT_N_COL1
}

VECMATH_FINLINE void VECTORCALL v_mat44_mul33r(mat44f &dest, mat44f_cref m1, mat44f_cref m2)
{
  vec4f col0 = v_mat44_mul_vec3v(m1, m2.col0);
  vec4f col1 = v_mat44_mul_vec3v(m1, m2.col1);
  vec4f col2 = v_mat44_mul_vec3v(m1, m2.col2);
  dest.col0 = col0;
  dest.col1 = col1;
  dest.col2 = col2;
  dest.col3 = m1.col3;
}
VECMATH_FINLINE void VECTORCALL v_mat44_mul33(mat44f &dest, mat44f_cref m1, mat33f_cref m2)
{
  vec4f col0 = v_mat44_mul_vec3v(m1, m2.col0);
  vec4f col1 = v_mat44_mul_vec3v(m1, m2.col1);
  vec4f col2 = v_mat44_mul_vec3v(m1, m2.col2);
  dest.col0 = col0;
  dest.col1 = col1;
  dest.col2 = col2;
  dest.col3 = m1.col3;
}
VECMATH_FINLINE void VECTORCALL v_mat33_mul(mat33f &dest, mat33f_cref m1, mat33f_cref m2)
{
  vec4f col0 = v_mat33_mul_vec3(m1, m2.col0);
  vec4f col1 = v_mat33_mul_vec3(m1, m2.col1);
  vec4f col2 = v_mat33_mul_vec3(m1, m2.col2);
  dest.col0 = col0;
  dest.col1 = col1;
  dest.col2 = col2;
}
VECMATH_FINLINE void VECTORCALL v_mat33_mul33r(mat33f &dest, mat33f_cref m1, mat44f_cref m2)
{
  vec4f col0 = v_mat33_mul_vec3(m1, m2.col0);
  vec4f col1 = v_mat33_mul_vec3(m1, m2.col1);
  vec4f col2 = v_mat33_mul_vec3(m1, m2.col2);
  dest.col0 = col0;
  dest.col1 = col1;
  dest.col2 = col2;
}
VECMATH_FINLINE void VECTORCALL v_mat33_orthonormalize(mat33f &dest, mat33f_cref m)
{
  vec4f c2 = v_norm3(v_cross3(m.col0, m.col1));
  vec4f c1 = v_norm3(v_cross3(c2, m.col0));
  vec4f c0 = v_norm3(v_cross3(c1, c2));
  dest.col2 = c2;
  dest.col1 = c1;
  dest.col0 = c0;
}
VECMATH_FINLINE void VECTORCALL v_mat44_orthonormalize33(mat44f &dest, mat44f_cref m)
{
  vec4f c2 = v_norm3(v_cross3(m.col0, m.col1));
  vec4f c1 = v_norm3(v_cross3(c2, m.col0));
  vec4f c0 = v_norm3(v_cross3(c1, c2));
  dest.col2 = c2;
  dest.col1 = c1;
  dest.col0 = c0;
}
VECMATH_FINLINE void VECTORCALL v_mat33_orthonormal_inverse(mat33f &dest, mat33f_cref m)
{
  v_mat33_transpose(dest, m);
}
VECMATH_FINLINE void VECTORCALL v_mat44_orthonormal_inverse43(mat44f &dest, mat44f_cref m)
{
  mat33f m3;
  v_mat44_transpose_to_mat33(m3, m.col0, m.col1, m.col2, v_zero());
  dest.set33(m3);
  dest.col3 = v_sub(V_C_UNIT_0001, v_mat44_mul_vec3v(dest, m.col3));
}

VECMATH_FINLINE vec4f VECTORCALL v_mat33_det(mat33f_cref m)
{
  return v_dot3(m.col2, v_cross3(m.col0, m.col1));
}
VECMATH_FINLINE void VECTORCALL v_mat44_inverse43(mat44f &dest, mat44f_cref m)
{
  mat33f m3;
  m3.col0 = m.col0;
  m3.col1 = m.col1;
  m3.col2 = m.col2;
  v_mat33_inverse(m3, m3);
  dest.set33(m3);
  dest.col3 = v_neg(v_mat44_mul_vec3v(dest, m.col3));
}
VECMATH_FINLINE vec4f VECTORCALL v_mat44_det43(mat44f_cref m)
{
  return v_dot3(m.col2, v_cross3(m.col0, m.col1));
}

VECMATH_FINLINE void VECTORCALL v_bbox3_init_empty(bbox3f &b)
{
  b.bmin = V_C_MAX_VAL;
  b.bmax = v_sub(v_zero(), b.bmin);
}
VECMATH_FINLINE void VECTORCALL v_bbox3_init(bbox3f &b, vec3f p) { b.bmin = b.bmax = p; }
VECMATH_FINLINE void VECTORCALL v_bbox3_init(bbox3f &b, mat44f_cref m, bbox3f_cref b2)
{
  // What we're doing here is this:
  // xxxx*m0 + yyyy*m1 + zzzz*m2 + m3
  // xxxx*m0 + yyyy*m1 + ZZZZ*m2 + m3
  // xxxx*m0 + YYYY*m1 + zzzz*m2 + m3
  // xxxx*m0 + YYYY*m1 + ZZZZ*m2 + m3
  // XXXX*m0 + yyyy*m1 + zzzz*m2 + m3
  // XXXX*m0 + yyyy*m1 + ZZZZ*m2 + m3
  // XXXX*m0 + YYYY*m1 + zzzz*m2 + m3
  // XXXX*m0 + YYYY*m1 + ZZZZ*m2 + m3
  // Which we don't need to do at all as we just need to calculate 1/4 of this first and then summ up
  vec4f boxMulM_0_0 = v_mul(v_splat_x(b2.bmin), m.col0);
  vec4f boxMulM_0_1 = v_mul(v_splat_x(b2.bmax), m.col0);
  vec4f boxMulM_1_0 = v_mul(v_splat_y(b2.bmin), m.col1);
  vec4f boxMulM_1_1 = v_mul(v_splat_y(b2.bmax), m.col1);
  vec4f boxMulM_2_0 = v_mul(v_splat_z(b2.bmin), m.col2);
  vec4f boxMulM_2_1 = v_mul(v_splat_z(b2.bmax), m.col2);

  // Summing y and z
  vec4f boxSum_0_0 = v_add(boxMulM_1_0, boxMulM_2_0);
  vec4f boxSum_0_1 = v_add(boxMulM_1_0, boxMulM_2_1);
  vec4f boxSum_1_0 = v_add(boxMulM_1_1, boxMulM_2_0);
  vec4f boxSum_1_1 = v_add(boxMulM_1_1, boxMulM_2_1);
#define COMBINE_BOX(a,b,c) v_add(boxMulM_0_##a, boxSum_##b##_##c)
  v_bbox3_init(  b, COMBINE_BOX(0, 0, 0));
  v_bbox3_add_pt(b, COMBINE_BOX(0, 0, 1));
  v_bbox3_add_pt(b, COMBINE_BOX(0, 1, 0));
  v_bbox3_add_pt(b, COMBINE_BOX(0, 1, 1));
  v_bbox3_add_pt(b, COMBINE_BOX(1, 0, 0));
  v_bbox3_add_pt(b, COMBINE_BOX(1, 0, 1));
  v_bbox3_add_pt(b, COMBINE_BOX(1, 1, 0));
  v_bbox3_add_pt(b, COMBINE_BOX(1, 1, 1));
#undef COMBINE_BOX
  b.bmin = v_add(b.bmin, m.col3);
  b.bmax = v_add(b.bmax, m.col3);
}

//return all mask if empty
VECMATH_FINLINE vec4f VECTORCALL v_bbox_isempty(bbox3f_cref bbox) {return v_cmp_gt(bbox.bmin, bbox.bmax);}

VECMATH_FINLINE void VECTORCALL v_bbox3_add_pt(bbox3f &b, vec3f p)
{
  b.bmin = v_min(b.bmin, p);
  b.bmax = v_max(b.bmax, p);
}
VECMATH_FINLINE void VECTORCALL v_bbox3_add_box(bbox3f &b, bbox3f_cref b2)
{
  b.bmin = v_min(b.bmin, b2.bmin);
  b.bmax = v_max(b.bmax, b2.bmax);
}

VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt000(bbox3f_cref b) { return b.bmin; }
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt111(bbox3f_cref b) { return b.bmax; }

VECMATH_FINLINE void VECTORCALL v_bbox3_add_transformed_box(bbox3f &b, mat44f_cref m, bbox3f_cref b2)
{
  bbox3f temp;
  v_bbox3_init(temp, m, b2);
  v_bbox3_add_box(b, temp);
}

VECMATH_FINLINE vec3f VECTORCALL v_bbox3_size(bbox3f_cref b) { return v_sub(b.bmax, b.bmin); }
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_test_pt_inside(bbox3f_cref b, vec3f p)
{
  vec3f m = v_and(v_cmp_ge(p, b.bmin), v_cmp_ge(b.bmax, p));
  return v_and(v_splat_x(m), v_and(v_splat_y(m), v_splat_z(m)));
}
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_test_box_inside(bbox3f_cref b, bbox3f_cref b2)
{
  return v_and(v_bbox3_test_pt_inside(b, b2.bmin), v_bbox3_test_pt_inside(b, b2.bmax));
}
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_test_box_intersect(bbox3f_cref b1, bbox3f_cref b2)
{
  vec3f m = v_and(v_cmp_ge(b2.bmax, b1.bmin), v_cmp_ge(b1.bmax, b2.bmin));
  return v_and(v_splat_x(m), v_and(v_splat_y(m), v_splat_z(m)));
}

//any box can be empty and full world
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_test_box_intersect_safe(bbox3f_cref b1, bbox3f_cref b2)
{
  vec3f m = v_and(v_and(v_cmp_gt(b1.bmax, b1.bmin), v_cmp_gt(b2.bmax, b2.bmin)),
                  v_and(v_cmp_ge(b2.bmax, b1.bmin), v_cmp_ge(b1.bmax, b2.bmin)));
  return v_and(v_splat_x(m), v_and(v_splat_y(m), v_splat_z(m)));
}

VECMATH_FINLINE int VECTORCALL v_bbox3_test_sph_intersect(bbox3f_cref box, vec4f bsph_r2)
{
  vec4f distSq = v_length3_sq_x(v_add(v_max(v_sub(box.bmin, bsph_r2), v_zero()),
                                      v_max(v_sub(bsph_r2, box.bmax), v_zero()))); // Dist from sph center to bounding box squared
  return v_test_vec_x_le_0(v_sub_x(distSq, v_splat_w(bsph_r2)));
}

VECMATH_FINLINE vec4f VECTORCALL v_bbox3_outer_rad(bbox3f_cref b) { return v_bbox3_outer_rad(b.bmin, b.bmax); }
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_inner_rad(bbox3f_cref b) { return v_bbox3_inner_rad(b.bmin, b.bmax); }

VECMATH_FINLINE bool VECTORCALL v_bbox3_is_empty(bbox3f_cref bbox)
{
  vec3f boxSize = v_bbox3_size(bbox);
  vec3f boxValid = v_min(boxSize, v_min(v_rot_1(boxSize), v_rot_2(boxSize)));
  return v_test_vec_x_le_0(boxValid);
}

VECMATH_FINLINE quat4f VECTORCALL v_quat_lerp(vec4f tttt, quat4f a, quat4f b)
{
  return v_lerp_vec4f(tttt, a, b);
}

VECMATH_FINLINE quat4f VECTORCALL v_quat_conjugate(quat4f q) { return v_perm_xyzd(v_neg(q), q); }

VECMATH_FINLINE vec3f VECTORCALL v_quat_mul_vec3(quat4f q, vec3f v)
{
  vec3f t = v_cross3(q, v);
  t = v_add(t,t);//*2
  return v_add(v, v_madd(v_splat_w(q), t, v_cross3(q, t)));//v + q.w * t + v_cross3(q.xyz, t);
}

#if _TARGET_SIMD_VMX|_TARGET_SIMD_SPU|_TARGET_SIMD_SSE
VECMATH_FINLINE quat4f VECTORCALL v_un_quat_from_mat(vec3f col0, vec3f col1, vec3f col2)
{
  vec4f xx_yy, xx_yy_zz_xx, yy_zz_xx_yy, zz_xx_yy_zz, diagSum, diagDiff;
  vec4f zy_xz_yx, yz_zx_xy, sum, diff;
  vec4f radicand, invSqrt, scale;
  vec4f res0, res1, res2, res3;
  vec4f xx, yy, zz;
  quat4f res;

  xx_yy = v_perm_xbzw(col0, col1);
#if _TARGET_SIMD_VMX|_TARGET_SIMD_SPU
  xx_yy_zz_xx = v_perm_xycx(xx_yy, col2);
  yy_zz_xx_yy = v_perm_ycxy(xx_yy, col2);
  zz_xx_yy_zz = v_perm_cxyc(xx_yy, col2);
#elif _TARGET_SIMD_SSE
  xx_yy_zz_xx = v_perm_xycw(_mm_shuffle_ps(xx_yy, xx_yy, _MM_SHUFFLE(0,0,1,0)), col2);
  yy_zz_xx_yy = _mm_shuffle_ps(xx_yy_zz_xx, xx_yy_zz_xx, _MM_SHUFFLE(1,0,2,1));
  zz_xx_yy_zz = _mm_shuffle_ps(xx_yy_zz_xx, xx_yy_zz_xx, _MM_SHUFFLE(2,1,0,2));
#endif

  diagSum = v_add(v_add(xx_yy_zz_xx, yy_zz_xx_yy), zz_xx_yy_zz );
  diagDiff = v_sub(v_sub(xx_yy_zz_xx, yy_zz_xx_yy), zz_xx_yy_zz );
  radicand = v_add(v_perm_xyzd(diagDiff, diagSum), V_C_ONE);
  invSqrt = v_rsqrt4(v_sel(radicand, V_C_ONE, v_cmp_ge(v_zero(), radicand)));

  zy_xz_yx = v_perm_xycw(col0, col1);
  zy_xz_yx = v_perm_zayx(zy_xz_yx, col2);
  yz_zx_xy = v_perm_ayzw(col0, col1);
  yz_zx_xy = v_perm_bzxx(yz_zx_xy, col2);

  sum = v_add(zy_xz_yx, yz_zx_xy);
  diff = v_sub(zy_xz_yx, yz_zx_xy);

  scale = v_mul(invSqrt, V_C_HALF);
  res0 = v_perm_xzya(sum, diff);
  res1 = v_perm_zxxb(sum, diff);
  res2 = v_perm_yxxc(sum, diff);
  res3 = diff;
  res0 = v_perm_ayzw(res0, radicand);
  res1 = v_perm_xbzw(res1, radicand);
  res2 = v_perm_xycw(res2, radicand);
  res3 = v_perm_xyzd(res3, radicand);
  res0 = v_mul( res0, v_splat_x(scale));
  res1 = v_mul( res1, v_splat_y(scale));
  res2 = v_mul( res2, v_splat_z(scale));
  res3 = v_mul( res3, v_splat_w(scale));

  xx = v_splat_x(col0);
  yy = v_splat_y(col1);
  zz = v_splat_z(col2);
  res = v_sel(res0, res1, v_cmp_gt(yy, xx));
  res = v_sel(res, res2, v_and(v_cmp_gt(zz, xx), v_cmp_gt(zz, yy)));
  return v_sel(res, res3, v_cmp_gt(v_splat_x(diagSum), v_zero()));
}
#endif
VECMATH_FINLINE quat4f VECTORCALL v_un_quat_from_mat3(mat33f_cref m)
{
  return v_un_quat_from_mat(m.col0, m.col1, m.col2);
}
VECMATH_FINLINE quat4f VECTORCALL v_un_quat_from_mat4(mat44f_cref m)
{
  return v_un_quat_from_mat(m.col0, m.col1, m.col2);
}

//! make quaternion to rotate 'v0' to 'v1'; v0 and v1 must be normalized
VECMATH_FINLINE quat4f VECTORCALL v_un_quat_from_unit_arc(vec3f v0, vec3f v1)
{
  vec4f cosAngle = v_dot3(v0, v1);
  vec4f cosAngleX2Plus2 = v_madd_x(cosAngle, V_C_TWO, V_C_TWO);
  if (v_extract_x(cosAngleX2Plus2) > 1e-4)
  {
    vec3f crossVec = v_cross3(v0, v1);
    vec4f recipCosHalfAngleX2 = v_rsqrt_x(cosAngleX2Plus2);
    vec4f cosHalfAngleX2 = v_mul_x(recipCosHalfAngleX2, cosAngleX2Plus2);
    return v_perm_xyzd(
      v_mul(crossVec, v_splat_x(recipCosHalfAngleX2)),
      v_splat_x(v_mul_x(cosHalfAngleX2, V_C_HALF)));
  }
  // slow path for opposite vectors
  if (v_test_vec_x_eq_0(v0))
    return v_perm_xzbx(v_mul(v0, V_C_UNIT_0010), v_neg(v0));
  return v_perm_yaxx(v_mul(v0, V_C_UNIT_0100), v_neg(v0));
}

//! make quaternion to rotate 'v0' to 'v1'
VECMATH_FINLINE quat4f VECTORCALL v_un_quat_from_arc(vec3f v0, vec3f v1)
{
  vec4f inv_len_product = v_rsqrt_x(v_mul_x(v_length3_sq_x(v0), v_length3_sq_x(v1)));
  vec4f cosAngle = v_mul_x(v_dot3(v0, v1), inv_len_product);
  vec4f cosAngleX2Plus2 = v_madd_x(cosAngle, V_C_TWO, V_C_TWO);
  if (v_extract_x(cosAngleX2Plus2) > 1e-4)
  {
    vec3f crossVec = v_cross3(v0, v1);
    vec4f recipCosHalfAngleX2 = v_rsqrt_x(cosAngleX2Plus2);
    vec4f cosHalfAngleX2 = v_mul_x(recipCosHalfAngleX2, cosAngleX2Plus2);
    return v_perm_xyzd(
      v_mul(crossVec, v_splat_x(v_mul_x(recipCosHalfAngleX2, inv_len_product))),
      v_splat_x(v_mul_x(cosHalfAngleX2, V_C_HALF)));
  }
  // slow path for opposite vectors
  if (v_test_vec_x_eq_0(v0))
    return v_perm_xzbx(v_mul(v0, V_C_UNIT_0010), v_neg(v0));
  return v_perm_yaxx(v_mul(v0, V_C_UNIT_0100), v_neg(v0));
}

//! make quaternion to rotate 'ang' radians around 'v' vector; v must be normalized
VECMATH_FINLINE quat4f VECTORCALL v_un_quat_from_unit_vec_ang(vec3f v, vec4f ang)
{
  vec4f s, c;
  v_sincos_x(v_mul(ang, V_C_HALF), s, c);
  return v_perm_xyzd(v_mul(v, s), c);
}


VECMATH_FINLINE quat4f VECTORCALL v_quat_qslerp(float t, quat4f l, quat4f r)
{
  float ca = v_extract_x(v_dot4_x(l, r));
  //todo: vectorize
  float d = fabsf(ca);
  float k = 0.931872f + d * (-1.25654f + d * 0.331442f);
  float ot = t + t * (t - 0.5f) * (t - 1) * k;

  float lt = 1 - ot;
  float rt = ca > 0 ? ot : -ot;

  return v_norm4(v_add(v_mul(l, v_splats(lt)), v_mul(r, v_splats(rt))));
}

VECMATH_FINLINE quat4f VECTORCALL v_quat_qsquad(float t,
  const quat4f &q0, const quat4f &q1, const quat4f &q2, const quat4f &q3)
{
  quat4f tmp0, tmp1;
  tmp0 = v_quat_qslerp(t, q0, q3);
  tmp1 = v_quat_qslerp(t, q1, q2);
  return v_quat_qslerp(2*t*(1-t), tmp0, tmp1);
}

VECMATH_FINLINE quat4f VECTORCALL v_quat_mul_quat(quat4f q1, quat4f q2)
{
  vec4f qv, tmp0, tmp1, tmp2, tmp3;
  vec4f product, l_wxyz, r_wxyz, xy, qw;
  tmp0 = v_perm_yzxw(q1);
  tmp1 = v_perm_zxyw(q2);
  tmp2 = v_perm_zxyw(q1);
  tmp3 = v_perm_yzxw(q2);
  qv = v_mul(v_splat_w(q1), q2);
  qv = v_madd(v_splat_w(q2), q1, qv);
  qv = v_madd(tmp0, tmp1, qv);
  qv = v_nmsub(tmp2, tmp3, qv);
  product = v_mul(q1, q2);
  l_wxyz = v_perm_wxyz(q1);
  r_wxyz = v_perm_wxyz(q2);
  qw = v_nmsub(l_wxyz, r_wxyz, product);
  xy = v_madd(l_wxyz, r_wxyz, product);
  qw = v_sub(qw, v_perm_zwxy(xy));
  return v_perm_xyzd(qv, qw);
}


//! make 3x3 rotation matrix from quaternion
VECMATH_FINLINE void VECTORCALL v_mat33_from_quat(mat33f &dest, quat4f rot)
{
  vec4f xyzw_2, wwww, yzxw, zxyw, yzxw_2, zxyw_2;
  vec4f tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;

  xyzw_2 = v_add(rot, rot);
  wwww = v_splat_w(rot);
  yzxw = v_perm_yzxw(rot); yzxw_2 = v_perm_yzxw(xyzw_2);
  zxyw = v_perm_zxyw(rot); zxyw_2 = v_perm_zxyw(xyzw_2);
  tmp0 = v_mul(yzxw_2, wwww);
  tmp1 = v_nmsub(yzxw, yzxw_2, V_C_ONE);
  tmp2 = v_mul(yzxw, xyzw_2);
  tmp0 = v_madd(zxyw, xyzw_2, tmp0);
  tmp1 = v_nmsub(zxyw, zxyw_2, tmp1);
  tmp2 = v_nmsub(zxyw_2, wwww, tmp2);
  tmp3 = v_perm_ayzw(tmp0, tmp1);
  tmp4 = v_perm_ayzw(tmp1, tmp2);
  tmp5 = v_perm_ayzw(tmp2, tmp0);
  dest.col0 = v_perm_xycw(tmp3, tmp2);
  dest.col1 = v_perm_xycw(tmp4, tmp0);
  dest.col2 = v_perm_xycw(tmp5, tmp1);
}

//! compose 3x3 matrix from rotation/scale
VECMATH_FINLINE void VECTORCALL v_mat33_compose(mat33f &dest, quat4f rot, vec4f scale)
{
  vec4f xyzw_2, wwww, yzxw, zxyw, yzxw_2, zxyw_2;
  vec4f tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;

  xyzw_2 = v_add(rot, rot);
  wwww = v_splat_w(rot);
  yzxw = v_perm_yzxw(rot); yzxw_2 = v_perm_yzxw(xyzw_2);
  zxyw = v_perm_zxyw(rot); zxyw_2 = v_perm_zxyw(xyzw_2);
  tmp0 = v_mul(yzxw_2, wwww);
  tmp1 = v_nmsub(yzxw, yzxw_2, V_C_ONE);
  tmp2 = v_mul(yzxw, xyzw_2);
  tmp0 = v_madd(zxyw, xyzw_2, tmp0);
  tmp1 = v_nmsub(zxyw, zxyw_2, tmp1);
  tmp2 = v_nmsub(zxyw_2, wwww, tmp2);
  tmp3 = v_perm_ayzw(tmp0, tmp1);
  tmp4 = v_perm_ayzw(tmp1, tmp2);
  tmp5 = v_perm_ayzw(tmp2, tmp0);
  dest.col0 = v_mul(v_perm_xycw(tmp3, tmp2), v_splat_x(scale));
  dest.col1 = v_mul(v_perm_xycw(tmp4, tmp0), v_splat_y(scale));
  dest.col2 = v_mul(v_perm_xycw(tmp5, tmp1), v_splat_z(scale));
}

//! make 4x4 matrix from quaternion and position
VECMATH_FINLINE void VECTORCALL v_mat44_from_quat(mat44f &dest, quat4f q, vec4f pos)
{
  mat33f m;
  v_mat33_from_quat(m, q);
  dest.set33(m);
  dest.col3 = pos;
}

//! compose 4x4 matrix from position/rotation/scale
VECMATH_FINLINE void VECTORCALL v_mat44_compose(mat44f &dest, vec4f pos, quat4f rot, vec4f scale)
{
  vec4f xyzw_2, wwww, yzxw, zxyw, yzxw_2, zxyw_2;
  vec4f tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;
  vec4f select_xyz = (vec4f)V_CI_MASK1110;

  xyzw_2 = v_add(rot, rot);
  wwww = v_splat_w(rot);
  yzxw = v_perm_yzxw(rot); yzxw_2 = v_perm_yzxw(xyzw_2);
  zxyw = v_perm_zxyw(rot); zxyw_2 = v_perm_zxyw(xyzw_2);
  tmp0 = v_mul(yzxw_2, wwww);
  tmp1 = v_nmsub(yzxw, yzxw_2, V_C_ONE);
  tmp2 = v_mul(yzxw, xyzw_2);
  tmp0 = v_madd(zxyw, xyzw_2, tmp0);
  tmp1 = v_nmsub(zxyw, zxyw_2, tmp1);
  tmp2 = v_nmsub(zxyw_2, wwww, tmp2);
  tmp3 = v_perm_ayzw(tmp0, tmp1);
  tmp4 = v_perm_ayzw(tmp1, tmp2);
  tmp5 = v_perm_ayzw(tmp2, tmp0);
  dest.col0 = v_and(v_mul(v_perm_xycw(tmp3, tmp2), v_splat_x(scale)), select_xyz);
  dest.col1 = v_and(v_mul(v_perm_xycw(tmp4, tmp0), v_splat_y(scale)), select_xyz);
  dest.col2 = v_and(v_mul(v_perm_xycw(tmp5, tmp1), v_splat_z(scale)), select_xyz);
  dest.col3 = v_perm_xyzd(pos, V_C_ONE);
}

//! decompose 3x3 matrix to rotation/scale
VECMATH_FINLINE void VECTORCALL v_mat33_decompose(mat33f_cref &tm, quat4f &rot, vec4f &scl)
{
  scl = v_perm_ayzw(
    v_perm_wxyz(v_perm_ayzw(v_length3_sq(tm.col2), v_length3_sq(tm.col1))),
    v_length3_sq(tm.col0));
  scl = v_sqrt4(scl);
  if (v_test_vec_x_lt_0(v_mat33_det(tm)))
    scl = v_perm_xycw(scl, v_neg(scl));

  rot = v_un_quat_from_mat(
    v_div(tm.col0, v_splat_x(scl)),
    v_div(tm.col1, v_splat_y(scl)),
    v_div(tm.col2, v_splat_z(scl)));
  rot = v_norm4(rot);
}

//! decompose 4x4 matrix to position/rotation/scale
VECMATH_FINLINE void VECTORCALL v_mat4_decompose(mat44f_cref &tm, vec3f &pos, quat4f &rot, vec4f &scl)
{
  pos = tm.col3;

  scl = v_perm_ayzw(
    v_perm_wxyz(v_perm_ayzw(v_length3_sq(tm.col2), v_length3_sq(tm.col1))),
    v_length3_sq(tm.col0));
  scl = v_sqrt4(scl);
  if (v_test_vec_x_lt_0(v_mat44_det43(tm)))
    scl = v_perm_xycw(scl, v_neg(scl));

  rot = v_un_quat_from_mat(
    v_div(tm.col0, v_splat_x(scl)),
    v_div(tm.col1, v_splat_y(scl)),
    v_div(tm.col2, v_splat_z(scl)));
  rot = v_norm4(rot);
}

//! make rotational matrix 3x3 to rotate 'ang' radians around 'v'
VECMATH_FINLINE void VECTORCALL v_mat33_make_rot_cw(mat33f &dest, vec3f v, vec4f ang)
{
  vec4f s, c, oneMinusC, axisS, negAxisS, xxxx, yyyy, zzzz, tmp0, tmp1, tmp2;
  v_sincos_x(ang, s, c);
  xxxx = v_splat_x(v);
  yyyy = v_splat_y(v);
  zzzz = v_splat_z(v);
  oneMinusC = v_sub(V_C_ONE, c);
  axisS = v_mul(v, s);
  negAxisS = v_neg(axisS);
  tmp0 = v_perm_xzbx(axisS, negAxisS);
  tmp1 = v_perm_caxx(axisS, negAxisS);
  tmp2 = v_perm_yaxx(axisS, negAxisS);
  tmp0 = v_perm_ayzw(tmp0, c);
  tmp1 = v_perm_xbzw(tmp1, c);
  tmp2 = v_perm_xycw(tmp2, c);

  vec4f axisOneMinusC = v_mul(v, oneMinusC);
  dest.col0 = v_madd(xxxx, axisOneMinusC, tmp0);
  dest.col1 = v_madd(yyyy, axisOneMinusC, tmp1);
  dest.col2 = v_madd(zzzz, axisOneMinusC, tmp2);
}

//! make rotational matrix 3x3 to rotate 'ang' radians around X axis
VECMATH_FINLINE void VECTORCALL v_mat33_make_rot_cw_x(mat33f &dest, vec4f ang)
{
  vec4f s, c, res1, res2;
  vec4f select_y = (vec4f)V_CI_MASK0100;
  v_sincos_x(ang, s, c);

  dest.col0 = V_C_UNIT_1000;
  res1 = v_and(c, select_y);
  dest.col1 = v_perm_xycw(res1, s);
  res2 = v_and(v_neg(s), select_y);
  dest.col2 = v_perm_xycw(res2, c);
}

//! make rotational matrix 3x3 to rotate 'ang' radians around Y axis
VECMATH_FINLINE void VECTORCALL v_mat33_make_rot_cw_y(mat33f &dest, vec4f ang)
{
  vec4f s, c, res0, res2;
  vec4f select_x = (vec4f)V_CI_MASK1000;
  v_sincos_x(ang, s, c);

  res0 = v_and(c, select_x);
  dest.col0 = v_perm_xycw(res0, v_neg(s));
  dest.col1 = V_C_UNIT_0100;
  res2 = v_and(s, select_x);
  dest.col2 = v_perm_xycw(res2, c);
}

//! make rotational matrix 3x3 to rotate 'ang' radians around Z axis
VECMATH_FINLINE void VECTORCALL v_mat33_make_rot_cw_z(mat33f &dest, vec4f ang)
{
  vec4f s, c, res0, res1;
  vec4f select_x = (vec4f)V_CI_MASK1000;
  v_sincos4(ang, s, c);

  res0 = v_and(c, select_x);
  dest.col0 = v_perm_xbzw(res0, s);
  res1 = v_and(v_neg(s), select_x);
  dest.col1 = v_perm_xbzw(res1, c);
  dest.col2 = V_C_UNIT_0010;
}

//! make composite matrix 3x3: =rot_z(ang.z)*rot_y(ang.y)*rot_x(ang.x)
VECMATH_FINLINE void VECTORCALL v_mat33_make_rot_cw_zyx(mat33f &dest, vec4f ang_xyz)
{
  vec4f s, negS, c, X0, X1, Y0, Y1, Z0, Z1, tmp;
  v_sincos4(v_and(ang_xyz, (vec4f)V_CI_MASK1110), s, c);
  negS = v_neg(s);
  Z0 = v_merge_lw(c, s);
  Z1 = v_merge_lw(negS, c);
  Z1 = v_and(Z1, (vec4f)V_CI_MASK1110);
  Y0 = v_perm_bbyx(negS, c);
  Y1 = v_perm_bbyx(c, s);
  X0 = v_splat_x(s);
  X1 = v_splat_x(c);
  tmp = v_mul( Z0, Y1);
  dest.col0 = v_mul(Z0, Y0);
  dest.col1 = v_madd(Z1, X1, v_mul(tmp, X0));
  dest.col2 = v_nmsub(Z1, X0, v_mul(tmp, X1));
}

VECMATH_FINLINE plane3f VECTORCALL v_norm_plane(plane3f in) { return v_norm3(in); }

VECMATH_FINLINE vec3f VECTORCALL v_ray_intersect_plane(vec3f A, vec3f B, plane3f P, vec4f &behind, vec4f &t)
{
  vec3f dir = v_sub(B, A);
  vec4f dot = v_dot3(dir, P);
  t = v_div(v_sub(v_neg(v_splat_w(P)),v_dot3(A, P)), dot);
  behind = v_or(v_cmp_gt(v_zero(), t), v_cmp_eq(dot, v_zero()));
  return v_madd(t, dir, A);
}

VECMATH_FINLINE vec3f VECTORCALL three_plane_intersection(plane3f p0, plane3f p1, plane3f p2, vec4f &invalid)
{
  vec4f n1_n2 = v_cross3(p1, p2), n2_n0 = v_cross3(p2, p0), n0_n1 = v_cross3(p0, p1);
  vec4f cosTheta = v_dot3(p0, n1_n2);
  vec4f zero = v_zero();
  invalid = v_cmp_eq(cosTheta, zero);
  vec4f secTheta = v_rcp(cosTheta);

  vec4f intersectPt;
  intersectPt = v_nmsub(n1_n2, v_splat_w(p0), zero);
  intersectPt = v_nmsub(n2_n0, v_splat_w(p1), intersectPt);
  intersectPt = v_nmsub(n0_n1, v_splat_w(p2), intersectPt);
  return v_mul(intersectPt, secTheta);
}
VECMATH_FINLINE void VECTORCALL v_unsafe_two_plane_intersection(plane3f p1, plane3f p2, vec3f &point, vec3f &dir)
{
  dir = v_cross3(p1, p2);//p3_normal
  // calculate the final (point, normal)
  point = v_add(v_mul(v_cross3(dir, p2), v_splat_w(p1)), v_mul(v_cross3(p1, dir), v_splat_w(p2)));
  //vec4f det = v_dot3(dir, dir);
  //for safe check if abs(det) is bigger then eps
  point = v_div(point, v_dot3(dir, dir));
}

VECMATH_FINLINE vec3f VECTORCALL v_unsafe_ray_intersect_plane(vec3f point, vec3f dir, plane3f P)
{
  vec3f t = v_div(v_sub(v_neg(v_splat_w(P)), v_dot3(point, P)), v_dot3(dir, P));
  //for safe check if abs(v_dot3(dir, P)) is bigger than eps
  return v_madd(t, dir, point);
}

VECMATH_FINLINE vec3f VECTORCALL closest_point_in_segment(vec3f A, vec3f B, vec3f P)
{
  vec3f AP = v_sub(P, A);
  vec3f AB = v_sub(B, A);
  vec3f AB2 = v_length3_sq(AB);
  vec3f t = v_div(v_dot3(AP, AB), AB2);
  //clamp segment
  t = v_max(t, v_zero());
  t = v_min(t, V_C_ONE);
  return v_madd(AB, t, A);
}

VECMATH_FINLINE void VECTORCALL vis_transform_points_4(vec4f* dest, vec4f x, vec4f y, vec4f z, mat44f_cref mat)
{
#define COMP(c, attr) \
  vec4f res_ ## c = v_splat_##attr(mat.col3); \
  res_ ## c = v_madd(z, v_splat_##attr(mat.col2), res_ ## c); \
  res_ ## c = v_madd(y, v_splat_##attr(mat.col1), res_ ## c); \
  res_ ## c = v_madd(x, v_splat_##attr(mat.col0), res_ ## c); \
  dest[c] = res_ ## c;
  COMP(0,x);
  COMP(1,y);
  COMP(2,z);
  COMP(3,w);
#undef COMP
}

VECMATH_FINLINE void VECTORCALL vis_transform_points_4(vec4f* dest, vec4f x, vec4f y, mat44f_cref mat)
{
#define COMP(c, attr) \
  vec4f res_ ## c = v_splat_##attr(mat.col3); \
  res_ ## c = v_madd(y, v_splat_##attr(mat.col1), res_ ## c); \
  res_ ## c = v_madd(x, v_splat_##attr(mat.col0), res_ ## c); \
  dest[c] = res_ ## c;
  COMP(0,x);
  COMP(1,y);
  COMP(2,z);
  COMP(3,w);
#undef COMP
}


#if _TARGET_SIMD_NEON|_TARGET_SIMD_SSE
VECMATH_FINLINE vec4f VECTORCALL vis_hor_or_ff_0(vec4f a)
{
  vec4f m = v_or(v_rot_1(a), a);
  return v_or(v_rot_2(m), m);
}
#endif

#if _TARGET_SIMD_SSE
VECMATH_FINLINE int VECTORCALL v_test_vec_mask_eq_0(vec4f v) { return (~unsigned(-_mm_movemask_ps(v))) >> 31; }
VECMATH_FINLINE int VECTORCALL v_test_vec_mask_neq_0(vec4f v) { return (unsigned(-_mm_movemask_ps(v))) >> 31; }
#else
VECMATH_FINLINE int VECTORCALL v_test_vec_mask_eq_0(vec4f v)
{
  return v_test_vec_x_eq_0(v_cast_vec4f(v_srli(v_cast_vec4i(vis_hor_or_ff_0(v)), 31)));
}

VECMATH_FINLINE int VECTORCALL v_test_vec_mask_neq_0(vec4f v)
{
  return !v_test_vec_mask_eq_0(v);
}
#endif

//universal visibility function (accepts worldviewproj matrix)

///return zero if not visible. 0xff in all bytes, otherwise
///really fast, ~0.075 us per test on PS3

///for branching: (~v_test_vec_x_eq_0( v_is_visible(bmin, bmax, clip)))&1 will be 1 if visible, zero otherwise;
VECMATH_FINLINE vec4f VECTORCALL v_is_visible(vec3f bmin, vec3f bmax, mat44f_cref clip)
{
  vec4f zero = v_zero();

  // get aabb points (SoA)
  vec4f minmax_x = v_perm_xXxX(bmin, bmax);
  vec4f minmax_y = v_perm_yyYY(bmin, bmax);
  vec4f minmax_z_0 = v_splat_z(bmin);
  vec4f minmax_z_1 = v_splat_z(bmax);

  // transform points to clip space
  vec4f points_cs_0[4];
  vec4f points_cs_1[4];

  vis_transform_points_4(points_cs_0, minmax_x, minmax_y, minmax_z_0, clip);
  vis_transform_points_4(points_cs_1, minmax_x, minmax_y, minmax_z_1, clip);

  // calculate -w
  vec4f points_cs_0_negw = v_neg(points_cs_0[3]);
  vec4f points_cs_1_negw = v_neg(points_cs_1[3]);

  // for each plane...
  #define NOUT(a, b, c, d) vis_hor_or_ff_0(v_or(v_cmp_gt(a, b), v_cmp_gt(c, d)))

  vec4f nout0 = NOUT(points_cs_0[0], points_cs_0_negw, points_cs_1[0], points_cs_1_negw);
  vec4f nout1 = NOUT(points_cs_0[3], points_cs_0[0], points_cs_1[3], points_cs_1[0]);
  vec4f nout2 = NOUT(points_cs_0[1], points_cs_0_negw, points_cs_1[1], points_cs_1_negw);
  vec4f nout3 = NOUT(points_cs_0[3], points_cs_0[1], points_cs_1[3], points_cs_1[1]);
  vec4f nout4 = NOUT(points_cs_0[2], zero, points_cs_1[2], zero);
  vec4f nout5 = NOUT(points_cs_0[3], points_cs_0[2], points_cs_1[3], points_cs_1[2]);

  #undef NOUT

  // merge "not outside" flags
  vec4f nout01 = v_and(nout0, nout1);
  vec4f nout012 = v_and(nout01, nout2);

  vec4f nout34 = v_and(nout3, nout4);
  vec4f nout345 = v_and(nout34, nout5);

  vec4f nout = v_and(nout012, nout345);

  return nout;
}

#if _TARGET_SIMD_SSE
VECMATH_FINLINE int VECTORCALL v_is_visible_b(vec3f bmin, vec3f bmax, mat44f_cref clip)
{
  // get aabb points (SoA)
  vec4f minmax_x = v_perm_xXxX(bmin, bmax);
  vec4f minmax_y = v_perm_yyYY(bmin, bmax);
  vec4f minmax_z_0 = v_splat_z(bmin);
  vec4f minmax_z_1 = v_splat_z(bmax);

  // transform points to clip space
  vec4f points_cs_0[4];
  vec4f points_cs_1[4];

  vis_transform_points_4(points_cs_0, minmax_x, minmax_y, minmax_z_0, clip);
  vis_transform_points_4(points_cs_1, minmax_x, minmax_y, minmax_z_1, clip);

  // calculate -w
  vec4f points_cs_0_negw = v_neg(points_cs_0[3]);
  vec4f points_cs_1_negw = v_neg(points_cs_1[3]);

  // for each plane...
  #define NOUT(a, b, c, d) (unsigned(-_mm_movemask_ps(v_or(v_cmp_gt(a, b), v_cmp_gt(c, d)))))
  unsigned nout;
  nout = (NOUT(points_cs_0[0], points_cs_0_negw, points_cs_1[0], points_cs_1_negw));
  nout &= (NOUT(points_cs_0[3], points_cs_0[0], points_cs_1[3], points_cs_1[0]));
  nout &= (NOUT(points_cs_0[1], points_cs_0_negw, points_cs_1[1], points_cs_1_negw));
  nout &= (NOUT(points_cs_0[3], points_cs_0[1], points_cs_1[3], points_cs_1[1]));
  nout &= (NOUT(points_cs_0[2], v_zero(), points_cs_1[2], v_zero()));
  nout &= (NOUT(points_cs_0[3], points_cs_0[2], points_cs_1[3], points_cs_1[2]));

  #undef NOUT

  // merge "not outside" flags
  return nout>>31;
}
#else
VECMATH_FINLINE int VECTORCALL v_is_visible_b(vec3f bmin, vec3f bmax, mat44f_cref clip)
{
  vec4f ret = v_is_visible(bmin, bmax, clip);
  return (~v_test_vec_x_eq_0(ret))&1;
}
#endif

VECMATH_FINLINE void VECTORCALL v_construct_camplanes(mat44f_cref clip,
  vec4f &camPlanes0, vec4f &camPlanes1, vec4f &camPlanes2, vec4f &camPlanes3, vec4f &camPlanes4, vec4f &camPlanes5)
{
  mat44f m2;
  v_mat44_transpose(m2, clip);
  camPlanes0 = v_sub(m2.col3, m2.col0);  // right
  camPlanes1 = v_add(m2.col3, m2.col0);  // left
  camPlanes2 = v_sub(m2.col3, m2.col1);  // top
  camPlanes3 = v_add(m2.col3, m2.col1);  // bottom
  camPlanes4 = v_sub(m2.col3, m2.col2);  // far if forward depth (near otherwise)
  camPlanes5 = m2.col2;  // near if forward depth (far otherwise)
}

VECMATH_FINLINE void VECTORCALL v_frustum_box_unsafe(bbox3f& box,
                                 vec4f camPlanes0, vec4f camPlanes1, vec4f camPlanes2,
                                 const vec4f &camPlanes3, const vec4f &camPlanes4, const vec4f &camPlanes5)
{
  vec3f invalid;
  box.bmax = box.bmin = three_plane_intersection(camPlanes5, camPlanes2, camPlanes1, invalid);
  v_bbox3_add_pt(box, three_plane_intersection(camPlanes4, camPlanes2, camPlanes1, invalid));
  v_bbox3_add_pt(box, three_plane_intersection(camPlanes5, camPlanes3, camPlanes1, invalid));
  v_bbox3_add_pt(box, three_plane_intersection(camPlanes4, camPlanes3, camPlanes1, invalid));
  v_bbox3_add_pt(box, three_plane_intersection(camPlanes5, camPlanes2, camPlanes0, invalid));
  v_bbox3_add_pt(box, three_plane_intersection(camPlanes4, camPlanes2, camPlanes0, invalid));
  v_bbox3_add_pt(box, three_plane_intersection(camPlanes5, camPlanes3, camPlanes0, invalid));
  v_bbox3_add_pt(box, three_plane_intersection(camPlanes4, camPlanes3, camPlanes0, invalid));
}

VECMATH_FINLINE int VECTORCALL v_is_visible_sphere(vec3f center, vec3f r,
                  vec4f plane03X, const vec4f& plane03Y, const vec4f& plane03Z, const vec4f& plane03W,
                  const vec4f& plane4, const vec4f& plane5)
{
  vec4f res03;
  res03 = v_madd(v_splat_x(center), plane03X, plane03W);
  res03 = v_madd(v_splat_y(center), plane03Y, res03);
  res03 = v_madd(v_splat_z(center), plane03Z, res03);
  res03 = v_add(res03, r);
  res03 = v_or(res03, v_add(v_add(v_dot3(center, plane4), r), v_splat_w(plane4)));
  res03 = v_or(res03, v_add(v_add(v_dot3(center, plane5), r), v_splat_w(plane5)));

  return v_test_vec_mask_eq_0(res03);
}

VECMATH_FINLINE int VECTORCALL v_sphere_intersect(vec3f center, vec3f r,
                  vec3f plane03X, const vec4f& plane03Y, const vec4f& plane03Z, const vec4f& plane03W,
                  const vec4f& plane4, const vec4f& plane5)
{
  vec4f res03;
  res03 = v_madd(v_splat_x(center), plane03X, plane03W);
  res03 = v_madd(v_splat_y(center), plane03Y, res03);
  res03 = v_madd(v_splat_z(center), plane03Z, res03);
  res03 = v_add(res03, r);
  res03 = v_or(res03, v_add(v_add(v_dot3(center, plane4), r), v_splat_w(plane4)));
  res03 = v_or(res03, v_add(v_add(v_dot3(center, plane5), r), v_splat_w(plane5)));

  if (v_test_vec_mask_neq_0(res03))
    return 0;

  res03 = v_madd(v_splat_x(center), plane03X, plane03W);
  res03 = v_madd(v_splat_y(center), plane03Y, res03);
  res03 = v_madd(v_splat_z(center), plane03Z, res03);
  res03 = v_sub(res03, r);
  res03 = v_or(res03, v_add(v_sub(v_dot3(center, plane4), r), v_splat_w(plane4)));
  res03 = v_or(res03, v_add(v_sub(v_dot3(center, plane5), r), v_splat_w(plane5)));

  return v_test_vec_mask_neq_0(res03) + 1;
}

VECMATH_FINLINE int VECTORCALL v_is_visible_box_extent2(vec3f center, vec3f extent,//center and extent should be multiplied by 2
                  vec4f plane03X, const vec4f& plane03Y, const vec4f& plane03Z, const vec4f& plane03W2,
                  const vec4f& plane4W2, const vec4f& plane5W2)
{
  vec4f signmask = v_cast_vec4f(V_CI_SIGN_MASK);
  vec4f res03;
  res03 = v_madd(v_add(v_splat_x(center), v_xor(v_splat_x(extent), v_and(plane03X, signmask))), plane03X, plane03W2);
  res03 = v_madd(v_add(v_splat_y(center), v_xor(v_splat_y(extent), v_and(plane03Y, signmask))), plane03Y, res03);
  res03 = v_madd(v_add(v_splat_z(center), v_xor(v_splat_z(extent), v_and(plane03Z, signmask))), plane03Z, res03);
  res03 = v_or(res03, v_add(v_dot3(v_add(v_xor(extent, v_and(plane4W2, signmask)), center), plane4W2), v_splat_w(plane4W2)));
  res03 = v_or(res03, v_add(v_dot3(v_add(v_xor(extent, v_and(plane5W2, signmask)), center), plane5W2), v_splat_w(plane5W2)));

  return v_test_vec_mask_eq_0(res03);
}

VECMATH_FINLINE int VECTORCALL v_box_frustum_intersect_extent2(vec3f center, vec3f extent,//center and extent should be multiplied by 2
                  vec4f plane03X, const vec4f& plane03Y, const vec4f& plane03Z, const vec4f& plane03W2,
                  const vec4f& plane4W2, const vec4f& plane5W2)
{
  vec4f signmask = v_cast_vec4f(V_CI_SIGN_MASK);
  vec4f res03;
  res03 = v_madd(v_add(v_splat_x(center), v_xor(v_splat_x(extent), v_and(plane03X, signmask))), plane03X, plane03W2);
  res03 = v_madd(v_add(v_splat_y(center), v_xor(v_splat_y(extent), v_and(plane03Y, signmask))), plane03Y, res03);
  res03 = v_madd(v_add(v_splat_z(center), v_xor(v_splat_z(extent), v_and(plane03Z, signmask))), plane03Z, res03);
  res03 = v_or(res03, v_add(v_dot3(v_add(v_xor(extent, v_and(plane4W2, signmask)), center), plane4W2), v_splat_w(plane4W2)));
  res03 = v_or(res03, v_add(v_dot3(v_add(v_xor(extent, v_and(plane5W2, signmask)), center), plane5W2), v_splat_w(plane5W2)));

  if (v_test_vec_mask_neq_0(res03))
    return 0;

  res03 = v_madd(v_sub(v_splat_x(center), v_xor(v_splat_x(extent), v_and(plane03X, signmask))), plane03X, plane03W2);
  res03 = v_madd(v_sub(v_splat_y(center), v_xor(v_splat_y(extent), v_and(plane03Y, signmask))), plane03Y, res03);
  res03 = v_madd(v_sub(v_splat_z(center), v_xor(v_splat_z(extent), v_and(plane03Z, signmask))), plane03Z, res03);
  res03 = v_or(res03, v_add(v_dot3(v_sub(center, v_xor(extent, v_and(plane4W2, signmask))), plane4W2), v_splat_w(plane4W2)));
  res03 = v_or(res03, v_add(v_dot3(v_sub(center, v_xor(extent, v_and(plane5W2, signmask))), plane5W2), v_splat_w(plane5W2)));

  return v_test_vec_mask_neq_0(res03) + 1;
}

VECMATH_FINLINE int VECTORCALL v_is_visible_extent_fast(vec3f center, vec3f extent, mat44f_cref clip)//center and extent should be multiplied by 2
{
  //construct frustum
  vec3f plane03X, plane03Y, plane03Z, plane03W2, plane4W2, plane5W2;
  v_construct_camplanes(clip, plane03X, plane03Y, plane03Z, plane03W2, plane4W2, plane5W2);
  v_mat44_transpose(plane03X, plane03Y, plane03Z, plane03W2);
  plane03W2 = v_add(plane03W2, plane03W2);
  plane4W2 = v_perm_xyzd(plane4W2, v_add(plane4W2, plane4W2));
  plane5W2 = v_perm_xyzd(plane5W2, v_add(plane5W2, plane5W2));

  //perform test
  return v_is_visible_box_extent2(center, extent, plane03X, plane03Y, plane03Z, plane03W2, plane4W2, plane5W2);
}

VECMATH_FINLINE int VECTORCALL v_frustum_intersect(vec3f center, vec3f extent, mat44f_cref clip)//center and extent should be multiplied by 2
{
  //construct frustum
  vec3f plane03X, plane03Y, plane03Z, plane03W2, plane4W2, plane5W2;
  v_construct_camplanes(clip, plane03X, plane03Y, plane03Z, plane03W2, plane4W2, plane5W2);
  v_mat44_transpose(plane03X, plane03Y, plane03Z, plane03W2);
  plane03W2 = v_add(plane03W2, plane03W2);
  plane4W2 = v_perm_xyzd(plane4W2, v_add(plane4W2, plane4W2));
  plane5W2 = v_perm_xyzd(plane5W2, v_add(plane5W2, plane5W2));
  //perform test
  return v_box_frustum_intersect_extent2(center, extent, plane03X, plane03Y, plane03Z, plane03W2, plane4W2, plane5W2);
}

VECMATH_FINLINE int VECTORCALL v_is_visible_b_fast(vec3f bmin, vec3f bmax, mat44f_cref clip)
{
  vec4f center = v_add(bmax, bmin);
  vec4f extent = v_sub(bmax, bmin);
  return v_is_visible_extent_fast(center, extent, clip);
}

VECMATH_FINLINE int VECTORCALL v_is_visible_box_extent2(vec3f center, vec3f extent,//center and extent should be multiplied by 2
                  vec4f plane03X, const vec4f& plane03Y, const vec4f& plane03Z, const vec4f& plane03W2,
                  const vec4f& plane47X, const vec4f& plane47Y, const vec4f& plane47Z, const vec4f& plane47W2)
{
  vec4f signmask = v_cast_vec4f(V_CI_SIGN_MASK);
  vec4f res03, res47;
  res03 = v_madd(v_add(v_splat_x(center), v_xor(v_splat_x(extent), v_and(plane03X, signmask))), plane03X, plane03W2);
  res03 = v_madd(v_add(v_splat_y(center), v_xor(v_splat_y(extent), v_and(plane03Y, signmask))), plane03Y, res03);
  res03 = v_madd(v_add(v_splat_z(center), v_xor(v_splat_z(extent), v_and(plane03Z, signmask))), plane03Z, res03);

  res47 = v_madd(v_add(v_splat_x(center), v_xor(v_splat_x(extent), v_and(plane47X, signmask))), plane47X, plane47W2);
  res47 = v_madd(v_add(v_splat_y(center), v_xor(v_splat_y(extent), v_and(plane47Y, signmask))), plane47Y, res47);
  res47 = v_madd(v_add(v_splat_z(center), v_xor(v_splat_z(extent), v_and(plane47Z, signmask))), plane47Z, res47);

  int result = v_signmask(v_or(res03, res47));
  return (~unsigned(-result))>>31;
}

VECMATH_FINLINE int VECTORCALL v_box_frustum_intersect_extent2(vec3f center, vec3f extent,//center and extent should be multiplied by 2
                  vec4f plane03X, const vec4f& plane03Y, const vec4f& plane03Z, const vec4f& plane03W2,
                  const vec4f& plane47X, const vec4f& plane47Y, const vec4f& plane47Z, const vec4f& plane47W2)
{
  /*
  vec4f signmask = v_cast_vec4f(V_CI_SIGN_MASK);
  vec4f res03, res47;
  res03 = v_madd(v_add(v_splat_x(center), v_xor(v_splat_x(extent), v_and(plane03X, signmask))), plane03X, plane03W2);
  res03 = v_madd(v_add(v_splat_y(center), v_xor(v_splat_y(extent), v_and(plane03Y, signmask))), plane03Y, res03);
  res03 = v_madd(v_add(v_splat_z(center), v_xor(v_splat_z(extent), v_and(plane03Z, signmask))), plane03Z, res03);

  res47 = v_madd(v_add(v_splat_x(center), v_xor(v_splat_x(extent), v_and(plane47X, signmask))), plane47X, plane47W2);
  res47 = v_madd(v_add(v_splat_y(center), v_xor(v_splat_y(extent), v_and(plane47Y, signmask))), plane47Y, res47);
  res47 = v_madd(v_add(v_splat_z(center), v_xor(v_splat_z(extent), v_and(plane47Z, signmask))), plane47Z, res47);

  int result = v_signmask(v_or(res03, res47));
  if ((unsigned(-result))>>31)
    return 0;
  res03 = v_madd(v_sub(v_splat_x(center), v_xor(v_splat_x(extent), v_and(plane03X, signmask))), plane03X, plane03W2);
  res03 = v_madd(v_sub(v_splat_y(center), v_xor(v_splat_y(extent), v_and(plane03Y, signmask))), plane03Y, res03);
  res03 = v_madd(v_sub(v_splat_z(center), v_xor(v_splat_z(extent), v_and(plane03Z, signmask))), plane03Z, res03);

  res47 = v_madd(v_sub(v_splat_x(center), v_xor(v_splat_x(extent), v_and(plane47X, signmask))), plane47X, plane47W2);
  res47 = v_madd(v_sub(v_splat_y(center), v_xor(v_splat_y(extent), v_and(plane47Y, signmask))), plane47Y, res47);
  res47 = v_madd(v_sub(v_splat_z(center), v_xor(v_splat_z(extent), v_and(plane47Z, signmask))), plane47Z, res47);
  result = v_signmask(v_or(res03, res47));
  return ((unsigned(-result))>>31) + 1;
  /*/
  vec4f signmask = v_cast_vec4f(V_CI_SIGN_MASK);
  vec4f res03Base, res47Base, res03Add, res47Add;
  res03Base = v_madd(v_splat_x(center), plane03X, plane03W2);
  res03Base = v_madd(v_splat_y(center), plane03Y, res03Base);
  res03Base = v_madd(v_splat_z(center), plane03Z, res03Base);
  res03Add =  v_mul(v_xor(v_splat_x(extent), v_and(plane03X, signmask)), plane03X);
  res03Add = v_madd(v_xor(v_splat_y(extent), v_and(plane03Y, signmask)), plane03Y, res03Add);
  res03Add = v_madd(v_xor(v_splat_z(extent), v_and(plane03Z, signmask)), plane03Z, res03Add);

  res47Base = v_madd(v_splat_x(center), plane47X, plane47W2);
  res47Base = v_madd(v_splat_y(center), plane47Y, res47Base);
  res47Base = v_madd(v_splat_z(center), plane47Z, res47Base);
  res47Add =  v_mul(v_xor(v_splat_x(extent), v_and(plane47X, signmask)), plane47X);
  res47Add = v_madd(v_xor(v_splat_y(extent), v_and(plane47Y, signmask)), plane47Y, res47Add);
  res47Add = v_madd(v_xor(v_splat_z(extent), v_and(plane47Z, signmask)), plane47Z, res47Add);

  int result = v_signmask(v_or(v_add(res03Base, res03Add), v_add(res47Base, res47Add)));
  if ((unsigned(-result))>>31)
    return 0;

  result = v_signmask(v_or(v_sub(res03Base, res03Add), v_sub(res47Base, res47Add)));
  return ((unsigned(-result))>>31) + 1;
  //*/
}

VECMATH_FINLINE int VECTORCALL v_is_visible_sphere(vec3f center, vec3f r,
                  vec3f plane03X, const vec4f& plane03Y, const vec4f& plane03Z, const vec4f& plane03W,
                  const vec4f& plane47X, const vec4f& plane47Y, const vec4f& plane47Z, const vec4f& plane47W)
{
  vec4f res03, res47;
  res03 = v_madd(v_splat_x(center), plane03X, plane03W);
  res03 = v_madd(v_splat_y(center), plane03Y, res03);
  res03 = v_madd(v_splat_z(center), plane03Z, res03);
  res47 = v_madd(v_splat_x(center), plane47X, plane47W);
  res47 = v_madd(v_splat_y(center), plane47Y, res47);
  res47 = v_madd(v_splat_z(center), plane47Z, res47);
  res03 = v_or(v_add(res03, r), v_add(res47, r));
  int result = v_signmask(res03);
  return (~unsigned(-result))>>31;
}

VECMATH_FINLINE int VECTORCALL v_sphere_intersect(vec3f center, vec3f r,
                  vec3f plane03X, const vec4f& plane03Y, const vec4f& plane03Z, const vec4f& plane03W,
                  const vec4f& plane47X, const vec4f& plane47Y, const vec4f& plane47Z, const vec4f& plane47W)
{
  vec4f res03, res47;
  res03 = v_madd(v_splat_x(center), plane03X, plane03W);
  res03 = v_madd(v_splat_y(center), plane03Y, res03);
  res03 = v_madd(v_splat_z(center), plane03Z, res03);
  res47 = v_madd(v_splat_x(center), plane47X, plane47W);
  res47 = v_madd(v_splat_y(center), plane47Y, res47);
  res47 = v_madd(v_splat_z(center), plane47Z, res47);
  vec4f resOut = v_or(v_add(res03, r), v_add(res47, r));
  int result = v_signmask(resOut);
  if ((unsigned(-result))>>31)
    return 0;

  vec4f resIn = v_or(v_sub(res03, r), v_sub(res47, r));
  result = v_signmask(resIn);
  return ((unsigned(-result))>>31) + 1;
}

VECMATH_FINLINE int VECTORCALL v_is_visible_extent_fast_8planes(vec3f center, vec3f extent, mat44f_cref clip)//just correct box extents, not multiplied
{
  //construct frustum
  vec3f plane03X, plane03Y, plane03Z, plane03W, plane47X, plane47Y, plane47Z, plane47W;
  v_construct_camplanes(clip, plane03X, plane03Y, plane03Z, plane03W, plane47X, plane47Y);
  v_mat44_transpose(plane03X, plane03Y, plane03Z, plane03W);
  plane47Z = plane47X, plane47W = plane47Y;//we can use some useful planes instead of replicating
  v_mat44_transpose(plane47X, plane47Y, plane47Z, plane47W);
  //perform test
  return v_is_visible_box_extent2(center, extent, plane03X, plane03Y, plane03Z, plane03W, plane47X, plane47Y, plane47Z, plane47W);
}

VECMATH_FINLINE int VECTORCALL v_is_visible_b_fast_8planes(vec3f bmin, vec3f bmax, mat44f_cref clip)
{
  vec4f center = v_mul(v_add(bmax, bmin), V_C_HALF);
  vec4f extent = v_sub(bmax, center);
  return v_is_visible_extent_fast_8planes(center, extent, clip);
}

//todo: move to consts
alignas(16) static float v_screen_full_screen[4] = {-2, 2, -2, 2};
alignas(16) static float v_screen_div_eps[4] = {0.0001f,0.0001f,0.0001f,0.0001f};

//universal visibility function (accepts worldviewproj matrix)

///return zero if not visible in frustum or if bbox screen size is smaller, than threshold. not zero, otherwise
// also, screen_box is minX, maxX, minY, maxY - in clipspace coordinates  (-1, -1) .. (1,1)

VECMATH_FINLINE int VECTORCALL v_screen_size_b(vec3f bmin, vec3f bmax, vec3f threshold, vec4f &screen_box, mat44f_cref clip)
{
  // get aabb points (SoA)
  vec4f minmax_x = v_perm_xXxX(bmin, bmax);
  vec4f minmax_y = v_perm_yyYY(bmin, bmax);
  vec4f minmax_z_0 = v_splat_z(bmin);
  vec4f minmax_z_1 = v_splat_z(bmax);

  // transform points to clip space
  vec4f points_cs_0[4];
  vec4f points_cs_1[4];

  vis_transform_points_4(points_cs_0, minmax_x, minmax_y, minmax_z_0, clip);
  vis_transform_points_4(points_cs_1, minmax_x, minmax_y, minmax_z_1, clip);

  // calculate -w
  vec4f points_cs_0_negw = v_neg(points_cs_0[3]);
  vec4f points_cs_1_negw = v_neg(points_cs_1[3]);

#if _TARGET_SIMD_SSE
  #define NOUT(a, b, c, d) (unsigned(-_mm_movemask_ps(v_or(v_cmp_gt(a, b), v_cmp_gt(c, d)))))
  unsigned nout;
  nout = (NOUT(points_cs_0[0], points_cs_0_negw, points_cs_1[0], points_cs_1_negw));
  nout &= (NOUT(points_cs_0[3], points_cs_0[0], points_cs_1[3], points_cs_1[0]));
  nout &= (NOUT(points_cs_0[1], points_cs_0_negw, points_cs_1[1], points_cs_1_negw));
  nout &= (NOUT(points_cs_0[3], points_cs_0[1], points_cs_1[3], points_cs_1[1]));
  nout &= (NOUT(points_cs_0[2], v_zero(), points_cs_1[2], v_zero()));
  nout &= (NOUT(points_cs_0[3], points_cs_0[2], points_cs_1[3], points_cs_1[2]));

  // merge "not outside" flags
  if ((nout&(1<<31)) == 0)
    return 0;

#else
  #define NOUT(a, b, c, d) vis_hor_or_ff_0(v_or(v_cmp_gt(a, b), v_cmp_gt(c, d)))
  vec4f nout0 = NOUT(points_cs_0[0], points_cs_0_negw, points_cs_1[0], points_cs_1_negw);
  vec4f nout1 = NOUT(points_cs_0[3], points_cs_0[0], points_cs_1[3], points_cs_1[0]);
  vec4f nout2 = NOUT(points_cs_0[1], points_cs_0_negw, points_cs_1[1], points_cs_1_negw);
  vec4f nout3 = NOUT(points_cs_0[3], points_cs_0[1], points_cs_1[3], points_cs_1[1]);
  vec4f nout4 = NOUT(points_cs_0[2], v_zero(), points_cs_1[2], v_zero());
  vec4f nout5 = NOUT(points_cs_0[3], points_cs_0[2], points_cs_1[3], points_cs_1[2]);

  // merge "not outside" flags
  vec4f nout01 = v_and(nout0, nout1);
  vec4f nout012 = v_and(nout01, nout2);

  vec4f nout34 = v_and(nout3, nout4);
  vec4f nout345 = v_and(nout34, nout5);

  vec4f nout = v_and(nout012, nout345);

  if (v_test_vec_x_eq_0(nout)) //"not outside"=0 -> outside=1
    return 0;

#endif
  #undef NOUT
  vec4f eps = *(vec4f*)v_screen_div_eps;
  vec4f valid_cs_0 = v_cmp_gt(points_cs_0[3], eps);
  vec4f valid_cs_1 = v_cmp_gt(points_cs_1[3], eps);
  vec4f valid_cs = v_and(valid_cs_0, valid_cs_1);

  #if _TARGET_SIMD_SSE
  if (_mm_movemask_ps(valid_cs) != 15)
  #else
  valid_cs = v_and(valid_cs, v_rot_1(valid_cs));//xy, yz, zw, wx
  valid_cs = v_and(valid_cs, v_rot_2(valid_cs));//xyzw,yzwx, zwxy, wxyz
  if (v_test_vec_x_eq_0(valid_cs))
  #endif
  {
    screen_box = *(vec4f*)v_screen_full_screen;
    return -1;
  }

  vec4f inv_cs0_3 = v_rcp(points_cs_0[3]);
  vec4f inv_cs1_3 = v_rcp(points_cs_1[3]);
  vec4f xxxx0 = v_mul(points_cs_0[0], inv_cs0_3);
  vec4f xxxx1 = v_mul(points_cs_1[0], inv_cs1_3);
  vec4f yyyy0 = v_mul(points_cs_0[1], inv_cs0_3);
  vec4f yyyy1 = v_mul(points_cs_1[1], inv_cs1_3);

  vec4f point01 = v_merge_hw(xxxx0, yyyy0);//xy, xy
  vec4f point23 = v_merge_lw(xxxx0, yyyy0);//xy, xy
  vec4f point45 = v_merge_hw(xxxx1, yyyy1);//xy, xy
  vec4f point67 = v_merge_lw(xxxx1, yyyy1);//xy, xy
  vec4f minXY = v_min(v_min(point01, point23), v_min(point45, point67));
  minXY = v_min(minXY, v_rot_2(minXY));
  vec4f maxXY = v_max(v_max(point01, point23), v_max(point45, point67));
  maxXY = v_max(maxXY, v_rot_2(maxXY));

  screen_box = v_merge_hw(minXY, maxXY);
  vec4f screenSizeVisible = v_sub(maxXY, minXY);
  screenSizeVisible = v_cmp_ge(threshold, screenSizeVisible);
  #if _TARGET_SIMD_SSE
  if ((_mm_movemask_ps(screenSizeVisible)&3) != 0)
    return 0;
  #else
  screenSizeVisible = v_or(screenSizeVisible, v_rot_1(screenSizeVisible));
  if (!v_test_vec_x_eq_0(screenSizeVisible))
    return 0;
  #endif
  return 1;
}

///return zero if not visible in frustum or if bbox screen size is smaller, than threshold. 1, if all vertex are in front of near plane, -1 (and fullscreen rect) otherwise
// also, screen_box is minX, maxX, minY, maxY - in clipspace coordinates  (-1, -1) .. (1,1), minmax_w is wWwW (minw, maxw, minw, maxw)
VECMATH_FINLINE int VECTORCALL
  v_screen_size_b(vec3f bmin, vec3f bmax, vec3f threshold, vec4f &screen_box, vec4f &minmax_w, mat44f_cref clip)
{
  // get aabb points (SoA)
  vec4f minmax_x = v_perm_xXxX(bmin, bmax);
  vec4f minmax_y = v_perm_yyYY(bmin, bmax);
  vec4f minmax_z_0 = v_splat_z(bmin);
  vec4f minmax_z_1 = v_splat_z(bmax);

  // transform points to clip space
  vec4f points_cs_0[4];
  vec4f points_cs_1[4];

  vis_transform_points_4(points_cs_0, minmax_x, minmax_y, minmax_z_0, clip);
  vis_transform_points_4(points_cs_1, minmax_x, minmax_y, minmax_z_1, clip);

  // calculate -w
  vec4f points_cs_0_negw = v_neg(points_cs_0[3]);
  vec4f points_cs_1_negw = v_neg(points_cs_1[3]);

#if _TARGET_SIMD_SSE
  #define NOUT(a, b, c, d) (unsigned(-_mm_movemask_ps(v_or(v_cmp_gt(a, b), v_cmp_gt(c, d)))))
  unsigned nout;
  nout = (NOUT(points_cs_0[0], points_cs_0_negw, points_cs_1[0], points_cs_1_negw));
  nout &= (NOUT(points_cs_0[3], points_cs_0[0], points_cs_1[3], points_cs_1[0]));
  nout &= (NOUT(points_cs_0[1], points_cs_0_negw, points_cs_1[1], points_cs_1_negw));
  nout &= (NOUT(points_cs_0[3], points_cs_0[1], points_cs_1[3], points_cs_1[1]));
  nout &= (NOUT(points_cs_0[2], v_zero(), points_cs_1[2], v_zero()));
  nout &= (NOUT(points_cs_0[3], points_cs_0[2], points_cs_1[3], points_cs_1[2]));

  // merge "not outside" flags
  if ((nout&(1<<31)) == 0)
    return 0;

#else
  #define NOUT(a, b, c, d) vis_hor_or_ff_0(v_or(v_cmp_gt(a, b), v_cmp_gt(c, d)))
  vec4f nout0 = NOUT(points_cs_0[0], points_cs_0_negw, points_cs_1[0], points_cs_1_negw);
  vec4f nout1 = NOUT(points_cs_0[3], points_cs_0[0], points_cs_1[3], points_cs_1[0]);
  vec4f nout2 = NOUT(points_cs_0[1], points_cs_0_negw, points_cs_1[1], points_cs_1_negw);
  vec4f nout3 = NOUT(points_cs_0[3], points_cs_0[1], points_cs_1[3], points_cs_1[1]);
  vec4f nout4 = NOUT(points_cs_0[2], v_zero(), points_cs_1[2], v_zero());
  vec4f nout5 = NOUT(points_cs_0[3], points_cs_0[2], points_cs_1[3], points_cs_1[2]);

  // merge "not outside" flags
  vec4f nout01 = v_and(nout0, nout1);
  vec4f nout012 = v_and(nout01, nout2);

  vec4f nout34 = v_and(nout3, nout4);
  vec4f nout345 = v_and(nout34, nout5);

  vec4f nout = v_and(nout012, nout345);

  if (v_test_vec_x_eq_0(nout)) //"not outside"=0 -> outside=1
    return 0;

#endif
  #undef NOUT
  vec4f min_w = v_min(points_cs_0[3], points_cs_1[3]);
  min_w = v_min(min_w, v_rot_2(min_w));
  min_w = v_min(min_w, v_rot_1(min_w));

  vec4f max_w = v_max(points_cs_0[3], points_cs_1[3]);
  max_w = v_max(max_w, v_rot_2(max_w));
  minmax_w = v_perm_xaxa(min_w, v_max(max_w, v_rot_1(max_w)));

  vec4f eps = *(vec4f*)v_screen_div_eps;
  if (v_test_vec_x_lt(min_w, eps))
  {
    screen_box = *(vec4f*)v_screen_full_screen;
    return -1;
  }

  vec4f inv_cs0_3 = v_rcp(points_cs_0[3]);
  vec4f inv_cs1_3 = v_rcp(points_cs_1[3]);
  vec4f xxxx0 = v_mul(points_cs_0[0], inv_cs0_3);
  vec4f xxxx1 = v_mul(points_cs_1[0], inv_cs1_3);
  vec4f yyyy0 = v_mul(points_cs_0[1], inv_cs0_3);
  vec4f yyyy1 = v_mul(points_cs_1[1], inv_cs1_3);

  vec4f point01 = v_merge_hw(xxxx0, yyyy0);//xy, xy
  vec4f point23 = v_merge_lw(xxxx0, yyyy0);//xy, xy
  vec4f point45 = v_merge_hw(xxxx1, yyyy1);//xy, xy
  vec4f point67 = v_merge_lw(xxxx1, yyyy1);//xy, xy
  vec4f minXY = v_min(v_min(point01, point23), v_min(point45, point67));
  minXY = v_min(minXY, v_rot_2(minXY));
  vec4f maxXY = v_max(v_max(point01, point23), v_max(point45, point67));
  maxXY = v_max(maxXY, v_rot_2(maxXY));

  screen_box = v_merge_hw(minXY, maxXY);
  vec4f screenSizeVisible = v_sub(maxXY, minXY);
  screenSizeVisible = v_cmp_ge(threshold, screenSizeVisible);
  #if _TARGET_SIMD_SSE
  if ((_mm_movemask_ps(screenSizeVisible)&3) != 0)
    return 0;
  #else
  screenSizeVisible = v_or(screenSizeVisible, v_rot_1(screenSizeVisible));
  if (!v_test_vec_x_eq_0(screenSizeVisible))
    return 0;
  #endif
  return 1;
}

//universal visibility function (accepts worldviewproj matrix)

///return zero if not visible in frustum or if bbox screen size is smaller, than threshold. not zero, otherwise
// also, screen_box is minX, maxX, minY, maxY - in clipspace coordinates  (-1, -1) .. (1,1)

VECMATH_FINLINE int VECTORCALL v_screen_size_b(vec3f box2_xyXY, vec3f threshold, vec4f &screen_box, mat44f_cref clip)
{
  // get aabb points (SoA)
  vec4f minmax_x = v_perm_xzxz(box2_xyXY);
  vec4f minmax_y = v_perm_yyww(box2_xyXY);

  // transform points to clip space
  vec4f points_cs_0[4];

  vis_transform_points_4(points_cs_0, minmax_x, minmax_y, clip);

  // calculate -w
  vec4f points_cs_0_negw = v_neg(points_cs_0[3]);

#if _TARGET_SIMD_SSE
  #define NOUT(a, b) (unsigned(-_mm_movemask_ps(v_cmp_gt(a, b))))
  unsigned nout;
  nout = (NOUT(points_cs_0[0], points_cs_0_negw));
  nout &= (NOUT(points_cs_0[3], points_cs_0[0]));
  nout &= (NOUT(points_cs_0[1], points_cs_0_negw));
  nout &= (NOUT(points_cs_0[3], points_cs_0[1]));
  nout &= (NOUT(points_cs_0[2], v_zero()));
  nout &= (NOUT(points_cs_0[3], points_cs_0[2]));

  // merge "not outside" flags
  if ((nout&(1<<31)) == 0)
    return 0;

#else
  #define NOUT(a, b) vis_hor_or_ff_0(v_cmp_gt(a, b))
  vec4f nout0 = NOUT(points_cs_0[0], points_cs_0_negw);
  vec4f nout1 = NOUT(points_cs_0[3], points_cs_0[0]);
  vec4f nout2 = NOUT(points_cs_0[1], points_cs_0_negw);
  vec4f nout3 = NOUT(points_cs_0[3], points_cs_0[1]);
  vec4f nout4 = NOUT(points_cs_0[2], v_zero());
  vec4f nout5 = NOUT(points_cs_0[3], points_cs_0[2]);

  // merge "not outside" flags
  vec4f nout01 = v_and(nout0, nout1);
  vec4f nout012 = v_and(nout01, nout2);

  vec4f nout34 = v_and(nout3, nout4);
  vec4f nout345 = v_and(nout34, nout5);

  vec4f nout = v_and(nout012, nout345);

  if (v_test_vec_x_eq_0(nout)) //"not outside"=0 -> outside=1
    return 0;

#endif
  #undef NOUT
  vec4f eps = *(vec4f*)v_screen_div_eps;
  vec4f valid_cs = v_cmp_gt(points_cs_0[3], eps);

  #if _TARGET_SIMD_SSE
  if (_mm_movemask_ps(valid_cs) != 15)
  #else
  valid_cs = v_and(valid_cs, v_rot_1(valid_cs));//xy, yz, zw, wx
  valid_cs = v_and(valid_cs, v_rot_2(valid_cs));//xyzw,yzwx, zwxy, wxyz
  if (v_test_vec_x_eq_0(valid_cs))
  #endif
  {
    screen_box = *(vec4f*)v_screen_full_screen;
    return -1;
  }

  vec4f inv_cs0_3 = v_rcp(points_cs_0[3]);
  vec4f xxxx0 = v_mul(points_cs_0[0], inv_cs0_3);
  vec4f yyyy0 = v_mul(points_cs_0[1], inv_cs0_3);

  vec4f point01 = v_merge_hw(xxxx0, yyyy0);//xy, xy
  vec4f point23 = v_merge_lw(xxxx0, yyyy0);//xy, xy
  vec4f minXY = v_min(point01, point23);
  minXY = v_min(minXY, v_rot_2(minXY));
  vec4f maxXY = v_max(point01, point23);
  maxXY = v_max(maxXY, v_rot_2(maxXY));

  screen_box = v_merge_hw(minXY, maxXY);
  vec4f screenSizeVisible = v_sub(maxXY, minXY);
  screenSizeVisible = v_cmp_ge(threshold, screenSizeVisible);
  #if _TARGET_SIMD_SSE
  if ((_mm_movemask_ps(screenSizeVisible)&3) != 0)
    return 0;
  #else
  screenSizeVisible = v_or(screenSizeVisible, v_rot_1(screenSizeVisible));
  if (!v_test_vec_x_eq_0(screenSizeVisible))
    return 0;
  #endif
  return 1;
}


//just a bit faster then scalar version. The same speed, if you'll have 4 v_ld. use if you have vectors
VECMATH_INLINE int VECTORCALL v_test_segment_box_intersection_dir(vec3f start, vec3f dir, bbox3f_cref box)
{
  // avoid using pair of v_div (due to compiler may change them uncontrollably to v_mul(a, v_rcp(b))
  // and thus get NaN instead of expected +inf and -inf)
  vec3f rcp_dir = v_sel(v_rcp(dir), V_C_MAX_VAL, v_cmp_eq(dir, v_zero()));
  vec3f l1 = v_mul(v_sub(box.bmin, start), rcp_dir);
  vec3f l2 = v_mul(v_sub(box.bmax, start), rcp_dir);
  vec3f lmin = v_min(l1, l2), lmax = v_max(l1, l2);
  //x
  vec3f lmin0 = v_perm_yzxw(lmin), lmax0 = v_perm_yzxw(lmax);
  //lmax' = max(x, y), max(y,z), max(z,x)
  lmax = v_min(lmax0, lmax);
  lmin = v_max(lmin0, lmin);

  lmin0 = v_perm_yzxw(lmin), lmax0 = v_perm_yzxw(lmax);
  //lmax'' = max(max(x,y), max(y,z)), max(max(y,z), max(z,x)), max(max(z,x), max(y,x))
  lmax = v_min(lmax0, lmax);
  lmin = v_max(lmin0, lmin);
  #if !_TARGET_SIMD_SSE
  vec4f result = v_and(v_and(v_cmp_ge(lmax, v_zero()), v_cmp_ge(lmax, lmin)), v_cmp_ge(V_C_ONE, lmin));
  result = v_splat_x(result);
  return !v_test_vec_x_eq_0(result);
  #else
  vec4f result = v_and(v_and(_mm_cmpge_ss(lmax, v_zero()), _mm_cmpge_ss(lmax, lmin)), _mm_cmpge_ss(V_C_ONE, lmin));
  return _mm_movemask_ps(result)&1;
  #endif
}

VECMATH_FINLINE int VECTORCALL v_test_segment_box_intersection(vec3f start, vec3f end, bbox3f_cref box)
{ return v_test_segment_box_intersection_dir(start, v_sub(end, start), box); }


//Point3 pc = sphere_center - p;
//real t = min(max(pc*normalized_dir, 0), mint);
//return lengthSq(normalized_dir*t - pc) < r2;
VECMATH_FINLINE bool VECTORCALL v_test_segment_sphere_intersection_dir_t(vec3f p0,
                                            vec3f normalized_dir,
                                            vec4f len,
                                            const vec4f &sphere_center__sr2)
{
  vec3f ap = v_sub(sphere_center__sr2, p0);
  vec4f t = v_min (v_max(v_dot3(ap, normalized_dir), v_zero()), len);
  return v_test_vec_x_le(v_length3_sq(v_sub(v_mul(normalized_dir, t), ap)), v_splat_w(sphere_center__sr2));
}

//actually the same speed as scalar version! (could be a bit faster, if true, and not early true)
// use only if you are working with vectors
///returns 0 if hit, 0xFF otherwise
#if _TARGET_SIMD_SSE
VECMATH_INLINE vec4f VECTORCALL v_test_segment_sphere_intersection_dir(vec3f p0,
                                            vec3f dir,
                                            vec3f sphere_center,
                                            const vec4f &squared_sphere_radius)
#else
VECMATH_INLINE vec4f VECTORCALL v_test_segment_sphere_intersection_dir(vec3f p0,
                                            vec3f dir,
                                            vec3f sphere_center,
                                            vec4f squared_sphere_radius)
#endif
{
  vec3f ap = v_sub(sphere_center, p0);
  vec4f lenSq = v_length3_sq(dir);
  vec4f t = v_min(v_max(v_div(v_dot3(ap, dir), v_max(lenSq, v_splats(0.00001f))), v_zero()), V_C_ONE);
  return v_cmp_gt(v_length3_sq(v_sub(v_mul(dir, t), ap)), squared_sphere_radius);
}

#if _TARGET_SIMD_SSE
VECMATH_FINLINE bool VECTORCALL v_test_segment_sphere_intersection(vec3f p0,
                                              vec3f p1,
                                              vec3f sphere_center,
                                              const vec4f &squared_sphere_radius)
#else
//actually just a be a bit faster than scalar version! use only if you are working with vectors already
VECMATH_FINLINE bool VECTORCALL v_test_segment_sphere_intersection(vec3f p0,
                                              vec3f p1,
                                              vec3f sphere_center,
                                              vec4f squared_sphere_radius)
#endif
{
  vec3f dir = v_sub(p1, p0);
  vec4f lenSq = v_length3_sq(dir);
  vec3f ap = v_sub(sphere_center, p0);
  vec4f t = v_min(v_max(v_div(v_dot3(ap, dir), v_max(lenSq, v_splats(0.00001f))), v_zero()), V_C_ONE);
  return v_test_vec_x_le(v_length3_sq(v_sub(v_mul(dir,t), ap)), squared_sphere_radius);
}

#if _TARGET_SIMD_SSE
VECMATH_FINLINE bool VECTORCALL v_test_segment_sphere_intersection_dir_b(vec3f p0,
                                              vec3f dir,
                                              vec3f sphere_center,
                                              const vec4f &squared_sphere_radius)
#else
//actually just a be a bit faster than scalar version! use only if you are working with vectors already
VECMATH_FINLINE bool VECTORCALL v_test_segment_sphere_intersection_dir_b(vec3f p0,
                                              vec3f dir,
                                              vec3f sphere_center,
                                              vec4f squared_sphere_radius)
#endif
{
  vec3f ap = v_sub(sphere_center, p0);
  vec4f lenSq = v_length3_sq(dir);
  vec4f t = v_min(v_max(v_div(v_dot3(ap, dir), v_max(lenSq, v_splats(0.00001f))), v_zero()), V_C_ONE);
  return v_test_vec_x_le(v_length3_sq(v_sub(v_mul(dir,t), ap)), squared_sphere_radius);
}

#if _TARGET_SIMD_SSE
VECMATH_INLINE bool VECTORCALL v_test_ray_sphere_intersection_b(vec3f p0,
                                            vec3f normalized_dir,
                                            vec3f sphere_center,
                                            const vec4f &squared_sphere_radius)
#else
VECMATH_INLINE bool VECTORCALL v_test_ray_sphere_intersection_b(vec3f p0,
                                            vec3f normalized_dir,
                                            vec3f sphere_center,
                                            vec4f squared_sphere_radius)
#endif
{

  vec3f ap = v_sub(sphere_center, p0);
  vec4f t = v_max(v_dot3(ap, normalized_dir), v_zero());
  return v_test_vec_x_le(v_length3_sq(v_sub(v_mul(normalized_dir, t), ap)), squared_sphere_radius);
}

VECMATH_FINLINE void VECTORCALL v_mat_43ca_from_mat44(float * __restrict m43, const mat44f &tm)
{
  vec4f c0 = tm.col0;
  vec4f c1 = tm.col1;
  vec4f c2 = tm.col2;
  vec4f c3 = tm.col3;
  //overlapped writes
  v_st(m43, c0);
  v_stu(((char *)(m43+3)), c1);
  v_stu(((char *)(m43+6)), c2);
  alignas(16) float vc4[4];
  v_st(vc4, c3);
  m43[ 9] = vc4[0];
  m43[10] = vc4[1];
  m43[11] = vc4[2];
}

VECMATH_FINLINE void VECTORCALL v_mat_43cu_from_mat44(float * __restrict m43, const mat44f &tm)
{
  vec4f c0 = tm.col0;
  vec4f c1 = tm.col1;
  vec4f c2 = tm.col2;
  vec4f c3 = tm.col3;
  //overlapped writes
  v_stu(m43, c0);
  v_stu(((char *)(m43+3)), c1);
  v_stu(((char *)(m43+6)), c2);
  alignas(16) float vc4[4];
  v_st(vc4, c3);
  m43[ 9] = vc4[0];
  m43[10] = vc4[1];
  m43[11] = vc4[2];
}


//mat44f from unaligned TMatrix
VECMATH_FINLINE void VECTORCALL v_mat44_make_from_43cu(mat44f &tmV, const float *const __restrict m43)
{
  tmV.col0 = v_and(v_ldu(m43+0), (vec4f)V_CI_MASK1110);
  tmV.col1 = v_and(v_ldu(m43+3), (vec4f)V_CI_MASK1110);
  tmV.col2 = v_and(v_ldu(m43+6), (vec4f)V_CI_MASK1110);
  tmV.col3 = v_add(v_and(v_ldu(m43+9), (vec4f)V_CI_MASK1110), V_C_UNIT_0001);
}

//mat44f from aligned TMatrix
VECMATH_FINLINE void VECTORCALL v_mat44_make_from_43ca(mat44f &tmV, const float *const __restrict m43)
{
  tmV.col0 = v_and(v_ld(m43+0), (vec4f)V_CI_MASK1110);
  tmV.col1 = v_and(v_ldu(m43+3), (vec4f)V_CI_MASK1110);
  tmV.col2 = v_and(v_ldu(m43+6), (vec4f)V_CI_MASK1110);
  tmV.col3 = v_add(v_and(v_ldu(m43+9), (vec4f)V_CI_MASK1110), V_C_UNIT_0001);
}

VECMATH_FINLINE vec4f VECTORCALL v_div_est(vec4f a, vec4f b) {return v_mul(a, v_rcp_est(b));}
#if _TARGET_SIMD_SSE
VECMATH_FINLINE vec4f VECTORCALL is_neg_special(vec4f a) {return v_cast_vec4f(v_srai(v_cast_vec4i(a), 31));}
#else
VECMATH_FINLINE vec4f VECTORCALL is_neg_special(vec4f a) {vec4f msbit = v_msbit(); return v_cmp_eq_w(v_and(a, msbit), msbit);}
#endif
#define REPLICATE4(v) {v, v, v, v}
alignas(16)static const float V_C_MINUS_ONE[4] = REPLICATE4(-1.f);
alignas(16)static const float _ps_atan_t0[4] = REPLICATE4(-0.91646118527267623468e-1f);
alignas(16)static const float _ps_atan_t1[4] = REPLICATE4(-0.13956945682312098640e1f);
alignas(16)static const float _ps_atan_t2[4] = REPLICATE4(-0.94393926122725531747e2f);
alignas(16)static const float _ps_atan_t3[4] = REPLICATE4( 0.12888383034157279340e2f);

alignas(16)static const float _ps_atan_s0[4] = REPLICATE4(0.12797564625607904396e1f);
alignas(16)static const float _ps_atan_s1[4] = REPLICATE4(0.21972168858277355914e1f);
alignas(16)static const float _ps_atan_s2[4] = REPLICATE4(0.68193064729268275701e1f);
alignas(16)static const float _ps_atan_s3[4] = REPLICATE4(0.28205206687035841409e2f);
alignas(16)static const float _ps_am_pi_o_2[4] = REPLICATE4(1.570796326794895f);

static const vec4f_const _pi          = v_cast_vec4f(v_splatsi(0x40490fdb)); // 3.141593f
static const vec4f_const _half_pi     = v_cast_vec4f(v_splatsi(0x3fc90fdb));
static const vec4f_const _neg_pi      = v_neg(_pi);
static const vec4f_const _neg_half_pi = v_neg(_half_pi);

#undef REPLICATE4
//approximate atan |error| is < 0.00045
VECMATH_INLINE vec4f VECTORCALL v_atan_est(vec4f x)  // any x
{
  vec4f xRcp = v_rcp_est(x);

  vec4f isOut1m1 = v_or(v_cmp_gt(x, V_C_ONE), v_cmp_ge(*(vec4f*)V_C_MINUS_ONE, x));
  vec4f xUsed = v_sel(x, xRcp, isOut1m1);

  vec4f xUsedSq = v_mul(xUsed, xUsed);
  vec4f atanPoly;
  atanPoly = v_add(xUsedSq, *(vec4f*)_ps_atan_s0);
  atanPoly = v_mul(v_rcp_est(atanPoly), *(vec4f*)_ps_atan_t0);

  atanPoly = v_add(atanPoly, v_add(xUsedSq, *(vec4f*)_ps_atan_s1));
  atanPoly = v_mul(v_rcp_est(atanPoly), *(vec4f*)_ps_atan_t1);

  atanPoly = v_add(atanPoly, v_add(xUsedSq, *(vec4f*)_ps_atan_s2));
  atanPoly = v_mul(v_rcp_est(atanPoly), *(vec4f*)_ps_atan_t2);

  atanPoly = v_add(  atanPoly, v_add(xUsedSq, *(vec4f*)_ps_atan_s3));
  atanPoly = v_mul(v_rcp_est(atanPoly), v_mul(xUsed, *(vec4f*)_ps_atan_t3));

  vec4f res = v_or(v_and(xUsed, v_msbit()), *(vec4f*)_ps_am_pi_o_2);
  res = v_sub(res, atanPoly);

  return v_sel(atanPoly, res, isOut1m1);
}

// fast atan version. |error| is < 0.000007
VECMATH_INLINE vec4f VECTORCALL v_atan(vec4f x)  // any x
{
  vec4f xRcp = v_rcp(x);

  vec4f isOut1m1 = v_or(v_cmp_gt(x, V_C_ONE), v_cmp_ge(*(vec4f*)V_C_MINUS_ONE, x));
  vec4f xUsed = v_sel(x, xRcp, isOut1m1);

  vec4f xUsedSq = v_mul(xUsed, xUsed);
  vec4f atanPoly;
  atanPoly = v_add(xUsedSq, *(vec4f*)_ps_atan_s0);
  atanPoly = v_mul(v_rcp(atanPoly), *(vec4f*)_ps_atan_t0);

  atanPoly = v_add(atanPoly, v_add(xUsedSq, *(vec4f*)_ps_atan_s1));
  atanPoly = v_mul(v_rcp(atanPoly), *(vec4f*)_ps_atan_t1);

  atanPoly = v_add(atanPoly, v_add(xUsedSq, *(vec4f*)_ps_atan_s2));
  atanPoly = v_mul(v_rcp(atanPoly), *(vec4f*)_ps_atan_t2);

  atanPoly = v_add(  atanPoly, v_add(xUsedSq, *(vec4f*)_ps_atan_s3));
  atanPoly = v_mul(v_rcp(atanPoly), v_mul(xUsed, *(vec4f*)_ps_atan_t3));

  vec4f res = v_or(v_and(xUsed, v_msbit()), *(vec4f*)_ps_am_pi_o_2);
  res = v_sub(res, atanPoly);

  return v_sel(atanPoly, res, isOut1m1);
}

// fast atan2 version. |error| is < 0.000007
VECMATH_INLINE vec4f VECTORCALL v_atan2(vec4f y, vec4f x)
{
  vec4f pi = v_cast_vec4f(v_splatsi(0x40490fdb));        // 3.141593f


  // compute the atan
  vec4f raw_atan = v_atan(v_div(y, x));

  vec4f neg_x = is_neg_special(x);
  vec4f neg_y = is_neg_special(y);

  vec4f in_quad2 = v_andnot(neg_y, neg_x);
  vec4f quad2_fixed = v_sel(raw_atan, v_add(raw_atan, pi), in_quad2);


  // move from quadrant 1 to 3 by subtracting PI
  vec4f in_quad3 = v_and(neg_x, neg_y);
  vec4f quad23_fixed = v_sel(quad2_fixed, v_sub(raw_atan, pi), in_quad3);



  vec4f y_zero = v_cmp_eq(x, v_zero());
  vec4f halfpi = v_cast_vec4f(v_splatsi(0x3fc90fdb));
  vec4f yzeropos_fixed = v_sel(quad23_fixed, halfpi, v_and(y_zero, v_cmp_gt(y, v_zero())));
  vec4f yzeroneg_fixed = v_sel(yzeropos_fixed, v_neg(halfpi), v_and(y_zero, v_cmp_ge(v_zero(), y)));
  vec4f x_yzero = v_andnot(v_and(y_zero, v_cmp_eq(y, v_zero())), yzeroneg_fixed);


  return x_yzero;
}

// fast approx atan version. |error| is < 0.0004
// ~50% faster then v_atan
// NOTE: does not handle any of the following inputs:
// (+0, +0), (+0, -0), (-0, +0), (-0, -0)
// could be fixed to handle
VECMATH_INLINE vec4f VECTORCALL v_atan2_est(vec4f y, vec4f x)
{
  vec4f pi = v_cast_vec4f(v_splatsi(0x40490fdb));        // 3.141593f


  // compute the atan
  vec4f raw_atan = v_atan_est(v_div_est(y, x));

  vec4f neg_x = is_neg_special(x);
  vec4f neg_y = is_neg_special(y);

  vec4f in_quad2 = v_andnot(neg_y, neg_x);
  vec4f quad2_fixed = v_sel(raw_atan, v_add(raw_atan, pi), in_quad2);


  // move from quadrant 1 to 3 by subtracting PI
  vec4f in_quad3 = v_and(neg_x, neg_y);
  vec4f quad23_fixed = v_sel(quad2_fixed, v_sub(raw_atan, pi), in_quad3);



  vec4f y_zero = v_cmp_eq(x, v_zero());
  vec4f halfpi = v_cast_vec4f(v_splatsi(0x3fc90fdb));
  vec4f yzeropos_fixed = v_sel(quad23_fixed, halfpi, v_and(y_zero, v_cmp_gt(y, v_zero())));
  vec4f yzeroneg_fixed = v_sel(yzeropos_fixed, v_neg(halfpi), v_and(y_zero, v_cmp_ge(v_zero(), y)));


  return yzeroneg_fixed;
}


VECMATH_INLINE vec4f VECTORCALL v_asin(vec4f a)
{
  vec4f divisor = v_sqrt4(v_sub(V_C_ONE, v_mul(a,a)));
  return v_sel(v_atan(v_div(a, divisor)), _neg_half_pi, v_cmp_eq(divisor, v_zero()));
}

VECMATH_INLINE vec4f VECTORCALL v_acos(vec4f a)
{
  vec4f one = V_C_ONE;
  vec4f divisor = v_add(one, a);
  a = v_sqrt4(v_div(v_sub(one, a), divisor));
  a = v_atan(a);
  return v_sel(v_add(a, a), _neg_pi, v_cmp_ge(v_zero(), divisor));
}

VECMATH_INLINE vec4f VECTORCALL v_asin_x(vec4f a)
{
  vec4f divisor = v_sqrt_x(v_sub_x(V_C_ONE, v_mul_x(a,a)));
  return v_sel(v_atan(v_div_x(a, divisor)), _neg_half_pi, v_cmp_eq(divisor, v_zero()));
}

VECMATH_INLINE vec4f VECTORCALL v_acos_x(vec4f a)
{
  vec4f one = V_C_ONE;
  vec4f divisor = v_add_x(one, a);
  a = v_sqrt_x(v_div_x(v_sub_x(one, a), divisor));
  a = v_atan(a);
  return v_sel(v_add_x(a, a), _neg_pi, v_cmp_ge(v_zero(), divisor));
}


#define POLY0(x, c0) v_splats(c0)
#define POLY1(x, c0, c1) v_add(v_mul(POLY0(x, c1), x), v_splats(c0))
#define POLY2(x, c0, c1, c2) v_add(v_mul(POLY1(x, c1, c2), x), v_splats(c0))
#define POLY3(x, c0, c1, c2, c3) v_add(v_mul(POLY2(x, c1, c2, c3), x), v_splats(c0))
#define POLY4(x, c0, c1, c2, c3, c4) v_add(v_mul(POLY3(x, c1, c2, c3, c4), x), v_splats(c0))
#define POLY5(x, c0, c1, c2, c3, c4, c5) v_add(v_mul(POLY4(x, c1, c2, c3, c4, c5), x), v_splats(c0))

#define EXP_DEF_PART\
   vec4i ipart;\
   vec4f fpart, expipart, expfpart;\
   x = v_min(x, v_splats( 129.00000f));\
   x = v_max(x, v_splats(-126.99999f));\
   ipart = v_cvt_roundi(v_sub(x, V_C_HALF));\
   fpart = v_sub(x, v_cvt_vec4f(ipart));\
   expipart = v_cast_vec4f(v_slli(v_addi(ipart, v_splatsi(127)), 23));\

   /* minimax polynomial fit of 2**x, in range [-0.5, 0.5[ */
VECMATH_INLINE vec4f VECTORCALL v_exp2_est_p5(vec4f x)
{
  EXP_DEF_PART
  expfpart = POLY5(fpart, 9.9999994e-1f, 6.9315308e-1f, 2.4015361e-1f, 5.5826318e-2f, 8.9893397e-3f, 1.8775767e-3f);
  return v_mul(expipart, expfpart);
}

VECMATH_INLINE vec4f VECTORCALL v_exp2_est_p4(vec4f x)
{
  EXP_DEF_PART
  expfpart = POLY4(fpart, 1.0000026f, 6.9300383e-1f, 2.4144275e-1f, 5.2011464e-2f, 1.3534167e-2f);
  return v_mul(expipart, expfpart);
}
VECMATH_INLINE vec4f VECTORCALL v_exp2_est_p3(vec4f x)
{
  EXP_DEF_PART
  expfpart = POLY3(fpart, 9.9992520e-1f, 6.9583356e-1f, 2.2606716e-1f, 7.8024521e-2f);
  return v_mul(expipart, expfpart);
}
VECMATH_INLINE vec4f VECTORCALL v_exp2_est_p2(vec4f x)
{
  EXP_DEF_PART
  expfpart = POLY2(fpart, 1.0017247f, 6.5763628e-1f, 3.3718944e-1f);
  return v_mul(expipart, expfpart);
}

VECMATH_INLINE vec4f VECTORCALL v_exp2(vec4f x)
{
  EXP_DEF_PART
  expfpart = POLY5(fpart, 9.9999994e-1f, 6.9315308e-1f, 2.4015361e-1f, 5.5826318e-2f, 8.9893397e-3f, 1.8775767e-3f);
  return v_sel(v_mul(expipart, expfpart), expipart, v_cmp_eq(fpart, v_zero()));//ensure that exp2(int) = 2^int
}

#undef EXP_DEF_PART

#define LOG_DEF_PART\
   static const vec4i exp = v_splatsi(0x7F800000);\
   static const vec4i mant = v_splatsi(0x007FFFFF);\
   vec4i i = v_cast_vec4i(x);\
   vec4f e = v_cvt_vec4f(v_subi(v_srli(v_andi(i, exp), 23), v_splatsi(127)));\
   vec4f m = v_or(v_cast_vec4f(v_andi(i, mant)), V_C_ONE);\
   vec4f p;

VECMATH_INLINE vec4f VECTORCALL v_log2_est_p5(vec4f x)
{
   LOG_DEF_PART
   p = POLY5( m, 3.1157899f, -3.3241990f, 2.5988452f, -1.2315303f,  3.1821337e-1f, -3.4436006e-2f);
   /* This effectively increases the polynomial degree by one, but ensures that log2(1) == 0*/
   return v_add(v_mul(p, v_sub(m, V_C_ONE)), e);
}

VECMATH_INLINE vec4f VECTORCALL v_log2_est_p4(vec4f x)
{
   LOG_DEF_PART
   p = POLY4(m, 2.8882704548164776201f, -2.52074962577807006663f,
                1.48116647521213171641f, -0.465725644288844778798f, 0.0596515482674574969533f);
   return v_add(v_mul(p, v_sub(m, V_C_ONE)), e);
}
VECMATH_INLINE vec4f VECTORCALL v_log2_est_p3(vec4f x)
{
   LOG_DEF_PART
   p = POLY3(m, 2.61761038894603480148f, -1.75647175389045657003f, 0.688243882994381274313f, -0.107254423828329604454f);
   return v_add(v_mul(p, v_sub(m, V_C_ONE)), e);
}
VECMATH_INLINE vec4f VECTORCALL v_log2_est_p2(vec4f x)
{
   LOG_DEF_PART
   p = POLY2(m, 2.28330284476918490682f, -1.04913055217340124191f, 0.204446009836232697516f);
   return v_add(v_mul(p, v_sub(m, V_C_ONE)), e);
}

#undef LOG_DEF_PART

VECMATH_INLINE vec4f VECTORCALL v_exp2_est(vec4f x) {return v_exp2_est_p4(x);}
VECMATH_INLINE vec4f VECTORCALL v_log2_est(vec4f x) {return v_log2_est_p4(x);}

VECMATH_FINLINE vec4f VECTORCALL v_pow_est(vec4f x, vec4f y)
{
   return v_exp2_est_p4(v_mul(v_log2_est_p5(x), y));
}
//natural log
VECMATH_FINLINE vec4f VECTORCALL v_log(vec4f x){return v_mul(v_log2_est_p5(x), v_splats(0.6931471805599453f));}
//natural exponent
VECMATH_FINLINE vec4f VECTORCALL v_exp(vec4f x){return v_exp2(v_mul(x, v_splats(1.4426950408889634073599f)));}//log2(e)
//safer pow. checks for y == 0
VECMATH_FINLINE vec4f VECTORCALL v_pow(vec4f x, vec4f y)
{
   vec4f ret = v_exp2(v_mul(v_log2_est_p5(x), y));
   ret = v_sel(ret, V_C_ONE, v_cmp_eq(y, v_zero()));
   return ret;
}

#undef POLY0
#undef POLY1
#undef POLY2
#undef POLY3
#undef POLY4
#undef POLY5

#if !_TARGET_SIMD_SSE && !_TARGET_SIMD_NEON
VECMATH_FINLINE vec4f VECTORCALL v_distance3p_x(plane3f a, vec3f b) {return v_distance3p(a,b);}
#endif

VECMATH_FINLINE plane3f VECTORCALL v_make_plane_dir(vec3f p0, vec3f dir0, vec3f dir1)
{
  vec3f n = v_cross3(dir0, dir1);
  return v_make_plane_norm(p0, n);
}
VECMATH_FINLINE plane3f VECTORCALL v_make_plane(vec3f p0, vec3f p1, vec3f p2)
{
  return v_make_plane_dir(p0, v_sub(p1,p0), v_sub(p2, p0));
}

VECMATH_FINLINE plane3f VECTORCALL v_make_plane_norm(vec3f p0, vec3f norm)
{
  vec3f d = v_neg(v_dot3(norm, p0));
  return v_perm_xyzd(norm, d);
}

VECMATH_FINLINE plane3f VECTORCALL v_transform_plane(plane3f plane, mat44f_cref transform)
{
  // simple way
  vec3f p0 = v_mul(plane, v_neg(v_splat_w(plane))); // p0 = norm * -d
  return v_make_plane_norm(v_mat44_mul_vec3p(transform, p0), v_mat44_mul_vec3v(transform, plane));
}

VECMATH_FINLINE vec4f VECTORCALL v_distance_sq_to_bbox_x(vec4f bmin, vec4f bmax, vec4f c)
{
  vec4f dist = v_sub(bmin, c);
  vec4f dmin3 = v_and(v_cmp_gt(bmin, c), v_mul(dist,dist));

  dist = v_sub(bmax, c);
  dmin3 = v_add(dmin3, v_and(v_cmp_gt(c, bmax), v_mul(dist,dist)));
  return v_add_x(v_add_x(dmin3, v_splat_z(dmin3)), v_splat_y(dmin3));
}

VECMATH_FINLINE vec4f VECTORCALL v_distance_sq_to_bbox_2d_x(vec4f bmin, vec4f bmax, vec4f c)
{
  vec4f dist = v_sub(bmin, c);
  vec4f dmin3 = v_and(v_cmp_gt(bmin, c), v_mul(dist,dist));

  dist = v_sub(bmax, c);
  dmin3 = v_add(dmin3, v_and(v_cmp_gt(c, bmax), v_mul(dist,dist)));
  return v_add_x(dmin3, v_splat_z(dmin3));
}

VECMATH_FINLINE vec4f VECTORCALL v_max_dist_sq_to_bbox_x(vec4f bmin, vec4f bmax, vec4f c)
{
  vec4f dist1 = v_sub(bmin, c);
  dist1 = v_mul(dist1, dist1);
  vec4f dist2 = v_sub(bmax, c);
  dist2 = v_mul(dist2, dist2);
  dist2 = v_max(dist2, dist1);
  return v_add_x(v_add_x(dist2, v_splat_z(dist2)), v_splat_y(dist2));
}

VECMATH_FINLINE vec3f VECTORCALL v_closest_bbox_point(vec3f bmin, vec3f bmax, vec3f c)
{
  c = v_sel(c, bmin, v_cmp_gt(bmin, c));
  c = v_sel(c, bmax, v_cmp_gt(c, bmax));
  return c;
}


//returns point on infinite line which is closes to point
VECMATH_FINLINE vec3f VECTORCALL closest_point_on_line(vec3f point, vec3f a, vec3f dir)
{
  vec3f t = v_dot3(v_sub(point, a), dir);// t param along line
  return v_madd(dir, t, a);//pt is point on line
}

//returns point on segment which is closes to point
VECMATH_FINLINE vec3f VECTORCALL closest_point_on_seg(vec3f point, vec3f a, vec3f b)
{
  vec3f dir = v_sub(b, a);
  vec4f dirN = v_length3(dir);
  dir = v_mul(dir, v_splat_x(v_rcp_x(dirN)));
  vec3f t = v_dot3(v_sub(point, a), dir);// t param along line
  t = v_min(v_max(t, v_zero()), dirN);//clamp param
  return v_madd(dir, t, a);
};

VECMATH_FINLINE vec4f VECTORCALL distance_to_line_x(vec3f point, vec3f a, vec3f dir)
{
  vec3f pa = v_sub(point, a);
  vec3f t = v_dot3(pa, dir);// t param along line
  return v_length3_x(v_sub(pa, v_mul(dir, t)));
};

VECMATH_FINLINE vec4f VECTORCALL distance_to_seg_x(vec3f point, vec3f a, vec3f b)
{
  vec3f pt = closest_point_on_seg(point, a, b);
  return v_length3_x(v_sub(point, pt));
};


VECMATH_FINLINE vec4i VECTORCALL v_float_to_half(vec4f v)
{
  vec4i fltInt32 = v_cast_vec4i(v);
  vec4i fltInt16 = v_slli(v_srli(fltInt32, 31), 5);
  vec4i tmp = v_andi(v_srli(fltInt32, 23), V_CI_0FF);
  tmp = v_andi( v_subi(tmp, V_CI_070), v_srli(v_srai(v_subi(V_CI_070, tmp), 4), 27));
  fltInt16 = v_slli(v_ori(fltInt16, tmp), 10);
  fltInt16 = v_ori(fltInt16, v_andi(v_srli(fltInt32, 13), V_CI_3FF));
  return fltInt16;
}

VECMATH_FINLINE void VECTORCALL v_float_to_half(uint16_t* __restrict m, const vec4f v)
{
  v_stu_w_half(m, v_packus(v_float_to_half(v)));
}

VECMATH_FINLINE vec4f VECTORCALL v_half_to_float(const uint16_t* __restrict m)
{
  vec4i v_in = v_lduush((const unsigned short*)m);
  vec4i fltInt16 = v_in;
  vec4i fltInt32 = v_slli(v_srli(fltInt16, 15), 8);
  vec4i  tmp = v_andi(v_srli(fltInt16, 10), V_CI_01F);
  tmp = v_addi(tmp, V_CI_070);
  fltInt32 = v_slli(v_ori(fltInt32, tmp), 23);
  fltInt32 = v_ori(fltInt32, v_andi(v_slli(fltInt16, 13), V_CI_07FFFFF));
  return v_cast_vec4f(fltInt32);
}

VECMATH_FINLINE uint32_t v_float_to_byte ( vec4f x )
{
    vec4i y = v_cvt_roundi(x);
    y = v_packs(y);
    y = v_packus16(y, y);
    return v_extract_xi(y);
}

VECMATH_FINLINE vec4f v_byte_to_float ( uint32_t x )
{
    vec4i y = v_splatsi(x);
    y = v_cvt_byte_vec4i(y);
    y = v_cvt_ush_vec4i(y);
    return v_cvt_vec4f(y);
}

VECMATH_INLINE vec3f VECTORCALL v_triangle_bounding_sphere_center( const vec3f& p1, const vec3f& p2, const vec3f& p3 )
{
  vec3f edge1 = v_sub(p2, p1);
  vec3f edge2 = v_sub(p3, p1);
  vec3f edge3 = v_sub(p2, p3);

  vec4f nd1 = v_dot3(edge1, edge2);
  vec4f nd2 = v_dot3(edge3, edge1);
  vec4f nd3 = v_neg(v_dot3(edge2, edge3));
  vec3f d1, d2;
  vec4f mask1, mask;
  d1 = d2 = v_zero();
  mask1 = v_cmp_gt(v_zero(), nd1);
  d1 = v_and(mask1, p3);
  d2 = v_and(mask1, edge3);

  mask = v_cmp_gt(v_zero(), nd2);
  d1 = v_sel(d1, p1, mask);
  d2 = v_sel(d2, edge2, mask);
  mask1 = v_or(mask1, mask);

  mask = v_cmp_gt(v_zero(), nd3);
  d1 = v_sel(d1, p1, mask);
  d2 = v_sel(d2, edge1, mask);
  mask1 = v_or(mask1, mask);
  if (!v_test_vec_x_eq_0(mask1))
    return v_add(d1, v_mul(d2, V_C_HALF));//radius = 0.5*length(d2);
  vec3f c1  = v_mul(nd2, nd3);
  vec3f c2  = v_mul(nd3, nd1);
  vec3f c3  = v_mul(nd1, nd2);
  vec3f cmul2   = v_add(v_add(c1, c2), c3);
  cmul2 = v_add(cmul2, cmul2);

  return v_div(v_add(
                     v_madd(p1, v_add(c2, c3), v_mul(p2, v_add(c3, c1))),
                     v_mul(p3, v_add(c1, c2))
                     ),
               cmul2);//r       = 0.5f * sqrt( fabsf(safediv((nd1 + nd2) * (nd2 + nd3) * (nd3 + nd1), c)) );
}

//this is ~3 times faster for valid floats (not nans, infs, etc), than int(floorf())
VECMATH_INLINE int VECTORCALL vec_floori(float x)
{
  return v_extract_xi(v_cvt_floori(v_splats(x)));
}

//this is ~2 times faster for valid floats (not nans, infs, etc), than int(floorf())
VECMATH_INLINE int VECTORCALL vec_float_to_int(float x)
{
  return v_extract_xi(v_cvt_vec4i(v_splats(x)));
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
