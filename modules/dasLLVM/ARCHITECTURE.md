# dasLLVM Architecture

The design document `REVIEW.md` cites. Numbered sections are the stable reference targets;
usage and installation live in `README.md`, the debugger rail and its roadmap in `DEBUGGING.md`.

## 1. The jit backend pipeline

`run_jit` (`llvm_jit_run.das`) drives one linear pipeline per program: **hash** (content-address
the DLL - first per-function AOT hash plus the loop-hint/llvm-code folds), **init** (engine +
target machine), **declare** (LLVM function declarations for the jit set), **probe** (open the
cached DLL, compare per-function hashes), **irgen** (the das IR emitter over every function),
**optimize** (the LLVM pass pipeline at the requested level, plus the opt-in IR dump and the
post-opt verify), **emit+link** (artifact production - for the DLL path `write_dll` in
`llvm_jit_common.das`, which itself splits into **emit-obj**, machine-code emission, and
**link**, the lld-link spawn), **install** (resolve externs, instrument sim nodes), and
**finalize** (engine teardown / state install). On a cache hit irgen, optimize, and emit+link
are skipped and read as zero.

### 1.1 The timing contract

Every phase above reports its wall time in the `LLVM JIT time:` breakdown printed under
`options log_compile_time` / `policies.log_compile_time` - the same option the front end uses
for its `compiler took` line. The contract exists because an untimed phase is invisible exactly
when someone is hunting where compile time went: a phase outside the breakdown makes the
printed numbers sum short of the always-on total, and the gap has no name. Nested totals are
allowed - a parent entry (emit+link) may cover steps a callee reports separately (emit-obj,
link) - provided both levels print. When a phase is split into finer steps, each step reports
its own number; an aggregate label silently absorbing new sub-steps breaks the contract the
same way an untimed phase does.

Under `--jit-split-modules` (`run_split_codegen` in `llvm_jit_run.das`) the same labels map
differently: **declare** reads ~zero (declaration moves inside the partition loop), **irgen**
covers partitioning plus every partition's declare/irgen/ctor work, **optimize** carries the
`jit_par_emit_run` pool wall - per-job passes AND object emission interleave on the workers -
plus the post-optimize verifies and the partition teardown, and **emit+link** is the
`link_dll_from_objects` link plus the DLL reopen only. The finer steps print per the contract:
one `LLVM JIT time: job {obj} passes ... emit ...` line per partition (from the pool, under the
same log option) and the `link ... (N objects)` line under emit+link. With the obj cache on
(`--jit-obj-cache`, default under split), cached partitions skip declare/irgen/optimize
entirely: the key fold and probe cost lands in **irgen**, the `obj cache - K/N partitions
cached` line prints unconditionally beside the split announce, and per-partition
`obj cache hit` lines print under the log option.

### 1.2 The codegen tier

Code that EMITS machine code - the surface whose changes bump `LLVM_JIT_CODEGEN_VERSION` -
is IR generation, target-machine setup, the `[llvm_code]` generator bodies, and the jit call
ABI: the generated function signatures, name scheme, prologue, and the externs the install
phase binds. The file set that carries this surface is `EMITTER_FILES` in
`tests-cpp/small/test_jit_emitter_pin.cpp` (repo root); the pin test makes every text change to
one of those files visible (re-pin `LLVM_JIT_EMITTER_HASH`), and the bump is owed when the
emitted code for identical inputs can differ - a comment, a nolint, or a same-value rewrite
inside an emitter file re-pins without a bump.

`--jit-opt-level` (CLI, over `policies.jit_opt_level`, default 3) drives both the optimize
pipeline and the DLL path's codegen-side target machine. `write_exe` and AOT-object emission
(`emit_object_only`) deliberately stay at codegen level 3: shipped artifacts are not
content-addressed, so a tier change there has no cache guard to catch it. At level 0 the
injected tune-policy default becomes `fallback` (`jit_cli_opt_level()` in `llvm_tune.das`):
tune winners are raced under O3 codegen, so an O0 run cannot represent them and must not block
on the tuner to mint them.

## 2. Codegen identity - the DLL cache

