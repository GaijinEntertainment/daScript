# llvm_tune internals (repo-only)

Read before touching `modules/dasLLVM/daslib/llvm_tune.das` or its tests. The framework
itself - every annotation, the policy rail, the sidecar contract - is `skills/tune.md`; this
file is only what maintaining the implementation needs on top of it.

`llvm_tune.das` registers one annotation of its own, `[tune_policy_default_applied]`, the
marker the default-policy pass stamps. The six user-facing names plus the default-policy
infer pass are registered by `daslib/tune`, whose shells forward here - `tune_apply`,
`tune_perm_apply`, `tune_companion_apply`, `tune_scope_apply`, `tune_policy_apply`,
`tune_default_policy_apply`, and `llvm_code_apply` in `llvm_code.das` - under
`static_if (typeinfo module_exists(llvm_tune))`, and do nothing when the guard is
false. Adding an annotation means adding both halves. A shell may not stay behind in this
module: **a module's macro state is per-module**, so `g_scopes`, `g_perm_rows` and
`g_companion_rows` are only consistent while every writer and reader runs in one macro
context - which is why the infer pass moved to `daslib/tune` with the annotations.

## Macro-context traps

- **Cross-module restamp does not work.** Mutating a *required* library's already-compiled
  `[tune]` function from the root apply (strip + restamp `[llvm_code]`) trips the re-infer
  pass with an access violation in `Function::visit`. Library kernels take their winner at
  their own `[tune]` time; the runtime-tune-and-re-exec model exists to avoid the restamp.
- **The app identity comes from process argv, never the AST.** Each module compiles as its
  own `Program`, so `compiling_program().thisModuleName` names the library, not the root - a
  library `[tune]` apply cannot see the app script. `tune_app_script()` parses the process
  command line instead. A host that compiles other programs in-process (dastest, the MCP
  server) therefore resolves to *its own* sidecar; dastest pins `DAS_TUNE_POLICY=fallback`
  so that can never tune.
- **`find_module(name)` resolves against `this_program()`** - the runtime program, null in
  macro context, so the call is an access violation. Walk `compiling_program()`, or use
  `find_compiling_module(name)` - which PANICS on a miss, never returns null; a "compiled yet?"
  question walks `compiling_program() |> for_each_module()` (`module_compiled`).
- **`get_command_line_arguments()` aliases a process-global locked array.** Never `delete` it.
- **A shared macro-module global is a per-requiring-module copy.** `llvm_exe` calling an
  `llvm_tune` function reads `llvm_exe`'s own (empty) copy of `g_scopes` - the same trap as
  the `[llvm_code]` generator registry. Anything a second macro module must read travels
  through the AST, a file, or the environment. Hence `tune_scopes_status(prog)` walks
  `[tune_scope]` structure annotations plus `[tune]` functions rather than the bank, and
  `collect_status` reads the stamps back off the AST.
- **`stamp_llvm_code` records `tune_suffix` / `tune_from` / `tune_source`** as extra
  `[llvm_code]` args (generators ignore unknown args by contract), and `tune_status` reads that
  stamped truth off the AST - there is no macro-state bank to keep coherent. It appends the
  annotation from module `tune`, the module that registers `[llvm_code]`; readers match it by
  name, so nothing downstream cares which module it came from.
- **`--tune` is decided at runtime** by the guard (`tune_cli_force`, the same
  `apply_cli_tune_flags` parse over `get_user_args()`), because a warm module cache can serve a
  compile whose macros never saw this run's argv; the policy pass reads it at macro time only to
  warn when no scope exists to tune. The re-exec strips it so the child converges.

## Where the pieces live

- The default policy is injected by the `TuneDefaultPolicyShell` infer pass in `daslib/tune`,
  which forwards to `tune_default_policy_apply` here. A root is detected by the anonymous
  module name, and the `tune_policy` marker annotation the pass appends is its once-only guard.
  The pass reaches a root that sees `daslib/tune`, so a scope-owning library re-exports THAT
  (`require daslib/tune public`).
