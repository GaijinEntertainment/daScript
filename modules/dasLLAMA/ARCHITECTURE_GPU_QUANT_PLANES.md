# dasLLAMA Architecture - the Metal quant plane reads

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document
carries the split scale plane a Metal blob stores a superblock
format's scale row in, the iquant GEMV codebook grid read with its per-box f4-slab twin,
the GEMV site abstraction the decode families share, the fat-threadgroup iq4xs family, the per-32 planes'
small-batch form and wide one-row GEMV, and the form matrix every Metal GEMM stamp family keeps. The GPU
backend role table these sections build on stays in `ARCHITECTURE_GPU.md#gpu-backends`.

### The Metal kq split scale plane {#metal-kq-split-scale-plane}

Every superblock format but k4, k5, q40, iq4nl and nv4 stores its Metal-blob scale row SPLIT into two
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
ignores `doff`. nv4 binds the same way: its 16-byte row is the sixteen UE4M3 sub-block scales
verbatim, byte `2g + h` scaling half `h` of 32-block `g` (`nv4_sub_scale`, the byte at half through
`ue4m3_half_f32`), the iq4 family's stamps reading it under `NV4` with the doubled e2m1 codebook
(`mx4_val`) in the codebook slot. A plane under a per-tensor `.scale` (`Model.site_scale`) declines
`kquant_native` by site and member: the kernels apply no epilogue factor. The Vulkan tier does not use this form; the device scale row it binds is
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
five-word block, and the `AFF` constant the affine members' verbatim codes under their 64-group pair,
`ARCHITECTURE_ENGINE_FORMATS_MLX.md#mlx-metal-forms`) - a per-32 plane binds its own plane pair (`kq_dense_b32`,
`b32_planes_of`) where a superblock plane binds the split scale plane.

### The fat-threadgroup iq4xs GEMV family {#metal-kq-fat-gemv}

**The single row's iq4xs sites take a 256-lane threadgroup of sixteen rows over one staged x; the
plain 64-lane stamp stays beside it.** `MetalKqFatIq4T` (`dasllama/dasllama_metal_kernels.das`) is
eight simdgroups of the plain stamp's row pair on the plain stamp's lane map and sum order, so a
row it writes is the plain stamp's bit for bit; what changes is that x is staged ONCE a threadgroup
in threadgroup memory instead of once a row pair, and that up to three weight planes ride one
dispatch over that x (`KqFatArgs.tg1`, `tg2` the slots' first threadgroups, a slot's quant plane
and its scale buffer bound as bytes and as halves, the d plane `dd<slot>` halves past the strips -
one bind a view, because a generated builder holds 32 arguments and the plain layout's three binds
a plane would not fit three planes beside the x, the y rows and the fused seams). The stash is
sized to the dispatch: the stamps declare `tgmem_param = "tgb"` and the dispatcher passes
`kq_fat_tgb(n)` - the layout's members ahead of the stash (name-sorted by the emitter: the
codebook, the reduce slab) plus K floats - so a 512-wide reduction leaves a core room for eight
threadgroups where the 16 KB a 4096-wide one needs leaves it two. Four stamps carry the fused
chain's seams: the plain form, `N` (the row's rms norm under a norm row in the prologue - the
residual seam's column order, `mad` squares, workgroup reduce and `w * (x * inv)` spelling, so the
normed x matches the seam's to the ulp the compiler's math mode leaves between two kernel bodies,
which is the kernel cell's bar), `A` (one plane, y = resid + dot, in place where y binds as the
residual row) and `NX` (the gate and up planes over the same rows, silu(gate) x up into one y, the
normed x stored by threadgroup 0 for the kernels that read it after). `enc_site_gemv` takes the
plain fat form for an iq4xs site whose reduction is a multiple of 256 within 4096 under
`DASLLAMA_METAL_KQ_FAT` at 1 or more; the fused chain the stamps serve at 2 is
`ARCHITECTURE_GPU_MTP_DECODE.md#metal-fat-chain`.

### The per-32 small-batch form {#metal-b32-small-batch}

A per-32 dense plane serves two to eight x rows - a batched decode step, the verify, a prefill window
of eight rows or fewer - in ONE dispatch over its own planes (`enc_b32_mvb`), and the mul_mm tile past
eight (`enc_kq_site_b`). The form is the GEMV's template under its `MVB` axis (`mvb_body`): the
GEMV's lane map and row pair, plus `NC` x columns a column group on `gl_WorkGroupID.y` - two, four or
eight, stamped `MetalB32MvB<NC><Fmt>`, past eight rows the eight-column stamp over `ceil(rows / 8)`
groups. A lane decodes its block once (`blk_w`: q5_1's `d * (q + 16h) + m`, iq4nl's codebook value)
and dots it against every column through `blk_dot`, the text the GEMV sums, so the plane streams once
for every row where one dispatch a row streamed it once a row.

**A batched row is the two-row one-row GEMV's bit for bit; the wide form (planes of 512 rows and
more, `#metal-b32-wide-gemv`) has no batched twin.** Both forms sum one lane's dot in one order and
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

### The per-32 wide one-row GEMV {#metal-b32-wide-gemv}

