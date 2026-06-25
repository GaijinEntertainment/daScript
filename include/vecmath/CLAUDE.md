# vecmath - SIMD Math Library

## Overview
Platform-abstracted SIMD vector math library. Wraps SSE2/SSSE3/SSE4.1 (x86) and NEON (ARM)
behind a unified C API. Used pervasively throughout the Dagor Engine for all performance-critical
math: transforms, physics, BVH traversal, culling, animation, etc.

## Key Types (dag_vecMathDecl.h)
- `vec4f` / `vec3f` -- 128-bit float vector (__m128 on SSE, float32x4_t on NEON)
- `vec4i` -- 128-bit integer vector (__m128i / int32x4_t)
- `mat33f` -- 3x3 column-major matrix (3 x vec3f)
- `mat44f` -- 4x4 column-major matrix (4 x vec4f)
- `mat43f` -- 4x3 row-major matrix (3 x vec4f, each row is xyzw where w = translation component)
- `bbox3f` -- AABB {bmin, bmax} as two vec4f
- `bsph3f` -- bounding sphere {center.xyz, radius.w}
- `quat4f` -- quaternion as vec4f
- `plane3f` -- plane {normal.xyz, D.w}

## File Layout
| File | Contents |
|------|----------|
| `dag_vecMathDecl.h` | Type declarations, platform detection, alignment macros |
| `dag_vecMath.h` | Full API declarations (~900 functions), platform-independent |
| `dag_vecMath_const.h` | Constants: V_C_HALF, V_C_ONE, V_C_PI, V_C_UNIT_1000, V_CI_MASK*, etc. |
| `dag_vecMath_pc_sse.h` | SSE implementation of all functions |
| `dag_vecMath_neon.h` | NEON (ARM) implementation of all functions |
| `dag_vecMath_common.h` | Shared implementations (bbox, frustum, quat, matrix ops built on core intrinsics) |
| `dag_vecMath_trig.h` | Polynomial approximations for sin/cos/tan/atan/asin/acos |

## Naming Conventions
- `v_` prefix for all functions
- `v_splats(float)` -- broadcast scalar to all lanes
- `v_splatsi(int)` -- broadcast int scalar
- `v_splat_x/y/z/w(v)` -- broadcast one lane
- `v_perm_XYZW(v)` -- swizzle/permute (e.g. `v_perm_yzxw`)
- `v_dot3_x(a,b)` -- dot product result in .x only (faster)
- `v_dot3(a,b)` -- dot product broadcast to all lanes
- `v_cross3(a,b)` -- 3D cross product
- `v_length3/v_length3_x` -- length (broadcast vs .x-only)
- `v_norm3(v)` -- normalize
- `v_madd(a,b,c)` -- fused multiply-add: a*b+c
- `v_rcp(v)` / `v_rcp_est(v)` -- reciprocal (precise / estimate)
- `v_rsqrt(v)` / `v_rsqrt_est(v)` -- reciprocal sqrt
- `v_cmp_gt/ge/lt/le/eq(a,b)` -- compare, returns mask (0xFFFFFFFF or 0)
- `v_and/v_or/v_xor/v_andnot/v_sel/v_btsel` -- bitwise ops and select
- `v_cast_vec4i/v_cast_vec4f` -- reinterpret cast (no conversion)
- `v_cvt_vec4i/v_cvt_vec4f` -- convert int<->float
- `v_half_to_float(v)` / `v_float_to_half_rtne(v)` -- FP16 conversion
- `v_float_to_half_up/down` -- directional rounding for conservative bounds
- `v_check_xyz_all_true(v)` -- test mask results
- `v_signmask(v)` -- extract sign bits as int (1|2|4|8)

## Matrix Operations
- `v_mat44_mul(dest, m1, m2)` -- 4x4 multiply
- `v_mat44_inverse(dest, m)` / `v_mat44_inverse43(dest, m)` -- inverse
- `v_mat44_transpose(dest, src)` -- transpose
- `v_mat43_transpose_to_mat44(dest, src)` -- row-major 4x3 -> column-major 4x4
- `v_mat44_mul_vec3p(m, v)` -- transform point (w=1)
- `v_mat44_mul_vec3v(m, v)` -- transform direction (w=0)
- `v_mat33_mul_vec3(m, v)` -- 3x3 transform
- `v_mat43_mul_vec3p(m, v)` -- row-major 4x3 point transform

## AABB Operations (bbox3f)
- `v_bbox3_init_empty(b)` / `v_bbox3_init(b, p)` -- create
- `v_bbox3_add_pt(b, p)` / `v_bbox3_add_box(b, b2)` -- extend
- `v_bbox3_center(b)` / `v_bbox3_size(b)` -- queries
- `v_bbox3_test_pt_inside(b, p)` -- containment
- `v_bbox3_test_box_intersect(b1, b2)` -- overlap
- `v_bbox3_rotate_init(b, col0, col1, col2, bb)` -- oriented AABB

## Performance Notes
- Functions marked VECMATH_FINLINE are always force-inlined
- VECTORCALL convention used on MSVC/Clang for SSE targets (passes vec4f in registers)
- `_x` suffix variants (e.g. v_dot3_x) are faster -- result only in .x, no broadcast
- `_est` variants use hardware estimate + optional Newton-Raphson (faster, less precise)
- `_unprecise` variants use raw hardware estimate only (fastest, least precise)
- Avoid v_extract_x/y/z/w in hot loops -- moves data from SIMD to scalar registers
- .w component of vec3f can be anything (even NaN) for 3D operations
- v_ldu_p3 loads 4 floats for a 3-component vector (fast but reads 1 extra float)
- v_ldu_p3_safe only reads exactly 3 floats (slower, use at page boundaries)

## Include Path
```cpp
#include <vecmath/dag_vecMath.h>        // full API
#include <vecmath/dag_vecMathDecl.h>    // types only (forward declarations)
```

IMPORTANT: This is `prog/1stPartyLibs/vecmath/`, NOT `prog/engine/publicInclude/vecmath/`.
The 1stPartyLibs version is the correct one used by most engine code.
