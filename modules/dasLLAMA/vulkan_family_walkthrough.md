# Vulkan family walkthrough — the class-kernels branch vs llama.cpp (2026-08-06)

**Not record-grade.** das numbers are `-jit` debug runs of `lcpp_bench.das` (flavor debug-jit;
the released-exe rail was not used); llama.cpp numbers are `llama-bench` b9860 (fdb1db877),
Vulkan backend. Box: zen2 / RTX 5060 Ti 16GB (PCIe 5.0 x8, NV_coopmat2), quiet, 16 threads
self-pinned, tune sidecar pinned (no auto-tune). Shapes: pp512 / tg128 x 5 reps, `-ngl 99`
both sides. Branch state: carriers on meta-SSBO class twins (9dd5a4498), heap-derived weight
cap (f41e60c14). Kept for the record — no publication intent.

## Dense families

| Model | das engage | das tg128 | lcpp tg128 | tg % | das pp512 | lcpp pp512 | pp % |
|---|---|---|---|---|---|---|---|
| tinyllama-1.1B Q8 | resident ladder (ctx 2048) | 291.6 ± 0.4 | 294.7 ± 0.7 | 99.0 | 7241.7 ± 60.3 | 20900.0 ± 192.5 | 34.6 |
| Llama-3.2-3B Q8 | resident ladder (ctx 49128, auto cap) | 104.3 ± 0.2 | 109.7 ± 0.2 | 95.1 | 5811.7 ± 532.6 | 7696.5 ± 66.6 | 75.5 |
| Llama-3.1-8B Q8 (pre-slab) | DECLINED: fmt-0 arena 7.5GB > 4GiB maxStorageBufferRange (per-op fallback) | 7.2 ± 0.8 | 49.8 ± 0.1 | 14 | 110.0 ± 1.9 | 3719.8 ± 10.3 | 3 |
| Llama-3.1-8B Q8 (post-slab, VRAM_MB=11000) | resident ladder, TWO arena slabs (ctx 11544) | 46.5 ± 0.04 | 49.8 ± 0.1 | 93.3 | 2018.5 ± 2.5 | 3719.8 ± 10.3 | 54.3 |
| Qwen2.5-1.5B Q8 | DECLINED: attn_qkv_bias (per-op fallback) | 35.6 ± 0.5 | 202.4 ± 0.6 | 18 | 465 ± 23 | 14986 ± 69 | 3 |
| Qwen3-4B Q4_K_M | resident ladder (ctx 5606, k4/k6 + qk-norm) | 126.8 ± 0.8 | 132.8 ± 0.2 | 95.5 | 4614 ± 115 | 4955 ± 20 | 93.1 |
| Phi-3.5-mini Q4_K_M | resident ladder (ctx 15629) | 143.3 ± 0.3 | 145.0 ± 0.3 | 98.8 | 3329.7 ± 6.9 | 5535.6 ± 296.8 | 60.2 |
| gemma-2-2b Q8 | DECLINED: softcaps + sandwich norms + SWA (per-op fallback) | 22.8 ± 0.02 | 124.1 ± 0.3 | 18 | 327 ± 16 | 9565.7 ± 997.2 | 3.4 |
| gemma-3-1b Q8 | DECLINED: sandwich norms + SWA (per-op fallback) | 58.5 ± 0.1 | 236.2 ± 0.6 | 25 | 636 ± 56 | 21540.4 ± 252.3 | 3.0 |
| gemma-4-E2B Q8 | DECLINED: gemma cluster + PLE (per-op fallback) | 23.9 ± 0.02 | 120.6 ± 1.7 | 20 | 306.0 ± 2.4 | 8493.9 ± 62.0 | 3.6 |

Mistral-7B Q8 skipped (7.7GB — behind the arena-slab ceiling, only Q8 in the zoo).

## Hybrid / MoE families

| Model | das engage | das tg128 | lcpp tg128 | tg % | das pp512 | lcpp pp512 | pp % |
|---|---|---|---|---|---|---|---|
| Qwen3.5-0.8B Q8 (deltanet) | per-op rails: 18 dn triples + 6 attn quads + dn chains (no hybrid ladder) | 97.7 ± 0.3 | 276.9 ± 0.7 | 35 | 1376.1 ± 4.2 | 17075.0 ± 163.4 | 8 |
| Qwen1.5-MoE-A2.7B Q8 (14.2GiB) | MoE tier: experts [2..24) resident, [0..2) streamed, shexp async | 30.8 ± 0.1 | 32.6 ± 0.2 | 94.4 | 414.6 ± 2.0 | 1315.1 ± 64.7 | 31.5 |
| Qwen3-30B-A3B Q4_K_M (18.6GiB) | MoE tier AUTO: experts [13..48) resident, [0..13) streamed, 48 attn quads | 36.9 ± 0.6 | 35.7 ± 0.5 | **103.2** | 463.2 ± 1.2 | 503.0 ± 19.6 | 92.1 |
| Qwen3.6-35B-A3B Q4_K_M (20.6GiB, deltanet+MoE) | MoE tier AUTO: 110 dense + 40 shexp + 30 dn triples + 10 attn quads + experts [17..40) resident / [0..17) streamed | 37.2 ± 0.02 | 35.5 ± 0.3 | **104.7** | 399.3 ± 0.9 | 365.9 ± 21.8 | **109.1** |

