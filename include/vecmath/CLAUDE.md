# vecmath - SIMD Math Library

## Overview
Platform-abstracted SIMD vector math library. Wraps SSE2/SSSE3/SSE4.1 (x86), NEON (ARM) and a
scalar per-lane fallback for targets with no SIMD ISA behind a
unified C API. Used pervasively throughout the Dagor Engine for all performance-critical math:
transforms, physics, BVH traversal, culling, animation, etc.

## Key Types (dag_vecMathDecl.h)
- `vec4f` / `vec3f` -- 128-bit float vector (__m128 on SSE, float32x4_t on NEON, a 16-byte struct on scalar)
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
| `dag_vecMath.h` | Full API declarations (~650 functions) with comments, platform-independent |
| `dag_vecMath_const.h` | Constants: V_C_HALF, V_C_ONE, V_C_PI, V_C_UNIT_1000, V_CI_MASK*, etc. |
| `dag_vecMath_pc_sse.h` | SSE low-level implementation of basic functions |
| `dag_vecMath_neon.h` | NEON (ARM) low-level implementation of basic functions |
| `dag_vecMath_scalar.h` | Scalar per-lane implementation of basic functions (no-SIMD fallback, forceable with `_TARGET_SIMD_SCALAR=1`) |
| `dag_vecMath_double.h` | `vec4d` double-precision math (SSE/AVX, NEON and scalar in one file); include via dag_vecMath.h |
| `dag_vecMath_common.h` | Shared implementations (bbox, frustum, quat, matrix ops built on core intrinsics) without hw-specific intrinsics |
| `dag_vecMath_trig.h` | Polynomial approximations for sin/cos/tan/atan/asin/acos |

## API Conventions
dag_vecMath.h is the API reference: every `v_`-prefixed function is declared there with a
comment. Grep it by prefix before writing a new helper - what you need probably exists
(v_mat44_*, v_mat33_*, v_mat43_*, v_quat_*, v_bbox3_*, v_bsph*, v_plane*, v_frustum*, v_ray*,
v_triangle*).

- Suffix scheme:
  - `_x` -- result in .x only, .yzw are allowed to contain anything (even NaN); cheaper than the
    broadcast form
  - `_est` -- hardware estimate refined by Newton-Raphson steps; sequence and step count vary
    per op and platform to land at similar useful precision (not an automatic win, see microarch
    notes)
  - `_unprecise` -- raw hardware estimate only (fastest, least precise)
  - `_safe` -- tolerates edge inputs (zero divisors, page-boundary loads); the divisor check
    (v_is_unsafe_divisor: |x| < V_C_VERY_SMALL_VAL, 4e-19 ~ sqrt(FLT_MIN)) is tuned to stay safe
    under squaring: if x passes, x*x does not underflow and 1/(x*x) does not overflow
- Basic rcp/sqrt/rsqrt use real division/square-root instructions, never hardware estimate
  instructions; estimates enter only through the _est/_unprecise variants
- 3-component ops ignore .w of inputs: it can be anything, even NaN. Most gameplay code safely
  carries 3d vectors in vec3f with garbage in .w, but some render code requires .w = 0. Same for
  mat44f holding a 4x3 transform: row3 (the .w lane of the columns) can be anything for gameplay,
  but the GPU requires 0,0,0,1 there
- Loads/stores: v_ld/v_st aligned, v_ldu/v_stu unaligned, *i for integer. On modern cores
  unaligned is as fast as aligned within a cache line, so aligned forms act mostly as a hardware
  alignment assert. Encodings differ: legacy (non-VEX) SSE load+op memory operands (addps xmm,
  [mem]) require alignment, so on the SSE4.1 PC build only v_ld folds into the consuming
  instruction (v_ldu stays a separate movups); VEX operands don't require alignment, both forms
  fold, and a folded v_ld loses the movaps fault; NEON never checks alignment
- v_ldu_p3 reads 4 floats for a 3-component vector (fast): harmless on the stack, but a heap
  array of packed 3-float positions whose last element ends exactly at a page end crashes on the
  final load - use v_ldu_p3_safe (reads exactly 3, slower) there. ASAN/TSAN builds route v_ldu_p3
  to the safe path automatically, so sanitizers neither catch nor reproduce the over-read
