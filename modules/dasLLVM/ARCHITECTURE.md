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
**link**, the lld-link spawn; for `-lib` `write_lib`, which links a shared library or archives a
static one and writes the C header beside it), **install** (resolve externs, instrument sim nodes), and
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

The per-artifact entry emitters are OUTSIDE that surface: `llvm_exe.das` (a standalone exe's
`main`) and `llvm_lib.das` (a `-lib`'s C entry points and thunks) emit startup glue for artifacts
nothing content-addresses, so neither is in `EMITTER_FILES` and neither owes a version bump. Both
artifacts run in `LlvmJitMode.EXE`, which is what makes the exe startup shareable: the four
`emit_standalone_*` helpers in `llvm_exe.das` are the shared halves, split so a library can put the
process-global half behind a once guard and the per-context half behind its own catch boundary.

### 1.3 A library's runtime is process-global, its environment is per-thread {#lib-runtime-scope}

An exe owns its process: one thread runs `main`, registers the modules, and drains them on the way
out. A library owns none of that, and three consequences shape `llvm_lib.das`.

**The environment is thread-local** (`daScriptEnvironment::bound` / `owned`,
`include/daScript/ast/ast.h`), but the module registration behind it happens once. So
`jit_lib_run_once` does both jobs: the first caller registers, and every later caller - on any
thread - is BOUND to that first registration's environment. Without the binding a second thread's
`<P>_create` dereferences a null `getBound()->modules` inside `jit_init_extern_function`. The
create-failure message (`g_jitLibCreateError`) is process-global for the same reason read from the
other side: a C host reads it through `<P>_last_error(nullptr)`, and a per-thread copy would answer
null on every thread but the one whose create failed.

**Only one daslang runtime fits in a process**, because `jit_register_Module_*`
(`REGISTER_MODULE_IN_NAMESPACE`) carries no already-created guard. A library therefore checks for
one before registering and DECLINES - `<P>_create` returns null with the reason in
`<P>_last_error(nullptr)` - rather than letting the abort reach a C caller that was promised a
return value. This covers a second `-lib` artifact and a host that registered the modules itself.

**Nothing calls the shutdown functions for a library.** A jitted `SimFunction` gets a zeroed
`FuncInfo` (`jit_lib`'s `registerJitFunction`, `src/builtin/module_jit.cpp`), so
`Context::runShutdownScript` - which selects on `FuncInfo::flag_shutdown`, set only by the
interpreter's debug-info builder - finds none. `<P>_destroy` therefore emits the program's
`[finalize]` / `[shutdown]` calls itself, the same way `<P>_create` emits its `[init]` calls. The
process-level drain is an `atexit` hook armed beside the registration it balances, and it rebinds
the recorded environment first so the draining thread need not be the creating one.

`--jit-opt-level` (CLI, over `policies.jit_opt_level`, default 3) drives both the optimize
pipeline and the DLL path's codegen-side target machine. `write_exe`, `write_lib` and AOT-object
emission (`emit_object_only`) deliberately stay at codegen level 3: shipped artifacts are not
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
never heard of. Both the tier gates (`init_jit_target_flags`) and the target machine's feature
string (`create_default_target_machine`) therefore take the union of the two: an LLVM host-string
hit OR a `cpu_supports` hit (fullfp16 additionally reads darwin-arm64 as always-on - every
Apple Silicon part has it). A cross-compile triple takes neither - only the force env.

The two ways a feature reaches the target machine's string license different things. A
detection-derived append - `+dotprod` always, `+i8mm` when `cpu_supports` confirms it - is
EXECUTION-safe: the silicon running this process really has the instruction. A
`DAS_JIT_ARM64_FORCE_FEATURES` append is EMISSION-only: it may name silicon this box does not
have, so the artifact is for another machine and executing it here traps.

## 5. The tune sidecar is a module-cache dependency {#tune-sidecar-cache-pin}

`[tuned]` and `[tune_policy]` stamp a function's hints at macro time out of the tune sidecar,
and the module cache stores the stamped AST. A re-mint therefore has to invalidate the cached
record, or a later run serves stamps minted against the old sidecar until some source file
changes. `read_manifest` (`daslib/llvm_tune.das`) registers the sidecar path with
`add_module_cache_dependency` on every read; the record carries the path with the file's byte
size and content hash, and the reader re-validates both before it trusts the payload. Content,
not mtime: an app that rewrites its sidecar byte-identically on exit must not churn the cache.

The registration runs before the staleness gate, and for a path that does not exist yet,
because the mints that matter most produce no successful read - the first mint has no sidecar,
and a re-mint replaces one the gate rejected. An absent file registers as size -1 and hash 0,
which the next run's re-validation sees change. Registering is a no-op outside compilation, so
the manifest's runtime readers reach the same call unconditionally.

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
