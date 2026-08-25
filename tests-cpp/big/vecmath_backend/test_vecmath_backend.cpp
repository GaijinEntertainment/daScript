// Backend-agnostic vecmath battery, built once per backend (see CMakeLists.txt);
// standalone on purpose - linking libDaScript (compiled with the native vec4f)
// into a scalar-forced TU would mix two vec4f ABIs in one binary. Rows guarded
// with !_TARGET_SIMD_NEON pin SSE-flavored semantics the scalar backend promises
// to match (NaN/tie ordering, sign-bit select, out-of-range converts, shift
// counts past the lane width) - NEON diverges there by its own contract.

#ifndef _MSC_VER
#ifndef __forceinline
#define __forceinline inline
#endif
#endif

#include <vecmath/dag_vecMath.h>
// compile pin: daScriptC.h shares vec4f_scalar_t/vec4i_scalar_t with dag_vecMathDecl.h
// under VECMATH_SCALAR_TYPES_DEFINED - this TU pins the vecmath-first include order
#include <daScript/daScriptC.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cmath>

#if defined(EXPECT_SCALAR) && !defined(_TARGET_SIMD_SCALAR)
#error this target must select the scalar vecmath backend
#endif
#if defined(EXPECT_NATIVE) && defined(_TARGET_SIMD_SCALAR)
#error this target must select the native SIMD vecmath backend
#endif

static int g_failed = 0;

static unsigned lane_bits(vec4f v, int k) { unsigned u[4]; memcpy(u, &v, 16); return u[k]; }
static unsigned lane_bitsi(vec4i v, int k) { unsigned u[4]; memcpy(u, &v, 16); return u[k]; }

static void check_lanes(const char *name, vec4f v, unsigned x, unsigned y, unsigned z, unsigned w)
{
  unsigned e[4] = {x, y, z, w};
  for (int k = 0; k < 4; k++)
    if (lane_bits(v, k) != e[k]) {
      printf("FAIL %s lane %d: got %08x want %08x\n", name, k, lane_bits(v, k), e[k]);
      g_failed++;
      return;
    }
}
static void check_lanesi(const char *name, vec4i v, unsigned x, unsigned y, unsigned z, unsigned w)
{
  unsigned e[4] = {x, y, z, w};
  for (int k = 0; k < 4; k++)
    if (lane_bitsi(v, k) != e[k]) {
      printf("FAIL %s lane %d: got %08x want %08x\n", name, k, lane_bitsi(v, k), e[k]);
      g_failed++;
      return;
    }
}
static void check_near(const char *name, vec4f v, float x, float y, float z, float w, float tol)
{
  float e[4] = {x, y, z, w}, g[4]; memcpy(g, &v, 16);
  for (int k = 0; k < 4; k++)
    if (!(fabsf(g[k] - e[k]) <= tol)) {
      printf("FAIL %s lane %d: got %g want %g (tol %g)\n", name, k, g[k], e[k], tol);
      g_failed++;
      return;
    }
}
static void check_double(const char *name, double got, double want)
{
  if (got != want) { printf("FAIL %s: got %.17g want %.17g\n", name, got, want); g_failed++; }
}

static void check_int(const char *name, long long got, long long want)
{
  if (got != want) { printf("FAIL %s: got %lld want %lld\n", name, got, want); g_failed++; }
}
static unsigned f2u(float f) { unsigned u; memcpy(&u, &f, 4); return u; }

