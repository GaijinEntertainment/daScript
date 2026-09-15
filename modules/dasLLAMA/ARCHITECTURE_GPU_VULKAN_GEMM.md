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
fit on the card first is `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md` sec.2.2n-2.2o; the per-op
tier's decode-era mechanisms are `ARCHITECTURE_GPU_VULKAN_DECODE.md` sec.2.2r-2.2v; the GPU
backend role table these sections build on stays in `ARCHITECTURE_GPU.md` sec.1.5.

### 2.2k The cm2 decode callbacks read their quant bytes as 16-bit lanes {#cm2-decode-16bit-lanes}

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
IQ3_XXS aux32 words) is assembled from its two lanes and shifted. The
scalar callback is written in PAIR form: every read the two elements of an aligned pair share -
the grid byte, the sign word, the scale row - is derived from the pair's first element (`e &
~1u`), both values are computed and the element is selected last, because the driver runs the
scalar callback two elements at a time and commons what the bodies share (the RTX 5080's scalar
arm: the iq2xxs and iq2s expert stamps 11-13% faster on the skewed schedule, `moesk:iq2xxs` e+s
0.357 -> 0.316 ms, `moesk:iq2s` 0.428 -> 0.380, the 35B's pp512 3987 -> 4201). Every table a
decode reads at a runtime index is staged into a `@workgroup` array ahead of the tile loop, never
selected out of a register vector per element: the iq4 formats' 16-entry codebook
(`kvalues_iq4nl`, IQ4_XS and IQ4_NL) as f16, each grid format's codebook as its own word array -
the reference exe's shared-memory table staging (`ARCHITECTURE_MEASUREMENT.md` sec.2.5).

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

### 2.2l The cm2 tile pick and the coopmat default ladder {#cm2-tile-pick-and-default}

Every dispatch time in this section is the probe's (`harness/vk_gemm_probe.das`, the arm named where it decides) on the RunPod RTX PRO 4500 under the tree's defaults unless another box is named; a window's time is the prefill profiler's (`DASLLAMA_GPU_PROF=1`, `benchmarks/lcpp_bench.das -- -p 512`) on the same card. **The l/m tile pick and the split-k pick are one wave model** (`cm2_gemm_pick`). For a GEMM of
width `d` over `cnt` rows the l tile (256-row columns) and the m tile (128-row columns) each take
some number of workgroups; a grid runs in whole waves over the device's SM count, and a wave costs
its k steps at the column's step weight - m 10, l 16, s 5 - since the l column does twice m's work
in 1.6 times its step (RTX PRO 4500: an m step 1.4 us, an l step 2.24). The whole GEMM takes the
column of fewer units, a tie to l (E4B's 16384-wide gate l 424 us against m 474, E2B's 6144-wide
gate l 126 against m 112). Four rules sit ahead of the comparison: a region of 64 rows or fewer takes the s tile (32-row columns - the per-op
tier's MoE expert-bucket shape, ~32 rows to each of 128 experts a 512-token window), a window of 128 rows or fewer takes m (the l column would run half empty), a GEMM whose m columns fill a quarter of the SMs or fewer takes s (gemma-3-1b's k and v, 8 m tiles on 82 SMs: 805 -> 574 us a window), and a device
that reports no SM count takes l and never splits k. Beyond `(d, cnt, sm_count)` the pick reads only
two values fixed at init - the served mode and `DASLLAMA_CM2_TILE` - so the class the pipeline binds and the tile rule the meta fill
writes can never disagree; `cnt` is the AVERAGE rows per active region of the dispatch, so one tile serves every region of a per-op MoE schedule. The resident MoE
block makes no pick: its device schedule cuts every bucket into s and m pieces by size and
dispatches both classes per plane (`ARCHITECTURE_GPU_VULKAN_MOE.md` sec.2.2af) - the s stamp and
the e stamp, the m column at the format's k step, keyed `CM2_TC_E` in the class ladders - which a
real window's skew needs (a 467-row bucket is 15 s tiles or 4 m columns). A region below the s
tile's row count goes to the decode GEMV family. The s and m tiles' fast path loads a partial column
UNCLAMPED (the layout's row dimension rounded up to the column) and clamps only the store, so every
f16 plane the chain feeds them - the gathered activation image and the hidden plane - is sized with
128 rows of slack past its last region (`TILE_READ_SLACK`, `ffn_cm2_chunk_rows`); the l tile takes
the edge path on a partial column, since only a window's last column is ever partial there; the
dense chain's planes hold the whole window's rows, so a partial m column's load stays inside them. A
partial last WEIGHT tile (a 2112-wide plane's 17th, a 704-wide expert's 6th) loads the plane's last
whole 128 rows instead - in bounds, unclamped, its overlap with the tile before it rewriting the same
values - since a clamped weight load runs every tile at a third the speed (the fast path's own layout
clamped: q8 E2B down 268 -> 612 us) and through the edge path the dispatch waited on the partial workgroups (the 26B's shared expert, k6: 164 us against 71 whole). `STILE` is inert on the KHR classes.

**The k step follows the column and the decode; the k loop is unrolled by hand, a superblock per
block.** The template's k step (`BK`) is 64 on the dense l and m tiles and on the expert stamps of
the K-quants, q4_0, q8 and the 4-bit LUT formats, and 32 on the s and e stamps of the five
grid-codebook formats (iq2xxs, iq2xs, iq2s, iq3xxs, iq3s; the e stamp is `<Fmt>Cm2EBatch`, the
`cm2e_cls_*` ladder beside the KHR one); a stamp's `AT`/`BT` carry its depth. A grid decode is
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

**The split-k pick weighs the same units over one to eight 256-aligned chunks of K, at both
columns where the whole pick chose between them.** A split's units add its reduce - 9 us behind
the barrier and 2.5 MB a us over the partial planes and the sum (`SplitKReduce`, 7 to 18 us
measured) - and serves only under eleven twelfths of the whole; K under 2048 never splits, the planes
stay within the scratch (256 KiB an SM), and a count whose last chunk would start past K is skipped.
The split arm's k offset is masked to the chunk alignment so the compiler knows what k = 0 tells it
on a whole GEMM: unmasked, the same loop ran at half the rate (gemma-3-1b's down through one chunk
304 us against 152 whole), so every earlier split lost; the whole GEMM keeps its literal-bound loop (the l
stamp through the split loop's variables ran the E4B, 12B and gemma-3-4b gates a sixth slower). Measured at 512 rows, whole against the pick with its reduce: gemma-3-1b's down (36 m tiles,
K 6912) 152 -> 78 us in four l chunks (the reference exe's GEMM at the same shape: 93), E2B's 12288-deep down 268 -> 162 in
three, E4B's 16384-deep down 398 -> 350 and gemma-2's 214 -> 182 in two; the 12B's 15360-deep down
stays whole. The group is the role's workgroups plus its chain neighbours' - q with k and v, gate with
up - because the hazard-mask rail lets independent roles co-run while every split role serializes
through the one scratch plane (`VHZ_SK`): the whole GEMM's waves count the group, and a split's units
scale by the group's size over the role's own; so k and v beside q run whole, and the split is the lone role's - wo, down, a classifier.

**The f16 feed admits q8 and every kq superblock format** (`kq_sb`) - the set the cm2 decode
callbacks cover (sec.2.2k) - and each (format, tile) pair has ONE stamped class, reached through
one dispatcher per stage (`cm2_cls_ensure`, `cm2_cls_set`, `cm2_cls_enc`), all three keyed on the
same `(fmt, ml)` pair, so the pipeline a role ensures, the set it binds and the kernel it encodes
are never three different classes; the per-format arm of `harness/vk_gemm_probe.das` drives the
same ladders, so a probe row times the class the driver serves. The decode GEMV keeps its quant
chains: the feed format pick is decoupled from the weight format.

**The served GEMM mode resolves once, at init, through one ladder.** cm2 where the device has
NV_cooperative_matrix2, else mm where it has KHR_cooperative_matrix, else sdot4; `DASLLAMA_COOPMAT`
overrides the ladder by name, and a cm2 request or force on a device without the extension lands
on mm. The same resolver stamps the mode into the `.dlim` flavor configuration, so the recorded
mode and the running mode cannot drift. The four-wide decode callback is not in that
configuration: a cm2 tile names both callbacks (`coopmatLoadTensorDecode`'s tenth argument) - the
format's own `decode_v4` under the template's `DECV4` axis (every kq superblock format, sec.2.2k),
else the `DECVEC` axis's synthesized twin, where a new format starts until its `cm2:<fmt>` row
decides - and the device decides which one runs (`DASLLAMA_VK_DECVEC` and the extension at
creation); neither choice shapes an image byte, so the bake identity ignores it, a serve-only
knob being no configuration field. `decvec_on` is the run's arm, on the `device ready` line.

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
k loop runs the literal `0 .. n` with the store at the row base rather than the general
`k0`/`k1`/`ybase` form, although those values are exactly `0`, `n` and `0` on that path: the general
spelling costs 27% of prefill throughput (`benchmarks/lcpp_bench.das` pp512, RTX 5060 Ti).

### 2.2m Class-pipeline creation is the Vulkan tier's one shader A/B seat {#vk-class-pipeline-build}

`vkd_class_pipe` is the single place a class kernel's SPIR-V becomes a pipeline, so every shader
instrument hangs there and nothing else has to know about them. The four-wide decode fallback
hangs there too: when the device carries no `VK_NV_cooperative_matrix_decode_vector`
(`decvec_on` false) the served words go through `strip_decode_vector` (the capability, the
extension and every load's `DecodeVectorFunc` operand removed, the scalar callback left to
serve), after the override and before the shader module, so a dumped or overridden blob is
always the emitted, unstripped one. The scalar arm makes a MoE prefill window about half again as
long (the 35B-A3B's 512-row window 160 ms with the twin against 237 without on the RTX 5060 Ti), and
the tier warns at device init when the driver reports no such extension; which drivers list it,
and the reference exe's own two arms measured beside ours, are `followup_vulkan.md` item 45. The seat
is also the in-process A/B: `vkd_pipes_rebuild` marks every class slot stale, so the next ensure
rebuilds it under whatever `decvec_on` says, which is how the `cm2:<fmt>` probe runs both arms interleaved in one process.

**The dump and the keeping seam run before the override:** `DASLLAMA_VK_SPV_DUMP=<dir>` writes the
EMITTED words as `<dir>/<kernel>.spv`, `g_vkd_spv_keep` keeps the same words in `g_vkd_spv_kept` under
the kernel's name for the kernel cells, and `DASLLAMA_VK_SPV_OVERRIDE=<dir>` then serves that directory's
file - a round trip (dump, edit or spirv-opt, serve back) a dump taken after the override would not give.

**Full subgroups are a whole-run arm, never a per-pipeline one.** `DASLLAMA_VK_FULLSG` plus a
device that reports the feature sets `g_gpu.full_sg_on` once at device init, and every class
pipeline is then built with `REQUIRE_FULL_SUBGROUPS`, so an A/B compares two whole runs. Plain
is the default: pinned measures slower on the mm_a gate shape.

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
(`fa_cm2_h64` / `h128`, `ARCHITECTURE_GPU_VULKAN.md` sec.2.2j) when `has_coopmat2_fa` holds - the
device reports the cm2 flash features (reductions, conversions, per-element ops) AND was created in
cm2 mode, since a forced mode enables no coopmat2 extension - the fa knob is on, the head size is 64 or 128, and the model's attention is
not gated - this chain wires neither the h256 stamps nor their gated epilogue, so gated models
keep the flash-style `at_attn` pass. The tile reads f16 K/V: the chain keeps its f32 roped-k /
raw-v planes at absolute positions for the host readback the CPU cache store consumes, and
fills f16 shadows of them with the base-less `f16cvt` over the whole attended prefix each window; the fa
output lands in the same out plane `at_attn` writes, so the requant and `wo` stages never learn which pass ran.

**The per-op attention chain adds a q/k/v projection bias (qwen2moe) in its prep stage.** The
layer's `[q | k | v]` row uploads to one device buffer per call and binds to `AtPrep`, whose q
and k passes add their slice (`boff` 0 and `qd`) to each projection element before the norm and
the rope - where the CPU chain adds it. v has no prep pass of its own, so a biased layer runs a
third `AtPrep` over the raw v window with the rope half 0 and the norm off, which makes the
kernel a copy plus bias in place (`boff` `qd + kv_dim`); the copy into the absolute-position v
plane and the host readback then both carry the bias. A model without the bias runs the two passes with
`hasb` 0 and never reads the binding, and the attention-quad rail's `arch_ok` test does not name the bias.

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
accumulator width, the two-by-four tiling - costs rate: the word stage the most, the
accumulator width next, the tiling least; the probe's `khrx` arms measure them
(`ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md` sec.2.5a) and `followup_vulkan.md` item 42 keeps
the figures. On a partial token column the edge store costs nothing beyond the padded rows: per
computed row a 300-token window runs at the whole-window rate.

**The two staging arrays are the whole footprint by design.** A third array for the bounce
costs the tile a seventh of its rate, because 8 KB more shared memory per workgroup is one
workgroup fewer per SM. The k step is 32 (the reference exe's BK): a 64-deep step doubles the
staging tiles, halves the workgroups an SM holds, and with the 8 KB iq2s grid beside them
reaches the 49152 B of workgroup memory the tier requires of a device. The tile is sized to
that 49152 B floor; a device that offers less workgroup memory is not a target.

**The arm exists at one geometry** - 128 weights by 128 tokens, k step 32 - so in mm mode the
tile pick answers 128 and the wave model weighs its k chunks alone (a deep, narrow GEMM splits k
into the same scratch planes and reduce the cm2 tiles use), and `cm2_cls_ensure/set/enc` route to
the `khr_cls_*` ladders, the same `(fmt)` key on both. The f16 feed admits a kq format in mm mode
only on a 32-lane subgroup (`khr_kq_tile_on`): the body indexes eight subgroups over the tile, so
a wave64 device (four subgroups per 256-thread workgroup) keeps its kq planes on the sdot4 tile.

### 2.2ah The decode GEMV family splits a subgroup across rows by the row length {#kq-gemv-lanes}

**A subgroup of the kq GEMV family takes one, two or four output rows, each row's lanes a cluster
of the fold.** A lane takes one 32-block per step (`gemv_shell`), four steps straight-line so
four blocks' loads are in flight before a sum waits on one (a rolled loop issued a block's loads
after the last block's sum), the guarded single step as the tail; a row of nb blocks over 32
lanes has nb / 32 blocks per lane: four at K 4096, two at K 2048, under one at a MoE's expert rows
(K 512 to 1408, 16 to 44 blocks), where most of the subgroup idled and the DRAM rate fell to a
third of the k4 band. `gemv_lanes_per_row` picks the lanes per row from the row's blocks - 8 to
24 blocks; 8 for the grid formats and 16 for the k-lattice to 48; 16 to 96; past that the whole
subgroup for the k-lattice and 16 for the grid formats - and `gemv_enc` sizes the grid to match
(a workgroup's subgroups each take subgroup_size / lanes rows). The grid formats are the
codebook and grid decodes (iq2s, iq2xs, iq2xxs, iq3xxs, iq4xs, iq4nl, `gemv_grid_fmt`); iq3s
takes the k-lattice split (K 1408: 354 / 395 / 386 GB/s at 32 / 16 / 8 lanes). The push block
carries the lanes (0 = the whole subgroup, the q8 GEMV's one form); the fold is an xor-shuffle
butterfly over the row's lanes at 8 or 16 (the subgroup shuffle every GEMV already requires; a
clustered add would ask for a feature the tier never checks) and the whole-subgroup add
otherwise, every lane reducing, a dead row at zero, so the butterflies stay whole; the
lane-to-block map sets the row's summation order, so a resident-vs-CPU bar reads a different
noise sample than the one-row form, inside the same class. RTX 5060 Ti, DRAM-bound planes
(`harness/vk_gemv_probe.das <n> <d>`, GB/s at 32 / 16 / 8 lanes): K 512 iq2s 100 / 194 / 297,
iq2xxs 142 / 233 / 351, k4 394 / 402 / 413, k6 386 / 414 / 407; K 1408 iq2s 198 / 299 / 368,
k6 386 / 399 / 373, k4 394 / 394 / 382; K 2048 iq2s 311 / 384 / 388, k6 417 / 416 / 369; K
4096 iq2s 376 / 408 / 344, k6 403 / 387 / 326, k4 400 / 404 / 406; K 5632 k4 411 / 399 / 386,
k6 396 / 384 / 287, iq2s 377 / 393 / 296. Upstream's mat-vec splits K over 16 threads and
blocks two to four rows per thread (`rm_kq`, `NUM_ROWS` in `mul_mat_vec_*.comp`): the same
bytes in flight by the other axis.
