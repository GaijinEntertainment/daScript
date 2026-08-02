# Tuning dasLLAMA for this box

How to produce a per-box performance profile — which kernel backend, which loop-hint
permutation per kernel, which token-block / L2 budget, how many threads — and how to measure
without fooling yourself. The second part is the important one: **the make-or-break of any
tuner is measurement, not codegen.** Every "breakthrough" we later retracted was a
measurement artifact; every real win survived the discipline below.

Companion docs (archived in the daslang repo under `history/dasLLAMA/`): `x64_arch.md` (what's universal vs per-box), `get_x64_going.md` (bring-up —
finish it first; tuning an incorrect kernel is worse than pointless). For the tune *framework*
itself (the `[tune]`/`[tune_scope]`/`[tune_policy]` annotations, the manifest format, the mode
contract) see `doc/source/reference/tune.rst`; this doc is the dasLLAMA-specific application of
it plus the measurement discipline.

---

## Zero-config: `dasllama-server` tunes itself

The everyday path needs no manual step. `dasllama_math_gen` declares
`[tune_scope(name = "dasllama", tuner = "../harness/gen_tune_probe.das")]`, so the gen GEMM
family resolves its per-box winner from `<das_root>/dasllama.tune.json` — shared by
**every** dasLLAMA application on the box. `dasllama-server` declares
`[tune_policy(missing = "auto")]`: the first start on an untuned box runs `gen_tune_probe`,
writes the manifest, and relaunches itself with the winners; thereafter it serves directly and
logs the tune status at startup. `--tune` forces a re-tune; `DAS_TUNE_POLICY=error` skips
per-start tuning while developing (it prints the tuner command instead).

Two sibling CLI tools share the same manifest — no per-app scope declaration, since requiring
dasLLAMA pulls in its `[tune_scope]`: `ask` (prompt → completion, reporting ttft + prefill/decode
t/s) and `wav2txt` (audio → text, reporting decode/transcribe time + real-time factor). Both carry
the same `[tune_policy(missing = "auto")]`, so whichever of the three you run first tunes the box
and the rest are instant. Tune once, and every dasLLAMA app on the box is tuned.

