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
| the same after the routing retile | 10.34 | 2270.5 / 124.6 | 3520.0 / 116.6 | 0.65x / 1.07x | |
| Qwen3.6-35B-A3B UD-IQ2_XXS (hybrid) | 10.76 | 2252.3 / 95.8 | 2853.1 / 71.6 | 0.79x / 1.34x | whole: 11110 MB, mirror 1992 MB at ctx 102011 |

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
4. **The hybrid MoE.** DONE 2026-09-09: a recurrent MoE layer takes the routed block after its
   deltanet head - the deltanet registration builds the layer with the shared expert as its
   dense triple, and the routed block registers on it through its own seat
   (`rdec_set_moe_experts`); the window chain's tail already served both heads. The UD-IQ3_S
   files (13.7 GB) do not plan on the 16 GB card, so the fitting file is Qwen3.6-35B-A3B
   UD-IQ2_XXS (10.76 GB; its Qwen3.5 twin is stocked too): whole on the card at 11110 MB with a
   1992 MB mirror at ctx 102011, pp512 2252.3 / tg128 95.8 against llama.cpp's 2853.1 / 71.6
   (0.79x / 1.34x). Gate: `test_gpu_resident_moe.das`'s hybrid fixture at one and two windows
   (the conv tail and the state across the seam), the same bar and controls.
