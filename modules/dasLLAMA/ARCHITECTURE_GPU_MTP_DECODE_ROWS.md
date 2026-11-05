# dasLLAMA Architecture - the Metal decode driver's small-batch rows forms

Companion to `ARCHITECTURE_GPU_MTP_DECODE.md`; a section is cited by its anchor.
### The K-quant small-batch form is a per-box crown {#kq-rows-crown}

**A verify row on a K-quant plane costs what the box says, not what the kernel comment says.**
At two to eight rows `enc_kq_site_b` dispatches either the small-batch twin (one weight pass per
row group) or B single-row GEMV passes. The K-quant GEMV is ALU-bound, so sharing the weight pass
pays only where ALU is plentiful: on the M5 Max the two-row twin costs 0.52-0.89 of two passes,
on the M4 Pro 1.35-1.5 for k4 while k6's twin still wins. The mint races the production wrappers
per format (`race_kq_rows`, twin against two passes) and crowns `kq_rows_<fmt>` where the passes
win; `metal_decode_init` reads the crowns once into `g_kq_rows_crowned` and the dispatcher takes
the passes exactly there. An unraced box keeps the twin. The M4 Pro's Qwen3.8-27B round paid
about two steps for its two verify rows under the twin (the round clocks, `PERF_LEDGER.md`'s MTP
section) - the reason its depth-1 speculation lost.

