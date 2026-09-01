# Technique memo: llama.cpp's cooperative-matrix-2 prefill GEMM vs ours

Read-only research. Trees: `D:\Work\llama.cpp` at `6c84c7d5d` (2026-08-27) and
`D:\Work\daScript-iquant`. Nothing in either tree was modified.

Target box for every number below: RTX 5060 Ti (Blackwell GB206, 36 SMs), the box
`D:\Work\daScript-iquant\modules\dasLLAMA\ARCHITECTURE_GPU_VULKAN.md:49` names.

One environment fact that shapes the whole comparison: the local llama.cpp Vulkan build
reports `GL_NV_cooperative_matrix_decode_vector not supported by glslc`
(`C:\Users\Boris\.claude\jobs\7f9893d9\tmp\tbo_build_vk.log`). llama.cpp therefore runs the
scalar (one element per callback) decode path here, the same as ours. The comparison is
apples to apples today. It stops being so the moment that glslc is upgraded — see delta 7.

---

## 1. llama.cpp's cm2 GEMM, in ten facts

**F1 — One shader, six pipelines per quant type, three tile geometries.**
Geometry arrives as spec constants `{BLOCK_SIZE, BM, BN, BK, enable_smaller_matrices}`
(`D:\Work\llama.cpp\ggml\src\ggml-vulkan\vulkan-shaders\mul_mm_cm2.comp:31-39`). On coopmat2 the
values are large `{256, 128, 256, 64, 1}`, medium `{256, 128, 128, 64, 1}`, small
`{256, 32, 64, 128, 0}` — and they are byte-identical for legacy quants and for K-quants
(`D:\Work\llama.cpp\ggml\src\ggml-vulkan\ggml-vulkan.cpp:4263-4277`). Each is stamped twice,
unaligned and aligned, giving six (`ggml-vulkan.cpp:4664-4671`). There is **no per-format tile
tuning** on the cm2 path.

**F2 — Operands: f16 times f16 into an f16 accumulator, stored as f32.**
`mat_a` is `BM x BK` of the quantized weight, `mat_b` is `BK x BN` of the activation, both
`MAT_TYPE = FLOAT_TYPE = float16_t` on coopmat2 (`mul_mm_cm2.comp:98-102`,
`vulkan-shaders-gen.cpp:492-529`). `coopMatMulAdd` accumulates into `ACC_TYPE`, which is
`float16_t` at `GGML_PREC_DEFAULT` and `float` only at `GGML_PREC_F32`
(`vulkan-shaders-gen.cpp:471-475`; the pick is `ggml-vulkan.cpp:7818-7820`). The device is only
admitted to the cm2 path if it advertises fp16 A and B with both fp16 and fp32 accumulators at
workgroup scope and flexible dimensions (`ggml-vulkan.cpp:6806-6838, 6863-6869`). Result is
converted to `D_TYPE = float` for the store (`mul_mm_cm2.comp:493-495`).

**F3 — There is no integer activation path on coopmat2.**
An f32 `src1` on a coopmat2 device is unconditionally marked non-contiguous
(`ggml-vulkan.cpp:9276-9278`), which forces `quantize_y` false (`ggml-vulkan.cpp:9285`), and the
q8_1 integer-dot `mul_mmq.comp` shader is never even compiled for coopmat2
(`vulkan-shaders-gen.cpp:626`). Activations are always f16 here.

**F4 — The f32-to-f16 activation conversion is one dispatch, cached across sibling matmuls.**
It runs through `ggml_vk_cpy_to_contiguous` into the `prealloc_y` scratch, but only when the
last conversion did not already produce the same tensor with the same pipeline
(`ggml-vulkan.cpp:9441-9453`, guarded by `prealloc_y_last_pipeline_used` /
`prealloc_y_last_tensor_used`). Q, K and V therefore pay one conversion between them.

**F5 — Weights are decoded inside the driver's block load; nothing is pre-dequantized.**
`coopMatLoadTensorNV(mat_a, data_a, pos_a, sliceTensorLayoutNV(...), dequantFuncA)`
(`mul_mm_cm2.comp:381, 468`). The tensor layout carries the quant block size
(`setTensorLayoutBlockSizeNV(tensorLayoutA, 1, QUANT_K)`, `mul_mm_cm2.comp:315-318`) so the
driver resolves the block pointer and hands the callback a `buffer_reference` to it. The weight
stays in ggml's on-disk block layout: no repack, no staging pass, no extra VRAM.

