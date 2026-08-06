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
5. **DONE — the sweep + the measurement night (2026-08-06, commit 86eadd98c).** Census,
   execution, and measured verdicts below. Headline: off-tax 0.00%, disarmed rail 0.07%,
   armed ≈0.7% — the zero-cost claim is MEASURED, and lcpp's 5–7% profiler tax is
   embarrassed at every level.

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

**Step 1 — census result (2026-08-06 night; true totals 319 `ref_time_ticks` + 74
`get_time_usec` consumers + 1 nsec — plain grep; the MCP `grep_usage` silently truncated
`dasllama_common.das` at ~line 3740, 13 reported vs 27 real — tool bug, reported):**
- **C2-rail (erased when off): the `prof_add` family and nothing else** — 172 starts
  (blocks 60, arch_qwen35 37, common 27, ple 22, moe 19, batch 7; every `ref_time_ticks` in
  those six files feeds `prof_add`, verified per site). Swap: starts become `prof_ticks()`
  (folds to `0l` off-rail), `prof_add`/`forward_profile_reset` bodies gate on
  `static_if (JOBQUE_PROFILING_ENABLED)`; reset PANICS off-rail. Fail-fast checks added:
  decode_prof + asr_stage_probe (unconditional — profiling is their purpose), lcpp_bench +
  verify_batch_probe (`--prof` only).
- **ASR family stays runtime-armed — census overturned the plan here.** All 68
  `asr_prof_add` sites back the encode-split DELIVERABLE (`lcpp_bench` samples
  `asr_prof_ms(ebucket)` on every ASR cell, rail or no rail), so compile-time erasure would
  break site records on off-builds. Its always-on start ticks are ~0.01–0.03% of an encode —
  unresolvable; left exactly as-is.
- **C1 `// clock: control` (marked):** prefix-cache `last_hit_at`/`born_at` (LRU eviction
  ordering) + the image tmp-name tick (uniqueness suffix, not timing).
- **Already-runtime-gated instruments, untouched:** `g_pf_prof` (math_default), `g_conv_prof`
  (load, incl. the one nsec site), metal step-trace (cap-armed), the vulkan `vk_prof()` log
  families (~20 per-chunk starts, log-gated, noise-level).
- **Metal `g_us_*` stage buckets (15 starts): untouched** — the chase/batch harnesses read
  them as a live numbers rail (`metal_decode_stage_stats`), metal is frozen this arc, and
  they run ~4 pairs/token (25× sparser than the CPU family).
- **C3 one-shot cold logs (~35: load stages, image bake/map, tokenizer, layout, gpu_resident,
  vulkan upload) and C4 serve stats (sampling `stat_*`): stay.** The planned C3→zone
  conversion is DROPPED for this arc: cost is zero (cold), and the trace window never covers
  model load, so zones there would record nothing — revisit only with a zone primitive plus
  load-window tracing, as its own decision.

**Step 2 — sweep executed in one batch** (the C2 set collapsed to a chokepoint gate + a
mechanical 172-site spelling swap, so file-by-file batching bought nothing). Gates all green:
compile_check ×4 harnesses, lint ×12 files, kernels suite 4/4, image mechanics 22 passed /
16 skipped, llama2c forward parity 10/10, plus a both-worlds decode_prof probe (off-rail =
fast refusal, on-rail = armed buckets report).

**Step 3 — the measurement night: RAN 2026-08-06, grid complete, no voids.**

Tune-gate deviation, forced and handled: `bin/daslang` was rebuilt at 01:53 (the getConstExpr
C++ fix), and a sidecar older than the binary reads as ABSENT by design — so the bench
sidecars re-minted regardless of code changes (`m1.tune.json` itself was never touched).
Protocol: pre-mint each bench sidecar ONCE on the quiet box, verify winners against the
pre-rebuild lineage (lcpp: 27/31 identical + 3 exact twins + 1 real flip — `copy_floats`
u4 +10.5%, a bandwidth kernel; asr: 5/5 identical), then FREEZE — all three arms share the
one sidecar, shasum-verified after every cell. Tune-neutral by construction.

Cells: Llama-3.2-1B Q8 CPU (lcpp_bench, p512 n128 r5, debug-jit flavor — relative A/B, not
record-grade) + canary-qwen-2.5B on jfk3 (asr_bench, r4, rep-0 discarded, CPU prefill).
Interleaved A → B-off → B-on ×3 per bench, strictly serial, arm A via git-checkout flips.
17/18 cells cv ≤ 0.5%; worst 2.3%.