- Store ports are scarcer than load ports (big cores sustain 2-3 loads but only 1-2 stores per
  cycle), and v_stu_p3 issues two store uops (8-byte + 4-byte); when the layout allows
  overwriting the padding, prefer one full v_st/v_stu
- Lane ops: v_splats(float) / v_splatsi(int) broadcast a scalar, v_splat_x/y/z/w broadcast a lane,
  v_extract_x/y/z/w read a lane out
- Swizzles: v_perm_<lanes> with xyzw lane names; two-vector perms name the second operand's lanes
  abcd (v_perm_xzac); v_rot_1/2/3 rotate lanes. These are hand-mapped to cheap forms on both ISAs -
  prefer them over raw intrinsics or exotic custom shuffles
- Compares (v_cmp_gt/ge/lt/le/eq) return per-lane masks (all-ones or 0); consume masks with bitwise
  ops (v_and/v_or/v_xor/v_andnot), branchless selects (v_sel/v_sel_b/v_btsel/v_bbox3_sel), the
  v_check_*/v_test_* branching helpers, or v_signmask (sign bits as int)
- v_cast_* reinterprets bits (free), v_cvt_* converts values (int<->float)
- FP16: v_half_to_float / v_float_to_half_rtne; _up/_down variants round directionally for
  conservative bounds

## Correctness traps
- Matrix functions tolerate aliasing: dest may be an input (v_mat44_mul/v_mat33_mul read all
  inputs into temporaries before the first store, v_mat44_transpose takes src by value,
  v_mat44_inverse43 copies its input first). Preserve this property when adding functions -
  callers write v_mat44_mul(m, m, rel)
- v_sel selectors must be canonical per-lane masks (all-ones/zero, as v_cmp_* produce): SSE4.1
  blendvps reads only the sign bit, but the SSE2 path and NEON vbsl select per bit - a sign-only
  selector works on the PC build and silently breaks on other targets
- v_norm* of a zero or near-zero vector produces inf/NaN lanes; v_norm*_safe(a, def) returns def
  when length^2 fails the unsafe-divisor check
- Function results are usually fully defined: _x forms define .x only (see suffix scheme) and
  broadcast forms fill all 4 lanes. The few ops that return a vec3f (v_cross3, v_norm3,
  v_mat44_scale43_sq, vec3 transforms) leave .w unspecified unless the header comment says
  otherwise - do not let it leak into 4-component math

## Performance Notes
- Most functions are VECMATH_FINLINE (force-inlined) and rely on dead code elimination of unused
  results; big ones (ray-box, triangle tests) use VECMATH_INLINE (plain inline) instead - prefer
  that for new large bodies
- VECTORCALL convention used on MSVC/Clang for SSE targets (passes vec4f in registers). Vector
  ABIs differ per platform: SysV (Linux/PS) treats all xmm as scratch and passes up to 8 vector
  args in xmm0-7; Windows x64 keeps xmm6-15 callee-saved and needs __vectorcall to pass vectors
  in registers; AArch64 passes in v0-v7 and preserves only the low 64 bits of v8-v15 across
  calls. A non-inlined call therefore spills different amounts of vector state per platform
- We prefer branchless implementations using v_sel, v_sel_b, v_btsel, v_bbox3_sel
- We sometimes extract scalar floats for more readable comparisons, but never use stdlib float
  functions inside vecmath

## Our target hardware
- Main build is x86-64 Windows PC client compiled for 64-bit target with SSE 4.1
- Typical PC is Skylake/Zen or newer with AVX2+FMA; SSE 4.1 is the compatibility floor, not the
  typical machine, and Skylake (2015) is the old tail - weight optimization toward recent big
  cores. On desktop hybrids SIMD-heavy threadpool jobs mostly run on P-cores, but laptop and
  budget chips have few or no P-cores, so the small-core guidance below applies to cheap PCs too
- Calibrate PC capability against the Steam hardware survey (store.steampowered.com/hwsurvey; June
  2026: SSE4.1 98%, AVX 97%, AVX2 95%, AVX-512 23%; 6 or 8 physical cores are the most common),
  keeping in mind our games also run well on machines older than the Steam average
- Current generation consoles (Xbox Series X/S, PS5) are AMD Zen2: Scarlett builds with
  -march=znver2 (PS5 Sony clang targets the same CPU), so AVX2 + FMA + BMI2 with real 256-bit
  datapaths