**F6 — Almost nothing is staged in shared memory, and the one exception is Q4_K/Q5_K scales.**
`mul_mm_cm2.comp` declares only `row_ids` and `ballots_sh`, both under `MUL_MAT_ID`
(`mul_mm_cm2.comp:108, 116`) — contrast the non-cm2 shader, which stages both operands
(`mul_mm.comp:130-131`). The exceptions are the iq codebooks
(`types.glsl:918, 1031, 1201, 1501, 1573, 1683, 1813`, copied by `init_iq_shmem`) and a
**Q4_K/Q5_K-only scale hoist**: `fetch_scalesQ4_K` reads the whole 16-byte scale head of a
superblock as ONE `uvec4` (`dequant_funcs_cm2.glsl:449, 461`, via the
`buffer_reference_align = 16` `block_q4_K_packed128` alias at `:429-431`), and
`store_scalesQ4_K` unpacks it into 8 ready-multiplied `(d, m)` pairs in
`shAscales[8 * (BM+2)]` (`:442-444, 484-523`). Every element decode then reads one `vec2` with
no shifting at all (`:537-540`). The comment states the intent outright: "we decode a tile's
worth of scales into shared memory and then process the whole tile using those scales"
(`:435-440`).

**F7 — Two tensor loads, one multiply-add, zero barriers per k-step.**
The inner body is exactly `coopMatLoadTensorNV(mat_a, ...)`, `coopMatLoadTensorNV(mat_b, ...)`,
`sum = coopMatMulAdd(mat_a, mat_b, sum)` (`mul_mm_cm2.comp:464-473`). The only barriers in the
k loop are the two inside `store_scalesQ4_K` (`dequant_funcs_cm2.glsl:486, 522`), which fire once
per `QUANT_K = 256` elements of k — that is once per four `BK = 64` steps — and only for
Q4_K/Q5_K. Every other format runs the whole k loop barrier-free. The scale fetch and store are
explicitly pipelined: the next block's scales are fetched while the current block's are being
consumed (`mul_mm_cm2.comp:363-374`).

**F8 — A clamp-free fast path, entered by construction rather than by luck.**
The fast path requires a whole M tile, a whole N tile against `padded_N`, `start_k % 256 == 0`,
`end_k % BK == 0` and `stride_b % 8 == 0` (`mul_mm_cm2.comp:344-349`). Strides are then masked
`&= ~7` purely to make 16-byte alignment provable to the driver's address analysis
(`:299-306, 350-355`). Crucially, the N side is **padded in the buffer**:
`padded_n = ROUNDUP_POW2(ne11, pipeline->wg_denoms[1])` (`ggml-vulkan.cpp:9319`), so even the last
token tile takes the unclamped path. The unaligned fallback uses clamp-Constant layouts and is
marked `[[dont_unroll]]` (`mul_mm_cm2.comp:607`).

**F9 — The tail N tile gets a narrower accumulator.**
`enable_smaller_matrices` makes the final column tile allocate `BN/2` or `BN/4` accumulators and
loop over that width (`mul_mm_cm2.comp:36-38, 367-409, 410-452`), so a partial token column does
not carry a full-width accumulator through the whole k loop.

**F10 — Split-k with a separate vectorised reduce.**
Split fires when `k >= 2048` and the tile grid fills less than half the SMs; the factor is
`shader_core_count / tiles`, capped at 8, with the chunk rounded to 256 and splits shed if the
last chunk would be empty (`ggml-vulkan.cpp:8795-8826`). Partials land in `prealloc_split_k` and a
256-thread `vec4` shader sums them (`ggml-vulkan.cpp:8909-8925`,
`mul_mat_split_k_reduce.comp:17-47`).

**Worked pick for a 1B prefill shape on this box** (M = 2048 weight rows, N = 512 tokens,
K = 2048, 36 SMs): `tiles_m = 16 * 4 = 64 > 36`, so `prefer_large` is true and the aligned large
pipeline is chosen — BM 128, BN 256, BK 64, 256 invocations
(`ggml-vulkan.cpp:8839-8863`, `9309-9312`). Split-k: 32 tiles, neither `<= 18` nor `<= 24`, so
`split_k = 1` (`ggml-vulkan.cpp:8795-8807`). Grid is 16 by 2 by 1 workgroups
(`ggml-vulkan.cpp:8896`, `8221-8229`).

