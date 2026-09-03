# The verify-cost arc - why a speculative verify row costs what it costs

Branch `bbatkin/verify-cost`. The MTP arc (PR 3928) left one number unexplained: llama.cpp's
four-row verify costs about 1.05 plain steps while ours costs 0.76 to 1.28 steps PER ROW
(`PERF_LEDGER.md`, the MTP section). The research report on the pinned llama.cpp build
(`~/.claude/plans/mtp-research/verify_cost_llamacpp.md`, 2026-09-03) says their number is kernel
selection: small-batch kernels that dot one weight read against several activation columns, so the
weight bytes per step do not grow with the row count until M = 5 (Metal) or M = 4 (CPU). This arc
measures where OUR extra row goes, per stage, then changes the kernels the split names. Metal
first, the CPU after.

## Rules

- A number that decides a change comes from one process with the arms interleaved
  (`benchmarks/REVIEW.md`); the records-grade receipts at the end come from the ruler
  (`performance/records/mtp/`), never from the dev rail.
- Every stage has a prediction written here BEFORE its run; the run either confirms it or the
  plan says what the prediction missed.
- A kernel change lands with its kernel gate (CPU-reference parity, negative-controlled) and a
  per-box race; crowns re-mint on both boxes before any receipt.

## S0 - the per-stage split of the k-row verify (Metal)

Instrument: `harness/batch_rows_probe.das --sameslab --knockouts` - the same-slab step at B rows
under the Metal decode knockout rail (`set_metal_decode_skip`: `gemv`, `attn`, `ew`, `moe_rt`),
arms interleaved per row count in one process; the stage cost is `full - knockout`. B = 1 is the
plain step through the single-row driver, B >= 2 the same-slab verify through the batch driver;
both drivers honor the same knockout names.

Predictions (M5 Max, 512-token context, ms/step as a fraction of the B = 1 full step):

The batch rail declines hybrid graphs, so the dense carrier here is gemma-4-12B Q4_K_M (its
ladder: 1 / 2 / 3 / 4 / 5 / 8 rows = 1.00 / 0.83 / 1.19 / 1.19 / 2.03 / 2.06 on the M5); the
hybrid Qwen3.8-27B verify runs only through the NextN round and gets its own split (S0b) through
the round's profiler sections.

| model | stage | B=1 | B=2 | B=4 | B=8 |
|---|---|---|---|---|---|
| gemma-4-12B Q4_K_M (dense, twin crowned) | gemv | 0.80 | 0.63 | 0.85 | 1.60 |
| | attn | 0.05 | 0.10 | 0.20 | 0.40 |
| | ew + residual | 0.15 | 0.10 | 0.14 | 0.06 |
| | **total** | 1.00 | 0.83 (measured) | 1.19 (measured) | 2.06 (measured) |
| gemma-4-26B-A4B Q4_K_M (MoE) | gemv (experts inside) | 0.85 | 1.35 | 2.6 | 5.0 |
| | attn | 0.05 | 0.10 | 0.20 | 0.40 |
| | ew + residual | 0.10 | 0.12 | 0.15 | 0.25 |
| | **total** | 1.00 | 1.45 (measured) | ~3.0 | ~5.7 |

The claim under test: on the dense carrier the weights stream once through B = 4 and the row
cost is attention plus elementwise; on the MoE carrier the routed experts re-stream per row
(`enc_moe_gemv` has no gather), and that alone is the 1.45x.

**S0 results, gemma-4-12B on the M5 Max (2026-09-03, fresh mint 866a03046, `-jit`, 16 steps,
one process per row count):**

| rows through the batch driver | 1 | 2 | 3 | 4 | 5 | 8 | 9 |
|---|---|---|---|---|---|---|---|
| ms/step | 15.44 | 17.38 | 24.61 | 24.48 | 42.16 | 42.71 | 83.64 |
| x the one-row step | 1.00 | 1.13 | 1.59 | 1.59 | 2.73 | 2.77 | 5.41 |
| marginal row | - | +0.13 | +0.47 | +0.00 | +1.15 | +0.00 (x3) | +2.64 |