Jit DLLs are content-addressed: `jit_dll_basename` folds the per-function AOT hashes,
`LLVM_JIT_CODEGEN_VERSION`, the opt/size levels, prologue and debug-info flags, and the target
triple - same inputs, same filename, cache hit. AST-level changes therefore self-invalidate
through the function hashes; **emitter-level changes do not** - a change that alters generated
machine code for identical inputs (IR generation, target-machine setup, `[llvm_code]`
generators, the jit ABI) is invisible to the key and silently serves stale code from cache
unless `LLVM_JIT_CODEGEN_VERSION` is bumped. Stamped `[llvm_code]` *arguments* are not
emitter-level: they fold into both cache keys per function (the hint folds), so a change that
merely re-selects which perm gets stamped - the `[tune]` machinery - self-invalidates with no
bump. "The jit call ABI" is the contract between the
generated code and the engine: the generated function signatures and name scheme
(`create_uid_nodes` / `get_dll_fn_name`), the prologue shape (`jit_emit_prologue`), the
`LlvmJitFlags`/`LlvmJitMode` inputs to the emitter, and the extern-resolution surface the
install phase binds (`ResolveExternVisitor`, `generate_llvm_code`, `instrument_jit`). A pinned
`jit_output_path` bypasses the content-addressed name entirely; its probe compares function
hashes only, which is why the summary line asserts the opt-level tag only when the tier is
actually known.

### 2.1 The split obj cache - positional invalidation

Under `--jit-split-modules`, each per-module partition object is content-addressed too
(`--jit-obj-cache`, on by default under split): its key is the running fold of every module
hash up to and including its own - symbol names, per-function AOT hashes, and the JIT-only
hint folds - combined with `jit_env_salt`, the ONE helper both the DLL key and the partition
keys fold their config/environment inputs through (a component folded into one key but not
the other would let a config change link stale objects). The chained prefix makes
invalidation **positional**: module order is topological, so a change in module j re-keys
every partition from j on, while everything before j links its cached `.o` - the probe is
bare file existence, before any per-partition LLVM state is created, so a hit skips
declaration, irgen, and the optimize/emit pool outright. Three consequences for consumers:
**require order is the cache layout** - a module you edit often belongs as late in the
require chain as its dependencies allow (registration-only requires, like the dasLLAMA GPU
tiers, belong at the END of an umbrella, not in a root module everything depends on); a
rename-without-body change still re-keys, because the module hash folds symbol names, not
just function hashes; and **the cache holds exactly one generation** - the GC keep-set is
the current link set, so reverting an edit is an eviction, not a hit: the run after a revert
re-emits from the reverted module on, same as the edit did.

## 3. Overrides and their announces