**Results vs predictions:**
- **Off-tax: 0.00% tg (82.71 → 82.71), pp and asr wall zero within ±0.25%.** Predicted
  zero-within-noise — HIT; the "possibly 0.0–0.3% faster" tail did NOT materialize: the
  deleted ~160 pairs/token (~10–25 µs) sit under the ±0.15% launch spread even on a quiet
  box. The miss's payload: the always-on prof tax on the M1 was ALREADY unmeasurable — the
  deletion's value is the guarantee-by-construction (and whatever a zen2-class box shows),
  not a Mac speedup.
- **On-tax, disarmed (rail compiled in, buckets not armed — the shipped-available state):
  +0.07% tg — zero.** The 0.5–2% prediction mis-scoped this arm: bench cells never arm
  buckets, and a disarmed rail's per-pair mechanism equals pre-sweep HEAD's, so ≈0 is
  arithmetically forced.
- **On-tax, armed (labeled approximate — one decode_prof 1B cell, armed by construction,
  depth-8 window): 82.21 vs 82.77 t/s ≈ 0.7%** — inside the predicted 0.5–2% band, and the
  bucket report itself stays outside timed windows in the benches by design. lcpp's 5–7%
  profiler tax is embarrassed at both levels: ~0.07% to have profiling available, ~0.7%
  fully armed.
- cv ≤ 1.5% predicted: held (one 2.3% pp cell, one 2.4%-spread asr cell, both kept).
6. **AMX-for-ASR audit — the M1 leg ran 2026-08-06 (structural + measured), M4 leg remains.**
   Structural: the override seam is exactly `matmul_batch` (f32) + `matmul_bf16_batch`
   (bf16); every ASR tower GEMM reaches it through `tw_mm`'s fp32 fork, but the SHIPPED q8
   towers take `matmul_q8q8_batch` (NEON, seam-invisible by the #3562 crossover verdict) —
   only conv1 and non-32-multiple projectors ride the seam by default. The BNNS-f16 lane is
   UNREACHABLE from ASR (bf16 seam only; towers have no bf16 planes) — the M4-crowning hope
   needs a small f32→f16 fp-seam lane (the `f16_plane_ptr` conversion cache generalizes).
   Measured (whisper-turbo + canary, jfk3, interleaved): fp32-NEON is 1.8–6× slower than q8
   (`matmul_batch_core` never got the generated-GEMM treatment — not a real contender);
   fp32+AMX landed +16–19% wall behind q8. The encoder trace tags + `asr_stage_probe
   --trace` (built this leg) decomposed it: d=1280 GEMMs ran 5 sgemm strips / 3 idle lanes
   (eff 4.3) while 8-strip fc1 tied q8. `DASLLAMA_ACCEL_STRIPS=8` (new knob, [EnvConfig]
   rail): attn_proj and attn_out now BEAT q8, encode gap +15% → +4.2%; fc2's +42% residual
   is the f16-lane target — if the 2× traffic cut scales, AMX-f16 beats the q8 encoder
   outright. CPU-side only; predictions-first stands.

   **Both follow-ups BUILT and measured same-day (commit 897a7b57a), both opt-in:**
   - **f32→f16 fp-seam lane** (`DASLLAMA_ACCEL_F16` now serves fp32 planes, cached
     conversion): prediction MISSED — M1's hgemm is WORSE than sgemm on the fat-K shape
     (fc2 +43%, squares −8..−10%, net +6%). The M1-generation AMX f16 story, consistent
     with the lane shipping uncrowned here; the lane is the M4/SME vehicle, not an M1 win.
   - **Hybrid AMX+NEON row split** (`DASLLAMA_ACCEL_HYBRID=<neon %>`, `tw_mm` hook, AMX
     rows dequanted from the q8 blob through the active grp<mr> repack layout — transcript
     IDENTICAL to q8, one precision class): correct but 1.4–2× slower at every split. A
     token-blocked `dot_q8q8` loop is 6–8× off the generated tile kernel per row, and the
     four-pane trace shows it as 42% idle / par 4.7 — the NEON side straggles while AMX
     strips wait. **The concept waits on a row-range fork-callable tile core from the
     math_gen emitter + per-shape (tune-crownable) splits** — the "no black holes" prize
     (~1.7× combined throughput) is real but needs that sub-arc.
   - Next: the M4 leg (SME f16 + its strip-count question); the tile row-core sub-arc if
     the hybrid prize is pursued. Two compiler warts owed minimal repros: das-function
     calls inside a `maybe_parallel_for` block trip the new_job qmacro rewrite
     (30921 / "expecting lambda declaration, ExprAscend"), and `grep_usage` file
     truncation (reported in phase 5).

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