A per-32 plane of 512 rows or more serves its one-row GEMV on the wide form (`wide_body`, the
`MetalKqGemv<Fmt>W` stamps on `MetalB32GemvWideT`; `b32_gemv_form`), the two-row and K-split forms
below that. A lane takes a whole block - its eight quads (`WQ`), one 16-byte read of a four-bit block -
and a simdgroup 32 blocks a step; the lane loads its block's x once, sums it, and holds it against the
simdgroup's `RS` rows. Each row folds once a block, `sc.x * (codes . x) + sc.y * sum x` - the same
fold for every member, since q5_1's `m`, an affine member's bias and the codebook members' zero all sit
in `blk_scale`'s second half - where the K-split form folds the pair into every four-element quad.
`blk_code` gives the codes the fold multiplies: q5_1's `q + 16h`, the codebook value, and an affine
member's code masked in place, never shifted, its bit offset's power of two divided back out of x
once a step (`x_prescale`), so the dot is exact wherever the shifted one is. A row past the plane
reads the plane's last row and stores nothing. The form compiles with fast math: it has no batched
twin whose rows it must equal bit for bit, and the batched rows (`#metal-b32-small-batch`) keep their
own pair of forms.

Raced on the Qwen3.8-27B mlx4b shapes against the K-split stamp every one of them took, planes
streamed from DRAM (M5 Max, us a dispatch, GB/s over 0.5625 bytes an element): FFN gate or up 17408 x
5120 98.8 against 87.6 (507 / 572); down 5120 x 17408 101.0 / 91.2 (497 / 550); deltanet qkv 10240 x
5120 60.3 / 52.9; z 6144 x 5120 37.9 / 32.7; out 5120 x 6144 38.2 / 32.8 (464 / 540); attention q 12288
x 5120 71.4 / 63.0; k or v 1024 x 5120 8.6 / 7.0; head 248320 x 5120 1299 / 1196 (551 / 598). q5_1
and iq4nl32 gain alike (iq4nl32's deltanet out 420 against 524 GB/s); mlx8b ties, already at the
bandwidth its 32-byte block reads. Under 512 rows the forms tie or the K-split one wins (256 x 4096
6.2 against 6.8). More rows a simdgroup loses on this box - four rows 513, eight 492 GB/s at the
deltanet out against one row's 540 - and half a block a lane loses to the whole block (505), so the
stamps take `RS` 1 and `WQ` 8; both stay template constants. The whole Qwen3.8-27B mlx4b step (one
row) reads 27.69 ms on the wide form against 30.55 on the K-split one. `DASLLAMA_METAL_B32_WIDE=0`
pins the two-row and K-split forms; `metal_b32_wide_count` counts the wide dispatches.

A routed stack's expert-indexed GEMV takes the same body on the gathered site (`MetalMoeGemv<Fmt>W` on
`MetalMoeGemvB32WideT`, `enc_moe_gemv_b32`): one expert row a simdgroup, the slot and the stream on the grid,
on an expert of 512 rows or more in a `B32_MOE_WIDE_FMTS` member. Raced on the one-row step against the
two-row form (three processes a side, M5 Max): the Qwen3.6-35B-A3B mlx4b snapshot 5.18 against 5.46 ms,
the Qwen1.5-MoE Q5_1 mint 4.22 / 4.35, its Q5_0 mint 4.19 / 4.21, its Q4_0 mint (q4032 downs) 3.57 / 3.54 -
so the codebook-table members (q4032, and iq4nl32, which shares its block and table walk) keep the two-row
form. `DASLLAMA_METAL_B32_MOE_WIDE=0` pins the two-row form;
`metal_b32_moe_wide_count` counts the wide dispatches.

### The per-32 fused gate|up GEMV {#metal-b32-glu}

The single row's dense FFN on a per-32 member reads its gate and up planes in one dispatch on the
wide form (`enc_b32_glu`, the `MetalKqGemv<Fmt>Glu` stamps, the template's `GLU` arm): a simdgroup
owns one output row, holds the lane's block of x once against the gate row and the up row, and stores
`gk_act_mul(gate, up, act)` - no gate and up rows land in the panel, and no activation pass runs. The
two planes bind as one: both sit in the member's plane pair, so the stamp binds the lower one's offset
and reads each plane from its first block past it (`B32GluArgs.e1`, `e3`). A pair in two buffers, or
narrower than the wide form serves, takes the two GEMVs and the activation pass; so do the verify and
batch rows, whose fused form is q8's alone (`w13_fused_*`). The form ships off
(`DASLLAMA_METAL_B32_GLU=1` arms it; `metal_b32_glu_count` counts its dispatches): raced alone on the
27B's 17408 x 5120 pair it reads 171.2 us against the unfused chain's 175.6, but the whole step
(Qwen3.8-27B mlx4b, one row, M5 Max) measures 27.84 ms with it against 27.69 without, so the
isolated race does not predict the step here.

### The Metal GEMM form matrix {#metal-kq-form-matrix}

A Metal GEMM stamp names itself `stamp = "<family>:<fmt>:<form>"`: the family (`kq_mm` for the
dense GEMM, `moe_mm` and `moe_mm_split` for the routed experts' GEMM), the `KqFmt` member, and the
form (`t`, `th`, `th128`, `thdb`, ...). `check_kq_form_matrix` (`REVIEW.das`) reads every such
stamp in the `dasllama/dasllama_metal_*.das` files and holds each member that stamps in a family
to three things:

- **The family's whole form set.** The set is every form any member of the family stamps; a
  member that lacks one is a finding.
- **Its own race.** The Metal files carry the member's race name - `"kq_mulmm_<fmt>"` in the
  `kq_mm` family, `"moe_mulmm_<fmt>"` in the two MoE families - so a `metal_tensor_race` verdict
  picks between the member's tensor twin and its base.
- **A dev-W dequant pass, in the `kq_mm` family.** The member has an `enc_kq_dq_<fmt>` or an
  `enc_dequant_<fmt>h` dispatch, so every dense format takes the dev-W arm or none does.

The reason is measurement: two formats that differ in forms, race or dev-W arm dispatch through
different kernel sequences on one machine, so a timing compared across them measures the
sequences, not the formats' kernels.
