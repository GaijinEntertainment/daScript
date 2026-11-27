# dasLLAMA Architecture - the Vulkan tier's KHR staged tile and int8 tiles

Companion to `ARCHITECTURE_GPU_VULKAN_GEMM.md`; a section is cited by its anchor. This document carries the window
GEMMs over a file's f32 rows that ride the KHR cooperative-matrix arm of the cm2 template on every coopmat card, and the
int8 tiles of the routed block on the KHR s8 matrices. The arm
itself - the tile that dequantizes a plane's own bytes to f16 in shared memory ahead of the KHR multiply - is
`ARCHITECTURE_GPU_SHARED_GEMM.md#khr-mm-kq-tile`. The cm2
tiles, their decode callbacks and the tile pick both forms share are `ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes`
and `ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`.

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
at ad39b47963, each figure an arm's best round, `PERF_LEDGER.md`) the scalar tile reads 3.1 TFLOP/s at both E2B
shapes and the staged tile 75-80. `DASLLAMA_VK_PLE_F32_TILE=0`
holds the scalar tile, the same-build A/B.

**A K-quant window plane stays on the cm2 tiles whichever decode arm the driver runs.** On the
scalar arm (a driver without `VK_NV_cooperative_matrix_decode_vector`) the cm2 l stamp takes
7-25% less time than the staged tile at every FFN, q and wo shape of the E2B and the 4B for k4,
k5, k6 and iq4xs at whole 512-row windows, the staged tile reads level with the m stamp, and the
two read within 8% of each other at the 256-wide shapes (`./bin/daslang -jit
modules/dasLLAMA/harness/vk_gemm_probe.das -- kqst:<fmt>` at ad39b47963, the same box); the staged tile's
128-token column and its two staging arrays give up the l column's reach, which the decode's
cost on the scalar arm does not buy back. A window off the l column (the gate at 500 rows) turns
the order: the l stamp takes its edge path, and the staged tile reads 2-11% under the m stamp
there (`followup_vulkan.md` row 149).

### The routed block's pieces ride int8 tiles where the served set names their column {#khr-int8-tiles}

