# vecmath

A small, header-only SIMD vector math library with one portable API across x86
(SSE2/SSSE3/SSE4.1), ARM (NEON / AArch64), and any other CPU through a scalar
per-lane backend. Write your math once; it compiles to
good vector code on desktop, consoles, and mobile.

vecmath is the math core of the [Dagor Engine](https://github.com/GaijinEntertainment/DagorEngine)
and powers its transforms, physics, culling, and animation. This repository is the
standalone, dependency-free version of those headers.

## Why

- **One API, many CPUs.** You call `v_add`, `v_mat44_mul`, `v_norm3`. The header
  selects the SSE or NEON implementation for whatever you build for. No `#ifdef`
  soup in your own code.
- **Header-only, no dependencies.** Add the include path and go. Nothing to build
  or link.
- **Zero-overhead.** Types are the native SIMD registers (`__m128` / `float32x4_t`),
  passed in registers. Almost everything is force-inlined, so unused results melt
  away and there is no wrapper-object cost.
- **Batteries included.** Vectors, 3x3 / 4x3 / 4x4 matrices, quaternions, planes,
  bounding boxes and spheres, frustum culling, ray/triangle intersection, fast
  trig/exp approximations, and a double-precision `vec4d` layer.

## Requirements

- C++11 or later.
- An x86 target with at least SSE2, or an AArch64 (ARMv8) target with NEON;
  any other target (Cortex-M, RISC-V without V, ...) uses the scalar backend,
  selected automatically or forced with `_TARGET_SIMD_SCALAR=1`.
- MSVC, Clang, or GCC.

The target ISA is auto-detected from the usual compiler macros (`__SSE4_1__`,
`__ARM_NEON`, ...). To pin it explicitly, define one of these before including:

```cpp
#define _TARGET_SIMD_SSE 4   // 2 = SSE2, 3 = SSSE3, 4 = SSE4.1
// or
#define _TARGET_SIMD_NEON 1
```

## Getting started

This is a header-only library. Copy the headers somewhere on your include path
(a `vecmath/` subfolder is the convention) and include the API:

```cpp
#include <vecmath/dag_vecMath.h>       // full API
#include <vecmath/dag_vecMathDecl.h>   // types only (forward declarations)
```

With CMake you can expose it as an interface target:

```cmake
add_library(vecmath INTERFACE)
target_include_directories(vecmath INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
# consumers: target_link_libraries(my_app PRIVATE vecmath)
```

## Types

| Type | Meaning |
|------|---------|
| `vec4f` / `vec3f` | 128-bit float vector. `vec3f` is a `vec4f` whose `.w` lane is ignored. |
| `vec4i` | 128-bit integer vector (four 32-bit ints) |
| `mat33f` | 3x3 matrix, column-major (`col0..col2`) |
| `mat44f` | 4x4 matrix, column-major (`col0..col3`) |
| `mat43f` | 4x3 matrix, row-major (`row0..row2`); each row's `.w` is the translation |
| `quat4f` | quaternion (stored as a `vec4f`) |
| `plane3f` | plane: `.xyz` = normal, `.w` = D |
| `bbox3f` | axis-aligned bounding box `{bmin, bmax}` |
| `bsph3f` | bounding sphere: `.xyz` = center, `.w` = radius |

## Examples

### Frustum + LOD culling pass

Build the frame's world-to-clip matrix, then collect the visible objects and their
LOD in one sweep:

```cpp
void cullScene(vec3f camPos, vec3f camDir, vec3f camUp, float wk, float hk, float zn, float zf,
               const SceneObj *objs, int count, VisibleObj *out, int &outCount)
{
  mat44f view, proj, globtm;
  v_mat44_make_look_at(view, camPos, v_add(camPos, camDir), camUp);
  v_mat44_make_persp_reverse(proj, wk, hk, zn, zf);        // reverse-Z
  v_mat44_mul(globtm, proj, view);

  outCount = 0;
  for (int i = 0; i < count; i++)
  {
    const bbox3f &b = objs[i].worldBox;
    if (v_test_vec_x_eqi_0(v_is_visible(b.bmin, b.bmax, globtm)))
      continue;                                            // fully outside the frustum

    float d2 = v_extract_x(v_length3_sq_x(v_sub(v_bbox3_center(b), camPos)));
    out[outCount++] = { i, d2 > objs[i].lodSwitchDistSq }; // pick LOD from squared distance
  }
}
```

### Radial explosion with line-of-sight

Distance falloff plus an occlusion raycast against world geometry:

```cpp
void applyBlast(vec3f center, float radius, float maxDmg,
                Actor *actors, int actorCount, const bbox3f *world, int worldCount)
{
  for (int a = 0; a < actorCount; a++)
  {
    vec3f to   = v_sub(actors[a].pos, center);
    float dist = v_extract_x(v_length3_x(to));
    if (dist >= radius)
      continue;                                            // out of range

    vec3f dir = v_norm3(to);
    bool blocked = false;
    for (int w = 0; w < worldCount && !blocked; w++)
    {
      vec3f t;
      blocked = v_ray_box_intersection(center, dir, t, world[w]) && v_extract_x(t) < dist;
    }
    if (!blocked)
      actors[a].hp -= maxDmg * (1.f - dist / radius);      // linear falloff
  }
}
```

### Turret: smoothly aim and rebuild its transform

Shortest-arc aiming slerped by this frame's turn rate, composed straight into a
world matrix:

```cpp
void updateTurret(Turret &t, vec3f target, float turnFrac)
{
  vec3f facing = v_quat_mul_vec3(t.rot, t.restDir);        // current muzzle direction
  vec3f wanted = v_norm3(v_sub(target, t.pos));
  quat4f goal  = v_quat_mul_quat(v_quat_from_unit_arc(facing, wanted), t.rot);

  t.rot = v_quat_slerp(v_splats(turnFrac), t.rot, goal);
  v_mat44_compose(t.tm, t.pos, t.rot, V_C_ONE);            // pos + rotation, unit scale
}
```

### Naming conventions

- `_x` - result kept in the `.x` lane only; the other lanes may hold anything.
  Cheaper than the all-lanes (broadcast) form.
- `_est` - fast hardware estimate refined toward useful precision.
- `_unprecise` - raw hardware estimate; fastest, least precise.
- `_safe` - tolerates edge inputs such as zero divisors.
- `v_ld` / `v_st` are aligned; `v_ldu` / `v_stu` are unaligned; a trailing `i` is
  the integer variant.
- `v_cast_*` reinterprets the bits (free); `v_cvt_*` converts the value
  (int <-> float).

## Files

| File | Contents |
|------|----------|
| `dag_vecMathDecl.h` | Type declarations, platform detection, alignment macros |
| `dag_vecMath.h` | Full API declarations with comments |
| `dag_vecMath_const.h` | Named constants (`V_C_HALF`, `V_C_ONE`, `V_C_PI`, lane masks, ...) |
| `dag_vecMath_pc_sse.h` | SSE implementation |
| `dag_vecMath_neon.h` | NEON (AArch64) implementation |
| `dag_vecMath_common.h` | Portable implementations built on the core intrinsics |
| `dag_vecMath_trig.h` | Polynomial sin / cos / tan / atan / asin / acos approximations |

## License

BSD-3-Clause. See [LICENSE](LICENSE). Copyright (C) Gaijin Games KFT.
