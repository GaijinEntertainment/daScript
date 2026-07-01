# Tuning dasLLAMA for this box

How to produce a per-box performance profile — which kernel backend, which loop-hint
permutation per kernel, which token-block / L2 budget, how many threads — and how to measure
without fooling yourself. The second part is the important one: **the make-or-break of any
tuner is measurement, not codegen.** Every "breakthrough" we later retracted was a
measurement artifact; every real win survived the discipline below.

Companion docs: `x64_arch.md` (what's universal vs per-box), `get_x64_going.md` (bring-up —
finish it first; tuning an incorrect kernel is worse than pointless).

---

## The tunable axes

| Axis | Knob | Bound when | Consumer |
|---|---|---|---|
| Kernel backend (ISA) | auto by priority; `pin_kernel_backend(name)` for A/B; loader picks repack backends | runtime | `matmul_q8q8*` wrappers |
| Loop hints per kernel (`vectorize_width` × `unroll_count`) | `box_profile.json`, read at **compile time** by `[tuned]` | compile (JIT re-keys automatically) | the 6 migrated float kernels (dot, axpy, add/mul/scale_inplace, copy_floats) |
| Token block `TB` | `set_q8_token_block(n)` (default 128) | runtime | the repack-tier batch kernel only |
| L2 budget (TB cliff guard) | `set_q8_l2_budget(bytes)` (default 4 MB — provisional, one M1 Max) | runtime | `effective_token_block(tb, n) = clamp(tb, 1, budget/n)` (`dasllama_math.das:301`) |
| Threads | `DAS_JOBQUE_THREADS` env (overrides all); build-time `-DDAS_MAX_HW_JOBS=N` | process | every `maybe_parallel_for` |

Axes are ~independent (unroll ⊥ width ⊥ TB ⊥ threads) — sweep them separately
(coordinate descent), never as a full cross-product.

---

## Measurement discipline (read before recording a single number)

1. **Interleaved A/B in ONE process.** Rotate candidates round-robin, best-of-N per candidate,
   never sequential blocks. Sequential runs confound the comparison with thermal drift and
   background load. (History: an 886→695 GMAC/s "regression" was a remote-desktop agent
   stealing CPU, not code. Interleaving is robust to both; absolute numbers are not.)
2. **Correctness-gate EVERY candidate before timing it** — against an f64 scalar reference
   (kernels) or bit-identity (pure reschedules). Fast-but-wrong is the worst outcome, and a
   miscompiled variant is often *fast*.
3. **Best-of-N + confirm the winner** with a re-measure. A single measurement picks the
   noise-luckiest candidate, not the best one.
4. **Report deltas vs the shipped baseline, and treat ≤2-3% as the noise floor.** On M1 a
   real 2% (vec16 register pressure) and noise 2% are indistinguishable without repetition
   across independent runs. Don't bake in a win you can't reproduce three times.
5. **Isolated-kernel parity ≠ in-pipeline parity.** The M1 GEMM benched at parity isolated
   (ntok=512, cache-resident) while trailing 18% in-pipeline (ntok=2048, DRAM-bound). Always
   confirm kernel wins end-to-end (`prefill_perf.das`), and profile per-op before believing a
   bottleneck story (see the ggml patch below).
6. **Quiet machine for absolute numbers** — no remote-desktop/streaming session, cool box.
   CPU wall-clock is immune to GPU theft but not to CPU theft or thermal throttling.

---

## Tool 1 — `harness/tune_kernels.das` (loop-hint sweep → `box_profile.json`)

```
bin/daslang -jit modules/dasLLAMA/harness/tune_kernels.das      # run from the repo root
```

Sweeps all 6 migrated float kernels across the 20-permutation grid
(`{plain, u2, u4, u8} ∪ {vec4,vec8,vec16,vec32} × {-, u2, u4, u8}`), interleaved best-of-N
(80 rounds × 2000 reps at N=4096), f64 correctness gate per variant, reports each variant as
%Δ vs the shipped `vec8_u2` baseline, and writes the full profile:

```json
{"dot":"vec8_u2","axpy":"vec8_u2","add_inplace":"vec8_u2",
 "mul_inplace":"vec8_u2","scale_inplace":"vec16_u4","copy_floats":"vec8_u2"}
```

**How the profile is consumed:** `[tuned]` (`dasllama_tune.das`) reads `box_profile.json` at
**compile time** (cwd-relative — compile from the repo root), keyed by function name;
precedence = explicit `perm=` pin > profile > `DEFAULT_PERM` (`vec8_u2`). Missing file or key
= silent default, so an untuned box ships the hand-tuned M1 hints. The file is **gitignored**
(per-box artifact) and any change **re-keys the JIT DLL cache automatically** (loop hints are
folded into `jit_dll_basename` — no manual `.jitted_scripts` clearing). A consumer compile
logs `dasllama_tune: dot <- vec16 (box_profile.json)` per applied entry — that's your proof
it took.

**Interpreting the sweep** (M1 Max reference, so you know a healthy result when you see it):

- width-8 was confirmed optimal *everywhere* on NEON (128-bit regs) — the sweep merely
  rediscovered the hand default. On AVX-512 (512-bit regs) expect `vec16`/`vec32` to enter
  the frame — that's why they're in the grid.