Qwen3-30B note: llama.cpp's plain `-ngl 99` FAILS (ErrorOutOfDeviceMemory, no auto-spill);
its row needed hand-tuned `--n-cpu-moe 13` — the layer count copied from das's automatic
placement. The doesn't-fit MoE class is a das WIN on coopmat2 hardware: strategy > kernels.

gpt-oss-20b not measured: mx4 device kernels are CPU/Metal-only (followup item 6) — a known
decline, nothing to learn from the row.

gemma-4-26B-A4B Q4_K_M not measured (not really supported: q51 stacks are CPU-only, item 6) —
but its load CRASHED the tier (assert "kq row length must be a superblock multiple" via the
image-rail would-accept probe) and that got FIXED in-walkthrough: row-length servability is
now a decline at every entry (vk_moe_upload_stack, vk_arena_place, resident sizing walk,
verdict class 3 for the slice replay, heat-cache sizing, vk_plane_bytes). Post-fix the 26B
loads and serves clean (dense planes + the placeable expert layer resident, rest CPU).

## Probes (tinyllama Q8, same shapes)

| Arm | pp512 | tg128 | Reading |
|---|---|---|---|
| mode 3 mul_mm (default) | 7241.7 ± 60.3 | 291.6 ± 0.4 | the shipping config |
| mode 4 cm2 (DASLLAMA_COOPMAT=cm2, q8n) | 5916.8 ± 125.4 | 250.7 ± 0.2 | our cm2 kernel loses to our own mm; gap = kernel quality, not coverage; mode 4 also drags decode onto q8n GEMV (-14% tg) |
| llama.cpp NV_coopmat2 | 20900.0 ± 192.5 | 294.7 ± 0.7 | the hardware's proof point |

Re-record probe (--rerecord-ab, priced the push-form carrier flip before the TokMeta design):
tinyllama tg 285.8 -> 254.5 (-10.9%), Qwen3-4B tg 125.2 -> 114.8 (-8.3%) — ~20us/layer/token
re-encode cost; pp a wash. (Baselines predate the carrier conversion; post-conversion tg is
291.6 / 126.8 — the meta-SSBO twins measured +2.0% / +1.3% over the old rail.)

## The findings ledger

1. Decode is at parity on every resident family (95-99%); the whole competitive story on
   coopmat2 hardware is the prefill GEMM (followup_vulkan.md item 11).
2. Weight cap was an 8GB-era hardcode — fixed in-arc (f41e60c14): 4200 -> 14682 MB here.
3. The 4GiB maxStorageBufferRange caps the resident arena — 8B-class dense blocked; arena
   slabs = in-arc follow-up AFTER the sweep, gates the PR (MoltenVK main factor) (item 12).
   LANDED (14beb3a32): the 8B goes resident across two slabs at 93.3% tg / 54.3% pp — in
   line with the other dense residents (post-slab row above; measured on a mildly busy box,
   not walkthrough-clean). BUT the uncapped run armed at auto-ctx 25590 (weights 7.97GB +
   KV 6.5GB = 14.5GB of the 16GB card, zero desktop headroom) and WDDM demotion took tg to
   3.65 ± 0.01 / pp to 293 ± 47 — the ctx negotiation oversubscribes; needs a KV-side
   headroom margin.
4. qwen2 = one-flag unlock (attn_qkv_bias, item 13); gemma2/3/4-dense = one cluster
   (sandwich norms + SWA + softcaps, item 14). Osmosis-class work, not focus.
5. Hybrids serve correctly on per-op rails but need the hybrid ladder for competitive tg
   (item 2 datum); MoE decode is near-parity ALREADY on the cooperative tier — the MoE
   strategy layer (heat cache, residency, async shexp) has no llama.cpp analog.
6. The doesn't-fit MoE class is a das WIN on coopmat2 hardware: Qwen3-30B 103% tg,
   Qwen3.6-35B (deltanet+MoE) 105% tg AND 109% pp — both fully automatic placements vs
   their hand-tuned --n-cpu-moe (counts copied from our logs; plain -ngl 99 OOMs). The
   newest shapes are where das leads TODAY, before the cm2 kernel arc.
