# llvm_tune internals (repo-only)

Read before touching `modules/dasLLVM/daslib/llvm_tune.das` or its tests. The framework
itself - every annotation, the policy rail, the sidecar contract - is `skills/tune.md`; this
file is only what maintaining the implementation needs on top of it.

`llvm_tune.das` registers no annotation of its own. All six names plus the default-policy
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

`tune_aot_gate()` and `tune_exe_gate()` are deliberately separate: cross-box artifacts are
fully tune-free, while `-exe` still stamps and still emits the status `[init]`. **`aot_module`
is not a tune signal** - it means "module-shaped compile", which dastest sets on every test
compile and the AST serializer needs.

The `-exe` emission half is coupled. A native exe carrying any `[llvm_code]` kernel computes
its target flags *with* host features and targets the host machine; the decision is made once
in `run_jit` (`exe_host_cpu`) before the flag pass and reused at `init_jit_target_flags`,
`init_jit` and `write_exe`. Flags and machine must never diverge - host-F16C flags on a
generic machine lower fp16 to libcalls msvcrt does not link. Witness: `g_target_host_features`
plus the `llvm_code_selftest::host_features` probe in `tests/jit_tests/exe_host_cpu.das`.

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
pass when `!jit_enabled()`. The defaults-profile rail is `llvm_tune_profiles.das` (+ its
`_client` / `_lib` / `_tuner` fixtures): it copies the fixtures into a per-process temp
directory, redirects the copied lib's `defaults =` at a `tune_defaults/` beside them, writes its
profile files there at runtime, named by this box's own `tune_cpu_class()`, and fakes a
less-capable minting box with an empty `features` field to force the unlocked-seat race. It
never writes under the tracked `llvm_tune_profiles_defaults/`; `modules/dasLLVM/REVIEW.das`
reds a test that does.

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
