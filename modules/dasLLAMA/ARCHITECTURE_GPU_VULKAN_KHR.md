# dasLLAMA Architecture - the Vulkan tier's KHR staged tile

Companion to `ARCHITECTURE_GPU_VULKAN_GEMM.md`; a section is cited by its anchor. This document carries the KHR
cooperative-matrix arm of the cm2 template - the tile that dequantizes a plane's own bytes to f16 in shared memory ahead
of the KHR multiply - and the K-quant window form a coopmat2 card whose driver lists no decode-vector takes on it. The cm2
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