- Previous generation consoles (Xbox One, PS4) are AMD Jaguar: Xbox One builds with -march=btver2
  -mno-bmi2 (PS4 Sony clang targets the same CPU), so AVX but NO FMA and no BMI2, and 256-bit
  operations are slow (emulated by 2x128)
- Console performance matters on both generations; both are active platforms
- Dedicated servers compiled for Linux 64-bit target with -march=haswell (AVX2 + FMA + BMI2; see
  skyquake/prog/jamfile)
- Mobile phones (ARMv8, mostly Android) are a first-class production platform - NEON code quality
  matters as much as SSE
- Nintendo Switch is aarch64 Cortex-A57 (-mcpu=cortex-a57+fp+simd+crypto+crc): 128-bit ASIMD
  execution without the A53/A55-style 64-bit split, but the pipes are asymmetric - integer SIMD
  dual-issues more than FP, 128-bit FP FMA is about one per cycle (half of A76+), 128-bit integer
  multiply ~0.5/cycle, FP divide blocks for 7-32 cycles
- We support an SSE2 min-cpu build (very rarely used), but ARMv7 is not supported
- Both clang (fast-math / -ffp-contract=fast) and MSVC since VS2022 (/fp:fast) contract intrinsic
  mul+add into FMA when the target has FMA; neither contracts under default FP settings.
  v_madd/v_msub/v_nmsub are the flag-independent spelling: on NEON v_madd is explicit vfmaq_f32, on
  x86 it uses FMA when the target has it. FMA also rounds differently than mul+add: the product is
  not rounded before the add - more precise, but a trap in code like a*b + c*d != 0.0, where the
  result can differ between contracted and non-contracted builds (and so between platforms). Libs
  that need cross-ISA consistency (physJolt, gamePhys/collision, rendInst) define VECMATH_NO_FMA
  and build with -mno-fma (x86), -fno-unsafe-math-optimizations and -ffp-contract=off: the define
  unfuses the explicit vfmaq in v_madd, the flags stop compiler contraction (contract=off alone
  is not enough under fast-math)
- All platforms build with fast-math semantics: clang platforms pass -ffast-math -ffinite-math-only
  plus explicit -mrecip=none, Windows and Xbox build with /fp:fast. Either way the compiler may
  reassociate and contract FP math but must NOT auto-substitute div/sqrt with rcpps/rsqrtps
  estimates - estimates enter only through the explicit _est/_unprecise API. Finite-math-only also
  means NaN/Inf tests must defeat the optimizer, which is why v_is_nan and v_is_not_finite use
  volatile
- NaN semantics are relaxed by fast-math: the compiler assumes ordered inputs and freely mirrors
  or inverts comparisons (a < b into !(a >= b), reversed or negated cmp predicates) when that
  encodes better. IEEE comparisons with a NaN operand are all false, so a mirrored form returns
  the opposite - NaN behavior in the binary can differ from what the code says, and between
  builds and platforms. Do not rely on compare direction to filter NaNs
- Denormals are flushed to zero (FTZ/DAZ on x86, FPCR.FZ on ARM), so subnormals never appear in
  our math: no gradual underflow, the smallest magnitude surviving is FLT_MIN (~1.18e-38)
- DEV builds compiled by MSVC for 32-bit target with x87 scalar operations and enabled NaN
  exceptions, but for SSE intrinsics any exceptions are disabled

## Microarchitecture notes (per-instruction cost asymmetries)

Intrinsics are not exact assembly coding: the compiler treats them as semantics, not fixed
encodings, and may use equivalent instructions when they are faster; our compilation options
(fast-math, contraction) deliberately give it that freedom. The notes below are about what is
actually emitted - when a cost matters, check the disassembly of the hot loop.

Scalar vs packed div/sqrt. On the big cores we target, scalar and packed full-precision div/sqrt
have similar throughput, but small cores serve lanes from a narrow unit, so the packed form costs
up to lanes-times more:
- AMD Jaguar (XB1/PS4): sqrtss ~16c vs sqrtps ~27c, div similar, neither fully pipelined
- Intel E-cores (Gracemont in hybrid PCs): narrow divider, packed div/sqrt ~2x worse than scalar
- ARM little cores (Cortex-A53/A55): 64-bit NEON datapath splits every 128-bit op into 2 uops, so
  the 2-lane forms sqrt/div use are half cost