- `scope_fnames_joined` appends a `version_of` pin to the joined kernel names as one
  `@version:key=value` token, so every carrier of that string - the policy guards, the
  resolver contract, daspkg's status walk - enforces the pin with no signature change.
- `module_kernel_fnames` derives a scope's demand from the AST. `tune_sidecar_verdict` is the
  named-reason completeness API; `tune_sidecar_missing_entries` is its bare-list twin.
- `cpu_supports` must answer on arm64 as well as x86 (sysctl / `AT_HWCAP` /
  `IsProcessorFeaturePresent`, LLVM target-feature spellings). An x86-only probe that fails
  closed leaves every arm `requires=` gate silently never firing.
- `DAS_TUNE_POLICY=reference` is env-only by construction: the `[tune]` stamps in required
  libraries compile before the root that would declare it, so `missing = "reference"` is
  rejected at the annotation.

## The gates

`tune_aot_gate()` and `tune_exe_gate()` are deliberately separate. `tune_aot_gate()` makes a
cross-box artifact fully tune-free, but keeps the grid for a `--jit-compile-only` dump under a
cross triple, so the x64 emission rail can inspect it off-box. `tune_exe_gate()` is true for both
`-exe` and `-lib`: the command line sets one compile policy for both, `jit_exe_mode`, and the
option that makes the JIT emit a library is set after the compile, too late for a macro. Both
artifacts still stamp and still emit the status `[init]`. A macro that must tell a library from
an exe asks whether the root has a `main`. The default-policy pass adds the status `[init]` (the
generated function `` `tune`status`init ``) when the root has a `main`. For a root without one,
`library_status_init` adds the same function, with a `tune_library_init()` call ahead of the
rows. **`aot_module`
is not a tune signal** - it means "module-shaped compile", which dastest sets on every test
compile and the AST serializer needs.

The `-exe` emission half is coupled. A native exe carrying any `[llvm_code]` kernel computes
its target flags *with* host features and targets the host machine; the decision is made once
in `run_jit` (`exe_host_cpu`) before the flag pass and reused at `init_jit_target_flags`,
`init_jit` and `write_exe`. Flags and machine must never diverge - host-F16C flags on a
generic machine lower fp16 to libcalls msvcrt does not link. Witness: `g_target_host_features`
plus the `llvm_code_selftest::host_features` probe in `modules/dasLLVM/tests/llvm_jit_baseline.das`.

## The fat mode

Mechanism: `modules/dasLLVM/ARCHITECTURE_TARGET_FEATURES.md#fat-mode`. Maintenance notes:

- `stamp_fat` runs INSTEAD of the normal-mode pick, so a rule that changes how a winner is
  resolved (eligibility, companions' own entries, the fallback chain) has two homes: `tune_apply`
  and `fat_pick`. Both read the same profile files through `profile_kernels(klass, path,
  onlyClass)`; the fat side asks for one exact class, the normal side walks the chain.
- The dispatch is daslang generated by `fat_arm` - a multi-line `qmacro_expr` per arm. A
  one-line `{ $e(call); return }` does not parse; keep the arms multi-line.
- A trivially pure kernel with constant call sites is IPSCCP-folded at O3 and its clones
  vanish from the module: that is legal, not a defect. The fixture passes an argv length.
- The clone's `noinline` is load-bearing on AArch64 (the inliner admits a callee with more
  features when its body carries no target intrinsic); x86 refuses on features alone.
- `tune_fat_box_in_class` is cpuid only on purpose - the exe's runtime must never read the
  `DAS_JIT_BASELINE` of the build or a `DAS_JIT_*_FORCE_FEATURES` override for the pick.
- Every `pin_profile_chain` under fat pins the WHOLE ladder, because the ship set reads classes
  above the baseline.
- Every runtime word `llvm_tune` sets at `[init]` and reads later (`tune_fat_mask`,
  `tune_fat_built`, `tune_library_built`, and their setters) is an `[llvm_code]` stub over an
  LLVM-side global (`llvm_tune_fat_word.das`), never a plain das global - a new word takes the
  same shape: an engine's
  pooled job contexts are forked with the init script skipped, and a das global there is zero,
  which dispatches every worker to the baseline clone. The fixture's `FORK` value is that cell.
  The stubs' `[llvm_code]` spelling is `llvm_code_shell.das` - `llvm_tune` cannot require
  `daslib/tune` - and that require must stay non-public, or a program sees two annotations of
  the name.
- `llvm_tune` must never require the LLVM bindings (`llvm_boost`, `llvm_jit_code`, anything
  under `bindings/`): its closure reaches every `[tune_policy]` program, JIT or not, and the
  bindings' `[extern]` stubs land in the `dasbind` builtin in a per-process set and order, which
  the module cache reads as a builtin hash drift (`test_tuned.das`'s cache cell reds first).
  Generators live in modules `llvm_user_modules` requires; the annotation shell is its own module.

## The forced file

Mechanism: `modules/dasLLVM/ARCHITECTURE_TARGET_FEATURES.md#tune-forced-file`. Maintenance notes:

- `tune_force_path()` is the only reader of the forced-file knob, `DAS_TUNE_FORCE`. `tune_manifest_path` answers the in-process
  override first, then the forced file, then `DAS_TUNE_MANIFEST`, then the default beside the app.
- `is_forced(path)` is the one test that turns off the box and age checks. Those checks live in
  two places, `tune_sidecar_stale` and `tune_sidecar_verdict`, so a new check that judges a sidecar
  by who wrote it or when consults `is_forced` in both.
- Every sidecar write goes through `save_sidecar_doc`, which refuses the forced file. A writer
  that bypasses it writes over the file the build is pinned to.
- A forced file counts as a named file everywhere `DAS_TUNE_MANIFEST` does -
  `tune_manifest_explicit`, `tune_policy_gated`, `warn_manifest_untuned` read both knobs - so a new
  "was a file named" test reads both.
- `announce_forced_file` prints its one line once a process, from the default-policy pass of a
  root with a `[tune_scope]`, and prints nothing in a stamp-free compile.

## The dasLLAMA tuner - the worked consumer

`modules/dasLLAMA/harness/dasllama_tuner.das` is a wrapper spawning two separate processes:
`gen_tune_probe.das` (the `[tune]` generator grid), then `tune_kernels.das` (the `[tuned]`
loop-hint grid plus the `"runtime"` knob snapshot). Both upsert the one env-pointed sidecar
and each end-gates its *own* write, so an interrupt during the second half leaves the first
half's mint on disk. They share the noise probe in `harness/tuner_noise.das` and the flag spec
in `harness/tuner_cli.das`.

The shipped fallback has one source: `[tuned]` banks each kernel's resolved fallback and
`[dasllama_fallbacks]` emits `dasllama_tuned_fallbacks()` from that bank, so the harness looks
its baseline up instead of hard-coding one. That matters because a sub-0.5% win reverts to the
baseline, and a per-ISA chain resolves differently per box. The bank survives the module hop
only because a macro in the *same* macro module reads it; the cross-macro-module trap above
still applies to everything else.

## Testing

`modules/dasLLVM/tests/llvm_tune_scope.das` is the end-to-end pattern: spawn the app as child
daslang processes across the policy flavors, using the `llvm_code_selftest::add_plus_k`
generator (it emits `a+b+k`) so the *result* fingerprints which permutation was stamped. The
tuner is a seconds-fast fake that upserts through `tune_manifest_set`. Two scopes sharing one
sidecar prove the upsert preserves the other's keys, and that tuned-ness is per-key
completeness. `llvm_tune_manifest.das` covers the write->stamp round trip and the staleness
rail (back-dating with `set_mtime` + `mktime`). Both run under `-jit` and short-circuit to
pass when `!jit_enabled()`. The defaults-profile rail is `llvm_tune_scope.das` (+ its
`_client` / `_lib` / `_tuner` fixtures): it copies the fixtures into a per-process temp
directory, redirects the copied lib's `defaults =` at a `tune_defaults/` beside them, writes its
profile files there at runtime, named by this box's own `tune_cpu_class()`, and fakes a
less-capable minting box with an empty `features` field to force the unlocked-seat race. It
never writes under the tracked `llvm_tune_profiles_defaults/`; `modules/dasLLVM/REVIEW.das`
reds a test that does. The fat rail is `llvm_tune.das` (+ `_llvm_tune_fat_client.das` /
`_llvm_tune_fat_lib.das`): it writes a profile for the ladder's lowest and top class, builds
the client as a fat exe with `--jit-dump`, asserts each clone's attribute group and the
LLVM-side mask word in the IR, then runs the exe unpinned, pinned, and pinned outside the box
or outside the carried clones; the client's `FORK` value dispatches from a pooled skip-init
job context. Its last leg builds `_llvm_tune_fat_clib.das` as a fat `-lib` and loads it from
`_llvm_tune_fat_clib_host.das`: the class pick runs when the host creates the library,
`tune_library_built()` reads true there, and a pin to a class of another architecture fails the
create. `test_llvm_tune_forced_file` is the forced-file rail: a library and a `-jit` program
stamping from a file another box wrote, and the refused save (`_llvm_tune_force_save.das`). The
baseline rail is `llvm_jit_baseline.das`: the announce, the tier bits the
class bakes, the foreign and unknown class refusals, and the cross-target arm
(`--jit-target` + `--jit-compile-only`). Both build into a per-process temp directory. A
change to the fat emitter also gets the served-model rail: a `daspkg release --fat` of
`lcpp_bench` on an x86 box of a class above the baseline, whose `sanity:` lines must name the
model table's fixture tokens - the pooled-fork mask bug passed every fixture and only that
rail showed it.

## Shipped defaults profiles - where the pieces live

- `defaults=` resolves against the declaring file at `[tune_scope]` apply and rides
  `TuneScope.defaultsPath`; the guard emission bakes it, together with the scope's
  `scope_gated_families_joined` string - `family=requires+requires` rows, `;`-joined, only for
  families with a gated row, read off the AST - into the `tune_auto_prepare` /
  `tune_restart_needed` call, because the runtime has no AST.
- `locate_profile_doc` is the ONE ladder walk (class chain, version pin) both halves share, so
  compile time and runtime cannot pick different profiles. Compile-time: `profile_kernels`
  reads the `[tune_scope]` annotations OFF THE AST (a macro global is a per-requiring-context
  copy; the `[tuned]` rail in `dasllama_tune` reaches the same profile through
  `tune_kernel_pick`), registers every candidate path as a module-cache dependency, and the
  `[tune]` apply stamps the pick with `tune_source = "profile"`. That is why a scope with
  `defaults=` must compile before its `covers=` modules - the scope apply refuses one that
  already compiled (`module_compiled`, a non-panicking twin of `find_compiling_module`).
- Runtime: `profile_try_adopt` diffs the gated string against the profile's `features`
  fingerprint and hands the residue back in its `onlyFilter` out-param. An empty residue is a
  COVERED box: no write, no `g_tuned_scopes` bump, no relaunch - the compile already stamped
  the same picks. A residue `tune_sidecar_merge`s the kernels into the app sidecar as a normal
  local write, skips the scope resolver, and rides `run_scope_tuner(scope, onlyFilter)`, which
  arms `DAS_TUNE_ONLY` for that one spawn. It declines outright when `tune_manifest_path()` no
  longer equals the `manifestPath` the guard was compiled for.
- `--tune` / `--tune-only` are read at RUNTIME by the guard (`tune_cli_force`, the same
  `apply_cli_tune_flags` parse), and the auto guard is emitted for a complete scope too: under
  the module cache a compile's macros may never have seen this run's argv. The parse reads
  `get_user_args()`, not raw argv: that is what slices a standalone exe's command line the
  same way as `daslang script.das -- args`.
- `tune_profile_export` carries `kernels` plus the string provenance that survives, dropping
  `binary` / `box` / `engine_sha` and re-stamping `origin` = `profile`, `class` = the export's
  class argument, and `written`: a profile's identity is class + version pin + features, never
  a commit.
