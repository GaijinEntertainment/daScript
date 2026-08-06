# Profiling rail — zero-cost markers + AMX-for-ASR audit

Boris-picked follow-up to the canary-apples arc (2026-08-06). Two legs, deliberately small, no
contact with anything serious; the AMX audit is the acceptance run for the new profiling rail.
Metal stays frozen until the unification/macro work — nothing in this arc touches it.

## The design, as settled

**Zero-cost mechanism (probe-verified 2026-08-06):** markers gate on `static_if
(JOBQUE_PROFILING_ENABLED)` where the constant is a global `let` folded at compile time.
`static_if` accepts a global `let` as its condition — bools and arithmetic (`LEVEL > 2`) both
fold, cross-module too. The dead branch erases pre-infer (the platform-half idiom already
depends on this), the marker defs become empty, and `-jit` inlines empty callees across module
boundaries — literally nothing left in the hot path. The folded constant lands in the semantic
hash, so the JIT `.dll` cache self-invalidates on flag flips; no `LLVM_JIT_CODEGEN_VERSION`
bump needed.

**The one missing piece is `daslib/build_const`:** a call macro that reads the process
command line / environment AT COMPILE TIME and splices a literal. Probe-verified: the macro
fires at global-init scope (`let PROF = has_arg("--prof")` expands), macro-time
`get_command_line_arguments()` sees the full daslang argv including post-`--` program args,
and the spliced literal feeds `static_if` same-module and cross-module. ~25 lines; the probe
is essentially the module. Nothing existing covers this (`constant_expression` only folds
already-constant call arguments; the driver has no `-D`).

**Carriers: both.** `has_arg("--flag")` / `arg_value` / `arg_int` (argv) and `env_flag("NAME")`
/ `env_value` / `env_int` (environment).

**Semantics (document, don't solve):** the constant folds from whatever the COMPILE-time
process saw. Interp/`-jit` compile per run, so it is the run's command line/env. `-exe` bakes
at build: an exe built with profiling enabled ships enabled — off by default, stated in the RST.

**Coherence rule across all rails:** one bool-parse rule (dasllama_env's: unset → default;
`0/false/off/no/empty` → false; else true) and one precedence (**argv > env > default**) shared
by build_const, the clargs env axis, and the env readers. "How do I set a knob" is one mental
model whether it resolves at compile time or run time. One deliberate divergence: the clargs
env axis is LOUD on garbage (`DASLLAMA_THREADS=fifty` is an error), unlike the registry
readers' silent-default.

## Phases

1. **DONE — `daslib/build_const`** (nine accessors: `build_flag`/`build_value`/`build_int`
   both-carrier + argv/env singles; tests spawn a fixture with controlled argv/env; RST
   wired). Set-but-empty env counts as unset on every accessor. The combined accessors are
   the recommended spelling — a gating global's init must stay a SINGLE call (see
   "Compiler gap" below).
2. **DONE — marker rail gated** — `profile_tag`/`profile_marker` + dasLLAMA's
   `trace_tag`/`trace_marker`/`set_trace_tags` erase without the flag; control plane stays
   live; `decode_prof --trace` refuses loudly on a rail-less build; `test_jobque_trace`
   asserts BOTH worlds (ON = annotations land, OFF = erasure contract). Flipping the flag
   cold-rebuilds the JIT cache (semantic hash) — confirmed live.
3. **clargs env axis** — `@clarg_env = "NAME"` per field + struct-level
   `[clarg_env_prefix="X"]` auto-derivation (`cpu_prefill` → `X_CPU_PREFILL`,
   `@clarg_env=""` opts out), precedence argv > env > default, help output shows the env twin
   per option. Serves tools (detect-dupe, daspkg, dasllama-server, preflight); kills the
   "env reads growing beside the clargs struct" pattern.
4. **The sweep** — ~391 raw `get_clock`-family sites in `modules/dasLLAMA/dasllama/` alone.
   Classify: perf-bucket accumulators → zones on the marker rail; big UNMARKED intervals
   (model load, dlim map, tokenizer init, warmup) → zones so the timeline finally shows them;
   log-writes-inside-timed-intervals → move the I/O out or make the write its own zone so the
   tax is visible instead of silently folded into the surrounding bucket. Bench-harness
   stopwatches (lcpp_bench timing loops) stay — they ARE the measurement, not instrumentation.
5. **AMX-for-ASR audit (acceptance run)** — everything from #3562 carries: dasAccelerate,
   the float-batch override rail, the VECLIB pin + strip-dispatch, the `min_mmac` floor.
   Audit = which ASR encoder ops are float-plane GEMMs above the floor. ASR encoders are a
   better AMX target than the LLM ladder was (float-heavy, encoder batch inherently large —
   a whisper 30s window is ~1500 frames vs the ntok≥32 gate). Watch: the 35B density-vs-size
   floor lesson (many mid-size calls amortize the wake tax but can't clear a per-call floor);
   the BNNS-f16 lane shipped uncrowned with M4/M5 as its stated target — ASR-on-M4 may crown
   it for the first time. Predictions before the A/Bs, per the game. CPU-side only.

## Tail end (own arc, NOT this one)

**`[env_config]` library-knob migration** — module-declared knob structs whose macro generates
the cold-path read-once loader (PERF027 by construction), typed access, the doc-gen registry
entry, and the coverage test's ground truth; the hand-maintained `k(o, ...)` list in
`dasllama_env.das` dies. ~130 knobs. When we get there, look at the code again for the
shortcut — the `"DASLLAMA_"` literal prefix at every read site is a mechanical hook: a tiny
macro/tool can enumerate the reads and EMIT the declarations, so the migration may be mostly
generated.

## Compiler gap — FIXED (Boris-approved "mirror fix", same arc)

Under `no_optimizations` (the lint/LSP compile profile — CI's lint lane), a GLOBAL's
initializer expression was never folded, so `static_if (P)` where `let P = a || b` failed
with `error[30229]` there while working in normal compiles. Fixed in `getConstExpr`
(`ast_infer_type_helper.cpp`): the global-var arm recurses into a non-literal init, and
new ExprOp1/ExprOp2 arms fold pure builtin operators over constant children on a
DETACHED clone — never in place, so lint rules still see original source shapes.
Regression: `tests/typer_errors/test_static_if_profiles.das` (composed inits resolve;
genuinely non-constant conds still refuse).

## Lint candidates (report at arc end, don't build mid-arc)

- Raw clock-pair timing in engine dirs once the marker rail is canonical.
- Raw `get_env_variable` outside the sanctioned rails (env readers / clargs / build_const).

## Scope guards

- No metal contact (frozen until unification/macro work).
- No tune-framework timing changes; no C++ dasprof twins; bench stopwatches stay.
- Zero-cost claim gets MEASURED, not asserted: off-tax A/B on asr_bench/lcpp_bench (target:
  indistinguishable), on-tax measured and stated (lcpp's profiler tax is 5–7%; ours should
  embarrass it).
