# dasLLVM Architecture

The design document `REVIEW.md` cites. Numbered sections are the stable reference targets;
usage and installation live in `README.md`, the debugger rail and its roadmap in `DEBUGGING.md`.

## 1. The jit backend pipeline

`run_jit` (`llvm_jit_run.das`) drives one linear pipeline per program: **hash** (content-address
the DLL — first per-function AOT hash plus the loop-hint/llvm-code folds), **init** (engine +
target machine), **declare** (LLVM function declarations for the jit set), **probe** (open the
cached DLL, compare per-function hashes), **irgen** (the das IR emitter over every function),
**optimize** (the LLVM pass pipeline at the requested level, plus the opt-in IR dump and the
post-opt verify), **emit+link** (artifact production — for the DLL path `write_dll` in
`llvm_jit_common.das`, which itself splits into **emit-obj**, machine-code emission, and
**link**, the lld-link spawn), **install** (resolve externs, instrument sim nodes), and
**finalize** (engine teardown / state install). On a cache hit irgen, optimize, and emit+link
are skipped and read as zero.

### 1.1 The timing contract

Every phase above reports its wall time in the `LLVM JIT time:` breakdown printed under
`options log_compile_time` / `policies.log_compile_time` — the same option the front end uses
for its `compiler took` line. The contract exists because an untimed phase is invisible exactly
when someone is hunting where compile time went: a phase outside the breakdown makes the
printed numbers sum short of the always-on total, and the gap has no name. Nested totals are
allowed — a parent entry (emit+link) may cover steps a callee reports separately (emit-obj,
link) — provided both levels print. When a phase is split into finer steps, each step reports
its own number; an aggregate label silently absorbing new sub-steps breaks the contract the
same way an untimed phase does.

Under `--jit-split-modules` (`run_split_codegen` in `llvm_jit_run.das`) the same labels map
differently: **declare** reads ~zero (declaration moves inside the partition loop), **irgen**
covers partitioning plus every partition's declare/irgen/ctor work, **optimize** carries the
`jit_par_emit_run` pool wall — per-job passes AND object emission interleave on the workers —
plus the post-optimize verifies and the partition teardown, and **emit+link** is the
`link_dll_from_objects` link plus the DLL reopen only. The finer steps print per the contract:
one `LLVM JIT time: job {obj} passes … emit …` line per partition (from the pool, under the
same log option) and the `link … (N objects)` line under emit+link. With the obj cache on
(`--jit-obj-cache`, default under split), cached partitions skip declare/irgen/optimize
entirely: the key fold and probe cost lands in **irgen**, the `obj cache - K/N partitions
cached` line prints unconditionally beside the split announce, and per-partition
`obj cache hit` lines print under the log option.

### 1.2 The codegen tier

Code that EMITS machine code — the surface whose changes bump `LLVM_JIT_CODEGEN_VERSION` —
is IR generation, target-machine setup, the `[llvm_code]` generator bodies, and the jit call
ABI: the generated function signatures, name scheme, prologue, and the externs the install
phase binds. The authoritative file set is `EMITTER_FILES` in
`tests-cpp/small/test_jit_emitter_pin.cpp` (repo root).

`--jit-opt-level` (CLI, over `policies.jit_opt_level`, default 3) drives both the optimize
pipeline and the DLL path's codegen-side target machine. `write_exe` and AOT-object emission
(`emit_object_only`) deliberately stay at codegen level 3: shipped artifacts are not
content-addressed, so a tier change there has no cache guard to catch it. At level 0 the
injected tune-policy default becomes `fallback` (`jit_cli_opt_level()` in `llvm_tune.das`):
tune winners are raced under O3 codegen, so an O0 run cannot represent them and must not block
on the tuner to mint them.

## 2. Codegen identity — the DLL cache

