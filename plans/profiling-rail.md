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
   the recommended spelling; composed initializers also resolve everywhere since the
   compiler fix below.
2. **DONE — marker rail gated** — `profile_tag`/`profile_marker` + dasLLAMA's
   `trace_tag`/`trace_marker`/`set_trace_tags` erase without the flag; control plane stays
   live; `decode_prof --trace` refuses loudly on a rail-less build; `test_jobque_trace`
   asserts BOTH worlds (ON = annotations land, OFF = erasure contract). Flipping the flag
   cold-rebuilds the JIT cache (semantic hash) — confirmed live.
3. **DONE — clargs env axis** — `@clarg_env = "NAME"` per field +
   `[CommandLineArgs(env_prefix = "X")]` derivation (`cpu_prefill` → `X_CPU_PREFILL`,
   `@clarg_env = ""` opts out), argv > env > default with argv keeping its errors, shared
   bool rule, loud garbage, required satisfied by either carrier, mutex argv-only, help
   shows `(env: NAME)`. Implemented at the three `CommandArgumentInfo` chokepoints so
   parse/enum/required inherit it untouched. 83/83 + daspkg 220-test consumer suite green.
4. **DONE — `[EnvConfig]` + the dasLLAMA knob migration (pulled forward — Boris).**
   Macro half: `[EnvConfig(env_prefix="X")]` in clargs generates `env_config(type<T>) : T`
   (field initializers as defaults, garbage numerics WARN and keep the default) plus
   `get_env_config_info(type<T>)`; extended with int64 and `Option<T>` (tri-state) fields
   and the doc-rail info (`default_doc` from `@clarg_default_doc`-or-initializer,
   `is_path` from `@clarg_path`). Migration half: the 146-name registry became eleven
   `[EnvConfig]` area structs in dasllama_env (name-for-name — verified by diffing the
   generated name set against the old doc), loaded ONCE at context init into `let g_env_*`
   globals (dependency order makes them safe from any requirer's globals and `[init]`s);
   ~230 read sites across dasllama/, harness/, benchmarks/, performance/, tests/ and
   dasllama-server became field reads (tri-states as `Option ?? computed`); tutorials keep
   one registered raw read for pedagogy; dynamic names (bench repro forwarder, lcpp
   levers) go through sanctioned `env_is_set`/`env_value_of`. ENVIRONMENT.md regenerates
   from the info; test_env_registry gained STRICT teeth (zero raw env reads in
   modules/dasLLAMA + utils/dasllama-* outside dasllama_env.das) and the CODEREVIEW env
   rule is live. Three previously-invisible lab filters (DASMETAL_LAB_ARMS/SHAPES/
   VARIANTS) got declared. Delta sweeps came back clean: no `VAR=`-empty disable idioms
   (the two hits target the tune framework's own readers), no `=0` presence-flag users.
   Deliberate semantics deltas, now documented in ENVIRONMENT.md: set-but-empty is unset;
   garbage warns; presence-flags honor `=0`; in-process `set_env_variable` is invisible
   to the loaded config.
5. **The sweep** — 392 raw clock-family sites (320 `ref_time_ticks`, 92 `get_time_usec`,
   1 `get_time_nsec`) in `modules/dasLLAMA/dasllama/`. Execution plan below; the acceptance
   instrument is AGGREGATE-ONLY — a single stopwatch pair is ~50–80 ns on the M1, 100x below
   what any box resolves, so per-site A/Bs would be 392 readings of noise at ~40 min each.
   Bench-harness stopwatches (lcpp_bench timing loops) stay — they ARE the measurement, not
   instrumentation.

### Phase 5 execution plan (the quiet-M1 night)

**Predictions, on record before any cell runs (the game):**
- Off-tax (pre-sweep HEAD vs post-sweep with profiling off): **zero within noise; if anything
  post-sweep is 0.0–0.3% faster on tg** (the always-on bucket pairs — ~160 clock pairs/token
  ≈ 10–25 µs against a ~10 ms 1B token — get DELETED from the off build). pp: no measurable
  delta (same pairs amortize over the whole batch).
- On-tax (post-sweep on vs off, marker zones at bucket density): **0.5–2% tg** — must
  embarrass lcpp's 5–7% profiler tax or the design failed.
- cv per cell on the quiet M1 ≤ 1.5%; interleaving cancels the overnight thermal drift that
  makes laptop A-then-B sequencing lie.

**Step 1 — census + classification (no box needed).** Walk all 392 sites into four classes:
- **C1 load-bearing** — the elapsed value FEEDS LOGIC (backend auto-selection, adaptive
  gates, tune measurement, watchdog timeouts). UNTOUCHED, marked `// clock: control` so the
  future lint can whitelist them. Getting one of these wrong breaks selection, not profiling —
  this classification is the real per-site work and it is review, not measurement.
- **C2 perf-bucket accumulators** (blocks.das 60, the arch files, moe/metal decode) → marker
  zones on the rail, erased when `JOBQUE_PROFILING` is off. `decode_prof --prof` then needs
  the rail compiled in and refuses loudly without it — same kaboom precedent as `--trace`.
- **C3 big one-shot intervals** (model load, dlim mint/map, tokenizer init, warmup — image 27,
  parakeet 27, audio 22...) → zones so the timeline finally shows them; log-writes inside a
  timed interval move out or become their own zone. Cold-path: cost is irrelevant, visibility
  is the point.
- **C4 serve/bench stopwatches that ARE the deliverable** — stay.

**Step 2 — sweep in batches** (C2 by file, then C3), compile + lint per batch, cheap
correctness gates between batches: kernels suite + image mechanics arm (model-less), one
llama2c forward-parity file. No timing gates mid-sweep — the transform is mechanical and a
regression wouldn't resolve anyway.

**Step 3 — the measurement night** (box handed over, caffeinate agent on, no user apps):
1. Tune-gate check first: if `m1.tune.json` is stale against the current binary, re-mint ONCE
   and use that manifest for every arm — a stale sidecar sandbags all cells equally-ish but
   voids the board rule, and ALLOW_UNTUNED numbers are never published.
2. Three arms, all coexisting in `.jitted_scripts` (distinct semantic hashes): **A** =
   pre-sweep HEAD, **B-off** = post-sweep, **B-on** = post-sweep with `JOBQUE_PROFILING=1`.
   Two benches: lcpp_bench (1B-class decode-heavy cell) + asr_bench. Cold-compile each arm
   once (~10–20 min total), then interleave launches A, B-off, B-on, repeat ×3 — 18 cells,
   strictly serial, one process on the box, complete logs kept, cv recorded per cell,
   cv > 3% voids the cell and it reruns.
3. Morning report: off-tax and on-tax with cv, per bench, against the predictions above —
   misses are the payload.

Budget: sweep is code time; the night is ~45 min compile + ~2 hrs of cells. Fits well inside
one sleep.
6. **AMX-for-ASR audit (acceptance run)** — everything from #3562 carries: dasAccelerate,
   the float-batch override rail, the VECLIB pin + strip-dispatch, the `min_mmac` floor.
   Audit = which ASR encoder ops are float-plane GEMMs above the floor. ASR encoders are a
   better AMX target than the LLM ladder was (float-heavy, encoder batch inherently large —
   a whisper 30s window is ~1500 frames vs the ntok≥32 gate). Watch: the 35B density-vs-size
   floor lesson (many mid-size calls amortize the wake tax but can't clear a per-call floor);
   the BNNS-f16 lane shipped uncrowned with M4/M5 as its stated target — ASR-on-M4 may crown
   it for the first time. Predictions before the A/Bs, per the game. CPU-side only.

## Tail end (own arc)

The tree-wide (non-dasLLAMA) raw `get_env_variable` cleanup + the lint rule banning raw
reads outside the sanctioned rails — `[EnvConfig]` has now proved out in dasLLAMA
(146 knobs, zero raw reads left in the module), so the shape is settled: declare fields,
load once, read `g_env_*`.

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