- FMA-bound kernels (dot/axpy/add/mul) showed a *repeatable, monotonic* preference for more
  unroll (u4-u8 > u2 — accumulators hiding FMA latency) but at ≤2% = noise floor → **not**
  baked in. That's the honest outcome shape: physically coherent, too small to matter.
- Bandwidth-bound kernels (`copy_floats`) are width/unroll **don't-cares** — the winner jumps
  randomly between runs. Don't chase it.

**Adding a kernel to the rig:** make it a `def template <name>_template` with the hot loop
marked `for [tune = 1] (...)`, add a bare-`[tuned]` reconstitution stub `<name>` (empty body),
keep the current hand hints as a grid permutation (bit-identity of the shipped default is the
invariant), then add a `[dasllama_grid(src="<name>_template")]` + bench block in
`tune_kernels.das`. `tests/dasLLAMA/test_tune/test_tuned/test_grid.das` are the patterns.

## Tool 2 — `harness/tune_tb.das` (token-block sweep)

```
bin/daslang -jit modules/dasLLAMA/harness/tune_tb.das
```

Pins the repack backend, repacks, sweeps `TB ∈ {128..2048}` × output-width `n`, interleaved,
with a **bit-identity gate** (TB only reschedules the token loop — any diff is a scheduling
bug, so the gate is exact equality, not tolerance). It calls `set_q8_l2_budget(1 << 40)` to
disable the production clamp and measure the *raw* curve. Report-only — it does not write the
profile.

**The physics (measured, and it surprised us):** TB matters only when the full activation
(`ntok × n` floats) spills the cache that serves re-streaming; the optimum is **not** a peak
at L2/n but a **cliff** — TB* sits at the small end (128) for every realistic `n`, and large
TB falls off hard once `TB × n` spills (+13% → +46%); the knee moves down as `n` grows.
`d` doesn't move TB*, only the margin. Hence the shipped design: default TB=128 + the
`effective_token_block` guard as pure insurance (inert at defaults), **not** a per-shape TB
map (there was ~0% to capture).

**On x64:** the harness pins `arm64-laneq` and skips cleanly elsewhere — when an x64
token-blocked kernel exists, generalize the pin (or copy the harness). Re-derive the budget
from the *measured* cliff, not the spec sheet: x64 cache hierarchy differs in kind (small
private L2 per core + large shared L3, vs M1's big shared L2), so which level bounds the
re-streamed slice is an empirical question. Expect the cliff shape, find its knee, set
`set_q8_l2_budget` under it, done.

## Tool 3 — `benchmarks/matmul/bench_gemm_iso.das` (isolated GEMM ledger)

```
bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/matmul/bench_gemm_iso.das
```

Production `matmul_q8q8_batch` on the real Llama shapes (qo 2048×2048, w13 2048×5632,
w2 5632×2048, cls 2048×32000) at ntok=512, threaded, GMAC/s aggregate + per-core, both
backend tiers. Sweep threads via `DAS_JOBQUE_THREADS=1,2,4,...`. Retarget the shape table for
other models. This is the kernel scoreboard; `prefill_perf.das` is the end-to-end truth.

## Tool 4 — per-op profiling (ours and theirs)

- **Ours:** `prefill_profile_report()` buckets (embed / rope_build / rope / kv_store / attn /
  mm_qkv / mm_wo / mm_ffn / act / gate / final...) — `benchmarks/prefill_perf.das` drives
  them. Caveat: `mm_gemm`/`mm_requant` are inner-leaf timers of ALL matmuls and double-count
  against the `mm_*` site buckets — compare within a tier, don't sum across tiers.
- **Theirs (the recipe that found the M1 gap):** patch llama.cpp's
  `ggml_graph_compute_thread` (ggml-cpu.c) to accumulate thread-0 wall time per `node->op`
  into a static table dumped at exit → a MUL_MAT / FLASH_ATTN_EXT / ROPE / ... breakdown to
  set against our buckets. ~15 lines, revert after. This is how "we're behind" became "we're
  31% *ahead* on attention; the whole gap is the projection GEMM" — profile before optimizing.
- Fairness: end-to-end comparisons run ours with `options _jit_fast_math = true` (ggml
  hand-codes equivalent FP laxity; strict-IEEE-us vs fast-ggml understates us ~8-10%).
  Tests and oracle gates stay bit-exact — never put fast-math on a test.

---

## Suggested order for a fresh box

1. Thread sweep first (`DAS_JOBQUE_THREADS` × `bench_gemm_iso`) — establishes the parallel
   ceiling everything else is measured under.
2. `tune_kernels.das` → `box_profile.json` → recompile a consumer and confirm the
   `dasllama_tune:` log lines → re-run an end-to-end bench to see if it moved anything.
3. If a token-blocked (repack) kernel exists: `tune_tb.das` raw curve → set budget under the
   cliff; leave TB at 128 unless the curve says otherwise.
4. `prefill_perf.das` + `llama-bench` head-to-head; if a gap remains, per-op profile both
   sides (Tool 4) before touching any kernel.

**When to stop:** GEMM at isolated parity with llama.cpp is the floor, not a lever; ≤2-3%
candidates are noise; on M1 the campaign ended at ~100-108% of llama.cpp prefill with the
remaining serial inches (pack loops) judged diminishing returns. The goal of this doc is that
the x64 box reaches its *own* floor with defaults that are data (`box_profile.json`, TB,
budget, threads) — not another fork of the kernels.
