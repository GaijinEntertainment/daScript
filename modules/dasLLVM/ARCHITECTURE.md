# dasLLVM Architecture

The design document `REVIEW.md` cites. A section's `{#anchor}` is its stable reference target;
usage and installation live in `README.md`, the debugger rail and its roadmap in `DEBUGGING.md`.
Companions: `ARCHITECTURE_TARGET_FEATURES.md` (CPU feature truth, the tier gates, the CPU classes),
`ARCHITECTURE_DEBUG_INFO.md` (the `--jit-debug` DWARF rail - sec.12), `ARCHITECTURE_JIT_ENTRY.md`
(the entry module, the emitter-free cache hit, the candidate-set key), `ARCHITECTURE_EXE.md` (the
standalone exe's link decision and startup - sec.10), `ARCHITECTURE_FAST_MATH.md`
(`[never_fast_math]`), `ARCHITECTURE_CODEGEN_IDENTITY.md` (the DLL cache key and the split obj
cache - sec.2) and `ARCHITECTURE_VECTOR_MATH.md` (the inline-polynomial vector math rail - sec.8).

## 1. The jit backend pipeline

`run_jit_linked` (`llvm_jit_link.das`) takes a DLL cache hit by itself - **hash**, **probe**,
**install**, **finalize** - and hands every other run to `run_jit` (`llvm_jit_run.das`) in the
emitter's macro context (`ARCHITECTURE_JIT_ENTRY.md`). `run_jit` drives one linear pipeline per
program: **hash** (the DLL key over the candidate set: per-function AOT hashes plus the hint
folds), **init** (engine + target machine), **declare** (LLVM function declarations for the jit
set), **probe** (open the cached DLL, compare per-function hashes), **irgen** (the das IR emitter
over every function), **optimize** (the LLVM pass pipeline at the requested level, plus the
opt-in IR dump and the post-opt verify), **emit+link** (artifact production - `write_artifact` in
`llvm_jit_common.das`, itself **emit-obj**, machine-code emission, then **link**; which artifact
is the caller's choice, a JIT DLL or an exe or, for `-lib`, a shared library or static archive
with the C header written beside it), **install** (resolve externs, instrument sim nodes), and
**finalize** (engine teardown / state install). On a hit irgen, optimize and emit+link read as zero.

### 1.1 The timing contract {#jit-timing-contract}

Every phase above reports its wall time in the `LLVM JIT time:` breakdown printed under
`options log_compile_time` / `policies.log_compile_time` - the same option the front end uses
for its `compiler took` line. The contract exists because an untimed phase is invisible exactly
when someone is hunting where compile time went: a phase outside the breakdown makes the printed
numbers sum short of the always-on total, and the gap has no name. Nested totals are allowed - a
parent entry (emit+link) may cover steps a callee reports separately (emit-obj, link) - provided both
levels print. When a phase is split into finer steps, each step reports its own number; an aggregate
label silently absorbing new sub-steps breaks the contract the same way an untimed phase does.

Under `--jit-split-modules` (`run_split_codegen` in `llvm_jit_run.das`) the same labels map
differently: **declare** reads ~zero (declaration moves inside the partition loop), **irgen**
covers partitioning plus every partition's declare/irgen/ctor work, **optimize** carries the
`jit_par_emit_run` pool wall - per-job passes AND object emission interleave on the workers -
plus the post-optimize verifies and the partition teardown, and **emit+link** is the
`link_from_objects` link plus the DLL reopen only. The finer steps print per the contract:
one `LLVM JIT time: job {obj} passes ... emit ...` line per partition (from the pool, under the
same log option) and the `link ... (N objects)` line under emit+link. With the obj cache on
(`--jit-obj-cache`, default under split), cached partitions skip declare/irgen/optimize
entirely: the key fold and probe cost lands in **irgen**, the `obj cache - K/N partitions
cached` line prints unconditionally beside the split announce, and per-partition
`obj cache hit` lines print under the log option.

An `-exe` takes the same driver when its build passes `--jit-split-modules` (its default stays
one unit): the entry rides a partition of its own (`exe_main`), the partitions link once with no
obj cache, and the objects and the response file go with the link. Under `--jit-lto` the
partitions leave as bitcode, every function stamped with the cpu and features the target machine
would carry (`default_target_cpu_features` - lld's LTO backend builds its subtarget from the
module, not from a machine), and the link runs LTO (`lto_linker_args`: `/opt:lldlto=3` on
lld-link, `-flto` on a POSIX driver, where the plan links through `clang++` unless
`--jit-path-to-linker` names another - a GNU driver handles no bitcode). `DAS_JIT_PROBE_LTO` is
the DLL path's dev twin of that rail; the slots the partitions own and the LTO link's needs are `ARCHITECTURE_EXE.md` sec.3 and 4.

### 1.2 The codegen tier {#codegen-tier}

Code that EMITS machine code - the surface whose changes bump `LLVM_JIT_CODEGEN_VERSION` -
is IR generation, target-machine setup, the `[llvm_code]` generator bodies, and the jit call
ABI: the generated function signatures, name scheme, prologue, and the externs the install
phase binds. The file set that carries this surface is `EMITTER_FILES` in
`tests-cpp/small/test_jit_emitter_pin.cpp` (repo root); the pin test makes every text change to one
of those files visible (re-pin `LLVM_JIT_EMITTER_HASH`), and the bump is owed when the emitted code for
identical inputs can differ - a comment, a nolint, or a same-value rewrite inside an emitter file re-pins without a bump.

The per-artifact entry emitters are OUTSIDE that surface: `llvm_exe.das` (a standalone exe's
`main`) and its `-lib` half (the C entry points and thunks) emit startup glue for artifacts
nothing content-addresses, so neither is in `EMITTER_FILES` and neither owes a version bump. Both
artifacts run in `LlvmJitMode.EXE`, which is what makes the exe startup shareable: the four
`emit_standalone_*` helpers in `llvm_exe.das` are the shared halves, split so a library can put the
process-global half behind a once guard and the per-context half behind its own catch boundary.

### 1.3 A library's runtime

`daslang -lib` emits an artifact that loads into a process it does not own; its runtime,
environment and shutdown rules are `ARCHITECTURE_LIB.md` sec. 1.3.

### 1.4 A constant make-array literal is constant data {#const-data-literal}

A make-array literal of four elements or more (`MIN_CONST_DATA_ELEMS`) whose elements are all numeric or
vector constants (`is_const_data_literal`: the scalar and two- or four-wide vector element types; a
three-wide vector's padding, a string's runtime pointer and a fixed-array element - told apart by its size -
stay on the store path) lowers to a private constant array and one `memcpy`, not a store per element; an
element the builder did not fold to a constant of the element type sends the whole literal to a store loop over the collected values. A SPIR-V kernel blob is a thousand `uint4`, and the per-element form put a
620k-line initializer in front of the optimizer and the backend: most of the one-unit exe's codegen wall.

## 2. Codegen identity - the DLL cache

Moved to `ARCHITECTURE_CODEGEN_IDENTITY.md`: sec.2 (the content-addressed DLL key and the jit call
ABI) and sec.2.1 (the split obj cache's positional invalidation).

## 3. Overrides and their announces {#override-knobs}

The backend's override knobs - the escapes that change what a run compiles, tunes, or emits
beyond its defaults - are: `DAS_TUNE_POLICY` (replaces the declared/injected tune policy),
`DAS_TUNE_MODE` (grid/tuner compile modes, and `fat` - one clone per shipped CPU class in a
standalone exe; `ARCHITECTURE_TARGET_FEATURES.md` sec.11), `DAS_TUNE_FAT_CLASS` (pins the class a
fat exe runs at startup), `DAS_TUNE_MANIFEST` (pins the sidecar),
`DAS_TUNE_NOISE_CV` (recalibrates the tuner noise gate), `DAS_TUNE_NOISE_OVERRIDE` (mints
through a failing gate), `--tune` (forced re-mint), `--tune-only` / `DAS_TUNE_ONLY` (re-mints
only the named families; the policy guard arms it itself for a profile's residue),
`DAS_TUNE_CONTROL` (a supervisor's stop request - tuners abort between families), `--jit-obj-cache=0` (forces every split
partition to re-emit, bypassing the obj cache), `--jit-lto` (a split `-exe`'s partitions emit
bitcode and its link runs LTO), `DAS_JIT_PROBE_LTO` (the DLL path's twin: split partitions emit
bitcode and the link runs lld LTO - a dev probe artifact), `DAS_JIT_X64_FORCE_FEATURES` /
`DAS_JIT_ARM64_FORCE_FEATURES` (force CPU features past detection - emission, the cache keys,
and `cpu_supports`-based tune eligibility all follow), `DAS_JIT_BASELINE` (build for a CPU class
instead of the box - the machine, the gates, the tune ladder and the cache keys all follow;
`ARCHITECTURE_TARGET_FEATURES.md` sec.10), `--jit-debug` / `-g` (emit DWARF or CodeView debug
info and promote every argument to a stack slot for it; `ARCHITECTURE_DEBUG_INFO.md` sec.12), `--jit-compile-only` / `options jit_compile_only` (build, verify and optimize, then write, load and install nothing - the program runs interpreted; under a `--jit-target` cross triple it also keeps the `[tune]` grid a cross artifact would stamp as reference bodies, which is what the emission dump inspects), `--jit-sanitize` / `options jit_sanitize` (`address` runs LLVM's `asan` pass after the optimizer on functions marked `sanitize_address`; `undefined` runs `bounds-checking`; a sanitizer host defaults to its own, `none` to nothing; an exe or a library links with `-fsanitize=` for the host's sanitizers and the requested ones, through `clang++` unless a linker is named; an ASan host lowers every array push to the runtime call, which keeps the buffer's container annotation - `src/misc/ARCHITECTURE.md` sec.10), and the
runtime escape API `tune_suppress_mint(knob)` (a library `[init]` suppresses the auto/restart mint; the
caller passes the knob name it acts for). The announce contract: an override announces at the point it
CHANGES THE OUTCOME - at least one line naming the knob (its env spelling, or the caller-supplied knob
name for `tune_suppress_mint`); a set-but-inert override may stay silent, and per-scope or per-site
repeats are correct. A library that only exposes the override bit (a `*_overridden` query such as
`tune_noise_threshold_overridden`) discharges the contract when the announce lands at the consumer in
the same change. Verbosity knobs (`DAS_TUNE_VERBOSITY`) shape only how much is printed, not what runs -
they are not overrides under this contract.

Environment knobs load ONCE, at context init, into the `[EnvConfig]` structs `g_env_jit` /
`g_env_tune` (`llvm_env.das`) - a mid-process `setenv` changes nothing the backend reads.
In-process overrides therefore go through the tune setters (`tune_set_verbosity`,
`tune_set_noise_cv`, ...), which also arm spawned children by exporting the matching variable.

## 4. Host CPU feature truth on aarch64

Moved to `ARCHITECTURE_TARGET_FEATURES.md` sec.4, with sec.6 (the x64 tier gates) and sec.10
(CPU classes and `DAS_JIT_BASELINE`).

## 5. The tune sidecar is a module-cache dependency {#tune-sidecar-cache-pin}

`[tuned]` and `[tune_policy]` stamp a function's hints at macro time out of the tune sidecar,
and the module cache stores the stamped AST. A re-mint therefore has to invalidate the cached
record, or a later run serves stamps minted against the old sidecar until some source file
changes. The stamping paths (`tune_apply`, `tune_kernel_pick`) register the sidecar path with
`add_module_cache_dependency` through `pin_module_cache_dependency` before they read it; the
record carries the path with the file's byte size and content hash, and the reader re-validates both
before it trusts the payload. Content, not mtime: an app that rewrites its sidecar byte-identically on exit must not churn the cache.

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

## 6. The x64 kernel-matrix tier gates

Moved to `ARCHITECTURE_TARGET_FEATURES.md` sec.6.

## 7. A constant-folded GEP is not an instruction {#gep-constant-fold}

`LLVMBuildGEP2` over a global with a constant index does not create an instruction - LLVM folds
it into a `ConstantExpr`, one shared object per distinct expression in the context. Any API that
casts a "just built" GEP to `GetElementPtrInst` therefore writes through the wrong type into the
constant's memory when the fold happened - `LLVMSetIsInBounds` was the instance that corrupted
the context (heap damage surfacing in `LLVMContextDispose` at teardown). The in-bounds form is
requested at build time (`LLVMBuildInBoundsGEP2`), which folds to an in-bounds `ConstantExpr`
correctly; the `llvm_boost` wrapper's `inbounds` default rides that builder.

## 8. The inline-polynomial rail

Moved to `ARCHITECTURE_VECTOR_MATH.md`: sec.8 (the rail) and sec.8.1-8.3 (fusion, the log2 estimate,
the hyperbolics' divergence).

## 9. The idot family's target lowerings

Moved to `ARCHITECTURE_TARGET_FEATURES.md`: sec.9 (the three lowerings, and why the relaxed-SIMD
dot is not one of them), which is where the target's feature string it reads already lives.

## 10. The standalone exe

Moved to `ARCHITECTURE_EXE.md`: sec.1 (the require-resolver rows) and sec.2 (a global
initializer's addresses are filled at startup).

## 11. A global's address is a memory(none) lookup at its use site

JIT code reaches a das global through `jit_get_global_mnh(mnh, ctx)` (`jit_get_shared_mnh` for a
shared one): the mangled-name hash is static, and the offset it names is added to the context's
globals base, which is what a cross-context call needs - the same JIT function runs on any context
of the program, each with its own base, so the address cannot be baked. The emitter
(`visitExprVar`, `daslib/llvm_jit.das`) emits the call where the variable is used and declares it
`memory(none)` (`daslib/llvm_jit_common.das`): the result depends on nothing but its arguments for
the lifetime of the context, so LLVM CSEs one use against a dominating one, hoists a loop's lookup
into the preheader, and - the call not being `speculatable` - never moves it ahead of a branch, so
a global written on one branch is looked up on that branch only. Every access to a global in a
function therefore shares one base pointer, which is what lets LLVM see `xs[j]` and `xs[j + 1]` as
adjacent. Under `options solid_context` the address is instead `context->globals + stackTop`,
computed once per function in the entry block.

## 12. A macro-time line prints once per module-cache record {#macro-line-cache-replay}

A compile that hits a front-end module-cache record replays the cached AST and runs no macro
again. A line a macro prints at compile time - the `llvm_tune:` apply lines, a `[tune]`-family
compile error - therefore prints on the run that wrote the record and on no later run, and an
assertion that such a line is absent passes on a hit whatever the macro would do. A line printed
after the front end - by the backend or by the runtime guard, such as the `LLVM JIT:` announce,
the covered-box announce or `re-tuning (--tune)` - prints on every run, hit or miss.
