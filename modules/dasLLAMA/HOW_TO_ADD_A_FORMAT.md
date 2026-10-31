# How to add a weight format to dasLLAMA

This document walks the code in the order a new format lands and cites the architecture section
that owns each step.

The worked example is IQ4_XS; where a later format took a different path, the step says so.

A backend is the CPU, Vulkan or Metal; "both GPU backends" means Vulkan and Metal. A site is a
weight plane a backend's serving call reads.

**A format is landed when it meets `REVIEW_KQ_FORMATS.md`'s site rule at every site x backend
pair step 0, question 5 lists.** `REVIEW_KQ_FORMATS.md` binds what serving means, the decline and
the cell that proves a served claim - read it before step 1; step 8's cells prove each pair.

## 0. Decide the format's shape before touching code

Five questions decide which existing family the format rides and where it serves; answer the
first three from `ggml-common.h` (the block struct) and `ggml-quants.c` (`dequantize_row_<fmt>`),
the last two from the tree:

1. **Block geometry.** 256-weight superblock (every K-quant, every i-quant) or 32-weight block
   (`Q4_0`, `Q5_1`, `IQ4_NL`)? A superblock format joins the `kq_sb` lattice (Q8_K activations, `%
   256` rows, the grp<mr> repack - `mr` rows interleaved per repack group - the stamped kq kernels);
   a 32-block format rides per-32 planes like `q51`, `iq4nl32` and `q4032`
   (`ARCHITECTURE_ENGINE_FORMATS.md#per-32-rail`).
   `ARCHITECTURE_ENGINE_FORMATS.md#formats-and-data-movement` owns the lattice split. A disk type
   may ride BOTH: IQ4_NL is `iq4nl` (a superblock member, eight 32-blocks a superblock) on a `% 256`
   row and `iq4nl32` on any plane - dense, expert or table - whose rows are off the 256 lattice and
   on the 32 lattice (`kq_fmt_row_ok`); Q4_0 is `q40` and `q4032` the same way. A disk type with no
   superblock member rides its per-32 member on every row (Q5_1 as `q51`, Q5_0 as `q5032`). A per-32
   member serves at every site on every backend (`ARCHITECTURE_ENGINE_FORMATS.md#per-32-tables`), so
   it brings the table sites' arms with it.
2. **Weight reconstruction.** Shift/mask nibbles (`q4_0`, K-quants), a 16-entry codebook on the
   nibble (`IQ4_XS`, `IQ4_NL`), or a grid gather (`IQ2_*`, `IQ3_*`)? Shift/mask and codebook
   formats keep the k4 nibble tiling and add an unpack step; grid formats decode to bytes first
   (the byte-expanded panel k5/k6 already use).
