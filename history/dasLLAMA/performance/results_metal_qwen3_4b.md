# Metal Qwen3-4B format scoreboard (living doc — CURRENT numbers only; history in git)

Qwen3-4B-Instruct-2507 — the family-waves Wave A showcase (NEOX rope + QK-norm + qd != dim:
dim 2560 / qd 4096 / kv 1024 / hidden 9728, 36 layers). Metal vs Metal, one rep per cell,
the 3B methodology verbatim (`results_metal_3b.md`): das = dasLLAMA (`-jit`, mapped `.dlim`
image, `MetalMode.required`); llama.cpp = stock flags, Metal default, `-fa auto`;
single-stream from `llama-bench -r 1`, batch ladder from `llama-batched-bench -npp 512
-ntg 128`. das tg128 B=1 rows are GREEDY; batch rows use `--fixed-token 100` with
`DASLLAMA_METAL_BATCH_PIPE=1`; zero-copy logits on everywhere. lcpp baselines pinned in
`baseline_metal_qwen3_4b_m1.tsv`, NOT re-measured. Ratio = das / llama.cpp, >1 = das
faster; winners bold.

## Apple M1 Max — daslang branch `bbatkin/dasllama-metal-wave-a`, llama.cpp 7642f1c, 2026-07-18 (Parsec off; Q8 re-measured post blob-only .dlim — kq cells 2026-07-17, spot-checked flat-or-better)

das rails: `batch_decode_perf.das --prefill 512 --ngen 128 --bmax 16 --kv f16
--fixed-token 100` (pins: decode=metal, backend=portable, mirror 3072MB; B=1 row from a
separate greedy `--bmax 1` run, no pipe) + `prefill_perf.das` N=512 row (prefill=metal).

### Q4_K_M (native kq planes: k4 + k6)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 914.3 | 929.2 | 0.98x |
| tg128 B=1 | 89.2 | 82.1 | **1.09x** |
| tg128 B=2 | 122.2 | 89.2 | **1.37x** |
| tg128 B=4 | 151.5 | 127.7 | **1.19x** |
| tg128 B=8 | 163.0 | 152.2 | **1.07x** |
| tg128 B=16 | 247.6 | 234.5 | **1.06x** |

Decode all green; the daily-driver format leads single-stream (the kq spec chain, chase
round 1) and the whole batch ladder. pp512 -1.6% — see the pp line note below.

### Q8_0 (q8 repack, s16 scale plane — the load default)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 1041.8 | 1064.9 | 0.98x |
| tg128 B=1 | 59.5 | 67.7 | **0.88x RED** |
| tg128 B=2 | 113.1 | 103.0 | **1.10x** |
| tg128 B=4 | 192.3 | 159.6 | **1.20x** |
| tg128 B=8 | 205.7 | 190.4 | **1.08x** |
| tg128 B=16 | 350.6 | 266.0 | **1.32x** |

Blob-only .dlim rewiring (2026-07-18): B=4/8/16 UP +5/+10/+24% (whole-blob binds, the ys
per-segment stores, qkv split-K stand-down); B=1 DOWN 64.5 -> 59.5 — the single-stream q8
GEMV family's blob addressing replaced the planar kernels' one byte4 vector load with 4
scalar loads (the 34B block's +2 quant phase), and the 1B-4B-class GEMVs are issue-bound
(the 12B, DRAM-bound, is flat). Fix designed, ledgered top of the queue: a third uint16
buffer view (2-byte-aligned in the blob) + sign-extend unpack, or the MvB2 shape at
nrows=1 (ys=0/xs4=0 double-store trick, zero kernel changes). Was 0.95x = the sequential-
phase anomaly class before the rewiring.

### Q6_K (pure k6)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 918.3 | 916.3 | **1.00x** |
| tg128 B=1 | 78.2 | 78.2 | **1.00x** |
| tg128 B=2 | 121.4 | 83.7 | **1.45x** |
| tg128 B=4 | 142.0 | 124.2 | **1.14x** |
| tg128 B=8 | 153.6 | 137.6 | **1.12x** |
| tg128 B=16 | 232.2 | 230.8 | **1.01x** |

B=1 was 0.85x pre-chase (greedy 66.6): the tied-k6 classifier gated the spec chain OFF, so
every greedy step paid the ~1ms CPU tail PLUS the 607KB logits scan's DRAM contention right
at the k6 stream's bandwidth wall. The kq spec chain (chase round 1) closed it to 0.99x —
the fused H-form rope-store (chase round 2: bias+norm+rope+store in ONE
head-cooperative dispatch on f16 mirrors) closed the last point to a dead tie.

### Q5_K_M (k5-dominant, k6 mixed in)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 820.1 | 831.2 | 0.99x |
| tg128 B=1 | 67.5 | 66.5 | **1.02x** |
| tg128 B=2 | 104.3 | 71.2 | **1.46x** |
| tg128 B=4 | 142.3 | 120.5 | **1.18x** |
| tg128 B=8 | 154.3 | 143.0 | **1.08x** |
| tg128 B=16 | 228.2 | 219.0 | **1.04x** |

B=1 was 0.90x pre-chase (59.9): the b2r k5 GEMV loses ~10% to the select-form b1c at the
4B's n=2560 sites (gemv lab, every d: qkv/w13/cls) — production now picks b1c at n < 3072
/ small-d sites (gpu/step 15.87 -> 14.84ms), the kq spec chain hides the CPU tail, and the
fused H-form rope-store (round 2) adds the last point.
B=2 1.46x = the k5 ext twin vs lcpp's weakest K-quant mul_mv, same as the 3B board.

## Board summary (post chase rounds 1+2)

Batch B=2..16 GREEN across every format (17 of 18 cells; B=2 leads 1.28-1.46x); pp512 at
the line (0.98-1.00x). **B=1 after chase rounds 1+2: 1.09 / 0.95 / 1.00 / 1.02** (was
1.00 / 0.95 / 0.85 / 0.90). The levers:
1. **The kq spec chain** (the 3B arc's parked ledger item, now shipped): tied-k6
   classifiers ride the GPU argmax + a MetalEmbedK6 winner-row gather, so greedy B=1 chains
   on the GPU exactly like cls_q8 — the ~1ms/step CPU tail AND the 607KB logits-scan DRAM
   contention disappear. Q6 +16%, Q4 +7%, Q5 +4%.
2. **k5 per-site variant pick**: the select-form b1c (gemv-lab winner at every n=2560
   shape, -9..-11% vs b2r and under the lcpp wall) is production for n < 3072 / small-d
   sites; b2r keeps n >= 3072. Q5 gpu/step 15.87 -> 14.84ms.
3. **The fused H-form rope-store** (round 2, the former ledger item): on QK-norm x f16
   mirrors, bias + per-head RMS + rope + store run as ONE head-cooperative dispatch —
   the prepass dispatch and its q/k round trip disappear (+~1% on Q4/Q5/Q6; other KV
   codecs keep prepass + flat).

Remaining, pre-existing class:
- **Q8_0 B=1 0.95x** — the sequential-phase anomaly the merged 3B board carries at 0.97
  (same signature: the batched-B1 form 0.98 beats the sequential loop on the same
  forward(); spec on, q8 GEMVs measure 320-412 wGB/s at 4B shapes = healthy, s32 A/B
  slower, H-form neutral here). The parked per-32-step bucket diagnostic is the named
  next step.
