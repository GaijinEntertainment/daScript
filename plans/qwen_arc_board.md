# The Qwen arc board - every downloaded Qwen text model, one at a time, smallest first

The arc's working memory: which Qwen GGUF on the reference box works on the Vulkan tier, which
does not, and how each row stands against llama.cpp on the same box state. A model that does
not work is fixed before the next one is measured. The board is appended as the walk goes; the
final PR body of the arc quotes it.

## The rig

- Box: RTX 5060 Ti 16 GB (16311 MiB), Windows 11, driver 616.56, 16 compute lanes; the desktop
  holds ~0.65 GB of dedicated video memory fresh (a browser and a chat client took it to 2.9 GB).
- Models: `D:/Work/llama.cpp/models/`. Reference: llama.cpp b10660, `build-vulkan-357`
  (`llama-bench -p 512 -n 128 -ngl 99 -fa 1 -t 16 -o md`).
- Ours: `benchmarks/lcpp_bench.das -- -m <gguf> -o md --for-debug-purposes` under
  `DASLLAMA_GPU=1 DASLLAMA_GPU_MIN_CTX=2048 DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=16`, no
  VRAM pin (the resident plan sizes itself against the OS's room). Debug-jit grade: the CPU tune
  sidecar is stale on this box, which the GPU rows do not read.
- Arms: **cm2** = NV_cooperative_matrix2 (the default on both engines); **KHR** = cooperative
  matrix without cm2 (ours `DASLLAMA_COOPMAT=mm`, theirs `GGML_VK_DISABLE_COOPMAT2=1`). The
  no-coopmat arm (`DASLLAMA_COOPMAT=sdot4 DASLLAMA_VK_FA=0` / `GGML_VK_DISABLE_COOPMAT=1`) is the
  end-of-arc sweep, `modules/dasLLAMA/followup_vulkan.md` item 42.
- "Works" = the resident driver arms (or declines with a named reason the per-op rails then serve),
  the bench's sanity gate passes (finite logits, the argmax the cm2 arm printed), and the rows
  come out. Parity = same or better; below 1.0x needs a stated reason.
- Script: the session scratchpad's `bench_model.sh <gguf>` runs the four rows and writes
  `model_<name>.md`; the numbers land here by hand.

## The roster (text decoders; the mmproj / ASR / Omni companions are other arcs)

| # | model | GB | arch | status | ours cm2 pp / tg | llama.cpp cm2 pp / tg | ratio | ours KHR pp / tg | llama.cpp KHR pp / tg |
|---|---|---|---|---|---|---|---|---|---|
| 1 | Qwen2.5-0.5B-Instruct-Q8_0 | 0.53 | qwen2 | works (fixed: the q/k/v bias arm) | 42355 / 411.8 | 32338 / 381.9 | 1.31 / 1.08 | 33375 / 411.6 | 25437 / 365.9 |
| 2 | Qwen3.5-0.8B-Q4_K_M (local requant, the hybrid test twin) | 0.53 | qwen35 hybrid | works; KHR re-measured on the word-stage kq tile (8580 -> 16052) | 16651 / 314.2 | 16752 / 280.0 | 0.99 / 1.12 | 16052 / 313.3 | 14760 / 280.3 |
| 3 | Qwen3-0.6B-Q8_0 | 0.64 | qwen3 | works; tg 0.95x = the dispatch count per token (see notes) | 29065 / 318.8 | 22736 / 335.9 | 1.28 / 0.95 | 21187 / 319.8 | 20919 / 330.6 |
| 4 | Qwen2.5-1.5B-Instruct-IQ3_XS | 0.73 | qwen2 (bias) + iq2s/iq3xxs planes | works; pp 0.91x = the grid formats' cm2 decode callbacks; KHR re-measured on the word-stage kq tile (4482 -> 12058) | 12517 / 264.7 | 13830 / 252.0 | 0.91 / 1.05 | 12058 / 263.8 | 10809 / 245.9 |
| 5 | Qwen3.5-0.8B-Q8_0 | 0.81 | qwen35 hybrid | works | 19222 / 272.5 | 16624 / 243.6 | 1.16 / 1.12 | 17007 / 274.0 | 14978 / 242.6 |
| 6 | Qwen2.5-1.5B-Instruct-Q8_0 | 1.65 | qwen2 | works; tg 0.93x (see notes) | 14080 / 180.1 | 14184 / 193.1 | 0.99 / 0.93 | 11309 / 181.2 | 10813 / 189.0 |
| 7 | Qwen3-4B-Instruct-2507-Q4_K_M | 2.50 | qwen3 | works; tg 0.93x (see notes); KHR re-measured on the word-stage kq tile (`followup_vulkan.md` item 42): 1564 -> 3051 -> 4764 | 5150 / 117.6 | 5142 / 126.3 | 1.00 / 0.93 | 4764 / 116.8 | 4221 / 125.7 |
| 8 | Qwen3-4B-Instruct-2507-Q5_K_M | 2.89 | qwen3 | works; tg 0.93x (see notes); KHR re-measured on the word-stage kq tile (1489 -> 4422) | 4700 / 104.8 | 4965 / 113.2 | 0.95 / 0.93 | 4422 / 102.9 | 4015 / 111.3 |
| 9 | Qwen3-4B-Instruct-2507-Q6_K | 3.31 | qwen3 | works; KHR re-measured on the word-stage kq tile (1581 -> 4284) | 4566 / 95.88 | 4762 / 97.50 | 0.96 / 0.98 | 4284 / 95.69 | 3911 / 96.81 |
| 10 | Qwen3-4B-Instruct-2507-Q8_0 | 4.28 | qwen3 | works; tg 0.94x (see notes) | 5907 / 79.84 | 5064 / 85.03 | 1.17 / 0.94 | 5304 / 79.70 | 4614 / 84.14 |
| 11 | Qwen3.5-4B-Q8_0 | 4.48 | qwen35 hybrid | works (fixed: the mirror's binding cap on a hybrid) | 5087 / 74.37 | 4120 / 74.78 | 1.23 / 0.99 | 4562 / 74.74 | 3657 / 74.46 |
| 12 | Qwen3.5-9B-IQ4_XS | 5.47 | qwen35 hybrid | works (re-measured no pin, ctx 262143); KHR re-measured on the word-stage kq tile (738 -> 2560) | 2700 / 62.94 | 2794 / 66.82 | 0.97 / 0.94 | 2560 / 62.70 | 2199 / 66.26 |
| 13 | Qwen3.5-9B-MTP-Q5_K_M | 6.64 | qwen35 hybrid | works; KHR re-measured on the word-stage kq tile (728 -> 2525) | 2743 / 57.29 | 2752 / 59.67 | 1.00 / 0.96 | 2525 / 57.33 | 2102 / 59.41 |
| 14 | Qwen3.5-9B-MTP-UD-Q5_K_XL | 6.87 | qwen35 hybrid | works (re-measured no pin; pp at 10 reps - the 5-rep run drifted, see notes); KHR re-measured on the word-stage kq tile (767 -> 1788): the one KHR row under parity - see notes | 2698 / 56.40 | 2783 / 58.12 | 0.97 / 0.97 | 1788 / 56.53 | 2098 / 57.86 |
| 15 | Qwen3.5-9B-Q8_0 | 9.53 | qwen35 hybrid | works (re-measured no pin, ctx 174167) | 3227 / 43.82 | 2799 / 43.99 | 1.15 / 1.00 | 2140 / 43.58 | 1871 / 43.95 |
| 16 | Qwen3.5-9B-MTP-Q8_0 | 9.79 | qwen35 hybrid | works | 3233 / 43.82 | 2797 / 44.03 | 1.16 / 1.00 | 2150 / 43.62 | 1873 / 44.00 |
| 17 | Qwen3.8-27B.i1-IQ3_S | 12.60 | qwen35 hybrid | works (re-measured no pin, ctx 30623; the pinned run read 752 / 24.95); KHR re-measured on the word-stage kq tile under the 14000 MB pin (238.9 -> 736.7) | 802.9 / 27.51 | 807.9 / 24.67 | 0.99 / 1.12 | 736.7 / 27.35 | 650.6 / 24.65 |
| 18 | Qwen3.8-27B-UD-Q3_K_XL | 13.15 | qwen35 hybrid | works (re-measured no pin, ctx 24651; the 5-rep run caught the stall at 803.9 +-33.8 / 25.29 - the row carries the flat 20-rep profiled run, see notes); KHR re-measured on the word-stage kq tile under the 14000 MB pin (228.2 -> 729.2) | 840.0 / 24.92 | 812.6 / 24.88 | 1.03 / 1.00 | 729.2 / 25.04 | 675.4 / 24.80 |
| 19 | Qwen3.8-27B-UD-IQ4_XS | 14.25 | qwen35 hybrid | works, no pin (cm2 columns); KHR re-measured on the word-stage kq tile under `DASLLAMA_GPU_VRAM_MB=14000` - the desktop holds 1.1 GB, which leaves the no-pin plan's KV room under the 2048-position minimum (`followup_vulkan.md` item 42): 221 -> 395 -> 741 | 865.4 / 23.51 | 814.6 / 24.13 | 1.06 / 0.97 | 740.7 / 23.24 | 675.2 / 24.07 |
| 20 | Qwen1.5-MoE-A2.7B-Chat.Q8_0 | 15.23 | qwen2moe | works (fixed: the per-op attention chain's bias arm + a 2048-wide kv cap); LAGS on the per-op MoE prefill structure and a span without a shared-expert arm (see notes; first run 390.2 / 29.31 = 0.11 / 0.56) | 506.1 / 39.04 | 3507 / 52.15 | 0.14 / 0.75 | 525.5 / 39.94 | 2185 / 51.84 |
| 21 | Qwen3-30B-A3B-Instruct-2507-Q4_K_M | 18.56 | qwen3moe | works on the per-op tier: experts of layers [13..48) resident, [0..13) streamed; llama.cpp -ngl 99 OOMs, its cells are the same-split offload (experts 0-12 on the CPU; -ngl 36 in the notes); KHR re-measured on the word-stage kq tile (499.9 -> 712.0, the run's split [14..48) resident) | 749.8 / 66.45 | 612.5 / 42.24 | 1.22 / 1.57 | 712.0 / 64.53 | 588.6 / 42.49 |
| 22 | Qwen3-Coder-30B-A3B-Instruct-Q4_K_M | 18.56 | qwen3moe | works on the per-op tier like row 21 (see notes 22 and 24 for its cm2 and first KHR rows); KHR prefill re-measured on the word-stage kq tile (494.3 -> 764.1); the same run's decode read 33.61 against 63.60 before and row 21's 64.53 in the same chain - the re-run met the lost GPU and is owed after the reboot | 745.3 / 65.50 | 608.8 / 40.65 | 1.22 / 1.61 | 764.1 / 33.61 | 572.6 / 41.81 |
| 23 | Qwen3.8-27B-Q4_K_M | 18.97 | qwen35 hybrid | does not fit 16 GB on either engine (llama.cpp OOM) | | | | | |
| 24 | Qwen3.6-35B-A3B-MTP-UD-Q4_K_M | 22.13 | qwen3moe hybrid | works on the per-op tier: deltanet triples of 30 layers, attention quads of 10, experts of [16..40) resident, [0..16) streamed; llama.cpp -ngl 99 OOMs, its cells = the same-split offload (experts 0-15 on the CPU; -ngl 24 in the notes); KHR re-measured on the word-stage kq tile (422.3 -> 665.4; that run's split [17..40) resident) | 657.6 / 36.39 | 414.1 / 36.76 | 1.59 / 0.99 | 665.4 / 36.86 | 393.4 / 37.06 |
| 25 | Qwen3-Coder-30B-A3B-Instruct-Q8_0 | 32.48 | qwen3moe | works on the per-op tier: attention quads of 48 layers, experts of [29..48) resident, [0..29) streamed; llama.cpp cells = the same-split offload (experts 0-28 on the CPU; -ngl 20 read 201.0 / 17.38) | 279.1 / 30.64 | 210.5 / 22.27 | 1.33 / 1.38 | 276.8 / 29.82 | 202.7 / 22.65 |
| 26 | Qwen3.6-35B-A3B-Q8_0 | 36.90 | qwen3moe hybrid | works on the per-op tier: 30 deltanet triples, 10 attention quads, experts of [27..40) resident, [0..27) streamed; llama.cpp cells = the same-split offload (-ngl 15 read 173.5 / 14.66); the KHR arm's prefill argmax is a near-tie flip (198 at 8.333 against 220 at 8.355), decode argmax and logits match | 221.4 / 28.61 | 177.8 / 25.84 | 1.25 / 1.11 | 239.0 / 28.90 | 176.3 / 26.16 |
| 27 | Qwen3-Coder-Next-Q4_K_M | 48.53 | qwen3next | works on the per-op tier: 132 dense planes, 48 shared experts, 36 deltanet triples, 12 attention quads resident, experts of [37..48) resident, [0..37) streamed; the FIRST run died out of host heap in the image mint (see notes); llama.cpp cells = the same-split offload (-ngl 13 read 115.4 / 13.88); both arms' prefill argmax are whitespace near-ties (198 vs 197), decode argmax matches; KHR re-measured on the word-stage kq tile (155.8 -> 319.9; that run's split [38..48) resident) | 381.3 / 27.76 | 118.1 / 22.14 | 3.23 / 1.25 | 319.9 / 27.05 | 116.4 / 22.50 |

The 9B rows 12/14/15 and the 27B rows 17/18 were measured 2026-09-06 with the grid-format GEMV
fix and the memory-priority flip already in; the 27B rows 17 and 18 ran under a VRAM pin below
the box's knee (the auto arm without the OS query declined them then). Row 19 is the no-pin
auto arm of 2026-09-07. Ratios read ours over llama.cpp.

## Per-model notes

### 1. Qwen2.5-0.5B-Instruct-Q8_0
First run: the resident driver declined the whole qwen2 family ("no arm for: attention qkv
bias") and the per-op rails served at 1228 / 104.7 (0.04x / 0.28x). Fix: the q/k/v projection
bias folded into the rope stage (decode, prefill, the fused qk-norm twin), the bias rows
uploaded once, the seat installed separately so a tier without it still declines by name
(`ARCHITECTURE_GPU_VULKAN_DECODE.md`, the bias paragraph). Second run found the demotion
check's counter query costing a prefill's worth of time on an 18 ms prefill - gated to once a
second. Parity gate: `tests/test_gpu_resident_qwen2.das` (forced-feed logits within 6% of the
CPU chain at one and two windows; the noise sits flat at 0.30-0.50 of a 10-18 max logit).

### 3. Qwen3-0.6B-Q8_0
Decode profile: GPU 2870 us/token (attn 550, gate 393, q 328, down 315, tail 387, the rest
small) against a host wall of 3141 us/token - the GPU work already matches llama.cpp's 2.98 ms
token; the 8.6% gap is the per-token host path (two memcpys, the submit, the fence wait, the
608 KB logits copy). Prefill reps on a ~20 ms window swing +-25-40% between runs (the mean at
or above llama.cpp); the same rig reads +-0.1% on the 27B.

### 6-10. The 1.5B and 4B dense rows' decode (0.93-0.98x)
Profile on Qwen3-4B-Instruct-2507-Q4_K_M, both engines, per token: ours GPU 8232 us under an
8545 us host wall; llama.cpp's per-op logger sums 8160 us against its 7.92 ms free-running token.
The GEMVs favor us - qkv 904 vs 1292, gate+up 2530 vs 2652, down 1582 vs 1606, classifier 752 vs
750 - and pay 403 us of activation requant llama.cpp does not have (net +131 us ours). Three fixed
costs carry the gap: decode attention 650 vs 306 us (18 vs 8.5 us per layer at a short context -
a per-dispatch floor, the 0.6B reads the same 19.6 us/layer), the residual add + norm 601 vs 312
us (theirs folds the add into the wo/down GEMV: MUL_MAT_ADD), and the host path 313 us (two
memcpys, the submit, the fence wait, the logits copy). Together ~0.9 ms of an 8.5 ms token; the
same three sit near 1% each on the 27B. Q6_K reads 0.98x because llama.cpp's q6_K GEMV runs at
963 GFLOPS against 1326 for q4_K. Levers for after the walk: the attention kernel's per-layer
floor, the fused residual add, a fence poll in place of the blocking wait.

### 12-16. The 9B block, and the prefill rep swing
Every 9B file arms without a pin (ctx 262143 on the IQ4_XS - the binding cap - down to 174167
on the Q8_0 files, the VRAM fit). Prefill on our side swings between runs on some files and not
others: UD-Q5_K_XL read 2537 +-132 at 5 reps, then 2667 +-48 at 10 reps (five reps at 2698-2708,
then 2620, 2644, 2661, 2672, 2561 - a step down and a drift over the second second of load), then
2698 +-8 at 10 reps under a 250 ms clock sampler (SM 2760-2820 MHz, 122-165 W of the 180 W limit,
50-56 C, no throttle bit, llama.cpp's window the same clocks). Decode never moves (56.3-56.6 across
30 reps). The demotion check is not it (the whole OS memory query costs 45 us). The 27B Q3_K_XL
shows the event's shape at 20 reps: 845 844 842 845 844 | 516 | 786 785 831 828 830 816 826 828
832 833 833 844 843 844 - ONE rep loses 0.39 s outright, then the rate climbs back over ten reps;
decode over the same 20 reps sits at 25.25-25.28. Under the per-window profile the same file ran
20 reps flat at 840.0 +-0.9 (host prep 1.0-1.5 ms, record 2.7-2.9 ms, GPU 585 ms per window, every
window alike), and the box has 256 GB of RAM with 236 GB free, so neither the host chain nor memory
pressure is the stall. Seen in 3 of ~12 rows (12, 14, 18), twice at the sixth timed prefill, never
under either sampler run. Open: a one-shot stall of ~0.4 s with a paging-shaped recovery - an
interfering GPU client, or the memory manager taking the idle gap. From row 20 on the rig samples
clocks, utilization and memory.used at 100 ms beside every run of ours, so the next occurrence is
caught. Rows 14 and 18 carry their flat repeat runs.

### 11. Qwen3.5-4B-Q8_0
First run: the resident driver declined at the device prepare - "KV mirror (8 x 262144 x 1024)
exceeds maxStorageBufferRange 4294967295" - and the per-op rails served with the dense FFN of 32
layers on the CPU: 254.9 / 15.37 (0.06x / 0.21x). The context negotiation read the mirror's
per-position stride from layer 0, which is recurrent on Qwen3.5 (kv width 0), so the stride was
0 and the binding cap was skipped; the file's 262144 context then put one mirror side exactly
one byte past the 4 GiB range. The 0.8B hybrid armed at 262144 only because its side is 1536 MB;
the 9B's weights push it down the VRAM path first. Fix: `resident_binding_ctx` sums the rows
across the layers (a recurrent layer adds 0), cited in `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`
2.2n; model-free cell in `tests/test_gpu_serving_declines.das`.

### 20. Qwen1.5-MoE-A2.7B-Chat.Q8_0
The first MoE row, served by the per-op tier (the resident driver is dense-only by design). Two
causes for 0.11x / 0.56x, both named in the load: "the attention-quad rail declined - no arm for:
attention qkv bias" - every layer's attention runs on the CPU with a device round trip per layer
(the resident driver's bias arm from row 1 lives in the rope kernels the per-op chain also binds,
but the chain still binds the placeholder and the prefill's `AtPrep` stage has no bias arm at all;
`attn_dec_shape_ok` and the rail's `arch_ok` both decline on the flag) - and "expert stacks of
layers [2..24) resident, [0..2) streamed": 14.5 GB of Q8 experts on a 16 GB card, the per-op tier's
budget keeping the 1 GiB reserve llama.cpp does not (its 14.17 GiB image sits at the brim at 3584 /
52.09). Sanity argmax matches across arms. The fix is the per-op attention chain's bias arm: the
hook signatures gain the layer's bias rows beside rmsq/rmsk, the decode chain binds the rows and
sets boff/hasb, `AtPrep` gains the binding and the add before rope, the rail and the shape gate
drop the flag when the seat is installed; cells in test_vulkan_dec_tail (bias arm) and the AtPrep
kernel unit. The family is qwen2moe (Qwen1.5-MoE, Qwen2-57B-A14B), gpt-oss and glm4moe also carry
the flag. DONE (Boris pulled it forward): the hooks carry the layer's bias rows (the prefill hook
drops the derivable kv_mul to stay under daslang's 32-argument invoke cap), the decode block binds
the row to the rope kernels' existing arm, `AtPrep` gains the binding with a third in-place pass
over the raw v window, the span carries the rows too; model-free arms in the kernel prep cell,
the tier's attention chain and the decode tail. The next gate then fired - "geometry outside the
chain (hs 128, qd 2048, kvd 2048)": 16 heads of 128 with no GQA is a 2048-wide kv, the chain's
planes were sized for 1024 - so `AT_MAX_KV` is 2048 (~48 MB more for the k/v planes and their f16
shadows). With both: attention quads of 24 layers resident, 506.1 / 39.04 (0.14x / 0.75x), sanity
argmax unchanged. What remains is structural: prefill rides the per-op tier's per-stage synchronous
chain (attention submit, CPU kv store + routing + gather, FFN submit, per layer) at ~1.7 TFLOPS
effective against llama.cpp's whole graph on the device (its 14.17 GiB image fits where our 1 GiB
reserve streams 3-4 layers); decode runs per-layer submits because the whole-token span declines a
shared expert (`span_model_ok`: `n_ff_shexp > 0`) - a shared-expert stage in the span is the lever
for tg, the MoE prefill chain is an arc-level item (the resident driver is dense-only).

### 21. Qwen3-30B-A3B-Instruct-2507-Q4_K_M (and the MoE reference protocol)
An 18.5 GB file on a 16 GB card: llama.cpp at `-ngl 99` fails the load (a 1 GB device allocation
refused), so its rows are the two offload settings the 30B arc settled on: the experts of layers
0-12 on the CPU (`-ot 'blk\.([0-9]|1[0-2])\.ffn_.*_exps=CPU'`, the split that mirrors our streamed
layers) - cm2 612.5 / 42.24, KHR 588.6 / 42.49 - and `-ngl 36`, their best decode then - cm2 597.7 /
45.53, KHR 566.1 / 45.55. Ours 749.8 / 66.45 (cm2) is 1.22x / 1.57x of the same-split row and 1.25x
/ 1.46x of their best decode; when that arc merged this file read 348.4 / 35.5 against their 566.4 /
37.0. The KHR arm's 499.9 is the kq-tile gap again (0.85x of their KHR same-split row); decode 1.52x.
Every MoE row past this one that does not fit at -ngl 99 uses the same two settings.

### 22 and 24. Qwen3-Coder-30B-A3B Q4_K_M, Qwen3.6-35B-A3B-MTP UD-Q4_K_M
The Coder twin of row 21 reads the same: 745.3 / 65.50 against 608.8 / 40.65 same-split (1.22x /
1.61x) and 582.0 / 45.14 at -ngl 36; KHR 494.3 / 63.60 against 572.6 / 41.81. The 35B hybrid (22 GB,
30 deltanet + 10 attention layers) streams 16 layers' experts: 657.6 / 36.39 against llama.cpp's
same split 414.1 / 36.76 (1.59x / 0.99x) and against -ngl 24, which is worse for them (346.3 /
21.23: the layer offload takes the recurrent layers off the device too, the expert split keeps
them on). KHR 422.3 / 37.03 against 393.4 / 37.06 (1.07x / 1.00x) - the kq-tile gap narrows here
because the streamed half of the prefill is PCIe-bound on both arms.

### 25-27. The files past the card (32-48 GB)
All three run on the per-op tier with most layers' experts streamed from pinned host memory, and
all three stay ahead of llama.cpp at the matching split (its `-ot` puts those experts on the CPU):
the Coder-30B Q8 1.33x / 1.38x (29 of 48 layers streamed), the 35B Q8 1.25x / 1.11x (27 of 40),
and Coder-Next Q4 3.23x / 1.25x (37 of 48). The Coder-Next arithmetic says why the last one is
so far ahead: 512 experts of which 10 fire, 1.6 MB each in Q4, so a token streams ~590 MB across
37 layers - 27 ms at the slot's ~22 GB/s - and a 512-row window reads each streamed stack once,
30 GB in 1.34 s; llama.cpp's CPU pass over the same experts takes 4.3 s. None of these is an
interactive configuration on this card (the Q8 files read 28-31 tok/s, the interactive picks are
the Q4 30B/35B rows at 66 / 36 tok/s); they prove the zoo runs and where the bus binds. The KHR
arm's prefill gap widens with streaming (Coder-Next 155.8 against 381.3): the streamed stacks are
kq planes, which had no KHR tile when these rows were taken, and their GEMMs are the whole window. Both arms' prefill argmax
on the biggest files are whitespace near-ties (a space, a newline, a tab within 0.05 logits);
decode argmax and logits match across arms on every row.

The Coder-Next's first run died: "out of heap memory, requested 91 GB, used 232 GB" on a 256 GB
box, because D: had filled with the day's 37 debug-flavor `.dlim` images (293 GB), the image save
aborted mid-plane, and the mint path then loaded the model a second time beside the first
(`followup_general.md` item 120; the rig runs `DASLLAMA_IMAGE=0` since). The re-run loaded once,
in 13 s from the page cache.

### 19. Qwen3.8-27B-UD-IQ4_XS
Resident image 12899 MB (+1.4% over the file's blocks). Decode profile against llama.cpp's
per-op logger: GEMVs 37.7 vs 37.0 ms/token (gate+up faster than theirs, the beta/alpha pair 0.46
ms slower - the long-rows-few-outputs GEMV shape), small ops equal; the residual add is fused
into their down/out mat-vecs. Levers left: the beta/alpha GEMV shape, the fused residual add
(~1% each). On the KHR arm prefill was 0.33x before the kq formats had a KHR-coopmat tile, 0.59x
with it, 0.73x after its slab fix, and 1.10x on the word-stage tile with f16 accumulators the row
above carries (`followup_vulkan.md` item 42).
