# Metal 3B format scoreboard (living doc — CURRENT numbers only; history in git)

Llama-3.2-3B-Instruct, Metal vs Metal, one rep per cell. das = dasLLAMA (`-jit`, mapped `.dlim`
image, `MetalMode.required` — CPU fallback panics, so every cell is honestly GPU). llama.cpp =
stock flags, Metal default, `-fa auto`; single-stream from `llama-bench -r 1`, batch ladder from
`llama-batched-bench -npp 512 -ntg 128`. das protocol: tg128 B=1 rows are GREEDY (the real
single-stream workload — the speculative decode chain is a product feature and fixed-token
feeding defeats it, costing q8 14%); batch rows use `--fixed-token` (batched-bench parity) with
`DASLLAMA_METAL_BATCH_PIPE=1` (one-deep deferred landing — exact for fixed-token; wrong for
logit-gated sampling, so it stays opt-in). Zero-copy logits are on everywhere (the classifier
writes each session's storage via bytesNoCopy wrappers). lcpp baselines are pinned in
`baseline_metal_3b_m1.tsv` and are NOT re-measured — only the das side reruns. Hand-maintained
(not generated). Ratio = das / llama.cpp, >1 = das faster; winners bold.

## Apple M1 Max — daslang branch `bbatkin/dasllama-3b-format-profiling`, llama.cpp 7642f1c, 2026-07-17 (Parsec off)

das rails: `batch_decode_perf.das -p 512 -n 128 -b 16 --kv f16 --fixed-token 100` (pins:
decode=metal, backend=portable, mirror 3072MB) + `prefill_perf.das` N=512 row.

Measurement validity: never chain a timed window straight after a gate/suite run (peak-load
thermal throttle — the fan tells you); idle a few minutes first, gap the formats, and check
each format's SEQUENTIAL phases for flatness — >2-3% wobble means throttle-tainted, rerun it.
High-B cells still carry ±2% honest window variance; a cell at 0.99-1.01x is AT the line.

### Q4_K_M (native kq planes: k4 + k6, tied Q6_K classifier)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 1215.6 | 1213.5 | **1.00x** |
| tg128 B=1 | 112.9 | 101.5 | **1.11x** |
| tg128 B=2 | 157.3 | 112.8 | **1.39x** |
| tg128 B=4 | 194.7 | 167.1 | **1.17x** |
| tg128 B=8 | 209.0 | 200.3 | **1.04x** |
| tg128 B=16 | 327.6 | 315.7 | **1.04x** |

ALL GREEN. B=2..8 ride the kq ext small-batch mv twins, B>=9 the kq mul_mm twins; the k6
round-1 kernels serve the k6 sites, and the pipe + zero-copy landing hides the batch CPU
tail. lcpp's B=1 batched-bench row is 93.3 (its batch machinery costs ~8% vs llama-bench's
101.5 single loop); das beats both. Follow-up lever: the spec chain is gated OFF here (tied
cls = k6, not cls_q8) — extending speculation to the kq classifier stacks on the 1.11x.

### Q8_0 (q8 repack, s16 scale plane — the load default)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 1380.5 | 1395.9 | 0.99x |
| tg128 B=1 | 84.1 | 86.4 | **0.97x RED** |
| tg128 B=2 | 140.0 | 127.9 | **1.09x** |
| tg128 B=4 | 233.0 | 218.7 | **1.07x** |
| tg128 B=8 | 250.9 | 252.7 | 0.99x |
| tg128 B=16 | 402.0 | 362.7 | **1.11x** |

OPEN: tg128 B=1 at 0.97x (greedy; was 0.95 before zero-copy) — the sequential-phase anomaly
persists (the batched-B1 form does 85.6 = 0.99x on the same forward(); warmup-immune, not
clock ramp; per-32-step bucket split is the parked diagnostic). B=8 -0.7% and pp512 -1.1% =
the wobble/deferral class.

### Q6_K (pure k6 — every weight site rides the k6 kernels)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 1214.7 | 1199.2 | **1.01x** |
| tg128 B=1 | 97.8 | 97.3 | **1.01x** |
| tg128 B=2 | 159.7 | 107.5 | **1.49x** |
| tg128 B=4 | 189.0 | 164.9 | **1.15x** |
| tg128 B=8 | 196.1 | 195.6 | **1.00x** |
| tg128 B=16 | 307.6 | 310.6 | 0.99x |

The round-1 k6 kernels (mul_mm A-stage fma fold + vectorized 6-bit compose; ext twins on
uint views) flipped the format's broad red (was pp 0.92 / B=8 0.90 / B=16 0.93); the pipe +
zero-copy landing closed the batch CPU tail. B=16 -1.0% = the wobble class (per-step GPU
50.9ms vs lcpp's 51.5ms total — nothing kernel-shaped left in the gap). An ext-twin probe at
B=16 lost to the mul_mm (128 vs 294 tok/s); the B>8 crossover stands.

### Q5_K_M (k5-dominant, k6 mixed in; B=2..8 ride the k5 ext mv twins)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 1090.4 | 1090.8 | **1.00x** |
| tg128 B=1 | 83.1 | 84.0 | 0.99x |
| tg128 B=2 | 134.4 | 90.5 | **1.49x** |
| tg128 B=4 | 181.3 | 159.1 | **1.14x** |
| tg128 B=8 | 192.9 | 187.3 | **1.03x** |
| tg128 B=16 | 300.9 | 294.7 | **1.02x** |

B=2 1.49x is the board's biggest cell (lcpp has no small-batch ext form at ne11 2-3 and its
k5 mul_mv is its weakest K-quant kernel — their Q5_K_M tg128 trails their own Q6_K despite
smaller weights). B=1 -1.1% (greedy) — the same sequential-phase class as Q8 B=1, scaled by
k5's lower tok/s; the lab has our k5 b1d/b2r walls above lcpp's per-kernel.
