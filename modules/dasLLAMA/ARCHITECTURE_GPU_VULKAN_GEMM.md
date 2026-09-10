# dasLLAMA Architecture - the Vulkan tier's GEMM tile family

Companion to `ARCHITECTURE_GPU_VULKAN.md`; section numbers are `ARCHITECTURE.md`'s. This
document carries sections 2.2k-2.2m, 2.2q, 2.2ae and 2.2ah, the cooperative-matrix tiles the
Vulkan tier's GEMMs run on and the decode GEMV family's lane split: how a cm2 tile decodes its
quant bytes, how a tile and the served GEMM mode are picked, the class-pipeline build seat both
shader instruments hang on, the MoE expert chain on those tiles, the KHR arm's hand-staged
tile, and how a GEMV subgroup splits across short rows. `ARCHITECTURE_GPU_VULKAN.md` carries
the prefill window chain that dispatches them (sec.2.2j) and its recurrent block (sec.2.2ad),
the Q8 requant byte store (sec.2.2p), the decode GEMV family's grid codebook buffer
(sec.2.2ab), and the tile probe's shared descriptor set layout (sec.2.2ac). What a model has to
fit on the card before any of this runs is `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`'s sections
2.2n-2.2o. The decode-era mechanisms of the per-op tier are
`ARCHITECTURE_GPU_VULKAN_DECODE.md`'s sections 2.2r-2.2v. The GPU backend role table these
sections build on stays in `ARCHITECTURE_GPU.md` sec.1.5.

### 2.2k The cm2 decode callbacks read their quant bytes as 16-bit lanes {#cm2-decode-16bit-lanes}

A cm2 tile's decode callback runs inside the driver's block load, and the vendor driver's shader
compiler pattern-matches only one load width into that path: a 16-bit load (`int16[N]` block
members), with sub-fields pulled out by shift and mask. A 32-bit word with a variable shift runs
slower; an `unpack8` of a 32-bit word indexed by a runtime value (a byte4 dynamic select) drops
the whole kernel off the block-load path, to about a third of the rate. Every cm2 decode - q8
and every kq superblock format - is spelled the 16-bit way, which is why the block structs are
`int16` arrays over the same bytes. A byte the decode needs at a runtime position comes out of
its lane by a shift - `(uint(int(blk.qs[i >> 1u])) & 0xFFFFu) >> ((i & 1u) * 8u)` - not by an
`unpack8(w)[i & 1u]` byte2 lane select: the select reads the same lane, but a decode built on
selects runs 1.1x to 1.5x slower than the shift form on the expert-schedule shape
(`harness/vk_gemm_probe.das -- moe:<fmt>`, RTX 5060 Ti, per gate/up plane, the shift form:
iq2xxs 724 us, iq3xxs 569, iq3s 766, iq2s 736 against llama.cpp's cm2 `mul_mat_id` tile at
754 / 788 / 870 / 797; the select form read 1.28x, 1.24x, 1.49x and 1.08x of those times on the
same shape). A sign index that straddles two bytes (the
IQ2_XXS and IQ3_XXS aux32 words) is assembled from its two lanes and shifted, never built from
two selected bytes.
Every table a decode reads at a runtime index is staged into a `@workgroup` array ahead of the
tile loop, never selected out of a register vector per element: the iq4 formats' 16-entry
codebook (`kvalues_iq4nl`, shared by IQ4_XS and IQ4_NL) as f16, each grid format's codebook as
its own word array. That is the shared-memory table-staging form of the reference exe -
llama.cpp's Vulkan build, the upstream binary `benchmarks/lcpp_bench.das` measures against
(`ARCHITECTURE_MEASUREMENT.md` sec.2.5).

Every kq format's four-wide twin is hand-written (`decode_v4`, the template's `DECV4` axis): it
keeps the same spelling and shares what four consecutive elements share. A K-quant twin reads
its four quant bytes as two 16-bit lanes and extracts the sub-block's scale pair once; a grid
format's twin looks its grid word up once and takes the four bytes and the four sign bits from
it. The synthesized twin (`DECVEC`, the axis a new format starts on) repeats the whole scalar
body four times - the lane selects, the scale-plane words, the grid lookup and the sign parity
- and on the grid formats it runs slower than the scalar callback for exactly that reason. The
twin computes each element in the scalar's operation order, so the tile's CPU oracle holds under
either callback; which callback a box runs is the `device ready` line's `four-wide decode`.

