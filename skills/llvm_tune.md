# llvm_tune — the `[tune]` kernel-tuning framework

Read before touching `modules/dasLLVM/daslib/llvm_tune.das`, adding a `[tune]` kernel family,
wiring a library into a per-box tune scope, or giving an app a tune policy. Full user-facing
reference: `skills/tune.md`. Worked application: `modules/dasLLAMA/tune_for_this_box.md`.

## What it is

Turns one reference function into a tuned `[llvm_code]` kernel family: a grid of generator
permutations, a PER-APP sidecar recording which one wins, and a policy rail. Winners are
**compile-time stamps** — the front end reads the sidecar and stamps the winning perm onto the
function before codegen. The generator only fires under the LLVM JIT (`-jit`); every other tier
runs the reference body.

**The sidecar** is `<app>.tune.json` beside the app — the root script the process runs (first
`.das` on argv before `--`), else the binary itself (standalone exe / embedded host).
`DAS_TUNE_MANIFEST` env overrides the location. Sectioned: `"kernels"` (perm winners, flat
fname→suffix), `"runtime"` (library knobs — dasLLAMA's), `"provenance"` (refreshed each write).
**Staleness:** a sidecar older (mtime) than the running binary reads as ABSENT everywhere and
`tune_manifest_set` resets it — measurements never outlive the binary (kills the worktree
hand-copy trap). Several libraries share one file: tuners UPSERT their own keys
(`tune_manifest_set` preserves the rest); "is scope X tuned" = per-key completeness
(`tune_sidecar_entries_complete`), never file existence.

## The annotation set

- `[tune_perm(<gen args>)]` — one grid row; args pass to the generator + fold into the JIT DLL key. `suffix=` renames; `requires=` is a fallback-eligibility hint (`,` = AND of `|`-separated ORs, matched against host CPU features + `DAS_JIT_*_FORCE_FEATURES`). Evaluated by the public `tune_requires_ok(expr)`; `tune_pick_fallback(chain)` runs the same rule over a `;`-chain of `suffix` / `suffix:requires` entries — that is what gives dasLLAMA's `[tuned]` a per-ISA default from ONE annotation, since its perms are generated and have no row to hang `requires=` on. **`cpu_supports` answers on arm64 as of this work** (sysctl / AT_HWCAP / IsProcessorFeaturePresent, LLVM target-feature spellings); before that it was x86-only and fail-closed, so any arm gate silently never fired.
- `[tune(gen="key", fallback="suffix;chain")]` — closes the perm bracket (applies run in declaration order). Needs an explicit return type. `fallback=` is a `;`-chain, first `requires=`-passing perm wins; `reference` forces the original body.
- `[tune_companion(fn="sibling", gen="key")]` — between the perms and `tune()`; stamps a sibling with the SAME perm from the same entry (two-function coherence).
- `[tune_scope(name="lib", tuner="rel/path.das", covers="modA;modB")]` — on a dummy struct in the **library** module owning the kernels; names the tuner unit that regenerates this library's keys in the app sidecar. `tuner=` resolves against the declaring file. `covers=` extends the DEMAND to other modules' kernels: completeness = a sidecar entry for every `[tune]` + non-`perm=`-pinned `[tuned]` fn across declaring+covered modules (AST-derived — `module_kernel_fnames`); a tuner missing a demanded kernel re-tunes every start and the startup warning NAMES it (`tune_sidecar_missing_entries`). Reading winners needs no scope — every `[tune]`/`[tuned]` reads the app sidecar. **Visibility trap:** the default-auto pass fires only for roots that SEE llvm_tune — the scope-owning library must `require llvm/daslib/llvm_tune public` (re-export llvm_tune ALONE — a blanket public that also re-exports jobque_boost floods requirers with ambiguities like `Stream`).
- `[tune_policy(missing = fallback|warn|error|auto|restart)]` — on the app's `main`. `error` = dev mode (fail compile, print tuner cmd). `auto` = tune at runtime + re-exec — **and the DEFAULT since 2026-07-12 ("untuned does not start")**: any root-with-`main` app whose libs declare scopes gets it with NO annotation (the `TuneDefaultPolicy` infer pass injects it; root detection = the anonymous module name; the appended `tune_policy` marker annotation is the once-only/dedup). Escapes: `DAS_TUNE_POLICY=fallback` (CI kill switch), the host's `-documentation` policy for documentation/reflection roots (all `[tune]` transforms and policy are inert; query through `is_building_documentation()`), the standing gates, and structurally any root without `main` — dastest test files never tune. `DAS_TUNE_POLICY` env overrides any declared flavor.
- `tune_status()` — runtime rows (fname, suffix, source manifest/fallback/reference, scope, path); populated when `[tune_policy]` is present. Log at startup.
- **Progress events** — `tune_progress_plan/_kernel_begin/_kernel_step/_kernel_end` emit `@tune <kind> k=v` lines from a tuner; `tune_progress_feed/_line/_active/_reset` is the consumer seam. `run_scope_tuner` relays them. **One rule decides who draws: a process renders when its OWN stdout is a terminal (`daslib/tty`), otherwise it forwards events verbatim.** Forwarding is unconditional; `DAS_TUNE_VERBOSITY` (`silent`/`normal`/`verbose`, set by `--tune-quiet`/`--tune-verbose`) gates only the human-visible half — that is how `silent` means nothing on a terminal while a supervisor still gets every event. Raw tuner chatter is muted **only while a display is live**, so a tuner that emits no events prints exactly as before (adopting the protocol must never be a prerequisite for being visible). Nothing displayed is ever predicted — no ETA, no expected duration.
- **The noise gate** — the framework carries the measurement-statistics + refusal policy a harness gates with: `tune_median`/`tune_cv_pct` (rank finalists by median-of-finalist-rounds, flag a best-of far under its own median), `tune_noise_threshold_pct(paranoid)` (2%/1%, `DAS_TUNE_NOISE_CV` calibration override), `tune_noise_override()` (`DAS_TUNE_NOISE_OVERRIDE` — the harness stamps `noise: overridden`), and `tune_provenance_note(key, value)` (merged into `"provenance"` on every sidecar save; identity keys always win). The dasLLAMA halves share the probe itself via `modules/dasLLAMA/harness/tuner_noise.das` (repo-only): probe at start/mid/end, refuse + exit nonzero above threshold, one settle-and-retry per failed probe, a noisy END probe = nothing written (the gen half defers ALL `tune_manifest_set` calls into one end-gated `tune_sidecar_merge`). The closing probe runs BEFORE any GPU race in the process — the bracket must cover the window that produced the winners, and a thread that blocked on the GPU wakes cold (the same E-core transient a sleep causes: median ~1.9×, cv 20-30%, recovers under ~2s of sustained load).
- `[tune_manifest]` is GONE (2026-07-12) — the per-app sidecar made it redundant; its root-module-only restamp rail died with it.

## The load-bearing facts (learned the hard way)

- **Tuning cannot run mid-compile.** The tuner is a separate daslang process; you cannot spawn it during a macro apply and adopt its winners in the same compile. `auto`/`--tune` therefore tune at **runtime** (a guard prepended to `main`) then **re-exec** the process — the fresh compile stamps the winners at `[tune]` time. This is why `auto` needs `main` to return void or int and re-launches via `popen_argv`.
- **Cross-module restamp is broken.** Mutating a *required* library's already-compiled `[tune]` function from the root apply (strip+restamp `[llvm_code]`) trips the re-infer pass (`Function::visit` AV). Library kernels take winners at `[tune]` time from the app sidecar, NOT via a post-hoc restamp; the re-exec model exists precisely to avoid this. (The `[tune_manifest]` restamp rail was deleted with the sidecar rework.)
- **The app identity comes from PROCESS argv, not the AST.** Each module compiles as its own Program (`compiling_program().thisModuleName` = the library, not the root), so a library `[tune]` apply cannot see the root file — `tune_app_script()` parses the process command line instead. Hosts that compile other programs in-process (dastest, MCP) resolve to THEIR own sidecar; dastest pins `DAS_TUNE_POLICY=fallback` so that never tunes.
- **`find_module(name)` resolves against `this_program()`** — the *runtime* program, NULL in macro context → null-`Program` AV. In a macro use `compiling_program()` (walk it) or `find_compiling_module(name)`, never `find_module(name)`.
- **`get_command_line_arguments()` aliases a process-global locked Array** — never `delete` it.
- **`--tune` is read at macro time** from the compiler argv (after `--`), and stripped from the re-exec so the child converges instead of looping.
- **`DAS_JOBQUE_THREADS=N` means N total compute lanes**, not N spawned workers: JobQue creates
  N-1 worker threads and the calling thread is the final lane. Set it in the process environment
  before launch; an app's later TOML/CLI `threads` setting may cap dispatch width but cannot resize
  a queue already created during JIT/tune startup. `set_jobque_threads_cap`, by contrast, takes the
  spawned-worker cap, so code targeting N total lanes passes `N-1` before the first queue use.
- **Gates:** cross-box artifacts (`policies.tune_frozen` — set by the `-aot` driver and dastest's AST serializer) are fully tune-free, and AOT-consuming runs (`policies.aot`) only when the JIT is off (a stamp changes the semantic hash → AOT link fails; under `-jit` the JIT supersedes AOT bodies, stamps stay live). `aot_module` is NOT a tune signal (it means "module-shaped compile" — dastest sets it on every test compile and the serializer needs it). `-exe` (`jit_exe_mode`) still STAMPS (an exe next to a sidecar ships those winners; else the generic fallback) and still emits the status `[init]` (the artifact self-reports via `tune_status()`/`log_tune_status`), but the policy rail is dead. Split as `tune_aot_gate()` vs `tune_exe_gate()`. **daspkg release ALWAYS mints**: the release-deps JSON carries `tune_scopes` (per-key completeness via `tune_scopes_status(prog)` — AST-walked, see the trap below); every scope with a tuner re-mints on every release — `--quick` is the ONLY mode that inherits, and only a COMPLETE sidecar (incomplete/stale mints even under `--quick`; the frozen exe never tunes, so unmeasured winners never ship). A tuner refusal (noise gate, validation) FAILS the release outright. Release prints the shipped provenance per scope, REBUILDS after minting, and ships the sidecar beside the exe as `<bundle>.tune.json` touched newer than the exe (older = stale = ignored). The manifest release checks is the SOURCE-side one (beside the app script); the bundle gets a byte-identical copy.
- **Shared macro-module globals are PER-REQUIRING-MODULE COPIES.** llvm_exe calling an llvm_tune function reads llvm_exe's *copy* of `g_scopes` (empty) — the same trap as the `[llvm_code]` generator registry ("the right table copy"). Anything another macro module must read goes through the AST (annotations/stamps), files, or env — never a shared global. That's why `tune_scopes_status(prog)` walks `[tune_scope]` structure annotations + `[tune]` functions instead of the bank, and why `collect_status` reads stamps off the AST. **Emission half (fixed 2026-07-12):** a native `-exe` carrying any `[llvm_code]` kernel computes the target flags WITH host features and targets the host machine — decided ONCE before the flag pass in `run_jit` (`exe_host_cpu`) and used at `init_jit_target_flags`/`init_jit`/`write_exe` alike; generic flags would make every feature-gated generator decline to its reference body (the 11-15× dasLLAMA release regression), and flags/machine must never diverge (host-F16C flags on a generic machine lower fp16 to libcalls msvcrt never links). Kernel-free exes stay generic/redistributable. Artifact witness: `g_target_host_features` + the `llvm_code_selftest::host_features` probe (`tests/jit_tests/exe_host_cpu.das`, repo-only).
- **`stamp_llvm_code` records `tune_suffix`/`tune_from`** as extra `[llvm_code]` args — generators ignore unknown args by contract, and `tune_status` reads that stamped truth back off the AST (no macro-state bank).

## The dasLLAMA tuner (the worked consumer)

`modules/dasLLAMA/harness/dasllama_tuner.das` (repo-only) is the scope tuner — a wrapper spawning
`gen_tune_probe.das` (the `[tune]` generator grid) then `tune_kernels.das` (the `[tuned]`
loop-hint grid + the `"runtime"` knob snapshot), both upserting the one env-pointed sidecar.
Two race modes only: default (`tune_kernels` 20 screening / 80 finalist rounds — the `phase` on
a step event is `screening`/`finalists`, `live` is what `screening_keep` computes; `gen_tune_probe`
flat best-of-6) and `--tune-paranoid` (240 / best-of-18, 1% gate). There is NO fast race mode —
short races cannot resolve sub-2% twins; the debugging concession is `daspkg release --quick`
(accept the existing sidecar, race nothing). The confirm pass is never cut — a correctness gate,
not a budget. The flag spec is shared: `harness/tuner_cli.das`, required by bare name (same-directory).

**Self-validation judges same-window ORDER, never cross-window levels.** After the sweep,
`tune_kernels` re-races a heavy subset twice at full budget; the verdict is
`tune_validate_verdict` over per-window `TuneValidateRow`s: a kernel FAILS the mint only when a
re-race rejects the recorded winner outright, or the SAME challenger beats it past the reproduce
band (`max(1.5×floor, 1%)`) in BOTH windows — deliberately narrower than the original
any-single-window-flip rule, because a one-window flip or differing challengers is state
scramble among twins, not a false crown. The band still stands without the old drift backstop:
it bounds twin scramble between QUIET mints (measured ≤1.2%), and the burst-minted-winner class
the drift bound guarded is caught by the same-challenger consistency test instead (a burst
crown loses both quiet re-races). Absolute median drift between race and re-race is stamped
(`validation_max_drift_pct`) and noted past 5%/3%, but never fails — box levels move with load
history (DVFS, heat soak) while same-window order holds, so cross-window medians prove nothing
(measured on M4: 4–8% level drift on compute-bound kernels with in-burst cv 0.6%).
`DAS_TUNE_NOISE_OVERRIDE=1` mints through BOTH gates — a failed noise probe and a failed
validation verdict — stamped `noise: overridden` / `validation: overridden` in provenance.

**The shipped fallback has ONE source.** `[tuned]` banks each kernel's resolved fallback and
`[dasllama_fallbacks]` (a dummy struct in `tune_kernels.das`, declared after the requires that
pull the kernels in) emits `dasllama_tuned_fallbacks() : table<string;string>` from that bank.
`report()` looks the baseline up there rather than taking a literal — which matters because the
baseline is not cosmetic: a sub-0.5% win reverts to it, and that decides what lands in the
sidecar. A per-ISA chain resolves per box, so a literal could not have followed it. The bank
survives the module hop because it is read by a macro in the SAME macro module (`dasllama_tune`);
the cross-macro-module trap still applies to anything else.
`tune_kernels` records explicit shipped-fallback entries for kernels it doesn't sweep yet
(dot_bf16, add_scale_inplace, the tq4 codec four, dot_q8q8_f16s) — a missing demanded key
would re-tune every start. Adding a `[tuned]` kernel ⇒ add its key to tune_kernels' winners
map (sweep it, or record its shipped fallback).

## Adding a kernel family to a scope

1. Write the reference `def` with an explicit return type; register the generator key under `[llvm_code]` (see `llvm/daslib/llvm_jit_code.das` + `llvm_user_modules.das`).
2. Add the `[tune_perm]` grid + `tune(gen=, fallback=)`; companions between them.
3. If the library owns a scope already (`[tune_scope]`), the new family joins it automatically (same manifest). Otherwise declare one on a dummy struct in that module.
4. The harness benches `<name>_variants()` (the grid registry, present under `DAS_TUNE_MODE=tune`) and calls `tune_manifest_set("<name>", winner)`.

## Testing

`modules/dasLLVM/tests/llvm_tune_scope.das` (repo-only) is the end-to-end pattern: spawn the app as child daslang
processes across the policy flavors, use the `llvm_code_selftest::add_plus_k` generator (emits
`a+b+k`) so the RESULT is a fingerprint of the stamped perm. The tuner is a seconds-fast fake
that upserts via `tune_manifest_set`. Two scopes sharing the one sidecar prove the upsert
preserves the other's keys and that per-scope tuned-ness is key completeness.
`llvm_tune_manifest.das` covers the write→stamp round-trip plus the staleness rail (back-dates
the sidecar with `set_mtime` + `mktime`). Runs with `-jit`; the `[test]` short-circuits to pass
when `!jit_enabled()`.