Because of this, prefer _x variants + v_splat_x over packed sqrt/div whenever only one lane holds
real work: generally equivalent on big cores and faster on the narrow small cores we target. This
is why broadcast v_length*/v_norm*/v_mat44_max_scale43 route through v_sqrt_x internally. Cheap
ops (add/mul/min/max/shuffles) have no such penalty on x86; do not contort code to scalarize them.

Instructions we avoid (microcoded or high latency):
- dpps/dppd: multi-uop and high-latency on every x86 core we care about, worst on Jaguar; explicit
  mul + shuffle + add sequences schedule better and transfer naturally to NEON.
- haddps/hsubps: decode to 2 shuffles + add on all x86 cores we target - never beat explicit
  shuffle + add and they load the shuffle port; NEON pairwise vpadd/faddp IS single and cheap.
- pmulld (v_muli on SSE4): historically expensive on several Intel big-core generations (cheap
  on Zen); keep it off critical dependency chains unless measured on the actual target.
- MXCSR changes (rounding mode etc.): serialize the FP pipeline; use roundps based
  v_floor/v_ceil/v_round instead of touching control registers.
- NEON multi-register table lookups (tbl2/tbl3/tbl4): 2-4x cost on little cores; single-register
  vqtbl1q is fine.

Port-unique units and domain crossings:
- Skylake-family Intel big cores execute ALL vector shuffles on one port (port 5); blends and
  bitwise logic run on 3 ports, so in shuffle-heavy code prefer blend-based two-vector perms
  (v_perm_xbzw etc.) when the lane pattern allows, and and-masks or immediate blends over
  insertps/unpck for zeroing/merging lanes; movss/movsd register merges are shuffles too. Ice
  Lake adds a second in-lane shuffle port (cross-lane stays on port 5), Zen always has two, so
  this is mainly a Skylake-era concern - tuning for one shuffle port never hurts newer cores.
- FP mul/add/FMA have two ports on big x86 cores, so dependency chains, not ports, are usually the
  limit: structure reductions as two independent accumulator chains joined at the end (the
  v_mat44_mul_vec4 pattern).
- A compile-time vector constant (v_splats(1.f)) is typically a full-width constant-pool load -
  load-port work; splatting a runtime scalar already in a register costs a shuffle instead.
  vbroadcastss helps only with a memory source; from a register it is another port 5 shuffle.
- Register-register moves (movaps) are usually eliminated at rename on big x86 cores, but they
  cost front-end bandwidth, elimination has limits (errata disabled it on some generations), and
  Jaguar executes them: do not fear compiler temporaries, but eyeball move-heavy hot loops.
- Modern ARM big cores (Cortex-A76+, Apple M) have two symmetric 128-bit ASIMD pipes that accept
  nearly everything; older A57/A72 concentrate FP on one pipe. Shuffle-port pressure is an
  x86/Intel concern; the scarce NEON resources are the divider and little-core width (see above).
- Divider/sqrt capacity is scarce on all targets: big x86 cores pipeline FP div/sqrt partially
  (Skylake divps: ~11c latency, a result per ~3c), ARM cores through A72 block outright (7-32
  cycles, no overlap), and consecutive independent div/sqrt serialize either way. When all lanes
  carry real work, batch them into one packed op (the v_mat44_remove_scale33 pattern: one rsqrt
  for three lengths); when only one lane does, use _x forms (see above).
- GPR<->vector crossings (movd/pinsr/pextr, umov/fmov on ARM) cost several cycles each way and are
  especially slow on Jaguar. Keep math in vector registers; extract once at the end. movmskps/ptest
  results live on the GPR side: fine for branching, avoid feeding them back into vector code.
- Integer shuffles on float data (and vice versa) can add 1-cycle bypass delays on Jaguar and older
  Intel cores; prefer same-domain shuffles when an equivalent exists.

SSE habits that do not transfer to NEON (most developers know only SSE):
- x86 has movmskps, so v_signmask + integer compare is a cheap idiom there. NEON has no movemask
  instruction and emulates it: with current clang codegen v_signmask(a) == 0b1111 is 10
  instructions vs 5 for v_check_xyzw_all_true(a) (on SSE both are 4). Prefer the
  v_check_*/v_test_* helpers, which pick the optimal per-platform form, or stay branchless.
- shufps encodes any lane pattern in one op, so SSE code shuffles freely. NEON has no generic
  immediate shuffle: clang maps common patterns to ext/zip/uzp/trn/rev, but exotic ones lower to
  tbl with a loaded index constant - another reason to use the named v_perm_*/v_rot_* helpers.