One decode body serves the tensor load's callback, the cm2 tiles and the CPU oracle. A kernel
body can also call it on the plane element itself - `decode_v4(wq[i], ...)`, where the
element's index travels and the callee chains through the plane, so no block is copied. The
test `test_vkd_direct_decode` in `tests/test_vulkan_kernels.das` keeps that emitter capability.

### 2.2l The cm2 tile pick and the coopmat default ladder {#cm2-tile-pick-and-default}

**The l/m tile pick is a wave-efficiency comparison.** For a GEMM of width `d` over `cnt` rows
the l tile (256-row columns) and the m tile (128-row columns) each take some number of
workgroups. Each grid runs in whole waves over the device's SM count, so a grid's wave count
times that SM count is the slots it allocates. The pick takes the tile whose workgroups fill
the larger share of its allocated slots, the two ratios compared by cross-multiplying. The m
tile wins only on a strict win; a tie goes to l, whose bigger tile carries twice the arithmetic
intensity. Three rules sit ahead of the comparison: a region of 64 rows or fewer takes the s
tile (32-row columns - the per-op tier's MoE expert-bucket shape, where a 512-token window
routes ~32 rows to each of 128 experts on average), a window of 128 rows or fewer takes m (the
l column would run half empty), and a device that reports no SM count takes l and never splits
k. Beyond `(d, cnt, sm_count)` the pick reads only two values fixed at init - the served mode
and `DASLLAMA_CM2_TILE` - so the class the pipeline binds and the tile rule the meta fill writes
can never disagree; `cnt` is the AVERAGE rows per active region of the dispatch, so one tile
serves every region of a per-op MoE schedule. The resident MoE block makes no pick: its device
schedule cuts every bucket into s and m pieces by size and dispatches both classes per plane
(`ARCHITECTURE_GPU_VULKAN_MOE.md` sec.2.2af), which is what a real window's skew needs - one tile
per bucket costs the same whatever its fill, and a 467-row bucket is 15 s tiles or 4 m
columns. A region below the s tile's row count goes to the decode GEMV family, not to a tile.
The s and m tiles' fast path loads a partial column UNCLAMPED (the layout's row dimension
rounded up to the column) and clamps only the store, so every f16 plane the chain feeds them -
the gathered activation image and the hidden plane - is sized with 128 rows of slack past its
last region (`TILE_READ_SLACK`, `ffn_cm2_chunk_rows`); the l tile takes the edge path on a
partial column, since only a window's last column is ever partial there. The dense chain's
planes carry no slack: they hold the whole window's rows whatever the last window's length, so
a partial m column's unclamped load stays inside them. The store-layout constant the m and s
tiles read (`STILE`) is inert on the KHR classes, whose tile never reads it.