Nine rows is the batch driver's kq mul_mm rail (`enc_kq_site_b` at `nrows > 8`, M padded to 32):
one weight stream, yet twice the eight-row step - the prefill GEMM at M = 32 runs one 32x64 tile
per threadgroup and cannot fill the part. The verify's own nine-row route (the eight-row form plus
one single pass, S1) is predicted at ~55 ms on the same ladder, so past eight rows the batch
driver's rule is the wrong one on this box (S2 #2b). Sixteen rows decline in `verify_batch_step`
(the same-slab buffers are `MTP_MAX_ROWS`-sized); the probe stops at nine.

The plain step through the single-row driver reads 20.79 ms beside the batch driver's 15.44 ms
one-row step - the same weights, the same box: the single-row driver carries 5.3 ms (26%) the
batch driver does not. That is a plain-decode lead in its own right (S4 below) and it is why the
earlier ladders read "two rows cheaper than one" - the denominators were two different drivers.

Row cost is quantized by the kernel form, not by bytes: rows 1-2 ride the two-column twin
(+13% for the second row), rows 3-4 the four-column form (+59% over one row, the fourth row
free), rows 5-8 a second four-column dispatch (+115%, then free). A form that streams the
weights once still pays for its extra columns in ALU: the twin's second column costs 13%, the
four-column form's third 47%. llama.cpp's M = 4 point (~1.05x) says the four-column form's cost
is ours to remove, not physics.

Knockout deltas, one process per row count (ms; `full - knockout`):

| rows | full | gemv | attn | ew | host + sync (residual) |
|---|---|---|---|---|---|
| 1 | 15.44 | 12.17 | 0.35 | 1.13 | 1.8 |
| 2 | 17.35 | 13.92 | 0.27 | 0.66 | 2.5 |
| 4 | 24.53 | 20.46 | ~0 | ~0 | 4.1 |
| 8 | 42.73 | 36.55 | 1.0 | noise | ~5 |

The gemv stage carries the whole ladder (1.00 / 1.14 / 1.68 / 3.00 of its one-row cost);
attention and elementwise are under a millisecond at this context; the residual grows slowly
with rows. At eight rows the run-to-run noise reaches 13% (a no-op knockout read 48.2 against
42.7), so eight-row deltas are direction only.

