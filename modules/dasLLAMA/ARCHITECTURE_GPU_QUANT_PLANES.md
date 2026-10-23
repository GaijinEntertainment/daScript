# dasLLAMA Architecture - the Metal quant plane reads

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document
carries the split scale plane a Metal blob stores a superblock
format's scale row in, the iquant GEMV codebook grid read with its per-box f4-slab twin,
the GEMV site abstraction the decode families share, and the per-32 planes' small-batch form. The GPU
backend role table these sections build on stays in `ARCHITECTURE_GPU.md#gpu-backends`.

### The Metal kq split scale plane {#metal-kq-split-scale-plane}

Every superblock format but k4, k5, q40 and iq4nl stores its Metal-blob scale row SPLIT into two
regions of one buffer: the 16-byte sub-scale strips of every superblock first, then the packed
per-superblock d tail. A kernel binds that one buffer twice - the strips at `soff = sb0 * 16` and
the tail at `doff = nsb * 16 + sb0 * 2` - so the two reads stride independently and the strip
read stays 16-byte aligned. k2 is the one shape variation: its tail is 4 bytes per superblock
(`nsb * 16 + sb0 * 4`), because it carries d and dmin. `kq_scales_of` builds the pair;
`metal_blob_scale_plane` mints it at bake time, folding each format's 20-byte decoded row into
`[16B strips][2B d]` (k3's row is 18 bytes and is already in that shape). iq3s4's strip is its
eight `1 + 2s` bytes where iq4xs's holds eight signed sub-scales, so the iq4 family's stamps read
both off one walk, `IQODD` swapping the codebook (`iq3s4_val`, `2q - 15`) for `iq4_lut`. The 2-byte tail is why
a region's bind offset must be a multiple of 512 elements - the `(off/256)*2` d-plane bind is
4-byte aligned only then - which is what `metal_blob_off_ok` and `moe_site_ok` check. iq4nl is
the exception: it reuses q40's 16-byte plane of eight f16 d per superblock, binds once, and
ignores `doff`. The Vulkan tier does not use this form; the device scale row it binds is
`ARCHITECTURE_GPU_SHARED_GEMM.md#kq-dev-scale-row`.

### The iquant GEMV grid read and its f4-slab twin {#metal-iquant-gemv-grid}

Every iquant Metal GEMV - iq3s, iq3xxs, iq2s, iq2xs, iq2xxs - reads its codebook grid DIRECT
off the module's hoisted constant tables and float-expands each word in place. That is the base
form and the one that ships everywhere.

Three of them carry a second form, the f4-slab twin (`MetalKqGemvIq3sF4`,
`MetalKqGemvIq3xxsF4`, `MetalKqGemvIq2xxsF4`): the whole grid staged into threadgroup memory
once per threadgroup, pre-expanded to `float4` magnitudes, signs applied by select, so the
inner loop does no byte extraction. The twin is a PER-BOX CROWN, never a heuristic and never a
default - it wins on M5-class GPUs and REGRESSES on M1/M4-class, which have less threadgroup
bandwidth to trade for the byte work. Its PSO compiles only when the box's tune manifest
carries the crown (`metal_tensor_crowned("kq_gemv_<fmt>_f4")`), and `enc_kq_gemv` picks the
twin exactly when that PSO is non-null, so a box with no crown never compiles it. The iq3s twin
also has a gathered form (`MetalMoeGemvIq3sF4`, the same template over the site axis of
`#metal-gemv-site`): the routed experts of a model whose expert planes are IQ3_S read their
GEMV through it, `enc_moe_gemv` picking it under the dense twin's crown, since the per-threadgroup
work - eight rows of one expert against a staged grid - is the dense race's shape at a shorter
row count.

The `kq_gemv_iq3s_f4` and `kq_gemv_iq3xxs_f4` crowns are raced (`race_gemv_f4_twin`).
`kq_gemv_iq2xxs_f4` cannot be settled by an isolated race at all and is minted from a serving
A/B instead - `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity`.

### The Metal GEMV site abstraction {#metal-gemv-site}

The Metal GEMV families share one site abstraction: `MetalGemvSiteT` carries the site hooks -
`site_setup`, `y_rows` and the activation read - and its `GATHERED` axis selects the MoE-gathered
site (a `MoeGemvArgs` uniform plus `sel[slot]`) over the plain `ndim`/`ddim` pair, so a format's
dot walks one weight row against the x row at either site and a stamp carries only its own x and
y bindings. Every format's GEMV is one template on it, stamped dense and gathered: the superblock
families, and the per-32 set (`MetalBlk32GemvT`, q5_1, iq4nl32, q4032 and q5032 over their verbatim disk
planes: the `Q51` constant picks q5_1's block, and under the codebook block the `Q40L` constant swaps
iq4nl32's table for q4_0's linear `q - 8`, the `Q50` constant for q5_0's 32-entry `q - 16` over the
five-word block) - a per-32 plane binds its own plane pair (`kq_dense_b32`,
`b32_planes_of`) where a superblock plane binds the split scale plane.

### The per-32 small-batch form {#metal-b32-small-batch}

A per-32 dense plane serves two to eight x rows - a batched decode step, the verify, a prefill window
of eight rows or fewer - in ONE dispatch over its own planes (`enc_b32_mvb`), and the mul_mm tile past
eight (`enc_kq_site_b`). The form is the GEMV's template under its `MVB` axis (`mvb_body`): the
GEMV's lane map and row pair, plus `NC` x columns a column group on `gl_WorkGroupID.y` - two, four or
eight, stamped `MetalB32MvB<NC><Fmt>`, past eight rows the eight-column stamp over `ceil(rows / 8)`
groups. A lane decodes its block once (`blk_w`: q5_1's `d * (q + 16h) + m`, iq4nl's codebook value)
and dots it against every column through `blk_dot`, the text the GEMV sums, so the plane streams once
for every row where one dispatch a row streamed it once a row.

**A batched row is the one-row GEMV's bit for bit.** Both forms sum one lane's dot in one order and
compile with fast math off, because reassociation lets the compiler sum the two kernels' columns
apart; fast math measures no faster on these bodies. The block geometry is the class's template
constants (`BE`, `QW`, `SH`), the q5_1 stamps overriding the words and halves a block.

Raced against the one-row dispatches it replaces and against a q8 copy's mv form, weights streamed
from DRAM (M5 Max, Llama-3.2-1B q5_1 shapes): the 2048 x 8192 up plane reads 27.9 / 30.3 / 43.6 us at
two / four / eight rows against 46.0 / 83.7 / 159.1 one row at a time and 32.5 / 35.9 / 60.2 on the
q8 copy; one row is the GEMV's own time (27.1 against the previous body's 27.0).

