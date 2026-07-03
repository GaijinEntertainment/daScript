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
| Kernel backend (ISA) | auto by priority; `pin_kernel_backend(name)` for A/B; loader picks repack backends; profile `runtime.backend` pins | runtime | `matmul_q8q8*` wrappers |
| Loop hints per kernel (`vectorize_width` × `unroll_count`) | `box_profile.json` flat keys, read at **compile time** by `[tuned]` | compile (JIT re-keys automatically) | all 16 `[tuned]` kernels: dot, axpy, add/mul/scale_inplace, copy_floats, softmax(+_sink), rmsnorm, dot_q4, dot_q8q8, dot_mx4q8, quantize_q8_0_into_ptr, rope_scaled_neox_tab, gemm_f32_uk_4x16, dot_q8q8_laneq4x4 |
| Token block `TB` | `set_q8_token_block(n)` (default 128) / profile `runtime.q8_token_block` | runtime | the repack-tier batch kernel only |
| L2 budget (TB cliff guard) | `set_q8_l2_budget(bytes)` (default 4 MB — provisional, one M1 Max) / profile `runtime.q8_l2_budget` | runtime | `effective_token_block(tb, n) = clamp(tb, 1, budget/n)` (dasllama_math.das) |
| Threading thresholds | `set_matmul_par_threshold`, `set_decode_attn_par_threshold` (decode attention over heads — default 262144, the measured M1 Max crossover; sweep inline-vs-threaded at a few context depths to re-derive on a new box), + the six prefill-pass setters (`set_attn/requant/norm/rope/kv_store/act_par_threshold`) / profile `runtime.*_par_threshold` | runtime | every `maybe_parallel_for` gate — the crossovers encode the box's ~90µs (M1) job-dispatch cost |
| Chunks per hw job | `set_q8_chunks_per_job` (default 2) / `set_q4_chunks_per_job` (default 4) / profile `runtime.q{8,4}_chunks_per_job` | runtime | the quantized matmuls' row split |
| Batch oversplit | `set_q8_batch_chunks_per_job` (default 4) / profile `runtime.q8_batch_chunks_per_job` — chunks = hw_jobs × this, so awake workers steal a straggler's remaining chunks (measured 3990X @32w: +9-14% on the fat shapes; the grp4 kernel floors at 4 groups/chunk — see the kernel note) | runtime | the x64 batch (prefill GEMM) kernels |
| Jobque spin | `set_jobque_spin_us` (default 30000, worker spin-before-park) / `set_jobque_join_poll` (default 50, ggml's poll denomination: level×1024×128 relax-rounds at join before parking) / profile `runtime.jobque_spin_us`, `runtime.jobque_join_poll` — both applied by `setup_dasllama_jobque` inside the queue | runtime | every fork/join dispatch |
| Threads | `DAS_JOBQUE_THREADS` env (overrides all); build-time `-DDAS_MAX_HW_JOBS=N`; profile `runtime.threads` is ADVISORY (logs a recommendation — worker count is fixed at jobque creation) | process | every `maybe_parallel_for` |

Axes are ~independent (unroll ⊥ width ⊥ TB ⊥ threads) — sweep them separately
(coordinate descent), never as a full cross-product.

**One file carries all of it.** `box_profile.json` resolves as `DASLLAMA_BOX_PROFILE` env when
set, else `<das_root>/box_profile.json` (per-install == per-box; `get_das_root()` — never the
cwd). Flat keys = the compile-time kernel perms (`[tuned]` reads them, precedence
`perm=` pin > profile > the kernel's `fallback=` > `vec8_u2`); the `"runtime"` object = the
runtime knobs above, applied by `load_model` (logged per entry) or explicitly via
`apply_box_profile_runtime(path)`. Direct `load_gguf` users opt in by calling the latter.

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

## Tool 1 — `harness/tune_kernels.das` (the box tuner → `box_profile.json`)

```
bin/daslang -jit modules/dasLLAMA/harness/tune_kernels.das
```

Sweeps every `[tuned]` kernel (15: the float elementwise set, softmax, rmsnorm, the quantized
dots, the activation requant, the NEOX rope-table leaf, the fp32 GEMM tile, and — arm64+JIT
only, clean skip elsewhere — the laneq4x4 tile) across the 20-permutation grid
(`{plain, u2, u4, u8} ∪ {vec4,vec8,vec16,vec32} × {-, u2, u4, u8}`), interleaved best-of-N
(80 rounds × 2000 reps at N=4096), correctness gate per variant (f64 reference; EXACT
quant/scale equality for the requant), reports each variant as %Δ vs that kernel's SHIPPED
fallback perm (`vec8_u2` for most; dot_q8q8 ships `vec16`, dot_q4 `vec4_u4`, the tile k-loops
`u2`, rmsnorm/requant `plain`), and writes the COMPLETE profile — flat kernel-perm keys plus a
`"runtime"` section snapshotting the current runtime knobs (hand-editable afterwards;
`softmax_sink` mirrors `softmax`'s winner — same loop shape):

```json
{"dot":"vec8_u2", "...":"...", "dot_q8q8":"vec16",
 "runtime":{"q8_token_block":128,"q8_l2_budget":4194304,"matmul_par_threshold":2000000,
            "attn_par_threshold":100000, "...":0, "q8_chunks_per_job":2,"threads":8}}
```

**How the profile is consumed:** the flat keys are read at **compile time** by `[tuned]`
(`dasllama_tune.das`), keyed by function name; precedence = explicit `perm=` pin > profile >
the kernel's `fallback=` > `vec8_u2`. The `"runtime"` section is applied at **run time** by
`load_model` (or `apply_box_profile_runtime(path)` for direct `load_gguf` users). Both sides
resolve the same path: `DASLLAMA_BOX_PROFILE` env, else `<das_root>/box_profile.json` — and
the tuner WRITES through that resolution too, failing loudly with env advice when das_root is
read-only (installed SDK). Missing file or key = silent default, so an untuned box ships the
hand-tuned M1 hints. The file is **gitignored** (per-box artifact) and any change **re-keys
the JIT DLL cache automatically** (loop hints are folded into `jit_dll_basename` — no manual
`.jitted_scripts` clearing). A consumer compile logs
`dasllama_tune: dot <- vec16 (<resolved path>)` per applied flat entry, and `load_model` logs
`dasLLAMA: box profile runtime: <key> = <v>` per applied runtime entry — that's your proof it
took.

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
marked `for [tune = 1] (...)` (several loops may share the tag — they get the same perm) and
an EXPLICIT return type (the variants registry is typed from `function_to_type(template)`; an
inferred return lands as `:auto` and won't match the clones), add a `[tuned]` reconstitution
stub `<name>` (empty body) — with `fallback="<perm>"` when the shipped hand hints aren't
`vec8_u2`, so bit-identity of the shipped default holds (that's the invariant) — then a
`[dasllama_grid(src="<name>_template")]` + bench block in `tune_kernels.das`, passing the
fallback as `report(...)`'s baseline. `tests/dasLLAMA/test_tune/test_tuned/test_grid.das` are
the patterns.

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

- **Ours:** `forward_profile_report()` buckets (embed / rope_build / rope / kv_store / attn /
  mm_qkv / mm_wo / mm_ffn / mm_moe / act / gate / final...) — decode and prefill feed the same
  accumulator, so reset the window around whichever phase you're measuring
  (`benchmarks/prefill_perf.das` drives the prefill paths; `llama3_perf.das` resets before its
  timed decode window and reports after — the decode table). Caveat: `mm_gemm`/`mm_requant`
  are inner-leaf timers of the batched matmuls and double-count against the `mm_*` site
  buckets — compare within a tier, don't sum across tiers.
- **Theirs (the recipe that found the M1 gap, now shipped):** `harness/ggml_op_profile.patch` —
  apply to llama.cpp with `git apply --ignore-whitespace` and rebuild. It times thread-0 wall
  per graph node in `ggml_graph_compute_thread` (ggml-cpu.c) into buckets NAMED LIKE OURS
  (MUL_MATs split by tensor name: Qcur/Kcur/Vcur → mm_qkv, kqv_out → mm_wo, ffn_* → mm_ffn,
  result_output → mm_cls, kq/kqv + FLASH_ATTN_EXT/SOFT_MAX → attn; SET_ROWS → kv_store;
  norm-weight MULs fold into rmsnorm), gated on `GGML_OP_PROFILE=1`, dumped to stderr at exit.
  Run `llama-bench -p 0 -n 64` for a decode table, `-p 512 -n 0` for prefill (separate
  processes — the dump is whole-process). Divide by the mm_cls node count (= tokens) for
  per-token numbers. This recipe is how "we're behind" became "we're 31% *ahead* on attention;
  the whole gap is the projection GEMM" on M1 — and on the 3990X it split the decode gap into
  ~1.17× on the repack-less quantized GEMVs + 1.66× on the small `mm_wo` — profile before optimizing.
- Fairness: end-to-end comparisons run ours with `options _jit_fast_math = true` (ggml
  hand-codes equivalent FP laxity; strict-IEEE-us vs fast-ggml understates us ~8-10%).
  Tests and oracle gates stay bit-exact — never put fast-math on a test.

---

## Suggested order for a fresh box

1. Thread sweep first (`DAS_JOBQUE_THREADS` can't be swept in-process — the worker count is
   fixed at jobque creation, so it's one process per value: `DAS_JOBQUE_THREADS=N` ×
   `bench_gemm_iso`) — establishes the parallel ceiling everything else is measured under.
   Record the winner in the profile's `runtime.threads` (advisory — `load_model` logs a
   recommendation when the live count differs).
2. `tune_kernels.das` → the complete `box_profile.json` (kernel perms + runtime snapshot) →
   recompile a consumer and confirm the `dasllama_tune:` log lines → re-run an end-to-end
   bench to see if it moved anything.
3. If a token-blocked (repack) kernel exists: `tune_tb.das` raw curve → hand-edit
   `runtime.q8_l2_budget` under the cliff; leave `q8_token_block` at 128 unless the curve
   says otherwise.
4. `prefill_perf.das` + `llama-bench` head-to-head; if a gap remains, per-op profile both
   sides (Tool 4) before touching any kernel.

**When to stop:** GEMM at isolated parity with llama.cpp is the floor, not a lever; ≤2-3%
candidates are noise; on M1 the campaign ended at ~100-108% of llama.cpp prefill with the
remaining serial inches (pack loops) judged diminishing returns. The goal of this doc is that
the x64 box reaches its *own* floor with defaults that are data (`box_profile.json`, TB,
budget, threads) — not another fork of the kernels.
