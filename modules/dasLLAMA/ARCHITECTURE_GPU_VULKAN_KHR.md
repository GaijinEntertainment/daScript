# dasLLAMA Architecture - the Vulkan tier's KHR staged tile

Companion to `ARCHITECTURE_GPU_VULKAN_GEMM.md`; a section is cited by its anchor. This document carries the window
GEMMs over a file's f32 rows that ride the KHR cooperative-matrix arm of the cm2 template on every coopmat card. The arm
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

