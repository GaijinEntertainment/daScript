# The fully-resident MoE chain on Vulkan (followup_vulkan item 43)

Cross-box development plan for the arc that follows the KHR tile PR. Retired when the work lands.

## Goal

A MoE file that fits the card whole runs the same way a dense file does on the whole-model
driver: one recorded window per prefill, one submit per token on decode, activations resident,
the router on the device. Today every MoE rides the per-op tier, which was built for files that
do not fit; on a file that fits it loses 2x to 7x on prefill and, where the decode span
declines, 2x on decode. The dev vehicle is the smallest MoE the loader takes, Qwen1.5-MoE-A2.7B,
minted to Q4_K_M from the Q8_0 on disk (9.50 GB; it fits every time); the targets are the
fitting files of every loadable MoE family, the 30B and 35B Qwens first.

## Where the time goes (measured 2026-09-08)

Dev-grade rows: `benchmarks/lcpp_bench.das -jit --for-debug-purposes -r 3 -p 512 -n 128 -t 16`
with `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1` (the post-rebase rebuild staled
the bench sidecar, so the class profile's CPU winners stamp - on a resident MoE the CPU runs
only the router and the glue); reference = llama-bench b10660 (build-vulkan-357)
`-ngl 99 -fa 1 -t 16 -r 3`; RTX 5060 Ti 16 GB, driver 616.56, the desktop holding 1.3 to 1.9 GB.

| file | GB | ours pp512 / tg128 | llama.cpp pp512 / tg128 | ratio | on the card |
|---|---|---|---|---|---|
| Qwen1.5-MoE-A2.7B Q4_K_M (local mint) | 9.50 | 538.1 / 38.3 | 5099.8 / 173.8 | 0.11x / 0.22x | whole: 24 expert stacks, shared experts, quads, classifier |
| Qwen1.5-MoE-A2.7B Q8_0 (board row 20) | 15.23 | 506.1 / 39.0 | 3507 / 52.2 | 0.14x / 0.75x | layers 0-2 streamed |
| Qwen3-30B-A3B-Instruct-2507 UD-Q3_K_XL | 13.83 | 931.7 / 81.3 | 1859.9 / 54.2 | 0.50x / 1.50x | layer 0 streamed; decode span ON |
| Qwen3.6-35B-A3B UD-IQ3_S | 13.68 | 638.3 / 41.1 | 2775.3 / 77.3 | 0.23x / 0.53x | layer 0 streamed; span declines (shared expert) |

