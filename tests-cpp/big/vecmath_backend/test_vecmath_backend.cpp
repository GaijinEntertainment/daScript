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
#include <stdio.h>
#include <string.h>
#include <math.h>

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

  if (g_failed) { printf("%d vecmath backend checks FAILED\n", g_failed); return 1; }
  printf("all vecmath backend checks passed\n");
  return 0;
}
