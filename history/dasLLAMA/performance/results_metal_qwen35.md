# Metal Qwen3.5/3.6 hybrid (qwen35) scoreboard (living doc — CURRENT numbers only; history in git)

Qwen3.5-4B (dense hybrid) + Qwen3.6-35B-A3B (hybrid MoE) — the Wave D showcase: 3-of-4
layers Gated-DeltaNet (GPU state mirror + fused sequential scan), gated full attention
(2x-wide Q, partial NEOX rope 64/256, σ out-gate); the 35B adds grouped MoE routing + the
sigmoid-gated shared expert + f32-on-disk β/α. Metal vs Metal, the 3B methodology
(`results_metal_3b.md`): das = dasLLAMA (`-jit`, mapped metal-flavor `.dlim`,
`DASLLAMA_PIN_DECODE=metal` = MetalMode.required); llama.cpp = stock flags, Metal default
(its GPU gated-delta-net kernel is default-on — an honest fully-GPU baseline), `-fa auto`;
single-stream from `llama-bench -r 1`. das rails: `prefill_perf.das` N=512 (REPS=3, settled
rep) + `batch_decode_perf.das -p 512 -n 128 -b 1 --kv f16` (GREEDY sequential). NO batch
ladder — Wave D is batchless by design (multistream MoE+state traffic doesn't amortize;
per-row batch rides the decode driver). BOTH sides measured interleaved in one Parsec-off
window; lcpp side recorded in `baseline_metal_qwen35_m1.tsv`. Requants are local
(`llama-quantize --allow-requantize` from the Q8), each parity-smoked 90 positions
token-exact first. Ratio = das / llama.cpp, >1 = das faster; winners bold.

## Apple M1 Max — daslang branch `bbatkin/dasllama-metal-wave-d` @ be6d8bea2, llama.cpp 7642f1c, 2026-07-20 settled window (Parsec off)

### Qwen3.5-4B Q8_0

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 1001.5 | 974.2 | **1.03x** |
| tg128 B=1 | 58.5 | 57.4 | **1.02x** |

### Qwen3.5-4B Q4_K_M (local requant from Q8)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 925.6 | 847.7 | **1.09x** |
| tg128 B=1 | 72.0 | 64.6 | **1.11x** |

### Qwen3.5-4B Q5_K_M (local requant from Q8)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 851.9 | 767.9 | **1.11x** |
| tg128 B=1 | 61.4 | 55.7 | **1.10x** |

### Qwen3.5-4B Q6_K (local requant from Q8)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 919.4 | 835.9 | **1.10x** |
| tg128 B=1 | 66.4 | 63.9 | **1.04x** |

Parity note (documented finding, not a blocker): the Q6_K requant's lockstep probe is
90/90 token-exact but shows a deterministic single-position logits spike (maxd 10.3 at
one comma step; every other position ~0.35, no compounding, argmax intact) — the k6
CPU-Q8_K-activations-vs-GPU-f32 class amplified on an outlier channel.

### Qwen3.6-35B-A3B Q4_K_M (UD; deltanet + grouped MoE + shexp whole-graph GPU)

| shape | das tok/s | lcpp tok/s | das/lcpp |
| :--- | ---: | ---: | ---: |
| pp512 | 852.3 | 831.0 | **1.03x** |
| tg128 B=1 | 58.4 | 54.2 | **1.08x** |

das leads every cell on the wave: the K-quant 4B rows 1.04-1.11x, Q8 at 1.02-1.03x, and
the whole-graph 35B (deltanet scan + routed experts + shexp + f32 β/α slabs all
GPU-resident) 1.03x prefill / 1.08x decode against llama.cpp's default-on fused GPU
gated-delta-net path.