The profiling suite drivers (`performance/gen_profile.das`, `performance/gen_asr_profile.das`)
carry the same policy — an untuned box tunes itself before the first measurement, and the
profile's platform block records the stamp provenance (`"tune"`: `fname=suffix (manifest|
fallback|reference)`), so fallback-tier numbers can never masquerade as tuned ones. (This
happened: a deleted manifest silently cost ~5% prefill across a whole published M1 sweep.)
Set `DASLLAMA_CONFIRM_MODEL=<q8 gguf>` so a divergent crown must prove itself in real prefill.
The tuner benches declined perms ONCE (they share the reference body) — a foreign-ISA grid
(arm64 skipping the x64 legs) tunes in ~1 minute, and the winner-picker no longer gets N
noise-lottery tickets for one body.

This zero-config path covers only the **gen GEMM manifest** (`gen_tune_probe`). The
`[tuned]` loop-hint kernels (the `"kernels"` section of the same `<app>.tune.json` sidecar,
below), the backend/thread/token-block runtime knobs, and the measurement discipline are still
the manual, hands-on story — read on.

---

## The tunable axes

| Axis | Knob | Bound when | Consumer |
|---|---|---|---|
| Kernel backend (ISA) | auto by priority; `pin_kernel_backend(name)` for A/B; loader picks repack backends; profile `runtime.backend` pins | runtime | `matmul_q8q8*` wrappers |
| Batch backend (hybrid) | `select_batch_backend(name)` / profile `runtime.batch_backend` / env `DASLLAMA_PIN_BATCH_BACKEND` (benches) — overrides ONLY the batch-shaped slots (prefill GEMM + mx4 batch) from a layout-compatible donor, GEMV slots stay with `runtime.backend`. For boxes where decode and prefill prefer different backends (e.g. portable GEMV + a row-major donor's batch). Survives load-time re-select | runtime | the batch matmul wrappers |
| Loop hints per kernel (`vectorize_width` × `unroll_count`) | the sidecar's `"kernels"` keys, read at **compile time** by `[tuned]` | compile (JIT re-keys automatically) | all 25 swept `[tuned]` kernels: dot, axpy, add/mul/scale_inplace, copy_floats, softmax(+_sink — mirrored, not swept separately), rmsnorm, dot_q4, dot_q8q8, dot_mx4q8, quantize_q8_0_into_ptr, quantize_q8_0_bs_into_ptr, rope_scaled_neox_tab, gemm_f32_uk_4x16, dot_q8q8_laneq4x4, the f16 codec set (dot_f16, axpy_f16, cvt_f32_to_f16, cvt_f16_to_f32), and the q8 KV-codec set (dot_q8kv, dot_q8q8kv, axpy_q8kv, cvt_q8kv_to_f32, quantize_q8kv_row) |
| Gen GEMM tile family (manifest) | `gen_tune_probe` (the `[tune_scope(name="dasllama")]` tuner — run by `dasllama-server`'s `policy=auto` / `--tune`, or by hand with `DAS_TUNE_MODE=tune`) benches the `[tune_perm]` grid and writes the winner to `<das_root>/dasllama.tune.json` — **guarded by the e2e confirm pass**: a winner that diverges from the per-ISA fallback must beat it in a real-model prefill A/B (`DASLLAMA_CONFIRM_MODEL=<q8 gguf>`, interleaved, challenger needs >×1.02, else the fallback stays pinned). The guard exists because the 1-core fixture regime CAN crown an e2e loser (SPR: amx won the tile bench, lost prefill ~2×; M1: a +3.3%-iso shape made only +2.0% e2e — rejected) | compile (stamp at `[tune]`) | `q8q8_tile_gen` + companions (the gen batch/gemv kernels) |
| K-quant tile families (manifest, per format) | the same `gen_tune_probe` tune run benches each kq family's own grid (mr × nrsplit per ISA tier) and writes `k4q8_tile_gen` / `k5q8_tile_gen` / `k6q8_tile_gen` entries — tile-best wins (the kq gemv is nrsplit-independent; same-mr rows share its plane), no child confirm pass (the stamp only moves that format's plane interleave, decode stays DRAM-bound — e2e-validated when the split landed: M1 mr4 = pp +10/+26/+22% on Q4_K_M/Q5_K_M/Q6_K, tg flat, MoE pp −3%; kq v2 lifted k4 pp512 to 0.88–0.95× lcpp on a CLEAN box (das 151–163 vs lcpp 172 — the earlier 158-vs-158 'parity' was two load-depressed reads coinciding) and MoE to 1.34×; the k5/k6 v2 port took Q5/Q6 pp to 0.84×/0.80×; kq v3 (panel-scratch byte-expanded tiles, 2026-07-12 PM) then took Q5 pp to 150–168 (1.14–1.28× lcpp) and Q6 to 140–143 (1.01–1.03×), tg at v2 parity — the tile reads a byte panel unpacked per (group, token-block) while the DRAM planes stay packed (decode is model-level DRAM-bound: pure byte planes cost tg −30%). Iso (M1): tile k4 95 / k5 89 / k6 76 GMAC/s at the probe's ×16 amortization, gemv 64 / 27.5 / 34.5 unchanged) | compile (stamp at `[tune]`) | `k{4,5,6}q8_tile_gen` + their gemv/layout companions (each format's planes take their OWN mr) |
| Token block `TB` | `set_q8_token_block(n)` (default 128) / profile `runtime.q8_token_block` | runtime | the repack-tier batch kernel only |
| L2 budget (TB cliff guard) | `set_q8_l2_budget(bytes)` (default 4 MB — provisional, one M1 Max) / profile `runtime.q8_l2_budget` | runtime | `effective_token_block(tb, n) = clamp(tb, 1, budget/n)` (dasllama_math.das) |
| Work-proportional dispatch | `set_target_chunk_work` (MACs per dispatch chunk, default 1 = dispatch everything the grain floors admit; lanes = clamp(work/target, 1, lanes); the pre-2026-07 default was 1M, which left every per-layer attn matmul of a ≤4B model single-lane) + `set_gemv_chunks_per_lane` (default 8 — fine-grain self-serve chunks per lane; 1 = the old equal split) + `set_gemv_lane_cap` / `set_batch_lane_cap` (per-regime lane ceilings, 0 = uncapped — the GEMV cap is the box's DRAM knee, measured by `harness/team_probe.das`) / profile `runtime.target_chunk_work` / `runtime.gemv_chunks_per_lane` / `runtime.gemv_lane_cap` / `runtime.batch_lane_cap` | runtime | every matmul dispatch (the `matmul_chunks*` shapers), the fused-chain gates, decode attention |
| Worker-pool limit | `set_dispatch_worker_limit(n)` (0 = no limit) / profile `runtime.dispatch_worker_limit` — workers above the limit sleep DORMANT (out of the spin loop AND the publish wake gate; `get_dispatch_lanes()` sizes shapers to the live pool). The tiny-model dial: an inline-dominated decode token runs faster with fewer awake workers, and no chunk shaping recovers that (zen2 135M: T=8 176 t/s vs T=48 149) | runtime (applied by `setup_dasllama_jobque_`) | the whole jobque worker pool |
| Per-op team rank gate | `set_team_rank_gate(v)` tri-state (-1 = unset/que default, 0 = off, 1 = on) / profile `runtime.team_rank_gate` — each team op admits only as many workers as its widest stage published chunks; gated ranks keep spinning, re-admitted by the next big op. Composes with full lanes (unlike the static limit): 135M T=48 emit +10% zen2 / +48% SPR, ≥1B neutral. ON for server-core boxes; heterogeneous P/E boxes (M1) keep it off — rank-static admission wobbles when low ranks land on slow cores. `DAS_JOBQUE_TEAM_RANK_GATE` env always wins over the profile (the A/B rail) | runtime (applied by `setup_dasllama_jobque_`) | every team dispatch |
| 2-D batch chunk grid | `set_batch_grid_2d(v)` (0 = off/1-D, 1 = fine grid — ggml's 16-token cells, 2 = wave-aligned — token chunks rounded so rc·ntc is whole L-waves) / profile `runtime.batch_grid_2d` / env `DASLLAMA_BATCH_GRID_2D` (benches + parity, the A/B rail). The knob ARMS a per-dispatch auto-gate: 2-D engages only when the 1-D grain-capped chunk count starves the admitted lanes (rows/16 < L), so big GEMMs keep the weight-stationary 1-D walk. Bit-exact both ways. High-lane boxes only: zen2 T=48 135M pp512 +8-15% (aligned > fine every rep), 1B +3-4%, unstarved shapes untouched; M1-class (≤11 lanes) can never engage — keep 0 | runtime | the repack-tier batch GEMM (gen ts=4 walk) |
| Threading thresholds (non-matmul) | the six prefill-pass setters (`set_attn/requant/norm/rope/kv_store/act_par_threshold`) / profile `runtime.*_par_threshold` | runtime | the norm/rope/kv/act/requant `maybe_parallel_for` gates — the crossovers encode the box's job-dispatch cost |
| Chunks per hw job | `set_q8_chunks_per_job` (default 2) / `set_q4_chunks_per_job` (default 4) / profile `runtime.q{8,4}_chunks_per_job` | runtime | the quantized matmuls' row split |
| Batch oversplit | `set_q8_batch_chunks_per_job` (default 4) / profile `runtime.q8_batch_chunks_per_job` — chunks = hw_jobs × this, so awake workers steal a straggler's remaining chunks (measured 3990X @32w: +9-14% on the fat shapes; the grp4 kernel floors at 4 groups/chunk — see the kernel note) | runtime | the x64 batch (prefill GEMM) kernels |
| Jobque spin | `set_jobque_spin_us` (default 30000, worker spin-before-park) / `set_jobque_join_poll` (default 50, ggml's poll denomination: level×1024×128 relax-rounds at join before parking) / profile `runtime.jobque_spin_us`, `runtime.jobque_join_poll` — both applied by `setup_dasllama_jobque` inside the queue | runtime | every fork/join dispatch |
| Threads | `DAS_JOBQUE_THREADS` env (overrides all); build-time `-DDAS_MAX_HW_JOBS=N`; profile `runtime.threads` is ADVISORY (logs a recommendation — worker count is fixed at jobque creation) | process | every `maybe_parallel_for` |

Axes are ~independent (unroll ⊥ width ⊥ TB ⊥ threads) — sweep them separately
(coordinate descent), never as a full cross-product.

**One file carries all of it — the PER-APP tune sidecar.** `box_profile_path()` now resolves
to `<app>.tune.json` beside the app (the root script the process runs, else the binary;
`DAS_TUNE_MANIFEST` env overrides) — shared with the `[tune]` generator winners. The
`"kernels"` section = the compile-time kernel perms (`[tuned]` reads them, precedence
`perm=` pin > sidecar > the kernel's `fallback=` chain (per-ISA) > `vec8_u2`; the `[tune]` families' perms
live in the same map); the `"runtime"` object = the runtime knobs above, applied by
`load_model` (logged per entry) or explicitly via `apply_box_profile_runtime(path)`. Direct
`load_gguf` users opt in by calling the latter. A sidecar OLDER than the running binary is
stale — every reader treats it as absent, and the next tuner write resets it.

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
   confirm kernel wins end-to-end (`benchmarks/lcpp_bench.das`), and profile per-op before
   believing a bottleneck story (see the ggml patch below).
6. **Quiet machine for absolute numbers** — no remote-desktop/streaming session, cool box.
   CPU wall-clock is immune to GPU theft but not to CPU theft or thermal throttling.
7. **Never benchmark interpreted — always `-jit`.** If a number looks ~100× off, check the flag
   first. The benches self-skip off-JIT and say so; heed the message rather than reading the
   fallback body's throughput as a result.
8. **ISA-pinned harnesses skip off their arch** (`tune_tb.das` pattern) — a clean skip, not a
   zero. Gate any new arch-specific bench the same way so the other box skips instead of
   silently measuring the portable fallback.

---

## Tool 1 — `harness/tune_kernels.das` (the loop-hint tuner → the app sidecar)

```
bin/daslang -jit modules/dasLLAMA/harness/tune_kernels.das

