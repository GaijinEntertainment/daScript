/*
 * Dagor Engine 5
 * Copyright (C) 2003-2021  Gaijin Entertainment.  All rights reserved
 *
 * (for conditions of distribution and use, see License)
*/

#ifndef _DAGOR_PUBLIC_MATH_DAG_VECMATH_CONST_H_
#define _DAGOR_PUBLIC_MATH_DAG_VECMATH_CONST_H_
#pragma once

#include "dag_vecMathDecl.h"

#if defined(_MSC_VER) && !defined(__clang__)
  #define DECL_VEC_CONST extern const __declspec(selectany) __declspec(align(16))
#else
  #define DECL_VEC_CONST alignas(16) static const
#endif

#define REPLICATE(v) v, v, v, v

#if _TARGET_SIMD_VMX|_TARGET_SIMD_SPU
  #define V_PERM_X 0x00010203
  #define V_PERM_Y 0x04050607
  #define V_PERM_Z 0x08090a0b
  #define V_PERM_W 0x0c0d0e0f
  #define V_PERM_A 0x10111213
  #define V_PERM_B 0x14151617
  #define V_PERM_C 0x18191a1b
  #define V_PERM_D 0x1c1d1e1f
#endif

#if _TARGET_SIMD_SSE
  DECL_VEC_CONST vec4f_const V_C_HALF = { REPLICATE(0.5f) };
  DECL_VEC_CONST vec4f_const V_C_ONE = { REPLICATE(1.0f) };
  DECL_VEC_CONST vec4f_const V_C_TWO = { REPLICATE(2.0f) };
  DECL_VEC_CONST vec4f_const V_C_PI = { REPLICATE(3.141592f) };                  // pi
  DECL_VEC_CONST vec4f_const V_C_HALFPI = { REPLICATE(3.141592f / 2.f) };        // pi/2
  DECL_VEC_CONST vec4f_const V_C_TWOPI = { REPLICATE(2.f * 3.141592f) };         // pi*2
  DECL_VEC_CONST vec4f_const V_C_PI_DIV_4 = { REPLICATE(0.785398f) };            // pi/4
  DECL_VEC_CONST vec4f_const V_C_2_DIV_PI = { REPLICATE(0.636619f) };            // 2/pi
  DECL_VEC_CONST vec4f_const V_C_4_DIV_PI = { REPLICATE(1.273239f) };            // 4/pi
  DECL_VEC_CONST vec4f_const V_C_FLT_EPSILON = { REPLICATE(1.192092896e-07f) };

  DECL_VEC_CONST vec4i_const V_CI_SIGN_MASK = { REPLICATE(0x80000000) };
  DECL_VEC_CONST vec4i_const V_CI_INV_SIGN_MASK = { REPLICATE(0x7FFFFFFF) };
  DECL_VEC_CONST vec4i_const V_CI_0 = { REPLICATE(0) };
  DECL_VEC_CONST vec4i_const V_CI_1 = { REPLICATE(1) };
  DECL_VEC_CONST vec4i_const V_CI_2 = { REPLICATE(2) };
  DECL_VEC_CONST vec4i_const V_CI_3 = { REPLICATE(3) };
  DECL_VEC_CONST vec4i_const V_CI_4 = { REPLICATE(4) };
#endif

