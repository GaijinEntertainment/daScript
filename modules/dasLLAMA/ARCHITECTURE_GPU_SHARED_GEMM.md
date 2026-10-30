# dasLLAMA Architecture - the GEMM tiles and GEMVs both discrete homes stamp

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries the bodies of the prefill's
batch GEMM tiles and of the decode GEMV family that `dasllama_gpu_kernels_discrete.das` holds for the Vulkan and CUDA
homes: how a cm2 tile's decode callbacks read their quant bytes, the mx4 decode, the device scale row the decodes read, the
tile body's k step, fast path and loop bounds, the KHR arm's hand-staged tile, the GEMV family's lane split and region walk,
and the routed gate-up fusion. How the Vulkan driver picks a tile, a k split and a GEMM mode is
`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`; the window GEMMs over a file's f32 rows that ride the KHR tile
are `ARCHITECTURE_GPU_VULKAN_KHR.md#f32-rows-staged-tile`.

### The cm2 decode callbacks read their quant bytes as 16-bit lanes {#cm2-decode-16bit-lanes}

A cm2 tile's decode callback runs inside the driver's block load, whose shader compiler
pattern-matches one load width into that path: a 16-bit load (`int16[N]` block members) with
sub-fields pulled out by shift and mask. A 32-bit word with a variable shift runs slower, and an
`unpack8` of a 32-bit word indexed by a runtime value drops the whole kernel off the block-load
path, to about a third of the rate - so every cm2 decode, q8, q51 and every kq superblock format,
is spelled the 16-bit way (the block structs are `int16` arrays over the same bytes), a byte at a
runtime position comes out of its lane by a shift, `(uint(int(blk.qs[i >> 1u])) & 0xFFFFu) >>
((i & 1u) * 8u)`, not an `unpack8(w)[i & 1u]` byte2 select (the same lane, but a decode built
on selects runs slower on the expert-schedule shape - `moe:<fmt>`, RTX 5060 Ti: iq2xxs 1.28x,
iq3xxs 1.24x, iq3s 1.49x, iq2s 1.08x), and a sign index straddling two bytes (the IQ2_XXS and
IQ3_XXS aux32 words) is assembled from its two lanes and shifted. The Q8_0 tile is the measured
exception: `Q8Cm2T.decode` selects its byte from the 16-bit lane with `unpack8`, and the shift
form read 1.5% under it on the dense prefill (Qwen3-4B Q8_0 pp512, RTX PRO 4500) - the dense shape
is bound elsewhere than the expert schedule the superblock readings came from. The
scalar callback is written in PAIR form: every read the two elements of an aligned pair share -
the grid byte, the sign word, the scale row - is derived from the pair's first element (`e &
~1u`), both values are computed and the element is selected last, because the driver runs the
scalar callback two elements at a time and commons what the bodies share (the RTX 5080's scalar
arm: the iq2xxs and iq2s expert stamps 11-13% faster on the skewed schedule, `moesk:iq2xxs` e+s
0.357 -> 0.316 ms, `moesk:iq2s` 0.428 -> 0.380, the 35B's pp512 3987 -> 4201). Every table a
decode reads at a runtime index is staged into a `@workgroup` array ahead of the tile loop, never
selected out of a register vector per element: the iq4 formats' 16-entry codebook
(`kvalues_iq4nl`, IQ4_XS and IQ4_NL) as f16, each grid format's codebook as its own word array -
the reference exe's shared-memory table staging (`ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig`).

A format whose sub-block scale takes an unpack per element stages a scale cache too (the
template's `SCACHE` axis): at a superblock's first k step the workgroup fills `sc_cache` - the
tile's 128 rows by eight sub-blocks of a premultiplied pair, two threads a row, a barrier either
side - and the decode reads its element's pair as one word, `sc_cache[g * SC_STRIDE + (row &
127u)]`, sub-block-major at a stride of 130 against bank conflicts. k4 and k5 cache `(d x sc,
dmin x mn)` from their five-word scale row in place of three scale-plane loads, a half unpack
and two multiplies per element; IQ4_XS (`SCIQ4`) caches `d x (ls - 32)` from its two-word row in
place of two loads and the six-bit rebuild; k6 (`SCK6`) caches `d x sc` for its sixteen int8
sub-block scales in place of two word loads, a sign extend, a half unpack and two multiplies an element
(its decode ran twice the reference exe's, whose tile stages scales for Q4_K and Q5_K alone); the grid formats' strips are read once per pair already. The refill keys on the k step (`sc_step`, `k % BLKW == 0`), and
`cm2_gemm_pick` cuts every chunk at a 256-aligned k, so no chunk boundary falls inside a superblock.
The pair array exists only on the formats that read it (the `SCPAIR` axis, every `SCACHE` format but k6,
which carries `sc6_cache` alone): a workgroup-array gate takes one axis name, and the 16 KB pair array
an unused gate left in the k6 tiles cost the CUDA home's k6 leaves their second block an SM.

Every kq format's four-wide twin is hand-written (`decode_v4`, the template's `DECV4` axis) in
the same spelling, sharing what four consecutive elements share - a K-quant twin reads its four
quant bytes as two 16-bit lanes and takes the sub-block's scale pair once, a grid format's twin
looks its grid word up once for the four bytes and four sign bits. The synthesized twin
(`DECVEC`, where a new format starts) repeats the whole scalar body four times and loses to the
scalar callback on the grid formats. The twin computes each element in the scalar's operation
order, so the tile's CPU oracle holds under either; which one a box runs is the `device ready`
line's `four-wide decode`. One decode body serves the tensor load's callback, the cm2 tiles and
the CPU oracle, and a kernel body can call it on the plane element itself - `decode_v4(wq[i],
...)`, the index travels and the callee chains through the plane (`test_vkd_direct_decode`).

### The mx4 cm2 decode needs no codebook {#cm2-mx4-decode}

The mx4 expert rail's `<Fmt>Cm2T` stamp needs no codebook - the e2m1 nibble decodes as the DOUBLED magnitude {0, 1, 2, 3,
4, 6, 8, 12}, built per byte by shift and mask, against the HALVED E8M0 scale (`e8m0_half`; the bit patterns below exponent
2 are the denormals), the CPU `dot_mx4q8_scalar`'s own arithmetic, so the tile, the decode GEMV and the CPU dot agree by
construction, and its scale plane is one byte a block, read four blocks a word.

### The device scale row the discrete decodes read {#kq-dev-scale-row}

The Vulkan tier does not use the Metal blob's split scale plane
(`ARCHITECTURE_GPU_QUANT_PLANES.md#metal-kq-split-scale-plane`) - it binds the decoded 20-byte row as five
uints per superblock, except for iq4xs, iq3s, iq3xxs and iq2xxs, whose device row is two words
(`kq_dev_ssb`): f16 d with the format's native high bits above it, then eight 4-bit sub-scale
fields - iq4xs's `scales_h` and `scales_l` verbatim, the others' `s` nibbles - which the
gather packs from the plane's decoded bytes and `iq4xs_sc` / `grid_sc` decode in the kernels.

