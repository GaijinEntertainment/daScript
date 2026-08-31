# How to add a weight format to dasLLAMA

A HOW_TO is the fourth document mood: `REVIEW.md` binds a diff, `ARCHITECTURE.md` says how the
system is, `/history` says what happened - this says **do this, then this**. It walks the code in
the order a new format lands, cites the architecture section that owns each step instead of
restating it, and keeps a QUIRKS ledger of every step where the pattern broke. It is validated by
execution: every step below was run in the session that wrote it, on the format named in the
per-format notes, or is marked untested. A step that turns out wrong during the next format's
bring-up is fixed here in that session.

The worked example is IQ4_XS (the pilot). The per-format notes at the end say what each later
format changed in the walk.

## 0. Decide the format's shape before touching code

Three questions decide which existing family the format rides; answer them from
`ggml-common.h` (the block struct) and `ggml-quants.c` (`dequantize_row_<fmt>`):

1. **Block geometry.** 256-weight superblock (every K-quant, every i-quant) or 32-weight block
   (`Q4_0`, `Q5_1`, `IQ4_NL`)? A superblock format joins the `kq_sb` lattice (Q8_K activations,
   `% 256` rows, the grp<mr> repack, the stamped kq kernels); a 32-block format rides per-32
   planes like `q51`. `ARCHITECTURE_ENGINE.md` sec.1.2 owns the lattice split.
2. **Weight reconstruction.** Shift/mask nibbles (`q4_0`, K-quants), a 16-entry codebook on the
   nibble (`IQ4_XS`, `IQ4_NL`), or a grid gather (`IQ2_*`, `IQ3_*`)? Shift/mask and codebook
   formats keep the k4 nibble tiling and add an unpack step; grid formats decode to bytes first
   (the byte-expanded panel k5/k6 already use).
