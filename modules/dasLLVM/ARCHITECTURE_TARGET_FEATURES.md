# dasLLVM architecture - target feature truth

Companion of `ARCHITECTURE.md` (contract: `../../ARCHITECTURE_COMMON.md`). Sections 4, 6 and 9 moved
here with their numbers - rules cite sections by number, and a number is never reused - and
section 12 continues the numbering, so the index reads in order. Each section is cited by the code
that embodies it. The CPU classes, the fat mode and tune under a class: `ARCHITECTURE_CPU_CLASSES.md`.

## 4. Host CPU feature truth on aarch64 {#aarch64-feature-truth}

An aarch64 host target reads its CPU features from two sources, because neither answers alone.
`LLVMGetHostCPUFeatures` returns an EMPTY string on macOS - there the CPU name is meant to imply
the features - and a part this LLVM cannot name maps to the generic CPU, where SDOT and SMMLA
have no instruction to select and codegen aborts. `cpu_supports` reads the operating system
instead (sysctl / `AT_HWCAP` / `IsProcessorFeaturePresent`), so it answers for silicon LLVM has
never heard of. The tier gates (`make_jit_target` - `JitTarget.features.dotprod`, `.i8mm`,
`.fullfp16`) and the target machine's feature string (`create_default_target_machine`) therefore
take the union of the two: an LLVM host-string hit OR a `cpu_supports` hit (fullfp16 additionally
reads darwin-arm64 as always-on - every Apple Silicon part has it). One asymmetry: the host rail's
machine string carries `+dotprod` unconditionally (every part the JIT has run on has it), while the
DotProd GATE probes like its siblings - on an ARMv8.0 host the gate declines and the `sdot4` family
compiles its fallback, whatever the string says. A cross-compile triple takes neither - only the force env - and so
does a generic-CPU standalone exe (one carrying no `[llvm_code]` kernel): its machine is the
ARMv8.0 baseline, which cannot select SDOT or SMMLA, so the DotProd and i8mm gates
(`arm64_dotprod`, `arm64_i8mm`) stay off there and every `aarch64_neon` call
that needs either compiles its daslang fallback body. The gates and the machine string are one
truth on both rails: a force-env feature raises the gate AND is appended to the generic machine.
The string reaches the code on three routes, one per engine: the DLL and exe rails build their
target machine from it (`create_default_target_machine`); the in-memory MCJIT - the engine every
run without a DLL cache ends on, a static daslang build always - is created through the C API
with no cpu and no feature string, so its subtarget is the generic one, and `stamp_default_target_attrs`
puts the default machine's cpu and string (the host's, or a baseline class's) on every defined function as `target-cpu` / `target-features`
attributes, which the per-function subtarget lookup honors (the route the fat-mode clones of
`ARCHITECTURE_CPU_CLASSES.md#fat-mode` already ride). A module the gates emitted SDOT into and the generic subtarget selects
aborts codegen with `Cannot select`; the attributes are what let the two agree.

The two ways a feature reaches the target machine's string license different things. A
detection-derived append - `+dotprod` always, `+i8mm` when `cpu_supports` confirms it - is
EXECUTION-safe: the silicon running this process really has the instruction. A
`DAS_JIT_ARM64_FORCE_FEATURES` append is EMISSION-only: it may name silicon this box does not
have, so the artifact is for another machine and executing it here traps.

## 6. The x64 kernel-matrix tier gates {#x64-tier-gates}