3. **Scale structure and its fold.** Per-block f16 (`q40`), superblock d/dmin + 6-bit sub-scale
   and min (`k4`/`k5`), superblock d + signed int8 sub-scale (`k6`, `IQ4_XS`). The fold decides
   whether the dot needs the activation block sums (`xbsp`): a min or an unsigned-nibble offset
   needs them, a signed reconstruction (codebook, k6's -32) does not. A constant factor in the
   reference dequant (the grid formats' 0.25 or 1/8) is removed at transcode by halving `d`
   (`f16_half_bits` is an exponent decrement), once per power of two, with the grid magnitudes
   stored halved where every level is even - the kernels then fold `d x strip x grid_byte` with
   no constant. The xor-and-add sign trick that fold relies on (`(g ^ mask) + m1`) is valid only
   because grid magnitudes are odd, so no byte carries into the next.
4. **Which existing template each GPU backend's kernels stamp.** A stamp is a class that compiles
   to a shader module (`class KqGemvIq4xs : Iq4xsGemvT {}`); a template is a `class template`
   other classes stamp or derive. A family template holds a kernel family's lane map, site hooks
   and guard arithmetic (Vulkan: `KqGemvLeafT`, `KqCm2BatchT`, `Blk32GemvRunT`; Metal:
   `MetalGemvSiteT`, `MetalKqTileDecodeT`, `MetalKqMulMmSplitTensorBase`, `MetalEmbedKqT`); a
   format template derives a family template and holds one format's block term (`Iq4xsGemvT :
   KqGemvLeafT`, `MetalKqGemvIq4T : MetalGemvSiteT`). A format with a tensor instruction of its own
   on one home (NVFP4: Blackwell's block-scaled FP4 mma) adds a home-own MMQ-shaped leaf there
   (`MmqFp4T` / `CuNv4Mmq` in `dasllama_cuda_classes.das`, its feed kernels beside it, the driver's
   leaf list and feeds in `dasllama_cuda_decode.das`) on top of the shared decode templates every
   home stamps - the non-native homes serve the format through those templates and stop there
   (`ARCHITECTURE_GPU_ASYMMETRIES.md`). A new format adds stamps, never a class
   written beside its family; `REVIEW_GPU_KERNEL_CLASSES.md` binds the class shape. Take the first
   rung that fits:
   - **An existing block term serves the format under a template constant** - the format's stamps
     set the constant, and its `static_if` arm in the template's block term switches the code
     table, level or fold. iq3s4 stamps the iq4 templates under `IQODD` on both GPU backends
     (`KqGemvIq3s4 : Iq4xsGemvT`, `Iq3s4Cm2T : K4Cm2T`, `MetalKqGemvIq3s4 : MetalKqGemvIq4T`). On
     Metal q40 stamps `MetalKqGemvIq4T` under `Q40`, and q4032 / q5032 stamp the per-32 templates
     under `Q40L` (iq4nl32's block, linear code) and `Q50` (q51's quant block under one d). On
     Vulkan the pairs derive the other way: `Iq4nlCm2T : Q40Cm2T`, `Iq4nl32Cm2T : Q4032Cm2T`, and
     `Q5032Cm2T : Q51Cm2T` under `Q50`. A format whose block is another member's block under a
     different code table, level or fold takes this rung on both GPU backends.
   - **The format's own block term on the family template** - a new format template, or the
     format's arm in the family template's block method. Vulkan: `<Fmt>GemvT : KqGemvLeafT`
     (`blk_decode`), a `KqCm2BatchT` format template (`decode`, `decode_v4`, `khr_stage16`), and
     for a per-32 GEMV a `Blk32GemvRunT` stamp naming its tag (`typedef FMT = KqTag_<fmt>`) with
     the format's `blk32_*` overloads on that tag. Metal: a GEMV format template on
     `MetalGemvSiteT` (`MetalKqGemvK4T` ... `MetalKqGemvIq4T` for the k-lattice and codebook
     formats, `MetalKqGridGemvT` with `blk_sum` for a grid format, `MetalBlk32GemvT` per-32), the
     format's arm in `MetalKqTileDecodeT.kq_decode` and `MetalKqMulMmSplitTensorBase.stage16` for
     the tiles, and a format template on `MetalEmbedKqT` for the gather (`MetalEmbedIq4T`). The
     block term holds only what the format changes; the lane map, the site hooks and the guard
     arithmetic stay in the family template.
   - **A new family template** - a new child of `KqGemvShellT` beside `KqGemvLeafT`, or a template
     beside `MetalGemvSiteT` - only when the format's index or guard arithmetic differs from every
     family's; it lands with a kernel cell per stamp and a race row.

   A new codebook format on Metal whose block is iq4's takes the first rung (a constant on
   `MetalKqGemvIq4T`, as `IQODD` is); one with a block of its own takes the second (a format
   template on `MetalGemvSiteT` beside `MetalKqGemvIq4T`). A row kernel the format brings that is
   format-free (a gather epilogue, a reduce) is one template in `dasllama_gpu_kernels_common.das`
   both GPU backends stamp (`ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-shared-kernels`, the ledger
   `ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-kernel-pairs`); the GEMV and GEMM tiles stay per
   backend, on each backend's family template.

   On Metal the format's tile stamps join the form matrix `check_kq_form_matrix` (`REVIEW.das`)
   holds, under the rule `REVIEW_KQ_FORMATS.md` states; list the format's forms, races and dequant
   pass here - a format landing one short reds the gate.
5. **Which sites each backend serves.** List every site x backend pair whose row length the
   format's block length divides and mark each served, with the step that adds its kernels, or
   declined by name at load (step 8 names each backend's decline). The list joins the PR body
   with the other answers, and step 8's served cells cover it.

   A format whose value carries a whole-tensor factor beside its block scales (NVFP4: ModelOpt's second
   scaling level, a `<tensor>.scale` GGUF tensor) keeps the plane bytes pure and lands the factor in the
   matmul epilogues (`ARCHITECTURE_ENGINE_FORMATS_NV4.md#nv4-site-scale`): `detect_site_scales` reads it per
   site after the layout, and every CPU path that reaches the planes - the two plane entries and the
   fused decode chains of `dasllama_blocks.das`, which invoke the rows cores on raw plane pointers and
   never pass through `mm_at_kq_pre` - multiplies its rows by it; the device rows carry the factor in
   their scale row's pad word, written by the gather, so the GPU decodes fold it with no epilogue (the
   kernel cells' fixtures fill that word through `kq_ws_plane`, never a bare `ws_plane` - a hashed pad
   reads as a wild f32, and a tile stages its scale table off the region's first row, so the word is one
   value a plane). A path left out serves the weights a thousandfold off and the text reads as garbage
   with every kernel cell green.

Write the answers down; they are the first lines of the PR body's format section.

**A served form of an existing disk type** (`iq3s4` = IQ3_S as a codebook plane,
`ARCHITECTURE_ENGINE_FORMATS.md#served-form`) is a member like any other, with four differences: the
descriptor row names the SOURCE type (`ggml_type`, `disk_bytes`) under its own schema and stream
ids; `kq_transcode_p` reads the source disk block and folds its decode into the served plane
(exactness against the source form is a `test_kquant.das` cell - both transcodes of one superblock,
dequants bit-identical, over LCG-perturbed source bytes); `kq_fmt_of` (`dasllama_load.das`) maps the
native tag to the served one behind an `[EnvConfig]` knob whose resolved value is a `DlimCpuConfig`
field (it changes plane bytes, so it is image identity - the struct's header rule); and a GPU tier
either decodes the served form or stays on the native one, decided per tier. A tier that decodes it
takes the whole of sec.6 (Vulkan) or sec.7 (Metal) for the member (iq3s4 on Vulkan and Metal: the
borrowed shape's stamps with its own codebook - Metal's iq4 family under `IQODD` - a census row and
a kernel cell per stamp; the kernel census is the test lists that hold each kernel's dispatch
counts, step 8). A tier that does not leaves the member off its roster and the knob's `auto` serves
the native form under it. A tier never half-joins: a member on some of a tier's ladders and on the
skip list of the others serves a load that passes admission and then has no kernel. The kernels are
the shape it borrows (iq3s4: the iq4xs LUT kernels over its own codebook, sec.4/5), and the tune
family is its own (`<fmt>q8_tile_gen` with the borrowed emitter's `fallback`, its two class-profile
rows copied from the shape it borrows).

## 1. Identity - `dasllama/dasllama_kqformat.das`

The taxonomy every other file keys off (`ARCHITECTURE_ENGINE_FORMATS.md#formats-and-data-movement`,
the `dasllama_kqformat.das` charter). One member and one descriptor row here; every
per-format accessor (`kq_sb`, `kq_qsb`, `kq_ssb`, `kq_elems`, `kq_schema_id`, `kq_stream_code`,
`kq_ggml_type`, the int-id twins in `dasllama_gemm_schema.das`) reads the row.

- Append the member to `KqFmt` - **append, never reorder**: the int value is the device stack tag
  (`vk_kq_schema_id`) and the image plane id. The member alone breaks every Vulkan build until its
  `<fmt>_batch_cm2e_cls` / `<fmt>_batch_khr_cls` stamps exist (`kq_tile_stamp` walks the whole enum
  in the `cm2_cls_*` ladder, and the kernel cells' `khr_cls_*` one), and a Mac whose dasVulkan
  module is not built never sees it - build the module first (`cmake --build
  modules/dasVulkan/build`; MoltenVK then also runs the sdot4 GEMV cells, the coopmat tile cells
  skip) and keep the identity and the sec.6 stamps in ONE compile-checked step.
- The `GGML_TYPE_<FMT>` constant, the stride constants `<FMT>_QSB` / `<FMT>_SSB` (bytes per
  superblock row of the quant and scale planes), and the `kq_desc` row: strides, weights per
  stride unit, disk bytes per stride unit (the ggml block bytes), the ggml type, the kernel/IR id
  and the stream code. The kernel ids are mnemonics - K-quants by bit width (4/5/6), Q4_0 = 40,
  i-quants = bit width x 10 + a variant digit (IQ4_XS = 44). Three id spaces exist -
  `int(KqFmt)`, the kernel id, and the stream/repack region code (0/2 are q8/q51, else the kernel
  id) - and the row carries all three; `test_kqformat` pins them and holds the disk bytes to
  `ggml_type_bytes`. A kernel id of 0, 1 or 2 collides in the stream space (Q2_K streams under
  20 and translates back at every dispatch boundary), so dodge those or claim a distinct code the
  same way, and key every region ladder on the translated kernel id, never on the stream code by
  position.
- A per-32 member also declares its device-plane block here: `KQ_<FMT>_QWORDS` (its quant words a
  block, where they are not the four nibble words of `KQ_B32_QWORDS`) and `KQ_<FMT>_SBYTES` (its
  scale bytes), the numbers the GPU stamps' block constants spell. Pin them in
  `tests/test_kqformat.das` in the same step: `test_b32_plane_words` holds words and bytes to the
  row's `disk_bytes`, and `test_metal_b32_block_constants` holds each Metal stamp's literal (`BE`,
  `QW`, `SH`, `B32W`, `B32S`, `B32E`) to the row's strides.
- A codebook goes here too; a grid or codebook table reaches worker lanes only as a function
  returning the literal (`iq4nl_lut()`, `iq3s_grid()`, `iq2s_grid2()`: a direct
  `return fixed_array<T>(...)`, no local), never as a module global - a team-lane kernel reads a
  `let` global as zero; the global twin (`IQ4NL_LUT = iq4nl_lut()`) serves tests and oracles.
- A signed-grid format (two grid words per octet, a sign byte flipping element j) shares the
  octet with the five it joins: `grid_octet_dot` (the dot), `store_grid_pair` (the dequant),
  `store_grid_panel_pair` + `sign_masks_iq` (the panel unpack) - what a new member writes is
  the grid-index and sign derivation alone. The helpers are generic in their operand ON PURPOSE:
  a generic instantiates in the caller's partition, and the split-module JIT inlines within a
  partition only - a plain kqformat function would be a real call per eight weights.
- `dasllama_gemm_schema.das`: `kq_reads_packed_planes` - the one packed-versus-panel predicate
  the batch cell generator, the probe, the tests and the bench all read.
- `tests/test_kqformat.das`: pin the enum value, the predicate, the strides, the id, the stream
  code, and the codebook's edge values; its radix guard holds every `int(KqFmt)` value under
  `DAT_KEY_FMTS` (`dasllama_vulkan_common.das`, the Vulkan decode block's layer-key radix) - a
  member whose `int(KqFmt)` reaches `DAT_KEY_FMTS` grows the radix in the same change.

## 2. Codec - `dasllama/dasllama_convert.das`, `dasllama/dasllama_gguf.das`

Every conversion lives in `dasllama_convert.das`; the GGUF container finds bytes and dispatches in
(`ARCHITECTURE_ENGINE_FORMATS.md#formats-and-data-movement`, the two files' charters).

- `KqTag_<fmt>` (`dasllama_kqformat.das`) - the format's empty tag struct beside the others; it
  is the overload key every per-format family resolves on, and `kq_fmt_stamp(fmt) <| $(F)`
  binds it, so a member without an overload fails the compile at the stamp. The tag is the
  LAST parameter of every family: the leaf's data pointers and `n` keep the six x64 register
  argument slots (a tag in slot 0 pushes `n` onto the stack and reshuffles the dot's register
  allocation).
- `kq_transcode_p(src, kq, ks; _f : KqTag_<fmt>)` - the ONE codec of the format: one stride
  unit (the superblock; q51's 32-block) from its disk bytes at `src` into the quant plane at
  `kq` and the scale plane at `ks`, pointer form. The array form the tests drive is the shared
  `kq_transcode_superblock(fmt, bytes, bo, kq, kqo, ks, kso)` (bounds-checked, then the stamp),
  and the bulk loader is the shared `gguf_transcode_kq` (`dasllama_gguf.das`: the descriptor
  row sizes the slice, `kq_transcode_check` + `guard_dst` + `with_tensor_view` +
  `maybe_parallel_for` over units, the stamp hoisted outside the loop) - neither needs an edit.
- `dequant_<fmt>_plane_superblock` - the reference dequant off the planes, in ggml's own float
  order (`dequantize_row_<fmt>` decides the order; match it operation for operation so the
  plane dequant is bit-exact against the file dequant).
- `tests/test_kquant.das`: a hand-packed synthetic superblock (`build_<fmt>_block`, written in
  the PACK direction so a misread cannot cancel; `|=` is not defined on `uint8`, so build each
  byte in an `int` and store it once) and an arm asserting the plane dequant equals the
  hand-computed weights exactly.

**Plane layout rule.** Quant plane = the disk nibbles verbatim when the disk pairing is already
what the kernels want (Q4_0 and IQ4_XS pair k/k+16 inside a 32-block - the k4 tiling; Q4_K
pairs k/k+32 and re-pairs at repack). Scale plane = a fixed-stride row per superblock. The kq
rule keeps the DISK scale block verbatim and decodes at repack (k4/k5: 16B block + 4B pad);
a format whose decoded form FITS the row may decode at transcode instead (IQ4_XS: f16 d, 2 pad,
8 x int8 sub-scale, 8 pad = the k4 decoded-row shape, so k4's repack addressing serves).

## 3. Planes and the loader - `dasllama_common.das`, `dasllama_load.das`, `dasllama_layout.das`

A lattice format's planes live in `Model.kq[int(KqFmt)]` (`KqPlanes`: the quant plane, the scale
plane, the frozen repack interleave), and every plane consumer - the load's cursors and sizing, the
streamed fill, the matmul dispatch, `embed_row`, the repack walkers, the device gathers, the PLE
(per-layer embedding) tripwire, the embed trim, the image walk and the bake identity - indexes that
table through the descriptor row's strides. A new lattice format therefore needs no edit in this
step: its row in `kq_desc` sizes, fills, streams, serializes and dispatches it. What still keys on
the format by hand:

- `dasllama_load.das`: `kq_fmt_of` (GGML type -> tag, with the native-knob gates); the bulk
  transcode call (`transcode_kq_tensor`) is generic over the descriptor row.
- `dasllama_layout.das`: the grouped-row branch of `moe_gpu_gather_stack_kq` - a format whose
  repack is not the uniform four-byte-column form needs its verbatim arm there; and
  `metal_blob_scale_plane`, where a format whose scale row already IS the device form (q40,
  iq4nl) is excluded by name. Bump `PACK_VERSION` with any pack edit: it folds into every image
  identity, so the next load re-bakes instead of mapping a stale `.dlim`.
- `dasllama_image.das`: bump `IMAGE_VERSION` when the plane table's shape or the meta order
  moves; the interleaves serialize in enum order, so an appended member lands last.

A per-32 format is OFF the plane table (`ARCHITECTURE_IMAGE.md#image-kq-table-sb-only`): its planes
are the top-level `Model` pair `<fmt>q` / `<fmt>s` (the `kq_desc` row still sizes them - `qsb`/`ssb`
per 32-block). The format joins `kq_dense_b32` (`dasllama_common.das`, the per-32 predicate) and the
pair accessors - both `kq_plane_pair` overloads (`dasllama_common.das`) and `kq_plane_pair_names`
(`dasllama_gpu_resident.das`); every seat that indexes the per-32 formats then takes it with no arm,
since each walks `kq_dense_b32` and keys its table by `int(KqFmt)`. A streamed region code naming no
plane-pair format panics in `stream_job_transcode`. What still keys on the format by hand:

- `dasllama_common.das`: `Model.<fmt>q`/`<fmt>s` and the served-bytes accounting.
- `dasllama_load.das`: `LayoutSizes.<fmt>_n`, `KqCursors.<fmt>`, the `kq_take` arm, the landing
  memcpy, the plane alloc's format list, `g_stream_plane_total["<fmt>q"]` and `stream_field_of`.
- `dasllama_layout.das`: `metal_blob_off_ok`.
- `dasllama_image.das`: the quant-plane names in `streamed_q_planes`, the scale planes summed in
  `vk_mint_extra_bytes`, and `IMAGE_VERSION`.
- `dasllama_config.das`: `DlimCpuConfig.<fmt>_mr` in the identity string and its arms in
  `dlim_b32_mr` / `set_dlim_b32_mr`.
- `tests/test_model_image.das`: the `ImgTableProbe` pair + slot cell.

The tag rule is `kq_fmt_row_ok` (`dasllama_common.das`; the expert twin `kq_fmt_expert_ok` is the
same rule), one rule for every site through `kq_tag`, and `note_demotion` counts a q8 fall alone,
so the load report reads no demotion for a plane the per-32 twin took. `kq_fmt_of` also tags the
PLE token table, so a per-32 member reaching `kq_fmt_of` brings the PLE gather's arms with it on
every backend (`ple_gather_row` on the CPU, the Metal PLE gather twins, the Vulkan `EMB` leaf the
resident plan seats the table under).

## 4. CPU kernels - `dasllama_math_default.das`, `dasllama_math_gen.das`, `dasllama_math.das`, `dasllama_repack.das`

- **Read the reference build's CPU kernel for the format FIRST** - `ggml-cpu/arch/x86/quants.c`
  `ggml_vec_dot_<fmt>_q8_K` (and the arm twin) in a llama.cpp checkout. Map each technique it
  uses onto the lattice as its own `[tune_perm]` spelling where it fits; the probe IS the
  side-by-side (perms race each other and the reference, per box). Never leave one of their
  techniques unmeasured.
- `dot_kq(kqrow, ksrow, xqp, xsp, xbsp, n; _f : KqTag_<fmt>)` - the portable disk-order row
  dot (exact integer inner sums, one float fold per superblock); the rows walk (`kq_rows_kernel`),
  the `kq_gemv_kernel` and `matmul_kq_groupn` arms and the gen tier's row tails come from the
  stamp. The enum overload `dot_kq(..., fmt : KqFmt)` the gates drive lives in the test fixture
  `tests/_kq_dot.das`, never in `dasllama_math_default`: a stamp there adds sixteen call sites
  to the dots in their own JIT partition, and the inliner's decisions over the leaves follow
  their call-site count. A
  kernel that can run on a forked worker takes its table as a per-call local or a pointer
  argument, never a module global: the fused chains invoke the rows cores inside job contexts,
  where a `let` global reads zero, so a codebook format's rows come out zero on every worker row
  and right on the main-context rows - the GEMV probes (no job queue) pass, the forward is
  garbage. The probe that catches it is any kernel gate run under
  `with_job_que() { setup_dasllama_jobque() ... }` with enough rows to fork.
- `dequant_<fmt>_row_grp` - the grp<mr> row dequant (own helper; `dequant_kq_row_grp` dispatches).
- `repack_grp(kq, ks, n, d, mr; _f : KqTag_<fmt>)` (`dasllama_repack.das`) - disk-order ->
  grp<mr> planes, the format's overload behind the shared `repack_kq_grp(fmt, ...)` router;
  tail rows (d % mr) stay disk-order untouched. Two primitives build every overload: the quant
  plane as `colw`-byte columns (`repack_columns` - the column-form formats are one line,
  `repack_column_form`) and the scale row field-major (`repack_fields` over a `RepackField`
  list: the k4 row, the k6 row, or the format's own order); only a nibble re-pairing (k4/k5,
  k6, k3) writes its quant half by hand. A format with both d and dmin takes a field-major grp
  scale header (`[16 sc x mr][mr x f16 d][mr x f16 dmin]`, k2's field list) so `load_f16_vec_at`
  serves d and dmin as vectors; the repack and both grp readers move in one bit-exact step.
- `dasllama_math_gen.das`: `<fmt>q8_layout_gen` (the layout companion), `<fmt>_grp_row_dot` (the
  scalar grp reference = the stubs' body and the repack oracle) with its `KqTag_<fmt>` overload of
  `grp_row_dot` - `tests/_kq_dot.das` stamps that family over every `KqFmt` member, so a format
  without one fails the compile - `<fmt>q8_gemv_gen` (body `kq_gemv_ref(..., KqTag_<fmt>())`, which
  walks rows through that overload) + `<fmt>q8_tile_gen` with the `[tune_perm]` grid copied from
  q40's and `tune(gen = "dasllama_gemm_gen::<fmt>_tile", ...)`, the format's `KqTag_<fmt>` overloads
  of `kq_gemv_gen` and `kq_tile_gen` (the same stamp binds them), `kq_layout_of` (`repack_kq_gen` /
  `repack_kq_bake` route through `repack_kq_grp` and need no arm), `kq_kernel_gen` (two ladders),
  `kq_batch_cell_gen` (`packed` + tile + tail ladders - a packed format with no arm here
  dereferences a null scale plane inside the k6 tile under the JIT, a panel format with no arm
  silently decodes as k6), `kq_batch_kernel_gen` tail, `kq_batch_groupn_gen` tail, `kq_groupn_gen`
  (two ladders), both `register_kernel_backend` rows (`kq_rows_<fmt> = @@<fmt>q8_gemv_gen`). The
  tile's `tile4` companion (`ARCHITECTURE_CPU_KERNELS.md#amx-bf16-tile`): the stub
  `<fmt>q8_tile4_gen` (body `kq_tile_ref(<id>, ...)`) with its `tune_companion` row (`<fmt>_tile4`),
  the format's `KqTag_<fmt>` overload of `kq_tile4_gen` - a `KqFmt` member without one fails the
  compile - and, for a format whose tile reads the packed planes (`kq_reads_packed_planes`), its arm
  in `kq_ref_row_dot`: a packed format with no arm there reads its group as a byte panel in every
  tile stub's reference body.
- `dasllama_math.das`: the `KernelBackend.kq_rows_<fmt>` slot, its `g_kq_rows_<fmt>` global,
  the activation copy, `kq_rows_fn`, the null-guard, the bake arm in `active_kq_layout_mr`.
- `dasllama_gemm_gen.das`: nothing to register - `register_dasllama_gemm_generators` walks
  `KqFmt` and registers `<fmt>_gemv`, `<fmt>_tile`, `<fmt>_tile4` and `<fmt>_panel` for every
  member with a schema id, each generator reading its format off its own key.
  Until the emitter arm exists, leave the format out of `kq_has_emitter`
  (`dasllama_gemm_schema.das`): every stub of the family then declines on every rail - the gemv,
  tile, tile4 and panel generators AND the shared companions (`perm_declines` reads the stub's
  format off its `<fmt>q8_` name), so the layout stays at the reference interleave the reference
  bodies read - a declined generator IS the framework's fallback, so the stubs are the cheapest
  way to have a real family; the `[tune]` family must exist before the emitter does because the
  loader's kq dispatch, the layout companion and the tuner's completeness check all resolve it
  by name. `test_kquant`'s declined-family cell holds that lockstep: a format outside
  `kq_has_emitter` reads a false witness and layout 4.
- The AMX bf16 leg (`ARCHITECTURE_CPU_KERNELS.md#amx-bf16-tile`): q40's grid carries two
  `amx_bf16` rows, and a family that copies them owes the leg's companions. In
  `dasllama_math_gen.das`: the stubs `<fmt>q8_tileform_gen` (returns 0), `<fmt>q8_amx_cfg_gen`,
  `<fmt>q8_witness_gen` and `<fmt>q8_panel_gen` (body `kq_panel_rows_bf16(<id>, ...)`) with
  their `tune_companion` rows (`kq_tileform`, `kq_amx_cfg`, `kq_witness`, `<fmt>_panel`), and
  the format's `KqTag_<fmt>` overload of `kq_tileform`, `kq_amx_arm` and `kq_panel_gen` - a
  `KqFmt` member without one fails the compile. In `dasllama_gemm_gen.das`: the format's
  arm in `panel_scales`, `panel_quads` and `panel_ioff` (the `<fmt>_panel`
  generator registers with the format). In
  `harness/gen_x64_emission_probe.das` and `gen_x64_emission_check.sh`: the panel registry and
  its gate row. A format whose panel is not written yet leaves the two rows out of its grid.
- `dasllama_math_gen.das`: the format's `kq_registries` overload, returning every registry of
  its family (tile, tile4, gemv by suffix, panel, the tileform and arm companions). The tuner
  walk, the TEST gate and `benchmarks/matmul/kq_kernel_bench.das` read the family from it - a
  format whose tile registry is non-empty is in the TEST gate and the tune rail, and a format
  with no emitter arm passes both at the reference layout (every seat reports `mr=4`, maxdiff
  0), since its companions decline with its tile.
- `harness/gen_tune_probe.das`: `pack_kq_disk_block`, `repack_kq_grp_fmt`, `build_kq_fixture`
  (block size, transcode, the yref oracle), `b32_layout_mrs`; and the
  `<fmt>q8_tile4_gen_variants()` term in `harness/gen_x64_emission_probe.das`, so the emission
  rail dumps the companion. Without these the scope's completeness check demands a sidecar
  entry the tuner never writes, and every start re-tunes.
- `tests/test_kquant.das`: add the format's schema id to `KQ_IDS`, its synthetic block to
  `synth_block` (`tests/_kq_fixtures.das` - a ternary whose `else` reads k6's block, so a
  format missing there gates k6's bytes; `followup_general.md` item 132), and a `plane_row` cell
  to `test_kq_transcode_planes`. The gates walk `KQ_IDS` and reach the format through
  `kq_fmt_of_id` and `tests/_kq_dot.das`'s `kq_fmt_stamp` calls (`dot_kq`, `kq_gemv_rt`,
  `kq_tile_rt`, `kq_grp_row_dot_rt`), so none needs a per-format arm: dot vs the fp64
  plane-dequant oracle, portable GEMV rows, repack at mr 4/8/16 (dots and row dequants
  bit-exact), 4-token tile vs per-token GEMVs, groupn (disk + grp slices), batch groupn.

A per-32 format's CPU kernels are ONE body over the format tag (`dasllama_math_default.das`, the
portable tier): the three expanded-row kernel shapes - `b32_batch_kernel`, `b32_groupn_kernel`,
`b32_batch_groupn_kernel` - take the tag last and reach the format through the `b32_*` overload
set (`b32_qb`/`b32_sb` strides, `b32_layout_mr`, `b32_expand_row`, `b32_expand_grp_row`,
`b32_gather_grp_scales`, `b32_dot_e`), so a new per-32 format writes its leaves and those
one-line overloads, never a kernel shape:

- **The leaves:** `dot_<fmt>q8_scalar` (THE float-order contract - per 32-block an exact int sum,
  one float fold), `expand_<fmt>_row` (the row as int8 bytes the fast kernels MAC over) with the
  `[tuned]` `dot_<fmt>e` twin of `dot_q51e` where its fold is new, and the grp<mr> pair
  `expand_<fmt>_grp_row` / `gather_<fmt>_grp_scales`. A format whose block is a member's block
  under another level writes each leaf as one call into that member's body: iq4nl32 and q4032
  share the codebook bodies through their `lut` argument (`dot_nl32q8_scalar`, `expand_nl32_row`,
  `expand_nl32_grp_row`, `dequant_nl32_plane_block`, `nl32_grp_row_dot`), q51 and q5032 the 5-bit
  bodies through the tag's `b5_level` / `b5_fold` / `b5_bytes` overloads (`dot_b5q8_scalar`,
  `expand_b5_row`, `expand_b5_grp_row`, `dequant_b5_plane_block` in `dasllama_convert.das`,
  `b5_grp_row_dot` in `dasllama_math_gen.das`).
- **Around them:** the three portable stamps `<fmt>q8_batch_kernel` / `<fmt>q8_groupn_kernel` /
  `<fmt>q8_batch_groupn_kernel` and the format's `register_b32_portable` row in
  `dasllama_math_default_register` - the kernels every backend's unset `B32Slots` entry takes;
  the `repack_grp(..., KqTag_<fmt>)` overload (`repack_columns` at colw 4 + `repack_fields` with a
  verbatim scale unit); the panic overloads of `dot_kq` / `kq_gemv_gen` / `kq_tile_gen` on the tag
  and the tag in the tag list of `tests/_kq_dot.das`'s panic `grp_row_dot` (the stamps bind every
  member).
- **The dispatch** is the slot table's and takes no arm: `KernelBackend.b32` and the active
  `g_b32` are indexed by `int(KqFmt)`, read by `matmul_b32q8_*`, `active_b32_layout_mr` and
  `active_repack_b32` (`dasllama_math.das`) and by `mm_b32_pre` / `mm_at_b32_groupn` /
  `mm_b_b32_groupn` (`dasllama_common.das`).
- **The gates** are `tests/test_kquant.das`'s per-32 cells over the tag (`p32_*`: plane unpack,
  dot vs fp64, groupn / batch / batch-groupn bit-match, the grp expand twins at mr 4/8, the GPU
  gather off grp planes) - the format adds its `p32_*` leaf overloads - plus the identity /
  inverse-map cells in `tests/test_repack.das`.
- **The gen-tier family** is q51's shape twice over. In `dasllama_math_gen.das`: the layout
  companion, the grp scalar walk (`<fmt>_grp_row_dot`), the `[tune_perm]` tile with its GEMV and
  layout companions, the format's `B32Slots` row in `gen_b32_slots` and its
  `set_bake_b32_repack` line, and the groupn / batch / batch-groupn slots - the slots are ONE body
  over the tag (`b32g_*`: layout, the tile and GEMV stamps, the scalar tail dot, and whether the
  fold reads the per-32 activation sums), so a per-32 format writes its leaves and those one-line
  overloads. In `dasllama_gemm_gen.das`: a `TileEmit` flag, a block body composed of the shared
  block pieces (iq4nl32's is the mx4 nibble+LUT dots, `emit_lut_nibble_dots`, under the f16 d
  rows `load_d16_rows` and the fold `emit_block_fold`; q5032's is q51's code loader
  `load_b5_codes` re-centred to q - 16, the signed `emit_block_dots` over `load_x_halves`, and the
  same d fold), the `emit_one_block` arm, the `nibblePlane` set in `setup_tile_emit`, and the
  `b32_gemv_gen_impl` / `b32_tile_gen_impl` walks parameterized on the block strides.
- **Then** the registration rows, the probe's per-32 family (`b32_tune_family` over `KqFmt.q51`
  and `KqFmt.iq4nl32` - the fixture width is the expert width the rail exists for), the
  `kq_kernel_bench` variant arms and the class-profile entries (`<fmt>q8_tile_gen`, `dot_<fmt>e`)
  in every `performance/defaults/*.json`.

Gate: `test_kqformat` + `test_kquant` under `-jit` (the stubs decline, so their reference bodies
run either way; every dasLLAMA test run is a `-jit` run). A new `[tune]` family re-tunes every
application on its first start unless the checked-in class profiles cover it, so land the family
in `performance/defaults/<class>.tune-defaults.json` (re-export on the reference boxes) in the
same arc, and never read a first end-to-end wall time as a load-time regression.

## 5. The JIT emitter - `dasllama_gemm_gen.das`

`ARCHITECTURE_ENGINE_FORMATS.md#cpu-kernel-tiers` names the generated GEMM tier's files and the
stamp that pins them. `kq_tile_gen` / `kq_gemv_gen` are one emitter
specialized on the format their key names
(`kq_gen_fmt`) at generation time: the group walk, the slice/loop machinery (`emit_slice`) and
the store epilogue are format-agnostic; only the block body and the primitives `setup_tile_emit`
wires differ. The IQ4_XS arms:

1. A block body, `emit_block_iq4xs`: `emit_block_mx4`'s nibble unpack + `lut_lookup` (`tbl1`
   / `pshufb`, the codebook baked as a constant vector) + `dot_lane` with `abs_w` - the SIGNED
   sign-trick lattice, because LUT weights are int8; `emit_block_kqv2`'s `kq_dot_lane` /
   `kq_dot_mem` run the unsigned-nibble lattice and do not apply - then k4's fold with the min
   term dropped: per block `iacc += sext(sc) * (idot_lo + idot_hi)`, per superblock
   `f += iacc * (d * d8)`. Plane addressing is the repack's: nibbles at
   `wb + ((blk*16 + j*4)*mr + qd*w8)`, `sc` at `sb + 4*mr + blk*mr + r`, `d` at `sb + 2*r`.
   The sub-scale granularity, not the family, decides the accumulator split: per-16 strips
   (k6, k3, k2, the iq2 family) force split lo/hi accumulators and two strip vector loads, a
   per-32 strip format rides the shared single `SExt` load and needs no new fold.
2. `emit_one_block`: `te.kq == 44` routes to the new body ahead of the `te.kq != 0` arm.
3. `setup_tile_emit(te, gc, p, needMx4 = fmt == 44)`: the mx4 primitive wiring is reused as
   is; the LUT bake picks `iq4nl_lut()` over the e2m1 table on `te.kq == 44`.
4. `kq_has_emitter` (`dasllama_gemm_schema.das`) admits 44, so the registered generators and the
   shared companions emit instead of declining.
5. No `perm_declines` change: the tbl1 rail already sits on every sdot perm (it is the mx4
   companion's), and pshufb is implied by the x64 tiers.

After ANY emitter change, `rm -rf .cache/daslang` (or bump `LLVM_JIT_CODEGEN_VERSION`) before
trusting a probe or bench: the registered generators run at codegen time and their bodies do not
fold into the cached DLL's hash, so a cache hit executes the OLD stamps with no signal - the
same numbers across every edit, the `DLL cache hit` line the only tell. A sidecar minted while the
stubs declined pins the family to `"reference"` and the emitter landing invalidates nothing;
re-mint with `-- --tune-only <fmt>q8_tile_gen` on the app (each app roots its own sidecar) or
delete the sidecar (`followup_general.md` item 134).

Gates, in order: `DAS_TUNE_MODE=test bin/Release/daslang.exe -jit harness/gen_tune_probe.das`
(the family must be in BOTH of the probe's lists) gates every perm of the family's grid against
the scalar oracle - a stamped perm reports its layout companion's `mr` (never 4) and a
fast-math-sized maxdiff (1.5e-5), a declined one `mr=4` and maxdiff 0 (that is the reference body
answering, not the emitter); then `test_kquant` under `-jit` (the tile-vs-GEMV gate is bit-exact
by construction); then the end-to-end run.

## 6. Vulkan - `dasllama_vulkan_classes.das`, `dasllama_vulkan_common.das`

The tier reads the CPU planes verbatim (`stack_plane_bytes` -> `arena_block_bytes` ->
`kq_qsb(vk_kq_schema_id(fmt))` x `kq_dev_ssb(fmt)` - 20 B for a decoded row, 8 B for the codebook
formats' two-word row the gather packs), so a format whose plane pair already has the 20 B
decoded scale row needs no upload work - only the id bridge and the kernels. The IQ4_XS arms:

1. `vk_kq_schema_id` (`dasllama_vulkan_common.das`): the `int(KqFmt)` -> kernel-id arm
   (`6 -> 44`). This is the third id space at its Vulkan seam; without the arm the arena plan
   panics on the first iq4xs stack.
2. `class template Iq4xsGemvT : KqGemvLeafT` - `def override blk_decode`: the block decoded once
   into its lo and hi packed quads (the q40 nibble tiling, `wq4[wsb * 8 + blk]`, each nibble word
   decoded through `iq4_word` into SIGNED lanes for `sdot4` - OpSDot, signed x signed, so the
   block-sum trick of q40/k4 does not apply: `fold_reads_bsum` stays false) and the fold's terms
   `(d * sc, 0, d * sc, 0)` with `sc` decoded by `iq4xs_sc` off the two-word device row (`scales_h`
   above d in word 0, the `scales_l` nibbles in word 1); the family's `blk_fold` dots the decoded
   block against each column. The two stamps are `KqGemvIq4xs : Iq4xsGemvT {}` and `KqGemvIq4xsN`
   with `override NCOL = true` - the template's `run` picks the shell; a grid format's template adds
   `override GRID = true` with its literal `GRID_WORDS` / `GRID_OFF` (REVIEW.das holds them to the
   `KQ_GRID_<FMT>` chain). The family's roster `kq_gemv_fmts` (q8 and every `kq_sb` member) admits
   every superblock format, so the N leaf is not optional: `gemv_cls_ensure_n` / `gemv_cls_enc_n`
   are `kq_tile_stamp` stamps over every `KqFmt` member, so a format with no N stamp fails the
   compile; the format joins `KQ_LEAF_FMTS` in `tests/test_vulkan_kernels.das`, which the ncol cell
   holds against `kq_gemv_fmts`, and its N stamp joins the census's blind list (its declared list of
   kernels no census row dispatches) in `tests/test_kernel_coverage.das` naming the regions file
   that carries it (the census's one-region loads never engage the N-row command). A codebook is
   packed into four `uint4` words and selected with a dynamic vector index plus a byte shift
   (`(tbl[q >> 2] >> ((q & 3) * 8)) & 0xFF`): a `fixed_array` local indexed per nibble lowers to
   Function storage the driver spills, and decodes at a third of the speed (`followup_general.md`
   item 136).
3. `KqBatchIq4xs : KqBatchBase` - `w_quad` decodes the staged words through `iq4_word` (k4's staging
   otherwise; the base's `stage_w` walks a thread's chunks and calls it), `stage_ws` fills ONE plane
   with `d * sc`, `blk_fma` is `xscl * ws * idot` (q40's without the `- 8 * bsum`). A class child is
   declared after its parent in the file - the class rail resolves parents in declaration order and
   reports "parent structure not found" otherwise. The word compose the GEMV leaf's `blk_decode` and
   the tile's `w_quad` both need is ONE free function beside `k5_dep` - a k-lattice format's
   `<fmt>_quad` over a `uint4` of packed words, a grid format's `<fmt>_idx4` / `<fmt>_sgn4` pick
   (the four grid words and their sign nibbles; the class gathers them through its own grid slab,
   the leaf through `grid4`) - never a second spelling in the tile.
4. Ladders: nothing to edit - `kq_batch_cls_ensure` / `kq_batch_cls_enc_for` / `gemv_cls_ensure` /
   `gemv_cls_enc` and the `cm2_cls_*` trio are `kq_tile_stamp` stamps over the enum, so the format's
   stamps exist with the right names or the compile fails; a per-32 format joins `kq_tile_stem`'s
   spelling and `CM2_TILE_NONE` (the dense tiles it never serves) in the same change.
5. The decode GEMV's lanes per row (`dasllama/dasllama_vulkan_classes.das`): join
   `gemv_grid_fmt` where the format's decode gathers from a codebook or a grid, and take the
   lanes from `gemv_lanes_per_row`, which every decode site reaches through `gemv_enc`. Which
   side of that rule the format sits on - grid or k-lattice - is decided by the
   `harness/vk_gemv_probe.das <n> <d>` sweep, its three lane splits per format (0 = the whole
   subgroup, 16, 8) over the row lengths the families serve; the rule itself is
   `ARCHITECTURE_GPU_SHARED_GEMM.md#kq-gemv-lanes` (`kq-gemv-lanes`).
6. Tests: `tests/_vkd_oracles.das` `kq_cls_ref` arm (the class-on-CPU oracle - the replay never
   runs the kernel head, so any workgroup state the kernel stages, a grid table among them, is
   filled there or the device compares against zeros), the family cells in
   `tests/test_vulkan_kernels.das`, and - because the codebook pack is new bit-math that a
   class-vs-device compare cannot see (both sides run the same `iq4_word`) - a float dequant
   straight off the plane bytes that the class oracle must match - iq4xs's is
   `lut_gemv_float_oracle`, and the format adds its arm to `kq_gemv_float_oracle` and its id to
   `kq_has_float_oracle` (`tests/_vkd_oracles.das`); the family cell compares only the formats
   `kq_has_float_oracle` names, so a witness left off it never runs.
   The cells fill planes with pseudo-random words, so a scale or strip byte takes values real
   quantized data never produces: the class, the cm2 decode, the float oracle and the f16 oracle
   read every field with the same signedness (the grid formats read strips unsigned on both
   rails), or the fork shows one tier at a time as each side is aligned. A family cell whose
   `enc` ladder lacks the new arm dispatches the else format's pipeline over the new planes and
   reads byte-stable across fix rounds.
7. The one-copy rare rail (`ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-arena`): the CPU reads a streamed
   layer's experts off the tier's pinned device-layout planes through the format's ROW-MAJOR kernel. In
   `dasllama_gemm_gen.das` the format takes an arm of the family body its planes fit - `rm_kq_gen` for a
   superblock format (its codes, sub-scales and the min or offset term; a codebook-grid block's weights come off
   `rm_grid_block`), `rm_b32_gen` for a per-32 one - and a
   `register_llvm_code_generator` row for its `<fmt>_rm_gemv` and `<fmt>_rm_tile4` keys (the row-major perms emit only
   once sec.5's emitter admits the format: `perm_declines` reads `kq_has_emitter` for every family a format's stubs name,
   so a row-major arm landed ahead of the grp-tier arm reads "(ref)" on every perm with no other sign); in
   `dasllama_math_gen.das` the `[tune]` family `<fmt>q8_rm_gemv_gen` (the shared perms, the tile stub
   `<fmt>q8_rm_tile4_gen` and the witness stub `<fmt>q8_rm_witness_gen` ahead of it as its companions, the
   reference `rm_kq_rows_ref` over the device-plane decode `rm_kq_dev_row`, which takes the format's arm, or the
   per-32 family's disk-order dot) and the `rm_gemv` / `rm_witness` rows of the format's `kq_registries`, which
   `gen_tune_probe` reads to gate and race the family
   (`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#rm-family-race`; a format with no rows is never raced, and
   `performance/REVIEW.das` fails every class profile that lacks the family's entry,
   `ARCHITECTURE_MEASUREMENT.md#committed-records-gate`); the stamp in
   every `performance/defaults/*.tune-defaults.json`; the rail's dispatch (`rm_dev_serves`, `rare_rows_core`'s
   device arm, `rm_dev_tile4` in `dasllama_blocks.das`). The host twin (`dasllama_gpu_host_twin.das`, both discrete
   homes': the format's GEMV template stamped with no dispatch home and run on the CPU - the kq family's `blk_contrib`
   through `KqGemvBase`, a per-32 or mx4 format's `blk_words` + `blk_xdot` on its own leaf) is the device layout's test
   oracle: a superblock format adds its `Host*` leaf and joins `host_kq_cls` with the grid fill the oracle's `kq_cls_ref`
   arm makes, a per-32 format a leaf, a pointer on `HostTwin` and an arm of `host_rows`, and `host_twin_serves` admits it. The cells are
   `tests/test_vulkan_host_twin.das`: the format joins its family's list in the twin cell, the row-major cell and
   the tile cell, and the planar stack the shell takes - `synth_kq_region` for a superblock format, the fixture's
   block builder through `kq_transcode_superblock` for a per-32 one - must land the same gather the engine
   makes. `rm_dev_serves` admits the format to the rail's device arm; a format it does not name reads the planar
   planes at the planar mirror's cost in RAM, so the format names itself there with its kernels.

A per-32 format (q51's, mx4's and iq4nl32's shape: 32-weight blocks, Q8_0-form activations, off
the kq lattice) takes none of the kq id bridge. It joins `kq_block32` (`dasllama_kqformat.das`, the
one predicate the drivers, the resident plan and `pf_f16_feed` read) and `arena_block_bytes` (the
CPU plane's own block strides; mx4's scale slab rounds to whole words, since the tiles read its
byte scales four blocks a word; iq4nl32's 2 B d is half a word, so its decodes and its GEMV pick
the half by the block's parity), and stamps the per-32 templates rather than a `KqGemvBase` child:

- the tiles: a `KqCm2BatchT` format template at `BLKW` 32 (`Q51Cm2T` with a word scale plane,
  `Mx4Cm2T` decoding the doubled e2m1 magnitudes under the halved E8M0 scale, `Iq4nl32Cm2T` on the
  `IQLUT` axis), with the s stamp the expert schedule's 32-row pieces dispatch, the l and m stamps
  the window chain's pick takes over a dense plane (`ARCHITECTURE_GPU_VULKAN.md#vk-b32-window-gemm`;
  the m stamp is the e column too - the format joins `KQ_CM2E_ALIASES_M`; an expert-only format such
  as mx4 stamps neither dense column and ships its own e stamp instead - it joins `cm2_s_only`,
  `cm2_cls_ensure` refuses it a dense column, and the plan declines a dense plane of it) and the
  `khr_stage16` method behind the `<Fmt>KhrBatch` stamp the mm-mode schedule takes;
- the decode GEMV: `Blk32GemvRunT` stamps (`Q51Gemv`, `Iq4nl32Gemv`, `Mx4Gemv`), one lane a block,
  `gemv_lanes_per_row` 0; a format whose gate and up stacks share a slab may add the fused
  gate+up+act+requant twin in `KqGemvK4Gu`'s shape (`Mx4GemvGu`).

A census row with no carrier in the stocked set lands in `VK_CENSUS_NEVER_DISPATCHED`. Its stamps
spell `<fmt>_batch_<tail>` the way q8's do, so the format joins `kq_tile_stem`'s per-32 list
(`dasllama_kqformat.das`) and the kq batch rail's per-32 predicate (`blk32_rail`,
`dasllama_vulkan_common.das`); an expert-only format whose dense column is refused also joins the
`CM2_TILE_NONE` pairs of the `cm2_cls_*` ladders (its l and m columns) and the s-column route of
`cm2_tile_tail` (`cm2_s_only`). The module gate (`REVIEW.das`) reads it twice:
every format on the cm2 template ships the KHR trio, and `cm2_dispatch_name` learns that the format
spells its expert stamp `<fmt>_batch_cm2e_cls` the way q8 does.

A grid format adds one more: its table joins the family's grid buffer (`kq_grid_dev` - a
`KQ_GRID_<FMT>` word offset, `KQ_GRID_WORDS` / `KQ_GRID_BYTES` grown, the accessor called
once per word into the host image) and its GEMV template sets `override GRID = true` with the
literal `GRID_WORDS` / `GRID_OFF` twins of that chain (`check_kq_gemv_grid_literals` holds them), so
`KqGemvLeafT`'s `stage_grid` stages `gridb[GRID_OFF + idx]` into its `@workgroup` table ahead of
either shell - never the `*_grid_word` accessor, which the batch and cm2 tiles keep
(`REVIEW_GPU_VULKAN.md`, `ARCHITECTURE_GPU_VULKAN.md#kq-gemv-grid-buffer`).

### 6b. The cm2 prefill tile - a decode method on the template

On an NV_coopmat2 device the f16 feed serves every kq format through ONE tile template
(`KqCm2BatchT`): a new format is a format template authoring `[spirv_decode] def decode` over the
DEVICE forms (quants as the gather lays them out - k4/k5 re-paired k/k+16, q40/iq4xs/k3 verbatim;
scales the `kq_dev_ssb(fmt)` row - 20 B decoded, or the codebook formats' two words) in PAIR form -
every shared read derived from `e & ~1u`, both elements computed, the element selected last
(`ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes`; iq2xxs's `decode` is the model) - plus
its width stamps (the l, m and s columns, and on a 32-step decode the expert schedule's e column -
the m column at the format's k step - on BOTH discrete homes: the Vulkan stamp in `dasllama_vulkan_classes.das` and the
CUDA `Cu<Fmt>Cm2E` stamp in `dasllama_cuda_classes.das`'s `CUD_CM2_TILES` ladder, since `kq_tile_stamp` asks each home
for every 32-step format's e stamp; each names its `BN`, `STILE`, the k step `BK` where it is not
the template's 64 with the unroll `UNR` that keeps the unrolled block at one superblock (`override
UNR = 8u` beside `override BK = 32u`), and the `AT`/`BT`/`ACC`/`ACCW` tile types of that depth -
copy k4's for a K-quant or LUT decode, iq2xxs's 32-deep s and e stamps for a grid-codebook decode,
and settle the k step on a whole-model MoE row, not the uniform probe alone). A 64-step format's e
column is its m stamp byte for byte, so it ships no e stamp and joins `KQ_CM2E_ALIASES_M` in
`dasllama_kqformat.das` instead; the gate holds that roster to every s stamp's k step. The class
ladder `cm2_cls_*` (and the kernel cells' `khr_cls_*`) takes no arm: `kq_tile_stamp` walks every
`KqFmt` member, so the tree fails to compile until every stamp the new member names exists - and it
must, since `pf_f16_feed` admits every `kq_sb` format on a cm2 device the moment the enum member
exists, and a ladder that fell through served the q8 tiles over the new planes - garbage text at
full speed. A codebook format raises the `IQLUT` axis - a gated `@workgroup` f16 table staged ahead
of the tile loop (the reference build's `init_iq_shmem` form); never select codes out of a register
vector per element inside a decode callback. A format whose sub-block scale takes an unpack per
element raises the `SCACHE` axis and reads its sub-block's premultiplied pair from `sc_cache` in the
decode, as k4 and k5 (the five-word K-quant scale row) and iq4xs (`SCIQ4`, its two-word row) do; a
third scale-row shape adds its fill arm to `sc_fill`
(`ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes`), and a scale-caching stamp keeps `BLKW`
at `BK x UNR` so the cache refills once per unrolled block - the module gate holds it on the
engine's stamps and on the probe's twins in `harness/vk_gemm_probe.das` alike. A kq format authors
its own four-wide decode twin: a second `[spirv_decode] def decode_v4` returning `half4` under
`override DECV4 = true`, computing the four consecutive elements in the scalar `decode`'s operation
order and sharing what they share (two 16-bit lanes and one scale extraction for a K-quant, one grid
word and its four sign bits for a grid format). A format may start on the template's `DECVEC` axis,
the twin the emitter synthesizes over `decode` by repeating the scalar body four times. A `[spirv_decode]`
method calls no method of its own class: a scale read factored into a helper fails the lowering with
`invoke is not supported in SPIR-V shaders` on every stamp of the template - inline it in each decode. The probe
decides which twin ships: run `harness/vk_gemm_probe.das -- cm2:<fmt>` (both twin arms in one
process); a format whose four-wide row loses to its scalar row ships a twin that wins or turns both
axes off. Gate: a device-form CPU oracle (`<fmt>f16_gemm_oracle`) and an l/m/s/e cell in
`tests/test_vulkan_kernels.das`.

The KHR instantiation adds one method (`ARCHITECTURE_GPU_SHARED_GEMM.md#khr-mm-kq-tile`, its first
paragraph): on the format template a `def override khr_stage16(blk, e0, sbase : uint) : void`
under `static_if (KHR)` that writes the weight row's 16 values `e0 .. e0 + 16` of block `blk`
into `khr_ao[sbase .. sbase + 8)` as f16 pairs, reading the plane as words - `wq4[...]` (`uint4`,
one or two loads where the run's bytes are 16-byte aligned: the nibble and byte formats) or
`wqw[...]` (`uint`, the byte-granular grid formats) - and the scale words once; the base declares
it `abstract`, so a stamp without one fails to compile. The k4 override is the pattern; the
decode methods' index math is the same, only read sixteen at a time. Then `<Fmt>KhrBatch :
<Fmt>Cm2T` with `override KHR = true`, `override BN = 128u`, the four cm2 typedefs the uncalled
tensor body still names (`AT`, `BT`, `ACC`, `ACCW` - copy k4's), a
`[vk_dispatch(name = "kq_batch_<fmt>_khr_cls", ...)]` (the `khr_cls_ensure/set/enc` ladders in
`dasllama_vulkan_classes.das` pick it up by name - `kq_tile_stamp` walks the enum), and the
format's kernel cell runs its KHR arm (`ARM_KHR`, tile 128) wherever the device has KHR coopmat
at subgroup 32 - on a device with both NV_coopmat2 and KHR coopmat the same run covers the cm2
l/m/s/e tiles and the KHR tile. No new oracle: the `<fmt>f16_gemm_oracle` already holds the KHR
arm, whose f16 accumulation sits inside the cell's 2e-2 relative bar.

## 7. Metal - `dasllama_metal_kernels.das`, `_common`, `_prefill`, `_shapes`, `dasllama_layout.das`

The tier compiles MSL and runs only on a Mac. It is NOT a plane-verbatim consumer like Vulkan: a
Metal-served model is the `metal_blob` flavor, whose kq scale planes are rebaked into device
forms (k4/k5: the 16 B disk block, k6: the split form) and whose CPU never reads a plane again. So
a format on Metal starts with a device-form decision, and every ladder below keys on `KqFmt` with
an `else` that means "k5" or "k6" - without its own arm a new format runs a wrong-layout kernel
silently, which is why the tier's gate (`kq_fmt_gpu_supported`) is closed by default. The IQ4_XS
arms:

1. **The device scale form: the k6 split form.** `metal_blob_scale_plane("iq4xss")`
   (`dasllama_layout.das`) turns the 20 B CPU row into `[nsb x 16 B strips][nsb x f16 d]` -
   the strip is the row's bytes 4..19 (`[8 signed sc][8 pad]`), d is bytes 0..1; 18 B/sb, the
   k6 numbers exactly. That choice makes `kq_scales_of` the k6 arm over `t.iq4xss`
   (`soff = sb0*16`, `doff = plane_sb*16 + sb0*2`), `metal_blob_off_ok` the k6 rule
   (`off % 512`), and every kernel bind the k6 pair (d plane at `s0off = doff`, strips at
   `soff`). `metal_blob_commit` lists the plane by name. Quants: `kq_quants_of` binds
   `t.iq4xsq` verbatim (128 B/sb). The split decides whether `metal_blob_off_ok` needs an arm at
   all: a `[16 B strips][4 B d+dmin tail]` split (Q2_K's) keeps every bind aligned by
   construction and takes none, where a verbatim 20 B row bind would need `off % 1024`.
2. **The codebook in MSL, `iq4_lut`:** four packed words selected by nibble range, byte
   extracted, widened with the k6 sign trick - no table memory. A grid of 2 to 8 KB goes in as
   a program-scope `constant` table: an all-literal `let` fixed-array local hoists there when it
   is `let`, its initializer is `fixed_array<T>(...)` of exactly N elements, and every element
   is an integer or float scalar literal (bool, half and vector literals do not hoist; the
   emitter names each refusal); any other fixed array is a `var` local with per-index stores.
3. **Kernels** (`dasllama_metal_kernels.das`), each a stamp of the family's template (sec.0,
   question 4) with the per-32 pairing (lane = one 32-block of every 4th superblock: 4 uints, 16
   lo + 16 hi weights) and `d * sc * LUT[q]` with no min term: `MetalKqGemvIq4xs` (dense) and
   `MetalMoeGemvIq4xs` (gathered), the decode GEMV stamps of `MetalKqGemvIq4T` on `MetalGemvSiteT`;
   the small-batch stamps `MetalKqMvB2Iq4xs` / `MetalKqMvB4Iq4xs` (of `MetalKqMvIq4T`) and
   `MetalKqMvB8Iq4xs` (of `MetalKqMvB8Iq4T`); and an `IQ4XS` arm in `MetalKqMulMmK45T`
   (prefill mul_mm; `MetalKqMulMmIq4xs` stamps it - the template's `static_if` has no `elif`,
   so a new arm re-nests the k4/k5 arm one level deeper, `followup_general.md` item 135). PSO
   globals + `compile_stamp(<Class>_<kernel>_msl, ok)` lines in the compile ladders and a slot in
   the `release_handles(...)` rosters; a per-format mul_mm stamp declares itself
   `[metal_dispatch(stamp = "<family>:<fmt>:<form>")]` and the lens derives its names; every
   `g_pso_*` global is declared by hand in `dasllama_metal_common.das`, since the lens only
   names it. The prefill's per-format picks are tables, one row per format: `pf_kq_dense_stamps`
   (every dense format's twins), `pf_moe_kq_row` (every superblock format's routed
   forms - the base and the four twins).
4. **Ladders:** `enc_kq_gemv`, `enc_kq_gemm_mm_b` (kernels), `pf_enc_kq_site_mm`, `enc_site_gemv`
   (the classifier site prefill and decode share), `moe_site_ok` + the `sb1/2/3` predicates
   (shapes), and last the gate. The tensor side is one class: a `stage16` override on
   `MetalKqMulMmSplitTensorBase` (`ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-kq-tensor-scaffold`)
   gives the format its T/TH mul_mm twins, its dev-W dequant pass (`MetalKqDequant<Fmt>`, plus the
   bake mirror and the split set in `pf_kq_split_fmt`), and its routed expert twins
   (`MetalMoeMulMm<Fmt>SplitT`, `ARCHITECTURE_GPU_PREFILL_MOE.md`); the decode GEMV derives from
   `MetalGemvSiteT` so the dense and the expert-indexed stamps share one body (`enc_moe_gemv`,
   `moe_fmt_metal_served`). A format whose decode already rides another format's split class stamps
   the four expert leaves (`T`, `TH`, `TH128`, `THR`) off THAT class's MoE template with its own
   template constants instead of authoring a new one - q40 off iq4xs's is the worked case. **The
   small-batch forms are a table row, not a ladder arm:** `KQ_MVB_FORMS` (kernels) carries the
   format's two-, four- and eight-row builders in their f32 stamps and their half-x stamps
   (`MetalKqMvB{2,4,8}<Fmt>H`: the f32 class with `override XHALF = true`, each with a
   `g_pso_kq_mvb*h_<fmt>` global compiled in `metal_decode_init` and released beside the f32 one), a
   `KQ_ROWS_RACE_FORMATS` row runs its `kq_rows_<fmt>` and `kq_rows_half_<fmt>` races, the gemv
   file's `KQ_HALF_FORMATS` list and the coverage census (`test_kernel_coverage.das`) name the
   stamps, and `enc_kq_mvb` asserts on a format with no row - a crowned half route (the half-x form
   a race crowned: recorded in the sidecar as the winner) on a format without its half stamps panics
   at the first dispatch. **Serving an expert plane is one table row and one decode arm**
   (`REVIEW_GPU.md` binds each per-format pick to its served-format predicate): the format's
   `pf_moe_kq_row` row (its simdgroup base `MetalMoeMulMm<Fmt> : MetalMoeMulMmKqT`, a
   template-constant stamp over the dense mul_mm's decode, and its four split twins) feeds every
   prefill pick, plus the `enc_moe_gemv` arm (decode); and `moe_site_ok`'s alignment arm must answer
   for the format exactly where `moe_fmt_metal_served` does - `test_moe_metal_expert_formats` walks
   the whole enum and reds on any disagreement. A compact-scale format (k4, k5, q40, iq4nl:
   `kq_scales_of` returns `doff` 0) sets the row's `d_soff` and binds its d plane at `soff` in the
   decode arm; the split formats bind it at `doff`, and copying their arm reads the scale plane's
   tail as the d row.
5. **Tests:** `dequant_iq4xs_plane_superblock_at` (`dasllama_convert.das`, the split-layout twin the
   CPU row calls) and the format's arm in `dequant_kq_plane_sb` on the BLOB carrier - a compact
   format's arm sits ahead of the `!metal_blob` fork, as q40's does; without it every kernel cell's
   CPU oracle panics `has no kq plane pair` and the cells read as errors, not as kernel misses -
   fixtures at fmt 44 in `tests/_metal_kernel_common.das` (`kq_fill_planes`,
   `kq_row_ref` - a compact format joins the assert's compact list there), then the format's arm in every per-format stamp picker and its calls:
   `kq_gemv_stamp` and `kq_mvb2_stamp` / `kq_mvb4_stamp` / `kq_mvb8_stamp` plus `kq_gemv_gate`'s
   variant-to-id ternary in `test_metal_gemv_kernels.das` (GEMV, B2/B4/B8), `kq_mm_base_stamp` /
   `kq_mm_form_kernel` in `tests/_metal_kq_kernels.das` (mul_mm, base and tensor forms). Each picker
   is an `if` chain whose last arm takes every format it does not name, so a format missing from any
   one of them gets the chain's last arm - another format's kernel under the new tag
   (`followup_general.md` item 132). Gate proof: a one-byte codebook mutation turns the format's
   cells red.

A per-32 expert format on Metal takes q51's shape, not the kq one - no scale rebake, both planes
bound verbatim through `b32_planes_of` (`dasllama_metal_common.das`, the pair off `kq_plane_pair`
and the strides off the descriptor row - no arm; `off % 256` keeps a 16 B quant / 2 B scale pair
16B-aligned, and `metal_blob_off_ok` + `moe_site_ok` say the same number). It brings:

- a format arm of `MetalBlk32GemvT` (8 threads a block, one float4 each; a codebook is staged to a
  `@workgroup` table as the kq iq4 GEMVs do, which makes the dispatch `tgmem =`) stamped dense and
  gathered;
- a per-32 arm of `MetalKqTileDecodeT.kq_decode` stamped as the dense mul_mm and the routed base
  `MetalMoeMulMm<Fmt> : MetalMoeMulMmKqT`;
- a per-32 arm of `MetalKqMulMmB32TensorT.stage16` stamped as the dense `T`/`TH`/`TH128`/`THDb`/
  `THDb128` twins and the dequant pass (`kq_mm:<fmt>:*`, behind the format's `kq_mulmm_<fmt>`
  crown in `pf_compile_kq_b32_tensor_twins`) and as the routed `T`/`TH`/`TH128`/`THR` twins of
  `MetalMoeMulMmB32SplitT` (`moe_mm_split:<fmt>:*`, compiled beside q51's in
  `pf_compile_moe_b32_twins`);
- the rows `pf_kq_b32_stamps` (the dense ladder's) and `pf_moe_kq_row` (the routed one's, no
  `enc_base`: the per-32 base binds the pair without a strip view, through `pf_enc_moe_mm_b32`);
- the races: two `race_stems` rows (`kq_mulmm_<fmt>`, `moe_mulmm_<fmt>`) in `race_kq_mulmm_b32`
  and `race_moe_mulmm_formats`;
- the pick `b32_twin_fits` (the tensor twins walk 64-deep K, so a `kdim % 64` stack takes the base
  form);
- its arms in `moe_fmt_metal_served`, `moe_site_ok`, `kq_dense_b32` (the per-32 reduction dims in
  `moe_metal_ok`, the dense sites' plane pair) and `kq_fmt_gpu_supported`.

These forms, races and the pass are the format's row of the form matrix (step 0, question 4). A
GEMV stamp that declares `fastmath=false` (the per-32 GEMV and its K-split twin do: fast math sums
a column's dot apart from the small-batch form) gets its `FASTMATH_OFF_EXEMPT` row in `REVIEW.das`
against `b32_mvb_gate`, and its name on the licensed set's line in
`ARCHITECTURE_ENGINE.md#engine-core` - without the row the module gate reds on the first stamp.

Tests:

- in `tests/test_metal_prefill_kernels.das`: `<fmt>_test_planes` (exact arithmetic - pow2 d,
  int x) with its arm in `moe_gemv_planes`, the format's arm in each stamp picker that spells
  the generated names (`b32_gemv_stamp`, `b32_mvb_stamp`, `b32_moe_gemv_stamp`,
  `b32_mm_kernels`), and the `moe_gemv_gate` / `moe_mulmm_b32_gate` rows;
- the four twin rows in the census's declared blind list (`CENSUS_NEVER_DISPATCHED`,
  `tests/test_kernel_coverage.das`);
- the served list of `test_moe_metal_expert_formats` (`tests/test_gpu_serving_declines.das`);
- the format's term in the census lists - `per32_planes` and `PER32_STEMS`
  (`tests/test_metal_format_census.das`) and `per32_planes` (`tests/test_metal_decode_parity.das`),
  which count and name the per-32 sites the census holds the dispatch counts to.

A Metal kernel lever is judged by a kernel-level probe, never by a tg128 row: an end-to-end row
carries noise above the lever's size, while the dispatch-loop probe (50 dispatches per encoder,
best of three encoders, GB/s off plane bytes) resolves 3% in seconds. On the Metal tier the app
sets `set_metal_mode(MetalMode.required)` BEFORE `load_model_`, then `convert_model_to_metal_blob`;
`run.das` itself declares CPU intent, and `DASLLAMA_GPU=1` on a Mac build with dasVulkan arms
Vulkan on MoltenVK, not Metal.

## 8. End to end

Every correctness-loop run (probe tests, parity, `run.das` spot checks) is the fast stack by
default - `daslang.exe -jit <app>.das ...` installs the AST module cache and the split-module JIT
unasked. A bench that must see one-unit codegen passes `--jit-split-modules=0` after the `--`
separator; a records row measures a released `-exe` and names its codegen form; `-no-module-cache`
opts out of the cache. The flags: `skills/internal/build_and_debug.md`. `run.das` takes the model
as the LAST argv token ending in `.gguf`: a flag placed after the path silently falls back to the
default model, so the run benches a different file.

A kernel spelling is raced without a model in `benchmarks/matmul/kq_kernel_bench.das` -
`DAS_TUNE_MODE=tune bin/Release/daslang.exe -jit
modules/dasLLAMA/benchmarks/matmul/kq_kernel_bench.das -- --fmt <fmt> --perm <substr>` times every
row of the tile's and gemv's `_variants()` registries at one thread on synthetic planes (seconds
per try), and the reference row at the same shape is the reference exe's `test-backend-ops perf
-o MUL_MAT -p "type_a=<type>,type_b=f32,m=4096,n=1,"` under `GGML_BENCH_THREADS=1`;
`harness/kernel_ladder.sh` runs both sides for every format and prints the box's ratio table. The
bench's fixture conditions are `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#cpu-kernel-bench-fixture`.
The app run comes only after a spelling wins there.

Pick the vehicle by census, not by name: an ftype is a MIX, and a natural download can put an
unsupported sibling on `attn_k`/`attn_q` so the file cannot load end to end. `harness/gguf_dump.das`
reads the tensor type table in seconds; pick a file whose mix is the supported set plus the new
format, or mint one from a Q8_0 with `llama-quantize --allow-requantize --imatrix <imatrix>
--tensor-type attn_k=<fmt> --tensor-type attn_q=<fmt> <Q8_0> <out> <FTYPE>`. A vehicle the resident
driver declines proves the kernels and not the tier; `resident_decline_reason` prints the reason.
Then `examples/dasLLAMA/run.das` against the reference build's `simple_ids` oracle for the same
prompt: the criterion is the margin form (`harness/oracle/simple_ids_margin.cpp` prints per-step
logit margins), not id equality - a fork whose top-2 IS our token at a sub-logit margin is the
expected class, and a stamped fold landing moves the flip point without changing it. When a walk
touches any region or stream ladder, run a MIXED vehicle (the new format beside a K-quant), not a
pure one: a region keyed on the wrong id space corrupts only the loaded model, and every serial gate
still passes. Then `test_model_image` for the image round trip, then the records rig (`BRINGUP.md`).

**Each site x backend pair the format serves has a served cell.** The pairs are step 0,
question 5's list: every pair whose row length the block length divides and that the load does
not decline. For each, a cell loads a file whose plane at that site carries the format and makes
the assertions `REVIEW_KQ_FORMATS.md`'s proving rule names.
On the Vulkan whole-model driver the cell also runs `served_every_call`
(`tests/_resident_feed.das`), which asserts no call took a CPU pass. Where no catalogued file puts
the format at the site (an expert stack, a token table), mint one: a `-local` recipe row in
`performance/model_specs.das` (`llama-quantize --allow-requantize --tensor-type <site>=<fmt>`,
or `--token-embedding-type <fmt>` for the table), minted on each box from its own download,
never copied between boxes.

A site the format does not serve on a GPU backend is declined at load by that backend's
mechanism; the CPU declines nothing:

Every backend's decline names the site (`kq_site_words`, `dasllama_kqformat.das`) and the
`KqFmt` member in the line the user reads:

- CPU: no decline - the CPU serves every member at every site whose row length the block length
  divides through step 4's kernels; a q8-rail demotion on such a row is a converted copy and a
  defect, never a decline; the load report's row (`conv_row_name`, `dasllama_load.das`) names the
  site beside the disk type and the served form;
- Vulkan: the whole-model driver's `resident_decline_reason` (`dasllama_gpu_resident.das`)
  returns the reason before a byte uploads - a layer's or shared expert's plane format by layer
  through `resident_layer_decline`, the tied classifier through `resident_cls_decline`;
- Metal: `decode_shape_decline` (`dasllama_metal_shapes.das`) answers
  `MetalDecodeDecline.kquant_native` for a dense or table plane whose format
  `kq_fmt_gpu_supported` refuses (`kq_load_gpu_unserved` names it), and `moe_fmt_metal_served`
  keeps an expert stack off the device under `graph` (`moe_fmts_unserved`), as the shared
  expert's and the deltanet planes' one-row GEMVs do theirs (`shexp_fmts_unserved`,
  `dn_fmts_unserved`); the drivers log `metal_fmt_decline_line` once and hand its words to the
  decline, so the required-mode panic names the same site and member.

The kernel census holds the dispatch counts the cells assert, so a new stamp joins its
backend's list in the same change:

- a Metal MSL kernel or a Vulkan kernel class: `tests/test_kernel_coverage.das`, a zoo row or its
  blind list (`CENSUS_NEVER_DISPATCHED` for Metal, `VK_CENSUS_NEVER_DISPATCHED` for Vulkan);
- a Metal site's per-format count: `tests/test_metal_format_census.das`;
- a Vulkan whole-model driver site: `tests/test_gpu_resident_format_census.das`.