**An int8 tile multiplies a weight plane's integer codes by rows quantized to int8, and applies the scales after the
product.** `KqMmqKhrT` (`dasllama_vulkan_classes`) is the window tile on the KHR s8 cooperative matrices, one body every
format stamps: 128 weight rows by `TOKS` rows a workgroup - the m stamp 128 rows on 512 lanes (`<format>_batch_mmq_cls`),
the s stamp 32 rows on 128 lanes (`<format>_batch_mmqs_cls`) - over the expert schedule's records and maps, so a column
dispatches it with the push constants and the workgroup count of its f16 tile
(`ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block`). The f16 tile dequantizes a weight block to f16 on its stage and
reads a gathered f16 image of the rows; the int8 tile stages the block's codes as they are and reads a megabyte of int8
rows, and on a device whose tiles are bound by what they load that is the faster form: on the Radeon 8060S the 35B's gate
and up plane (Q4_K, 512 rows by K 2048, the window's routed buckets) and its down plane (Q5_K, 2048 rows by K 512) each
take less time on the int8 tile than on the f16 tile (`PERF_LEDGER.md`, the entry on the routed block's int8 tiles
against its f16 tiles, which holds every device reading of this section; the probe's arm is `moeq`).

- **The feed** (`MmqFeedT`, one lane a row's 32-block) writes the block's 32 int8 - `round(x * 127 / max |x|)` - and two
  floats beside them: the block's scale, `max |x| / 127`, and the scale times the sum of the quants. `mmq_feed_cls` reads
  the window's f32 normed rows for the gate and up planes, `mmq_feed16_cls` the f16 hidden rows for the down plane.
- **The row map** (`MmqInv`) inverts the schedule's slot grid into each bucket row's token row. The tile reads a bucket
  row's quants through it (`tok`), so an int8 gate or up column needs no gathered image; the down plane's rows are already
  in bucket order and its map is the identity ramp.
- **A format is four hooks** - a block's plane words (`w_load0`, `w_load1`), its scale and min term (`w_scale`), its codes
  into the stage (`w_put`) - and the count of blocks a weight tile holds (`plane_tile`). Q4_K stages its nibbles, Q5_K the
  nibbles with their fifth bit, Q8_0 its bytes, Q6_K its six-bit codes less 32; `mmq_cls_has` names the formats that have
  the hooks, read off `MMQ_SKIP`. A Q4_K or Q5_K weight is `scale * code - min`, so a block's dot is `scale_w * scale_x * sum(code * q) - min *
  scale_x * sum(q)`: the integer product gives the first sum and the feed's second float is the second term's factor,
  which is why the feed stores it. Q6_K carries a scale a 16-element half and no min term (`HALF2`): its block's two
  16-deep products keep an accumulator each, and a lane scales each by its half's scale. Q5_1 and the per-32 Q5_0 keep
  20-byte blocks, which no 16-byte word holds whole, so their stamps bind the plane as words (`W32`): Q5_1's weight is
  `d * code + m`, the min arm with m added, and Q5_0's `d * (code - 16)`, the code less 16 as the int8. Q2_0's block is
  two code words (`W32`), each byte's four codes spread to the int8 `code - 1`, its d its two-block group's: no min term.
  Q1_0's block is one sign word (`W32`), each bit spread to the int8 +-1 under its four-block group's d: no min term.
- **A step is two 32-blocks of K, loaded a step ahead.** A lane's stage job - a weight row's block, a row's block - is read
  into registers before the multiply of the step in flight and written to workgroup memory after that step's barrier, so
  the loads of the next step are in the air while the tiles multiply. The K walk starts two blocks a weight tile of the
  plane further in and wraps, as the rotated f16 tile's does (`ARCHITECTURE_GPU_VULKAN_GEMM.md#vk-alt-stamps`).
- **A 32-block is two products of depth 16 into one s32 accumulator.** A 16 x 32 s8 matrix multiplies whole on the AMD
  Windows driver, and on RADV only its first sixteen columns reach the product; two 16 x 16 x 16 products are the form
  both drivers multiply whole, at the same time a plane on the 8060S.
- **A lane scales its own elements.** A subgroup owns a 32 x 32 block of the output - two 16-row weight fragments against
  two 16-row row fragments - and each lane keeps 32 f32 sums: after a product it reads its eight elements of the s32
  accumulator (`coopmatElem` at a constant index - an extract from the tile's value, which leaves the tile in its
  registers where a run-time index moves it to addressable storage) and adds each, times the two scales, into the sum of that
  element's (weight row, row). 32 sums a lane is the size the lanes hold: a 64-sum subgroup, or a lane-local array of the
  step's scales, spills them and the tile runs at half its rate.

**Which element a lane reads is the device's, so the tiles serve behind a device check.** The body is written to the
RDNA3 accumulator layout - lane L holds column L mod 16 and rows L / 16 + 2e of a 16 x 16 product, e its element index.
`vk_mmq_device_ok` runs `MmqProbe` once a process on one subgroup: a product whose element (r, c) is 17 (r + 1) + c, every
lane writing its eight elements, and two 16-deep products of -1 by 1 .. 32 into one accumulator, which read -528 where the
whole depth multiplies signed. A device that reads any of the 288 values otherwise keeps the f16 tiles and says so once.

**A layer with an int8 column takes the ordered bucket schedule.** The schedule's plain stamp hands out a bucket's rows
by atomic arrival, so which slot row gets which bucket row differs from pass to pass. That is free where every piece of a
bucket rides one tile family: a row's sum is the same wherever it lands. It is not free here: a bucket past one m column
puts its first rows on the m tile and its remainder on the s tile, the int8 m and s tiles are two shaders that need not
round alike, and a column's m pieces can ride the f16 tile beside int8 s pieces - so a row's place in the bucket picks its
arithmetic, and the same prompt read other logits each pass. `MoeSchedT` under `STABLE` (`moe_sched_ord_cls`,
`moe_sched2_ord_cls`, `place_in_order`) places the slot rows `WG` a round, each at its expert's cursor plus the round's
earlier rows of that expert, the cursors moved once the round has read them: bucket rows in slot-row order, the CPU
twin's map word for word. The window dispatches it on every layer with an int8 column (`pf_moe_block`); a layer with none
keeps the plain stamp.

**The served set carries a bit a column** (`VK_ALT_MMQ_GU_M`, `VK_ALT_MMQ_GU_S`, `VK_ALT_MMQ_DN_M`, `VK_ALT_MMQ_DN_S`,
`vk_mmq_col`): the gate and up m pieces, their s pieces, the down's m pieces, its s pieces. A layer's column rides the
int8 tile where its bit is on, the device is in mm mode and passes the check, the planes' formats have the hooks, the
plane's K is whole steps of the tile (`mmq_cls_fits`: superblocks of a superblock format, pairs of 32-blocks of any other)
and - for gate and up - the experts carry no bias rows; every other column keeps its f16
tile, and the two tiles of one plane write disjoint rows. The gate and up columns share one feed and one gather: with both
bits on the gather is not dispatched, and with one on the layer pays both. `vk_alt_mmq_by_vendor` sets the bits with no
race: an AMD device takes the gate and up pieces whole and the down's s pieces (`VK_ALT_MMQ_AMD`), and the down's m pieces
stay f16 - at K 512 the f16 m tile is the faster one on both AMD boxes, the Radeon 8060S and the Radeon 760M, and a Q6_K
down plane reads the same way: its s pieces faster on the int8 tile, its m pieces on the f16 one. The isolated tiles are
the wrong instrument for the pick, as the decode GEMV's are: on the 760M the int8 s tile at the gate and up shape is the
slower tile alone, and the served window is faster with it (the Qwen3.6-35B-A3B Q4_K_M at 512 tokens,
`benchmarks/lcpp_bench.das -p 512 -n 0 -r 3`: on both boxes the rule's three columns give the best rate, ahead of all
four and of none). Any other vendor takes none; `DASLLAMA_VK_ALT_STAMPS` pins any column on any device that passes the check.

The cells: `test_vkd_mmq_tiles` (both stamps of Q8_0, Q4_K, Q5_K, Q6_K, Q5_1 and the per-32 Q5_0 against the f16 oracles, over rows the int8 hold
exactly and a row map that is not the identity), `test_vkd_mmq_feed`, `test_vkd_mmq_inv` and `test_vkd_mmq_device_check`
(`tests/test_vulkan_kernels_gemm.das`), and `test_vk_alt_mmq_by_vendor` (`tests/test_gpu_serving_declines.das`).