The backend's override knobs - the escapes that change what a run compiles, tunes, or emits
beyond its defaults - are: `DAS_TUNE_POLICY` (replaces the declared/injected tune policy),
`DAS_TUNE_MODE` (grid/tuner compile modes), `DAS_TUNE_MANIFEST` (pins the sidecar),
`DAS_TUNE_NOISE_CV` (recalibrates the tuner noise gate), `DAS_TUNE_NOISE_OVERRIDE` (mints
through a failing gate), `--tune` (forced re-mint), `--tune-only` / `DAS_TUNE_ONLY` (re-mints
only the named families; the policy guard arms it itself for a profile's residue),
`DAS_TUNE_CONTROL` (a supervisor's stop request - tuners abort between families), `--jit-obj-cache=0` (forces every split
partition to re-emit, bypassing the obj cache), `DAS_JIT_PROBE_LTO` (split partitions emit
bitcode and the link runs lld LTO - a dev probe artifact), `DAS_JIT_X64_FORCE_FEATURES` /
`DAS_JIT_ARM64_FORCE_FEATURES` (force CPU features past detection - emission, the cache keys,
and `cpu_supports`-based tune eligibility all follow), and the runtime escape API
`tune_suppress_mint(knob)` (a library `[init]` suppresses the auto/restart mint; the caller
passes the knob name it acts for). The announce contract: an override announces at the point
it CHANGES THE OUTCOME - at least one line naming the knob (its env spelling, or the
caller-supplied knob name for `tune_suppress_mint`); a set-but-inert override may stay silent,
and per-scope or per-site repeats are correct. A library that only exposes the override bit
(a `*_overridden` query such as `tune_noise_threshold_overridden`) discharges the contract
when the announce lands at the consumer in the same change. Verbosity knobs
(`DAS_TUNE_VERBOSITY`) shape only how much is printed, not what runs - they are not overrides
under this contract.

Environment knobs load ONCE, at context init, into the `[EnvConfig]` structs `g_env_jit` /
`g_env_tune` (`llvm_env.das`) - a mid-process `setenv` changes nothing the backend reads.
In-process overrides therefore go through the tune setters (`tune_set_verbosity`,
`tune_set_noise_cv`, ...), which also arm spawned children by exporting the matching variable.

## 4. Host CPU feature truth on aarch64 {#aarch64-feature-truth}

An aarch64 host target reads its CPU features from two sources, because neither answers alone.
`LLVMGetHostCPUFeatures` returns an EMPTY string on macOS - there the CPU name is meant to imply
the features - and a part this LLVM cannot name maps to the generic CPU, where SDOT and SMMLA
have no instruction to select and codegen aborts. `cpu_supports` reads the operating system
instead (sysctl / `AT_HWCAP` / `IsProcessorFeaturePresent`), so it answers for silicon LLVM has
never heard of. The tier gates (`init_jit_target_flags` - `g_target_arm64_dotprod`, `_i8mm`,
`_fullfp16`) and the target machine's feature string (`create_default_target_machine`) therefore
take the union of the two: an LLVM host-string hit OR a `cpu_supports` hit (fullfp16 additionally
reads darwin-arm64 as always-on - every Apple Silicon part has it). One asymmetry: the host rail's
machine string carries `+dotprod` unconditionally (every part the JIT has run on has it), while the
DotProd GATE probes like its siblings - on an ARMv8.0 host the gate declines and the `sdot4` family
compiles its fallback, whatever the string says. A cross-compile triple takes neither - only the force env - and so
does a generic-CPU standalone exe (one carrying no `[llvm_code]` kernel): its machine is the
ARMv8.0 baseline, which cannot select SDOT or SMMLA, so the DotProd and i8mm gates
(`g_target_arm64_dotprod`, `g_target_arm64_i8mm`) stay off there and every `aarch64_neon` call
that needs either compiles its daslang fallback body. The gates and the machine string are one
truth on both rails: a force-env feature raises the gate AND is appended to the generic machine.

The two ways a feature reaches the target machine's string license different things. A
detection-derived append - `+dotprod` always, `+i8mm` when `cpu_supports` confirms it - is
EXECUTION-safe: the silicon running this process really has the instruction. A
`DAS_JIT_ARM64_FORCE_FEATURES` append is EMISSION-only: it may name silicon this box does not
have, so the artifact is for another machine and executing it here traps.

## 5. The tune sidecar is a module-cache dependency {#tune-sidecar-cache-pin}

`[tuned]` and `[tune_policy]` stamp a function's hints at macro time out of the tune sidecar,
and the module cache stores the stamped AST. A re-mint therefore has to invalidate the cached
record, or a later run serves stamps minted against the old sidecar until some source file
changes. The stamping paths (`tune_apply`, `tune_kernel_pick`) register the sidecar path with
`add_module_cache_dependency` through `pin_module_cache_dependency` before they read it; the
record carries the path with the file's byte size and content hash, and the reader re-validates
both before it trusts the payload. Content, not mtime: an app that rewrites its sidecar
byte-identically on exit must not churn the cache.

The registration runs before the staleness gate, and for a path that does not exist yet,
because the mints that matter most produce no successful read - the first mint has no sidecar,
and a re-mint replaces one the gate rejected. An absent file registers as size -1 and hash 0,
which the next run's re-validation sees change. The pin sits beside the read, not inside
`read_manifest`: the runtime shares that reader (the box-profile pin at load, `tune_status`),
and a standalone exe binds every extern its functions name at startup, so a reader carrying the
`ast_core` extern would drag the compiler module into every exe - and a wasm cross-link, which
sees only the compiler-free runtime archive, has nothing to bind it to.

The shipped defaults profiles are the same kind of input: with no sidecar entry a kernel
stamps its class entry out of `<defaults>/<class>.tune-defaults.json`, so `pin_profile_chain`
registers every candidate on the class ladder it tries, existing or not - a profile that
appears, or is re-exported after a re-mint, must invalidate the stamps minted without it. The
staleness gate itself compares the sidecar's mtime with the running binary's, which no content
hash sees; the host closes that hole by keying its default module cache on the binary's
mtime and size, so a rebuild is a fresh cache rather than a hit on pre-rebuild stamps.

## 6. The x64 kernel-matrix tier gates {#x64-tier-gates}

An x64 host target publishes eight boolean gates (`g_target_x64_*`), one per instruction tier the
kernel matrix and the tune grids select on: `avx2`, `f16c`, `vnni256` (256-bit VPDPBUSD by either
VEX AVX-VNNI or EVEX AVX512-VNNI+VL), `avx512bw` (zmm byte ops - BW, not merely F), `avx512vnni`
(zmm VPDPBUSD; implies bw, the sign trick around it is BW), `avx512vbmi` (VPERMI2B / VPERMB /
VPMULTISHIFTQB - the grid formats' symbol lattice), `vnniint8` (VEX VPDPBSSD, native s8 x s8),
and `amx` (both amx-tile and amx-int8; the per-process XTILEDATA grant is a separate runtime step
the family's own witness performs). `init_jit_target_flags` decides each from cpuid truth OR'd with
the `DAS_JIT_X64_FORCE_FEATURES` emission-only override; a cross triple or a generic target
(`host_features = false`) drops to forced-only truth - cpuid is not consulted, the force env is
the only tier source there.

The cpuid truth is `das_cpu_supports` (`src/builtin/module_builtin_runtime.cpp`), a hand-kept table
keyed by the LLVM target-feature spelling - so the force env and `llc -mattr` take the same names.
A tier feature usually lands as three parts: its cpuid line there, its name in
`TUNE_KNOWN_FEATURES` (`daslib/llvm_tune.das`, the profile fingerprint the `requires=` gates are
checked against), and - when the emitters branch on it - a `g_target_x64_*` gate. The cpuid line
is the load-bearing one: a name missing from the table answers false on every box, so the perm
that requires it declines everywhere and no error names the cause.

## 7. A constant-folded GEP is not an instruction {#gep-constant-fold}

`LLVMBuildGEP2` over a global with a constant index does not create an instruction - LLVM folds
it into a `ConstantExpr`, one shared object per distinct expression in the context. Any API that
casts a "just built" GEP to `GetElementPtrInst` therefore writes through the wrong type into the
constant's memory when the fold happened - `LLVMSetIsInBounds` was the instance that corrupted
the context (heap damage surfacing in `LLVMContextDispose` at teardown). The in-bounds form is
requested at build time (`LLVMBuildInBoundsGEP2`), which folds to an in-bounds `ConstantExpr`
correctly; the `llvm_boost` wrapper's `inbounds` default rides that builder.

## 8. The inline-polynomial rail {#vector-poly-rail}

`math::exp`, `sin`, `cos`, `tan`, `exp2`, `log2`, `log`, `pow` and the three hyperbolics on a
float VECTOR type are emitted as inline IR by the `build_vector_*` emitters in
`llvm_jit_intrin.das`, all behind one gate (`vmath_poly_gate`). The default lowering is
`@llvm.<op>.vNf32`, which scalarizes to N libm calls on any target without a vector libm -
Darwin-ARM and most others. Each emitter replaces that call with the SAME polynomial the
interpreter and AOT already run (vecmath, `include/vecmath/`), written as generic vector IR
(`fmuladd`, `trunc`, `roundeven`, `fptosi.sat`, integer masks and selects) that the backend lowers
to one NEON instruction apiece - so the three rails agree instead of merely being close; the
one family that cannot mirror vecmath is sec.8.3. sin and cos mirror `v_sincos` (quadrant =
round(x*2/pi), the two-constant Cody-Waite reduction, a degree-3-in-x^2 pair); tan mirrors
`v_tan` (4/pi octants, three reduction constants, its own minimax - not sin over cos); exp2,
log2, log and pow mirror `v_exp2`, `v_log2_est_p5`, `v_log` and `v_pow`. The gate is aarch64 by
measurement, not by portability: on x64 the always-computed guard branches of these kernels
cost more than they save. Scalar float and double keep the libm intrinsic on every target -
libm is correctly rounded and one scalar call carries no scalarization penalty. exp keeps the
ggml polynomial it had and differs from the interpreter by 3.9e-6 relative; routing it through
the exp2 emitter is the one change that would make it exact.

### 8.1 Fusion is part of the polynomial {#vector-poly-fusion}

A Horner step vecmath writes as `v_add(v_mul(..))` is emitted unfused - `vmath_poly_step`, an
fmul then an fadd - and only the chains vecmath writes as `v_madd` / `v_nmsub` go through
`vmath_fma`. The exp2 and log2 chains alternate sign heavily enough that one contracted step
moves the result by several ulp, so emitting `@llvm.fmuladd` for them costs bit-exact agreement
with the interpreter and AOT: measured over 200k lanes, unfused is identical and fused is up to
1.9e-6 apart. Where vecmath does fuse, fusion is load-bearing rather than optional - the sincos
Cody-Waite reduction rounds `x - qf*KC1` into noise at |x| ~ 1e5 without it.

The interpreter side of that bit-exactness is a precondition the emitters cannot enforce: it holds
while the host compiler does not contract vecmath's POLY macros itself. clang's default
`-ffp-contract=on` contracts only inside one source expression, so the inlined `v_add(v_mul(..))`
pair stays two instructions; GCC's default `fast` contracts across statements and would fuse them.
CMake pins neither flag, so on a GCC-built interpreter it is the vecmath rail that moves, not the
emitted one.

NaN carries lane for lane on this rail, and that has to be built in: a clamp or a float-to-int
conversion written with ordered compares replaces a NaN lane with a number, and no accuracy bound
can see the substitution because a bound only reads lanes that produced a number. So `tanh` selects
its operand back over its `[-9,9]` clamp through `fcmp uno`, and the sincos quadrant and the tan
octant convert through `llvm.fptosi.sat` rather than `fptosi`, whose result for NaN and for
out-of-range input is poison. `exp` is the one member of the rail that still diverges on NaN: the
JIT answers NaN for `exp(NaN)` where the interpreter answers inf.

### 8.2 log2 is an estimate, and its specials are not IEEE {#vector-log2-estimate}

`build_vector_log2` mirrors vecmath's `v_log2_est_p5` (`dag_vecMath_common.h`): the exponent
field gives the integer part, the mantissa is forced into [1,2) and fed to a degree-5 minimax,
and the `p*(m-1)` shape is what makes log2(1) exactly 0. It is an ESTIMATE, materially looser
than libm - about 3.7e-5 relative at its worst, just below x == 1 where that combine cancels -
and the interpreter and AOT have always used it, so the JIT matching it is the point: the
tiers agreeing is the property that wins, and a better log2 goes into vecmath for every tier,
never into one rail. `math::log` is this estimate scaled by ln2 and `math::pow` is
`exp2(log2_est(x) * y)`, so both inherit the error, pow amplified by |y|. The estimate reads
the exponent through a mask and therefore never produces -inf or NaN: log2(0) is -127 and
log2(-x) == log2(|x|). The dropped sign travels into `pow` with the estimate - the base reaches the
exponent as `log2_est(|x|)` - so `pow(-2, 3)` is +8, not libm's -8, and it is +8 on every tier.
`tests/llvm_vector_math.das` pins the bounds and the special values.

### 8.3 The hyperbolics have no interpreter twin {#vector-hyperbolic-divergence}

vecmath carries no vector sinh/cosh/tanh, so `SimPolicy` binds `vsinh`/`vcosh`/`vtanh`
(`aot_builtin_math.h`), which call libm once per lane. `build_vector_hyper` builds all three on
`build_vector_expf` instead, so on this one family the JIT diverges from interp and AOT by the
exp polynomial's error rather than agreeing with them - the opposite trade from every other
emitter on the rail, taken because the consumer (GELU over float4 rows) otherwise pays four
libm calls per vector. `tests/llvm_vector_math.das` asserts the size of that divergence.

## 9. The idot family's target lowerings {#idot-lowerings}

The exact integer dots on the 8-bit lattice have three lowerings, picked by target: one
`@llvm.aarch64.neon.sdot` where the target has DotProd (`g_target_arm64_dotprod` - the host rail's
`+dotprod` append, or the force env on the generic rail), the SIMD128 form on a wasm target
(`idot_wasm_simd128`), generic widen-multiply IR everywhere else. The native arms exist because
neither backend produces them from the generic form: AArch64 expands it to zip/uzp/smull instead
of folding to SDOT, and the wasm backend runs it a fifth as fast. The wasm form is the ISA's two
halves of an int8 dot, `i16x8.extmul_{low,high}_i8x16_s` (what LLVM makes of `mul(sext, sext)`) and
`i32x4.extadd_pairwise_i16x8_s`; the pairwise sums land as byte pairs, and one even/odd shuffle-add
folds them into the quad lanes the generic form defines - exact for every int8 lane. The
relaxed-SIMD dot (`i32x4.relaxed_dot_i8x16_i7x16_add_s`) is NOT used: its second operand is 7-bit,
so the sign trick that would feed it (`dot(w, x) == dot(sign(x)*w, |x|)`) wraps at -128 in either
operand and answers the wrong sign there, and `+relaxed-simd` is a whole-module switch that also
turns float-vector `min`/`max` and `mad` into engine-defined instructions (NaN and signed-zero
answers, fusion) - the feature string stays `+simd128,+nontrapping-fptoint`, the runtime archive's.
