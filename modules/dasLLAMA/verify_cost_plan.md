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

| rows through the batch driver | 1 | 2 | 3 | 4 | 5 | 8 |
|---|---|---|---|---|---|---|
| ms/step | 15.44 | 17.38 | 24.61 | 24.48 | 42.16 | 42.71 |
| x the one-row step | 1.00 | 1.13 | 1.59 | 1.59 | 2.73 | 2.77 |
| marginal row | - | +0.13 | +0.47 | +0.00 | +1.15 | +0.00 (x3) |

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

`MTP_MAX_ROWS = 9` and the round clamps k to 8, so depth 8 verifies 9 rows; the NextN site wrapper
has no rail switch past 8 rows and a K-quant site would reach the GEMM with mp = 9 against a
32-row grid. Test: a same-slab verify at 9 rows on a K-quant carrier (`mtp-vff9-<tag>`), expected
red today; the fix follows the test.

## S2 - the kernels the split names (ranked by the report, re-ranked by S0)

1. MoE expert gather on Metal - rows bucketed by expert so an expert plane streams once per
   verify (the CPU path already buckets). Predicted: gemma-26B two-row verify 1.45x -> 1.15-1.25x.
2. K-quant twin 3- and 5-column forms (today 3 rows pay the 4-column kernel, 5 rows re-grid).
   Predicted: M5 5-row 2.03x -> ~1.3x.
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
