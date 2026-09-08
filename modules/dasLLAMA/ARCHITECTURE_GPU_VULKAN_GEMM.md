# dasLLAMA Architecture - the Vulkan tier's GEMM tile family

Companion to `ARCHITECTURE_GPU_VULKAN.md`; section numbers are `ARCHITECTURE.md`'s. This
document carries sections 2.2k-2.2m and 2.2q, the cooperative-matrix tiles the Vulkan tier's
GEMMs run on: how a cm2 tile decodes its quant bytes, how a tile and the served GEMM mode are
picked, the class-pipeline build seat both shader instruments hang on, and the MoE expert chain
on those tiles. The prefill window chain that dispatches them, the Q8 requant byte store and the
decode GEMV family's grid codebook buffer are `ARCHITECTURE_GPU_VULKAN.md`'s sections 2.2j, 2.2p
and 2.2ab. What a model has to fit on the card before any of this runs is
`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`'s sections 2.2n-2.2o. The decode-era mechanisms of the
per-op tier are `ARCHITECTURE_GPU_VULKAN_DECODE.md`'s sections 2.2r-2.2v. The GPU backend role
table these sections build on stays in `ARCHITECTURE_GPU.md` sec.1.5.

### 2.2k The cm2 decode callbacks read their quant bytes as 16-bit lanes {#cm2-decode-16bit-lanes}

A cm2 tile's decode callback runs inside the driver's block load, and the vendor driver's shader
compiler pattern-matches only one spelling into that path: a 16-bit load (`int16[N]` block
members) followed by `unpack8(w)[i & 1u]` - a byte2 lane select - with sub-fields pulled out by
shift and mask. A 32-bit word with a variable shift runs slower; an `unpack8` of a 32-bit word
indexed by a runtime value (a byte4 dynamic select) drops the whole kernel off the block-load
path, to about a third of the rate. Every cm2 decode - q8 and the thirteen kq superblock formats -
is spelled the 16-bit way, which is why the block structs are `int16` arrays over the same
bytes. The IQ4_XS codebook is the one runtime-indexed read a decode makes: it is staged into a
16-entry `@workgroup` f16 table ahead of the tile loop (the reference exe's shared-memory table-staging form),
never selected out of a register vector per element.

Every kq format's four-wide twin is hand-laid (`decode_v4`, the template's `DECV4` axis): it
keeps the same spelling and shares what four consecutive elements share. A K-quant twin reads
its four quant bytes as two 16-bit lanes and extracts the sub-block's scale pair once; a grid
format's twin looks its grid word up once and takes the four bytes and the four sign bits from
it. The synthesized twin (`DECVEC`, the axis a new format starts on) repeats the whole scalar
body four times - the lane selects, the scale-plane words, the grid lookup and the sign parity
- and on the grid formats it lost to the scalar callback for exactly that reason. The twin
computes each element in the scalar's operation order, so the tile's CPU oracle holds under
either callback; which callback a box runs is the `device ready` line's `four-wide decode`.

### 2.2l The cm2 tile pick and the coopmat default ladder {#cm2-tile-pick-and-default}

**The l/m tile pick is a wave-efficiency comparison.** For a GEMM of width `d` over `cnt` rows
the l tile (256-row columns) and the m tile (128-row columns) each need some number of
workgroups; each grid runs in whole waves over the device's SM count, and the pick compares
occupied slots over allocated slots, cross-multiplied. The m tile wins only on a strict win; a
tie goes to l, whose bigger tile carries twice the arithmetic intensity. Three rules sit ahead
of the comparison: a region of 64 rows or fewer takes the s tile (32-row columns - the MoE
expert-bucket shape, where a 512-token window routes ~32 rows to each of 128 experts and an m
column would pad three quarters of every tile and take the edge path on all of them), a window
of 128 rows or fewer takes m (the l column would run half empty), and a device that reports no
SM count takes l and never splits k. The pick is PURE in `(d, cnt, sm_count)`, so the class the
pipeline binds and the tile rule the meta fill writes can never disagree; `cnt` is the AVERAGE
rows per active region of the dispatch, so one tile serves every region of a MoE schedule. The
narrow-n end below s is GEMV's. The s tile's fast path loads a partial 32-row column UNCLAMPED
and clamps only the store, so every f16 plane the chain feeds it - the gathered activation
image and the hidden plane - is sized with 32 rows of slack past its last region
(`ffn_cm2_chunk_rows`).