Jit DLLs are content-addressed: `jit_dll_basename` folds the per-function AOT hashes,
`LLVM_JIT_CODEGEN_VERSION`, the opt/size levels, prologue and debug-info flags, and the target
triple — same inputs, same filename, cache hit. AST-level changes therefore self-invalidate
through the function hashes; **emitter-level changes do not** — a change that alters generated
machine code for identical inputs (IR generation, target-machine setup, `[llvm_code]`
generators, the jit ABI) is invisible to the key and silently serves stale code from cache
unless `LLVM_JIT_CODEGEN_VERSION` is bumped. Stamped `[llvm_code]` *arguments* are not
emitter-level: they fold into both cache keys per function (the hint folds), so a change that
merely re-selects which perm gets stamped — the `[tune]` machinery — self-invalidates with no
bump. "The jit call ABI" is the contract between the
generated code and the engine: the generated function signatures and name scheme
(`create_uid_nodes` / `get_dll_fn_name`), the prologue shape (`jit_emit_prologue`), the
`LlvmJitFlags`/`LlvmJitMode` inputs to the emitter, and the extern-resolution surface the
install phase binds (`ResolveExternVisitor`, `generate_llvm_code`, `instrument_jit`). A pinned
`jit_output_path` bypasses the content-addressed name entirely; its probe compares function
hashes only, which is why the summary line asserts the opt-level tag only when the tier is
actually known.

### 2.1 The split obj cache — positional invalidation

Under `--jit-split-modules`, each per-module partition object is content-addressed too
(`--jit-obj-cache`, on by default under split): its key is the running fold of every module
hash up to and including its own — symbol names, per-function AOT hashes, and the JIT-only
hint folds — combined with `jit_env_salt`, the ONE helper both the DLL key and the partition
keys fold their config/environment inputs through (a component folded into one key but not
the other would let a config change link stale objects). The chained prefix makes
invalidation **positional**: module order is topological, so a change in module j re-keys
every partition from j on, while everything before j links its cached `.o` — the probe is
bare file existence, before any per-partition LLVM state is created, so a hit skips
declaration, irgen, and the optimize/emit pool outright. Three consequences for consumers:
**require order is the cache layout** — a module you edit often belongs as late in the
require chain as its dependencies allow (registration-only requires, like the dasLLAMA GPU
tiers, belong at the END of an umbrella, not in a root module everything depends on); a
rename-without-body change still re-keys, because the module hash folds symbol names, not
just function hashes; and **the cache holds exactly one generation** — the GC keep-set is
the current link set, so reverting an edit is an eviction, not a hit: the run after a revert
re-emits from the reverted module on, same as the edit did.

## 3. Overrides and their announces

The backend's override knobs — the escapes that change what a run compiles, tunes, or emits
beyond its defaults — are: `DAS_TUNE_POLICY` (replaces the declared/injected tune policy),
`DAS_TUNE_MODE` (grid/tuner compile modes), `DAS_TUNE_MANIFEST` (pins the sidecar),
`DAS_TUNE_NOISE_CV` (recalibrates the tuner noise gate), `DAS_TUNE_NOISE_OVERRIDE` (mints
through a failing gate), `--tune` (forced re-mint), `--jit-obj-cache=0` (forces every split
partition to re-emit, bypassing the obj cache), `DAS_JIT_PROBE_LTO` (split partitions emit
bitcode and the link runs lld LTO — a dev probe artifact), `DAS_JIT_X64_FORCE_FEATURES` /
`DAS_JIT_ARM64_FORCE_FEATURES` (force CPU features past detection — emission, the cache keys,
and `cpu_supports`-based tune eligibility all follow), and the runtime escape API
`tune_suppress_mint(knob)` (a library `[init]` suppresses the auto/restart mint; the caller
passes the knob name it acts for). The announce contract: an override announces at the point
it CHANGES THE OUTCOME — at least one line naming the knob (its env spelling, or the
caller-supplied knob name for `tune_suppress_mint`); a set-but-inert override may stay silent,
and per-scope or per-site repeats are correct. A library that only exposes the override bit
(a `*_overridden` query such as `tune_noise_threshold_overridden`) discharges the contract
when the announce lands at the consumer in the same change. Verbosity knobs
(`DAS_TUNE_VERBOSITY`) shape only how much is printed, not what runs — they are not overrides
under this contract.

Environment knobs load ONCE, at context init, into the `[EnvConfig]` structs `g_env_jit` /
`g_env_tune` (`llvm_env.das`) — a mid-process `setenv` changes nothing the backend reads.
In-process overrides therefore go through the tune setters (`tune_set_verbosity`,
`tune_set_noise_cv`, …), which also arm spawned children by exporting the matching variable.