The k4 twin itself has two forms, and the form is the box's too. The ext twin gives a thread one
weight row and two x columns; the two-row register tile (`MetalKqMvB2K4R2`) gives it two weight
rows and both columns, so every x float4 load feeds two rows - fewer loads per FMA, twice the
live registers. The tile wins where ALU is short (M4 Pro: 0.74-0.96 of two passes against the ext
twin's 1.36-1.49) and loses where it is not (M5 Max: 0.63-1.07 against 0.52-0.89). The mint races
the two production twins first (`race_kq_k4_form`) and crowns `kq_mvb2_k4_r2` where the tile
wins; `enc_kq_mvb` takes the tile at two rows exactly there, and the k4 rows race that follows
meets the box's twin form. An unraced box keeps the ext twin. Where the tile is crowned, three to
eight k4 rows never take the four- or eight-column forms either: `enc_kq_mvb_k4_pairs` walks the
tile over the column pairs and gives an odd tail one single-row pass. The wide forms are the ext
algorithm at more columns, and on the box that crowned the tile they cost more than B single
passes while two tiles do four rows in well under the four-column kernel's time; a third verify
row under the four-column form cost that box's Qwen3.8-27B round more than two extra steps (the
round clocks and the row ladder, `PERF_LEDGER.md`'s MTP section). In both races the
first arm is the incumbent and the second is the "tensor" side whose win crowns: `race_kq_rows`
times the twin against B single passes (the passes are "tensor"), `race_kq_k4_form` the ext twin
against the tile (the tile is "tensor").

**The k4 rows forms read x as halves where the box crowned it.** Every f32 form above is bound by
the x vector, not the weights: each decoded weight is dotted against a float4 of x fetched per
column, two bytes of x a weight against 0.56 of weight, so on the M1 class every form costs 0.65
to 0.8 of a single pass per column whatever its lane map. The half-x forms read x as `half4` and
halve that traffic. Every small-batch form shares one x mixin (`MetalKqXT`: the f32 rows or the
f16 twin at the same slot, `XHALF` picking), so a format's half stamp is its f32 form's class plus
`override XHALF = true`, and the dispatcher (`enc_kq_mvb`, a table of each format's six builders)
swaps the stamp and binds the twin. k4 alone rides wide forms instead (`MetalKqMvWK4T`: the
single-row GEMV's lane map over two, four or eight x columns), which land at 1.26, 1.8 and 2.8
passes for two, four and eight columns on the M1 Max, and under the f32 forms on the M5 Max too.
Their x is the f16 twin the row producer stores beside its f32 rows - the dual-store stamps of
the rows norm (`MetalAddRmsHX`, `MetalRmsNormHX`) and of the activation (`MetalSwigluHX`,
`MetalGegluHX`) - never a conversion dispatch: `StepRes.bxh` twins `bxb` and `StepRes.bfgh` the
gate panel, each eight rows deep because a form reads every one of its columns, and `xh_ok` /
`fgh_ok` say the twin is the panel's current rows (the norm sets it, attention and the w2 site
clear it; wo's x, the attention output, has no twin and keeps the f32 forms). The step writes the
twins where `kq_half_rows` holds - two to eight rows, and a plane whose format's crown is on
at a site the forms serve - or where the rows8 tables ride beside them (`step_twins`, `#rows8-affine`). `enc_kq_rows_half` routes a format: k4 to the wide form of its row
count (two rows the two-column, three or four the four-column, five to eight the eight-column),
the rest to `enc_kq_mvb` with the twin; the prefill's short-window peel hands its own dual-stored
twin through `pf_enc_kq_site_mm`. One crown a format, `kq_rows_half_<fmt>`: the race
(`race_kq_half`, the last of the decode races, once per format of `KQ_ROWS_RACE_FORMATS`) times
the box's f32 route - `enc_kq_rows_f32` under the rows verdicts before it - against the half
route at two, four and eight rows in one timed encoder, on a 27B-class FFN plane past the
last-level cache (the half route is "tensor"); `DASLLAMA_METAL_KQ_HALF=0` keeps the f32 forms
under every crown. The numerics are the prefill tiles' f16 X, already inside every parity bar;
the kernel cells hold every format's route at every row count (`test_metal_kq_half_forms`) and
the rows parity cell holds k4 and k6 token for token (`test_metal_decode_parity.das`, the
`batchB8-kq` arm's half-x leg).

**Five to eight rows take the matrix-unit form (`#rows8-matrix-unit`) on a k4 or q8 plane where the box's layout probe
passed, else the eight-row tensor stamps where the plane's tensor tile is crowned.** The row forms above
decode a K-quant weight once a column (the ALU bound at eight columns reads 3-3.5 passes a plane on the M5 Max) and
stream a q8 plane twice through the fixed-B4 ladder; the eight-row stamps (`kq_mm:<fmt>:t8` / `tsk8` on every dense
format's tensor class, `MetalQ8MulMmT8` / `TSk8` on the q8 blob: `MT = 8`) decode and stream a weight once for the whole
panel and run the tensor op at eight rows. `enc_batch_site` offers them first at five to eight rows on every tier
but the q8 mul_mm rail, whose panels pad otherwise (`pf_enc_kq_mm8`, `pf_enc_q8_mm8` in the prefill module, which owns the tensor stamps): a plane whose format's tensor
crown (`kq_mulmm_<fmt>`, `mulmm_q8`) is off, whose rows are not whole 64-row tiles, or - the q8 stamp - whose `d` is off 32
falls through to the row forms, so an uncrowned box is unchanged. The whole-K stamp alone dispatches `rows / 64`
threadgroups, too few to fill the device on a 5120-wide plane, so the split-K stamp carries the panel under the K-quant
scaffold's grid target (`metal_split_k_tgs_kq`) for the q8 plane too - the q8 32-row knee would leave an eight-row tile
at a quarter of the work on the same grid - and the reduce lands the partial planes at the site's y stride; a narrow
plane of 32-column tiles alone takes the split stamp at one way. The panel holds eight rows at five and up
(`ARCHITECTURE_GPU_MTP_DECODE.md#verify-row-pad`, the batch tier's pad), so the stamp's garbage rows land in owned scratch. On the M5 Max the 27B's
same-slab eight-row step falls from 2.28 to 1.78 plain steps (`PERF_LEDGER.md`, the block drafter's entry);
`DASLLAMA_METAL_ROWS_MM8=0` keeps the row forms (the A/B rail).

### The rows8 matrix-unit form {#rows8-matrix-unit}

**A k4, k6, iq4xs or q8 plane - or an affine member's (`#rows8-affine`) - at five to eight rows runs on the matrix unit with the
weight filled from registers: no threadgroup tile, no barrier, the codes exact.** The eight-row stamps above stage a decoded f16 tile in threadgroup memory for the
tensor op, and on the M5 Max that stage alone costs 1.4 one-row passes (a 64-deep chunk touches every 128-byte weight
line four times) while the op costs another 0.75 and the two never overlap inside a threadgroup. The rows8 kernels hand
`matmul2d` a LEFT operand the lanes fill from registers (the shader DSL's register-left `tmm2d_rx_*` flavor, whose
rule the dasMetal module's own architecture document carries) and the activations as a device half table: one 16 x 8 product a
scale block - 64 deep on k4, a superblock half; 32 deep on q8, a block - a simdgroup owning 16 weight rows, four a
threadgroup, 64 rows.

**Every format is an arm of one template, `MetalRows8T` in the prefill module; the formats differ in four hooks and nothing
else.** The left fill (the exact half of the code with its offset - k4 1024 + q sc, q8 1024 + q + 128, an affine member q - or
an int8: k6's code q - 32, iq4xs's codebook value), the product depth (`DEPTH`, one scale block: 16 on k6, 32 on q8 and iq4xs,
64 on k4 and the affine members, with
`NLOAD` code loads a `BLOCK`-wide block each feeding `NPER` products), the epilogue (the format's scale law over the block's
scale words and the producers' per-block token sums) and the slot law (where a product's k sits in the x table: k6's 16-blocks
8 h + 2 g + l16, every other format's products in order). The skeleton - the lane geometry, the K split, the block loop, the
plane store - is shared; the fill's arms stay in the kernel body, because the matrix unit's cooperative operand lives in the
body's scope. The arms (`MetalRows8K4`, `MetalRows8Q8`, `MetalRows8K6`, `MetalRows8Iq4xs`, the four affine members) are template instances whose
generated kernels compute the bodies they replaced bit for bit. `REVIEW.das`'s `check_rows8_kernel_strides` holds the fold:
a `MetalRows8*` class that is no template and declares its own `[metal_kernel]` body carries no format stride (256, 144, 210,
48, 34, 17); it licenses no names - the prep and the probe pass because they are format-free.

The lane geometry is the device's: a lane holds weight rows m0 and m0 + 8 (`rows8_m0`) and the k quads 4 kq + e of every
16-deep stripe (`rows8_kq`); its destination elements are tokens 4 kq + t of each row, and the lanes with bit 3 set hold
the pad columns of an 8-token product padded to 16. The x table carries the panel as halves in
the products' k-slot order - slot 4 kq + i of a 32-block at physical 8 kq + i, slot 16 + 4 kq + i at 8 kq + 4 + i - so a
lane's eight codes of a row are one 8-byte load, and each token row's sum of those halves over every 32-block beside it.
**The panel's producer writes the table where it writes the f16 twin.** On a step whose row count the form serves and whose
twins exist (`rows8_table_rows`), the norm into bxb (`MetalRmsNormHXT`, `MetalAddRmsHXT`, `MetalPreAddRmsHXT`) and the
gate panel's activation (`MetalSwigluHXT`, `MetalGegluHXT`) store each half a second time at its k-slot (`rows8_slot_at`)
and, a subgroup one aligned 32-block, the block's sum (`rows8_sum_at`), into the step's `bxt` / `bxs` and `bfgt` / `bfgs`;
the site reads them under the twin's validity (`site_x_twin`, `xt_ok` / `fgt_ok`), so the q, k, v, gate, up, down and
deltanet qkv / z sites dispatch no prep. The producers write the step's live rows alone: the pad rows' table columns hold
whatever the buffer last held, and the products' pad-token columns they feed are discarded. A panel with no producer table -
the attention output, the deltanet output, the classifier norm, the MTP head, a gemma-4 MoE step's norm - preps its own
(`MetalRows8Prep` into the global scratch, one prep a site: a panel's pointer says nothing about its contents between two
sites). The k4 left
element is the exact half 1024 + q sc (q the nibble, sc the sub-block's 6-bit scale: at most 945, so the bits are
0x6400 + q sc and the product carries no rounding), the q8 element 1024 + (q + 128); the superblock's d and dmin apply
once per product in fp32: y += d (dot - 1024 sum_x) - dmin (mn_0 sum_0 + mn_1 sum_1) on k4, y += d (dot - 1152 sum_x)
on q8.

**K splits inside the threadgroup, so y is written once and no reduce dispatches.** A threadgroup owns one 16-row tile and
its eight simdgroups take K's blocks interleaved - simdgroup s the blocks s, s + 8, .. (`BLOCK` wide: a 64-group on k4 and the
affine members, a superblock half on k6, a block on q8) - so the slices stream neighbouring weight lines at once. Each
simdgroup lands its eight accumulators in threadgroup memory ([slice][element][lane], `red`); after one barrier simdgroup s
sums element s across the slices in slice order and stores it at y's row stride. The grid is a threadgroup a tile
(`rows8_grid_of`): the 27B's gate / up 1088 threadgroups, its head 15,520, a 96-row q8 plane six - no plane needs ways for
occupancy, and no site writes planes. The shapes are race verdicts (`harness/rows8_mt_race.das`, `DASLLAMA_ROWS8_TG=1`: synthetic
planes past the SLC, three rounds in one process, the split-K form at its ways plus its reduce dispatch as the base): one
tile of eight interleaved slices reads 0.93-0.97 of the split-K form at gate / up, deltanet in, deltanet out and attention q,
0.79 at the head, 0.58 at the narrow k / v plane and 0.82-0.87 on the 8-bit member, against 1.01-1.02 at the 4-bit and k4
downs, the deepest K (68 superblocks) - the race times the reduce's kernel, not the dispatch gap it costs between the step's
kernels; contiguous slices, four or sixteen slices, two or four tiles a threadgroup, and a second outer way
over planes each tie or lose. A q8 plane under 256 rows the rows8 form declines rides the row forms, never the eight-row
tile stamp (`ROWS_MM8_Q8_MIN_ROWS`): a narrow plane's tile stamp and reduce outweigh its bytes.

**A k6 plane's product is one scale block deep, its scale in the epilogue.** A Q6_K code times its int8 sub-scale is not an
exact half (|q - 32| up to 32 times a scale up to 128), so the k4 trick of folding the scale into the operand does not
carry over: `MetalRows8K6` runs one 16 x 8 x 16 product per 16-element scale block, eight a superblock half, its LEFT operand
the int8 codes q - 32 (exact, the shader DSL's int8 flavor `tmm2d_rx_begin_i8`), and lands y += d sc dot in fp32 per
product. With no offset in the operand there are no token sums, and the product reads its RIGHT operand as the panel's
halves in natural order - no k-slot permutation. A lane's four codes of a superblock half h and an l16 come from three
4-byte loads (ql at byte 64 h + 16 l16 + 4 kq and 32 past it, qh at 128 + 32 h + 16 l16 + 4 kq) that serve the four
16-blocks g of that pair; each byte becomes q - 32 with its top bit set across the subtraction, so no borrow crosses a byte
(`rows8_k6_codes`). The scale and d come from the split scale plane's two binds (the strips at `soff`, the packed d tail at
`doff`), as the k6 stamps read them. Where the panel's producer wrote the f16 twin (`site_x_twin`'s `bxh` / `bfgh`), the
site reads it directly; where it did not - the attention output feeding wo, the drafter's panels - the site converts the
f32 panel into the rows8 scratch (`MetalHalfRows`, one conversion a site). The verify's classifier norm writes the twin
beside its f32 rows (`enc_rms_bhx`) where the classifier is a k6 plane on the form (`metal_rows8_twin_fed`), so the vocab
site needs no conversion. The raw-code form - unsigned codes with per-16 token sums carrying the -32 offset - measured slower.

**An iq4xs plane's product is one sub-block deep, its codebook in threadgroup memory.** An IQ4_XS weight is d ls kv[q], kv
the sixteen int8 codebook values (-127 .. 113) and ls the sub-block's signed 6-bit scale, so the codebook value is the exact
operand and d ls lands in fp32 per product: `MetalRows8Iq4xs` runs one 16 x 8 x 32 product a 32-element sub-block on the int8
flavor over the panel's halves in natural order, no token sums, y += d ls dot. A sub-block's 16 bytes carry element i in the
low nibble of byte i and element i + 16 in its high nibble, so lane kq's elements 4 kq + e and 16 + 4 kq + e of a row are the
two nibbles of the sub-block's word kq: one 4-byte load a row a product. The arm shares k6's binds - the split scale plane's
strips (the sub-scales, bytes 0 .. 7) and d tail, the quant plane at the third bind (`SPL`, the template's split-plane
switch) - and its x: the step's producers write the f16 twin wherever a five-to-eight-row site holds an iq4xs plane on the
form (`rows8_table_rows`), and the deltanet's input sites, the shared expert's gate and up and the batch step's classifier
read it; the deltanet and attention outputs convert their own panel (`MetalHalfRows`). The codebook lookup is the race's
verdict (`harness/rows8_mt_race.das`, the 35B's dense shapes, three rounds): a 256-entry table of a code byte's two values
(`iq4kv2`, staged by the threadgroup at entry), four lookups a word, against a 16-entry table (eight lookups) and a register
select over the four packed codebook words; the register select loses to both tables by a wide margin (its selects make the
kernel ALU-bound), the two tables tie, and the sub-block as the slices' unit ties the superblock. `DASLLAMA_METAL_PREFILL_SKIP=rows8noiq4xs`
keeps iq4xs on the eight-row stamps.

**The layout is probed before the form serves.** `rows8_init` compiles the kernels and runs `MetalRows8Probe` over two
one-hot tables: a lane fills a 16 x 64 left operand with row 64 + k under the geometry above, and every destination
element it assumes is (row, token) must read row 64 + k0[token], the pad columns invalid. A box whose matrix unit lays
its cooperative tensors out otherwise (another Apple generation) fails the probe and keeps the eight-row stamps;
`DASLLAMA_METAL_ROWS_MT=0` keeps them on any box (the A/B rail), and `DASLLAMA_METAL_PREFILL_SKIP=rows8prep` is the
knockout (every prep but the first skipped), `rows8nok4` / `rows8nok6` / `rows8noiq4xs` / `rows8noq8` each keep one format on the stamps; `metal_rows8_table_sites` counts the sites a producer's table or twin
fed, `metal_rows8_sites_of` the sites a format. The eight-row step's cost over the one-row step on the M5 Max is
`PERF_LEDGER.md`'s same-weights entry (1.15 on the 27B snapshot with the K split inside the threadgroup; the eight-row
stamps read 1.78 before the form).

**The DFlash2 drafter's sites take the same ladder over planes no `Model` holds.** Its pass and its commits
(`dasllama_metal_dflash`'s `df_mm`) offer the rows8 form first at five to eight rows, then the eight-row tensor stamps,
then the row forms - the q8 GEMV rows, and on a K-quant plane the f32-x rows route `enc_kq_rows_f32` - over the drafter's
own buffers: the Q8_0 blob, and a K-quant format's verbatim quants and its scale plane in the trunk blob's device form
(`metal_kq_scale_form`), bound at `kq_bind_offs` through the buffer-taking entries `pf_enc_rows8_kq_planes` and
`pf_enc_kq_mm8_planes` the `Model`-taking ones call. Every plane of the 27B's drafter is on the form's grid (its widest K is
fc's 25600, under the scratch's 32768), its Q6_K planes on the k6 form. A site preps its own
x table or halves (no producer of the pass writes one) and reduces its own planes, the pass's consumers being its own kernels; a
commit whose eight rows would run past the ring's end takes the row forms. `metal_dflash_sites` and
`metal_dflash_rows8_sites_of` (by format) and `metal_dflash_rows8_sites` count the drafter's sites since the attach.

Past eight rows the form is the panel's. The kq mul_mm twins dispatch `mp / 32` threadgroups along M, so `enc_kq_site_b` takes
them only over a panel padded to that tile (`mp` a multiple of 32) - the batch driver's at nine rows and up. The verify's panel
is padded to the GEMV forms' 4-row tile (`ARCHITECTURE_GPU_MTP_DECODE.md#verify-row-pad`), so its nine rows (`MTP_MAX_ROWS`, depth 8) ride the small-batch
forms in ONE dispatch: the eight- and four-column forms carry a column-group grid axis (`ceil(rows / 8)`, `ceil(rows / 4)`
groups), the eight-column form restaging its last live row in a group's columns past the live rows, so its x needs no pad
(`metal_kq_mvb_wide_count` counts the dispatches past eight rows); a box crowned to single passes keeps them past eight rows
too. An unpadded nine-row panel handed to the mul_mm dispatches no threadgroup at all; the twins' `mp % 32` contract names
that site instead of leaving the output unwritten.

### The affine members' rows8 form {#rows8-affine}

**An affine member's plane (mlx4, mlx8, mlx4b, mlx8b, mlx6b) at five to eight rows runs the rows8 form one 64-group a product,
its (scale, bias) pair in the epilogue.** A member's weight is s q + b with q unsigned
(`ARCHITECTURE_ENGINE_FORMATS_MLX.md#mlx-affine-planes`): q, 0..15, 0..63 or 0..255, is an exact half, s q is not, so the scale does not
fold into the operand as k4's does. The affine arms of `MetalRows8T` fill the LEFT operand with each code's half and run one
16 x 8 x 64 product a group - depth costs the matrix unit nothing - then land y += s dot + b sum_x in fp32, sum_x the group's two
32-block token sums of the half-rounded x the product read, so a site is the dense product over the f16-rounded panel. The pair
is one word a row a group (`factors`: BF16 halves on the b members, halves on mlx4 / mlx8, the scale low). `A8` or `A6` picks the
code width, `BF` the pair's half type.

**The x table is the k4 kernel's, and the member's lane map follows from it.** Slot 16 i + 4 kq + e of a 32-block sits at
physical 8 kq + 4 i + e (`rows8_slot_at`), and the probed geometry puts k-slot 32 c + 16 i + 4 kq + e of row m0 + 8 r at left
element ((c 2 + i) 2 + r) 4 + e, so lane kq's codes of a row in a group are elements 8 kq .. 8 kq + 7 of each 32-block c: on a
4-bit member word kq of the block (element k at bits 4k, so nibble 4 i + e of that word), on an 8-bit member the block's 8-byte
run kq (byte e of word i) - two loads a row a group (`aff_run`); on the 6-bit member the 48-bit run at bit 48 kq of the group's
twelve words (two words, at bit 16 of the first on an odd kq) and the same run 192 bits on, its four 24-bit quads' codes at bits
6e. The producers that write k4's table and sums serve the members
unchanged: a step whose five-to-eight-row sites hold a member on the form stores the twins and the tables for it whatever the
half-x crowns say (`rows8_table_rows`, `step_twins`). The members have no eight-row tensor stamp, so a site the form declines -
K off whole 64-groups, rows off whole 16-row tiles, `DASLLAMA_METAL_PREFILL_SKIP=rows8nomlx` (the knockout) - rides the
small-batch forms; `metal_rows8_sites_of` counts each member's sites.

**The operand, the table and the load are race verdicts, not assumptions.** The raw-MSL race (`harness/rows8_mt_race.das`,
synthetic BF16-pair planes past the SLC, three reps) times the form against the wide one-row form on
the 27B's 4-bit shapes; `PERF_LEDGER.md`'s same-weights entry carries the figures. The verdicts: a uint8 or int8 operand (an
8-bit code as q - 128, its 128 s folded into the bias) runs 1-3% slower - the kernel is load-bound; a 64-group table order
that puts a lane's codes of a row in one 8-byte (4-bit) or 16-byte (8-bit) load ties the two-load map; a half built as
1024 + q less 1024 ties the per-element conversion the kernel uses.