Prediction verdict: "gemv flat through B = 4" was wrong (the four-column form is 1.59x);
"attention plus elementwise under 10%" holds on the first pass (attn 0.05-0.2, ew 0.05-0.1 of a
step, within the knockouts' noise).

Instrument finding: knockout arms leave the process slower for the row counts measured after
them (the same-slab step read 26.25 ms after a knocked-out B = 1 pass, 17.38 ms clean), so a
knockout split runs one row count per process; the full arm always runs first. The mechanism
(a latch the knockout leaves armed, or Metal objects it leaks) is ledgered, not chased here.

## S4 - the plain step's 26% (found by S0)

The batch driver's one-row same-slab step (15.44 ms) beats the single-row decode driver's step
(20.79 ms) on gemma-4-12B by 26%. Prediction: the gap is host-side per-step work in the
single-row driver (the pipelined step's wait, the logits readback, the CPU argmax between
steps), not kernels - the two drivers dispatch the same GEMV forms at one row. Instrument: the
`mtp.*`-style profiler sections on both drivers' one-row step, or knockouts of the readback.
If it holds, plain decode on every Metal box is 20-25% off the table.

Mechanism (read from the driver, `metal_decode_forward`): the single-row driver pre-encodes the
NEXT step chained on the GPU's greedy argmax (`r.spec`), commits it before the current step
drains, and on a miss (the caller's token is not the argmax) discards the chained step and reruns
the whole step through the slow path, then backs the chain off exponentially (cooldown 1, 2, 4
... 256 steps) and re-arms on a hit. The probe feeds real-text tokens, so the chain misses; the
production greedy bench hits every step and reads 63.7 tok/s = 15.7 ms on this model (the m5
records), the batch driver's one-row number. The gap is therefore the cost of a mispredicted
chain, not of the driver's kernels - and it is exactly what a SAMPLED decode (temperature above
zero) pays whenever the sampled token is not the argmax. Instrument: the probe's `--feed
text|greedy` and `--spec -1|0|1` arms, one process each; the split is miss cost (text vs greedy
under the adaptive chain) and commit bubble (chain off vs forced-on under a greedy feed).

**S4 results, gemma-4-12B on the M5 Max (one process per arm, 32 steps; the `second:K` feeds 60):**

| feed | chain | ms/step | chain hits / misses |
|---|---|---|---|
| greedy (argmax every step) | adaptive | 14.87 | 33 / 0 |
| greedy | forced | 14.87 | 33 / 0 |
| text (real tokens) | off | 15.20 | - |
| text | adaptive | 18.97 | 0 / 5 |
| runner-up every 10th step | off | 15.40 | - |
| runner-up every 10th step | adaptive | 18.17 | 49 / 6 |
| runner-up every 3rd step | off | 15.93 | - |
| runner-up every 3rd step | adaptive | 26.02 | 21 / 20 |
| runner-up every 3rd step | forced | 25.60 | 41 / 20 |

The chain's whole upside is the commit turnaround: 0.33 ms, 2.2% of the step. A miss costs
about 30 ms - two steps: the chained step is waited out before the rerun. The adaptive policy
re-arms on every hit, so under a sampler that agrees with the argmax 90% of the time it costs
18%, at 67% agreement 63%. Any decode at temperature above zero on Metal has been paying this;
the greedy bench never saw it. Fix (commit pending): `sample_` marks the session `sampled` when it
draws at temp > 0 and `pre_encode_next` never chains such a session (`ARCHITECTURE_GPU_MTP.md`
sec.2.38); arm `arm15-spec-hint` of the decode parity suite holds it (negative control: without
the driver's check the sampled half reports 23 hits). The plain-decode lead the prediction named
does not exist: the greedy step already equals the batch driver's one-row step.

**S0 results, gemma-4-26B-A4B (MoE) on the M5 Max, same run conditions, one process per row
count (ms):**

| rows | full | x one-row | gemv | of which routed experts (`moe_rt`) | dense gemv (gemv - experts) | attn | ew | residual |
|---|---|---|---|---|---|---|---|---|
| 1 | 9.25 | 1.00 | 6.50 | 1.75 | 4.75 | 0.18 | 0.91 | 1.7 |
| 2 | 10.42 | 1.13 | 7.52 | 2.76 | 4.76 | ~0 | 0.52 | 2.4 |
| 4 | 12.80 | 1.38 | 9.71 | 4.73 | 4.98 | ~0 | 0.11 | 3.0 |
| 8 | 21.94 | 2.37 | 16.98 | 8.78 | 8.20 | ~0 | 0.18 | 4.8 |

The plain step through the single-row driver: 12.20 ms (the batch driver's one-row step is 24%
cheaper here too). The dense weights stream once through four rows (4.75 / 4.76 / 4.98); the
routed experts scale with the rows (1.75 / 2.76 / 4.73 / 8.78 = 1.0 / 1.6 / 2.7 / 5.0 - sub-linear
only by cache hits between rows that chose the same expert). Prediction confirmed: the expert
re-streaming is the whole marginal row cost through four rows; at eight the dense four-column
second dispatch joins it. S2's first kernel is the expert gather.

## S1 - the depth-8 rail hole (a failing test first)

`MTP_MAX_ROWS = 9` and the round clamps k to 8, so depth 8 verifies 9 rows. Found red by
`mtp-count8-3.8-27b`: the Qwen3.8-27B round at depth 8 decodes garbage from the third token. Two
holes, both silent: the K-quant site handed the mul_mm an unpadded nine-row panel (mp = 9, a 9/32
grid dispatches nothing, the twins carried no `requires`), and the q8 site computed rows 0..7 only
(two four-row tiles; `mtp-count8-0.8b` passed because eight straight accepts never happened in 40
tokens). Fix: kq sites ride the eight-row form plus a single pass per row past eight, q8 sites a
third four-row tile, and the kq mul_mm dispatchers carry `mp % 32`. Tests: `mtp-count8-<tag>` (the
round) and `test_metal_gemv_rows_tiles` (the q8 tiler at 2 / 5 / 9 rows). The batch driver's own
nine-row route was already right (mp 32); its kq mul_mm reads maxd 0.60 with zero flips on
gemma-12B, so `mtp-vff9` takes `MM_TOL` on kq carriers. Ruler records hold depths 1 and 2 only -
none poisoned.

## S2 - the kernels the split names (ranked by the report, re-ranked by S0)

Boris: depth past 4 is unrealistic, so the verify widths that matter are 2 to 5 rows.

**S2.-1 - does the round ever reach five rows?** Re-measured and recorded (ruler, depths 1..4,
both carriers, M5; the `_depths` records and the ledger bullet). Acceptance by position, of all
rounds: about 75-85% / 50-66% / 35% / 22% on BOTH carriers, so tokens per round run 1.77 / 2.50 /
2.90 / 3.02 (Qwen) and 1.75 / 2.25 / 2.52 / 2.75 (gemma). The fourth draft is worth a tenth to a
quarter of a token; the five-row verify serves depth 4 alone. Speed today: Qwen 1.23 / 1.18 /
1.31 / 0.76x, gemma 1.16 / 1.20 / 1.14 / 0.92x - depth 3 is the best Qwen point on this box, depth
4 loses on both. Implied round costs (tokens per round over the speedup, in steps): Qwen 1.44 /
2.12 / 2.21 / 3.97, gemma 1.51 / 1.88 / 2.21 / 2.99. Consequence: the five-column stamp (S2.2) is
struck - its only customer is depth 4.

**S2.-1b - the round profiled (`lcpp_bench --prof`, prompt 0, 128 tokens, Qwen3.8-27B, M5, plain
step 37 ms).** Depth 1: 75 rounds, draft 3.1 ms per draft, two-row verify 45.7 ms (1.24x a step),
walk 0.3, replay 0.5. Depth 3: 54 rounds, 3.0 ms per draft (9 ms per round), four-row verify 66 ms
(1.79x a step), walk 0.6, replay 1.3. The verify is 85-92% of the round, the draft chain 6-11% -
the earlier inference of 0.3 step per draft from the ruler ratios was wrong; a NextN draft costs
0.08 step. The round's four-row verify also costs 13% more than the batch driver's four-row
same-slab step (66 vs 59 ms): the head layer, the serial encoder in `encode_verify_step`, the
host prep. So the verify is the lever, twice over: the K-quant row forms (1.59x -> ~1.2x, S2.4)
and the round's own overhead over the batch driver (~0.2 step). A flat 1.2x verify takes Qwen
depth 3 from 1.31x to ~1.9x; the draft chain fix alone is worth ~5%.

The gemma round profiled the same way (plain step 8.1 ms - the greedy chain hits here, which is
why S0's 12.2 ms "plain step" read high): depth 1, 75 rounds, draft 0.03 ms per draft (the fused
chain), two-row verify 11.4 ms (1.41x a step), walk 0.01; depth 2, 63 rounds, three-row verify
13.9 ms (1.72x). The verify is 99% of the gemma round: the routed experts re-streamed per row
(S2 #1) on top of the batch step's fixed overhead (10.4 ms same-slab two-row step against the 8.1
ms step). Baseline table for the arc, ms per round phase (M5, prompt 0, 128 tokens):

| carrier | plain step | verify 2 rows | verify 3 rows | verify 4 rows | draft per draft | walk + replay |
|---|---|---|---|---|---|---|
| Qwen3.8-27B + Q8 head | 37 | 45.7 (1.24x) | - | 66 (1.79x) | 3.1 | 0.8-1.9 |
| gemma-26B + drafter | 8.1 | 11.4 (1.41x) | 13.9 (1.72x) | - | 0.03 | 0.01 |

**S2.0 - their K-quant ladder, measured, not inferred.** The report's "1.05x at M = 4" for
K-quants is an inference from kernel selection. Read side by side, llama.cpp's
`kernel_mul_mv_ext_q4x4_f32_impl` has our twin's structure exactly: 8 threads per weight row, 4
rows per simdgroup, 2 simdgroups, a 16-weight dequant per thread per chunk dotted against `r1ptg`
x columns read from device memory - the same x-load-per-column pattern as `MetalKqMvK4T`. Nothing
in it explains a free fourth column. Prediction before measuring: their Q4_K step at M = 4 costs
1.35-1.5x their M = 1 step (ours: 1.59x), at M = 2 about 1.9x (plain mul_mv, two streams; ours
1.13x), at M = 5 about 1.5x (ours 2.73x). Instrument: `llama-batched-bench -npl 1,2,3,4,5,8` on
gemma-4-12B Q4_K_M, the per-step time = T_TG / ntg at each parallel count. If their M = 4 really
reads near 1.05x, our twin has a defect to find; if it reads where predicted, the gap to close is
the five-row point (a 5-column form) and the four-column form's ALU, not a missing trick.

Measured (llama-batched-bench b10659, Metal, gemma-4-12B Q4_K_M, M5 Max, npp 128, ntg 32, one
process; ms per step = T_TG / 32) against our same-slab ladder from S0:

| rows | 1 | 2 | 3 | 4 | 5 | 8 |
|---|---|---|---|---|---|---|
| llama.cpp ms/step | 16.6 | 19.5 | 26.9 | 26.8 | 28.3 | 45.7 |
| llama.cpp x one row | 1.00 | 1.17 | 1.62 | 1.61 | 1.70 | 2.75 |
| ours x one row | 1.00 | 1.13 | 1.59 | 1.59 | 2.73 | 2.77 |

The two ladders are the same curve. The report's 1.05x at M = 4 is refuted (1.61x), and so is
its "K-quant M = 2 falls to plain mul_mv" (1.17x - the ext gate reads differently in this build,
or the plain kernel is not two streams; either way the number is the number). Through four rows
we are at parity; the one point where they are ahead is five rows, where their `r1ptg = 5` stamp
keeps one weight stream (1.70x) and our ladder jumps to the eight-column form (2.73x). Both
engines pay 1.6x for four columns because both kernels are instruction-bound, not byte-bound:
per 16 weights a thread issues about 64 dequant instructions once and 4 x loads plus 16 FMAs per
column, so each column adds a quarter of the one-column cost - exactly the 1.17 / 1.6 / 2.75
shape. A five-column stamp buys the depth-4 point (2.73x to about 1.7x); beating 1.6x at four
rows needs a kernel that does not spend 20 instructions per column per 16 weights - the
simdgroup-matrix form (S2.4 below).

1. MoE expert gather on Metal - rows bucketed by expert so an expert plane streams once per
   verify (the CPU path already buckets). Predicted: gemma-26B two-row verify 1.45x -> 1.15-1.25x.
2. K-quant twin 3- and 5-column forms (today 3 rows pay the 4-column kernel, 5 rows re-grid).
   Predicted: M5 5-row 2.03x -> ~1.3x.
   2b. The batch driver's `nrows > 8` kq mul_mm rail (measured 5.41x at nine rows against 2.77x
   at eight): route nine to sixteen rows as eight-row forms plus passes, or give the kq mul_mm an
   M = 32 form that fills the part. Predicted: nine rows 5.41x -> ~3.5x.
   2c. **S2.4 - the eight-column simdgroup-matrix form.** The mul_mm at M-pad 32 already dequantizes
   each weight ONCE into a half tile and lets the matrix unit do the columns, but its 32x64 tile
   leaves d/64 threadgroups (60 on gemma-12B) - the occupancy failure behind 5.41x at nine rows.
   A verify-shaped twin: one simdgroup owns 8 weight rows x K, dequantizes its K-quant superblocks
   to half once, and multiply-accumulates against the x panel staged as 8 half columns (M padded
   to 8) - `simdgroup_multiply_accumulate` over 8x8x8 tiles, grid d/8 rows per simdgroup so the
   part fills at any d. Per 16 weights the column cost becomes one matrix instruction per 8
   columns instead of 20 scalar instructions per column, so the ladder should flatten: predicted
   1.15-1.3x at 2..8 rows (the dequant issue stays, the x loads and FMAs go), against 1.6x at
   four and 2.7x at five to eight today. The depth-4 verify would cost ~1.2 steps instead of 2.7,
   which roughly doubles what speculation returns at that depth. Numerics: the half x panel is
   the mul_mm's (maxd 0.6, zero flips - `MM_TOL`). Order: a k4 lab stamp raced against the
   four-column form on the M5 first; k6 and iq4xs next; the crown per box as the kq forms have
   today (the M4 Pro has no tensor unit to speak of - its simdgroup matmul may not win).

   **S2.4 lab verdict (M5 Max, `bench_metal_gemv_kernels` k4 arms, Qwen3.8-27B site shapes, ms per
   dispatch, best of 3; the weights sit in cache here, so the single pass reads 830 GB/s and every
   ratio is the ALU/issue cost alone):**

   | form | columns | q 5120x5120 | w13 5120x17408 | w2 17408x5120 |
   |---|---|---|---|---|
   | single pass (`k4_b1c`) | 1 | 0.018 | 0.054-0.058 | 0.057-0.060 |
   | production twin B2 | 2 | 0.029 | 0.089 | 0.102 |
   | production twin B4 | 4 | 0.048 | 0.146 | 0.186-0.192 |
   | production twin B8 | 8 | 0.102 | 0.286 | 0.341 |
   | simdgroup 8x8 form (`k4_mm8`, padded stage) | 8 | 0.076 | 0.237 | 0.258 |
   | its knockouts: no matmul / no dequant stores | 8 | 0.027 / 0.057 | 0.090 / 0.177 | 0.092 / 0.188 |
   | prefill tensor twin at MT 8 / 16 (`k4_tmm8/16`) | 8 / 16 | 0.097 / 0.103 | 0.211 / 0.217 | 0.337 / 0.371 |
   | tensor form, GEMV dequant, one step per superblock (`k4_tmv8k`) | 8 | 0.061 | 0.168 | 0.270 |

   Reading: the classic simdgroup multiply-accumulate runs at plain FMA rate on this GPU (its tile
   loop is 65% of `k4_mm8`; without it the stage-and-dequant floor is 1.5x the single pass), so it
   only removed the x loads and gained 20-25% per column over the four-column twin. The Metal-4
   tensor op is nearly free per column (16 columns cost what 8 do) but its tile at m = 8 is small
   and the dequant-to-half stage is the cost: the best stamp beats the eight-column twin by 40%
   and the four-column twin never - it computes eight columns at 3.4x the single pass where the
   twin does four at 2.7x. The lab-to-step calibration (twin B8 5.7x lab = 2.77x step, twin B4 2.7x
   lab = 1.59x step) predicts 5-8 verify rows at ~1.9x a step instead of 2.7x, and 3-4 rows
   unchanged at 1.59x. Depth 3 - the point that matters - does not move: on the M5 the K-quant
   four-column form is already at this GPU's FMA-issue floor. The forms stay as lab arms; nothing
   ships from S2.4 until a depth that uses 5-8 rows pays (it does not, S2.-1).
3. The twin's occupancy on ALU-short parts (the M4 Pro crowns single passes around it).
   Predicted: retires the per-box crown; M4 Pro 2-row 1.20x -> ~0.9x.
4. Pair-walk 4+1 / 4+2 forms. Predicted: M4 Pro 4-row 2.34x -> ~1.6x.
5. The NextN round in the gemma round's shape: one command buffer, in-graph argmax, one join.
6. Attention rows sharing one KV read (their Q = 2..4 buckets).

## S3 - the CPU verify

Instrument: `benchmarks/verify_batch_probe.das` (the marginal cost `b` of one extra row through
`forward_prefill` at width 1, 2, 4 over real corpus tokens; `--prof` dumps the section buckets).
The CPU batch nest is weight-stationary at any M, but the CPU round is fixed at two positions and
the non-repacked K-quant arm runs a full pass per position. Predicted: the two-row verify reads
~2.0x a step on Qwen3.8-27B Q4_K_M today (the round loses at 0.92x); routed through the
weight-stationary nest it reads ~1.1x and the CPU round wins.
