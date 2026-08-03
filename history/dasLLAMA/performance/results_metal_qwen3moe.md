# Metal Qwen3-30B-A3B (qwen3moe) format scoreboard (living doc — CURRENT numbers only; history in git)

Qwen3-30B-A3B-Instruct-2507 — the Wave C MoE showcase (GPU routing: router GEMV + top-k
select + expert-indexed GEMVs on decode/batch, CSR-gathered mul_mm on prefill; dim 2048 /
ne 128 / k 8 / nfe 768, 48 layers). Metal vs Metal, the 3B methodology
(`results_metal_3b.md`): das = dasLLAMA (`-jit`, mapped metal-flavor `.dlim`,
`DASLLAMA_PIN_DECODE=metal` = MetalMode.required); llama.cpp = stock flags, Metal default,
`-fa auto`; single-stream from `llama-bench -r 1`, batch ladder from `llama-batched-bench
-npp 512 -ntg 128 -c 16384`. das tg128 B=1 rows are GREEDY; batch rows use `--fixed-token
100` with `DASLLAMA_METAL_BATCH_PIPE=1`. BOTH sides measured interleaved in one Parsec-off
window (pinned baselines do not transfer across windows — cross-window moves are
non-uniform); pp512 settled over 3 reps, and any cell showing a transient >10% dip
(page-in noise while 17-31GB models cycle through a 64GB box) re-measured both sides.
lcpp side recorded in `baseline_metal_qwen3moe_m1.tsv`. Ratio = das / llama.cpp,
>1 = das faster; winners bold.

## Apple M1 Max — daslang branch `bbatkin/dasllama-metal-wave-c` @ 4bfb79989, llama.cpp 7642f1c, 2026-07-19 settled window (Parsec off)

### Q4_K_M (native kq expert stacks: k4 gate/up + k6 down)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 861.0 | 836.2 | **1.03x** |
| tg128 B=1 | 73.4 | 74.6 | 0.98x |
| tg128 B=2 | 96.0 | 87.1 | **1.10x** |
| tg128 B=4 | 113.0 | 111.0 | **1.02x** |
| tg128 B=8 | 126.5 | 128.3 | 0.99x |
| tg128 B=16 | 141.4 | 144.7 | 0.98x |

pp512 at parity (was 0.93x before the stage-5 prefill chase: route-chain rework + k6
mul_mm dequant rework). Single-stream and the B>=4 ladder hold the parity class; B=2
leads 1.10x.

### Q5_K_M (local requant from Q8; k5 gate/up + k6 down — parity smoke 24/24 token-exact)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 776.2 | 786.7 | 0.99x |
| tg128 B=1 | 60.2 | 63.2 | 0.95x |
| tg128 B=2 | 83.2 | 71.9 | **1.16x** |
| tg128 B=4 | 98.3 | 92.0 | **1.07x** |
| tg128 B=8 | 108.5 | 103.5 | **1.05x** |
| tg128 B=16 | 120.4 | 114.3 | **1.05x** |

pp512 at parity (was 0.90x). Batch ladder all green; B=1 0.95x is the k5 single-stream
class (the 4B chase's b1c select-form pick does not yet apply to the expert-indexed
twin — perf ledger).

### Q6_K (local requant from Q8; pure-k6 stacks — parity smoke 24/24 token-exact)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 836.9 | 840.3 | 1.00x |
| tg128 B=1 | 71.0 | 68.2 | **1.04x** |
| tg128 B=2 | 98.9 | 79.7 | **1.24x** |
| tg128 B=4 | 118.4 | 100.1 | **1.18x** |
| tg128 B=8 | 132.9 | 114.4 | **1.16x** |
| tg128 B=16 | 148.4 | 126.6 | **1.17x** |

pp512 dead-even (was 0.87x and rep-unstable — the k6 mul_mm A-stage rework folded the
byte-position shifts into exact power-of-two pre-scales, killing both the gap and the
variance). B=1 leads 1.04x, the whole batch ladder green 1.16-1.24x.

### Q8_0 (q8 expert stacks through the 34B blob rail)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 971.2 | 952.3 | **1.02x** |
| tg128 B=1 | 67.9 | 55.7 | **1.22x** |
| tg128 B=2 | 102.3 | 69.0 | **1.48x** |
| tg128 B=4 | 133.1 | 81.7 | **1.63x** |
| tg128 B=8 | 146.3 | 88.5 | **1.65x** |
| tg128 B=16 | 157.7 | 92.9 | **1.70x** |

das sweeps the ENTIRE Q8 board — pp512 included (was 0.90x). The expert-indexed q8 blob
GEMVs (one weight pass per (slot, stream) with GPU-side routing) against llama.cpp's
per-token q8 MoE mul_mv: the same weakness class as their kq B=2/3 hole, here across the
whole ladder and growing with B.