### The cm2 tile body's k step, fast path and loop bounds {#cm2-tile-body}

**The k step follows the column and the decode; the k loop is unrolled by hand, a superblock per
block.** The template's k step (`BK`) is 64 on the dense l and m tiles and on the expert stamps of the K-quants, q4_0, q8,
the 4-bit LUT formats and the per-32 formats - there the e column IS the m stamp (`KQ_CM2E_ALIASES_M` in `dasllama_kqformat.das` names them, the
`cm2_cls_*` ladder's e tail picks their m class through `kq_tile_target`, `REVIEW.das` holds the roster to the s stamps' k steps) - and 32 on the s and e stamps
of the five grid-codebook formats (iq2xxs, iq2xs, iq2s, iq3xxs, iq3s) and of mx4 (their e stamp is `<Fmt>Cm2EBatch`); a stamp's `AT`/`BT` carry its depth. The class pick ladders (`cm2_cls_*` for the engine, over every tile tail; `khr_cls_*` for the kernel cells' KHR arm, in the classes file) are stamped by `kq_tile_stamp` over every `KqFmt` member from one placeholder body, so a format without its stamp fails the compile, never a window. A grid decode is
occupancy-bound - a 64-deep column holds twice the A tile, and with the codebook lookup's live range
a workgroup fewer fits an SM (the iq2xxs gate/up plane 0.611 against 0.730 ms at 32, `moe:<fmt>`,
RTX 5060 Ti) - while a light decode is step-bound (the k4 s tile 0.744 against 0.679 at 32 (`moesk:k4`), its
dense l and m tiles 44 against 49 TFLOP/s (`cm2:k4`), the 35B whole-model row 2996 -> 3236 at 32). The unroll
(`UNR`: 4 steps of 64, 8 of 32 - 256 elements either way) is written out, since the driver leaves a
rolled loop rolled whatever control its `OpLoopMerge` carries (`[unroll]` read the k6 tiles at half
rate, `cm2:k6` on the RTX 5060 Ti), and the block is one superblock because the decode inlines once per copy: an eight-copy
stamp's code, refetched after each window's weight stream had passed the L2, cost a 64-workgroup
GEMM 16 us of its 27 (`cold:k6`'s flush row, RTX 5080), and four copies run its m tiles 10-14%
faster hot (`cm2:k6`); the 32-deep e stamps read alike at 8 and 1 (`moesk:`).

**The dense l tile accumulates in f16 on both homes, and its cells' bar is an accumulation bar, not
an indexing one.** `ACC` is `coopmatWgAcc_f16_128x256` in the Vulkan and the CUDA l stamp and the f32
`ACCW` is written once, after the clamp; an f32 `ACC` on the CUDA stamp (the same body, the convert a
copy) reads 0.35 of the f16 stamp's rate at two blocks an SM (the 128 x 256 f32 accumulator spills
at 128 registers) and 0.57 to 0.77 at one block, on the three rate shapes of
`harness/gemm_cuda_vs_vulkan.das`, the same-run Vulkan arm within 2 percent across the runs, so f16
stays. The error it costs: a `mma.sync` k16 step sums its sixteen products wide and rounds the
running sum to f16 once, so over `s = k / 16` steps the sum carries at most `s` roundings of
`2^-11` of a partial sum (k 576: 36 steps, 1.8e-2 worst; k 2048: 128, 6.3e-2) and, the roundings
uncorrelated, `sqrt(s) * 2^-11` typically (2.9e-3 and 5.5e-3 of a partial sum, which the hashed
fixture holds under its output's largest magnitude). The q8 l-tile cells of `tests/test_cuda_kernels.das`
read 1.7e-3 of the largest output at k 576 and 1.4e-3 at k 2048 against their 2e-3 bar
(`TILE_BAR`): the bar measures the accumulation, not only the decode and the indexing, and a fixture
whose partial sums run larger than its outputs will miss it before the tile is wrong.

**The tile's fast path is what makes the loads unclamped.** It runs when the weight tile is whole
(`m0 + 128 <= d`), the token column is whole or the stamp carries the partial-column path (`STILE`:
the s and m columns), and K is a whole number of BK steps; the layouts are then created
clamp-Undefined and the B and output strides are masked to a multiple of 8 f16 (`stride &= ~7`) -
a mask that changes nothing while `n` and `d` are 32-multiples, as every served shape is, and
exists to make the alignment PROVABLE to the driver's address analysis, which keeps the loads on
the wide path. A partial-column stamp gates only the weight tile: its partial token column loads
unclamped and that column's store clamps (`tensorLayout2DPad`), while a whole column stores
unclamped on every stamp (a clamp there measured free: k4 m tile 48.0 against 48.1 TFLOP/s at
the gate shape, `cm2:k4`, RTX 5060 Ti). Everything else takes the edge path with clamped layouts.

**The no-split arm keeps literal loop bounds and a literal store base.** Where `ksplit` is zero the
k loop runs the literal `0 .. n` with the store at the row base rather than the general `k0`/`k1`/`ybase`
form, although those values are exactly `0`, `n` and `0` there: the general spelling costs 27% of prefill throughput (`benchmarks/lcpp_bench.das` pp512, RTX 5060 Ti).

**A k split stacks partial planes, and a split dispatch carries one region.** Under a nonzero
`ksplit` the workgroups stack `nsplit` planes of weight tiles by token tiles; plane `ks` covers k
`[k0, k1)` and stores its f32 partials at `ybase`, and the reduce sums the planes into y. A plane's
size rides the region's `row0 + cnt`, so the split serves a single-region (dense) dispatch alone:
the planes of several regions would overlap.

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
32 tokens as eight 16x16 f32 accumulators (a `coopmatAcc_f32_16x16[8]` walked under
`for [unroll_full]`, so every fragment sits in registers) and loads four weight fragments and
two token fragments per 16-deep multiply-add round: 96 fragment loads per workgroup per k step
against 144 when every subgroup owned a 16-row strip across all 128 tokens. The accumulators
are f32 on every KHR arm (this tile, the mm l and m tiles, the flash tile's P V step): RDNA3's
matrix cores round an f16 accumulator toward zero at every multiply-add (the dasVulkan
compute-features suite's rounding probe reads it), so a k-long sum in f16 drifts low there by
about a percent of a row where NVIDIA's round-to-nearest stays inside the bars, and the KHR arms
serve the cards without cm2, where the f32 accumulate costs no tensor rate. A whole tile stores its fragments straight
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
a wave64 device the tier cannot pin at 32 (four subgroups per 256-thread workgroup,
`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`) keeps its kq planes on the sdot4 tile.

### The decode GEMV family splits a subgroup across rows by the row length {#kq-gemv-lanes}

**A subgroup of the kq GEMV family takes one, two or four output rows, each row's lanes a cluster of
the fold.** A leaf decodes a block once (`blk_decode`: the packed int8 quads and four fold terms,
the block sums skipped where `fold_reads_bsum` is false) and one shared `blk_fold` dots it per
activation block, so the N-column form pays a block's codebook gathers, grid lookups and bit
deposits once for all columns; both forms compile one `blk_fold` text and add its plain product, so
both sum bit for bit. A lane takes one 32-block per step (`gemv_shell`), four steps straight-line
so four blocks' loads are in flight before a sum waits on one, the guarded single step as the tail;
a row of nb blocks over 32 lanes has nb / 32 blocks per lane: four at K 4096, under one at a MoE's
expert rows (K 512 to 1408), where most of the subgroup idled and the DRAM rate fell to a third of
the k4 band. `gemv_lanes_per_row` picks the lanes per row from the row's blocks - 8 to 24 blocks; 8
for the grid formats and 16 for the k-lattice to 48; 16 to 96; past that the whole subgroup for the
k-lattice and 16 for the grid formats - and `gemv_enc` sizes the grid to match (a workgroup's
subgroups each take subgroup_size / lanes rows). The grid formats are the codebook and grid decodes
(iq2s, iq2xs, iq2xxs, iq3xxs, iq4xs, iq4nl, iq3s4, nv4, `gemv_grid_fmt`); iq3s takes the k-lattice split (K
1408: 354 / 395 / 386 GB/s at 32 / 16 / 8 lanes). The push block carries the lanes (0 = the whole
subgroup, the q8 GEMV's one form); the fold is an xor-shuffle butterfly over the row's lanes at 8 or
16 and the whole-subgroup add otherwise, a dead row at zero, so the butterflies stay whole; the
lane-to-block map sets the row's summation order, so a resident-vs-CPU bar reads a different noise
sample than the one-row form. RTX 5060 Ti, DRAM-bound planes (`harness/vk_gemv_probe.das <n> <d>`,
GB/s at 32 / 16 / 8 lanes): K 512 iq2s 100 / 194 / 297, iq2xxs 142 / 233 / 351, k4 394 / 402 / 413,
k6 386 / 414 / 407; K 1408 iq2s 198 / 299 / 368, k6 386 / 399 / 373, k4 394 / 394 / 382; K 2048 iq2s
311 / 384 / 388, k6 417 / 416 / 369; K 4096 iq2s 376 / 408 / 344, k6 403 / 387 / 326, k4 400 / 404 /
406; K 5632 k4 411 / 399 / 386, k6 396 / 384 / 287, iq2s 377 / 393 / 296. Upstream's mat-vec splits
K over 16 threads and blocks two to four rows per thread (`rm_kq`, `NUM_ROWS` in `mul_mat_vec_*.comp`): the same bytes in flight by the other axis.

### The one-column leaves walk their regions a row at a time {#gemv-region-walk}

The one-column leaves walk their regions interleaved a row at a time (`r = rg % nreg`, the y row at `r * d + row`), so an
expert two slots share leaves DRAM once and the second slot reads it from cache - the reference's MoE GEMV puts every
token's dot of one row index in one block for the same reason - where a region-major walk put the two reads a whole expert
stack apart.

### The routed gate-up fusion {#moe-gate-up-fused}

Native-MXFP4 routed stacks take `KqGemvK4Gu`'s fusion (`Mx4GemvGu` on `Mx4Gemv`'s row dot;
the `GEMV_SHELL_GU` stamp `RLayer.egu` reads, under its own set layout): a workgroup owns 32 hidden rows of one slot, a subgroup a row over both
stacks - the up stack's regions at their own binding, inside the gate stack's slab - and the
first subgroup adds the slot's expert bias rows, activates and quantizes the block with the
requant kernel's own amax and quant, so the bytes are the split path's.