Batch and MoE, for completeness: plain batches ride `gl_WorkGroupID.z` with `broadcast2`/
`broadcast3` (`mul_mm_cm2.comp:253-261`); MoE builds a shared `row_ids` table with a
subgroup-ballot prefix scan and stores through a per-element callback
(`mul_mm_cm2.comp:163-227, 150-161`).

---

## 2. Ours, in the same terms

**O1 — Same three tile geometries, same workgroup size, same accumulator type.**
`KqCm2BatchT::run` is `[spirv_kernel(local_size_x = 256)]`
(`D:\Work\daScript-iquant\modules\dasLLAMA\dasllama\dasllama_vulkan_classes.das:4825`). A is
`coopmatWgA_f16_128x64` (`:4888`), so BM 128 and BK 64 are fixed; BN is a template constant of
256, 128 or 32 with matching B and accumulator types (`:4809`, `:5336-5361` for q8 and the same
four typedefs repeated per format through `:5713`). The accumulator is
`coopmatWgAcc_f16_128xBN` — f16, like llama.cpp's default — clamped to the f16 range and widened
to f32 only for the store (`:4923-4926`).

**O2 — Same tile pick, arrived at independently and landing in the same place.**
`cm2_tile_cols` compares occupied against allocated wave slots cross-multiplied, ties to large,
with `cnt <= 64` forcing small and `cnt <= 128` forcing medium
(`D:\Work\daScript-iquant\modules\dasLLAMA\dasllama\dasllama_vulkan_common.das:129-151`). For
d = 2048, cnt = 512, 36 cores it returns 256, the same large tile llama.cpp picks.

**O3 — Same split-k rule, near-identical constants.**
`cm2_split_k` gates on `n >= 2048`, `d >= 128`, `cnt >= tc`, computes `cores / tiles` capped at
8, rounds the chunk to 256 and sheds stranded splits
(`dasllama_vulkan_common.das:155-179`). The in-kernel plane decomposition is
`dasllama_vulkan_classes.das:4832-4846`; the reduce is a separate class
(`:5722-5751`), encoded at `dasllama_vulkan_prefill.das:355-366`.

**O4 — Same decode-in-load mechanism.**
`coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, 64u, self.decode)`
(`dasllama_vulkan_classes.das:4911`), with the block size on the layout
(`tensorLayoutSetBlockSize(fla, 1u, BLKW)`, `:4896`). Weights are never materialised in f16.

**O5 — Same fast/edge split and the same stride-masking trick.**
Fast path when `m0 + 128 <= pa.d`, the token column is whole or the small tile is stamped, and
`pa.n % 64 == 0` (`:4893`); strides masked `& ~7u` on the B and output layouts (`:4902, 4906`).
Edge path uses clamp-Constant layouts (`:4950-5005`).

**O6 — Same k-loop shape, one degree more unrolling, zero barriers.**
Two loads and one `coopmatMulAdd` per 64-wide k step, the outer loop unrolled 8 deep for 512 k
per iteration with a scalar drain (`:4907-4922`). The kernel contains exactly one `barrier()`,
before the loop, to publish `wg_blk0` and any staged codebook (`:4887`). llama.cpp unrolls 4 deep
at `QUANT_K = 256` (`mul_mm_cm2.comp:341-342`).

**O7 — The activation f16 conversion is likewise one dispatch per sibling group.**
`f16cvt_cls` feeds q/k/v (`dasllama_vulkan_prefill.das:518-524`) and gate/up (`:562-568`);
`actf16_cls` fuses the SwiGLU epilogue with the conversion feeding down (`:569-576`). All of it
is recorded into the measured command buffer. This is at least as good as llama.cpp's caching,
and the fused activation is strictly better.