int main()
{
  const vec4f a = v_make_vec4f(1.5f, -2.25f, 3.75f, -0.5f);
  const vec4f b = v_make_vec4f(2.0f, 0.5f, -1.0f, 4.0f);
  const vec4i ia = v_make_vec4i(3, -7, 123456, -2000000000);
  const vec4i ib = v_make_vec4i(-5, 11, 7, 3);
  const float nanf_v = nanf("");

  check_lanes("add", v_add(a, b), f2u(3.5f), f2u(-1.75f), f2u(2.75f), f2u(3.5f));
  check_lanes("sub", v_sub(a, b), f2u(-0.5f), f2u(-2.75f), f2u(4.75f), f2u(-4.5f));
  check_lanes("mul", v_mul(a, b), f2u(3.0f), f2u(-1.125f), f2u(-3.75f), f2u(-2.0f));
  check_lanes("div", v_div(a, b), f2u(0.75f), f2u(-4.5f), f2u(-3.75f), f2u(-0.125f));
  check_lanes("madd", v_madd(a, b, b), f2u(5.0f), f2u(-0.625f), f2u(-4.75f), f2u(2.0f));
  check_lanes("nmsub", v_nmsub(a, b, b), f2u(-1.0f), f2u(1.625f), f2u(2.75f), f2u(6.0f));
  check_lanes("sqrt", v_sqrt(v_make_vec4f(4.0f, 9.0f, 2.25f, 0.0f)), f2u(2.0f), f2u(3.0f), f2u(1.5f), f2u(0.0f));
  // no -0.0f lane: pc_sse's clang arm (v_max(v_neg(a), a)) keeps -0 where the bit-mask arms clear it
  check_lanes("abs", v_abs(v_make_vec4f(-1.5f, 1.5f, -37.5f, 0.0f)), f2u(1.5f), f2u(1.5f), f2u(37.5f), 0u);
  check_lanes("neg", v_neg(v_make_vec4f(-1.5f, 1.5f, 0.0f, -0.0f)), f2u(1.5f), f2u(-1.5f), 0x80000000u, 0u);

  check_int("add_x", (long long)f2u(v_extract_x(v_add_x(a, b))), (long long)f2u(3.5f));
  check_int("nmsub_x", (long long)f2u(v_extract_x(v_nmsub_x(a, b, b))), (long long)f2u(-1.0f));
  check_int("sqrt_x", (long long)f2u(v_extract_x(v_sqrt_x(v_make_vec4f(4.0f, 5.0f, 6.0f, 7.0f)))), (long long)f2u(2.0f));
#if !defined(_TARGET_SIMD_NEON)
  check_lanes("add_x_keeps_yzw", v_add_x(a, b), f2u(3.5f), f2u(-2.25f), f2u(3.75f), f2u(-0.5f));
  check_lanes("nmsub_x_keeps_c_yzw", v_nmsub_x(a, b, b), f2u(-1.0f), f2u(0.5f), f2u(-1.0f), f2u(4.0f));
  check_lanes("sqrt_x_keeps_yzw", v_sqrt_x(v_make_vec4f(4.0f, 5.0f, 6.0f, 7.0f)), f2u(2.0f), f2u(5.0f), f2u(6.0f), f2u(7.0f));
#endif

  check_lanes("min", v_min(a, b), f2u(1.5f), f2u(-2.25f), f2u(-1.0f), f2u(-0.5f));
  check_lanes("max", v_max(a, b), f2u(2.0f), f2u(0.5f), f2u(3.75f), f2u(4.0f));
#if !defined(_TARGET_SIMD_NEON)
  check_lanes("min_second_operand_on_nan", v_min(v_splats(nanf_v), v_splats(1.0f)), f2u(1.0f), f2u(1.0f), f2u(1.0f), f2u(1.0f));
  check_lanes("max_second_operand_on_tie", v_max(v_splats(0.0f), v_splats(-0.0f)), 0x80000000u, 0x80000000u, 0x80000000u, 0x80000000u);
#endif

  const vec4f halves = v_make_vec4f(2.5f, -2.5f, 3.5f, -3.5f);
  check_lanes("floor", v_floor(halves), f2u(2.0f), f2u(-3.0f), f2u(3.0f), f2u(-4.0f));
  check_lanes("ceil", v_ceil(halves), f2u(3.0f), f2u(-2.0f), f2u(4.0f), f2u(-3.0f));
  check_lanes("trunc", v_trunc(halves), f2u(2.0f), f2u(-2.0f), f2u(3.0f), f2u(-3.0f));
  check_lanes("round_ieee", v_round_ieee(halves), f2u(2.0f), f2u(-2.0f), f2u(4.0f), f2u(-4.0f));

  check_lanesi("cvtt", v_cvti_vec4i(a), 1u, 0xFFFFFFFEu, 3u, 0u);
  check_lanesi("cvtr", v_cvt_roundi_ieee(halves), 2u, 0xFFFFFFFEu, 4u, 0xFFFFFFFCu);
#if !defined(_TARGET_SIMD_NEON)
  check_lanesi("cvtt_ovf", v_cvti_vec4i(v_make_vec4f(3e9f, -3e9f, nanf_v, 100.75f)),
               0x80000000u, 0x80000000u, 0x80000000u, 100u);
  check_lanesi("cvtr_ovf", v_cvt_roundi_ieee(v_make_vec4f(3e9f, -3e9f, nanf_v, 100.5f)),
               0x80000000u, 0x80000000u, 0x80000000u, 100u);
#endif
  check_lanesi("cvt_floori", v_cvt_floori(halves), 2u, 0xFFFFFFFDu, 3u, 0xFFFFFFFCu);
  check_lanes("cvti2f", v_cvti_vec4f(ia), f2u(3.0f), f2u(-7.0f), f2u(123456.0f), f2u(-2000000000.0f));
  check_lanes("cvtu2f", v_cvtu_vec4f_ieee(v_make_vec4i(-1, 5, 0, 65536)),
              f2u(4294967296.0f), f2u(5.0f), 0u, f2u(65536.0f));

  check_lanes("cmp_gt", v_cmp_gt(a, b), 0u, 0u, 0xFFFFFFFFu, 0u);
  check_lanes("cmp_eq", v_cmp_eq(a, v_make_vec4f(1.5f, 0.f, 3.75f, 0.f)), 0xFFFFFFFFu, 0u, 0xFFFFFFFFu, 0u);
  check_lanes("cmp_eqi_f", v_cmp_eqi(v_splats(nanf_v), v_splats(nanf_v)),
              0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu);
  check_lanes("cmp_ge_nan_false", v_cmp_ge(v_splats(nanf_v), v_splats(nanf_v)), 0u, 0u, 0u, 0u);
  check_lanes("cmp_neq_nan_true", v_cmp_neq(v_splats(nanf_v), v_splats(nanf_v)),
              0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu);
  check_lanesi("cmp_lti", v_cmp_lti(ia, ib), 0u, 0xFFFFFFFFu, 0u, 0xFFFFFFFFu);
  check_lanesi("cmp_gti", v_cmp_gti(ia, ib), 0xFFFFFFFFu, 0u, 0xFFFFFFFFu, 0u);
  check_lanesi("cmp_eqi", v_cmp_eqi(ia, v_make_vec4i(3, 0, 123456, 0)), 0xFFFFFFFFu, 0u, 0xFFFFFFFFu, 0u);

  check_lanes("sel_mask", v_sel(a, b, v_cmp_gt(a, b)), f2u(1.5f), f2u(-2.25f), f2u(-1.0f), f2u(-0.5f));
#if !defined(_TARGET_SIMD_NEON)
  const vec4f signs = v_make_vec4f(-1.0f, 1.0f, -0.0f, 0.0f);
  check_lanes("sel_reads_sign_bit_only", v_sel(a, b, signs), f2u(2.0f), f2u(-2.25f), f2u(-1.0f), f2u(-0.5f));
#endif
  check_int("signmask", v_signmask(v_make_vec4f(-1.0f, 1.0f, -0.0f, 0.0f)), 0b0101);
  check_int("check_xyz", v_check_xyz_all_true(v_cast_vec4f(v_make_vec4i(-1, -1, -1, 0))) ? 1 : 0, 1);
  check_int("check_xyz_false_dir", v_check_xyz_all_true(v_cast_vec4f(v_make_vec4i(-1, 0, -1, -1))) ? 1 : 0, 0);
  check_int("check_xyz_all_false", v_check_xyz_all_false(v_cast_vec4f(v_make_vec4i(0, 0, 0, -1))) ? 1 : 0, 1);
  check_int("check_xyz_any_true", v_check_xyz_any_true(v_cast_vec4f(v_make_vec4i(0, -1, 0, 0))) ? 1 : 0, 1);
  check_int("check_xyzw_all_true", v_check_xyzw_all_true(v_cast_vec4f(v_make_vec4i(-1, -1, -1, 0))) ? 1 : 0, 0);
  check_int("check_xyzw_all_false", v_check_xyzw_all_false(v_zero()) ? 1 : 0, 1);
  check_int("check_xyzw_any_true", v_check_xyzw_any_true(v_cast_vec4f(v_make_vec4i(0, 0, 0, -1))) ? 1 : 0, 1);
#if !defined(_TARGET_SIMD_NEON)
  check_int("check_xyz_reads_sign_bits", v_check_xyz_all_true(v_make_vec4f(-1.f, -1.f, -1.f, 0.f)) ? 1 : 0, 1);
#endif
  check_int("all_bits_zeros", v_test_all_bits_zeros(v_zero()) ? 1 : 0, 1);
  check_int("all_bits_zeros_false_dir", v_test_all_bits_zeros(v_cast_vec4f(v_make_vec4i(0, 0, 1, 0))) ? 1 : 0, 0);
  check_int("all_bits_ones", v_test_all_bits_ones(v_set_all_bits()) ? 1 : 0, 1);
  check_int("all_bits_ones_false_dir", v_test_all_bits_ones(v_cast_vec4f(v_make_vec4i(-1, -1, -2, -1))) ? 1 : 0, 0);
  check_int("any_bit_set", v_test_any_bit_set(v_cast_vec4f(v_make_vec4i(0, 0, 0, 1))) ? 1 : 0, 1);
  check_int("any_bit_set_false_dir", v_test_any_bit_set(v_zero()) ? 1 : 0, 0);
  check_int("test_vec_x_eq", v_test_vec_x_eq(a, v_make_vec4f(1.5f, 9.f, 9.f, 9.f)), 1);
  check_int("test_vec_x_lt", v_test_vec_x_lt(a, b), 1);
  check_int("test_vec_x_le_0", v_test_vec_x_le_0(v_make_vec4f(-0.5f, 1.f, 1.f, 1.f)), 1);
  check_int("test_vec_x_ge_0_false_dir", v_test_vec_x_ge_0(v_make_vec4f(-0.5f, 1.f, 1.f, 1.f)), 0);
  check_int("test_vec_x_eqi_0", v_test_vec_x_eqi_0(v_zero()), 1);

  check_lanes("and", v_and(v_splats(3.0f), v_splats(2.0f)), f2u(2.0f), f2u(2.0f), f2u(2.0f), f2u(2.0f));
  check_lanes("xor_self", v_xor(a, a), 0u, 0u, 0u, 0u);
  check_lanes("not_zero", v_not(v_zero()), 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu);
  check_lanes("btsel", v_btsel(a, b, v_cast_vec4f(v_make_vec4i(0, -1, 0, -1))),
              f2u(1.5f), f2u(0.5f), f2u(3.75f), f2u(4.0f));

  check_lanesi("addi", v_addi(ia, ib), 0xFFFFFFFEu, 4u, 0x1E247u, 0x88CA6C03u);
  check_lanesi("muli", v_muli(v_make_vec4i(100000, -3, 7, 1 << 30), v_make_vec4i(100000, 5, -7, 4)),
               0x540BE400u, 0xFFFFFFF1u, 0xFFFFFFCFu, 0u);
  check_lanesi("mini", v_mini(ia, ib), 0xFFFFFFFBu, 0xFFFFFFF9u, 7u, 0x88CA6C00u);
  check_lanesi("minu", v_minu(ia, ib), 3u, 11u, 7u, 3u);
  check_lanesi("maxi", v_maxi(ia, ib), 3u, 11u, 0x1E240u, 3u);
  check_lanesi("maxu", v_maxu(ia, ib), 0xFFFFFFFBu, 0xFFFFFFF9u, 0x1E240u, 0x88CA6C00u);
#if !defined(_TARGET_SIMD_NEON)
  check_lanesi("seli_sign_bit", v_seli(ia, ib, v_make_vec4i(-1, 0, INT32_MIN, 1)),
               0xFFFFFFFBu, 0xFFFFFFF9u, 7u, 0x88CA6C00u);
#endif
  check_lanesi("absi", v_absi(v_make_vec4i(-3, 3, INT32_MIN, 0)), 3u, 3u, 0x80000000u, 0u);
  check_lanesi("slli", v_slli(ia, 3), 24u, 0xFFFFFFC8u, 0xF1200u, 0x46536000u);
  check_lanesi("srli", v_srli(ia, 3), 0u, 0x1FFFFFFFu, 0x3C48u, 0x11194D80u);
  check_lanesi("srai", v_srai(ia, 3), 0u, 0xFFFFFFFFu, 0x3C48u, 0xF1194D80u);
#if !defined(_TARGET_SIMD_NEON)
  check_lanesi("slli_ge32_zeroes", v_slli(ia, 32), 0u, 0u, 0u, 0u);
  check_lanesi("srli_ge32_zeroes", v_srli(ia, 32), 0u, 0u, 0u, 0u);
  check_lanesi("srai_gt31_sign_fills", v_srai(ia, 40), 0u, 0xFFFFFFFFu, 0u, 0xFFFFFFFFu);
#endif
  check_lanesi("packs", v_packs(v_make_vec4i(70000, -70000, 5, -5)),
               0x80007FFFu, 0xFFFB0005u, 0x80007FFFu, 0xFFFB0005u);
  check_lanesi("packus", v_packus(v_make_vec4i(70000, -70000, 5, 65535)),
               0x0000FFFFu, 0xFFFF0005u, 0x0000FFFFu, 0xFFFF0005u);
  check_lanesi("packus16", v_packus16(v_packs(v_make_vec4i(300, -5, 100, 255))),
               0xFF6400FFu, 0xFF6400FFu, 0xFF6400FFu, 0xFF6400FFu);
  check_lanesi("madd_i16_wraps", v_madd_i16(v_splatsi16(-32768), v_splatsi16(-32768)),
               0x80000000u, 0x80000000u, 0x80000000u, 0x80000000u);

  check_lanes("splat_y", v_splat_y(a), f2u(-2.25f), f2u(-2.25f), f2u(-2.25f), f2u(-2.25f));
  check_lanes("splat_z", v_splat_z(a), f2u(3.75f), f2u(3.75f), f2u(3.75f), f2u(3.75f));
  check_lanes("splat_w", v_splat_w(a), f2u(-0.5f), f2u(-0.5f), f2u(-0.5f), f2u(-0.5f));
  check_lanes("rot_1", v_rot_1(a), f2u(-2.25f), f2u(3.75f), f2u(-0.5f), f2u(1.5f));
  check_lanes("rot_2", v_rot_2(a), f2u(3.75f), f2u(-0.5f), f2u(1.5f), f2u(-2.25f));
  check_lanes("rot_3", v_rot_3(a), f2u(-0.5f), f2u(1.5f), f2u(-2.25f), f2u(3.75f));
  check_lanes("perm_yzxw", v_perm_yzxw(a), f2u(-2.25f), f2u(3.75f), f2u(1.5f), f2u(-0.5f));
  check_lanes("perm_xzac", v_perm_xzac(a, b), f2u(1.5f), f2u(3.75f), f2u(2.0f), f2u(-1.0f));
  check_lanes("perm_ayzw", v_perm_ayzw(a, b), f2u(2.0f), f2u(-2.25f), f2u(3.75f), f2u(-0.5f));
  check_lanes("merge_hw", v_merge_hw(a, b), f2u(1.5f), f2u(2.0f), f2u(-2.25f), f2u(0.5f));
  check_lanesi("interleave_lo32", v_interleave_lo_i32(ia, ib), 3u, 0xFFFFFFFBu, 0xFFFFFFF9u, 11u);
  check_int("extract_z", (long long)f2u(v_extract_z(a)), (long long)f2u(3.75f));
  check_int("extract_wi", v_extract_wi(ia), -2000000000);
  check_int("extract_xi64", v_extract_xi64(v_splatsi64(0x123456789abcdef0ll)), 0x123456789abcdef0ll);

  {
    const float p3[3] = {9.f, 8.f, 7.f};
    check_lanes("ldu_p3_safe", v_ldu_p3_safe(p3), f2u(9.f), f2u(8.f), f2u(7.f), 0u);
    float out[4] = {0, 0, 0, -1.f};
    v_stu_p3(out, a);
    check_int("stu_p3_w_untouched", (long long)f2u(out[3]), (long long)f2u(-1.f));
    check_int("stu_p3_z", (long long)f2u(out[2]), (long long)f2u(3.75f));
    const short sh[4] = {-1, 2, -3, 4};
    check_lanesi("ldush", v_ldush(sh), 0xFFFFFFFFu, 2u, 0xFFFFFFFDu, 4u);
    check_lanesi("lduush", v_lduush((const unsigned short *)sh), 0xFFFFu, 2u, 0xFFFDu, 4u);
  }

  check_near("dot3", v_dot3(a, b), 1.875f - 3.75f, 1.875f - 3.75f, 1.875f - 3.75f, 1.875f - 3.75f, 1e-6f);
  {
    float dx = v_extract_x(v_dot4_x(a, b)); // only .x is contract for _x ops; yzw differ per SSE tier
    if (!(fabsf(dx - -3.875f) <= 1e-5f)) { printf("FAIL dot4_x: %g\n", dx); g_failed++; }
  }
  {
    float hx = v_extract_x(v_hadd4_x(a));
    if (!(fabsf(hx - 2.5f) <= 1e-6f)) { printf("FAIL hadd4_x: %g\n", hx); g_failed++; }
  }

  //tolerance covers the SIMD backends' rcpps/rsqrtps estimates; the scalar backend computes these exactly
  check_near("rcp_est", v_rcp_est(b), 0.5f, 2.0f, -1.0f, 0.25f, 2e-3f);
  check_near("rsqrt_est", v_rsqrt_est(v_make_vec4f(4.f, 16.f, 1.f, 64.f)), 0.5f, 0.25f, 1.f, 0.125f, 2e-3f);
  check_near("norm3", v_norm3(v_make_vec4f(3.f, 0.f, 4.f, 0.f)), 0.6f, 0.f, 0.8f, 0.f, 1e-5f);

  {
    mat44f m;
    m.col0 = v_make_vec4f(2, 0, 0, 0);
    m.col1 = v_make_vec4f(0, 3, 0, 0);
    m.col2 = v_make_vec4f(1, 0, 4, 0);
    m.col3 = v_make_vec4f(5, 6, 7, 1);
    float det_x = v_extract_x(v_mat44_det(m));
    if (!(fabsf(det_x - 24.f) <= 1e-4f)) { printf("FAIL det: %g\n", det_x); g_failed++; }
    mat44f inv, id;
    v_mat44_inverse(inv, m);
    v_mat44_mul(id, m, inv);
    check_near("inv_id0", id.col0, 1.f, 0.f, 0.f, 0.f, 1e-5f);
    check_near("inv_id1", id.col1, 0.f, 1.f, 0.f, 0.f, 1e-5f);
    check_near("inv_id2", id.col2, 0.f, 0.f, 1.f, 0.f, 1e-5f);
    check_near("inv_id3", id.col3, 0.f, 0.f, 0.f, 1.f, 1e-5f);
    mat44f t;
    v_mat44_transpose(t, m);
    check_lanes("transpose0", t.col0, f2u(2.f), 0u, f2u(1.f), f2u(5.f));
    check_lanes("transpose1", t.col1, 0u, f2u(3.f), 0u, f2u(6.f));
    check_lanes("transpose2", t.col2, 0u, 0u, f2u(4.f), f2u(7.f));
    check_lanes("transpose3", t.col3, 0u, 0u, 0u, f2u(1.f));
    check_lanes("mul_vec3p", v_mat44_mul_vec3p(m, v_make_vec4f(1, 2, 3, 0)),
                f2u(10.f), f2u(12.f), f2u(19.f), f2u(1.f));
  }

  check_near("sin", v_sin(v_make_vec4f(0.f, 1.5707963f, 3.1415926f, -1.5707963f)), 0.f, 1.f, 0.f, -1.f, 1e-5f);
  check_near("cos", v_cos(v_make_vec4f(0.f, 1.5707963f, 3.1415926f, -1.5707963f)), 1.f, 0.f, -1.f, 0.f, 1e-5f);

  check_lanes("h2f", v_half_to_float(v_make_vec4i(0x3C00, 0x4000, 0xC000, 0x3555)),
              f2u(1.0f), f2u(2.0f), f2u(-2.0f), f2u(0.33325195f));
  check_lanesi("f2h", v_float_to_half_rtne(v_make_vec4f(1.0f, -2.0f, 0.5f, 65504.f)),
               0x3C00u, 0xC000u, 0x3800u, 0x7BFFu);

  check_lanes("is_nan", v_is_nan(v_make_vec4f(1.f, nanf_v, 3.f, nanf_v)),
              0u, 0xFFFFFFFFu, 0u, 0xFFFFFFFFu);

  check_lanes("add_pairs", v_add_pairs(a, b), f2u(-0.75f), f2u(3.25f), f2u(2.5f), f2u(3.0f));
  check_lanes("min_pairs", v_min_pairs(a, b), f2u(-2.25f), f2u(-0.5f), f2u(0.5f), f2u(-1.0f));
  check_lanes("max_pairs", v_max_pairs(a, b), f2u(1.5f), f2u(3.75f), f2u(2.0f), f2u(4.0f));
  check_lanesi("addi_pairs", v_addi_pairs(v_make_vec4i(1, 2, 30, 40), v_make_vec4i(500, 600, 7000, 8000)),
               3u, 70u, 1100u, 15000u);
  check_lanesi("mini_pairs", v_mini_pairs(ia, ib), 0xFFFFFFF9u, 0x88CA6C00u, 0xFFFFFFFBu, 3u);
  check_lanesi("maxi_pairs", v_maxi_pairs(ia, ib), 3u, 0x1E240u, 11u, 7u);

  check_lanes("hmin", v_hmin(a), f2u(-2.25f), f2u(-2.25f), f2u(-2.25f), f2u(-2.25f));
  check_lanes("hmax", v_hmax(a), f2u(3.75f), f2u(3.75f), f2u(3.75f), f2u(3.75f));
  check_lanes("hmin3", v_hmin3(a), f2u(-2.25f), f2u(-2.25f), f2u(-2.25f), f2u(-2.25f));
  check_lanes("hmax3", v_hmax3(b), f2u(2.0f), f2u(2.0f), f2u(2.0f), f2u(2.0f));
  check_lanesi("hmini", v_hmini(ia), 0x88CA6C00u, 0x88CA6C00u, 0x88CA6C00u, 0x88CA6C00u);
  check_lanesi("hmaxi", v_hmaxi(ia), 0x1E240u, 0x1E240u, 0x1E240u, 0x1E240u);
  check_lanesi("hmini3", v_hmini3(ia), 0xFFFFFFF9u, 0xFFFFFFF9u, 0xFFFFFFF9u, 0xFFFFFFF9u);
  check_lanesi("hmaxi3", v_hmaxi3(ia), 0x1E240u, 0x1E240u, 0x1E240u, 0x1E240u);

  check_lanes("round_away", v_round(v_make_vec4f(2.5f, -2.5f, 0.4f, -0.6f)),
              f2u(3.0f), f2u(-3.0f), f2u(0.0f), f2u(-1.0f));
  check_lanesi("cvt_roundi_away", v_cvt_roundi(v_make_vec4f(2.5f, -2.5f, 0.4f, -0.6f)),
               3u, 0xFFFFFFFDu, 0u, 0xFFFFFFFFu);

  {
    vec4f c3 = v_cross3(v_make_vec4f(1, 2, 3, 0), v_make_vec4f(4, 5, 6, 0));
    check_int("cross3_x", (long long)f2u(v_extract_x(c3)), (long long)f2u(-3.f));
    check_int("cross3_y", (long long)f2u(v_extract_y(c3)), (long long)f2u(6.f));
    check_int("cross3_z", (long long)f2u(v_extract_z(c3)), (long long)f2u(-3.f));
  }

  check_lanes("abs_diff", v_abs_diff(a, b), f2u(0.5f), f2u(2.75f), f2u(4.75f), f2u(4.5f));
  check_lanes("cmp_abs_ge", v_cmp_abs_ge(a, b), 0u, 0xFFFFFFFFu, 0xFFFFFFFFu, 0u);
  check_lanes("cmp_abs_gt", v_cmp_abs_gt(a, b), 0u, 0xFFFFFFFFu, 0xFFFFFFFFu, 0u);
  check_lanes("cmp_abs_gt_ties", v_cmp_abs_gt(a, v_neg(a)), 0u, 0u, 0u, 0u);
  check_lanesi("cmp_eqi8", v_cmp_eqi8(v_make_vec4i(0x01020304, 0x05060708, -1, 0),
                                      v_make_vec4i(0x01FF03FF, 0x05060708, -1, 1)),
               0xFF00FF00u, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFF00u);
  check_lanesi("cvtu_trunc", v_cvtu_vec4i(v_make_vec4f(1.9f, 0.f, 2.5f, 100.7f)), 1u, 0u, 2u, 100u);

  {
    const vec4f zOnlyMask = v_cmp_gt(a, b);
    check_int("truemask", v_truemask(zOnlyMask), 0b0100);
    check_int("count_true", v_count_true(zOnlyMask), 1);
    check_int("count_true4", v_count_true(v_set_all_bits()), 4);
    check_int("is_any_neg_b", v_is_any_neg_b(zOnlyMask) ? 1 : 0, 1);
    check_int("is_any_neg_b_false", v_is_any_neg_b(v_zero()) ? 1 : 0, 0);
    check_int("merge_planes_all", v_is_merge_planes_nout(v_set_all_bits(), v_set_all_bits(), v_set_all_bits(),
                                                         v_set_all_bits(), v_set_all_bits(), v_set_all_bits()), -1);
    check_int("merge_planes_one_out", v_is_merge_planes_nout(v_set_all_bits(), v_zero(), v_set_all_bits(),
                                                             v_set_all_bits(), v_set_all_bits(), v_set_all_bits()), 0);
  }

  check_lanesi("perm_i8", v_perm_i8(v_make_vec4i(0x03020100, 0x07060504, 0x0B0A0908, 0x0F0E0D0C),
                                    v_make_vec4i(0x00010203, int(0x80808080), 0x0F0F0F0F, 0x04050607)),
               0x00010203u, 0u, 0x0F0F0F0Fu, 0x04050607u);

  check_lanes("perm_yxwz", v_perm_yxwz(a), f2u(-2.25f), f2u(1.5f), f2u(-0.5f), f2u(3.75f));
  check_lanes("perm_yzwa", v_perm_yzwa(a, b), f2u(-2.25f), f2u(3.75f), f2u(-0.5f), f2u(2.0f));
  check_lanes("perm_wabc", v_perm_wabc(a, b), f2u(-0.5f), f2u(2.0f), f2u(0.5f), f2u(-1.0f));
  check_lanes("perm_xazc", v_perm_xazc(a, b), f2u(1.5f), f2u(2.0f), f2u(3.75f), f2u(-1.0f));
  check_lanes("perm_ybwd", v_perm_ybwd(a, b), f2u(-2.25f), f2u(0.5f), f2u(-0.5f), f2u(4.0f));
  check_lanes("perm_zwab", v_perm_zwab(a, b), f2u(3.75f), f2u(-0.5f), f2u(2.0f), f2u(0.5f));
  check_lanesi("permi_yzxw", v_permi_yzxw(ia), 0xFFFFFFF9u, 0x1E240u, 3u, 0x88CA6C00u);
  check_lanesi("permi_zwzw", v_permi_zwzw(ia), 0x1E240u, 0x88CA6C00u, 0x1E240u, 0x88CA6C00u);
  check_lanesi("permi_xxyy", v_permi_xxyy(ia), 3u, 3u, 0xFFFFFFF9u, 0xFFFFFFF9u);
  check_lanesi("permi_xxzz", v_permi_xxzz(ia), 3u, 3u, 0x1E240u, 0x1E240u);
  check_lanesi("permi_xyxy", v_permi_xyxy(ia), 3u, 0xFFFFFFF9u, 3u, 0xFFFFFFF9u);
  check_lanesi("permi_xzxz", v_permi_xzxz(ia), 3u, 0x1E240u, 3u, 0x1E240u);
  check_lanesi("permi_ywyw", v_permi_ywyw(ia), 0xFFFFFFF9u, 0x88CA6C00u, 0xFFFFFFF9u, 0x88CA6C00u);
  check_lanesi("permi_yyww", v_permi_yyww(ia), 0xFFFFFFF9u, 0xFFFFFFF9u, 0x88CA6C00u, 0x88CA6C00u);
  check_lanesi("permi_yzxy", v_permi_yzxy(ia), 0xFFFFFFF9u, 0x1E240u, 3u, 0xFFFFFFF9u);
  check_lanesi("permi_zzww", v_permi_zzww(ia), 0x1E240u, 0x1E240u, 0x88CA6C00u, 0x88CA6C00u);
  check_lanesi("permi_wwyy", v_permi_wwyy(ia), 0x88CA6C00u, 0x88CA6C00u, 0xFFFFFFF9u, 0xFFFFFFF9u);

  {
    alignas(16) float aos[16];
    for (int k = 0; k < 16; k++) aos[k] = (float)k;
    vec4f x, y, z, w;
    v_ldu_soa2(aos, x, y);
    check_lanes("ldu_soa2_x", x, f2u(0.f), f2u(2.f), f2u(4.f), f2u(6.f));
    check_lanes("ldu_soa2_y", y, f2u(1.f), f2u(3.f), f2u(5.f), f2u(7.f));
    v_ld_soa2(aos, x, y);
    check_lanes("ld_soa2_x", x, f2u(0.f), f2u(2.f), f2u(4.f), f2u(6.f));
    check_lanes("ld_soa2_y", y, f2u(1.f), f2u(3.f), f2u(5.f), f2u(7.f));
    v_ldu_soa3(aos, x, y, z);
    check_lanes("ldu_soa3_x", x, f2u(0.f), f2u(3.f), f2u(6.f), f2u(9.f));
    check_lanes("ldu_soa3_y", y, f2u(1.f), f2u(4.f), f2u(7.f), f2u(10.f));
    check_lanes("ldu_soa3_z", z, f2u(2.f), f2u(5.f), f2u(8.f), f2u(11.f));
    v_ld_soa3(aos, x, y, z);
    check_lanes("ld_soa3_x", x, f2u(0.f), f2u(3.f), f2u(6.f), f2u(9.f));
    check_lanes("ld_soa3_z", z, f2u(2.f), f2u(5.f), f2u(8.f), f2u(11.f));
    v_ldu_soa4(aos, x, y, z, w);
    check_lanes("ldu_soa4_w", w, f2u(3.f), f2u(7.f), f2u(11.f), f2u(15.f));
    v_ld_soa4(aos, x, y, z, w);
    check_lanes("ld_soa4_x", x, f2u(0.f), f2u(4.f), f2u(8.f), f2u(12.f));
    check_lanes("ld_soa4_w", w, f2u(3.f), f2u(7.f), f2u(11.f), f2u(15.f));
    alignas(16) float back[16] = {0};
    v_ld_soa4(aos, x, y, z, w);
    v_st_soa2(back, x, y);
    check_int("st_soa2", (long long)f2u(back[5]), (long long)f2u(9.f));
    v_stu_soa2(back, x, y);
    check_int("stu_soa2", (long long)f2u(back[2]), (long long)f2u(4.f));
    v_st_soa3(back, x, y, z);
    check_int("st_soa3_y", (long long)f2u(back[4]), (long long)f2u(5.f));
    check_int("st_soa3_hi", (long long)f2u(back[11]), (long long)f2u(14.f));
    v_stu_soa3(back, x, y, z);
    check_int("stu_soa3_rt", (long long)f2u(back[3]), (long long)f2u(4.f));
    check_int("stu_soa3_rt2", (long long)f2u(back[11]), (long long)f2u(14.f));
    v_st_soa4(back, x, y, z, w);
    check_int("st_soa4", (long long)f2u(back[6]), (long long)f2u(6.f));
    v_stu_soa4(back, x, y, z, w);
    check_int("stu_soa4", (long long)f2u(back[9]), (long long)f2u(9.f));
#if !defined(_TARGET_SIMD_NEON) // v_interleave3/4 are not part of the NEON backend (it stores SoA via vst3q/vst4q directly)
    vec4f e0, e1, e2;
    v_interleave3(x, y, z, e0, e1, e2);
    check_lanes("interleave3_e0", e0, f2u(0.f), f2u(1.f), f2u(2.f), f2u(4.f));
    check_lanes("interleave3_e2", e2, f2u(10.f), f2u(12.f), f2u(13.f), f2u(14.f));
    vec4f i0, i1, i2, i3;
    v_interleave4(x, y, z, w, i0, i1, i2, i3);
    check_lanes("interleave4_e0", i0, f2u(0.f), f2u(1.f), f2u(2.f), f2u(3.f));
    check_lanes("interleave4_e1", i1, f2u(4.f), f2u(5.f), f2u(6.f), f2u(7.f));
    check_lanes("interleave4_e3", i3, f2u(12.f), f2u(13.f), f2u(14.f), f2u(15.f));
#endif
  }

  {
    const float m43[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    mat43f t43;
    v_mat43_make_from_43cu_unsafe(t43, m43);
    check_int("mat43_unsafe_r0x", (long long)f2u(v_extract_x(t43.row0)), (long long)f2u(1.f));
    check_int("mat43_unsafe_r0z", (long long)f2u(v_extract_z(t43.row0)), (long long)f2u(7.f));
    check_int("mat43_unsafe_r2y", (long long)f2u(v_extract_y(t43.row2)), (long long)f2u(6.f));
  }

  // vd_min/vd_max second-operand rule holds on every backend by contract - NaN row unguarded
  {
    vec4d da = vd_make_vec4d(1.5, -2.25, 3.75, -0.5);
    vec4d db = vd_make_vec4d(2.0, 0.5, -1.0, 4.0);
    check_double("vd_extract_y", vd_extract_y(da), -2.25);
    check_double("vd_add", vd_extract_z(vd_add(da, db)), 2.75);
    check_double("vd_sub", vd_extract_z(vd_sub(da, db)), 4.75);
    check_double("vd_mul", vd_extract_w(vd_mul(da, db)), -2.0);
    check_double("vd_div", vd_extract_x(vd_div(da, db)), 0.75);
    check_double("vd_neg", vd_extract_x(vd_neg(da)), -1.5);
    check_double("vd_zero_x", vd_extract_x(vd_zero()), 0.0);
    check_double("vd_zero_w", vd_extract_w(vd_zero()), 0.0);
    check_double("vd_min_nan", vd_extract_x(vd_min(vd_splats((double)nanf_v), vd_splats(1.0))), 1.0);
    check_int("vd_max_tie", std::signbit(vd_extract_x(vd_max(vd_splats(0.0), vd_splats(-0.0)))) ? 1 : 0, 1);
    check_double("vd_sqrt", vd_extract_y(vd_sqrt(vd_make_vec4d(4, 9, 16, 25))), 3.0);
    check_double("vd_sqrt_x", vd_extract_x(vd_sqrt_x(vd_make_vec4d(4, 9, 16, 25))), 2.0);
    vec4d dcl = vd_clamp(da, vd_zero(), vd_splats(1.0));
    check_double("vd_clamp_x", vd_extract_x(dcl), 1.0);
    check_double("vd_clamp_y", vd_extract_y(dcl), 0.0);
    check_double("vd_clamp_z", vd_extract_z(dcl), 1.0);
#if !defined(_TARGET_SIMD_NEON) // NEON vd_sqrt_x sqrts the whole low half; SSE/scalar preserve .y
    check_int("vd_sqrt_x_keeps_y", vd_extract_y(vd_sqrt_x(vd_make_vec4d(4, 9, 16, 25))) == 9.0 ? 1 : 0, 1);
#endif
    check_double("vd_hadd4_x", vd_extract_x(vd_hadd4_x(da)), 2.5);
    check_double("vd_hadd3_x", vd_extract_x(vd_hadd3_x(da)), 3.0);
    check_double("vd_hadd4_bcast", vd_extract_z(vd_hadd4(da)), 2.5);
    check_double("vd_hadd3_bcast", vd_extract_y(vd_hadd3(da)), 3.0);
    check_double("vd_hadd4_assoc", vd_extract_x(vd_hadd4_x(vd_make_vec4d(1e16, 1.0, -1e16, 1.0))), 1.0);
    check_double("vd_hadd3_assoc", vd_extract_x(vd_hadd3_x(vd_make_vec4d(1.0, 1e16, -1e16, 99.0))), 0.0);
    check_double("vd_dot3", vd_extract_x(vd_dot3(da, db)), -1.875);
    check_double("vd_dot3_x", vd_extract_x(vd_dot3_x(da, db)), -1.875);
    check_double("vd_dot4_x", vd_extract_x(vd_dot4_x(da, db)), -3.875);
    check_double("vd_dot4_bcast", vd_extract_y(vd_dot4(da, db)), -3.875);
    vec4d dv = vd_make_vec4d(1.0, 2.0, 2.0, 4.0);
    check_double("vd_length3_sq", vd_extract_y(vd_length3_sq(dv)), 9.0);
    check_double("vd_length4_sq", vd_extract_z(vd_length4_sq(dv)), 25.0);
    check_double("vd_length3", vd_extract_x(vd_length3(dv)), 3.0);
    check_double("vd_length4", vd_extract_y(vd_length4(dv)), 5.0);
    check_double("vd_length3_x", vd_extract_x(vd_length3_x(dv)), 3.0);
    check_double("vd_length4_x", vd_extract_x(vd_length4_x(dv)), 5.0);
    vec4d dc = vd_cross3(vd_make_vec4d(1, 2, 3, 0), vd_make_vec4d(4, 5, 6, 0));
    check_double("vd_cross3_x", vd_extract_x(dc), -3.0);
    check_double("vd_cross3_y", vd_extract_y(dc), 6.0);
    check_double("vd_cross3_z", vd_extract_z(dc), -3.0);
    check_double("vd_insert_x", vd_extract_x(vd_insert_x(da, 7.5)), 7.5);
    check_double("vd_insert_y", vd_extract_y(vd_insert_y(da, 7.5)), 7.5);
    check_double("vd_insert_z", vd_extract_z(vd_insert_z(da, 7.5)), 7.5);
    check_double("vd_insert_w_w", vd_extract_w(vd_insert_w(da, 7.5)), 7.5);
    check_double("vd_insert_w_z", vd_extract_z(vd_insert_w(da, 7.5)), 3.75);
    alignas(32) double dbuf[4] = {10.5, -11.25, 12.75, -13.5};
    check_double("vd_ld_z", vd_extract_z(vd_ld(dbuf)), 12.75);
    vd_st(dbuf, da);
    check_double("vd_st_lo", dbuf[0], 1.5);
    check_double("vd_st_hi", dbuf[3], -0.5);
    double ubuf[5];
    vd_stu(ubuf + 1, db);
    check_double("vd_stu_lo", ubuf[1], 2.0);
    check_double("vd_stu_hi", ubuf[4], 4.0);
    check_double("vd_ldu_y", vd_extract_y(vd_ldu(ubuf + 1)), 0.5);
    vec4f df = vd_cvt_to_vec4f(da);
    check_lanes("vd_to_vec4f", df, f2u(1.5f), f2u(-2.25f), f2u(3.75f), f2u(-0.5f));
    vec4d d2 = vd_cvt_from_vec4f(a);
    check_int("vd_from_vec4f", vd_extract_z(d2) == 3.75 ? 1 : 0, 1);
    vec4d di = vd_cvt_from_vec4i(v_make_vec4i(3, -7, 123456, -2000000000));
    check_int("vd_from_vec4i", vd_extract_w(di) == -2000000000.0 ? 1 : 0, 1);
#if !defined(_TARGET_SIMD_NEON) // NEON converts saturate; SSE/scalar yield INT32_MIN out of range
    check_lanesi("vd_to_vec4i_oor", vd_cvt_to_vec4i(vd_make_vec4d(3e9, -3e9, 1.0, -1.0)),
                 0x80000000u, 0x80000000u, 1u, 0xFFFFFFFFu);
#endif
    check_lanesi("vd_to_vec4i", vd_cvt_to_vec4i(vd_make_vec4d(1.9, -2.9, 100.5, -0.5)),
                 1u, 0xFFFFFFFEu, 100u, 0u);
    const double p3d[3] = {9.0, 8.0, 7.0};
    vec4d dp = vd_ldu_p3_safe(p3d);
    check_double("vd_ldu_p3_x", vd_extract_x(dp), 9.0);
    check_double("vd_ldu_p3_z", vd_extract_z(dp), 7.0);
    check_double("vd_ldu_p3_w", vd_extract_w(dp), 0.0);
    double outd[4] = {0, 0, 0, -1.0};
    vd_stu_p3(outd, da);
    check_double("vd_stu_p3_z", outd[2], 3.75);
    check_double("vd_stu_p3_sentinel", outd[3], -1.0);
  }

  if (g_failed) { printf("%d vecmath backend checks FAILED\n", g_failed); return 1; }
  printf("all vecmath backend checks passed\n");
  return 0;
}