**The split-k pick counts the dispatch group, not the GEMM.** With long K (2048 and up), a grid
that fills at most half the SMs splits its reduction across f32 partial planes that
`SplitKReduce` sums, into as many chunks as fill the device (SM count over workgroups); a grid
that fills up to two thirds splits into three. Eight chunks is the ceiling. Each chunk is
256-aligned, and a chunk count whose last chunk would then be empty drops by one. The grid it
measures is the role's own workgroups PLUS those of the chain neighbours it runs beside - q
with k and v, gate with up (`cm2_tiles`, the same pick each neighbour's own dispatch makes) -
because the hazard-mask rail lets independent roles co-run, while every split role serializes
through the one scratch plane (`VHZ_SK`) its neighbours would also claim. Counted alone, a
512-wide k or v projection over a 512-row window fills 16 of 36 SMs and splits in two; counted
beside q it runs whole, and k and v fill the device together. Split-k is left to the lone
role - wo, down, a small model's classifier - whose grid nothing else pads.

**The f16 feed admits q8 and every kq superblock format** (`kq_sb`) - the set the cm2 decode
callbacks cover (sec.2.2k) - and each (format, tile) pair has ONE stamped class. The
prefill driver reaches them through one dispatcher per stage (`cm2_cls_ensure`, `cm2_cls_set`,
`cm2_cls_enc`), all three keyed on the same `(fmt, ml)` pair, so the pipeline a role ensures,
the set it binds and the kernel it encodes can never be three different classes. The three are
not private: the per-format arm of `harness/vk_gemm_probe.das` drives the same ladders, so a
probe row times the class the driver would serve rather than a copy of it. The decode
GEMV keeps its quant chains: the feed format pick is decoupled from the weight format.

**The served GEMM mode resolves once, at init, through one ladder.** cm2 where the device has
NV_cooperative_matrix2, else mm where it has KHR_cooperative_matrix, else sdot4;
`DASLLAMA_COOPMAT` overrides the ladder by name, and a cm2 request or force on a device without
the extension lands on mm. The same resolver stamps the mode into the `.dlim` flavor
configuration, so the recorded mode and the running mode cannot drift. The four-wide decode
callback is not in that configuration. A cm2 tile names both callbacks
(`coopmatLoadTensorDecode`'s tenth argument): the format's own `decode_v4` where the template's
`DECV4` axis is on - every kq superblock format, sec.2.2k - else the `DECVEC` axis, which
synthesizes the twin from the scalar body and is where a new format starts; its `cm2:<fmt>`
probe row is what decides whether the format gets a hand-written twin. The device decides which
one the driver runs - it is created with `DASLLAMA_VK_DECVEC` and the extension - and neither
choice shapes an image byte, so the bake identity ignores it: a serve-only knob is not a
configuration field. `decvec_on` is the run's arm, announced on the `device ready` line.

**The tile's fast path is what makes the loads unclamped.** It runs when the weight tile is
whole (`m0 + 128 <= d`), the token column is whole or the stamp carries the partial-column path
(`STILE`: the s and m columns), and K is a whole number of BK steps; the layouts are then
created clamp-Undefined and the B and output strides are masked to a multiple of 8 f16
(`stride &= ~7`). The mask changes nothing while `n` and `d` are 32-multiples, which every
served shape is; it exists to make the alignment PROVABLE to the driver's address analysis,
which is what keeps the loads on the wide path. A partial-column stamp gates only the weight
tile: its partial token column loads unclamped and that column's store clamps
(`tensorLayout2DPad`), while a whole column stores unclamped on every stamp (the clamp on a
whole column measured free on the k4 m tile, 48.0 against 48.1 TFLOP/s at the gate shape
(`harness/vk_gemm_probe.das -- cm2:k4`, RTX 5060 Ti), so
the branch is there for the layout's meaning, not its cost). Everything else takes the edge
path with clamped layouts.

**The no-split arm keeps literal loop bounds and a literal store base.** Where `ksplit` is zero
the k loop runs the literal `0 .. n` with the store at the row base rather than the general
`k0`/`k1`/`ybase` form, although those values are exactly `0`, `n` and `0` on that path: the
general spelling costs 27% of prefill throughput (`benchmarks/lcpp_bench.das` pp512, 5060 Ti).
The split arm keeps the general form.

### 2.2m Class-pipeline creation is the Vulkan tier's one shader A/B seat {#vk-class-pipeline-build}

`vkd_class_pipe` is the single place a class kernel's SPIR-V becomes a pipeline, so both shader
instruments hang there and nothing else has to know about them. The four-wide decode fallback
hangs there too: when the device carries no `VK_NV_cooperative_matrix_decode_vector`
(`decvec_on` false) the served words go through `strip_decode_vector` (the capability, the
extension and every load's `DecodeVectorFunc` operand removed, the scalar callback left to
serve), after the override and before the shader module, so a dumped or overridden blob is
always the emitted, unstripped one. The scalar arm makes a MoE prefill window about half again as
long (the 35B-A3B's 512-row window 160 ms with the twin against 237 without on the RTX 5060 Ti), and
the tier warns at device init when the driver reports no such extension; which drivers list it,
and the reference exe's own two arms measured beside ours, are `followup_vulkan.md` item 45. The
seat is also the in-process A/B: `vkd_pipes_rebuild`
marks every class slot stale, so the next ensure rebuilds it under whatever `decvec_on` says,
which is how the `cm2:<fmt>` probe runs both arms interleaved in one process.

**The dump runs before the override.** `DASLLAMA_VK_SPV_DUMP=<dir>` writes the EMITTED words as
`<dir>/<kernel>.spv`; `DASLLAMA_VK_SPV_OVERRIDE=<dir>` then replaces them with that directory's
file. The order is what makes the pair a round trip: dump a kernel, edit or spirv-opt the file,
serve it back. A dump taken after the override would capture the served words, not the emitted
ones.

**Full subgroups are a whole-run arm, never a per-pipeline one.** `DASLLAMA_VK_FULLSG` plus a
device that reports the feature sets `g_gpu.full_sg_on` once at device init, and every class
pipeline is then built with `REQUIRE_FULL_SUBGROUPS`. A run never mixes pinned and plain
pipelines, so an A/B compares two whole runs. Plain is the default: pinned measures slower on
the mm_a gate shape.

### 2.2q The MoE expert batch arm rides the cm2 tiles through a device-side f16 gather {#cm2-expert-chain}

The per-op tier's expert FFN batch arm has two forms over the same region schedule. The quant
form takes the CPU's gathered activation image (the engine requantizes the normed rows, then
copies each bucket row's quants into expert order) and encodes the kq batch tiles. The f16 form
takes the window's f32 activation rows themselves - one per position - plus the combine's slot
map, and does the gather on the device: one workgroup per (position, slot) grid entry scatters
its position's row as f16 into the entry's bucket row, the inverse walk of the combine over the
same map. Gate and up then run the cm2 decode-in-load tiles over that f16 image, the act writes
the hidden plane as f16, and down runs the cm2 tiles again - the resident dense chain's
`pf_gemm_enc` feed, with the region records the quant form already fills. The engine asks the
tier per layer (`moe_gpu_ffn_xf_ok`): the answer is yes only in cm2 mode on a coopmat2 device,
for a gate/up/down triple whose every format the f16 feed admits (sec.2.2l), with the window
inside the x plane's cap - and on yes it skips its own requant and gather, so the CPU cost of
the layer's FFN is the routing alone. The f16 form is the combined (`npos > 0`) form only: the
combine is what makes the device-side gather pay, since neither the gathered image nor the
bucket rows ever cross PCIe. Streamed groups take the same arm after the slot bind.

**The shared expert of a qwen2moe-class layer takes the same arm as ONE region over every
position of the window** - its q8 triple is resident under the shexp mark, the slot map is the
identity and the combine runs at unit weight, so the host reduce scales its rows by the per-row
sigmoid gate in the CPU form's order. The CPU form of the same triple costs 582 ms of a 945 ms
window on Qwen1.5-MoE-A2.7B (`benchmarks/lcpp_bench.das -p 512 --prof`, the Q4_K_M mint, RTX
5060 Ti), the one term the arm exists to move.

**The per-op attention chain runs the same cm2 flash-attention tile the resident chain runs**
(`fa_cm2_h64` / `h128`, `ARCHITECTURE_GPU_VULKAN.md` sec.2.2j) when the device reports the cm2
flash-attention features (`has_coopmat2_fa` - cooperative-matrix reductions, conversions and
per-element ops), the fa knob is on, the head size is 64 or 128, and the model's attention is
not gated - this chain wires neither the h256 stamps nor their gated epilogue, so gated models
keep the flash-style `at_attn` pass. The tile reads f16 K/V: the chain keeps its f32 roped-k /
raw-v planes at absolute positions for the host readback the CPU cache store consumes, and
fills f16 shadows of them with the base-less `f16cvt` over the whole attended prefix each
window; the fa output lands in the same out plane `at_attn` writes, so the requant and `wo`
stages never learn which pass ran.

**The per-op attention chain adds a q/k/v projection bias (qwen2moe) in its prep stage.** The
layer's `[q | k | v]` row uploads to one device buffer per call and binds to `AtPrep`, whose q
and k passes add their slice (`boff` 0 and `qd`) to each projection element before the norm and
the rope - where the CPU chain adds it. v has no prep pass of its own, so a biased layer runs a
third `AtPrep` over the raw v window with the rope half 0 and the norm off, which makes the
kernel a copy plus bias in place (`boff` `qd + kv_dim`); the copy into the absolute-position v
plane and the host readback then both carry the bias. A model without the bias runs the two
passes with `hasb` 0 and never reads the binding, and the attention-quad rail's `arch_ok` test
does not name the bias.

### 2.2ae The KHR arm's hand-staged kq tile {#khr-mm-kq-tile}

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
stride of 20 words (16 plus 4 pad, so the fragment loads spread across banks). q8 never arrives
here - its mm-mode GEMM is the q8-fed mul_mm L-tile.

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
accumulator width, the two-by-four tiling - costs rate: the word stage the most, the
accumulator width next, the tiling least; the probe's `khrx` arms measure them
(`ARCHITECTURE_MEASUREMENT.md` sec.2.5) and `followup_vulkan.md` item 42 keeps the figures. On
a partial token column the edge store costs nothing beyond the padded rows: per computed row a
300-token window runs at the whole-window rate.

**The two staging arrays are the whole footprint by design.** A third array for the bounce
costs the tile a seventh of its rate, because 8 KB more shared memory per workgroup is one
workgroup fewer per SM. The k step is 32 (the reference exe's BK): a 64-deep step doubles the
staging tiles, halves the workgroups an SM holds, and with the 8 KB iq2s grid beside them
reaches the 49152 B of workgroup memory the tier requires of a device. The tile is sized to
that 49152 B floor; a device that offers less workgroup memory is not a target.

**The arm exists at one geometry** - 128 weights by 128 tokens, k step 32 - so in mm mode the
tile pick answers 128 and split-k never engages, and `cm2_cls_ensure/set/enc` route to the
`khr_cls_*` ladders, the same `(fmt)` key on both. The f16 feed admits a kq format in mm mode
only on a 32-lane subgroup (`khr_kq_tile_on`): the body indexes eight subgroups over the tile,
so a wave64 device (four subgroups per 256-thread workgroup) keeps its kq planes on the sdot4
batch tile.

### 2.2ah The decode GEMV family splits a subgroup across rows by the row length {#kq-gemv-lanes}

**A subgroup of the kq GEMV family takes one, two or four output rows, each row's lanes a cluster
of the fold.** A lane loads one 32-block per step (`gemv_shell`), so a row of nb blocks over 32
lanes keeps nb / 32 loads in flight per lane: two at K 2048, under one at a MoE's expert rows
(K 512 to 1408, 16 to 44 blocks), where most of the subgroup idled and the DRAM rate fell to a
third of the k4 band. `gemv_lanes_per_row` picks the lanes per row from the row's blocks - 8 to
24 blocks; 8 for the grid formats and 16 for the k-lattice to 48; 16 to 96; past that the whole
subgroup for the k-lattice and 16 for the grid formats - and `gemv_enc` sizes the grid to match
(a workgroup's subgroups each take subgroup_size / lanes rows). The grid formats are the
codebook and grid decodes (iq2s, iq2xs, iq2xxs, iq3xxs, iq4xs, iq4nl, `gemv_grid_fmt`); iq3s
takes the k-lattice split (K 1408: 354 / 395 / 386 GB/s at 32 / 16 / 8 lanes). The push block
carries the lanes (0 = the whole subgroup, the q8 GEMV's one form); the fold is an xor-shuffle
butterfly over the row's lanes at 8 or 16 - the subgroup shuffle every GEMV already requires,
where a clustered add would ask for the clustered subgroup feature the tier never checks - and
the whole-subgroup add otherwise, and every lane reduces, a dead row at zero, so the butterflies
stay whole. The lane-to-block map sets the row's summation order, so a resident-vs-CPU bar reads
a different noise sample than the one-row form did, inside the same class. Measured on the RTX
5060 Ti (`harness/vk_gemv_probe.das <n> <d>`, DRAM-bound planes, GB/s at 32 / 16 / 8 lanes): K 512 iq2s
100 / 194 / 297, iq2xxs 142 / 233 / 351, k4 394 / 402 / 413, k6 386 / 414 / 407; K 768 iq2s
148 / 221 / 337, iq2xxs 213 / 269 / 376, k4 403 / 404 / 414, k6 407 / 392 / 387; K 1408 iq2s
198 / 299 / 368, k6 386 / 399 / 373, k4 394 / 394 / 382; K 2048 iq2s 311 / 384 / 388, k6 417 /
416 / 369; K 4096 iq2s 376 / 408 / 344, k6 403 / 387 / 326, k4 400 / 404 / 406; K 5632 k4 411 /
399 / 386, k6 396 / 384 / 287, iq2s 377 / 393 / 296. llama.cpp's mat-vec splits K over 16 threads
and blocks two to four rows per thread (`rm_kq`, `NUM_ROWS` in its `mul_mat_vec_*.comp`): the
same bytes in flight by the other axis.