**O8 — Difference: the weight scale plane is a second SSBO, addressed by hand, per element.**
`wq` (quants) is binding 0 and `ws` (scales) is binding 1
(`dasllama_vulkan_classes.das:4804-4805`). Every decode callback computes its own scale address
`(wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u` and then extracts the sub-block scale with a 32-bit
load and a **runtime-variable shift** — per decoded element. Q4_K is the worst case, three
separate `ws` dwords at a 20-byte stride plus three variable shifts
(`:5034-5038`). Q6_K (`:5058-5062`), Q3_K (`:5143-5147`), Q2_K (`:5164-5167`), IQ4_XS
(`:5122-5125`), IQ3_S (`:5190-5193`), IQ3_XXS (`:5223-5226`), IQ2_S (`:5269-5273`) and IQ2_XS
(`:5295-5299`) all carry the same shape. llama.cpp reads every scale from the same block the
driver already resolved, and for Q4_K/Q5_K does the extraction eight times per 256 elements
instead of 256 times (F6).

**O9 — Difference: no shared-memory scale hoist for any format.**
The only workgroup memory declared is `wg_blk0` (4 bytes) plus one gated iq codebook of 32 bytes
to 8 KB (`dasllama_vulkan_classes.das:4808-4823`). There is no `shAscales` equivalent.

**O10 — Difference: the codebooks are computed per workgroup, not copied.**
`iq3s_gridc[gi] = iq3s_grid_word(int(gi))` and its siblings synthesise the table with arithmetic
at kernel entry (`:4861-4886`), where llama.cpp copies from a constant array
(`types.glsl:1204-1213`).

**O11 — Difference: a per-workgroup prologue that llama.cpp does not have.**
Each workgroup reads a region record from a `sched` SSBO — five scalar loads
(`dasllama_vulkan_classes.das:3972-3976`) — broadcasts `wg_blk0` through workgroup memory
(`:4849-4851`) and barriers (`:4887`). llama.cpp derives everything from `gl_WorkGroupID` with no
loads and no leading barrier. Ours buys MoE region lists with this; llama.cpp pays for the same
capability with a separate `MUL_MAT_ID` shader.

**O12 — Difference: no N padding and no narrow-tail accumulator.**
The activation plane is sized to `cnt`, not to a tile multiple, for the large and medium tiles
(`:4901`), so a partial last token column drops to the fully clamped edge path (`:4893`). Only
the small tile rounds its dimension up to 32 and keeps the unclamped load. There is no
`enable_smaller_matrices` analogue. At exactly 512 tokens with BN 256 this costs nothing; at
500 tokens it costs the last tile.

**O13 — Grid: one-dimensional.**
`vkCmdDispatch(raw, groups, 1u, 1u)` (`dasllama_vulkan_common.das:2469-2478`) with
`groups = ceil(rows/BN) * ceil(d/128) * nsplit` (`dasllama_vulkan_prefill.das:1089-1106`). Push
constants are 16 bytes, `{n, d, map_off, ksplit}` (`dasllama_vulkan_classes.das:2326-2331`);
everything else rides the SSBO region record. llama.cpp pushes a 17-word block with the full
geometry (`mul_mm_cm2.comp:41-70`).

**O14 — The measured ratios are NOT a uniform band, and the followup says otherwise.**
`followup_vulkan.md:540-546` (entry 34) states "~0.67-0.70x" and "every sb-format cm2 tile lands
in the same band" and "the gap is the shared batch-GEMM tier, not any one format's decode".
The per-format tables in
`D:\Work\daScript-iquant\modules\dasLLAMA\HOW_TO_ADD_A_FORMAT.md`, all 5060 Ti against llama.cpp
b10660 Vulkan at `-ngl 99` on 1B-class vehicles, say:

| format | pp512 ours / theirs | ratio | cite |
|---|---|---|---|
| IQ4_XS | 15334 / 17060 | 0.90x | `HOW_TO_ADD_A_FORMAT.md:1132` |
| Q2_K | 14544.0 / 16752.7 | 0.87x | `:774` |
| Q3_K | 14031 / 17509 | 0.80x | `:1088` |
| IQ4_NL | 15027.4 / 19177.8 | 0.78x | `:831` |
| IQ2_XXS | — | 0.78x | `:554` |
| IQ2_XS | 14023.3 / 18320.0 | 0.77x | `:606` |
| IQ3_S | 12539.6 / 17865 | 0.70x | `:1020` |
| IQ2_S | 12099.3 / 17377.5 | 0.70x | `:702` |
| IQ3_XXS | 12225.7 / 17807.7 | 0.69x | `:918` |
| Q4_K (the control) | — | 0.67x | `:987` |