5. **The expert GEMMs at small M.** The last term (192 ms on the 30B): a tile pick for
   32-row buckets, or a mul_mat_id-shaped kernel; measured on the probe first.
   5a DONE 2026-09-09 - the decode, not the tile. llama.cpp's coopmat2 branch creates no
   integer-dot `mul_mat_id` pipeline (`ggml-vulkan.cpp`, the `CREATE_MMQ ... _id_q8_1` block
   sits in the KHR branch), so every harness figure above is its cm2 tile, and its pick
   (`ggml_vk_guess_matmul_id_pipeline`) takes the l pipeline by the TOTAL token count (512 >
   64) whose `enable_smaller_matrices` shrinks a 32-row bucket's column to BN/4 = 32: the
   same 128 x 32 x 64 geometry as our s tile. The new probe arm (`harness/vk_gemm_probe.das
   -- moe:<fmt>`: the 30B schedule, 128 buckets of 32 rows, gate/up d 768 K 2048 and down
   d 2048 K 768, the device bound with its sentinel tail) put our tile beside theirs per
   format, gate/up plane in us: q8 892 (theirs 998), k4 728 (1009), iq4xs 632 (959), iq2xs 664
   (744) - ahead - but iq2xxs 955 (754), iq3xxs 728 (788), iq3s 1141 (870), iq2s 830 (797).
   The tile was not the term; the decode bodies were: iq2xs's decode reads one 16-bit lane
   while the four slow formats selected bytes out of lanes (`unpack8(lane)[i & 1]`) and built
   the IQ2_XXS / IQ3_XXS sign index from two selected bytes. Respelled as lane shifts (the
   aux32 assembled from two lanes): iq2xxs 749, iq3xxs 585, iq3s 767, iq2s 772. The rows:
   the 30B window 221.2 -> 187.7 ms (expert tiles 166 -> 133), pp512 2670.7 (0.76x), tg128
   124.6; the 35B 2542.7 (0.89x), tg128 95.3; the twin (k4) unchanged at 5208.4 / 143.1.
   5b DONE 2026-09-09 - the schedule's shape. llama.cpp's own per-op logger on the real 30B
   window (`GGML_VK_PERF_LOGGER=1 llama-bench -p 512`) reads its iq2_xxs gate/up plane at 594
   us - FASTER than its 754 on uniform buckets - because the l pipeline's column widens to 64
   or 128 rows on a big bucket, so the real router's skew amortizes each A decode over more
   rows; our s tile decoded the 128-row weight tile once per 32 rows whatever the bucket. The
   real window (the profile's new bucket report, the last MoE layer): 69 of 128 experts route,
   nine hold over 128 rows (the largest 467), 48 sit within 32 - 175 s tiles where a 32/128
   ladder runs 87. The first cut, whole 128-row chunks on the m class with the remainders on
   s, gained 2% on a 1 / (rank + 8) profile (`moesk:`): the mid-sized buckets (33 to 127 rows)
   were the cost, and a whole-column rule leaves them on the s tile. The ladder that landed:
   the m class takes the s tile's partial-column fast path (`STILE` generalized to the stamp's
   column, the store clamped, every f16 plane the tiles read carrying 128 rows of slack), and
   the schedule cuts a bucket past 32 rows into m columns with the last one partial - unless
   the remainder past whole columns fits the s column, which takes it - so a 100-row bucket is
   one m column instead of four s tiles (`sched_ladder_m_rows`); one schedule pass writes both
   piece lists (records [0, ne) s, [ne, 2 ne) m, the maps at 2048 words on, one packed two-way
   scan), two dispatches per plane under separate hazard bits (`VHZ_GATE_M` / `VHZ_UP_M` /
   `VHZ_MDN_M`) so they co-run. On the probe's real-shape profile (`moesk:iq2xxs`): gate/up 842
   -> 593 us, down 915 -> 650. The rows: the 30B window 187.7 -> 153.9 ms (expert tiles 133 ->
   97.7; llama.cpp 142.3 with its tiles at ~88), pp512 3242.0 (0.92x), tg128 123.6; the 35B
   2837.7 (0.99x), tg128 95.9; the twin's window 99.2 -> 94.5 ms, its row 5152.9 (the first
   measured rep after the warmup reads 101 ms on every model here - a driver warm-up the
   bench's one warmup does not absorb - so its three-rep mean and spread wander).
   What is left of the 30B window against theirs: the expert tiles ~10 ms, the attention head
   28 ms (q 9.0, wo 7.7, attn 5.0, qkn 2.8, rope 1.6, k+v 2.1), the router 6.6 and schedule
   8.1, the combine 4.0, act 2.8, gather 1.6, the residual adds and requants ~4.
   5c DONE 2026-09-09 - one more pass on the two parity misses, from the reference engine's own
   per-op tables (`GGML_VK_PERF_LOGGER=1`): on the 30B window we beat it on the attention head
   (qkn + rope 4.4 ms against its 10.7, the adds and norms 3.7 against 5.4) and lose on the
   router (6.6 against 1.2), the schedule (8.1 against none: it gathers inside its tile) and the
   gate/up tiles (~70 us per plane); on the twin's token its shared-expert GEMVs read 1.46 ms
   against our 2.39 because the loader held the shared expert as a q8 transcode of the file's
   Q4_K / Q6_K, twice the bytes the token reads. Three levers landed: (1) the loader keeps the
   shared expert's K-quant planes beside the q8 transcode the CPU and Metal chains read
   (`wsh*_fmt`, `wshk*_offs`; `IMAGE_VERSION` 36), and the whole-model driver places those - the
   twin's image 10107 -> 9756 MB, its token 7.06 -> 6.12 ms (sh_gate 793 -> 466 us, sh_up 776 ->
   457, sh_down 820 -> 558, the experts' own requant gone since they share the feed), tg128
   142.5 -> 162.7 (0.94x), pp512 5172.7; (2) the schedule's two per-expert slot walks became an
   atomic tally and an atomic cursor (`atomicAdd` on workgroup memory; the rows within a bucket
   land in an order nothing downstream reads, the kernel cell checks the map as a permutation):
   8.1 -> 0.46 ms on the 30B window; (3) the router tile is 64 x 32 with 4 x 2 blocks and the
   next K step's rows prefetched into registers as float4: 6.6 -> 4.3 ms. The 30B window 153.9
   -> 144.9 ms (llama.cpp 142.3), pp512 3448.9 (0.98x), tg128 124.6 (1.07x). With all three:
   the twin 5348.3 / 163.4 (5099.8 / 173.8: 1.05x / 0.94x; the six-rep pp mean still wanders
   with the first measured rep's driver warm-up), the 35B 2946.0 / 95.2 (2853.1 / 71.6: 1.03x /
   1.33x). A post-reboot run had read every role 5-15% slower and the 30B's tg128 at 119.7 - a
   throttled clock state (`nvidia-smi` beside the re-run showed the SM at 2827 MHz under load
   again), so a whole-row regression is re-measured before it is believed.
   5d DONE 2026-09-09 - the remainder pass, first lever: the routed combine folds into the
   residual step that follows it in both chains (`ClsArComb`, its f16 twin `ClsArCombF16B` for
   a next layer on the f16 feed; the residual step already read the FFN rows, so the fold takes
   one dispatch per layer out of the window and the token command, and `MoeCombineSh` goes). The
   fused step sums the FFN row first - the gated shared row, then the slots in order - and adds
   it to the residual, the order the two kernels took: the natural order (the residual first)
   moved every step's resident-vs-CPU difference and one step of the 35B two-window cell from
   0.39 to 1.50 logits against its 1.39 bar (a router near-tie downstream of the rounding); in
   the chain's order that step reads 0.39063567 again, the pre-fold value to the digit. The
   twin's token: ar2 360 us where ar2 + comb read 199 + 233, tg128 163.4 -> 165.4 (0.95x),
   pp512 5461.8 (1.07x); the 35B 2976.1 / 99.9 (1.04x / 1.40x); the 30B 3455.7 / 127.7 (0.98x
   / 1.10x), its window 143.7 ms against 142.3. A reading of the window profile: the stamp
   after the down tiles (`ar2+comb`, 4.7 ms; `comb` 4.1-4.8 before) absorbs the tiles' tail -
   the same kernel at one row costs 15 us, and 512 rows over 36 SMs cannot take 4 ms - so the
   window's remaining terms against the reference are the expert tiles (~98 ms against ~88),
   the attention head (~27), the router (4.3 against 1.2), the act 2.8, ar1 1.8, the gather
   1.6, and nothing in the combine. The store cleanup rode along: the cm2 tile picks its store
   layout by the column, not the stamp (a whole column stores unclamped on every stamp), and
   the per-stamp `FLO` typedef goes; the clamp on a whole column measured free (48.0 against
   48.1 TFLOP/s on the k4 m tile). A kernel-suite run during the pass read every cm2 cell 40x
   slow (k5 111 s against 1.1) while other programs held the box, and dastest's 1400 s timeout
   then died in its own watchdog (`timeout_tests` under the JIT reads a null); the same suite
   on the quiet box ran in 87 s - a run that looks 40x slow is the box, not the change.

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
