/*
 * Dagor Engine 5
 * Copyright (C) 2003-2021  Gaijin Entertainment.  All rights reserved
 *
 * (for conditions of distribution and use, see License)
*/

#ifndef _DAGOR_PUBLIC_MATH_DAG_VECMATH_H_
#define _DAGOR_PUBLIC_MATH_DAG_VECMATH_H_
#pragma once


#include "dag_vecMathDecl.h"
#include <stdint.h>
//
// basic arithmetic
//

//! 0
VECMATH_FINLINE vec4f VECTORCALL v_zero();
//! 0x80000000
VECMATH_FINLINE vec4f VECTORCALL v_msbit();
//! .xyzw = a[0]
VECMATH_FINLINE vec4f VECTORCALL v_splat4(const float *a);
//! load vector from 16-byte aligned memory
NO_ASAN_INLINE vec4f v_ld(const float *m);
NO_ASAN_INLINE vec4i v_ldi(const int *m);
//! load vector from unaligned memory
NO_ASAN_INLINE vec4f v_ldu(const float *m);
NO_ASAN_INLINE vec4i v_ldui(const int *m);
//! load one 32 bit element, zero others
NO_ASAN_INLINE vec4f v_ldu_x(const float *m);
//! load 3x32 bit elements fast by one 4x32 bit load, but safe for thread sanitizer
NO_ASAN_INLINE vec3f v_ldu_p3(const float *m);
//! load 3x32 bit elements safe, use it only when v_ldu_p3 can cause memory access crashes
NO_ASAN_INLINE vec3f v_ldu_p3_safe(const float *m);
//! load unaligned memory and unpacks vector from 4 signed short ints
VECMATH_FINLINE vec4i VECTORCALL v_ldush(const signed short *m);
//! load unaligned memory and unpacks vector from 4 unsigned short ints
VECMATH_FINLINE vec4i VECTORCALL v_lduush(const unsigned short *m);

//! load 64 bit from unaligned memory into .xy (.zw=0)
VECMATH_FINLINE vec4i VECTORCALL v_ldui_half(const void *m);
VECMATH_FINLINE vec4f VECTORCALL v_ldu_half(const void *m);

//! fetch the cache line that contains address m
VECMATH_FINLINE void v_prefetch(const void *m);

//! .xyzw = a.x
VECMATH_FINLINE vec4f VECTORCALL v_splat_x(vec4f a);
VECMATH_FINLINE vec4i VECTORCALL v_splat_xi(vec4i a);
//! .xyzw = a.y
VECMATH_FINLINE vec4f VECTORCALL v_splat_y(vec4f a);
VECMATH_FINLINE vec4i VECTORCALL v_splat_yi(vec4i a);
//! .xyzw = a.z
VECMATH_FINLINE vec4f VECTORCALL v_splat_z(vec4f a);
VECMATH_FINLINE vec4i VECTORCALL v_splat_zi(vec4i a);
//! .xyzw = a.w
VECMATH_FINLINE vec4f VECTORCALL v_splat_w(vec4f a);
VECMATH_FINLINE vec4i VECTORCALL v_splat_wi(vec4i a);

//! .xyzw = a
VECMATH_FINLINE vec4f VECTORCALL v_splats(float a);
VECMATH_FINLINE vec4i VECTORCALL v_splatsi(int a);
VECMATH_FINLINE vec4i VECTORCALL v_splatsi64(int64_t a);

//! .xyzw = {a 0 0 0}
VECMATH_FINLINE vec4f VECTORCALL v_set_x(float a);
VECMATH_FINLINE vec4i VECTORCALL v_seti_x(int a);

//! .xyzw = {x y z w}
VECMATH_FINLINE vec4f VECTORCALL v_make_vec4f(float x, float y, float z, float w);
VECMATH_FINLINE vec4i VECTORCALL v_make_vec4i(int x, int y, int z, int w);

//! store vector to  16-byte aligned memory
VECMATH_FINLINE void VECTORCALL v_st(void *m, vec4f v);
//! store vector to unaligned memory
VECMATH_FINLINE void VECTORCALL v_stu(void *m, vec4f v);
//! store low 64 bits of vector (.xy) to unaligned memory
VECMATH_FINLINE void VECTORCALL v_stu_half(void *m, vec4f v);

//! store vector to  16-byte aligned memory
VECMATH_FINLINE void VECTORCALL v_sti(void *m, vec4i v);
//! store vector to unaligned memory
VECMATH_FINLINE void VECTORCALL v_stui(void *m, vec4i v);
//! store low 64 bits of vector (.xy) to unaligned memory
VECMATH_FINLINE void VECTORCALL v_stui_half(void *m, vec4i v);

//! merge high words: .xyzw = {a.x, b.x, a.y, b.y}
VECMATH_FINLINE vec4f VECTORCALL v_merge_hw(vec4f a, vec4f b);
//! merge low words:  .xyzw = {a.z, b.z, a.w, b.w}
VECMATH_FINLINE vec4f VECTORCALL v_merge_lw(vec4f a, vec4f b);

//! return signbit mask for each compnent - 1|2|4|8. ith bit is ith float signbit
VECMATH_FINLINE int VECTORCALL v_signmask(vec4f a);

//! component-wise comparison: for C={xyzw}  .C = a.C==b.C ? 0xFFFFFFFF : 0
VECMATH_FINLINE vec4f VECTORCALL v_cmp_eq(vec4f a, vec4f b);
//! component-wise integer comparison: for C={xyzw}  .C = a.C==b.C ? 0xFFFFFFFF : 0
VECMATH_FINLINE vec4f VECTORCALL v_cmp_eqi(vec4f a, vec4f b);
VECMATH_FINLINE vec4i VECTORCALL v_cmp_eqi(vec4i a, vec4i b);
//! component-wise comparison: for C={xyzw}  .C = a.C>=b.C ? 0xFFFFFFFF : 0
VECMATH_FINLINE vec4f VECTORCALL v_cmp_ge(vec4f a, vec4f b);
//! component-wise comparison: for C={xyzw}  .C = a.C>b.C ? 0xFFFFFFFF : 0
VECMATH_FINLINE vec4f VECTORCALL v_cmp_gt(vec4f a, vec4f b);
VECMATH_FINLINE vec4i VECTORCALL v_cmp_gti(vec4i a, vec4i b);
VECMATH_FINLINE vec4i VECTORCALL v_cmp_lti(vec4i a, vec4i b);

//! a & b
VECMATH_FINLINE vec4f VECTORCALL v_and(vec4f a, vec4f b);
//! ~a & b
VECMATH_FINLINE vec4f VECTORCALL v_andnot(vec4f a, vec4f b);
VECMATH_FINLINE vec4i VECTORCALL v_andnoti(vec4i a, vec4i b);
//! a | b
VECMATH_FINLINE vec4f VECTORCALL v_or(vec4f a, vec4f b);
//! a ^ b
VECMATH_FINLINE vec4f VECTORCALL v_xor(vec4f a, vec4f b);
//! bit-wise select: for N={0..127}  .bitN = (c.bitN == 0) ? a.bitN : b.bitN
VECMATH_FINLINE vec4f VECTORCALL v_sel(vec4f a, vec4f b, vec4f c);
VECMATH_FINLINE vec4i VECTORCALL v_seli(vec4i a, vec4i b, vec4i c);