That is a 1.34x spread, not a band. Entry 34 was written on 2026-08-30
(`followup_vulkan.md:543`) when only k4 and iq3s had Vulkan rows; six formats landed after it
and none updated it. The document contradicts itself at `HOW_TO_ADD_A_FORMAT.md:606-607`
("0.77x - above the 0.70x tier class") and `:753-754` ("0.87x - above the 0.69-0.78 tier
class").

**O15 — The "shared tier" diagnosis is refuted by our own Q8 rows.**
Q8_0 on the same tile, the same chain, the same box measures 3B pp 7737.2 = 100.6% of the
reference exe and tinyllama pp 20986 = about 103.5% (`followup_vulkan.md:211-213`). If the deficit
were the shared batch-GEMM tier, Q8 would show it. And the kernel-rate probes already point at
the decode: Q8 cm2 clocks 62.2/64.7 TF/s on gate/down (`followup_vulkan.md:176-178`) while Q4_K
clocks 35.8-38.2 TF/s, "about 70% of Q8-cm2's rate — the nibble+scale extraction"
(`followup_vulkan.md:223-224`); Q6_K sat at 9.3-13.4 TF/s until one byte4 dynamic select was
respelled, after which it hit 32.9 TF/s (`followup_vulkan.md:229-234`).

The honest caveat is that these Q8 rows are 3B-shape and the per-format rows are 1B-shape, so
shape and decode are confounded in the evidence that exists. Nobody has run them apart.

---

## 3. The deltas, ranked by estimated pp512 payoff on a 1B/3B shape

Shape assumed throughout: dim 2048-3072, hidden 8192, 512 tokens, so the large tile
(BM 128, BN 256, BK 64) and split-k off on the wide GEMMs, split-k 2 on the narrow k/v
projections.

### Delta 1 — Hoist the per-(row, sub-block) scale into workgroup memory, llama.cpp's `shAscales` form. Estimated payoff: 10-20% on Q4_K/Q5_K, less on the rest.

Ours re-derives the scale for every decoded element: a 32-bit load from a second SSBO plus a
runtime-variable shift plus a mask plus a float convert, three times over for Q4_K
(`dasllama_vulkan_classes.das:5034-5038`). llama.cpp does that arithmetic once per (row,
sub-block) — 8 times per 256 elements per row — into `shAscales`, and the element decode is then
a single `vec2` read with no shifting (`dequant_funcs_cm2.glsl:484-523, 537-540`). That is a
32-fold reduction in scale-extraction ALU on the format that is currently our worst.

This is the strongest hypothesis because Q4_K is simultaneously (a) our worst ratio at 0.67x
and (b) the only format llama.cpp gives a special optimisation to on the cm2 path. The
coincidence is unlikely.

Cost to try: 8 sub-blocks times (BM + 2) times a `vec2` = 8320 bytes of workgroup memory
(`dequant_funcs_cm2.glsl:442-444`), which llama.cpp already spends without occupancy trouble on
the same tile. The fetch/store pipelining and the two barriers per 256 k
(`mul_mm_cm2.comp:363-374`, `dequant_funcs_cm2.glsl:486, 522`) come with it.

**Measurement that proves or kills it.** `harness/vk_gemm_probe.das` already has the instrument:
the `k6x` arm varies one decode callback against spellings of itself with the tile held fixed,
and its `flat` variant is "current compose w/o scale reads"
(`D:\Work\daScript-iquant\modules\dasLLAMA\harness\vk_gemm_probe.das:466-469`). Run `k6x` at the
q6k gate and down shapes it already pins (9728x2560x512 and 2560x9728x512,
`vk_gemm_probe.das:1287-1288`) and read the `nil` to `flat` to shipped ladder. The `flat`-to-
shipped gap is the entire scale-plane cost in TF/s. If it is under 5%, delta 1 is dead. Then add
a k4 twin of that bisect — the file has no k4 scale-plane arm today, only a k4 rate arm
(`vk_gemm_probe.das:357`).
Caveat: arm `DAS_TUNE_MANIFEST` or the checked-in
`performance/last_known_good_sidecar.json` first, or a bare probe run re-execs into a full
retune (`ARCHITECTURE_MEASUREMENT.md:53-56`).

### Delta 2 — Fold the scale plane back into the quant block, so the decode touches one buffer. Estimated payoff: 5-15%, uniform across formats.

llama.cpp's callback gets a `buffer_reference` the driver already resolved and reads d, dmin and
all 12 scale bytes as one 16-byte aligned `uvec4` (`dequant_funcs_cm2.glsl:429-431, 461`). Ours
carries two independent address streams and a 20-byte scale stride that is not 16-byte aligned
(`dasllama_vulkan_classes.das:4804-4805, 5034`). Two streams double the distinct addresses in
flight per tile and halve the chance the block-load path coalesces them.

This one is uniform across formats, which is what would explain the part of the deficit that
IQ4_XS still shows at its otherwise-matched decode-load count.

**Measurement.** The probe's `cm2x` arm already carries a `push` variant that moves the scale
base off the workgroup read onto a push constant
(`vk_gemm_probe.das:215-216`), and a `flat` variant with no scale read at all
(`:72-74`). Those bracket the address-arithmetic half. The buffer-count half needs a new arm: a
k4 fixture whose `ws` contents are interleaved into the `wq` plane and read through the same
block pointer. Compare TF/s on one shape (d 2048, K 2048, cnt 512, large tile, split off via
`DASLLAMA_CM2_SPLITK=1`). Kill it if the interleaved arm is within 3%.

### Delta 3 — Split the end-to-end budget before spending anything on the kernel. Estimated payoff: none directly; it decides whether deltas 1, 2 and 4-8 are worth doing at all.

Every ratio in the table above is end-to-end pp512 tokens per second, not GEMM time. An
end-to-end 0.67x does not prove the GEMM is 0.67x. The chain around it carries 452 nodes and 367
barriers per window on the 3B (`followup_vulkan.md:246-248`), plus the f16 conversion dispatches
(`dasllama_vulkan_prefill.das:760-771, 852-863, 883-892, 905-914`) and the hazard fences
(`dasllama_vulkan_common.das:2587-2620`).

**Measurement.** Two halves, both cheap.
On llama.cpp: `set GGML_VK_PERF_LOGGER=1` and run the same model and prompt through
`llama-bench -p 512 -n 0 -ngl 99`. It prints one line per op keyed by shape —
`MUL_MAT q4_K m=2048 n=512 k=2048: N x T us = T us (G GFLOPS/s)`
(`ggml-vulkan.cpp:2278-2297, 2240-2254`).
On ours: the prefill already timestamps every role (`pfq_ts`, `dasllama_vulkan_prefill.das:391`,
roles reported at `:983-985`). Sum the GEMM roles and compare against the sum of llama.cpp's
`MUL_MAT` rows for the same forward.
If our MUL_MAT total is at parity and the wall clock is not, the deficit is outside the GEMM and
deltas 1-2 are the wrong target. If our MUL_MAT total is 0.67x, the deficit is in the kernel and
delta 1 leads.

### Delta 4 — Pad the activation plane's N to the tile width and keep the fast path. Estimated payoff: zero at exactly 512 tokens, up to about `1/ceil(N/BN)` of the GEMM at other token counts.

llama.cpp pads B's N to the tile width in the prealloc buffer specifically so the last token
column still takes the unclamped path (`ggml-vulkan.cpp:9319`, gate at
`mul_mm_cm2.comp:345`). Ours sizes the plane to `cnt` and drops the partial column onto the fully
clamped edge path (`dasllama_vulkan_classes.das:4893, 4901`). At 512 tokens with BN 256 there is
no partial column, so this is invisible in the pp512 board — and visible in every real prompt
whose length is not a multiple of 256.

**Measurement.** Run pp at 512 and at 500 tokens, ours and llama.cpp, same model. Our ratio
should degrade at 500 and theirs should not. If both degrade equally, the edge path is not the
cause.

### Delta 5 — Narrow the tail accumulator (`enable_smaller_matrices`). Estimated payoff: same trigger as delta 4, roughly half its size.

llama.cpp allocates a `BN/2` or `BN/4` accumulator for the tail column
(`mul_mm_cm2.comp:36-38, 367, 410`), so a 64-token remainder does not drag a 256-wide
accumulator through the whole k loop. We have no analogue. Fix delta 4 first; this is the
refinement on top of it.

**Measurement.** Only meaningful after delta 4 lands. Same 500-token comparison, with and without
a narrowed tail stamp.

### Delta 6 — Reduce the inner unroll from 8 to 4. Estimated payoff: 0 to 5%, sign unknown.

Ours unrolls 8 deep, 512 k per outer iteration (`dasllama_vulkan_classes.das:4909-4916`);
llama.cpp unrolls 4 at `QUANT_K = 256` (`mul_mm_cm2.comp:341-342`). With an f16 128x256
accumulator already costing 64 registers per invocation, an 8-deep window the compiler chooses
to software-pipeline could cost an occupancy tier. The architecture doc records that a related
codegen spelling change was worth 27% of prefill throughput on this exact box
(`ARCHITECTURE_GPU_VULKAN.md:126-130`), so codegen sensitivity here is established, not
speculative.

**Measurement.** Build the k4 large class at unroll 4 and at 2 alongside the shipped 8 and time
all three on one shape through the probe's shape arms. Read the driver's register/occupancy
report if Nsight is attached; otherwise the timing alone decides.

### Delta 7 — No four-wide decode callback. Estimated payoff: zero today, large the day glslc is upgraded.

llama.cpp ships a `_v` twin of every decode function returning `f16vec4`
(`dequant_funcs_cm2.glsl:576-623` for Q4_K, `:778-805` for Q6_K, `:1002-1025` for IQ2_XS, and so
on for every format), wired through `GGML_VULKAN_COOPMAT2_DECODE_VECTOR`
(`mul_mm_cm2.comp:82-86`, `vulkan-shaders-gen.cpp:480-484`). The local glslc cannot build it
(`tbo_build_vk.log`), so llama.cpp runs scalar here — but the moment that toolchain moves,
llama.cpp's callback count drops fourfold and every delta above gets 4x harder to close. Our
`[spirv_decode]` surface has no vec4 form.

**Measurement.** None needed to establish the risk. To size it, build llama.cpp with a glslc that
supports the extension and re-run the same per-format board; the delta between the two llama.cpp
builds is what we would owe.

### Delta 8 — The per-workgroup prologue and the computed codebooks. Estimated payoff: under 2% on the dense 1B shape; real on MoE grids.

Five `sched` loads, a workgroup broadcast and a barrier before the first tile load
(`dasllama_vulkan_classes.das:3972-3976, 4849-4851, 4887`), plus synthesising 256 to 2048
codebook words with arithmetic rather than copying them (`:4861-4886`). On a 32-workgroup dense
dispatch this amortises away. On a MoE expert grid with many small dispatches it does not, which
is the GEMV twin of the problem `followup_vulkan.md:548-557` (entry 35) already flags for tg.

**Measurement.** A variant that hardcodes the single-region dense case — region 0, workgroup base
0, no `sched` read — A/B against the shipped class on the same shape. If the dense arm gains
under 2%, leave it alone and route the effort to the MoE grids instead.

**One correction to file regardless of the outcome.** `followup_vulkan.md:540-546` (entry 34)
is wrong on three counts against the repo's own tables: the band is 0.67-0.90x not
0.67-0.70x; "the gap is the shared batch-GEMM tier, not any one format's decode" is contradicted
by Q8 at 100.6%/103.5% (`:211-213`) and by the 62 versus 36-38 TF/s tile rates (`:176-178`,
`:223-224`); and "tg holds parity or better" is contradicted by IQ2_XS tg at 0.54x
(`HOW_TO_ADD_A_FORMAT.md:607`), which entry 35 itself documents.

---

## 4. What `test-backend-ops perf -o MUL_MAT` gives, and the exact flags

**The binary is already built:**
`D:\Work\llama.cpp\build-vulkan\bin\Release\test-backend-ops.exe`
(`C:\Users\Boris\.claude\jobs\7f9893d9\tmp\tbo_build_vk.log`, final line).

**Command:**

```
D:\Work\llama.cpp\build-vulkan\bin\Release\test-backend-ops.exe perf -o MUL_MAT -b Vulkan0
```

- `perf` selects performance mode (`tests/test-backend-ops.cpp:11013-11014`).
- `-o MUL_MAT` filters by `ggml_op_desc` (`:11021-11027`, usage at `:10992-10993`).
- `-b Vulkan0` filters by `ggml_backend_dev_name`; the Vulkan device name is
  `GGML_VK_NAME + index` = `"Vulkan0"` (`:11098`, `ggml-vulkan.cpp:7001`,
  `ggml/include/ggml-vulkan.h:10`).
- `--output csv` or `--output sql` for machine-readable rows (`:11040-11047`).
- `-p <regex>` filters on the case's `vars()` string, which for MUL_MAT is
  `type_a,type_b,m,n,k,bs,nr,per,k_v,o,src_overlap` (`:4490-4492`). Example:
  `-p "type_a=q4_K,.*n=512,k=14336"`.

**What each row reports:** average microseconds per op, GFLOPS computed as
`2*m*n*k*bs[0]*nr[0]*bs[1]*nr[1]` over wall time, and the run count
(`:4510-4513`, `:1639-1646`). The harness sizes the run count to about 100 GFLOP of work on a
GPU backend, duplicates the op that many times into one graph, does a warmup compute, then loops
whole-graph submits until at least one second has elapsed (`:1569-1601`, `:1620-1636`).

**What it gives us as a reference row:** a per-kernel GFLOPS number for llama.cpp's cm2 GEMM at a
fixed shape, isolated from the rest of a forward pass, on the same device and driver — the exact
counterpart to `vk_gemm_probe.das`'s per-shape TF/s rows. Because the graph repeats one node,
the f32-to-f16 activation conversion is converted once and reused across the duplicates
(`ggml-vulkan.cpp:9441-9453`), so the reported GFLOPS is close to pure GEMM.

**The limitation that matters, and the workaround.** Perf mode does not use the correctness test
set; it uses a fixed list built by `make_test_cases_perf` (`:10151`, dispatched at `:10727-10728`).
The only quantized MUL_MAT rows at 512 tokens in that list are

```
for (int bs : {1, 2, 3, 4, 5, 8, 512})
  for (ggml_type type_a : all_types)
    test_mul_mat(type_a, GGML_TYPE_F32, 4096, bs, 14336, {1,1}, {1,1});
```
(`:10321-10323`)

so the available shape is **m 4096, n 512, k 14336** — a Llama-8B FFN shape, not a 1B/3B one.
`-p` can only filter what is already in the list; it cannot create a 2048x512x2048 or
3072x512x8192 row. Getting our shape needs either a one-line addition to that loop (a local
patch to a tree this task treats as read-only) or `--test-file` with a graph exported by
`test-export-graph-ops` (`:11058-11061`, usage at `:10997`).

Two practical notes. First, the 4096x512x14336 row is still a useful reference: it takes the
same aligned large pipeline (`tiles_m = 32*4 = 128 > 36`, so `prefer_large`) and split-k 1
(128 tiles, well over `36*2/3`), which is the configuration a 1B prefill also lands on. Second,
for a shape-matched comparison the cheaper route is `GGML_VK_PERF_LOGGER=1` on a real
`llama-bench -p 512 -n 0 -ngl 99` run: it emits per-shape `MUL_MAT <type> m=.. n=.. k=..` rows
with GFLOPS from the actual model geometry (`ggml-vulkan.cpp:2278-2297`), which is what delta 3
needs anyway.

---

## Summary table

| dimension | llama.cpp cm2 | ours |
|---|---|---|
| tile large / medium / small | 128x256x64 / 128x128x64 / 32x64x128 | 128x256x64 / 128x128x64 / 128x32x64 |
| workgroup | 256 invocations | 256 invocations |
| accumulate | f16 (f32 on PREC_F32) | f16, clamped, widened to f32 at store |
| A operand | quantized, decoded in the driver's block load | same |
| B operand | f16, one cached conversion dispatch | f16, one conversion per sibling group, SwiGLU fused |
| scales | inside the block; Q4_K/Q5_K hoisted to shared memory | separate SSBO, re-extracted per element with variable shifts |
| barriers per k-step | 0 (2 per 256 k on Q4_K/Q5_K only) | 0 |
| loads per k-step | 1 A + 1 B | 1 A + 1 B |
| unroll | 4 x BK | 8 x BK |
| split-k | k>=2048, cores/tiles, cap 8, 256-aligned | identical rule |
| N padding | buffer padded to tile width | none (large/medium); small tile rounds to 32 |
| tail accumulator | BN/2, BN/4 | none |
| grid | 3-D from `gl_WorkGroupID` | 1-D plus an SSBO region record |
| batching | `gl_WorkGroupID.z` plus a separate MUL_MAT_ID shader | region list in the same kernel |
| vec4 decode | present, gated on glslc (absent locally) | absent |