An x64 host target's `JitTarget.features` (a `CpuFeatures` set) carries the instruction tiers the
kernel matrix and the tune grids select on - a single bit where the tier is one feature, a
bit of `x64_tiers(f) : X64Tiers` where it combines several: `avx2`, `f16c`, `vnni256` (256-bit VPDPBUSD by either
VEX AVX-VNNI or EVEX AVX512-VNNI+VL), `avx512bw` (zmm byte ops - BW, not merely F), `avx512vnni`
(zmm VPDPBUSD; implies bw, the sign trick around it is BW), `avx512vbmi` (VPERMI2B / VPERMB /
VPMULTISHIFTQB - the grid formats' symbol lattice), `vnniint8` (VEX VPDPBSSD, native s8 x s8),
`amx` (both amx-tile and amx-int8; the per-process XTILEDATA grant is a separate runtime step
the family's own witness performs) and `amx_bf16` (amx-tile, amx-bf16 and avx512bf16 -
TDPBF16PS and the panel's VCVTNEPS2BF16, the bf16 tile family; `amx-fp16` is probed for the Granite Rapids fp16 tiles but gates nothing yet). `make_jit_target` decides `avx2` and `f16c` from cpuid
alone on a host target, and every other tier from cpuid truth OR'd with the `DAS_JIT_X64_FORCE_FEATURES`
emission-only override; a cross triple or a generic target
(`host_features = false`) drops to forced-only truth - cpuid is not consulted, the force env is
the only tier source there.

The cpuid truth is `das_cpu_supports` (`src/builtin/module_builtin_runtime.cpp`), a hand-kept table
keyed by the LLVM target-feature spelling - so the force env and `llc -mattr` take the same names.
A tier feature usually lands as three parts: its cpuid line there, its bit in
`CpuFeatures` (`daslib/llvm_cpu_class.das`, the profile fingerprint the `requires=` gates are
checked against), and - when the emitters branch on a combination - an `X64Tiers` bit. The cpuid line
is the load-bearing one: a name missing from the table answers false on every box, so the perm
that requires it declines everywhere and no error names the cause.

## 9. The idot family's target lowerings {#idot-lowerings}

The exact integer dots on the 8-bit lattice have three lowerings, picked by target: one
`@llvm.aarch64.neon.sdot` where the target has DotProd (`JitTarget.features.dotprod` - the host rail's
`+dotprod` append, or the force env on the generic rail), the SIMD128 form on a wasm target
(`idot_wasm_simd128`), generic widen-multiply IR everywhere else. The native arms exist because
neither backend produces them from the generic form: AArch64 expands it to zip/uzp/smull instead
of folding to SDOT, and the wasm backend runs it a fifth as fast. The wasm form is the ISA's two
halves of an int8 dot, `i16x8.extmul_{low,high}_i8x16_s` (what LLVM makes of `mul(sext, sext)`) and
`i32x4.extadd_pairwise_i16x8_s`; the pairwise sums land as byte pairs, and one even/odd shuffle-add
folds them into the quad lanes the generic form defines - exact for every int8 lane. The scalar
`idot` sums every lane, so its wasm form adds the two pair-sum vectors as they are and carries no
fold: only `idot4`, whose callers read the four lanes, pays for the shuffles. An unsigned
first operand widens with ZExt rather than SExt, so the unsigned-by-signed form takes this same
path instead of the 16-lane generic IR; the i16 product still holds every value that pair can make
(255 * -128 = -32640), so the fold stays exact there too. The relaxed-SIMD dot
(`i32x4.relaxed_dot_i8x16_i7x16_add_s`) is NOT what the generic op lowers to, and sec.12 keeps
the feature out of the string: its second operand is 7-bit, so the sign trick that
would feed it (`dot(w, x) == dot(sign(x)*w, |x|)`) wraps at -128 in either operand and answers the
wrong sign there - an operand a caller can bound to 7 bits is a different op, not this one.

## 12. The wasm target's feature string {#wasm-feature-string}

A wasm cross-compile gets `+simd128,+nontrapping-fptoint`, with `+atomics,+bulk-memory` appended
on the threaded rail (`wasm_target_features`), on the machine that emits the object. The IR pass pipeline of a wasm compile runs under the host architecture's generic machine instead (`create_default_target_machine`): no host cpu, so two build boxes of one architecture optimize alike, and not the wasm machine, whose cost model lowers the q8 speech kernels to slower code. The string carries no `+relaxed-simd`, and the
reason is reach, not codegen: WebKit implements none of the relaxed opcodes, so one of them
anywhere in a module makes Safari and every iOS browser refuse the whole module at validation,
and the browser builds ship to every engine. What the feature would buy is `f32x4.relaxed_madd`,
the only multiply-add wasm has; without it a contracted multiply-add splits back into `f32x4.mul`
plus `f32x4.add`. Measured on the Pocket TTS page in Chrome the split form runs at the same
real-time factor as the fused one, so nothing is paid for the reach. The feature would also move
more than the multiply-add: with it on, the backend lowers an `nnan` float min/max on a vector
to `f32x4.relaxed_min` / `_max`, whose NaN and signed-zero answers are the engine's choice, and
the relaxed dot stays ruled out for the idot family by sec.9 above whatever the string says.

## Lowerings for a target with no instruction behind the call {#wasm-lowerings}

Two calls have no wasm instruction behind their native lowering, and the vectorizer leaves a
loop that holds either one scalar. `llvm.round` - round half away from zero - becomes a
`roundf` library call a lane: wasm's `nearest` rounds half to even. `build_round`
(`daslib/llvm_jit_intrin.das`) emits `math::round` and `math::roundi` on a wasm target as the
truncation, moved one away from zero where the dropped fraction reaches a half. The fraction
`x - trunc(x)` is exact and so is the step, so the value is `llvm.round`'s for every input, a
negative zero included. Every native class rounds inline, so the form is the wasm target's alone.

`f16_cvt::f16_to_f32` lowers to a hardware convert where the CPU has one: every aarch64 (the
scalar `fcvt` is ARMv8.0 baseline), and x86 with F16C. `get_intrinsic` picks by
`target_has_half_converts`, and the choice is emission correctness: a half convert with no
instruction behind it becomes a compiler-rt call the JIT does not link. A target without - wasm,
the `x86-base` class - takes `intrinsic_f16_to_f32_soft` (the `g_f16_soft_intrin_lookup` table),
a form with no half type in the IR: a normal half's bits shifted into a float's fields and
scaled by 2^112, a subnormal half's mantissa through an exact int-to-float and a 2^-24 scale,
an all-ones exponent kept all ones, the sign put back. No float subnormal enters it, so it
holds in a host that flushes them to zero. Its bits are the daslang body's for all 65536 inputs,
and unlike that three-way-branching body it vectorizes. `f32_to_f16` keeps its daslang body.

User hints reach a wasm function as they reach any other: the `[hint]` attributes, the
`noalias` and `nocapture` parameter lists and the loop metadata a `[tune = N]` loop carries.