//! convert to integer using cast
VECMATH_FINLINE vec4i VECTORCALL v_cast_vec4i(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_cast_vec4f(vec4i a);

//! convert to integer using round-to-zero mode
VECMATH_FINLINE vec4i VECTORCALL v_cvt_vec4i(vec4f a);
//! convert to float
VECMATH_FINLINE vec4f VECTORCALL v_cvt_vec4f(vec4i a);

//! unpacks 4 unsigned shorts (in low 64 bits of vector, .xy) to 4 ints
VECMATH_FINLINE vec4i VECTORCALL v_cvt_ush_vec4i(vec4i a);
//! unpacks 4 signed shorts (in low 64 bits of vector, .xy) to 4 ints
VECMATH_FINLINE vec4i VECTORCALL v_cvt_ssh_vec4i(vec4i a);
//! unpacks 8 unsigned bytes (in low 64 bits of vector, .xy) to 4 shorts
VECMATH_FINLINE vec4i VECTORCALL v_cvt_byte_vec4i(vec4i a);

//! converts float vector to 4 halfs and stores(unaligned)
VECMATH_FINLINE void VECTORCALL v_float_to_half(uint16_t* __restrict m, const vec4f v);
//! reads(unaligned) and unpacks 4 halfs into float vector
VECMATH_FINLINE vec4f VECTORCALL v_half_to_float(const uint16_t* __restrict m);

//! converts float vector to 4 halfs (lower 16 bits of vec4i)
VECMATH_FINLINE vec4i VECTORCALL v_float_to_half(vec4f v);

//! pack float vector to 4 bytes with saturation
VECMATH_FINLINE uint32_t v_float_to_byte ( vec4f x );

//! unpacks 4 bytes to float vector
VECMATH_FINLINE vec4f v_byte_to_float ( uint32_t x );

//! round to biggest integer (result remains fp)
VECMATH_FINLINE vec4f VECTORCALL v_ceil(vec4f a);

//! round to smallest integer (result remains fp)
VECMATH_FINLINE vec4f VECTORCALL v_floor(vec4f a);

//! round to biggest integer (result remains int)
VECMATH_FINLINE vec4i VECTORCALL v_cvt_ceili(vec4f a);

//! round to smallest integer (result remains int)
VECMATH_FINLINE vec4i VECTORCALL v_cvt_floori(vec4f a);

//! round to nearest integer (result remains int)
VECMATH_FINLINE vec4i VECTORCALL v_cvt_roundi(vec4f a);

//! round to nearest integer (result remains fp)
VECMATH_FINLINE vec4f VECTORCALL v_round(vec4f a);
//! (a + b)
VECMATH_FINLINE vec4f VECTORCALL v_add(vec4f a, vec4f b);
//! (a - b)
VECMATH_FINLINE vec4f VECTORCALL v_sub(vec4f a, vec4f b);
//! (a * b)
VECMATH_FINLINE vec4f VECTORCALL v_mul(vec4f a, vec4f b);
//! (a / b)
VECMATH_FINLINE vec4f VECTORCALL v_div(vec4f a, vec4f b);
//! (a % b) (float)
VECMATH_FINLINE vec4f VECTORCALL v_mod(vec4f a, vec4f b);
//! (a * b + c)
VECMATH_FINLINE vec4f VECTORCALL v_madd(vec4f a, vec4f b, vec4f c);
//! -(a * b - c) == c - a*b
VECMATH_FINLINE vec4f VECTORCALL v_nmsub(vec4f a, vec4f b, vec4f c);
//! .x = (a.x + b.x)
VECMATH_FINLINE vec4f VECTORCALL v_add_x(vec4f a, vec4f b);
//! .x = (a.x - b.x)
VECMATH_FINLINE vec4f VECTORCALL v_sub_x(vec4f a, vec4f b);
//! .x = (a.x * b.x)
VECMATH_FINLINE vec4f VECTORCALL v_mul_x(vec4f a, vec4f b);
//! .x = (a.x / b.x)
VECMATH_FINLINE vec4f VECTORCALL v_div_x(vec4f a, vec4f b);
//! .x = (a.x * b.x + c.x)
VECMATH_FINLINE vec4f VECTORCALL v_madd_x(vec4f a, vec4f b, vec4f c);
//! 1/a, fast estimate
VECMATH_FINLINE vec4f VECTORCALL v_rcp_est(vec4f a);
//! 1/a
VECMATH_FINLINE vec4f VECTORCALL v_rcp(vec4f a);
//! .x = 1/a.x, fast estimate
VECMATH_FINLINE vec4f VECTORCALL v_rcp_est_x(vec4f a);
//! .x = 1/a.x
VECMATH_FINLINE vec4f VECTORCALL v_rcp_x(vec4f a);
//! min(a,b)
VECMATH_FINLINE vec4f VECTORCALL v_min(vec4f a, vec4f b);
VECMATH_FINLINE vec4i VECTORCALL v_mini(vec4i a, vec4i b);
//! max(a,b)
VECMATH_FINLINE vec4f VECTORCALL v_max(vec4f a, vec4f b);
VECMATH_FINLINE vec4i VECTORCALL v_maxi(vec4i a, vec4i b);
//! -a
VECMATH_FINLINE vec4f VECTORCALL v_neg(vec4f a);
VECMATH_FINLINE vec4i VECTORCALL v_negi(vec4i a);
//! fabs(a)
VECMATH_FINLINE vec4f VECTORCALL v_abs(vec4f a);
VECMATH_FINLINE vec4i VECTORCALL v_absi(vec4i a);

//! LERP a to b using parameter tttt
VECMATH_FINLINE quat4f VECTORCALL v_lerp_vec4f(vec4f tttt, quat4f a, quat4f b);

//! (a + b)
VECMATH_FINLINE vec4i VECTORCALL v_addi(vec4i a, vec4i b);
//! (a - b)
VECMATH_FINLINE vec4i VECTORCALL v_subi(vec4i a, vec4i b);
//! (a * b), signed integers. result is only correctly defined if both operands and result is within [-1<<31, 1<<31) range
VECMATH_FINLINE vec4i VECTORCALL v_muli(vec4i a, vec4i b);

//! shift left (unsigned integer). bits is immediate value
VECMATH_FINLINE vec4i VECTORCALL v_slli(vec4i v, int bits);
//! shift right (unsigned integer). bits is immediate value
VECMATH_FINLINE vec4i VECTORCALL v_srli(vec4i v, int bits);
//! shift right (signed integer). bits is immediate value
VECMATH_FINLINE vec4i VECTORCALL v_srai(vec4i v, int bits);
//! shift left (unsigned integer). bits is variative value
VECMATH_FINLINE vec4i VECTORCALL v_slli_n(vec4i v, int bits);
//! shift right (unsigned integer). bits is variative value
VECMATH_FINLINE vec4i VECTORCALL v_srli_n(vec4i v, int bits);
//! shift right (signed integer). bits is variative value
VECMATH_FINLINE vec4i VECTORCALL v_srai_n(vec4i v, int bits);
//! shift left (unsigned integer). bits is variative 64-bit value
VECMATH_FINLINE vec4i VECTORCALL v_slli_n(vec4i v, vec4i bits);
//! shift left (unsigned integer). bits is variative 64-bit value
VECMATH_FINLINE vec4i VECTORCALL v_srli_n(vec4i v, vec4i bits);
//! shift right (signed integer). bits is variative 64-bit value
VECMATH_FINLINE vec4i VECTORCALL v_srai_n(vec4i v, vec4i bits);

//! shift left (unsigned integer)
VECMATH_FINLINE vec4i VECTORCALL v_sll(vec4i v, int bits);
//! shift right (unsigned integer)
VECMATH_FINLINE vec4i VECTORCALL v_srl(vec4i v, int bits);
//! shift right (signed integer)
VECMATH_FINLINE vec4i VECTORCALL v_sra(vec4i v, int bits);

//! a | b (bitwise, integer)
VECMATH_FINLINE vec4i VECTORCALL v_ori(vec4i a, vec4i b);
//! a & b (bitwise, integer)
VECMATH_FINLINE vec4i VECTORCALL v_andi(vec4i a, vec4i b);
//! a ^ b (bitwise, integer)
VECMATH_FINLINE vec4i VECTORCALL v_xori(vec4i a, vec4i b);

//! 1/sqrt_est(a), fast estimate
VECMATH_FINLINE vec4f VECTORCALL v_rsqrt4_fast(vec4f a);
//! 1/sqrt(a), Newton-Raphson refinement
VECMATH_FINLINE vec4f VECTORCALL v_rsqrt4(vec4f a);
//! .x = 1/sqrt_est(a)
VECMATH_FINLINE vec4f VECTORCALL v_rsqrt_fast_x(vec4f a);
//! .x = 1/sqrt(a)
VECMATH_FINLINE vec4f VECTORCALL v_rsqrt_x(vec4f a);

//! sqrt_est(a), fast estimate
VECMATH_FINLINE vec4f VECTORCALL v_sqrt4_fast(vec4f a);
//! sqrt(a), Newton-Raphson refinement
VECMATH_FINLINE vec4f VECTORCALL v_sqrt4(vec4f a);
//! .x = sqrt_est(a.x)
VECMATH_FINLINE vec4f VECTORCALL v_sqrt_fast_x(vec4f a);
//! .x = sqrt(a.x)
VECMATH_FINLINE vec4f VECTORCALL v_sqrt_x(vec4f a);

//! cyclic rotate
VECMATH_FINLINE vec4f VECTORCALL v_rot_1(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_rot_2(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_rot_3(vec4f a);

//! permutations (mostly used in common implementation)
VECMATH_FINLINE vec4f VECTORCALL v_perm_yzwx(vec4f a); //< alias for v_rot_1()
VECMATH_FINLINE vec4f VECTORCALL v_perm_zwxy(vec4f a); //< alias for v_rot_2()
VECMATH_FINLINE vec4f VECTORCALL v_perm_wxyz(vec4f a); //< alias for v_rot_3()
VECMATH_FINLINE vec4f VECTORCALL v_perm_yzxw(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_perm_zxyw(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_perm_xzxz(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_perm_xxyy(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_perm_zzww(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_perm_xxzz(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_perm_yyww(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_perm_xyxy(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_perm_yzxx(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_perm_yzxy(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_perm_ywyw(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_perm_xyzz(vec4f a);

VECMATH_FINLINE vec4f VECTORCALL v_perm_xzac(vec4f xyzw, vec4f abcd);
VECMATH_FINLINE vec4f VECTORCALL v_perm_ywbd(vec4f xyzw, vec4f abcd);
VECMATH_FINLINE vec4f VECTORCALL v_perm_xyab(vec4f xyzw, vec4f abcd);
VECMATH_FINLINE vec4f VECTORCALL v_perm_zwcd(vec4f xyzw, vec4f abcd);
VECMATH_FINLINE vec4f VECTORCALL v_perm_xaxa(vec4f xyzw, vec4f abcd);
VECMATH_FINLINE vec4f VECTORCALL v_perm_yybb(vec4f xyzw, vec4f abcd);
VECMATH_FINLINE vec4f VECTORCALL v_perm_bbyx(vec4f xyzw, vec4f abcd);
VECMATH_FINLINE vec4f VECTORCALL v_perm_ayzw(vec4f xyzw, vec4f abcd);
VECMATH_FINLINE vec4f VECTORCALL v_perm_xbzw(vec4f xyzw, vec4f abcd);
VECMATH_FINLINE vec4f VECTORCALL v_perm_xycw(vec4f xyzw, vec4f abcd);
VECMATH_FINLINE vec4f VECTORCALL v_perm_xyzd(vec4f xyzw, vec4f abcd);

#define v_perm_xyXY v_perm_xyab
#define v_perm_zwZW v_perm_zwcd
#define v_perm_xXxX v_perm_xaxa
#define v_perm_yyYY v_perm_yybb
#define v_perm_xzXZ v_perm_xzac
#define v_perm_ywYW v_perm_ywbd
#define v_perm_YYyx v_perm_bbyx
#define v_perm_Xyzw v_perm_ayzw
#define v_perm_xYzw v_perm_xbzw
#define v_perm_xyZw v_perm_xycw
#define v_perm_xyzW v_perm_xyzd


//! pack 8x 32-bit ints into 8x 16-bit ints
VECMATH_FINLINE vec4i VECTORCALL v_packs(vec4i a, vec4i b);
//! pack 8x 32-bit ints (value being duplicated) into 8x 16-bit ints
VECMATH_FINLINE vec4i VECTORCALL v_packs(vec4i a);
//! pack 8x 32-bit ints into 8x unsigned 16-bit ints
VECMATH_FINLINE vec4i VECTORCALL v_packus(vec4i a, vec4i b);
//! pack 8x 32-bit ints (value being duplicated) into 8x unsigned 16-bit ints
VECMATH_FINLINE vec4i VECTORCALL v_packus(vec4i a);
//! pack 16x 16-bit ints into 16x unsigned 8-bit ints
VECMATH_FINLINE vec4i VECTORCALL v_packus16(vec4i a, vec4i b);
//! pack 16x 16-bit ints into 16x unsigned 8-bit ints
VECMATH_FINLINE vec4i VECTORCALL v_packus16(vec4i a);


//
// vector algebra
//

//! dot product: .xyzw = (a.x * b.x + a.y * b.y + a.z * b.z); a.w, b.w could be anything (even NAN)
VECMATH_FINLINE vec4f VECTORCALL v_dot3(vec4f a, vec4f b);
//! dot product: .xyzw = (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w)
VECMATH_FINLINE vec4f VECTORCALL v_dot4(vec4f a, vec4f b);
//! dot product: .x = (a.x * b.x + a.y * b.y + a.z * b.z); a.w, b.w could be anything (even NAN)
VECMATH_FINLINE vec4f VECTORCALL v_dot3_x(vec4f a, vec4f b);
//! dot product: .x = (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w)
VECMATH_FINLINE vec4f VECTORCALL v_dot4_x(vec4f a, vec4f b);
//! cross product: a x b; ; a.w,b.w could be anything, even NAN
VECMATH_FINLINE vec3f VECTORCALL v_cross3(vec3f a, vec3f b);

//! make plane from 3 points
VECMATH_FINLINE plane3f VECTORCALL v_make_plane(vec3f p0, vec3f p1, vec3f p2);

//! make plane from point and two dirs from point
VECMATH_FINLINE plane3f VECTORCALL v_make_plane_dir(vec3f p0, vec3f dir0, vec3f dir1);

//! make plane from point and normal of the plane
VECMATH_FINLINE plane3f VECTORCALL v_make_plane_norm(vec3f p0, vec3f norm);

//! transform plane with matrix
VECMATH_FINLINE plane3f VECTORCALL v_transform_plane(plane3f plane, mat44f_cref transform);

//! distance from point b to plane a: .xyzw = (a.x * b.x + a.y * b.y + a.z * b.z + a.w)
VECMATH_FINLINE vec4f VECTORCALL v_distance3p(plane3f a, vec3f b);

//! distance from point b to plane a: .x = (a.x * b.x + a.y * b.y + a.z * b.z + a.w)
VECMATH_FINLINE vec4f VECTORCALL v_distance3p_x(plane3f a, vec3f b);

//! scalar triple product: (a x b) * c = dot3(cross3(a, b), c)
VECMATH_FINLINE vec3f VECTORCALL v_striple3(vec3f a, vec3f b, vec3f c);
//! vector triple product: a x (b x c) = b(a*c) - c(a*b) = cross3(a, cross(b, c)
VECMATH_FINLINE vec3f VECTORCALL v_vtriple3(vec3f a, vec3f b, vec3f c);

//! length squared: .xyzw = a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w
VECMATH_FINLINE vec4f VECTORCALL v_length4_sq(vec4f a);
//! length squared: .xyzw = a.x*a.x + a.y*a.y + a.z*a.z
VECMATH_FINLINE vec3f VECTORCALL v_length3_sq(vec3f a);

//! length: .xyzw = sqrt_est(a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w)
VECMATH_FINLINE vec4f VECTORCALL v_length4_est(vec4f a);
//! estimate length: .xyzw = sqrt_est(a.x*a.x + a.y*a.y + a.z*a.z)
VECMATH_FINLINE vec3f VECTORCALL v_length3_est(vec3f a);
//! estimate length: .xyzw = sqrt(a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w)
VECMATH_FINLINE vec4f VECTORCALL v_length4(vec4f a);
//! length: .xyzw = sqrt(a.x*a.x + a.y*a.y + a.z*a.z)
VECMATH_FINLINE vec3f VECTORCALL v_length3(vec3f a);
//! length squared: .x = a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w
VECMATH_FINLINE vec4f VECTORCALL v_length4_sq_x(vec4f a);
//! length squared: .x = a.x*a.x + a.y*a.y + a.z*a.z, a.w could be anything (even NAN)
VECMATH_FINLINE vec3f VECTORCALL v_length3_sq_x(vec3f a);

//! estimate length: .x = sqrt_est(a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w)
VECMATH_FINLINE vec4f VECTORCALL v_length4_est_x(vec4f a);
//! estimate length: .x = sqrt_est(a.x*a.x + a.y*a.y + a.z*a.z), a.w could be anything (even NAN)
VECMATH_FINLINE vec3f VECTORCALL v_length3_est_x(vec3f a);
//! length: .x = sqrt(a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w)
VECMATH_FINLINE vec4f VECTORCALL v_length4_x(vec4f a);
//! length: .x = sqrt(a.x*a.x + a.y*a.y + a.z*a.z), a.w could be anything (even NAN)
VECMATH_FINLINE vec3f VECTORCALL v_length3_x(vec3f a);

//! normalize: a/length(a). will return NaN for zero vector
VECMATH_FINLINE vec4f VECTORCALL v_norm4(vec4f a);
//! normalize: a/length(a), .w could be anything (even NAN). will return NaN for zero vector
VECMATH_FINLINE vec3f VECTORCALL v_norm3(vec3f a);
//! nans converted to zero, v_and(a, v_cmp_eq(a,a))
VECMATH_FINLINE vec4f VECTORCALL v_remove_nan(vec4f a);
//! safe normalize: a/length(a)
//! result is not guaranteed to be normalized, but is definetly not NaN (NAN components will be zero)
VECMATH_FINLINE vec4f VECTORCALL v_norm4_safe(vec4f a);
//! safe normalize: a/length(a), .w could be anything (even NAN)
//! result is not guaranteed to be normalized, but is definetly not NaN (NAN components will be zero)
VECMATH_FINLINE vec3f VECTORCALL v_norm3_safe(vec3f a);


//
// vector transformations
//

//! m * v,  matrix is treated column-major
VECMATH_FINLINE vec4f VECTORCALL v_mat44_mul_vec4(mat44f_cref m, vec4f v);
//! m * v,  matrix is treated column-major, v.w=0
VECMATH_FINLINE vec4f VECTORCALL v_mat44_mul_vec3v(mat44f_cref m, vec3f v);
//! m * v,  matrix is treated column-major, v.w=1
VECMATH_FINLINE vec4f VECTORCALL v_mat44_mul_vec3p(mat44f_cref m, vec3f v);
//! m * v,  matrix is treated column-major
VECMATH_FINLINE vec3f VECTORCALL v_mat33_mul_vec3(mat33f_cref m, vec3f v);
//! m * v,  matrix is treated row-major, v.w=0
VECMATH_FINLINE vec3f VECTORCALL v_mat43_mul_vec3v(mat43f_cref m, vec3f v);
//! m * v,  matrix is treated row-major, v.w=1
VECMATH_FINLINE vec3f VECTORCALL v_mat43_mul_vec3p(mat43f_cref m, vec3f v);

//! q * v, rotate vector using normalized quaternion
VECMATH_FINLINE vec3f VECTORCALL v_quat_mul_vec3(quat4f q, vec3f v);

//! q1 * q2, effectively multiply rotations
VECMATH_FINLINE quat4f VECTORCALL v_quat_mul_quat(quat4f q1, quat4f q2);

//! compute conjugate quaternion
VECMATH_FINLINE quat4f VECTORCALL v_quat_conjugate(quat4f q);

//
// matrix algebra
//

//! identity matrix 4x4
VECMATH_FINLINE void VECTORCALL v_mat44_ident(mat44f &dest);
//! identity matrix 4x4 with swapped X and Z columns
VECMATH_FINLINE void VECTORCALL v_mat44_ident_swapxz(mat44f &dest);
//! identity matrix 3x3
VECMATH_FINLINE void VECTORCALL v_mat33_ident(mat33f &dest);
//! identity matrix 4x4 with swapped X and Z columns
VECMATH_FINLINE void VECTORCALL v_mat33_ident_swapxz(mat33f &dest);

//! make perspective matrix 4x4
VECMATH_FINLINE void VECTORCALL v_mat44_make_persp_reverse(mat44f &dest, float wk, float hk, float zn, float zf);//zn - 1, zf - 0
VECMATH_FINLINE void VECTORCALL v_mat44_make_persp_forward(mat44f &dest, float wk, float hk, float zn, float zf);//zn - 0, zf - 1
VECMATH_FINLINE void VECTORCALL v_mat44_make_persp(mat44f &dest, float wk, float hk, float zn, float zf);// same as reverse
//! make view matrix 3x3 from look_dir
VECMATH_FINLINE void VECTORCALL v_mat33_make_from_look(mat33f &dest, vec4f look_dir);
//! make view submatrix 3x3 of 4x3 from look_dir
VECMATH_FINLINE void VECTORCALL v_mat44_make33_from_look(mat44f &dest, vec4f look_dir);
VECMATH_FINLINE void VECTORCALL v_view_matrix_from_tangentZ(mat33f &dest, vec4f look_dir);
//! make view submatrix 3x3 of 4x3 from look_dir
VECMATH_FINLINE void VECTORCALL v_mat44_make_look_at(mat44f &dest, vec4f eye, vec4f at, vec4f up);
//! make orthogonal projection matrix
VECMATH_FINLINE void VECTORCALL v_mat44_make_ortho(mat44f &dest, float w, float h, float zn, float zf);

//! make rotational matrix 3x3 to rotate 'ang' radians around 'v' clockwise
VECMATH_FINLINE void VECTORCALL v_mat33_make_rot_cw(mat33f &dest, vec3f v, vec4f ang);
//! make rotational matrix 3x3 to rotate 'ang' radians around X axis clockwise
VECMATH_FINLINE void VECTORCALL v_mat33_make_rot_cw_x(mat33f &dest, vec4f ang);
//! make rotational matrix 3x3 to rotate 'ang' radians around Y axis clockwise
VECMATH_FINLINE void VECTORCALL v_mat33_make_rot_cw_y(mat33f &dest, vec4f ang);
//! make rotational matrix 3x3 to rotate 'ang' radians around Z axis clockwise
VECMATH_FINLINE void VECTORCALL v_mat33_make_rot_cw_z(mat33f &dest, vec4f ang);
//! make composite matrix 3x3: =rot_z(ang.z)*rot_y(ang.y)*rot_x(ang.x)
VECMATH_FINLINE void VECTORCALL v_mat33_make_rot_cw_zyx(mat33f &dest, vec4f ang_xyz);

//! T(m), 4x4 -> 4x4
VECMATH_FINLINE void VECTORCALL v_mat44_transpose(mat44f &dest, mat44f_cref src);
VECMATH_FINLINE void VECTORCALL v_mat44_transpose(vec4f &r0, vec4f &r1, vec4f &r2, vec4f &r3);
//! T(m), 3x3 -> 3x3
VECMATH_FINLINE void VECTORCALL v_mat33_transpose(mat33f &dest, vec3f src_col0, vec3f src_col1, vec3f src_col2);
VECMATH_FINLINE void VECTORCALL v_mat33_transpose(mat33f &dest, mat33f_cref src);
//! T(m), 4x4 -> 3x3, omitting last column. So we can make .w not zero
//VECMATH_FINLINE void VECTORCALL v_mat44_transpose_to_mat33(mat33f &dest, vec3f col0, vec3f col1, vec3f col2, vec4f col3);

//! T(m), 4x4 column major -> 4x3 row major
VECMATH_FINLINE void VECTORCALL v_mat44_transpose_to_mat43(mat43f &dest, mat44f_cref src);
//! T(m), 4x3 row major -> 4x4 column major
VECMATH_FINLINE void VECTORCALL v_mat43_transpose_to_mat44(mat44f &dest, mat43f_cref src);
//! complement mat33 to mat44 (col1.w=col2.w=col0.w=0, col3=0,0,0,1)
VECMATH_FINLINE void VECTORCALL v_mat44_from_mat33(mat44f &dest, mat33f_cref m);
//! extract rotation/scale transformation from mat44 to mat33
VECMATH_FINLINE void VECTORCALL v_mat33_from_mat44(mat33f &dest, mat44f_cref m);

//! make 3x3 rotation matrix from quaternion
VECMATH_FINLINE void VECTORCALL v_mat33_from_quat(mat33f &dest, quat4f q);
//! make 4x4 matrix from quaternion and position
VECMATH_FINLINE void VECTORCALL v_mat44_from_quat(mat44f &dest, quat4f q, vec4f pos);
//! compose 4x4 matrix from position/rotation/scale
VECMATH_FINLINE void VECTORCALL v_mat44_compose(mat44f &dest, vec4f pos, quat4f rot, vec4f scale);
//! compose 3x3 matrix from rotation/scale
VECMATH_FINLINE void VECTORCALL v_mat33_compose(mat33f &dest, quat4f rot, vec4f scale);

//! decompose 3x3 matrix to rotation/scale
VECMATH_FINLINE void VECTORCALL v_mat33_decompose(mat33f_cref &tm, quat4f &rot, vec4f &scl);
//! decompose 4x4 matrix to position/rotation/scale (assuming ordinary matrix 4x3)
VECMATH_FINLINE void VECTORCALL v_mat4_decompose(mat44f_cref &tm, vec3f &pos, quat4f &rot, vec4f &scl);

//! m1+m2
VECMATH_FINLINE void VECTORCALL v_mat44_add(mat44f &dest, mat44f_cref m1, mat44f_cref m2);
//! m1+m2
VECMATH_FINLINE void VECTORCALL v_mat33_add(mat33f &dest, mat33f_cref m1, mat33f_cref m2);
//! m1-m2
VECMATH_FINLINE void VECTORCALL v_mat44_sub(mat44f &dest, mat44f_cref m1, mat44f_cref m2);
//! m1-m2
VECMATH_FINLINE void VECTORCALL v_mat33_sub(mat33f &dest, mat33f_cref m1, mat33f_cref m2);
//! -m
VECMATH_FINLINE void VECTORCALL v_mat33_neg(mat44f &dest, mat44f_cref m);
//! -m
VECMATH_FINLINE void VECTORCALL v_mat33_neg(mat33f &dest, mat33f_cref m);
//! per-elem multiply m1 and m2
VECMATH_FINLINE void VECTORCALL v_mat44_mul_elem(mat44f &dest, mat44f_cref m1, mat44f_cref m2);
//! per-elem multiply m1 and m2
VECMATH_FINLINE void VECTORCALL v_mat33_mul_elem(mat33f &dest, mat33f_cref m1, mat33f_cref m2);

//! m1*m2
VECMATH_FINLINE void VECTORCALL v_mat44_mul(mat44f &dest, mat44f_cref m1, mat44f_cref m2);
//! m1*m2, assuming m2.col1.w=m2.col2.w=m2.col0.w=0, m2.col3=0,0,0,1
VECMATH_FINLINE void VECTORCALL v_mat44_mul43(mat44f &dest, mat44f_cref m1, mat44f_cref m2);
//! m1*m2, assuming m2.col1.w=m2.col2.w=m2.col0.w=0, m2.col3=0,0,0,1.
//Faster, then v_mat44_mul43(dest, m1, v_mat43_transpose_to_mat44(m2));
VECMATH_FINLINE void VECTORCALL v_mat44_mul43(mat44f &dest, mat44f_cref m1, mat43f_cref m2);
//! m1*m2, taking only rotational matrix from m2
VECMATH_FINLINE void VECTORCALL v_mat44_mul33r(mat44f &dest, mat44f_cref m1, mat44f_cref m2);
//! m1*m2, assuming m2 is pure rotational matrix 3x3
VECMATH_FINLINE void VECTORCALL v_mat44_mul33(mat44f &dest, mat44f_cref m1, mat33f_cref m2);
//! m1*m2
VECMATH_FINLINE void VECTORCALL v_mat33_mul(mat33f &dest, mat33f_cref m1, mat33f_cref m2);
//! m1*m2, taking only rotational matrix from m2
VECMATH_FINLINE void VECTORCALL v_mat33_mul33r(mat33f &dest, mat33f_cref m1, mat44f_cref m2);
//! orthonormalize 3x3 part of 4x4 matrix
VECMATH_FINLINE void VECTORCALL v_mat44_orthonormalize33(mat44f &dest, mat44f_cref m);
//! orthonormalize 3x3 matrix
VECMATH_FINLINE void VECTORCALL v_mat33_orthonormalize(mat33f &dest, mat33f_cref m);
//! 1/m
VECMATH_FINLINE void VECTORCALL v_mat44_inverse(mat44f &dest, mat44f_cref m);
//! 1/m, assuming col1.w=col2.w=col0.w=0, col3=0,0,0,1
VECMATH_FINLINE void VECTORCALL v_mat44_inverse43(mat44f &dest, mat44f_cref m);
//! 1/m
VECMATH_FINLINE void VECTORCALL v_mat33_inverse(mat33f &dest, mat33f_cref m);
//! 1/m == T(m), with m being orthonormal
VECMATH_FINLINE void VECTORCALL v_mat44_orthonormal_inverse43(mat44f &dest, mat44f_cref m);
//! 1/m == T(m), with m being orthonormal
VECMATH_FINLINE void VECTORCALL v_mat33_orthonormal_inverse(mat33f &dest, mat33f_cref m);
//! Determinant(m)
VECMATH_FINLINE vec4f VECTORCALL v_mat44_det(mat44f_cref m);
//! Determinant(m), assuming col1.w=col2.w=col0.w=0, col3=0,0,0,1
VECMATH_FINLINE vec4f VECTORCALL v_mat44_det43(mat44f_cref m);
//! Determinant(m)
VECMATH_FINLINE vec4f VECTORCALL v_mat33_det(mat33f_cref m);

//! stores mat44f to aligned TMatrix from mat44f
VECMATH_FINLINE void VECTORCALL v_mat_43ca_from_mat44(float * __restrict m43, const mat44f &tm);

//! stores mat44f to unaligned TMatrix
VECMATH_FINLINE void VECTORCALL v_mat_43cu_from_mat44(float * __restrict m43, const mat44f &tm);

//! mat44f from unaligned TMatrix
VECMATH_FINLINE void VECTORCALL v_mat44_make_from_43cu(mat44f &tmV, const float *const __restrict m43);

//! mat44f from memaligned TMatrix
VECMATH_FINLINE void VECTORCALL v_mat44_make_from_43ca(mat44f &tmV, const float *const __restrict m43);


//
// Axis-aligned bounding boxes
//

//! init as empty
VECMATH_FINLINE void VECTORCALL v_bbox3_init_empty(bbox3f &b);
//! init with point
VECMATH_FINLINE void VECTORCALL v_bbox3_init(bbox3f &b, vec3f p);
//! init with bb2 transformed with 4x4 matrix m
VECMATH_FINLINE void VECTORCALL v_bbox3_init(bbox3f &b, mat44f_cref m, bbox3f_cref bb2);

//! extend bbox to enclose point
VECMATH_FINLINE void VECTORCALL v_bbox3_add_pt(bbox3f &b, vec3f p);
//! extend bbox to enclose bb2
VECMATH_FINLINE void VECTORCALL v_bbox3_add_box(bbox3f &b, bbox3f_cref b2);
//! extend bbox to enclose bb2 transformed with 4x4 matrix m
VECMATH_FINLINE void VECTORCALL v_bbox3_add_transformed_box(bbox3f &b, mat44f_cref m, bbox3f_cref b2);

//! .xyz = bbox dimensions; for empty bbox dimensions will be invalid (negative)
VECMATH_FINLINE vec3f VECTORCALL v_bbox3_size(bbox3f_cref b);
//! .xyz = bbox center
VECMATH_FINLINE vec3f VECTORCALL v_bbox3_center(bbox3f_cref b);
//! .x = radius of outer sphere
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_outer_rad(vec3f bmin, vec3f bmax);
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_outer_rad(bbox3f_cref b);
//! .x = radius of inner sphere
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_inner_rad(vec3f bmin, vec3f bmax);
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_inner_rad(bbox3f_cref b);

//! returns point (0,0,0), b.bmin
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt000(bbox3f_cref b);
//! returns point (1,1,1), b.bmax
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt111(bbox3f_cref b);
//! returns point (0,0,1): .x = b.bmin.x; .y = b.bmin.y; .z = b.bmax.z
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt001(bbox3f_cref b);
//! returns point (0,1,0): .x = b.bmin.x; .y = b.bmax.y; .z = b.bmin.z
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt010(bbox3f_cref b);
//! returns point (0,1,1): .x = b.bmin.x; .y = b.bmax.y; .z = b.bmax.z
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt011(bbox3f_cref b);
//! returns point (1,0,0): .x = b.bmax.x; .y = b.bmin.y; .z = b.bmin.z
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt100(bbox3f_cref b);
//! returns point (1,0,1): .x = b.bmax.x; .y = b.bmin.y; .z = b.bmax.z
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt101(bbox3f_cref b);
//! returns point (1,1,0): .x = b.bmax.x; .y = b.bmax.y; .z = b.bmin.z
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_pt110(bbox3f_cref b);

//! tests whether point is inside box and returns boolean as mask of all 1s or 0s
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_test_pt_inside(bbox3f_cref b, vec3f p);
//! tests whether box b2 if fully is inside box and returns boolean as mask of all 1s or 0s
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_test_box_inside(bbox3f_cref b, bbox3f_cref b2);
//! tests whether boxes intersect and returns boolean as mask of all 1s or 0s.if one box is whole world, and the other is inverse full (empty) - returns non-intersection
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_test_box_intersect(bbox3f_cref b1, bbox3f_cref b2);

//! tests whether boxes intersect and returns boolean as mask of all 1s or 0s. safe for above case
VECMATH_FINLINE vec4f VECTORCALL v_bbox3_test_box_intersect_safe(bbox3f_cref b1, bbox3f_cref b2);

//! tests whether point is inside box and returns boolean (0 or non-0)
VECMATH_FINLINE int VECTORCALL v_bbox3_test_pt_inside_b(bbox3f_cref b, vec3f p);
//! tests whether point is inside box xz and returns boolean (0 or non-0)
VECMATH_FINLINE int VECTORCALL v_bbox3_test_pt_inside_b_xz(bbox3f_cref b, vec3f p);
//! tests whether box b2 is fully inside box and returns boolean (0 or non-0)
VECMATH_FINLINE int VECTORCALL v_bbox3_test_box_inside_b(bbox3f_cref b1, bbox3f_cref b2);
//! tests whether boxes intersect and returns boolean (0 or non-0). if one box is whole world, and the other is inverse full (empty) - returns non-intersection
VECMATH_FINLINE int VECTORCALL v_bbox3_test_box_intersect_b(bbox3f_cref b1, bbox3f_cref b2);

//! tests whether boxes intersect and returns boolean (0 or non-0). safe for above case
VECMATH_FINLINE int VECTORCALL v_bbox3_test_box_intersect_b_safe(bbox3f_cref b1, bbox3f_cref b2);

//! tests whether box intersecs sphere and returns boolean
VECMATH_FINLINE int VECTORCALL v_bbox3_test_sph_intersect(bbox3f_cref box, vec4f bsph_r2);

//! .x = squared MINIMUM distance from point c to bbox (bmin, bmax)
VECMATH_FINLINE vec4f VECTORCALL v_distance_sq_to_bbox_x(vec4f bmin, vec4f bmax, vec4f c);

//! .x = squared MINIMUM 2d (xz) distance from point c to bbox (bmin, bmax)
VECMATH_FINLINE vec4f VECTORCALL v_distance_sq_to_bbox_2d_x(vec4f bmin, vec4f bmax, vec4f c);

//! .x = squared MAXIMUM 3d distance from point c to bbox (bmin, bmax)
VECMATH_FINLINE vec4f VECTORCALL v_max_dist_sq_to_bbox_x(vec4f bmin, vec4f bmax, vec4f c);

//returns point on infinite line which is closes to point
VECMATH_FINLINE vec3f VECTORCALL closest_point_on_line(vec3f point, vec3f a, vec3f dir);
//returns point on segment which is closes to point
VECMATH_FINLINE vec3f VECTORCALL closest_point_on_seg(vec3f point, vec3f a, vec3f b);
//distance to line in x component
VECMATH_FINLINE vec4f VECTORCALL distance_to_line_x(vec3f point, vec3f a, vec3f dir);
//distance to segment in x component
VECMATH_FINLINE vec4f VECTORCALL distance_to_seg_x(vec3f point, vec3f a, vec3f b);
//get closest to c bbox point. return c, if c is inside bbox. undefined for empty boxes
VECMATH_FINLINE vec3f VECTORCALL v_closest_bbox_point(vec3f bmin, vec3f bmax, vec3f c);

//visibility
///construct 6 planes from worldviewproj matrix, with reversed z-buffer camPlanes5 far, camPlanes4 near
VECMATH_FINLINE void VECTORCALL v_construct_camplanes(mat44f_cref clip,
  vec4f &camPlanes0, vec4f &camPlanes1, vec4f &camPlanes2, vec4f &camPlanes3, vec4f &camPlanes4, vec4f &camPlanes5);

//v_frustum_box. assume it has correct frustum planes intersection.
VECMATH_FINLINE void VECTORCALL v_frustum_box_unsafe(bbox3f& box,
                                 vec4f camPlanes0, vec4f camPlanes1, vec4f camPlanes2,
                                 const vec4f &camPlanes3, const vec4f &camPlanes4, const vec4f &camPlanes5);

//0,1,2 returns if sphere is outside (0), inisde(1), intersect(2) frustum, accepts 4 transponed planes and two others in regular
VECMATH_FINLINE int VECTORCALL v_sphere_intersect(vec3f center, vec3f r,
                  vec3f plane03X, const vec4f& plane03Y, const vec4f& plane03Z, const vec4f& plane03W,
                  const vec4f& plane4, const vec4f& plane5);

//0,1 returns if sphere is visible in frustum, accepts 4 transponed planes and two others in regular
VECMATH_FINLINE int VECTORCALL v_is_visible_sphere(vec3f center, vec3f r,
                  vec4f plane03X, const vec4f& plane03Y, const vec4f& plane03Z, const vec4f& plane03W,
                  const vec4f& plane4, const vec4f& plane5);

///universal visibility function (accepts worldviewproj matrix)
///return zero if not visible. nonzero, otherwise
// center and extent should be multiplied by 2.
// it is fastest implementation
VECMATH_FINLINE int VECTORCALL v_is_visible_extent_fast(vec3f center, vec3f extent, mat44f_cref clip);

// same as above (and will call v_is_visible_extent_fast), but accepts bbox)
VECMATH_FINLINE int VECTORCALL v_is_visible_b_fast(vec3f bmin, vec3f bmax, mat44f_cref clip);

///universal visibility function (accepts worldviewproj matrix)
///return zero if not visible in frustum or if unclamped bbox screen size is smaller, than threshold. not zero, otherwise
// also, out screen_box is minX, maxX, minY, maxY - in clipspace coordinates  (-1, -1) .. (1,1)
VECMATH_FINLINE int VECTORCALL v_screen_size_b(vec3f bmin, vec3f bmax, vec3f threshold, vec4f &screen_box, mat44f_cref clip);

/// same as previous but return w min, w max
/// return zero if not visible in frustum or if bbox screen size is smaller, than threshold. 1, if all vertex are in front of near plane, -1 (and fullscreen rect) otherwise
/// also, screen_box is minX, maxX, minY, maxY - in clipspace coordinates  (-1, -1) .. (1,1), minmax_w is wWwW (minw, maxw, minw, maxw)
VECMATH_FINLINE int VECTORCALL
v_screen_size_b(vec3f bmin, vec3f bmax, vec3f threshold, vec4f &screen_box, vec4f &minmax_w, mat44f_cref clip);


///universal visibility function (accepts worldviewproj matrix)
///return zero if not visible. nonzero, otherwise
///it is slower than v_is_visible_b_fast, so do not call it if you don't know what are you doing
///for branching: (~v_test_vec_x_eq_0(v_is_visible(bmin, bmax, clip)))&1 will be 1 if visible, zero otherwise;
///or use v_is_visible_b (it is faster on SSE)
VECMATH_FINLINE vec4f VECTORCALL v_is_visible(vec3f bmin, vec3f bmax, mat44f_cref clip);


///for branching: (~v_test_vec_x_eq_0( v_is_visible(bmin, bmax, clip)))&1 will be 1 if visible, zero otherwise;
VECMATH_FINLINE int VECTORCALL v_is_visible_b(vec3f bmin, vec3f bmax, mat44f_cref clip);

//! gets perfect triangle bounding sphere center.
// Warning - can work incorrectly on degenerative triangles (can produce nans)
VECMATH_INLINE vec3f VECTORCALL v_triangle_bounding_sphere_center( const vec3f& p1, const vec3f& p2, const vec3f& p3 );

//
// Quaternion math
//

//! make (unnormalized) quaternion from 3x3 rotation matrix
VECMATH_FINLINE quat4f VECTORCALL v_un_quat_from_mat3(mat33f_cref m);
//! make (unnormalized) quaternion from rotation part of 4x4 matrix
VECMATH_FINLINE quat4f VECTORCALL v_un_quat_from_mat4(mat44f_cref m);
//! make (unnormalized) quaternion from rotation part of 4x4 matrix
VECMATH_FINLINE quat4f VECTORCALL v_un_quat_from_mat(vec3f col0, vec3f col1, vec3f col2);

//! make (unnormalized) quaternion to rotate 'ang' radians around normalized 'v';
VECMATH_FINLINE quat4f VECTORCALL v_un_quat_from_unit_vec_ang(vec3f v, vec4f ang);
//! make (unnormalized) quaternion to rotate 'v0' to 'v1'; both 'v0' and 'v1' must be normalized
VECMATH_FINLINE quat4f VECTORCALL v_un_quat_from_unit_arc(vec3f v0, vec3f v1);
//! make (unnormalized) quaternion to rotate 'v0' to 'v1'
VECMATH_FINLINE quat4f VECTORCALL v_un_quat_from_arc(vec3f v0, vec3f v1);

//! linear interpolation between normalized quaternions using parameter tttt
VECMATH_FINLINE quat4f VECTORCALL v_quat_lerp(vec4f tttt, quat4f a, quat4f b);
//! fast quasi-spherical linear interpolation between normalized quaternions
VECMATH_FINLINE quat4f VECTORCALL v_quat_qslerp(float t, quat4f a, quat4f b);
//! fast quasi-spherical quadrangle interpolation between normalized quaternions
VECMATH_FINLINE quat4f VECTORCALL v_quat_qsquad(float t,
  const quat4f &q0, const quat4f &q1, const quat4f &q2, const quat4f &q3);

//
// Trigonometry
//

//! compute sine and cosine for all components: for C={xyzw}  s.C = sin(a.C); c.C = cos(a.C);
VECMATH_FINLINE void VECTORCALL v_sincos4(vec4f a, vec4f& s, vec4f& c);

//! compute sine and cosine for .x: s.x = sin(a.x); c.x = cos(a.x);
VECMATH_FINLINE void VECTORCALL v_sincos_x(vec4f a, vec4f& s, vec4f& c);

//! compute sine, cosine, tangent or arc for all components
VECMATH_FINLINE vec4f VECTORCALL v_sin(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_cos(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_tan(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_asin(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_acos(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_atan(vec4f a);

//! compute sine, cosine, tangent or arc for .x component
VECMATH_FINLINE vec4f VECTORCALL v_sin_x(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_cos_x(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_tan_x(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_asin_x(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_acos_x(vec4f a);
VECMATH_FINLINE vec4f VECTORCALL v_atan_x(vec4f a);

// compute approximate atan |error| is < 0.00045
VECMATH_FINLINE vec4f VECTORCALL v_atan_est(vec4f x);  // any x

// compute approximate atan2 |error| is < 0.0004
// ~40% faster then v_atan2
// NOTE: does not handle any of the following inputs:
// (+0, +0), (+0, -0), (-0, +0), (-0, -0)
VECMATH_INLINE vec4f VECTORCALL v_atan2_est(vec4f y, vec4f x);

//exp,log, pow
VECMATH_INLINE vec4f VECTORCALL v_exp2_est(vec4f x);
VECMATH_INLINE vec4f VECTORCALL v_exp2_est_p2(vec4f x);//polynom of 2
VECMATH_INLINE vec4f VECTORCALL v_exp2_est_p3(vec4f x);
VECMATH_INLINE vec4f VECTORCALL v_exp2_est_p4(vec4f x);
VECMATH_INLINE vec4f VECTORCALL v_exp2_est_p5(vec4f x);//polynom of 5
VECMATH_INLINE vec4f VECTORCALL v_log2_est(vec4f x);
VECMATH_INLINE vec4f VECTORCALL v_log2_est_p2(vec4f x);//polynom of 2
VECMATH_INLINE vec4f VECTORCALL v_log2_est_p3(vec4f x);
VECMATH_INLINE vec4f VECTORCALL v_log2_est_p4(vec4f x);
VECMATH_INLINE vec4f VECTORCALL v_log2_est_p5(vec4f x);//polynom of 5

VECMATH_FINLINE vec4f VECTORCALL v_pow_est(vec4f x, vec4f y);////exp_polynom_of_4(log_polynom_of_5(x)*y)
//
// vector -> scalar transformation (often incurs penalty!)
//

//! extracts fp32 component of float[4] by index
VECMATH_FINLINE float VECTORCALL v_extract(vec4f v, int i);
VECMATH_FINLINE int VECTORCALL v_extracti(vec4i v, int i);
//! extracts fp32 x-component of float[4]
VECMATH_FINLINE float VECTORCALL v_extract_x(vec4f v);
//! extracts fp32 y-component of float[4]
VECMATH_FINLINE float VECTORCALL v_extract_y(vec4f v);
//! extracts fp32 z-component of float[4]
VECMATH_FINLINE float VECTORCALL v_extract_z(vec4f v);
//! extracts fp32 w-component of float[4]
VECMATH_FINLINE float VECTORCALL v_extract_w(vec4f v);

//! extracts i16 x-component of short[8]
VECMATH_FINLINE short VECTORCALL v_extract_xi16(vec4i v);

//! extracts i64 x-component of i64[2]
VECMATH_FINLINE int64_t VECTORCALL v_extract_xi64 ( vec4i a );

//! extracts ith-component of short[8]
VECMATH_FINLINE short VECTORCALL v_extract_i16(vec4i v, int i);

//! extracts i32 x-component of int[4]
VECMATH_FINLINE int VECTORCALL v_extract_xi(vec4i v);
//! extracts i32 y-component of int[4]
VECMATH_FINLINE int VECTORCALL v_extract_yi(vec4i v);
//! extracts i32 z-component of int[4]
VECMATH_FINLINE int VECTORCALL v_extract_zi(vec4i v);
//! extracts i32 w-component of int[4]
VECMATH_FINLINE int VECTORCALL v_extract_wi(vec4i v);

//! insert scalar to vector by index
VECMATH_FINLINE vec4f VECTORCALL v_insert(float s, vec4f v, int i);

//! promote put scalar to specified place in vector; other elements undefined
VECMATH_FINLINE vec4f VECTORCALL v_promote(float s, int i);


//
// support for slow but sometimes necessary branching
//

//! (v.x == a.x) ? 1 : 0
VECMATH_FINLINE int VECTORCALL v_test_vec_x_eq(vec3f v, vec3f a);
//! (v.x  > a.x) ? 1 : 0
VECMATH_FINLINE int VECTORCALL v_test_vec_x_gt(vec3f v, vec3f a);
//! (v.x >= a.x) ? 1 : 0
VECMATH_FINLINE int VECTORCALL v_test_vec_x_ge(vec3f v, vec3f a);
//! (v.x  < a.x) ? 1 : 0
VECMATH_FINLINE int VECTORCALL v_test_vec_x_lt(vec3f v, vec3f a);
//! (v.x <= a.x) ? 1 : 0
VECMATH_FINLINE int VECTORCALL v_test_vec_x_le(vec3f v, vec3f a);

//! (v.x == 0) ? 1 : 0
VECMATH_FINLINE int VECTORCALL v_test_vec_x_eq_0(vec3f v);
//! (v.x  > 0) ? 1 : 0
VECMATH_FINLINE int VECTORCALL v_test_vec_x_gt_0(vec3f v);
//! (v.x >= 0) ? 1 : 0
VECMATH_FINLINE int VECTORCALL v_test_vec_x_ge_0(vec3f v);
//! (v.x  < 0) ? 1 : 0
VECMATH_FINLINE int VECTORCALL v_test_vec_x_lt_0(vec3f v);
//! (v.x <= 0) ? 1 : 0
VECMATH_FINLINE int VECTORCALL v_test_vec_x_le_0(vec3f v);


#include "dag_vecMath_const.h"

#if _TARGET_SIMD_SSE
  #include "dag_vecMath_pc_sse.h"
#elif _TARGET_SIMD_NEON
  #include "dag_vecMath_neon.h"
#else
 !error! unsupported target
#endif

#include "dag_vecMath_common.h"
#include "dag_vecMath_trig.h"

#endif