The 512-token window on the 30B (`--prof --jobque-profiling`, `DASLLAMA_GPU_PROF=1`, the two
timed reps agreeing within a few ms; wall 560 ms, llama.cpp's whole window 275 ms):

| stage | ms |
|---|---|
| attention chain, GPU: qkv 44 + wo 37 GEMMs on the quant-feed sdot4 tile (~12 TFLOP/s) | 81 |
| attention chain, GPU: host round trips per layer (dma 59, copies 9) | 68 |
| attention chain, GPU: prep, flash attention, requant | 9 |
| expert FFN chain, GPU submit and wait (1.85 TFLOP at ~9.6 TFLOP/s, the small-bucket regime) | 192 |
| router on the CPU | 68 |
| CPU glue: norms, residual adds, requants, reduce | 55 |

The 512-token window on the Qwen1.5 twin (same instruments; wall 945 ms): `mm_gemm` 582 ms -
the SHARED expert's gate/up/down run on the CPU, since the per-op prefill has no shexp batch
arm; the routed experts on the device 138 ms; attention 136 ms; router 24 ms. Its decode: 23
attention submits plus 23 FFN submits per token at ~165 us each (7.6 ms), the token at 26 ms -
the rest is the per-layer host glue the span would remove, and the span declines on
`n_ff_shexp > 0` (`span_model_ok`, `dasllama_blocks.das`).

## What exists

- The whole-model resident driver (`dasllama_gpu_resident.das`): the plan, the arena upload,
  the window chain with the f16-feed coopmat projections and resident activations
  (`ARCHITECTURE_GPU_VULKAN.md` sec.2.2j), the KV mirror, the recorded token command, the
  deltanet block (sec.2.2ad, `ARCHITECTURE_GPU_VULKAN_DECODE.md` sec.2.2v), the qkv bias
  (`resident_upload_bias`). It refuses `n_expert > 0` at the plan and at the upload.
- The per-op tier's device pieces the MoE block reuses: the f16 gather + cm2 expert chain +
  device combine (`vk_moe_ffn_batch_xf`, `ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2q), the
  expert stacks' residency walk (`moe_gpu_upload_resident`), the decode span's router GEMV
  (`RouterGemv`), top-k (`TopK`) and meta-writing schedule (`ARCHITECTURE_GPU_VULKAN_DECODE.md`
  sec.2.2t).
- The resident dense FFN block (gate/up/act/down on the f16 feed) - the shared expert's shape.

## The slices, in order

1. **The shared expert's prefill on the device.** DONE 2026-09-08: the shexp triple rides the
   routed experts' chain as one region over every position (the f16-fed cm2 form with an
   identity slot map at unit weight, else the quant form over the Q8_0 image); the reduce applies
   the sigmoid gate. The twin's window 945 -> 360 ms, pp512 538 -> 1430.4 (llama.cpp 5099.8),
   the shared expert 582 ms on the CPU -> 59 on the device; decode unchanged. Gates: the call
   shape in `test_vulkan_moe_cm2.das` (one region, identity map, tight bar + poison) and the
   forced-feed form on the twin in `test_gpu_moe_shexp.das` at a 0.12 bar - the arms part from
   layer 1 on the router's near-ties (ulp-level residual differences -> Q8 requant roundings ->
   a flipped pick), not on the shared expert's rows, which match the CPU form to 1e-4 (sdot4)
   and 0.5% (f16); two route-off sessions are bit-identical.
2. **The decode span takes the shared expert.** DONE 2026-09-09: the shexp triple rides inside
   the span beside the routed slots (its feed off the same normed row into its own stacks'
   images, its metas one fixed region the host writes once, its gate logit one more router GEMV
   row past the router's, one combine `DecCombineSh`); `span_model_ok` declines only gemma4's
   dense shared expert now. The twin's tg128 38.5 -> 134.0 (llama.cpp 173.8, 0.77x), the token
   26 -> 7.5 ms. Gates: `test_vulkan_dec_tail.das`'s span cell with a gated shared expert (the
   no-shexp reference must miss the device row; `vk_span_reset` between the two spans of one
   process) and the twin's fed steps through the span in `test_gpu_moe_shexp.das` with the
   span-tokens witness. The 35B and Qwen1.5 Q8 rows ride the same arm; gemma-4-26B waits on its
   sandwich norms.
3. **The resident MoE prefill window.** The plan admits a MoE whose stacks fit (the per-op
   walk's sizes; all-or-nothing like the dense case, the room named honestly against the
   desktop); the window chain gains an MoE FFN block: a router GEMM over the window's rows
   (f32 plane, the span's arithmetic batched), top-k per row, a device bucket schedule (counts,
   prefix sums, region metas, the tile dispatch indirect), the existing gather + cm2 expert
   chain + combine, the residual add fused; the shared expert as the dense block with the row
   scale. The projections ride the f16-feed coopmat tiles, nothing crosses the bus per layer.
   Win: the profile puts it at ~200 ms of the 30B's 560, i.e. ~0.9x llama.cpp before the
   expert GEMMs themselves move.
4. **The hybrid MoE.** The 35B rides slice 3 with the deltanet block already in the chain.
5. **The expert GEMMs at small M.** The last term (192 ms on the 30B): a tile pick for
   32-row buckets, or a mul_mat_id-shaped kernel; measured on the probe first.

Slices 1 and 2 are small and land the shared-expert families' rows; slice 3 is the arc's body.

## Tests and evidence

- Kernel cells with CPU oracles for every new kernel (router GEMM, batched top-k, the bucket
  schedule, the row scale), `tests/test_vulkan_kernels.das`.
- A forced-feed logits-tolerance file on the twin in the hybrid file's form
  (`test_gpu_resident_hybrid.das`), plus a routing witness: the device's picks equal the CPU's
  `moe_select_core` on the window up to ties.
- The twin gets its `model_specs.das` row (recipe: `llama-quantize --allow-requantize
  Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf <out> Q4_K_M`) and a parity fixture; the board rows for the
  30B and 35B need their rows, a sidecar re-mint (`--tune-only`) and the pregate.
- Done = every fitting MoE file of a loadable family at parity on both rows, the census
  (`coverage-vk`) with no MoE row the tier declines, the per-op tier untouched for files that
  stream.
