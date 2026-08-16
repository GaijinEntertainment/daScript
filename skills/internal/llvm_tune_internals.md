# llvm_tune internals (repo-only)

Read before touching `modules/dasLLVM/daslib/llvm_tune.das` or its tests. The framework
itself — every annotation, the policy rail, the sidecar contract — is `skills/tune.md`; this
file is only what maintaining the implementation needs on top of it.

## Macro-context traps

- **Cross-module restamp does not work.** Mutating a *required* library's already-compiled
  `[tune]` function from the root apply (strip + restamp `[llvm_code]`) trips the re-infer
  pass with an access violation in `Function::visit`. Library kernels take their winner at
  their own `[tune]` time; the runtime-tune-and-re-exec model exists to avoid the restamp.
- **The app identity comes from process argv, never the AST.** Each module compiles as its
  own `Program`, so `compiling_program().thisModuleName` names the library, not the root — a
  library `[tune]` apply cannot see the app script. `tune_app_script()` parses the process
  command line instead. A host that compiles other programs in-process (dastest, the MCP
  server) therefore resolves to *its own* sidecar; dastest pins `DAS_TUNE_POLICY=fallback`
  so that can never tune.
- **`find_module(name)` resolves against `this_program()`** — the runtime program, null in
  macro context, so the call is an access violation. Walk `compiling_program()`, or use
  `find_compiling_module(name)`.
- **`get_command_line_arguments()` aliases a process-global locked array.** Never `delete` it.
- **A shared macro-module global is a per-requiring-module copy.** `llvm_exe` calling an
  `llvm_tune` function reads `llvm_exe`'s own (empty) copy of `g_scopes` — the same trap as
  the `[llvm_code]` generator registry. Anything a second macro module must read travels
  through the AST, a file, or the environment. Hence `tune_scopes_status(prog)` walks
  `[tune_scope]` structure annotations plus `[tune]` functions rather than the bank, and
  `collect_status` reads the stamps back off the AST.
- **`stamp_llvm_code` records `tune_suffix` / `tune_from`** as extra `[llvm_code]` args
  (generators ignore unknown args by contract), and `tune_status` reads that stamped truth
  off the AST — there is no macro-state bank to keep coherent.
- **`--tune` is read at macro time** from the compiler argv after `--`, and stripped from the
  re-exec so the child converges instead of looping.

## Where the pieces live

- The default policy is injected by the `TuneDefaultPolicy` infer pass. A root is detected by
  the anonymous module name, and the `tune_policy` marker annotation the pass appends is its
  once-only guard.
- `scope_fnames_joined` appends a `version_of` pin to the joined kernel names as one
  `@version:key=value` token, so every carrier of that string — the policy guards, the
  resolver contract, daspkg's status walk — enforces the pin with no signature change.
- `module_kernel_fnames` derives a scope's demand from the AST. `tune_sidecar_verdict` is the
  named-reason completeness API; `tune_sidecar_missing_entries` is its bare-list twin.
- `cpu_supports` must answer on arm64 as well as x86 (sysctl / `AT_HWCAP` /
  `IsProcessorFeaturePresent`, LLVM target-feature spellings). An x86-only probe that fails
  closed leaves every arm `requires=` gate silently never firing.

## The gates

`tune_aot_gate()` and `tune_exe_gate()` are deliberately separate: cross-box artifacts are
fully tune-free, while `-exe` still stamps and still emits the status `[init]`. **`aot_module`
is not a tune signal** — it means "module-shaped compile", which dastest sets on every test
compile and the AST serializer needs.

The `-exe` emission half is coupled. A native exe carrying any `[llvm_code]` kernel computes
its target flags *with* host features and targets the host machine; the decision is made once
in `run_jit` (`exe_host_cpu`) before the flag pass and reused at `init_jit_target_flags`,
`init_jit` and `write_exe`. Flags and machine must never diverge — host-F16C flags on a
generic machine lower fp16 to libcalls msvcrt does not link. Witness: `g_target_host_features`
plus the `llvm_code_selftest::host_features` probe in `tests/jit_tests/exe_host_cpu.das`.

## The dasLLAMA tuner — the worked consumer

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
completeness. `llvm_tune_manifest.das` covers the write→stamp round trip and the staleness
rail (back-dating with `set_mtime` + `mktime`). Both run under `-jit` and short-circuit to
pass when `!jit_enabled()`.
