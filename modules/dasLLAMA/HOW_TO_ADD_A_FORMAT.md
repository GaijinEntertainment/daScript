# How to add a weight format to dasLLAMA

A HOW_TO is the third document mood: `REVIEW.md` binds a diff, the `ARCHITECTURE*.md` set says
how the system is - this says **do this, then this**. It walks the code in the order a new
format lands and cites the architecture section that owns each step instead of restating it. It
is validated by execution: every step below was run on a format that landed, or is marked
untested. A step that turns out wrong during the next format's bring-up is fixed here in that
session. Where the pattern breaks - a ladder the compiler does not find, a form that runs at a
tenth of the speed - the trap sits in the step where it bites; the work that would remove a trap
is a row of `followup_general.md` (items 131 to 136 are this recipe's).

The worked example is IQ4_XS; where a later format took a different path, the step says so.

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
   needs them, a signed reconstruction (codebook, k6's -32) does not. A constant factor in the
   reference dequant (the grid formats' 0.25 or 1/8) is removed at transcode by halving `d`
   (`f16_half_bits` is an exponent decrement), once per power of two, with the grid magnitudes
   stored halved where every level is even - the kernels then fold `d x strip x grid_byte` with
   no constant. The xor-and-add sign trick that fold relies on (`(g ^ mask) + m1`) is valid only
   because grid magnitudes are odd, so no byte carries into the next.

Write the answers down; they are the first lines of the PR body's format section.

## 1. Identity - `dasllama/dasllama_kqformat.das`

The taxonomy every other file keys off. One edit here, then the compiler finds the ladders.

- Append the member to `KqFmt` - **append, never reorder**: the int value is the device stack
  tag (`vk_kq_schema_id`) and the image plane id.
- `kq_sb` (both overloads) if it is a superblock format.
- `kq_schema_id`: the kernel/IR id. The ids are mnemonics - K-quants by bit width (4/5/6),
  Q4_0 = 40, i-quants = bit width x 10 + a variant digit (IQ4_XS = 44). Three id spaces exist -
  `int(KqFmt)`, the kernel id, and the stream/repack region code (`kq_stream_code`: 0/2 are
  q8/q51, else the kernel id) - and a new format touches all three; `test_kqformat` pins them.
  A kernel id of 0, 1 or 2 collides in the stream space (Q2_K streams under 20 and translates
  back at every dispatch boundary), so dodge those or claim a distinct code the same way, and
  key every region ladder on the translated kernel id, never on the stream code by position.
- The stride constants `<FMT>_QSB` / `<FMT>_SSB` (bytes per superblock row of the quant and
  scale planes) and the `kq_qsb` / `kq_ssb` arms. A codebook goes here too (`IQ4NL_LUT`); a grid
  or codebook table reaches worker lanes only as a function returning the literal
  (`iq3s_grid()`, `iq2s_grid2()`: a direct `return fixed_array<T>(...)`, no local), never as a
  module global - a team-lane kernel reads a `let` global as zero.
- `dasllama_gemm_schema.das`: the int-id twins `kq_qsb(int)` / `kq_ssb(int)`, and
  `kq_reads_packed_planes` - the one packed-versus-panel predicate the batch cell generator, the
  probe, the tests and the bench all read.
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
  the PACK direction so a misread cannot cancel; `|=` is not defined on `uint8`, so build each
  byte in an `int` and store it once) and an arm asserting the plane dequant equals the
  hand-computed weights exactly.

**Plane layout rule.** Quant plane = the disk nibbles verbatim when the disk pairing is already
what the kernels want (Q4_0 and IQ4_XS pair k/k+16 inside a 32-block - the k4 tiling; Q4_K
pairs k/k+32 and re-pairs at repack). Scale plane = a fixed-stride row per superblock. The kq
v2 rule keeps the DISK scale block verbatim and decodes at repack (k4/k5: 16B block + 4B pad);
a format whose decoded form FITS the row may decode at transcode instead (IQ4_XS: f16 d, 2 pad,
8 x int8 sub-scale, 8 pad = the k4 decoded-row shape, so k4's repack addressing serves).

## 3. Planes and the loader - `dasllama_common.das`, `dasllama_load.das`, `dasllama_layout.das`

This is the ladder walk (`followup_general.md` item 131). Every site is a flat
`if (fmt == KqFmt.k4) ... elif` chain; add the arm next to `q40`'s. The compiler does not find
these - a missing arm falls to the `else` panic (good) or silently to k6 (bad, the
`kq_kernel_gen` shape); grep `KqFmt.q40` and `== 40` and visit every hit.

- `Model`: the plane pair `<fmt>q` / `<fmt>s` and `kq_repack_mr<id>`.
- `dasllama_load.das`: `LayoutSizes.<fmt>_n`, `KqCursors.<fmt>`, `kq_take`, the `LayoutSizes`
  constructor, `stream_field_of` (the streamed image plane name), the scale-half landing
  `memcpy`, the stream repack `invoke(g_stream_repack, <id>, ...)`, `transcode_kq_tensor`,
  `load_big`, `kq_fmt_of` (GGML type -> tag), the two `noisy` log lines,
  `g_stream_plane_total["<fmt>q"]`, the plane `reserve`/`resize` block, and the
  repack-interleave freeze ladder (`t.kq_repack_mr<id> = active_kq_layout_mr(<id>)`, two copies:
  the streamed-save arm and the eager arm) - a format missing from that ladder keeps the field's
  default while its planes sit at the companion's `mr`, every `kq_active_mr` consumer reads the
  wrong interleave, and the model emits repeated tokens with no diagnostic; only an end-to-end
  run sees it.
- `dasllama_common.das`: `kq_active_mr`, `kq_fi`, `mm_at_kq_pre` (two arms), `mm_b_kq_tile`,
  `mm_at_kq_groupn` (two arms), `mm_b_kq_groupn`, `mm_b_kq_pre`, `kq_plane_q`, `kq_plane_s`,
  `embed_row` (four arms: grp and plane form, trimmed and untrimmed), the bake config fill
  (`c.kq_mr<id> = active_kq_layout_mr(<id>)`).
- `dasllama_layout.das`: the plane base pointers + the `rkq` ternary, `push_repack_kq`, the
  `moe_gpu_gather_stack_kq` plane ternaries - the gather walks grouped rows and tail rows
  (`d % mr`, or an unrepacked load) through two per-format ladders, so a format already in the
  device form needs its verbatim arm in both. Bump `PACK_VERSION` with any pack edit: it folds
  into every image identity, so the next load re-bakes instead of mapping a stale `.dlim`.
- `dasllama_gpu_resident.das` (embed trim), `dasllama_ple.das` (two arms),
  `dasllama_blocks.das` (`kq_bytes_per_weight`).
- `dasllama_config.das`: `DlimCpuConfig.kq_mr<id>` AND the identity string in `dlim_identity`
  (a field added without the string keys two interleaves identically); `dasllama_image.das`:
  `IMAGE_VERSION` bump, the streamed plane name list in `stream_extra_bytes`, `serialize_raw`
  of the new `kq_repack_mr<id>` in `serialize_image_meta` and `IMAGE_META_FIELDS` grown by one -
  the count tripwire fires at the first model load, after the tokenizer build.

## 4. CPU kernels - `dasllama_math_default.das`, `dasllama_math_gen.das`, `dasllama_math.das`, `dasllama_repack.das`

- **Read the reference build's CPU kernel for the format FIRST** - `ggml-cpu/arch/x86/quants.c`
  `ggml_vec_dot_<fmt>_q8_K` (and the arm twin) in the local clone. Map each technique it uses
  onto the lattice as its own `[tune_perm]` spelling where it fits; the probe IS the side-by-side
  (perms race each other and the reference, per box). Our CPU kernels typically win - keep it
  that way by never leaving one of their tricks unmeasured.
- `dot_<fmt>_q8` - the portable disk-order row dot (exact integer inner sums, one float fold per
  superblock), `<fmt>_rows_kernel`, the `kq_gemv_kernel` arm, the `matmul_kq_groupn` arm. A
  kernel that can run on a forked worker takes its table as a per-call local or a pointer
  argument, never a module global: the fused chains invoke the rows cores inside job contexts,
  where a `let` global reads zero, so a codebook format's rows come out zero on every worker row
  and right on the main-context rows - the GEMV probes (no job queue) pass, the forward is
  garbage. The probe that catches it is any kernel gate run under
  `with_job_que() { setup_dasllama_jobque() ... }` with enough rows to fork.
- `dequant_<fmt>_row_grp` - the grp<mr> row dequant (own helper; `dequant_kq_row_grp` dispatches).
- `repack_<fmt>_grp` (`dasllama_repack.das`) - disk-order -> grp<mr> planes; tail rows (d % mr)
  stay disk-order untouched. A format with both d and dmin takes a field-major grp scale header
  (`[16 sc x mr][mr x f16 d][mr x f16 dmin]`, `repack_k2_grp`'s shape) so `load_f16_vec_at`
  serves d and dmin as vectors; the repack and both grp readers move in one bit-exact step.
- `dasllama_math_gen.das`: `<fmt>q8_layout_gen` (the layout companion), `<fmt>_grp_row_dot`
  (the scalar grp reference = the stubs' body and the repack oracle), `<fmt>q8_gemv_gen` +
  `<fmt>q8_tile_gen` with the `[tune_perm]` grid copied from q40's and
  `tune(gen = "dasllama_gemm_gen::<fmt>_tile", ...)`, `kq_layout_of`, `repack_kq_gen` /
  `repack_kq_bake`, `kq_kernel_gen` (two ladders), `kq_batch_cell_gen` (`packed` + tile +
  tail ladders - a packed format with no arm here dereferences a null scale plane inside the k6
  tile under the JIT, a panel format with no arm silently decodes as k6), `kq_batch_kernel_gen`
  tail, `kq_batch_groupn_gen` tail, `kq_groupn_gen` (two ladders), both
  `register_kernel_backend` rows (`kq_rows_<fmt> = @@<fmt>q8_gemv_gen`).
- `dasllama_math.das`: the `KernelBackend.kq_rows_<fmt>` slot, its `g_kq_rows_<fmt>` global,
  the activation copy, `kq_rows_fn`, the null-guard, the bake arm in `active_kq_layout_mr`.
- `dasllama_gemm_gen.das`: `register_llvm_code_generator` for `<fmt>_gemv` and `<fmt>_tile`.
  Until the emitter arm exists, register generators that return `false` - a declined generator
  IS the framework's fallback, so the stubs are the cheapest way to have a real family; the
  `[tune]` family must exist before the emitter does because the loader's kq dispatch, the layout
  companion and the tuner's completeness check all resolve it by name.
- `harness/gen_tune_probe.das`: `pack_kq_disk_block`, `repack_kq_grp_fmt`, `build_kq_fixture`
  (block size, transcode, the yref oracle), `kq_tile_variants`, `kq_gemv_variants_by_suffix`,
  `kq_layout_mrs`, `run_kq_tile`'s `packed`, `kq_tile_entry`, the family list. Without this
  the scope's completeness check demands a sidecar entry the tuner never writes, and every
  start re-tunes. Keep a stub family out of the probe's TEST list until its emitter arm lands:
  the shared layout companion generates for any perm `perm_declines` admits, so a declined tile
  runs the reference body over planes repacked at the companion's `mr` and reads `maxdiff nan`
  (the tune-mode list is fine - the race gates each perm and mints "reference").
- `tests/test_kquant.das`: add the format to every gate's ladder and to the `[4, 5, 6, 40]`
  lists (`followup_general.md` item 132). The gates: dot vs the fp64 plane-dequant oracle,
  portable GEMV rows, repack at mr 4/8/16 (dots and row dequants bit-exact), 4-token tile vs
  per-token GEMVs, groupn (disk + grp slices), batch groupn.

Gate: `test_kqformat` + `test_kquant` under `-jit` (the stubs decline, so their reference bodies
run either way; every dasLLAMA test run is a `-jit` run). A new `[tune]` family re-tunes every
application on its first start unless the checked-in class profiles cover it, so land the family
in `performance/defaults/<class>.tune-defaults.json` (re-export on the reference boxes) in the
same arc, and never read a first end-to-end wall time as a load-time regression.

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
   The sub-scale granularity, not the family, decides the accumulator split: per-16 strips
   (k6, k3, k2, the iq2 family) force split lo/hi accumulators and two strip vector loads, a
   per-32 strip format rides the shared single `SExt` load and needs no new fold.
2. `emit_one_block`: `te.kq == 44` routes to the new body ahead of the `te.kq != 0` arm.
3. `setup_tile_emit(te, gc, p, needMx4 = fmt == 44)`: the mx4 primitive wiring is reused as
   is; the LUT bake picks `iq4nl_lut()` over the e2m1 table on `te.kq == 44`.
4. The two stubs become `=> kq_gemv_gen_impl(gc, 44)` / `kq_tile_gen_impl(gc, 44)`.
5. No `perm_declines` change: the tbl1 rail already sits on every sdot perm (it is the mx4
   companion's), and pshufb is implied by the x64 tiers.

After ANY emitter change, `rm -rf .jitted_scripts` (or bump `LLVM_JIT_CODEGEN_VERSION`) before
trusting a probe or bench: the registered generators run at codegen time and their bodies do not
fold into the cached DLL's hash, so a cache hit executes the OLD stamps with no signal - the
same numbers across every edit, the `DLL cache hit` line the only tell. A sidecar minted while the
stubs declined pins the family to `"reference"` and the emitter landing invalidates nothing;
re-mint with `-- --tune-only <fmt>q8_tile_gen` on the app (each app roots its own sidecar) or
delete the sidecar (`followup_general.md` item 134).

Gates, in order: `DAS_TUNE_MODE=test bin/Release/daslang.exe -jit harness/gen_tune_probe.das`
(the family must be in BOTH of the probe's lists) gates every perm of the family's grid against
the scalar oracle - a stamped perm reports its layout companion's `mr` (8 on this box) and a
fast-math-sized maxdiff (1.5e-5), a declined one `mr=4` and maxdiff 0 (that is the reference body
answering, not the emitter); then `test_kquant` under `-jit` (the tile-vs-GEMV gate is bit-exact
by construction); then the end-to-end run.

## 6. Vulkan - `dasllama_vulkan_classes.das`, `dasllama_vulkan_common.das`

The tier reads the CPU planes verbatim (`stack_plane_bytes` -> `arena_block_bytes` ->
`kq_qsb(vk_kq_schema_id(fmt))` x `kq_dev_ssb(fmt)` - 20 B for a decoded row, 8 B for the codebook
formats' two-word row the gather packs), so a format whose plane pair already has the 20 B
decoded scale row needs no upload work - only the id bridge and the kernels. IQ4_XS took:

1. `vk_kq_schema_id` (`dasllama_vulkan_common.das`): the `int(KqFmt)` -> kernel-id arm
   (`6 -> 44`). This is the third id space at its Vulkan seam; without the arm the arena plan
   panics on the first iq4xs stack.
2. `KqGemvIq4xs : KqGemvBase` - `def override blk_contrib`: the q40 nibble tiling
   (`wq4[wsb * 8 + blk]`), each nibble word decoded through `iq4_word` into SIGNED lanes for
   `sdot4` (OpSDot, signed x signed - the block-sum trick of q40/k4 does not apply and is not
   needed), scale `d * sc` with `sc` decoded by `iq4xs_sc` off the two-word device row
   (`scales_h` above d in word 0, the `scales_l` nibbles in word 1). A codebook is packed into
   four `uint4` words and selected with a dynamic vector index plus a byte shift
   (`(tbl[q >> 2] >> ((q & 3) * 8)) & 0xFF`): a `fixed_array` local indexed per nibble lowers
   to Function storage the driver spills, and decodes at a third of the speed
   (`followup_general.md` item 136).
3. `KqBatchIq4xs : KqBatchBase` - `stage_w` decodes the staged words through `iq4_word`
   (k4's staging otherwise), `stage_ws` fills ONE plane with `d * sc`, `blk_fma` is
   `xscl * ws * idot` (q40's without the `- 8 * bsum`). A class child is declared after its
   parent in the file - the class rail resolves parents in declaration order and reports
   "parent structure not found" otherwise.
4. Ladders: `kq_batch_cls_ensure` / `kq_batch_cls_enc_for` / `gemv_cls_ensure` /
   `gemv_cls_enc` gain an arm; `gemv_cls_set`'s four-way `||` became `kq_sb(fmt)`.
5. The decode GEMV's lanes per row (`dasllama/dasllama_vulkan_classes.das`): join
   `gemv_grid_fmt` where the format's decode gathers from a codebook or a grid, and take the
   lanes from `gemv_lanes_per_row`, which every decode site reaches through `gemv_enc`. Which
   side of that rule the format sits on - grid or k-lattice - is decided by the
   `harness/vk_gemv_probe.das <n> <d>` sweep, its three lane splits per format (0 = the whole
   subgroup, 16, 8) over the row lengths the families serve; the rule itself is
   `ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2ah (`kq-gemv-lanes`).
6. Tests: `tests/_vkd_oracles.das` `kq_cls_ref` arm (the class-on-CPU oracle - the replay never
   runs the kernel head, so any workgroup state the kernel stages, a grid table among them, is
   filled there or the device compares against zeros), the family cells in
   `tests/test_vulkan_kernels.das`, and - because the codebook pack is new bit-math that a
   class-vs-device compare cannot see (both sides run the same `iq4_word`) - a float dequant
   straight off the plane bytes (`iq4xs_gemv_float_oracle`) that the class oracle must match.
   The cells fill planes with pseudo-random words, so a scale or strip byte takes values real
   quantized data never produces: the class, the cm2 decode, the float oracle and the f16 oracle
   read every field with the same signedness (the grid formats read strips unsigned on both
   rails), or the fork shows one tier at a time as each side is aligned. A family cell whose
   `enc` ladder lacks the new arm dispatches the else format's pipeline over the new planes and
   reads byte-stable across fix rounds.

A grid format adds one more: its table joins the family's grid buffer (`kq_grid_dev` - a
`KQ_GRID_<FMT>` word offset, `KQ_GRID_WORDS` / `KQ_GRID_BYTES` grown, the accessor called
once per word into the host image) and the GEMV's `run` stages `gridb[KQ_GRID_<FMT> + idx]`
into its `@workgroup` table - never the `*_grid_word` accessor, which the batch and cm2 tiles
keep (`REVIEW_GPU_VULKAN.md`, `ARCHITECTURE_GPU_VULKAN.md` sec.2.2ab).

### 6b. The cm2 prefill tile - a decode method on the template

On an NV_coopmat2 device the f16 feed serves every kq format through ONE tile template
(`KqCm2BatchT`): a new format is a format template authoring `[spirv_decode] def decode` over
the DEVICE forms (quants as the gather lays them out - k4/k5 re-paired k/k+16, q40/iq4xs/k3
verbatim; scales the `kq_dev_ssb(fmt)` row - 20 B decoded, or the codebook formats' two words) in
PAIR form - every shared read derived from `e & ~1u`, both elements computed, the element selected
last (`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2k; iq2xxs's `decode` is the model) - plus four width
stamps (the l, m and s columns and the expert schedule's e column - the m column at the format's k
step; each names its `BN`, `STILE`, the k step `BK` where it is not the template's 64 with the
unroll `UNR` that keeps the unrolled block at one superblock (`override UNR = 8u` beside
`override BK = 32u`), and the `AT`/`BT`/`ACC`/`ACCW` tile types of that depth - copy k4's for a
K-quant or LUT decode, iq2xxs's 32-deep s and e stamps for a grid-codebook decode, and settle the k
step on a whole-model MoE row, not the uniform probe alone), and arms in the `cm2_cls_ensure/set/enc`
and `cm2e_cls_*` ladders. `pf_f16_feed` admits every `kq_sb` format on a cm2 device the moment the
enum member exists, so the ladder arms are due in the same change: `cm2_cls_ensure` ends in a verify
that names a kq format falling through, because without it the prefill served the q8 tiles over
the new planes - garbage text at full speed. A codebook format raises the `IQLUT` axis - a gated
`@workgroup` f16 table staged ahead of the tile loop (the reference build's `init_iq_shmem` form);
never select codes out of a register vector per element inside a decode callback. A format whose
sub-block scale takes an unpack per element raises the `SCACHE` axis and reads its sub-block's
premultiplied pair from `sc_cache` in the decode, as k4 and k5 (the five-word K-quant scale row)
and iq4xs (`SCIQ4`, its two-word row) do; a third scale-row shape adds its fill arm to `sc_fill`
(`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2k), and a scale-caching stamp keeps `BLKW` at
`BK x UNR` so the cache refills once per unrolled block - the module gate holds it on the engine's
stamps and on the probe's twins in `harness/vk_gemm_probe.das` alike. The four-wide decode twin is
the format's own: a second `[spirv_decode] def decode_v4` returning `half4` under
`override DECV4 = true`, computing the four consecutive elements in the scalar `decode`'s
operation order and sharing what they share (two 16-bit lanes and one scale extraction for a
K-quant, one grid word and its four sign bits for a grid format - every kq format today authors
one). A format may start on the template's `DECVEC` axis, the twin the emitter synthesizes over
`decode`, but the synthesized twin repeats the scalar body four times and lost to the scalar
callback on the grid formats. Run `harness/vk_gemm_probe.das -- cm2:<fmt>` (both twin arms in one
process): a format whose four-wide row loses to its scalar row ships a twin that wins or turns
both axes off. Gate: a device-form CPU oracle (`<fmt>f16_gemm_oracle`) and an l/m/s/e cell in
`tests/test_vulkan_kernels.das`.

The KHR instantiation adds one method (`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2ae, its first
paragraph): on the format template a `def override khr_stage16(blk, e0, sbase : uint) : void`
under `static_if (KHR)` that writes the weight row's 16 values `e0 .. e0 + 16` of block `blk`
into `khr_ao[sbase .. sbase + 8)` as f16 pairs, reading the plane as words - `wq4[...]` (`uint4`,
one or two loads where the run's bytes are 16-byte aligned: the nibble and byte formats) or
`wqw[...]` (`uint`, the byte-granular grid formats) - and the scale words once; the base declares
it `abstract`, so a stamp without one fails to compile. The k4 override is the pattern; the
decode methods' index math is the same, only read sixteen at a time. Then `<Fmt>KhrBatch :
<Fmt>Cm2T` with `override KHR = true`, `override BN = 128u`, the four cm2 typedefs the uncalled
tensor body still names (`AT`, `BT`, `ACC`, `ACCW` - copy k4's), a
`[vk_dispatch(name = "kq_batch_<fmt>_khr_cls", ...)]`, an arm in each of `khr_cls_ensure/set/enc`
(`dasllama_vulkan_prefill.das`), and the format's kernel cell runs its KHR arm (`ARM_KHR`, tile
128) wherever the device has KHR coopmat at subgroup 32 - on the 5060 Ti the same run covers the
cm2 l/m/s/e tiles and the KHR tile. No new oracle: the `<fmt>f16_gemm_oracle` already holds the KHR
arm, whose f16 accumulation sits inside the cell's 2e-2 relative bar.

## 7. Metal - `dasllama_metal_kernels.das`, `_common`, `_prefill`, `_shapes`, `dasllama_layout.das`

The tier compiles MSL and runs only on a Mac. It is NOT a plane-verbatim consumer like Vulkan: a
Metal-served model is the `metal_blob` flavor, whose kq scale planes are rebaked into device
forms (k4/k5: the 16 B disk block, k6: the split form) and whose CPU never reads a plane again. So
a format on Metal starts with a device-form decision, and every ladder below keys on `KqFmt` with
an `else` that means "k5" or "k6" - without its own arm a new format runs a wrong-layout kernel
silently, which is why the tier's gate (`kq_fmt_gpu_supported`) is closed by default. IQ4_XS took:

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
3. **Kernels** (`dasllama_metal_kernels.das`), each a copy of the k4/k6 shape with the per-32
   pairing (lane = one 32-block of every 4th superblock: 4 uints, 16 lo + 16 hi weights) and
   `d * sc * LUT[q]` with no min term: `MetalKqGemvIq4xs` (decode GEMV), `MetalKqMvIq4xsT`
   B2/B4 + `MetalKqMvB8Iq4xs` (small batch), and an `IQ4XS` arm in `MetalKqMulMmK45T`
   (prefill mul_mm; `MetalKqMulMmIq4xs` stamps it - the template's `static_if` has no `elif`,
   so a new arm re-nests the k4/k5 arm one level deeper, `followup_general.md` item 135). PSO
   globals + `compile_pso` + `release_pso` lines in the three ladders; every `g_pso_*` global is
   declared by hand in `dasllama_metal_common.das`, since `[metal_dispatch(pso = ...)]` only
   names it.
4. **Ladders:** `enc_kq_gemv`, `enc_kq_mvb`, `enc_kq_gemm_mm_b` (kernels), `pf_enc_kq_site_mm`
   (the base mul_mm only - no tensor / tall / dev-W twins: those are the M5 kernel pass),
   `pf_devw_panel_kq` (returns false - its `dq` pick would otherwise be k5's), `pf_enc_kq_gemv`
   (prefill), `moe_site_ok` + the `sb1/2/3` predicates (shapes), and last the gate.
5. **Tests:** `dequant_iq4xs_plane_superblock_at` (`dasllama_convert.das`, the split-layout
   twin the CPU row now calls), fixtures at fmt 44 in `tests/_metal_kernel_common.das`, the
   ladders + calls in `test_metal_gemv_kernels.das` (GEMV, B2/B4/B8) and
   `test_metal_gemm_kernels.das` (mul_mm, base form) - the ladders are nested ternaries whose
   `else` means k6, so a format missing from any one of them tests k6's kernel under the new
   tag (`followup_general.md` item 132). Gate proof: a one-byte codebook mutation turns the
   format's cells red.

A Metal kernel lever is judged by a kernel-level probe, never by a tg128 row: an end-to-end row
on the M1 carries about 8 t/s of noise, while the dispatch-loop probe (50 dispatches per encoder,
best of three encoders, GB/s off plane bytes) resolves 3% in seconds. On the Metal tier the app
sets `set_metal_mode(MetalMode.required)` BEFORE `load_model_`, then `convert_model_to_metal_blob`;
`run.das` itself declares CPU intent, and `DASLLAMA_GPU=1` on a Mac build with dasVulkan arms
Vulkan on MoltenVK, not Metal.

## 8. End to end

Every correctness-loop run (probe tests, parity, `run.das` spot checks) is the fast stack by
default - `daslang.exe -jit <app>.das ...` installs the AST module cache and the split-module JIT
unasked. A bench that must see one-unit codegen passes `--jit-split-modules=0` after the `--`
separator; a records row measures a released `-exe`, which always is one unit; `-no-module-cache`
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
app run comes only after a spelling wins there.

Pick the vehicle by census, not by name: an ftype is a MIX, and a natural download can put an
unsupported sibling on `attn_k`/`attn_q` so the file cannot load end to end. `harness/gguf_dump.das`
reads the tensor type table in seconds; pick a file whose mix is the supported set plus the new
format, or mint one from a Q8_0 with `llama-quantize --allow-requantize --imatrix <imatrix>
--tensor-type attn_k=<fmt> --tensor-type attn_q=<fmt> <Q8_0> <out> <FTYPE>`. A vehicle the
resident driver declines proves the kernels and not the tier; `resident_unserved_features` prints
the reason. Then `examples/dasLLAMA/run.das` against the reference build's `simple_ids` oracle for
the same prompt: the criterion is the margin form (`harness/oracle/simple_ids_margin.cpp` prints
per-step logit margins), not id equality - a fork whose top-2 IS our token at a sub-logit margin
is the expected class, and a stamped fold landing moves the flip point without changing it. When a
walk touches any region or stream ladder, run a MIXED vehicle (the new format beside a K-quant),
not a pure one: a region keyed on the wrong id space corrupts only the loaded model, and every
serial gate still passes. Then `test_model_image` for the image round trip, then the records rig.