# paranoid: 3x the finalist budget, 1% noise-gate ceiling, tighter validation
bin/daslang -jit modules/dasLLAMA/harness/tune_kernels.das -- --tune-paranoid

# full generator + kernel scope (also available as `daspkg release [--paranoid]`)
bin/daslang -jit modules/dasLLAMA/harness/dasllama_tuner.das
```

Sweeps every `[tuned]` kernel (25 — `TUNED_KERNEL_COUNT`, one `[dasllama_grid]` each: the float
elementwise set, softmax, rmsnorm, the quantized dots, the two activation requants, the NEOX
rope-table leaf, the fp32 GEMM tile, the f16 codec set, the q8 KV-codec set, and — arm64+JIT
only, clean skip elsewhere — the laneq4x4 tile) across the 20-permutation grid
(`{plain, u2, u4, u8} ∪ {vec4,vec8,vec16,vec32} × {-, u2, u4, u8}`), interleaved best-of-N.
The default screens the full valid grid for 20 rounds, retains the four fastest rows plus
anything within 5% of the leader, and continues those finalists to at most 80 rounds;
`--tune-paranoid` extends the finalists to 240. There is no fast RACE mode — short races
cannot resolve sub-2% twins (measured: 10/34 winner flips between back-to-back fast mints);
the debugging concession is accepting an existing sidecar, not racing cheaply. Every mode
runs one unscored warm-up pass and rotates row order each round; finalists are ranked by the
MEDIAN of their finalist rounds (best-of prints alongside), a winner must beat the shipped
fallback by more than the measured noise floor, and ties inside the floor break
deterministically (baseline first, then grid order). Noise gates probe the box at
start/mid/end — a failing gate refuses to tune, and a failing end gate writes nothing. After
the sweep, a five-kernel subset re-races twice and the winners must reproduce or the mint
fails. The measurement thread is hard-pinned by its child-process JobQue (QoS-pinned on
macOS, which has no core masks) and is automatically unpinned when that tuner child exits.
Each round runs 2000 reps at N=4096. The correctness
gate per variant (f64 reference; EXACT
quant/scale equality for the requant), reports each variant as %Δ vs that kernel's SHIPPED
fallback perm (`vec8_u2` for most; dot_q8q8 ships `vec16`, dot_q4 `vec4_u4`, the tile k-loops
`u2`, rmsnorm/requant `plain`), and UPSERTS the winners into the app sidecar's `"kernels"`
section plus a `"runtime"` section snapshotting the current runtime knobs (hand-editable
afterwards; everything else in the sidecar — the `[tune]` generator winners — survives;
`softmax_sink` mirrors `softmax`'s winner — same loop shape):

```json
{"kernels":{"dot":"vec8_u2", "...":"...", "dot_q8q8":"vec16"},
 "runtime":{"q8_token_block":128,"q8_l2_budget":4194304,"target_chunk_work":1000000,
            "attn_par_threshold":100000, "...":0, "q8_chunks_per_job":2,"threads":8}}
