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

## 7. Metal

Untested at the time of writing (no Mac in the session). `MetalKqGemvK4` / `MetalKqMulMmK45T`
/ `MetalKqMvK4T` are the twins to subclass; `dasllama_metal_shapes.das` carries the servable
predicates, `dasllama_metal_common.das` the plane buffers, `dasllama_metal_prefill.das` the
PSO ladders.

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

## Per-format notes

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
64/64 with the five-format family cells and the float witness, and the 1B IQ4_XS model on the resident driver reproduces the CPU text at gen 102 t/s (prefill 38 t/s on the 5-token prompt). Metal: pending.
