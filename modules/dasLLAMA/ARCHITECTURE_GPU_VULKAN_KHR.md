# dasLLAMA Architecture - the Vulkan tier's KHR staged tile

Companion to `ARCHITECTURE_GPU_VULKAN_GEMM.md`; a section is cited by its anchor. This document carries the KHR
cooperative-matrix arm of the cm2 template - the tile that dequantizes a plane's own bytes to f16 in shared memory ahead
of the KHR multiply - and the window GEMMs over a file's f32 rows that ride it on every coopmat card. The cm2
tiles, their decode callbacks and the tile pick both forms share are `ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes`
and `ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`.

### The KHR arm's hand-staged kq tile {#khr-mm-kq-tile}

**The mm mode serves the kq formats through the same template's KHR arm.** A device with
KHR_cooperative_matrix and no NV_coopmat2 (every AMD and Intel part, the GTX and Turing lines)
has no decode-in-load tensor API, so the cm2 template carries a second body under its `KHR`
axis, staged by hand. Each of the 256 threads writes one weight row's 16-wide half of the
32-wide k step. The weights go through the format's `khr_stage16`. It reads the row's 16
consecutive values from the quant plane as one or two 16-byte words (`wq4`, the plane aliased
as `uint4` on its binding; the byte-granular grid formats read 32-bit words through `wqw`),
reads the row's scale words once, and writes the 16 values into the stage as f16 pairs. The
activations come straight from the f16 plane as two 16-byte words (that plane aliased as
`uint4` on its binding), stored as they arrive. Both land in `@workgroup` `uint` arrays at a
stride of 20 words (16 plus 4 pad, so the fragment loads spread across banks). q8 arrives here too
(`Q8KhrBatch`, its stage a block's 16-weight half as four int8 words under the block's f16 scale), so a KHR-mode
card's dense q8 GEMMs take the f16 feed; the q8-fed mul_mm L-tile serves the sdot4 and int8 modes.

**The eight subgroups tile the 128 x 128 step two by four.** Each owns 64 weight rows against
32 tokens as eight 16x16 f16 accumulators (a `coopmatAcc_f16_16x16[8]` walked under
`for [unroll_full]`, so every fragment sits in registers) and loads four weight fragments and
two token fragments per 16-deep multiply-add round: 96 fragment loads per workgroup per k step
against 144 when every subgroup owned a 16-row strip across all 128 tokens. The accumulators
widen to f32 (`coopmatConvert`) before the store. A whole tile stores its fragments straight
into y (token-major, so a fragment's (weight, token) is `y[token * d + weight]`); an edge tile
bounces each widened fragment through the weight staging array - free once the k loop ends,
and its 2560 words hold the eight subgroups' 256-word fragments - and writes under the row and
column bounds through a bit cast. Moving any of the three levers back - the word stage, the f16
accumulator width, the two-by-four tiling - costs rate, the word stage the most and the tiling the
least; the probe's `khrx` arms measure them (`ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md#vk-gemm-probe`)
and `followup_vulkan.md` item 42 keeps the figures. On a partial token column the edge store costs
nothing beyond the padded rows: per computed row a 300-token window runs at the whole-window rate.

**The two staging arrays are the whole footprint by design.** A third array for the bounce
costs the tile a seventh of its rate, because 8 KB more shared memory per workgroup is one
workgroup fewer per SM. The k step is 32 (the reference exe's BK): a 64-deep step doubles the
staging tiles, halves the workgroups an SM holds, and with the 8 KB iq2s grid beside them
reaches the 49152 B of workgroup memory the tier requires of a device - the floor the tile is sized to; a device offering less is not a target.

**The arm exists at one geometry** - 128 weights by 128 tokens, k step 32 - so in mm mode the
tile pick answers 128 and the wave model weighs its k chunks alone (a deep, narrow GEMM splits k
into the same scratch planes and reduce the cm2 tiles use), and `cm2_cls_ensure/set/enc` take the KHR
tile as their tail-0 arm (`cm2_tile_tail`), the same `(fmt)` key the kernel cells' `khr_cls_*` ladder stamps. The f16 feed admits a kq format in mm mode
only on a 32-lane subgroup (`khr_kq_tile_on`): the body indexes eight subgroups over the tile, so
a wave64 device (four subgroups per 256-thread workgroup) keeps its kq planes on the sdot4 tile.

### A file's f32 rows ride the staged tile in the prefill window {#f32-rows-staged-tile}

**The per-layer-embedding gate and projection over a file's f32 rows run on the KHR staged tile
wherever the KHR kq tile serves** (`khr_kq_tile_on`: KHR cooperative matrices at subgroup 32, a
cm2 card included) and the model dim and the ple width sit on its 32-deep k step (`ple_f32_tile_ok`:
the f32 rows carry no block padding, so a row off it would read past its exactly sized plane), else
on the scalar f32 tile (`DnBaGemm`). The gemma-4 E-series files carry
those two planes as F32 whatever the file's quant mix (the E2B Q4_K_M's `inp_gate` and `proj`
among them). The stamp (`F32KhrBatch`) is the template's KHR arm over a block of four f32 words: a
thread's 16 weights are four 16-byte words of its row, rounded to f16 pairs on the stage, so the
plane's own words are what every dispatch reads and nothing of the rounding outlives the dispatch.
The window feeds both GEMMs f16 rows - the stream rows through the clamp convert ahead of the gate,
the act's gelu x side rows through `ActF16`'s strided side read ahead of the projection - and the
region record's block base is the layer's first element over four. The tile's one geometry
serves the 256-deep projection whole, and the gate over the most 256-aligned k chunks up to eight
`cm2_chunk` admits and whose partial planes fit the window's split-k scratch (`pf_f32_split` through
`cm2_split_fit`: six on the E2B's 1536, five on the E4B's 2560, fewer on a card whose scratch holds
fewer) into the split-k planes and their reduce: the gate is eight workgroups at 512 rows, so one dispatch's
latency is its 48 k steps in a row, and the window pays that latency, not the throughput (the
E2B gate one dispatch at a time 53.8 us whole, 17.0 split six ways).
On the RTX PRO 4500 (driver 580.159.04, `./bin/daslang -jit modules/dasLLAMA/harness/vk_gemm_probe.das -- plef32`
at 0fea2d9f3a, each figure an arm's best round, `PERF_LEDGER.md`) the scalar tile reads 3.1 TFLOP/s at both E2B
shapes and the staged tile 75-79. `DASLLAMA_VK_PLE_F32_TILE=0`
holds the scalar tile, the same-build A/B.

**A K-quant window plane stays on the cm2 tiles whichever decode arm the driver runs.** On the
scalar arm (a driver without `VK_NV_cooperative_matrix_decode_vector`) the cm2 l stamp takes
7-25% less time than the staged tile at every FFN, q and wo shape of the E2B and the 4B for k4,
k5, k6 and iq4xs at whole 512-row windows, the staged tile reads level with the m stamp, and the
two read within 7% of each other at the 256-wide shapes (`./bin/daslang -jit
modules/dasLLAMA/harness/vk_gemm_probe.das -- kqst:<fmt>` at 0fea2d9f3a, the same box); the staged tile's
128-token column and its two staging arrays give up the l column's reach, which the decode's
cost on the scalar arm does not buy back. A window off the l column (the gate at 500 rows) turns
the order: the l stamp takes its edge path, and the staged tile reads 2-11% under the m stamp
there (`followup_vulkan.md` row 149).