AMD vs Intel: most published optimization data is Intel-focused, yet Zen-aware tuning is
especially relevant for us - current-generation consoles are Zen 2, and AMD is ~45% of CPUs in
the Steam Hardware Survey, a large and growing share of the gaming-PC audience:
- several Intel costs simply do not exist on Zen: the single shuffle port (Zen always has two),
  expensive pmulld (~3-4c on Zen), and variable blends (blendvps, i.e. v_sel) are 1 uop on Zen vs
  2+ on most Intel cores
- Zen has separate FP add and FP mul/FMA pipes, so mixed add/mul streams sustain twice the
  throughput of Skylake-era Intel (which ran both on the same two FMA ports until Golden Cove
  added dedicated adders)
- dpps is even worse on AMD than on Intel (already avoided, see above)
- FMA latency is 5 cycles on Zen1/Zen2 (4 on Zen3+ and Intel big cores): madd dependency chains
  run a cycle per link slower on current consoles than Intel numbers suggest, so independent
  accumulators matter even more there
- AVX2 gathers are many-uop and slow on Zen2: on consoles vgatherdps is no faster than the
  equivalent scalar loads, so do not port Intel-tuned gather tricks there
- rcpps/rsqrtps estimate bits differ between AMD and Intel (and NEON differs from both), so
  _est/_unprecise results are not bit-reproducible across machines; keep estimates out of code
  that must be deterministic across clients (replays, lockstep)
- ties between otherwise equal encodings go to the AMD-friendly one; Intel big cores are rarely
  hurt by it

Other notes:
- aarch64 NEON has horizontal reduction instructions: fminv/fmaxv/sminv/smaxv (and integer addv)
  are single instructions; a float faddv exists only in SVE, so the vaddvq_f32 intrinsic compiles
  to an faddp pair. Both forms still beat the portable shuffle chains, which is why
  v_hmin/v_hmax/v_dot4/v_hadd4_x have NEON-specific implementations.
- Per-instruction costs drift between generations; calibrate on the last 2-3 (Intel Golden
  Cove/Raptor Cove P-cores 2021-2023, Arrow Lake; AMD Zen3/Zen4/Zen5), not on Skylake (2015):
  - FP add latency dropped from 4c (Skylake ran adds on the FMA units) to 3c on Golden Cove and
    ~2c on Zen5; FMA stays 4c
  - div/sqrt got faster every generation; on Zen3+/Alder Lake+ an occasional div or sqrt is rarely
    worth restructuring around, and _est pays off even less than on Skylake
  - Intel E-cores: Skymont (Arrow Lake, 2024) roughly doubles Gracemont's SIMD throughput, so the
    E-core penalty shrinks on the newest parts
  When a specific cost matters, check uops.info and AMD's per-family Software Optimization Guides
  (family 17h covers the console Zen2) instead of assuming 'modern x86'.
- ARM cores drift the same way: Cortex-A76 (2018) doubled big-core NEON throughput (two symmetric
  128-bit pipes), Cortex-X and Apple M run four; ARMv9 little cores (A510/A520) move past the
  A53/A55 64-bit-SIMD model, but SIMD width and sharing are configuration dependent - do not
  extrapolate A53/A55 costs to them; FP divide is much faster and no longer fully blocking from
  A76 on. Android fleets span all of these, so check the actual core, not 'ARM'.
- We target ARMv8/AArch64 only - a significant break from ARMv7, so ARM optimization articles from
  that era are often no longer valid: AArch64 has 32 x 128-bit registers (ARMv7 had 16),
  IEEE-compliant NEON FP (ARMv7 was flush-to-zero only), FMA, vector div/sqrt (ARMv7 NEON had only
  estimates) and horizontal reductions always available, and no VFP/NEON mode-switch or NEON-to-GPR
  pipeline stalls (crossings still cost a few cycles, see above)
- Full-precision div/sqrt is fast on big x86 cores, so _est variants mostly pay off on small
  cores, on ARM (where fdiv/fsqrt stay long-latency even on big cores), or when several are in
  flight; they are not an automatic win.

## Include Path
```cpp
#include <vecmath/dag_vecMath.h>        // full API
#include <vecmath/dag_vecMathDecl.h>    // types only (forward declarations)
```
