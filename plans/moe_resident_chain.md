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

After slice 3 (2026-09-09, the same instruments; the whole-model driver on both):

| file | GB | ours pp512 / tg128 | llama.cpp pp512 / tg128 | ratio | on the card |
|---|---|---|---|---|---|
| Qwen1.5-MoE-A2.7B Q4_K_M (local mint) | 9.50 | 5069.2 / 142.2 | 5099.8 / 173.8 | 0.99x / 0.82x | whole: 10107 MB, mirror 3069 MB at ctx 16368 |
| Qwen3-30B-A3B-Instruct-2507 UD-IQ2_XXS | 10.34 | 2176.7 / 124.4 | 3520.0 / 116.6 | 0.62x / 1.07x | whole: 10296 MB, mirror 2841 MB at ctx 30305 |

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
   span-tokens witness. The Qwen1.5 Q8_0 row (board row 20, layers 0-2 streamed) reads
   1139.9 / 96.2 on both slices against its 506.1 / 39.0 (llama.cpp 3507 / 52.2): the decode
   above llama.cpp's, the prefill 0.33x. The 35B rides the same arm; gemma-4-26B waits on its
   sandwich norms.
3. **The resident MoE prefill window.** DONE 2026-09-09: the whole-model driver admits a MoE
   whose expert stacks fit the arena (the plan counts the expert triples, the shared triples
   and the f32 router plane; sized before the per-op reserves, which a fitting plan forgoes);
   the window chain's routed block is five device stages over the FFN-normed rows - the router
   GEMM (16x16 tiles over the f32 plane, the gate row beside the experts), the per-row select
   (the decode top-k's core over each row), the device bucket schedule (one workgroup: counts,
   scans, the three planes' records and per-wg maps in the host fill's layout, a sentinel tail
   past the real workgroup count that the cm2 tile returns on, so the dispatch is an upper
   bound and nothing is indirect), the existing f16 gather + cm2 tiles + act, and the combine
   onto the FFN rows (the gated form over the shared expert's rows, which ride the dense
   tail); and the token command gains the same block in decode form (the span's router GEMV
   and top-k over the driver's routing smalls, the expert GEMVs over the arena planes through
   the top-k's slot regions, the combine). The twin: pp512 1435.9 -> 5069.2 (llama.cpp 5099.8,
   0.99x), tg128 134.0 -> 142.2 (173.8, 0.82x). Gates: the four routing kernels against CPU
   oracles with poisons in `test_vulkan_kernels.das`, the twin's forced feed against the
   all-CPU chain at one and two windows in `test_gpu_resident_moe.das` (0.15 bar, the
   one-step-off control 3.7-9.1 logits off, the census witnesses), the per-op tier's shexp
   file pinned to its own arm through `set_gpu_resident_route`. `DASLLAMA_GPU_RESIDENT=0` is the A/B lever.
   The token on the twin (`DASLLAMA_GPU_PROF=1`, 7.1-7.3 ms of GPU per token, 7.4-7.7 ms host
   wall; llama.cpp's token 5.75 ms): the shared expert's three q8 GEMVs 2.37 ms over 24 layers
   (830 MB, 78% of the card's bandwidth), the routed expert GEMVs 1.64 ms (k4, 467 MB, 63%), the
   attention quads 0.9 ms (at bandwidth), the classifier 0.6 ms, and ~1.1 ms of small dispatches
   (router, top-k, the norms, the requants, the acts, the combine - twelve per layer, launch-bound).
   The decode levers are therefore the GEMV efficiency on the shared and expert planes and the
   small-kernel count per layer (fusing the router with the top-k, the act into the down GEMV's
   feed), not the routing itself. The window (99.2 ms of GPU for 512 tokens, llama.cpp's 100.4):
   the expert tiles 58.6 ms (gate 18.5, up 17.2, down 22.9 - the small-bucket regime, slice 5),
   the shared expert 15.8, the attention head 10.9, the router GEMM 4.5 (187 us per layer for
   128 MFLOP), the bucket schedule 3.3 (137 us per layer), the rest 6. The 30B UD-IQ2_XXS
   window (230.7 ms of GPU; llama.cpp's 145.5): the expert tiles 165.7 ms (gate 57.9, up 57.0,
   down 50.8 - 4096 bucket rows over 128 experts, 32 rows each, the iq2xxs s tile at 10.7
   TFLOP/s: slice 5's term, 72% of the window), the attention head 28.5, the router GEMM 11.7
   (243 us per layer), the schedule 12.4 (258 us per layer), the rest 12. Its token (7.74 ms of
   GPU, llama.cpp's 8.58): the attention head 3.2 ms over 48 layers, the expert GEMVs 2.3, the
   small dispatches 1.6, the classifier 0.6. The router retiled (a 32x32 tile, 2x2 per
   invocation, K by 64) and the schedule's slot walk staged through shared memory took the 30B
   window to 221.2 ms (router 6.8, schedule 7.8): 2270.5 / 124.6 (0.65x / 1.07x); the twin
   5195.2 / 142.4 (1.02x / 0.82x). What is left in those two is latency: the schedule is one
   workgroup whose per-thread slot walk is serial (no atomics or subgroup prefix in the
   emitter yet), the router 142 us per layer for 268 MFLOP.
   Slice 5's reading of llama.cpp's `mul_mat_id` on coopmat2 (`ggml-vulkan.cpp`,
   `mul_mm_cm2.comp`): one workgroup per (expert, 128-row weight tile, token tile), the
   expert's rows gathered INSIDE the tile from the ids (`load_row_ids`, a subgroup ballot
   compaction after a `count_experts` pass), the tile picked by the TOTAL token count (the l
   tile, BN 256, at 512 tokens) and shrunk to BN/4 or BN/2 when the expert holds fewer rows
   (`enable_smaller_matrices`), so a 32-row expert runs a 128 x 64 x 64 MMA per K step on one
   A decode - our s tile runs 128 x 32 x 64 on the same decode. Both decode every expert
   weight once per window at 32 rows per expert; the levers to probe are the decode itself
   (their `DECODE_VECTOR` arm against our DECV4 twin on iq2xxs) and the row gather inside the
   tile (no gather kernel, no schedule). Their own harness at the 30B expert shape
   (`test-backend-ops perf -o MUL_MAT_ID -p "n_mats=128,n_used=8,b=0,m=768,n=512,k=2048"`,
   build-vulkan-357, the same card): iq2_xs 737 us (17.5 TFLOP/s), q4_0 853 (15.1), q8_0 988
   (13.1), q4_K 985 (13.1), q6_K 1031 (12.5), f16 1181 (10.9). Ours on the 30B window's
   iq2xxs-led mix: gate 1210 us per layer, up 1181, down 1060 (10.7-12.2 TFLOP/s) - a 1.2x to
   1.6x gap per plane, not the 1.6x the whole window shows, so the rest of the window (the
   attention head 28.5 ms, the act, combine, norm and requant dispatches ~12 ms, the router
   and schedule 14.6) carries the other half of the distance to their 145 ms.
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