**The split-k pick counts the dispatch group, not the GEMM.** Long K (2048 and up) on a grid
that would fill under half the SMs splits the reduction across f32 partial planes that
`SplitKReduce` sums (three chunks up to two thirds full, at most eight, each chunk 256-aligned
and a split that would strand an empty tail shed). The grid it measures is the role's own
workgroups PLUS those of the chain neighbours it runs beside - q with k and v, gate with up
(`cm2_tiles`, the same pick each neighbour's own dispatch makes) - because the hazard-mask rail
lets independent roles co-run, while every split role serializes through the one scratch plane
(`VHZ_SK`) its neighbours would also claim. Counted alone, a 512-wide k or v projection over a
512-row window fills 16 of 36 SMs and splits in two; counted beside q it runs whole, and k and v
fill the device together. Split-k is left to the lone role - wo, down, a small model's
classifier - whose grid nothing else pads.

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
callback is NOT in that configuration: a cm2 tile names both callbacks
(`coopmatLoadTensorDecode`'s tenth argument: the format's own `decode_v4` where the template's
`DECV4` axis is on - every kq superblock format today, sec.2.2k - else the `DECVEC` axis,
which synthesizes the twin from the scalar body and is where a new format starts, its
`cm2:<fmt>` probe row deciding whether a hand-laid twin is owed), the device created with
`DASLLAMA_VK_DECVEC` and the extension decides which
one the driver runs, and neither choice shapes an image byte, so the bake identity ignores it
(the configuration's own rule: a serve-only knob is never a field). `decvec_on` is the run's arm,
announced on the `device ready` line.

**The mm mode serves the kq formats through the same template's KHR arm.** A device with
KHR_cooperative_matrix and no NV_coopmat2 (every AMD and Intel part, the GTX and Turing lines)
has no decode-in-load tensor API, so the cm2 template carries a second body under its `KHR`
axis: the f16 weight tile is staged by hand - each of the 256 threads writes one weight row's
16-wide half of the 32-wide k step through the format's `khr_stage16`, which reads the run's
values from the quant plane as one or two 16-byte words (`wq4`, the plane aliased as `uint4` on
its binding; the byte-granular grid formats read 32-bit words, `wqw`) and the row's scale words
once, and the activation half from the f16 plane as two 16-byte words (that plane aliased as
`uint4` on its binding) - both into `@workgroup` `uint` arrays of f16 pairs at a stride of 20
words (16 plus 4 pad, so the fragment loads spread across banks; the activation words land as
they are, the decoded weights packed two halves to a word). The eight subgroups tile the step two
by four: each owns 64 weight rows against 32 tokens as eight 16x16 f16 accumulators (a
`coopmatAcc_f16_16x16[8]` walked under `for [unroll_full]`, so every fragment sits in registers)
and loads four weight fragments and two token fragments per 16-deep multiply-add round - 48
fragment loads per workgroup per k step against 144 when every subgroup owned a 16-row strip
across all 128 tokens. The accumulators widen to f32 (`coopmatConvert`) before the store: a
whole tile stores its fragments straight into y column-major at stride `d` (y is token-major, so
a fragment's (weight, token) is `y[token * d + weight]`); an edge tile bounces each widened
fragment through the weight staging array - free once the k loop ends, and its 2560 words hold
the eight subgroups' 256-word fragments - and writes under the row and column bounds through a
bit cast. The two staging arrays are the whole footprint by design: a third array for the
bounce cost the tile a seventh of its rate (the probe's `slab` arm), because 8 KB more shared
memory per workgroup is one workgroup fewer per SM. The k step is 32 (the reference exe's BK):
a 64-deep step doubles the staging tiles, halves the workgroups an SM holds, and with the 8 KB
iq2s grid beside them reaches the 49152 B of workgroup memory the tier requires of a device -
its floor; a 32 KiB part is not a target, and no kernel here is sized for one. The arm exists
at ONE geometry - 128 weights by 128 tokens, k step 32 - so in mode 3 the tile pick answers 128
and split-k never engages, and `cm2_cls_ensure/set/enc` route to the `khr_cls_*` ladders, the
same `(fmt)` key on both. The f16 feed admits a kq format in mode 3 only on a 32-lane subgroup
(`khr_kq_tile_on`): the body indexes eight subgroups over the tile, so a wave64 device (four
subgroups per 256-thread workgroup) keeps its kq planes on the sdot4 batch tile until the wave64
twin lands. q8 never arrives here - its mm-mode GEMM is the q8-fed mul_mm L-tile. What each
lever is worth, measured from the shipped tile by moving one lever back
(`harness/vk_gemm_probe.das -- khrx`, the RTX 5060 Ti, the 4B gate / down / q shapes,
TFLOP/s): the shipped class 53.8 / 59.8 / 57.9 and its probe copy 58.7 / 63.8 / 60.3 (the class
carries the region and split-k arithmetic the copy omits); the copy staging through the
four-wide decode callback in place of the words - eight 16-bit lane loads and three scale words
per 16 values - 35.4 / 35.9 / 35.2, the load-store pipe at 81% of its peak on that stage (Nsight
GPU Trace); with f32 accumulators 42.4 / 44.2 / 43.3, the register file at 99% and two
workgroups of eight warps per SM on the wider ones; on the 16-row strip tiling 54.1 / 57.2 /
55.8; the reference exe's geometry - four subgroups of 64 x 64 in a 128-thread workgroup,
sixteen accumulators each - 53.2 / 57.9 / 57.6, no better; a constant fill in place of the
weight stage 65.4 / 66.4 / 64.6, the loop's ceiling. The reference exe's own KHR tile reads
43-46 on the same shapes. The order mattered: while the callback stage held the load-store
pipe, neither the f16 accumulators alone (32.7 against 33.4) nor the tiling alone (35.5) moved
the tile, and the reference geometry ran at 22 (about 245 registers per lane, one workgroup per
SM) - the stage had to go first. The decode body the tensor load names as its callback still
serves the cm2 tiles and the CPU oracle; a kernel body can also call it on the plane element
itself (`decode_v4(wq[i], ...)`, the element's index travels and the callee chains through the
plane), the emitter capability `test_vkd_direct_decode` keeps.

**The tile's fast path is what makes the loads unclamped.** It runs when the weight tile is
whole (`m0 + 128 <= d`), the token column is whole or stamped s, and K is a whole number of BK
steps; the layouts are then created clamp-Undefined and the B and output strides are masked to
a multiple of 8 f16 (`stride &= ~7`). The mask is an identity on today's shapes - `n` and `d`
are 32-multiples - and it exists to make the alignment PROVABLE to the driver's address
analysis, which is what keeps the loads on the wide path. The s column gates only the weight
tile: its partial token column loads unclamped and its store clamps. Everything else takes the
edge path with clamped layouts.

**The no-split arm keeps literal loop bounds and a literal store base.** Where `ksplit` is zero
the k loop runs the literal `0 .. n` with the store at the row base rather than the general
`k0`/`k1`/`ybase` form, although those values are exactly `0`, `n` and `0` on that path: the
general spelling cost 27% of prefill throughput (`benchmarks/lcpp_bench.das` pp512, 5060 Ti).
The split arm keeps the general form.

### 2.2m Class-pipeline creation is the Vulkan tier's one shader A/B seat {#vk-class-pipeline-build}

`vkd_class_pipe` is the single place a class kernel's SPIR-V becomes a pipeline, so both shader
instruments hang there and nothing else has to know about them. The four-wide decode fallback
hangs there too: when the device was created without `VK_NV_cooperative_matrix_decode_vector`
(`decvec_on` false) the served words go through `strip_decode_vector` (the capability, the
extension and every load's `DecodeVectorFunc` operand removed, the scalar callback left to
serve), after the override and before the shader module, so a dumped or overridden blob is
always the emitted, unstripped one. The seat is also the in-process A/B: `vkd_pipes_rebuild`
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
pipelines, so an A/B compares two whole runs. Plain is the default: pinned measured slower on
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
tier per layer (`moe_gpu_ffn_xf_ok`): the answer is yes only in mode 4 on a coopmat2 device, for
a gate/up/down triple whose every format the f16 feed admits (sec.2.2l), with the window inside
the x plane's cap - and on yes it skips its own requant and gather, so the CPU cost of the
layer's FFN is the routing alone. The f16 form is the combined (`npos > 0`) form only: the
combine is what makes the device-side gather pay, since neither the gathered image nor the
bucket rows ever cross PCIe. Streamed groups take the same arm after the slot bind.

**The per-op attention chain runs the same cm2 flash-attention tile the resident chain runs**
(`fa_cm2_h64` / `h128`, `ARCHITECTURE_GPU_VULKAN.md` sec.2.2j) when the device carries the coopmat2-fa trio, the fa knob is
on, the head size is 64 or 128, and the model's attention is not gated - this chain wires neither
the h256 stamps nor their gated epilogue, so gated models keep the flash-style `at_attn` pass. The tile reads
f16 K/V: the chain keeps its f32 roped-k / raw-v planes at absolute positions for the host
readback the CPU cache store consumes, and fills f16 shadows of them with the base-less
`f16cvt` over the whole attended prefix each window; the fa output lands in the same out plane
`at_attn` writes, so the requant and `wo` stages never learn which pass ran.

**The per-op attention chain adds a q/k/v projection bias (qwen2moe) in its prep stage.** The
layer's `[q | k | v]` row uploads to one device buffer per call and binds to `AtPrep`, whose q and
k passes add their slice (`boff` 0 and `qd`) to each projection element before the norm and the
rope - where the CPU chain adds it. v has no prep pass of its own, so a biased layer runs a third
`AtPrep` over the raw v window with the rope half 0 and the norm off, which makes the kernel a
copy plus bias in place (`boff` `qd + kv_dim`); the copy into the absolute-position v plane and
the host readback then both carry the bias. A model without the bias runs the two passes with
`hasb` 0 and never reads the binding, and the rail's `arch_ok` no longer names the bias.