**A narrow plane splits its K across a threadgroup.** The two-row form puts `rows / 2` simdgroups on a
plane, too few on a 512-row k/v plane to hide the block walk's latency, so a plane under `b32_narrow`
takes the K-split stamps (`KSPLIT`, the `_ks` builders): one row a threadgroup, its four simdgroups each
walking every fourth stride of the row's blocks, their partial sums added in simdgroup order by the
first. The GEMV and the small-batch form split alike, so a batched row stays the one-row GEMV's bit for
bit within a form; the two forms sum one row's blocks in different orders, so a plane's rows round
apart across them. The rule, raced per shape (us, one / two / four / eight rows, the two-row form
against the K-split one): a 2048 x 512 k/v plane 6.5 / 6.8 / 7.2 / 8.2 against 3.7 / 4.2 / 5.2 / 6.7 (the q8
copy 3.6 / 4.5 / 8.0 / 14.8), so a plane of 512 rows or fewer always splits; a 2048-row plane wins at
one and two rows (the 8192 x 2048 down 34.5 / 35.3 against 27.8 / 28.8) and loses past them (45.9
against 100.7 at eight); and a one-row read splits wherever K reaches 2048 (the 2048 x 8192 up 27.0
against 26.3), where an 896 x 4864 plane's short K loses it (6.3 against 10.2).