```

**How the sidecar is consumed:** the `"kernels"` entries are read at **compile time** by
`[tuned]` (`dasllama_tune.das`), keyed by function name; precedence = explicit `perm=` pin >
sidecar > the kernel's `fallback=` > `vec8_u2`. The `"runtime"` section is applied at
**run time** by `load_model` (or `apply_box_profile_runtime(path)` for direct `load_gguf`
users). Both sides resolve the same path — the app sidecar (`tune_manifest_path()`:
`DAS_TUNE_MANIFEST` env, else `<app>.tune.json` beside the app) — and the tuner WRITES
through that resolution too, failing loudly with env advice when the app dir is read-only.
NOTE the per-app consequence: the tuner writes the sidecar of the app it RUNS AS
(`tune_kernels.tune.json` beside the harness) — to tune another app, point
`DAS_TUNE_MANIFEST` at that app's sidecar. Missing/stale file or key = silent default, so an
untuned box ships the hand-tuned M1 hints. The file is **gitignored** (per-app, per-box
artifact) and any change **re-keys the JIT DLL cache automatically** (loop hints are folded
into `jit_dll_basename` — no manual `.jitted_scripts` clearing). A consumer compile logs
`dasllama_tune: dot <- vec16 (<resolved path>)` per applied entry, and `load_model` logs
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
`vec8_u2`, so bit-identity of the shipped default holds (that's the invariant). `fallback=` is
a `;`-chain of `suffix` or `suffix:requires` entries, first passing entry wins, so ONE
annotation can ship a different default per ISA (`fallback="vec8_u8:dotprod;plain"`); a bare
name is just a one-entry chain. Then a
`[dasllama_grid(src="<name>_template")]` + bench block in `tune_kernels.das`, passing the
fallback as `report(...)`'s baseline. `modules/dasLLAMA/tests/test_tune/test_tuned/test_grid.das` are
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

**On x64:** the harness pins the gen repack tier (`arm64-gen` / `x64-gen`) and skips
cleanly where it isn't registered. Re-derive the budget
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
other models. This is the kernel scoreboard; `benchmarks/lcpp_bench.das` is the end-to-end truth.

## Tool 4 — per-op profiling (ours and theirs)

- **Ours:** `forward_profile_report()` buckets (embed / rope_build / rope / kv_store / attn /
  mm_qkv / mm_wo / mm_ffn / mm_moe / act / gate / final...) — decode and prefill feed the same
  accumulator, so reset the window around whichever phase you're measuring
  (`benchmarks/lcpp_bench.das` drives both: its pp reps exercise the prefill paths and its tg
  reps the decode path; archived single-phase drivers live in `history/dasLLAMA/benchmarks/`).
  Caveat: `mm_gemm`/`mm_requant`
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
- **Steady-state, not best-of (M1/Apple Silicon; trace-diagnosed 2026-07-12):** a
  first-run-after-idle prefill rides a ~3 s P-cluster boost window (+11% — one DVFS step,
  uniform per-chunk, all lanes; NOT E-core placement, NOT jobque — lane traces show 98–99%
  utilization in both modes). Back-to-back reps sit at the sustained clock at llama-bench
  stability (±0.6%), and llama-bench numbers ARE steady-state. Report the MEDIAN of ≥3
  back-to-back reps and discard the first-after-idle rep — best-of-N systematically picks
  the boost outlier.

---

## Suggested order for a fresh box

1. Thread sweep first (`DAS_JOBQUE_THREADS` can't be swept in-process — the worker count is
   fixed at jobque creation, so it's one process per value: `DAS_JOBQUE_THREADS=N` ×
   `bench_gemm_iso`) — establishes the parallel ceiling everything else is measured under.
   Record the winner in the profile's `runtime.threads` (advisory — `load_model` logs a
   recommendation when the live count differs).
2. `tune_kernels.das` → the complete `<app>.tune.json` sidecar (kernel perms + runtime snapshot) →
   recompile a consumer and confirm the `dasllama_tune:` log lines → re-run an end-to-end
   bench to see if it moved anything. **A "neutral" verdict here is only valid per-backend:
   `[tuned]` perms bite ONLY on the portable-tier kernels, so an intrinsic auto-backend
   (e.g. `x64-gen`) masks the profile entirely. Run the backend ladder
   (`DASLLAMA_PIN_BACKEND` per rung, profile ON, bracketed controls) before concluding —
   on the EPYC 9654 the profile was "neutral" under auto but +34% on portable
   (77→104 t/s), flipping portable from worst backend to best and beating auto by ~15%.
   Record the in-model winner in the profile's `runtime.backend` pin.
3. If a token-blocked (repack) kernel exists: `tune_tb.das` raw curve → hand-edit
   `runtime.q8_l2_budget` under the cliff; leave `q8_token_block` at 128 unless the curve
   says otherwise.
4. `benchmarks/lcpp_bench.das --ref <llama-bench>` head-to-head (same shapes, one run); if a
   gap remains, per-op profile both sides (Tool 4) before touching any kernel.

**When to stop:** GEMM at isolated parity with llama.cpp is the floor, not a lever; ≤2-3%
candidates are noise; on M1 the campaign ended at ~100-108% of llama.cpp prefill with the
remaining serial inches (pack loops) judged diminishing returns. The goal of this doc is that
the x64 box reaches its *own* floor with defaults that are data (the `<app>.tune.json` sidecar,
TB, budget, threads) — not another fork of the kernels.
