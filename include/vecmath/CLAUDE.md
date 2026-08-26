# vecmath - SIMD Math Library

Platform-abstracted SIMD math for transforms, physics, BVH traversal, culling and animation:
one `v_`-prefixed C API over whichever backend the target selects, plus a `vd_`-prefixed double
layer. The backend ladder is the `#if` chain in `dag_vecMathDecl.h`.

| File | Contents |
|------|----------|
| `dag_vecMathDecl.h` | Types, backend `#if` ladder, alignment macros |
| `dag_vecMath.h` | The API reference - every `v_` function declared with a comment; grep it by prefix (`v_mat44_`, `v_bbox3_`, `v_quat_`, `v_frustum_`) before hand-rolling |
| `dag_vecMath_const.h` | Constants (`V_C_*`, `V_CI_MASK*`) |
| `dag_vecMath_pc_sse.h` / `dag_vecMath_neon.h` / `dag_vecMath_scalar.h` | The three backends |
| `dag_vecMath_common.h` | Implementations written once on top of the primitives (bbox, frustum, quat, matrix) |
| `dag_vecMath_trig.h` | Polynomial sin/cos/tan/atan/asin/acos |
| `dag_vecMath_double.h` | The `vec4d` / `vd_` layer, per-backend arms in one file |
| `usage.md` | Which call to write, and the correctness traps (mask canonicality, mirrored matrices, FMA contraction, `.w` discipline) |
| `microarch.md` | Per-instruction cost and target hardware, for when no usage rule answers |
| `vec4d.md` | Anything `vec4d` / `vd_` |
| `README.md` / `LICENSE` | Upstream's own; the README is stale in places (says 7 headers, SSE+NEON only) - this file wins |

**Read the vendored doc that answers the question before adding or porting a function.** They
describe the upstream engine, not this repo: their build flags and library references
(`Point3_vec4`, physJolt, rendInst) are Dagor's, and all three predate the scalar backend, so
`vec4d.md` still calls `dag_vecMath_double.h` an SSE/AVX-and-NEON file.

**This folder vendors DagorEngine `prog/1stPartyLibs/vecmath/`** - not
`prog/engine/publicInclude/vecmath/`, a second, non-canonical copy upstream. `CLAUDE.md` is this
repo's own. Record the synced revision here on every sync; without it "matches upstream" has no
second operand. Synced: `75723669` (2026-08-25).

**A file that mentions `_TARGET_SIMD_SCALAR`, plus `dag_vecMath_scalar.h`, is fork-local; every
other file here stays byte-identical to upstream.** A sync replaces the byte-identical set
wholesale from upstream and re-applies the fork-local hooks on top, so a local edit to a
byte-identical file is lost without warning - land the change upstream first, or route it through
a fork-local hook.

**A sync that brings in new or changed `v_`/`vd_` functions gives each one its scalar arm in the
same change** - in `dag_vecMath_scalar.h`, or in `dag_vecMath_common.h` if it is written once on
top of the primitives. Upstream maintains SSE and NEON; nobody but this repo maintains scalar, and
a missing arm compiles everywhere except the target that selects it.

**The scalar backend is auto-selected on targets with no SIMD ISA (Cortex-M, RISC-V without V)
and forced elsewhere by the CMake option `DAS_VECMATH_SCALAR`, which defines
`_TARGET_SIMD_SCALAR=1`.** No header tests `DAS_VECMATH_SCALAR` - it exists so the x64 suite can
validate the backend.

**`vec4d` stays a local compute type: never a struct field crossing a TU boundary, never
serialized.** Its layout is backend- and flag-dependent - one `__m256d` under `__AVX__`, two
128-bit halves (`.xy`, `.zw`) on SSE without AVX (the default no-`/arch:AVX` x64 build) and on
NEON, a `double d[4]` on the scalar backend - so it can differ between translation units of one
binary; `VECMATH_VEC4D_256` reports which one a TU got.

**`vec4f_scalar_t` / `vec4i_scalar_t` are defined identically in `dag_vecMathDecl.h` and in
`include/daScript/daScriptC.h` (repo root), guarded by `VECMATH_SCALAR_TYPES_DEFINED`.** Whichever
header is included first defines the pair for both, so a change to tag name, members or alignment
lands in both files in the same change - otherwise the C API and the C++ API disagree on layout
with no diagnostic.

Include `<vecmath/dag_vecMath.h>` for the full API, `<vecmath/dag_vecMathDecl.h>` for types only.