#if _TARGET_SIMD_SSE
  DECL_VEC_CONST vec4f_const V_C_MAX_VAL = { REPLICATE(1e32f) };
  DECL_VEC_CONST vec4f_const V_C_MIN_VAL = { REPLICATE(-1e32f) };
  DECL_VEC_CONST vec4f_const V_C_EPS_VAL = { REPLICATE(1.19209290e-07f) };
  DECL_VEC_CONST vec4f_const V_C_UNIT_1000 = {1.0f, 0.0f, 0.0f, 0.0f};
  DECL_VEC_CONST vec4f_const V_C_UNIT_0100 = {0.0f, 1.0f, 0.0f, 0.0f};
  DECL_VEC_CONST vec4f_const V_C_UNIT_0010 = {0.0f, 0.0f, 1.0f, 0.0f};
  DECL_VEC_CONST vec4f_const V_C_UNIT_0001 = {0.0f, 0.0f, 0.0f, 1.0f};
  DECL_VEC_CONST vec4f_const V_C_UNIT_1110 = {1.0f, 1.0f, 1.0f, 0.0f};
  DECL_VEC_CONST vec4f_const V_C_UNIT_0011 = {0.0f, 0.0f, 1.0f, 1.0f};

  DECL_VEC_CONST vec4i_const V_CI_MASK0000 = { 0, 0, 0, 0 };
  DECL_VEC_CONST vec4i_const V_CI_MASK1000 = { 0xFFFFFFFF, 0, 0, 0 };
  DECL_VEC_CONST vec4i_const V_CI_MASK0100 = { 0, 0xFFFFFFFF, 0, 0 };
  DECL_VEC_CONST vec4i_const V_CI_MASK0010 = { 0, 0, 0xFFFFFFFF, 0 };
  DECL_VEC_CONST vec4i_const V_CI_MASK0001 = { 0, 0, 0, 0xFFFFFFFF };
  DECL_VEC_CONST vec4i_const V_CI_MASK1100 = { 0xFFFFFFFF, 0xFFFFFFFF, 0, 0 };
  DECL_VEC_CONST vec4i_const V_CI_MASK0110 = { 0, 0xFFFFFFFF, 0xFFFFFFFF, 0 };
  DECL_VEC_CONST vec4i_const V_CI_MASK0011 = { 0, 0, 0xFFFFFFFF, 0xFFFFFFFF };
  DECL_VEC_CONST vec4i_const V_CI_MASK1010 = { 0xFFFFFFFF, 0, 0xFFFFFFFF, 0 };
  DECL_VEC_CONST vec4i_const V_CI_MASK0101 = { 0, 0xFFFFFFFF, 0xFFFFFFFF, 0 };
  DECL_VEC_CONST vec4i_const V_CI_MASK1110 = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0 };
  DECL_VEC_CONST vec4i_const V_CI_MASK1101 = { 0xFFFFFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF };
  DECL_VEC_CONST vec4i_const V_CI_MASK1011 = { 0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFFFFFF };
  DECL_VEC_CONST vec4i_const V_CI_MASK0111 = { 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
  DECL_VEC_CONST vec4i_const V_CI_MASK1111 = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
  DECL_VEC_CONST vec4i_const V_CI_3FF = { REPLICATE( 0x3FF ) };
  DECL_VEC_CONST vec4i_const V_CI_070 = { REPLICATE( 0x70 ) };
  DECL_VEC_CONST vec4i_const V_CI_0FF = { REPLICATE( 0xFF ) };
  DECL_VEC_CONST vec4i_const V_CI_01F = { REPLICATE( 0x1F ) };
  DECL_VEC_CONST vec4i_const V_CI_07FFFFF = { REPLICATE( 0x7fffff ) };

#elif _TARGET_SIMD_NEON
  #define DVM_SPLAT1F(a) (vdupq_n_f32(a))
  #define DVM_SPLAT1I(a) (vdupq_n_s32(a))
  #define DVM_VECFLOAT4  (float32x4_t)
  #define DVM_VECUINT4   (int32x4_t)

  #define V_CI2_MASK10  (int32x2_t) { -1, 0 }
  #define V_CI2_MASK01  (int32x2_t) { 0, -1 }

  #define V_C_MAX_VAL DVM_SPLAT1F(1e32f)
  #define V_C_MIN_VAL DVM_SPLAT1F(-1e32f)
  #define V_C_EPS_VAL DVM_SPLAT1F(1.19209290e-07f)

  #define V_C_UNIT_1000 DVM_VECFLOAT4{ 1.0f, 0.0f, 0.0f, 0.0f }
  #define V_C_UNIT_0100 DVM_VECFLOAT4{ 0.0f, 1.0f, 0.0f, 0.0f }
  #define V_C_UNIT_0010 DVM_VECFLOAT4{ 0.0f, 0.0f, 1.0f, 0.0f }
  #define V_C_UNIT_0001 DVM_VECFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }
  #define V_C_UNIT_1110 DVM_VECFLOAT4{ 1.0f, 1.0f, 1.0f, 0.0f }
  #define V_C_UNIT_0011 DVM_VECFLOAT4{ 0.0f, 0.0f, 1.0f, 1.0f }

  #define V_CI_MASK0000 DVM_SPLAT1I(0)
  #define V_CI_MASK1000 DVM_VECUINT4{ -1, 0, 0, 0 }
  #define V_CI_MASK0100 DVM_VECUINT4{ 0, -1, 0, 0 }
  #define V_CI_MASK0010 DVM_VECUINT4{ 0, 0, -1, 0 }
  #define V_CI_MASK0001 DVM_VECUINT4{ 0, 0, 0, -1 }
  #define V_CI_MASK1100 DVM_VECUINT4{ -1, -1, 0, 0 }
  #define V_CI_MASK0110 DVM_VECUINT4{ 0, -1, -1, 0 }
  #define V_CI_MASK0011 DVM_VECUINT4{ 0, 0, -1, -1 }
  #define V_CI_MASK1010 DVM_VECUINT4{ -1, 0, -1, 0 }
  #define V_CI_MASK0101 DVM_VECUINT4{ 0, -1, 0, -1 }
  #define V_CI_MASK1110 DVM_VECUINT4{ -1, -1, -1, 0 }
  #define V_CI_MASK1101 DVM_VECUINT4{ -1, -1, 0, -1 }
  #define V_CI_MASK1011 DVM_VECUINT4{ -1, 0, -1, -1 }
  #define V_CI_MASK0111 DVM_VECUINT4{ 0, -1, -1, -1 }
  #define V_CI_MASK1111 DVM_VECUINT4{ -1, -1, -1, -1 }
  #define V_CI_3FF DVM_SPLAT1I( 0x3FF )
  #define V_CI_070 DVM_SPLAT1I( 0x70 )
  #define V_CI_0FF DVM_SPLAT1I( 0xFF )
  #define V_CI_0FF DVM_SPLAT1I( 0xFF )
  #define V_CI_01F DVM_SPLAT1I( 0x1F )
  #define V_CI_07FFFFF DVM_SPLAT1I(0x7fffff)
#endif

#if _TARGET_SIMD_NEON
  #define V_C_HALF            vdupq_n_f32(0.5f)
  #define V_C_ONE             vdupq_n_f32(1.0f)
  #define V_C_TWO             vdupq_n_f32(2.0f)
  #define V_C_PI              vdupq_n_f32(3.141592f)           // pi
  #define V_C_HALFPI          vdupq_n_f32(3.141592f / 2.f)     // pi/2
  #define V_C_TWOPI           vdupq_n_f32(2.f * 3.141592f)     // pi*2
  #define V_C_PI_DIV_4        vdupq_n_f32(0.785398f)           // pi/4
  #define V_C_2_DIV_PI        vdupq_n_f32(0.636619f)           // 2/pi
  #define V_C_4_DIV_PI        vdupq_n_f32(1.273239f)           // 4/pi
  #define V_C_FLT_EPSILON     vdupq_n_f32(1.192092896e-07f)

  #define V_CI_SIGN_MASK      vdupq_n_s32(0x80000000)
  #define V_CI_INV_SIGN_MASK  vdupq_n_s32(0x7FFFFFFF)
  #define V_CI_0              vdupq_n_s32(0)
  #define V_CI_1              vdupq_n_s32(1)
  #define V_CI_2              vdupq_n_s32(2)
  #define V_CI_3              vdupq_n_s32(3)
  #define V_CI_4              vdupq_n_s32(4)
#endif

#undef REPLICATE
#undef DECL_VEC_CONST

#endif