3. **Scale structure and its fold.** Per-block f16 (`q40`), superblock d/dmin + 6-bit sub-scale
   and min (`k4`/`k5`), superblock d + signed int8 sub-scale (`k6`, `IQ4_XS`). The fold decides
   whether the dot needs the activation block sums (`xbsp`): a min or an unsigned-nibble offset
   needs them, a signed reconstruction (codebook, k6's -32) does not.

Write the answers down; they are the first lines of the per-format note.

## 1. Identity - `dasllama/dasllama_kqformat.das`

The taxonomy every other file keys off. One edit here, then the compiler finds the ladders.

- Append the member to `KqFmt` - **append, never reorder**: the int value is the device stack
  tag (`vk_kq_schema_id`) and the image plane id.
- `kq_sb` (both overloads) if it is a superblock format.
- `kq_schema_id`: the kernel/IR id. The ids are mnemonics - K-quants by bit width (4/5/6),
  Q4_0 = 40, i-quants = bit width x 10 + a variant digit (IQ4_XS = 44). Three id spaces exist
  (QUIRK 5); this is the only bridge between two of them.
- The stride constants `<FMT>_QSB` / `<FMT>_SSB` (bytes per superblock row of the quant and
  scale planes) and the `kq_qsb` / `kq_ssb` arms. A codebook goes here too (`IQ4NL_LUT`).
- `dasllama_gemm_schema.das`: the int-id twins `kq_qsb(int)` / `kq_ssb(int)`.
- `tests/test_kqformat.das`: pin the enum value, the predicate, the strides, the id, the stream
  code, and the codebook's edge values.

## 2. Codec - `dasllama/dasllama_convert.das`, `dasllama/dasllama_gguf.das`

- `GGML_TYPE_<FMT>` constant (`dasllama_gguf.das`).
- `transcode_<fmt>_superblock(bytes, bo, kq, kqo, ks, kso)` - the per-superblock disk -> plane
  split, array form (what the tests drive).
- `dequant_<fmt>_plane_superblock` - the reference dequant off the planes, in ggml's own float
  order (`dequantize_row_<fmt>` decides the order; match it operation for operation so the
  plane dequant is bit-exact against the file dequant).
- `gguf_transcode_<fmt>` (`dasllama_gguf.das`) - the bulk, threaded, pointerized twin of the
  superblock transcode (`kq_transcode_check` + `guard_dst` + `with_tensor_view` +
  `maybe_parallel_for` over superblocks).
- `tests/test_kquant.das`: a hand-packed synthetic superblock (`build_<fmt>_block`, written in
  the PACK direction so a misread cannot cancel) and an arm asserting the plane dequant equals
  the hand-computed weights exactly.

**Plane layout rule.** Quant plane = the disk nibbles verbatim when the disk pairing is already
what the kernels want (Q4_0 and IQ4_XS pair k/k+16 inside a 32-block - the k4 tiling; Q4_K
pairs k/k+32 and re-pairs at repack). Scale plane = a fixed-stride row per superblock. The kq
v2 rule keeps the DISK scale block verbatim and decodes at repack (k4/k5: 16B block + 4B pad);
a format whose decoded form FITS the row may decode at transcode instead (IQ4_XS: f16 d, 2 pad,
8 x int8 sub-scale, 8 pad = the k4 decoded-row shape, so k4's repack addressing serves).

## 3. Planes and the loader - `dasllama_common.das`, `dasllama_load.das`, `dasllama_layout.das`

This is the ladder walk (QUIRK 1). Every site is a flat `if (fmt == KqFmt.k4) ... elif` chain;
add the arm next to `q40`'s. The compiler does not find these - a missing arm falls to the
`else` panic (good) or silently to k6 (bad, the `kq_kernel_gen` shape); grep `KqFmt.q40` and
`== 40` and visit every hit.

- `Model`: the plane pair `<fmt>q` / `<fmt>s` and `kq_repack_mr<id>`.
- `dasllama_load.das`: `LayoutSizes.<fmt>_n`, `KqCursors.<fmt>`, `kq_take`, the `LayoutSizes`
  constructor, `stream_field_of` (the streamed image plane name), the scale-half landing
  `memcpy`, the stream repack `invoke(g_stream_repack, <id>, ...)`, `transcode_kq_tensor`,
  `load_big`, `kq_fmt_of` (GGML type -> tag), the two `noisy` log lines,
  `g_stream_plane_total["<fmt>q"]`, the plane `reserve`/`resize` block.
- `dasllama_common.das`: `kq_active_mr`, `kq_fi`, `mm_at_kq_pre` (two arms), `mm_b_kq`,
  `mm_at_kq_groupn` (two arms), `mm_b_kq_groupn`, `mm_b_kq_pre`, `kq_plane_q`, `kq_plane_s`,
  `embed_row` (four arms: grp and plane form, trimmed and untrimmed), the bake config fill
  (`c.kq_mr<id> = active_kq_layout_mr(<id>)`).
- `dasllama_layout.das`: the plane base pointers + the `rkq` ternary, `push_repack_kq`, the
  `moe_gpu_gather_stack_kq` plane ternaries.
- `dasllama_gpu_resident.das` (embed trim), `dasllama_ple.das` (two arms),
  `dasllama_blocks.das` (`kq_bytes_per_weight`).
- `dasllama_config.das`: `DlimCpuConfig.kq_mr<id>` AND the identity string in
  `dlim_identity` (QUIRK 3); `dasllama_image.das`: `IMAGE_VERSION` bump + the streamed plane
  name list in `stream_extra_bytes`.

## 4. CPU kernels - `dasllama_math_default.das`, `dasllama_math_gen.das`, `dasllama_math.das`, `dasllama_repack.das`

- **Read llama.cpp's CPU kernel for the format FIRST** - `ggml-cpu/arch/x86/quants.c`
  `ggml_vec_dot_<fmt>_q8_K` (and the arm twin) in the local clone. Map each technique it uses
  onto the lattice as its own `[tune_perm]` spelling where it fits; the probe IS the side-by-side
  (perms race each other and the reference, per box). Our CPU kernels typically win - keep it
  that way by never leaving one of their tricks unmeasured.
- `dot_<fmt>_q8` - the portable disk-order row dot (exact integer inner sums, one float fold per
  superblock), `<fmt>_rows_kernel`, the `kq_gemv_kernel` arm, the `matmul_kq_groupn` arm.
- `dequant_<fmt>_row_grp` - the grp<mr> row dequant (own helper; `dequant_kq_row_grp` dispatches).
- `repack_<fmt>_grp` (`dasllama_repack.das`) - disk-order -> grp<mr> planes; tail rows (d % mr)
  stay disk-order untouched.
- `dasllama_math_gen.das`: `<fmt>q8_layout_gen` (the layout companion), `<fmt>_grp_row_dot`
  (the scalar grp reference = the stubs' body and the repack oracle), `<fmt>q8_gemv_gen` +
  `<fmt>q8_tile_gen` with the `[tune_perm]` grid copied from q40's and
  `tune(gen = "dasllama_gemm_gen::<fmt>_tile", ...)`, `kq_layout_of`, `repack_kq_gen` /
  `repack_kq_bake`, `kq_kernel_gen` (two ladders), `kq_batch_cell_gen` (`packed` + tile +
  tail ladders), `kq_batch_kernel_gen` tail, `kq_batch_groupn_gen` tail, `kq_groupn_gen` (two
  ladders), both `register_kernel_backend` rows (`kq_rows_<fmt> = @@<fmt>q8_gemv_gen`).
- `dasllama_math.das`: the `KernelBackend.kq_rows_<fmt>` slot, its `g_kq_rows_<fmt>` global,
  the activation copy, `kq_rows_fn`, the null-guard, the bake arm in `active_kq_layout_mr`.
- `dasllama_gemm_gen.das`: `register_llvm_code_generator` for `<fmt>_gemv` and `<fmt>_tile`.
  Until the emitter arm exists, register generators that return `false` - a declined
  generator IS the framework's fallback (QUIRK 4); the reference bodies serve.
- `harness/gen_tune_probe.das`: `pack_kq_disk_block`, `repack_kq_grp_fmt`, `build_kq_fixture`
  (block size, transcode, the yref oracle), `kq_tile_variants`, `kq_gemv_variants_by_suffix`,
  `kq_layout_mrs`, `run_kq_tile`'s `packed`, `kq_tile_entry`, the family list. Without this
  the scope's completeness check demands a sidecar entry the tuner never writes, and every
  start re-tunes.
- `tests/test_kquant.das`: add the format to every gate's ladder and to the `[4, 5, 6, 40]`
  lists (QUIRK 2). The gates: dot vs the fp64 plane-dequant oracle, portable GEMV rows,
  repack at mr 4/8/16 (dots and row dequants bit-exact), 4-token tile vs per-token GEMVs,
  groupn (disk + grp slices), batch groupn.

Gate: `test_kqformat` + `test_kquant` on the interpreter binary (the stubs' reference bodies).

## 5. The JIT emitter - `dasllama_gemm_gen.das`

`kq_tile_gen_impl(gc, fmt)` / `kq_gemv_gen_impl(gc, fmt)` are one emitter specialized on `fmt`
at generation time: the group walk, the slice/loop machinery (`emit_slice`) and the store
epilogue are format-agnostic; only the block body and the primitives `setup_tile_emit` wires
differ. IQ4_XS took:

1. A block body, `emit_block_iq4xs`: `emit_block_mx4`'s nibble unpack + `lut_lookup` (`tbl1`
   / `pshufb`, the codebook baked as a constant vector) + `dot_lane` with `abs_w` - the SIGNED
   sign-trick lattice, because LUT weights are int8; `emit_block_kqv2`'s `kq_dot_lane` /
   `kq_dot_mem` run the unsigned-nibble lattice and do not apply - then k4's fold with the min
   term dropped: per block `iacc += sext(sc) * (idot_lo + idot_hi)`, per superblock
   `f += iacc * (d * d8)`. Plane addressing is the repack's: nibbles at
   `wb + ((blk*16 + j*4)*mr + qd*w8)`, `sc` at `sb + 4*mr + blk*mr + r`, `d` at `sb + 2*r`.
2. `emit_one_block`: `te.kq == 44` routes to the new body ahead of the `te.kq != 0` arm.
3. `setup_tile_emit(te, gc, p, needMx4 = fmt == 44)`: the mx4 primitive wiring is reused as
   is; the LUT bake picks `iq4nl_lut()` over the e2m1 table on `te.kq == 44`.
4. The two stubs become `=> kq_gemv_gen_impl(gc, 44)` / `kq_tile_gen_impl(gc, 44)`.
5. No `perm_declines` change: the tbl1 rail already sits on every sdot perm (it is the mx4
   companion's), and pshufb is implied by the x64 tiers.

Gates, in order: `DAS_TUNE_MODE=test bin/Release/daslang.exe -jit harness/gen_tune_probe.das`
(the family must be in BOTH of the probe's lists - QUIRK 2) gates every perm of the family's
grid against the scalar oracle - a stamped perm reports its layout companion's `mr` (8 on this
box) and a fast-math-sized maxdiff (1.5e-5), a declined one `mr=4` and maxdiff 0 (that is the
reference body answering, not the emitter); then `test_kquant` under `-jit` (the tile-vs-GEMV
gate is bit-exact by construction); then the end-to-end run - and read QUIRK 11 before
trusting its numbers.

## 6. Vulkan - `dasllama_vulkan_classes.das`, `dasllama_vulkan_common.das`

The tier reads the CPU planes verbatim (`stack_plane_bytes` -> `arena_block_bytes` ->
`kq_qsb(vk_kq_schema_id(fmt))` x `KQ_DEV_SSB`), so a format whose plane pair already has the
20 B decoded scale row needs no upload work - only the id bridge and the kernels. IQ4_XS took:

1. `vk_kq_schema_id` (`dasllama_vulkan_common.das`): the `int(KqFmt)` -> kernel-id arm
   (`6 -> 44`). This is the third id space of QUIRK 5 at its Vulkan seam; without the arm the
   arena plan panics on the first iq4xs stack.
2. `KqGemvIq4xs : KqGemvBase` - `def override blk_contrib`: the q40 nibble tiling
   (`wq4[wsb * 8 + blk]`), each nibble word decoded through `iq4_word` (a `fixed_array` LUT
   local - the SPIR-V emitter lowers a `let` fixed array to a Function-storage variable and
   indexes it) into SIGNED lanes for `sdot4` (OpSDot, signed x signed - the block-sum trick of
   q40/k4 does not apply and is not needed), scale `d * sc` with `sc` the signed byte off word
   1..2 of the 5-word row (`unpack8` sign-extends, the k6 spelling).
3. `KqBatchIq4xs : KqBatchBase` - `stage_w` decodes the staged words through `iq4_word`
   (k4's staging otherwise), `stage_ws` fills ONE plane with `d * sc`, `blk_fma` is
   `xscl * ws * idot` (q40's without the `- 8 * bsum`).
4. Ladders: `kq_batch_cls_ensure` / `kq_batch_cls_enc_for` / `gemv_cls_ensure` /
   `gemv_cls_enc` gain an arm; `gemv_cls_set`'s four-way `||` became `kq_sb(fmt)`.
5. Tests: `tests/_vkd_oracles.das` `kq_cls_ref` arm (the class-on-CPU oracle), the two family
   cells in `tests/test_vulkan_kernels.das` go to five formats, and - because the codebook pack
   is new bit-math that a class-vs-device compare cannot see (both sides run the same
   `iq4_word`) - `iq4xs_gemv_float_oracle`, a float dequant straight off the plane bytes that
   the class oracle must match.

Not done, by ruling: a cm2 decode-in-load tile (`[spirv_decode] def decode_iq4xs`). The f16 feed
admits q8/k4/k6 only (`pf_f16_feed`), q40 and k5 have no cm2 tile either, and
`followup_vulkan.md` item 24 rules that new formats land on the one class template, not as
three more hand-stamped bodies. IQ4_XS prefill rides the kq batch tile like q40 does.

### 6b. The cm2 prefill tile - a decode method on the template

On an NV_coopmat2 device the f16 feed serves every kq format through ONE tile template
(`KqCm2BatchT`): a new format is a format template authoring `[spirv_decode] def decode` over
the DEVICE forms (quants as the gather lays them out - k4/k5 re-paired k/k+16, q40/iq4xs/k3
verbatim; scales the 20 B `KQ_DEV_SSB` row) plus three eight-line width stamps, arms in the
`cm2_cls_ensure/set/enc` ladders, and `pf_f16_feed` admits it via `kq_sb` automatically. A
codebook format raises the `IQLUT` axis - a gated `@workgroup` f16 table staged ahead of the
tile loop (llama.cpp's `init_iq_shmem` form); never select codes out of a register vector per
element inside a decode callback. Gate: a device-form CPU oracle (`<fmt>f16_gemm_oracle`) and
an l/m/s cell in `tests/test_vulkan_kernels.das`. Payoff on the 1B: iq4xs pp512 5161 -> 15334,
k3 5174 -> 14031 (0.90x / 0.80x llama.cpp's Vulkan, from 0.30x).

## 7. Metal - `dasllama_metal_kernels.das`, `_common`, `_prefill`, `_shapes`, `dasllama_layout.das`

Done for IQ4_XS over ssh on the M1 (the tier compiles MSL and runs only there). The tier is
NOT a plane-verbatim consumer like Vulkan: a Metal-served model is the `metal_blob` flavor,
whose kq scale planes are rebaked into device forms (k4/k5: the 16 B disk block, k6: the split
form) and whose CPU never reads a plane again. So a format on Metal starts with a device-form
decision, and every ladder below keys on `KqFmt` with an `else` that means "k5" or "k6" -
without its own arm a new format runs a wrong-layout kernel silently, which is why the tier's
gate (`kq_fmt_gpu_supported`) is closed by default. IQ4_XS took:

1. **The device scale form: the k6 split form.** `metal_blob_scale_plane("iq4xss")`
   (`dasllama_layout.das`) turns the 20 B CPU row into `[nsb x 16 B strips][nsb x f16 d]` -
   the strip is the row's bytes 4..19 (`[8 signed sc][8 pad]`), d is bytes 0..1; 18 B/sb, the
   k6 numbers exactly. That choice makes `kq_scales_of` the k6 arm over `t.iq4xss`
   (`soff = sb0*16`, `doff = plane_sb*16 + sb0*2`), `metal_blob_off_ok` the k6 rule
   (`off % 512`), and every kernel bind the k6 pair (d plane at `s0off = doff`, strips at
   `soff`). `metal_blob_commit` lists the plane by name. Quants: `kq_quants_of` binds
   `t.iq4xsq` verbatim (128 B/sb).
2. **The codebook in MSL, `iq4_lut`:** four packed words selected by nibble range, byte
   extracted, widened with the k6 sign trick - no table memory, no fixed-array literal (the MSL
   emitter takes fixed arrays as `var` locals with per-index stores, not as initializers).
3. **Kernels** (`dasllama_metal_kernels.das`), each a copy of the k4/k6 shape with the per-32
   pairing (lane = one 32-block of every 4th superblock: 4 uints, 16 lo + 16 hi weights) and
   `d * sc * LUT[q]` with no min term: `MetalKqGemvIq4xs` (decode GEMV), `MetalKqMvIq4xsT`
   B2/B4 + `MetalKqMvB8Iq4xs` (small batch), and an `IQ4XS` arm in `MetalKqMulMmK45T`
   (prefill mul_mm; `MetalKqMulMmIq4xs` stamps it). PSO globals + `compile_pso` +
   `release_pso` lines in the three ladders.
4. **Ladders:** `enc_kq_gemv`, `enc_kq_mvb`, `enc_kq_gemm_mm_b` (kernels), `pf_enc_kq_site_mm`
   (the base mul_mm only - no tensor / tall / dev-W twins: those are the M5 kernel pass),
   `pf_devw_panel_kq` (returns false - its `dq` pick would otherwise be k5's), `pf_enc_kq_gemv`
   (prefill), `moe_site_ok` + the `sb1/2/3` predicates (shapes), and last the gate.
5. **Tests:** `dequant_iq4xs_plane_superblock_at` (`dasllama_convert.das`, the split-layout
   twin the CPU row now calls), fixtures at fmt 44 in `tests/_metal_kernel_common.das`, the
   ladders + calls in `test_metal_gemv_kernels.das` (GEMV, B2/B4/B8) and
   `test_metal_gemm_kernels.das` (mul_mm, base form). Gate proof: a one-byte codebook mutation
   turns the iq4xs cells red.

Result: `test_metal_gemv_kernels` 2/2 and `test_metal_gemm_kernels` 2/2 on the M1 Max; the
tuner's first mint on that box crowned `iq4xsq8_tile_gen: mr8` (the NEON `tbl1` path of the
JIT emitter, `verdict=beats`), and `run.das` on the CPU/NEON tier reproduces the text at gen 131 t/s;
on the Metal tier (`set_metal_mode(MetalMode.required)` BEFORE `load_model_`, then
`convert_model_to_metal_blob` - `run.das` itself declares CPU intent, and `DASLLAMA_GPU=1` on a
Mac build with dasVulkan arms Vulkan-on-MoltenVK, 96 t/s, not Metal) the 1B IQ4_XS decodes the
same text at gen 153 t/s with `metal decode/prefill: resident path live on Apple M1 Max`. Not done: the tensor / tall / dev-W mul_mm twins and
the MoE GEMV/mul_mm trio for the format - ledgered for the M5 kernel pass.

## 8. End to end

A real file whose every tensor type is now loadable (the header census script in the session
scratchpad, or `harness/gguf_dump.das`), through `examples/dasLLAMA/run.das` against
`simple_ids.exe` from the llama.cpp reference build for the same prompt; then `test_model_image`
for the image round trip, then the records rig.

## QUIRKS

Every step where adding a format was not "the template plus one condition". Each entry: what,
where, why it is so today, what unquirked looks like. An empty ledger is a legitimate outcome.

1. **Per-format plane fields and ~40 hand ladders.** `Model` holds `k4q/k4s, k5q/k5s, k6q/k6s,
   q40q/q40s, q51q/q51s, iq4xsq/iq4xss`, and every consumer selects the pair with an
   `if (fmt == KqFmt.k4) ... elif` chain: 12 ladders in `dasllama_load.das`, 13 in
   `dasllama_common.das`, 3 in `dasllama_layout.das`, 8 in `dasllama_math_gen.das`, 3 in
   `dasllama_math_default.das`, 4 in `dasllama_math.das`, one each in `ple`, `gpu_resident`,
   `blocks`, `config`, `image`. Why: the planes grew one format at a time and each arm carries
   a different literal stride. Unquirked: one `KqPlanes` (quant, scale, mr) indexed by `KqFmt`
   on `Model`, and `kq_plane_q/s` as the only accessors - every ladder above collapses to one
   table lookup, and a new format is the enum member plus its strides.
2. **The test file ladders per format too.** `tests/test_kquant.das` builds fixtures, transcodes,
   dequants, dots, repacks and calls the stubs through the same `fmt == 4/5/6/40` chains in
   five gates (28 arms for one format), and raises `_cyclomatic_complexity` /
   `_function_length` per format added. `harness/gen_tune_probe.das` repeats the shape (9
   arms), and its test mode gates a hand list of families (the `kq_test_family(4l/5l/6l/40l)`
   calls plus their fixture arrays) SEPARATE from the tune-mode family array - IQ4_XS sat in
   the tune list and not the test list, so `GEN TUNE TEST OK` said nothing about it until the
   call was added. Unquirked: per-format dispatch helpers in one `_kq_fixtures.das` shared by
   the test and the harness (`kq_transcode_sb`, `kq_dequant_sb`, `kq_dot`, `kq_repack`,
   `kq_gemv_gen`, `kq_tile_gen`), each a single ladder, and ONE family array both probe modes
   walk.
3. **The bake identity is hand-formatted.** `DlimCpuConfig` gains `kq_mr<id>`, and
   `dlim_identity` must ALSO append it to the identity string by hand - a field added without
   the string leaves two images with different repack interleaves keyed identically. Why: the
   identity is a formatter, not a reflection. Unquirked: format the `kq_mr*` fields from a table
   the `KqFmt` enum drives.
4. **The `[tune]` family must exist before the emitter does.** `iq4xsq8_tile_gen` carries the
   full `[tune_perm]` grid and a `tune(gen = ...)` key from day one because the loader's kq
   dispatch, the layout companion and the tuner's completeness check all resolve the family by
   name; the generator behind the key is a two-line `=> false` until the emitter arm lands.
   Why: a declined generator is the framework's own fallback path, so the stubs are the
   cheapest way to have a real family. Not a defect - record it so nobody "fixes" the stubs.
5. **Three int id spaces.** `int(KqFmt)` (device stack tags, image plane ids, `vk_kq_schema_id`'s
   input), the kernel/IR id (`kq_schema_id`: 4/5/6/40/44 - runtime params of generated
   kernels, `kq_qsb(int)`, `kq_rows_fn`), and the stream/repack region code (`kq_stream_code`:
   0/2 for q8/q51, else the kernel id). A new format touches all three; `test_kqformat` pins
   them. Unquirked: one id, or at least one table that derives the other two.
6. **`|=` is not defined on `uint8`** in daslang (`blkb[i] |= uint8(x)` is a compile error).
   Build the byte in an `int` and store it once.
7. **The image meta list is hand-maintained behind a count tripwire.** `kq_repack_mr<id>` is a
   non-array `Model` field, so `serialize_image_meta` (`dasllama_image.das`) must
   `serialize_raw` it AND `IMAGE_META_FIELDS` must grow by one, or the first model load
   dies on `verify(count_meta_fields(t) == IMAGE_META_FIELDS)` - after the tokenizer build,
   not at compile time. The tripwire is deliberate (it catches a forgotten field); the hand
   list is the quirk. Unquirked: the per-format `mr` moves into the `KqPlanes` table of
   QUIRK 1 and the image serializes the table.
8. **The repack interleave is frozen onto the Model in a ladder of its own.** `layout_repack_kq`
   repacks every format with the layout companion's `mr`, and a SEPARATE ladder in
   `dasllama_load.das` (two copies: the streamed-save arm and the eager arm) records
   `t.kq_repack_mr<id> = active_kq_layout_mr(<id>)`. A format missing from that ladder keeps
   the field's default (4) while its planes sit at the companion's `mr` (8 on this box): the
   GEMV rail still reads the companion directly and is right, every `kq_active_mr` consumer -
   the fused decode chains, `embed_row`, the grp row dequant - reads the wrong interleave, and
   the model emits "time time time" with no diagnostic. Caught only by an end-to-end run; the
   unit gates build their planes in-process and never see the freeze. Unquirked: the repack
   records its own `mr` (one field per format in the `KqPlanes` table of QUIRK 1), and the
   freeze ladder disappears.
9. **A kernel that runs on a forked worker reads a module-level table as zeros.** The fused
   chains invoke the rows cores inside `team_parallel_stages` / `maybe_parallel_for` job
   contexts; a `let` global (`IQ4NL_LUT`) read there returns 0, so a codebook format's rows
   came out zero on every worker-run row and right on the main-context rows - the GEMV probes
   (no job queue) all passed, the forward was garbage. The k4/k5/k6/q40 cores never had a
   table; the mx4 path already hands its workers a `lutp` pointer for exactly this reason.
   Rule: a kernel that can run on a worker takes its table as a per-call local
   (`iq4nl_lut()`) or a pointer argument, never a module global; keep the global for tests,
   oracles and the emitter's constant bake. Probe that catches it: any kernel gate run under
   `with_job_que() { setup_dasllama_jobque() ... }` with enough rows to fork.
10. **The worktree session sees a stale binder.** LSP/MCP diagnostics in a fresh worktree come
   from the main tree's binary and report the new enum member as "not inferred" and every
   dasllama file as broken (`get_total_perf_cores` missing). Trust only the worktree binary:
   `bin/Release/daslang.exe dastest/dastest.das -- --test <file>`. Run the session inside the
   worktree once it is bootstrapped.
11. **A sidecar minted while the stubs declined pins the family to `"reference"`.** The app's
   auto-policy tune ran during the first end-to-end (QUIRK 4's stubs in place), every perm of
   `iq4xsq8_tile_gen` declined, and `examples/dasLLAMA/run.tune.json` recorded
   `"iq4xsq8_tile_gen" : "reference"` - the framework's explicit-reference form, which forces
   the original body. Sidecar staleness keys on the binary's mtime, and the emitter is `.das`
   (JIT-compiled), so landing it invalidates nothing: the next run logs the same
   `27 tune-stamped`, serves the reference body, and its text and t/s match the pre-emitter run
   exactly - a "the emitter changed nothing" reading that is false. Re-mint with `-- --tune`
   on the app (a whole-scope re-tune) or delete the sidecar. Unquirked: fold the family's
   generator hash (the JIT DLL cache key already carries it) into the sidecar identity, so a
   generator change reads as stale.
12. **A fresh worktree has no JIT until `lib/LLVM.dll` is staged - on every platform.** The
   M1 worktree's first `-jit` run died on `can't load library LLVM.dll`; `utils/mcp/setup.das`
   stages the codegen backend on Windows only. Copy `<main>/lib/LLVM.dll` (+ `.version`) into
   the worktree's `lib/` by hand on macOS/Linux. Unquirked: the posix arm of `stage_jit_backend`.
13. **The Metal test ladders are nested ternaries with an `else` = k6.** `kq_gemv_gate`,
   `kq_mvb_gate`, `kq_mulmm_gate` and the fixtures pick MSL sources / entries / fastmath /
   tgmem names per format in four parallel ternary chains each; a format missing from any one
   of them silently tests k6's kernel under the new format's tag. Same shape as the tier's
   own dispatch ladders (QUIRK 1's cousin on Metal). Unquirked: one per-format record
   (`src, entry, fastmath, tgmem`) per kernel family, indexed by format.
14. **The mul_mm template has no `static_if ... elif`.** Adding a format arm to
   `MetalKqMulMmK45T` beside `SIXBIT` meant re-nesting the k4/k5 arm one level deeper
   (`static_if (IQ4XS) {...} else { <the old arm> }`); the diff is mostly indentation.
   Unquirked: an `elif` on `static_if` in the MSL emitter.
15. **First run on a fresh box tunes before it serves.** `run.das` on the M1 spent its first
   minutes minting `run.tune.json` (every family, the confirm pass included) before loading
   the model - the auto policy, working as designed, but a Mac session's first end-to-end
   "hang" is that mint. Watch `@tune begin/end` lines in the log.
16. **A stub family fails the probe's test mode on every perm whose layout companion
   generates.** `q8q8_layout` is shared and generates for any perm `perm_declines` admits, so
   under `=> false` stubs the k3 family's `dot_maddubs_width256_mr8` perm repacked its planes
   at mr 8 while the declined tile ran the reference body at `k3q8_layout_gen()` = 4 -
   `maxdiff nan`. The lockstep between layout and tile declines IS `perm_declines`, which a
   stub does not share. Keep a stub family out of the probe's TEST list (the tune-mode list is
   fine: the race gates each perm and mints "reference"); it joins the test list with its
   emitter arm. This is why iq4xs entered that list only in Phase B.
17. **A new `[tune]` family re-tunes every application on its first start.** A sidecar with no
   entry for a demanded kernel re-tunes the whole scope (the framework's completeness rule), so
   `run.das` on the box spent minutes minting before the Q3_K_L load - and every other
   sidecar on the box (`chat.das`, the harness probes) does the same on its next run. Budget
   for it; do not read the first end-to-end wall time as a load-time regression.
18. **`DAS_TUNE_POLICY=fallback` is not "the reference body".** It walks the family's
   `fallback=` chain, and on a box where the chain's first viable perm is the crowned one the
   kill switch serves the very stamp under test - the k3 stamped-vs-"reference" rows diff came
   back bit-identical for that reason. The original body is reached only by a `"reference"`
   sidecar entry for the family (copy the app's sidecar next to the probe as
   `<probe>.tune.json`, edit the one entry). Unquirked: a policy value that names the reference
   tier outright.
19. **The device gather's tail-row branch is a second ladder.** `moe_gpu_gather_stack_kq`
   walks grouped rows (the grp<mr> interleave) and ungrouped rows (`d % mr` tails, or an
   unrepacked load) through two different per-format ladders; the ungrouped one re-pairs k4
   nibbles and re-decodes the 6-bit scale packing in its `else`, so a format that is already in
   the device form (q40, iq4xs, k3) needs its verbatim arm in BOTH. iq4xs had only the grouped
   one - latent, because every row of the dense 1B is grouped - and the k3 walk found it.
   Unquirked: one per-format "device form" predicate both branches key on.
20. **A `fixed_array` local in a SPIR-V kernel is Function-storage memory.** `iq4_word`'s
   first form held `kvalues_iq4nl` as a 16-entry `fixed_array<int>` local and indexed it per
   nibble; the emitter lowers a dynamically indexed local array to Function storage - private
   memory the driver spills - and the IQ4_XS Vulkan GEMV decoded at 105.7 t/s against k3's
   372.8 on fewer bytes per weight. The form that runs at speed packs the codebook into four
   `uint4` words (four int8 codes per word) and selects with a dynamic vector index plus a byte
   shift, `(tbl[q >> 2] >> ((q & 3) * 8)) & 0xFF`: 338.4 t/s, 0.99x llama.cpp, kernel suite
   64/64. SPIR-V-only: the Metal twin `iq4_lut` rewritten the same way measured flat (tg128
   144.3 +- 6.2 against 138.7 +- 5.4, pp512 2802 against 2953), so MSL's constant-address
   array stays - the Metal IQ4_XS decode gap (0.56x llama.cpp) is the lane map, not the
   lookup, and belongs to followup_general #58. Unquirked: the SPIR-V emitter lowers a `let`
   fixed_array of literals to a constant-storage array, or a lint on a dynamically indexed
   fixed_array local inside a kernel class.
21. **An emitter-arm edit does NOT invalidate the JIT DLL cache.** The registered code
   generators (`dasllama_gemm_gen.das`) run at codegen time and their bodies do not fold into
   the cached DLL's hash - after an emitter change, every `-jit` run that hits the cache
   executes the OLD stamps with no signal (the iq3s gemv arm "failed" three fix rounds in a
   row on byte-identical numbers before the cache-hit line gave it away; the hash was the same
   0xa3a02e12... across every edit). After ANY emitter change: `rm -rf .jitted_scripts` (or
   bump `LLVM_JIT_CODEGEN_VERSION`) before trusting a probe or bench. Unquirked: the cache
   hash folds the generator bodies (plans/interpreter ledger has the sibling
   `jit_dll_semantic_hash` item).

22. **A Metal kernel lever is judged by a kernel-level probe, never by tg128 e2e.** A tg128
   row on the M1 carries +-8 t/s of noise; the iq3s f4-slab GEMV form (+9% at the kernel)
   measured FLAT on e2e twice and was nearly discarded. The dispatch-loop probe (50 dispatches
   per encoder, best-of-3 encoders, GB/s off plane bytes) resolves 3% in seconds.
23. **Census the vehicle GGUF before committing to it.** An ftype is a MIX: the natural
   "IQ3_XXS" download (mradermacher i1) quantizes attn_k/q to IQ2_S - unsupported until the
   iq2 tier - so the file cannot load end to end. `gguf_census.py` (scratch) reads the tensor
   type table in seconds; pick a vehicle whose mix is {supported} + the new format only
   (bartowski's Qwen2.5-1.5B IQ3_XS carries iq3_xxs with iq3_s/q4_K/q6_K siblings).
24. **A layout/pack edit does NOT invalidate cached `.dlim` images.** The prepared-image
    filename hash folds the model and schema, not the packing code: after the grouped-scale
    copy fix, the rerun "mapped" the stale image and kept serving the broken device planes -
    garbage text and all-zero decode logits survived a correct fix. Delete the model's
    `*.dlim` beside the GGUF after ANY `dasllama_layout.das`/pack edit and confirm the next
    log line says "baked", not "mapped". Unquirked: fold a pack-code version into the image
    hash, the way QUIRK 21's fix would version the JIT DLL cache.

25. **The stream-code space is NOT the kernel-id space: q51 squats on 2.** Stream region
    tags are q8=0, mx4=1, q51=2, then the kq kernel ids - Q2_K's mnemonic id 2 collided and
    the first load dispatched k2 regions down the q51 repack arm (index out of range on the
    empty q51s plane). k2 streams under code 20 (kq_stream_code), translated back to kernel
    id 2 at the two dispatch boundaries (stream_repack_one's arm, repack_regions' fk). A new
    format's kernel id must dodge 0/1/2 in the stream space or claim a distinct code the
    same way.

## Per-format notes

### Q2_K Phase A (CPU, 2026-08-31)

Shape: 256-superblock, the k4/k5 scale STRUCTURE at k6's granularity - 16 per-16-element
sc/min nibble-pair bytes folded as (d*sc)*q - (dmin*mn); the 2-bit lanes are unsigned, so
the min term rides the activation 16-sums (xbsp), exactly dot_k4q8's shape. Disk 84B:
[16 sc/min][64 qs][f16 d][f16 dmin]. Planes: qs verbatim (K2_QSB 64 - k3's lanes minus the
hmask), the scale row reordered header-first [d][dmin][16 sc/min] (K2_SSB 20). Ids:
KqFmt.k2 = 11, kernel id 2, stream code 20 (QUIRK 25). Kernels: dot_k2q8 (dot_k4q8's fold
over 16 groups), k2_grp_row_dot, repack_k2_grp (16 four-byte columns x mr; scale
[16 sc x mr][mr x 4B header]); the tile rides the packed planes. Gates: test_kqformat
18/18, test_kquant 216 (200 pass, 16 env-gated skips), lint 0. E2e: the local requant
(Q2_K x64 + Q3_K/Q4_K/Q6_K siblings, all supported - llama-quantize with the imatrix, no
--tensor-type override needed) decodes coherently at gen 30 t/s reference bodies, greedy
ids 19/64 with the fork a 0.153-logit near-tie (top2 IS our token; the lossiest format
diverges earliest). No whole-scope re-mint fired on the first e2e (zero "@tune begin"
lines), unlike QUIRK 17's precedent - resolved at Phase B: a declined-stub family demands
no sidecar entry; the mint fires once real generators exist.

Phase B (JIT emitter, 2026-08-31): a fourth arm through emit_block_kqv2 - the k2 flag
swaps the quant compose to k3's column walk minus the hmask (2-bit lane at shift
2*(blk%4)), the scale reads to per-16 nibble extracts (ZExt the pair bytes to i32 lanes,
then &15 / >>4 - no i8-width splat needed; scv/mnv carry the two groups' sc, mnlo/mnhi
their mins), keeps the k63 split lo/hi accumulators behind a per16 flag
(vecBsums/fuseAcc/madd16/flush/bs0-bs1), and takes the k4/k5 two-fma epilogue (bacc folds
on dmin). Prerequisite refactor: the grp scale header went FIELD-MAJOR
([16 sc x mr][mr x f16 d][mr x f16 dmin]) so load_f16_vec_at serves the d/dmin vectors -
repack_k2_grp and both grp readers moved together, re-gated bit-exact. Gates: probe test
mode 11/11 k2 perms (maddubs mr8 stamps at 4.8e-7; vpdpbusd declines on zen2 as every
format), test_kquant -jit 212/216. Crowned dot_maddubs_width256_mr8; stamped e2e gen
30 -> 75 t/s, ids 19/64 with the same token-19 near-tie. zen2 16t vs llama.cpp clean-cpu
b10660: pp512 418.6 / 412.0 (1.02x), tg128 77.1 / 81.1 (0.95x). Vulkan, Metal: pending.

### IQ4_NL (the near-free one, 2026-08-30)

Shape: 32-element blocks, 18B each on disk - f16 d + 16 nibble bytes with the k/k+16
pairing. That is Q4_0's disk shape BYTE FOR BYTE; only the nibble semantics differ
(`kvalues_iq4nl` - already shipped as `IQ4NL_LUT` - instead of q - 8, so no offset and no
bsum term). The whole CPU story follows from that identity: planes = `Q40_QSB`/`Q40_SSB`
verbatim, transcode = q40's byte split with the type check swapped, grp repack = literally
`repack_q40_grp` (routed, not copied), and the tile rides the PACKED lists - no panel, no
gather. Identity 45. The scalar dot is iq4xs's LUT loop with q40's per-block f16-d fold;
the JIT emitter is one `nl` mode on `emit_block_iq4xs`: the per-block d vectors load with
the same `load_f16_vec_at` helper over q40's scale interleave, and the fold moves INTO the
block loop (float per-block fma) replacing the sub-scale integer sum - plus the LUT-bake
condition widening to `te.kq == 44 || 45`. QUIRK 16 replayed on cue: the pre-emitter gate
stamped 10 perms whose maddubs rows mismatched (declined generators at layout mr 8); the
emitter arm turned all 11 k45 rows green (maxdiff ~2e-6) in the same sitting.

Vehicle: a local requant again - IQ4_NL needs NO imatrix, just `--allow-requantize`
(mix: iq4_nl x94 + q5_K x18 + q6_K embd, all supported; census first, QUIRK 23). Gates:
`test_kqformat` 18/18, `test_kquant` 199 tests 0 failed, the probe 11/11 k45 perms. E2e
stamped: coherent text at gen 60 t/s, 11/64 greedy ids vs `simple_ids.exe` where the fork
is a 0.042-logit near-tie (the window's smallest margin by 100x) whose top-2 IS our token.
zen2 rows vs llama.cpp clean-cpu: pp512 618.3 vs 540.6 (1.14x), tg128 64.2 vs
62.2 (1.03x).

Vulkan followed as pure composition - not one new decode was written: `KqGemvIq4nl` =
iq4xs's `iq4_dot` codebook compose folded by q40's `q40_d` per-block d (no sub-scale, no
bsum); `KqBatchIq4nl : KqBatchIq4xs` overrides ONLY `stage_ws` with q40's (the LUT stage_w
and the no-min blk_fma inherit); `Iq4nlCm2T` = `VkK4Blk` + the existing `IQLUT` axis with
Q40Cm2T's d pick times `iq4lut[q]`. Schema fmt 10 -> 45, the nine-format family cells at
stride 32 words, the cm2 cell mirrors q40's with the codebook oracle. Gates: the suite
78/78 (the three cm2 tiles 0-off at 89600 cells each); the resident driver arms and runs
gen 245 t/s, greedy ids 11/64 vs `simple_ids.exe` (the near-tie class - the margin
oracle's step-11 tie is 0.042 logits on this stream). Rows (5060 Ti vs llama.cpp b10660
Vulkan): pp512 15027.4 vs 19177.8 (0.78x), tg128 340.7 vs 363.6 (0.94x).

Metal closed the format as the promised compose - the iq4xs kernels with the scale fold
swapped. No blob arm, no off-gate row, no split plane: q40's 16B/sb plane of 8 f16 d's IS
the device form, bound once at soff (kq_scales_of hands doff = 0), and every kernel reads
block bu's d at `kdh[blk*8+bu]` where iq4xs read per-sb d x strip byte. `MetalKqGemvIq4nl`,
`MetalKqMvIq4nlT` B2/B4 + `MetalKqMvB8Iq4nl` (the iq4xs shells over the threadgroup
codebook), and an `IQ4NL` constant on `MetalKqMulMmK45T` riding the IQ4XS arm with only the
dsc read flipped. One checklist line this walk misses easily: the `g_pso_*` globals are
DECLARED in dasllama_metal_common.das - the `[metal_dispatch]` pso= attribute only NAMES
them - so five new kernels = five decl lines there, or every reference errors 30838. Gates
on the M1 Max: test_metal_gemv_kernels 2/2, test_metal_gemm_kernels 2/2 (116 s corpus); the
metal-blob e2e decodes the same stream at gen 285 t/s, ids 11/64 with the fork = the
format's 0.042-logit near-tie at step 11 on every tier.

Where IQ4_NL landed (vs llama.cpp b10660, the local requant):

| tier | pp512 (ours / theirs) | tg128 (ours / theirs) |
|---|---|---|
| zen2 CPU | 618.3 / 540.6 (1.14x) | 64.2 / 62.2 (1.03x) |
| M1 CPU | 727.1 / 724.1 (1.00x) | 134.4 / 133.7 (1.01x) |
| 5060 Ti Vulkan | 15027.4 / 19177.8 (0.78x) | 340.7 / 363.6 (0.94x) |
| M1 Metal | 3599.5 / 3774.6 (0.95x) | 248.4 / 250.2 (0.99x) |

The nearly-free prediction held: not one new decode on any tier, no CPU tg tail (no
gather), and the only real bug of the format was the layout copy arm QUIRK 24 caught.

### IQ3_XXS (2026-08-30)

Shape: 256-superblock, 98B disk block - f16 d, 64 grid-index bytes (`iq3xxs_grid[256]`, one
byte = FOUR magnitudes), 8 x aux32 per superblock (bits 0..27 = four 7-bit `ksigns_iq2xs`
indices, bits 28..31 = the block scale ls; ggml folds w = 0.25 * d * (2ls+1) * grid * sign).
The plane design removes the 0.25 EXACTLY: every ggml grid level is even (4..62), so our
plane grid stores the bytes HALVED (2..31) and the transcode stores d halved
(`f16_half_bits` - an exponent decrement, exact down to the subnormal edge) - the kernel
fold is then iq3s's `d x strip x grid_byte` verbatim with strip = 2ls+1, and the smask sign
trick still holds (it needs g != 0, not odd). Planes 96/20: quants = [qs][aux] verbatim
(24 uniform grp columns), scale = the iq3s row. `ksigns_iq2xs` (bit 7 = even parity) ships
in kqformat and is shared with the coming IQ2 family. Identity 34.

The panel form is SHARED with iq3s: `unpack_iq3xxs_panel_grp` emits the same signed-byte
panel, so `kq_grp_row_dot_b`, the tile ladders and the batch cell take `fmt == 33 || 34`
conditions rather than new arms. Vehicle: `Qwen2.5-1.5B-Instruct-IQ3_XS` (bartowski - iq3_xxs
on attn_k/q + parts of ffn, everything else already supported); the obvious
mradermacher i1-IQ3_XXS 1B carries IQ2_S attn tensors and must WAIT for the iq2 tier -
census the candidate GGUF first (QUIRK 23). Gates: `test_kqformat` 18/18 (tables, parity
property, the halver edges), `test_kquant` 181 tests 0 failed with fmt 34 in every gate
loop - and the tile gate's `packed` list wrongly held 33 (the iq3s tile reads the
byte-expanded panel), a pre-existing red on x64 fixed in the same change. End to end
(reference bodies - both generators decline until the emitter arc): coherent text, 22/64
greedy ids vs llama.cpp's `simple_ids.exe` where the fork is a 0.12-logit near-tie whose
top-2 IS our token (`simple_ids_margin`), gen 19 t/s.

The JIT emitter followed the same day and is the smallest arm of the arc: the TILE is free
(the panel route widens - `emit_one_block`'s and `emit_block_iq4xs`'s `te.kq == 33` become
`33 || 34`; stride, scale row and fold are already the panel's), and only the gemv gather is
new. `emit_iq3xxs_gather` mirrors the iq3s gather with three changes: the aux32 sign word is
ONE aligned i32 column load per block (the four aux bytes of a block land contiguous in the
grp column layout), sign bytes come from a `dasllama.iq3xxs.ksigns` [128 x i32] global
(then the SAME smask nibble expansion), and the grid index is the bare qs byte into the
[256 x i32] HALVED grid - no 9th bit. Two probe lists must both gain the format or the gate
silently skips it: the test-mode fixture families AND the tune loop's
`fixed_array(4l, 5l, ...)` - the first run stamped 10 perms and gated NONE of them (grep
`k34` in the test log to prove coverage). Gates: 11/11 k34 perms ok vs the oracle (maxdiff
~2e-6); the tuner crowns `dot_maddubs_width256_mr8` verdict=beats (the zen2 board's usual
winner, same as iq3s/k3). E2e stamped: prefill 6 -> 17 t/s, gen 19 -> 41 t/s; ids 17/64
with the fork again ON the margin oracle's top-2 (0.43 logits at step 17 - stamped float
folds move the flip point, they do not change the class).

Vulkan came as the iq3s walk compressed: `vk_kq_schema_id` 9 -> 34; the 1 KB HALVED grid
stages into `@workgroup uint[256]` (one word per thread on the tile, four on the gemv), and
the ksigns TABLE DISAPPEARS - bit 7 of the sign byte is the even parity of the 7-bit index,
five ALU ops (`t = v ^ (v >> 4); t = t ^ (t >> 2); t = t ^ (t >> 1)`; the SPIR-V emitter has
no `^=`, spell the fold as plain assignments). `KqGemvIq3xxs : KqGemvBase` (24-word rows,
one aligned aux word per block, iq3s's fold via `iq4_sc` - the (2ls+1) strips read the same);
`KqBatchIq3xxs : KqBatchIq4xs` (the halved-grid gather at stage time); `Iq3xxsCm2T` on a new
`IQ3XGRID` gated axis (the aux 7-bit field spans at most two bytes - two unpack8 picks per
element). The sdot4 arms, cm2 dispatcher arms and the family/witness/cm2 test cells mirror
iq3s's rows exactly. VEHICLE trap beyond QUIRK 23: the Qwen2.5 vehicle proves kernels but
CANNOT arm the resident decode driver (attention QKV bias - a silent decline), so the
Vulkan e2e/bench vehicle is a LOCAL requant: `llama-quantize --allow-requantize --imatrix
<bartowski's> --tensor-type attn_k=iq3_xxs --tensor-type attn_q=iq3_xxs <Q8_0> out IQ3_XXS`
- llama.cpp's own ftype recipe puts IQ2_S on attn_k/q, the override keeps the mix inside
the supported set. Gates: the suite 76/76 (the three cm2 tiles 0-off at 89600 cells each,
the float witness in the family cell); the resident driver arms and matches llama.cpp's
greedy ids 32/64 with the fork a 0.093-logit near-tie whose top-2 IS our token (the
smallest margin in the window), gen 202 t/s. Rows (5060 Ti vs llama.cpp b10660 Vulkan,
the local requant): pp512 12225.7 vs 17807.7 (0.69x), tg128 372.1 vs 389.9 (0.95x).
Metal closed the format the same day, and the iq3s walk paid for itself twice: the blob
"iq3xxss" arm is the iq4xs 20->18 split verbatim, `kq_quants_of` binds at sb0*96, and the
kernels are the iq3s shapes with three deltas - `iq3xxs_gw` (the 1 KB halved grid through
the constant-table hoisting), `ksign7m` (the parity sign byte - no ksigns table on Metal
either), and 24-word rows with one aux word per block. The GEMV ships directly in the
crowned iq3s form (float4 magnitude slab + sign selects, 4 rows/simdgroup, dispatch
rows/8 - QUIRK 22's race already ran); the B2/B4/B8 twins and the `IQ3XXS` mul_mm arm (a
gated uint[256], the chain re-nested once more) follow their iq3s siblings. One
cross-tier find: the CPU dequant/dot strip reads went SIGNED (int8) to match the GPU
kernels' decode - real strips are 1..31 either way, but the synthetic test fills carry
high-bit bytes and the split-form fill is shared across formats. Gates on the M1 Max:
`test_metal_gemv_kernels` 2/2, `test_metal_gemm_kernels` 2/2; e2e decodes the SAME story
stream as the CPU/Vulkan tiers at gen 218 t/s (ttft 106ms).

Against llama.cpp b10660 (`lcpp_bench --for-debug-purposes`; zen2 = 16 threads, M1 = 8;
the local --tensor-type requant, iq3_xxs on attn_k/q + all ffn):

| tier | pp512 das / llama.cpp | tg128 das / llama.cpp |
|---|---|---|
| zen2 CPU | 507.2 / 136.0 (3.73x) | 56.7 / 72.6 (0.78x) |
| 5060 Ti Vulkan | 12225.7 / 17807.7 (0.69x - the tier class) | 372.1 / 389.9 (0.95x) |
| M1 CPU | 906.0 / 410.5 (2.21x) | 53.5 / 74.0 (0.72x) |
| M1 Metal | 3224.0 / 3429.9 (0.94x) | 213.5 / 227.3 (0.94x) |

(The tg tails - zen2 0.78x, M1 CPU 0.72x - are the arc's ledgered CPU-decode class,
followup_general #60/#61: the per-superblock gather never amortizes at one token.)

### IQ3_S (the third format - and the first grid format, 2026-08-30)

Shape: 256-superblock grid format - an 8-bit grid index plus a qh ninth bit selects
`iq3s_grid[512]` (each uint32 = 4 weight magnitudes), the block's own sign bytes flip them (no
ksigns table anywhere - the disk carries explicit signs), scale = f16 d x (1 + 2 x 4-bit
sub-scale); signed reconstruction, so no `xbsp` term. Disk block 110 B: f16 d, 64 qs, 8 qh,
32 signs, 4 packed scale nibbles. Plane pair: quants [qs 64][qh 8][signs 32] VERBATIM (104 B -
the k3 "disk is already the device form" answer), scale row = the iq4xs/k4 20 B shape with
(1 + 2s) decoded at transcode, so every k4-row consumer serves unchanged. Ids: `KqFmt.iq3s`
= 8, kernel id 33, stream code 33. The codebook question a grid format adds: the 2 KB table
ships as `iq3s_grid()` - the per-call-local worker-safe form `IQ4NL_LUT` documented, just 512
entries now - plus the main-context `IQ3S_GRID`; the repack is 26 uniform 4-byte columns (no
per-region split - simpler than k3's, since nothing shifts per lane). Gates on the worktree
binary: `test_kqformat` 16/16, `test_kquant` 163 tests 150 pass / 13 env-gated skips (the
batch-groupn family class every format shares); the emitter stubs decline (QUIRK 4), the
family minted `verdict=rejected` (QUIRK 16's shape) after the whole-scope re-tune (QUIRK 17),
and the reference bodies serve. End to end: `Llama-3.2-1B-Instruct-IQ3_M.gguf` (bartowski:
IQ3_S x78 + Q4_K x34 + Q6_K embd) through `run.das` matches llama.cpp's greedy ids **64 of
64** at gen 23 t/s - the first format to hold the whole comparison window token-for-token.
JIT emitter (section 5, the first side-by-side under the llama.cpp-kernel rule): the TILE
rides the panel route - `unpack_iq3s_panel_grp` gathers grid words + signs into the k5/k6
byte-expanded panel per group (one i32 store per grid word, sign nibble through a 16-entry
mask table, negate = `(w ^ m) + (m & 0x01010101)` - magnitudes are odd 1..15, no cross-byte
carry) - and the GEMV gathers each superblock into an alloca panel via an emitted per-row
loop (`emit_iq3s_gather`; grid + mask as private module constants); both then ride
`emit_block_iq4xs`'s sign-trick lattice with panel loads in place of the nibble+LUT. Two
finds along the way: `emit_slice` recorded SAVED block handles as phi incomings - a
block-splitting emitter leaves the builder elsewhere, so back-edges were malformed (fixed:
capture `LLVMGetInsertBlock` at the branch points); and QUIRK 21 ate three fix rounds.
Probe test mode: every k33 perm ok (maddubs mr8 stamped, 1.9e-6); the tuner crowned
`dot_maddubs_width256_mr8` at ~4.9x the reference body. Side by side on the zen2 (16t,
llama.cpp b10660 clean-cpu): pp512 516.9 vs 104.9 (**4.93x** - the panel amortizes the
gather across the tile; their per-row kernel re-gathers per token), tg128 52.4 vs 57.0
(0.92x - nothing amortizes at one token; the no-panel gemv spelling to close it is
followup_general #61). Stamped e2e: 63/64 greedy ids (the flip is the FINAL token, the
stamped-vs-reference near-tie class), gen 44 t/s.

Vulkan (section 6, the first grid format on the tier): `vk_kq_schema_id` 8 -> 33; the 2 KB
grid stages into `@workgroup uint[512]` once per kernel (llama.cpp's `init_iq_shmem` form) -
the source is `iq3s_grid_word`, a per-index accessor over a fixed_array local (QUIRK 20's
slow path, off the hot loop; a shader function cannot RETURN a fixed array), and the sign
nibble expands arithmetically (`mask = ((nib * 0x00204081) & 0x01010101) * 255`, negate =
`(g ^ mask) + m1`). `KqGemvIq3s : KqGemvBase` (26-word rows read as scalar uints - 104 B is
not uint4-aligned; iq4xs's scale fold via `iq4_sc`); `KqBatchIq3s : KqBatchIq4xs` (the grid
gather AT STAGE TIME per staged uint4; scale row and blk_fma inherited verbatim). TWO traps
this walk found: a test family cell whose enc ladder lacks the new arm silently dispatches
the ELSE format's pipeline over the new planes (fmt 8 ran the k6 tiles - 2200/2200 off with
byte-stable garbage across "fix" rounds; add the enc arm to EVERY per-fmt ladder in the
cell, not just ensure/strides); and `pf_f16_feed` admitting a format via `kq_sb` before its
cm2 tile exists sent iq3s prefill through the cm2 dispatcher whose fall-through served the
q8 tiles - garbage text at full speed. The feed now excludes iq3s until its tile lands and
all three cm2 ladders `verify` on a non-q8 fall-through. The CPU replay of a class whose
kernel stages workgroup state (the grid) must fill that state in `kq_cls_ref` - the replay
never runs the kernel head. Gates: the suite 71/72 with the seven-format family cells and
the iq3s float witness; the resident driver matches llama.cpp's greedy ids **64 of 64** at
gen 262 t/s. The cm2 tile followed in the same phase: an `IQ3GRID` gated axis on
`KqCm2BatchT` stages the 2 KB grid into a `@workgroup uint[512]` before the tile loop, and
`Iq3sCm2T`'s decode gathers per element over the 16-bit-lane block views (`VkIq3sBlk`:
qs/qh/signs as int16 arrays) with iq4xs's scale fold; three width stamps, the dispatcher
arms, and iq3s re-admitted to `pf_f16_feed`. The three tiles gate 0-off (89600 cells each);
the e2e holds 63/64 on the f16 feed (the final-token near-tie; the quant feed ran 64/64).
Rows (5060 Ti vs llama.cpp b10660 Vulkan): tg128 288.1 vs 324.2 (0.89x), pp512 12540 vs
17865 (0.70x - AT the tier's shared 1B-shape class; the k4 control on this box is 0.67x),
up from 6241 on the quant feed.

Metal came in five surfaces plus one emitter feature. The blob "iq3ss" arm is the iq4xs
20->18 split VERBATIM over `t.iq3ss` (same strip/d addressing, so `kq_scales_of` is a copy of
the iq4xs arm with the plane swapped); quants bind at `sb0*104`. The MSL emitter grew
constant-table hoisting for the grid (its own dasMetal commit: a `let` fixed-array local with
all-literal elements lowers to a program-scope `constant T name[N] = {...}` - before that,
ANY fixed-array initializer was a compile error, and there is no other way to put 2 KB of
data into a kernel). `iq3s_gw()` carries the 512-word literal; the GEMV and mul_mm stage it
into threadgroup memory, the Mv twins read it direct (unmeasured batch shapes). Kernels:
`MetalKqGemvIq3s` (the iq4xs parity shape but 4 rows/simdgroup - llama.cpp's N_R0_IQ3_S -
with the grid staged as a `float4[512]` MAGNITUDE slab and signs applied by select),
`MetalKqMvIq3sT` B2/B4 + `MetalKqMvB8Iq3s` (iq4xs shells, per-32-block lane map, consecutive
float4 x loads), an `IQ3S` arm in `MetalKqMulMmK45T` (threadgroup grid under
`@template_gate`; re-nest the chain - `} else static_if` is a parse error, QUIRK 14's rule
holds in the MSL emitter too).

The decode-GEMV gap got the full treatment: at n=2048 d=8192 the shipped kernel raced EIGHT
forms (tg-slab uint gather, constant-table gather, duplicated slab, gather deleted, sign
flip deleted, llama.cpp's exact 1-lane-per-block geometry, 2- vs 4-row, f4 magnitude slab) -
all land in 127-141 GB/s while k4 does 204 and k6 287 in the same harness. The compose chain
(9-bit index + per-nibble signs) is format-intrinsic on M1; the f4 slab is the best form
(+9%) and ships. Gates on the M1 Max: `test_metal_gemv_kernels` 2/2, `test_metal_gemm_kernels`
2/2 (~108s corpus), e2e decodes the coherent story at gen 227 t/s.

Against llama.cpp b10660 (`lcpp_bench --for-debug-purposes`, das = the debug-jit instrument;
zen2 = 16 threads, M1 = 8; IQ3_M, so attn_v/attn_output/ffn_down are Q4_K and the tied
embedding head is Q6_K - three formats share every decode step):

| tier | pp512 das / llama.cpp | tg128 das / llama.cpp |
|---|---|---|
| zen2 CPU | 516.9 / 104.9 (4.93x) | 52.4 / 57.0 (0.92x) |
| 5060 Ti Vulkan | 12539.6 / 17865 (0.70x) | 288.1 / 324.2 (0.89x) |
| M1 CPU | 886.2 / 433.6 (2.04x) | 57.4 / 66.6 (0.86x) |
| M1 Metal | 3237.6 / 3344.3 (0.97x) | 199.4 / 209.0 (0.95x) |

(The M1 Metal ref tg drifts 209-230 across back-to-back rounds - thermal; ratios are
same-run. The tg tails on every tier are the one ledgered class: followup_general #61/#62.)

### Q3_K (the second format, 2026-08-30)

Shape: 256-superblock, k6's scale structure exactly (16 per-16 signed sub-scales + f16 d, the
18 B row) over a 3-bit quant composed from a 2-bit lane (byte h*32 + l of `qs`, shift 2j, block
b = 4h + j) and an `hmask` bit (bit b of byte l), offset -4 folded like k6's -32. Disk block
110 B: hmask[32], qs[64], 12 packed 6-bit scales, f16 d. Planes: quants `[qs 64][hmask 32]`
verbatim (K3_QSB 96), the scale row DECODED at transcode (`q3k_scale6` = ggml's kmask unpack,
stored as int8 scale - 32; K3_SSB 18). Ids: `KqFmt.k3` = 7, kernel id 3 (bit width), stream
code 3. Kernels are k6's with a different compose: `dot_k3q8`, `k3_grp_row_dot`
(`isum - 4 * bsum`), `repack_k3_grp` (the 2-bit lanes and mask bits stay in place as 4-byte
columns x mr; k6's scale interleave). The tile reads the PACKED planes (`packed` includes 3) -
no byte-expanded panel. Walk cost: one Python patch twinning every `iq4xs` arm of the pilot's
CPU-slice commit (`63da7571e`) for k3 plus the hand-written codec/kernels/repack; the only
misses the census found were two plane-byte accounting sums the pilot itself had skipped.
Gates: `test_kqformat` 14/14, `test_kquant` 132/144 interpreted and 140/144 under `-jit`,
the probe's tune-mode family (test mode waits for the emitter - QUIRK 16). End to end:
`Llama-3.2-1B-Instruct-Q3_K_L.gguf` (bartowski: Q3_K x64 + Q5_K x48 + Q6_K embd) through `run.das` matches llama.cpp's greedy ids (`simple_ids.exe`, compared through `llama-tokenize --ids` on our text) for 52 of 64 tokens on the reference bodies, gen 22 t/s.

JIT emitter: no new block body - a `k3` flag through `emit_block_kqv2`'s k6 arms (`k63`
shares the scale row, the per-16 fold and the i16 flush cadence; `koff` = 4 / 32), the compose
being qs column `(blk/4)*8 + j` (lo) / `+ 4` (hi) shifted by `2 * (blk % 4)` OR the hmask
column `j` / `j + 4` bit `blk` shifted up two. The stubs became `kq_*_gen_impl(gc, 3)`, k3
joined the probe's test list. Gates: the probe 10/10 perms on x64 (`dot_maddubs_width256_mr8`
live, maxdiff 4.8e-7) and on the M1 (sdot mr 4/8 stamps bit-exact), `test_kquant -jit`
140/144 on both. After the sidecar re-mint (`k3q8_tile_gen: dot_maddubs_width256_mr8`,
`verdict=beats`) the 1B decodes at gen 60 t/s (22 on the reference body) and its text flips
at token 15 - a near tie: against the reference body on two real k3 tensors (10240 output rows,
scratchpad `k3_rows.das` with a `"reference"` sidecar entry) 4957 rows are bit-identical and
the worst relative difference is 1.7e-4 on a row of magnitude 6e-5 (fp32 fold-order noise).

Vulkan: `vk_kq_schema_id` 7 -> 3; `KqGemvK3 : KqGemvBase` (k6's blk_contrib with `k3_dot` -
the lane/mask compose over 6 uint4 per superblock, qs 0..3 and hmask 4..5 - and the offset 4)
and `KqBatchK3 : KqBatchK6` (a child of the k6 class: `stage_ws` inherited, `stage_w` composes
the staged words, `blk_fma` is k6's with 4). A child class must follow its parent in the file
(the class rail resolves parents in order). Five ladders, the class oracle arm, family cells at
six formats, `k3_gemv_float_oracle` as the witness. The device gather
(`moe_gpu_gather_stack_kq`) gained k3 arms in both its grouped and tail-row branches - and the
tail-row branch turned out to lack iq4xs arms too (QUIRK 19). `test_vulkan_kernels` 64/64;
the 1B Q3_K_L on the resident driver matches llama.cpp's greedy ids for 51 of 64 tokens at gen
284 t/s.

Metal: the k3 scale row IS the k6 form, so `metal_blob_scale_plane("k3s")` is the k6 split
verbatim over `t.k3s`, `kq_scales_of` the k6 arm, `metal_blob_off_ok` the k6 rule; quants bind
verbatim (96 B). Kernels are k6's twins with the compose: `MetalKqGemvK3` (k6's lane map - a
lane's 4 elements of each of a half's 4 blocks share the qs bytes at shifts 0/2/4/6 and the
hmask bits 4ip..4ip+3, exactly k6's `sums[0..3]` shape), `MetalKqMvK3T` B2/B4 + `MetalKqMvB8K3`
(k6's slot map over 24 uints per superblock: qs 0..15, hmask 16..23), a `K3` arm in
`MetalKqMulMmK45T` beside `SIXBIT` (the k4/k5/iq4xs arms re-nested once more - QUIRK 14). A
local named `half3` is a reserved vector type name in daslang; the emitter reports it as a
syntax error at the `let`. Ladders as iq4xs's; fixtures at fmt 3 reuse the k6 split-form fill,
`kq_row_ref` via `dequant_k3_plane_superblock_at`. Gates on the M1 Max:
`test_metal_gemv_kernels` 2/2, `test_metal_gemm_kernels` 2/2; the 1B Q3_K_L on the Metal tier
(`k3_metal_probe.das`, the iq4xs probe with the model swapped) decodes a coherent story at gen 223
t/s, taking the other side of the same token-15 near tie the CPU stamp took.

Against llama.cpp b10660 (`lcpp_bench`, das = the debug-jit instrument; zen2 = 16 threads, M1 =
8; Q3_K_L, so a third of the weight bytes are k5/k6):

| tier | pp512 das / llama.cpp | tg128 das / llama.cpp |
|---|---|---|
| zen2 CPU | 543.6 / 310.8 (1.75x) | 66.4 / 65.1 (1.02x) |
| 5060 Ti Vulkan | 14031 / 17509 (0.80x) | 374.5 / 349.1 (1.07x) |
| M1 CPU | 524.6 / 223.5 (2.35x) | 127.3 / 110.6 (1.15x) |
| M1 Metal | 3316 / 3219 (1.03x) | 200.9 / 193.0 (1.04x) |

Decode is at or above llama.cpp on every tier; the Vulkan prefill gap closed with the cm2
tile (section 6b, 0.30x -> 0.80x).

### IQ4_XS (the pilot, 2026-08-30)

Shape: 256-superblock, codebook nibble (`kvalues_iq4nl`, signed, no offset), scale = f16 d x
6-bit sub-scale - 32. Disk block 136 B: f16 d, u16 scales_h, 4 scales_l bytes, 128 nibbles
paired k/k+16 per 32-block. Plane pair: quants verbatim (the q40 tiling), scale row 20 B decoded
at transcode ([f16 d][2 pad][8 x int8 (ls-32)][8 pad] - the k4 decoded-row shape, so
`repack_k4_grp`'s scale addressing is reused verbatim in `repack_iq4xs_grp`). Dot fold:
`sum_blk sc * sum_k LUT[q] * x`, then `* (d * xs)` per superblock; no `xbsp` term. Ids:
`KqFmt.iq4xs` = 6, kernel id 44, stream code 44. Gates green on the interpreter binary
AND under `-jit`: `test_kqformat` 14/14, `test_kquant` 114/114 (iq4xs arms: transcode+dequant
exact, dot vs fp64 at n=512/1024/3072, GEMV rows, repack mr=4/8/16, tile, groupn). End to end:
`Llama-3.2-1B-Instruct-IQ4_XS.gguf` (bartowski; IQ4_XS x96 + Q5_K + Q6_K) through
`examples/dasLLAMA/run.das` reproduces llama.cpp's greedy continuation of "Once upon a time"
for ~45 tokens before the fast-math divergence (gen 42 t/s, reference bodies). Two bugs the
unit gates could not see, both caught only by the run: QUIRK 8 (the repack-mr freeze) and
QUIRK 9 (the codebook global on workers). Method that found them: a Python oracle that
dequantizes rows straight from the GGUF bytes, a daslang probe calling `mm_at_kq_pre` on the
same rows (run it UNDER the job queue), a layer bisect (`config.n_layers = L`, logits cosine
against a sibling quant of the same model), and a dump of the image's group-0 plane bytes
against the grp<mr> layout computed in Python. JIT emitter: `emit_block_iq4xs` (section 5);
family gate 10/10 perms, live stamp on this box `dot_maddubs_width256_mr8` (mr 8, maxdiff
1.5e-5), `test_kquant` 121/125 under `-jit`; after the sidecar re-mint (QUIRK 11) the 1B
decodes at 59-60 t/s against 39 t/s on the reference body, same text. The body rides mx4's
chunk-load + lane-splat dot path; `emit_block_kqv2`'s x64 `vpbroadcastd` / `madd16` chains are
the untried next lever. Vulkan (section 6): `KqGemvIq4xs` + `KqBatchIq4xs`, the kernel suite
64/64 with the five-format family cells and the float witness, and the 1B IQ4_XS model on the
resident driver reproduces the CPU text at gen 102 t/s (prefill 38 t/s on the 5-token prompt);
the codebook lookup then cost 3.5x on the GEMV until QUIRK 20's packed-word form. Metal
(section 7): `MetalKqGemvIq4xs`, the `MetalKqMvIq4xsT` B2/B4 + `MetalKqMvB8Iq4xs` twins, the
`IQ4XS` mul_mm arm; gates 2/2 + 2/2 on the M1 Max.

Against llama.cpp b10660 (`lcpp_bench`, das = the debug-jit instrument, ~8% under the tuned
exe; zen2 = 16 threads, M1 = 8):

| tier | pp512 das / llama.cpp | tg128 das / llama.cpp |
|---|---|---|
| zen2 CPU | 475.7 / 256.9 (1.85x) | 65.0 / 59.5 (1.09x) |
| 5060 Ti Vulkan | 15334 / 17060 (0.90x) | 334.7 / 340.7 (0.98x) |
| M1 CPU | 796.0 / 263.0 (3.03x) | 132.3 / 128.2 (1.03x) |
| M1 Metal | 3612 / 3575 (1.01x) | 249.4 / 249.3 (1.01x) |

Both gaps closed 2026-08-30: the cm2 decode tile (section 6b) took Vulkan prefill past the k4
control's 13144, and the threadgroup-LUT GEMV/mul_mm rework took Metal to parity on both axes;
what remains anywhere is the tier-wide small-model prefill gap the k4 control shares.
