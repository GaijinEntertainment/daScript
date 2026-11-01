# dasLLAMA Architecture - the kernels both GPU homes stamp

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries the kernels more than one
home runs - the Metal, Vulkan and CUDA homes: the template mechanism of `dasllama_gpu_kernels_common.das`, the
ledger of which kernels ride one template and what each stamp reads there, and which stay per home and why, the routed
block's routing kernels both discrete homes stamp (`#moe-routing-kernels`), and the deltanet kernels they stamp with
their smalls layout and capacities (`#dn-discrete-kernels`).

### Kernels both homes stamp {#gpu-shared-kernels}

A kernel more than one home runs is one `class template` in
`dasllama_gpu_kernels_common.das`: its `@ssbo` buffers with their `@role`s, its argument struct and its
body. The module requires no backend - a template's body resolves in the module
that stamps it. Each home stamps the template as a leaf class under its own dispatch annotation and its
own kernel method, which calls the body, so a kernel keeps the dispatch name, the pipeline and the census
key it has on that home.

- **The arguments are one `@kargs` member.** `kargs_as` (`dasllama_kernel_access.das`) runs in both
  dispatch lenses ahead of the kernel's emission and gives the member the form that home's emitter reads:
  a `@push_constant` block on Vulkan, a by-value `@uniform` at the binding past the class's highest on
  Metal - once: a class under two dispatch annotations passes through the lens twice, and the second pass
  keeps the binding the first gave, so each builder and the kernel agree on it. The struct is a member and
  not a parameter of the body: a parameter is passed by value, and the
  emitted kernel then copies the whole struct where a member is read a field at a time.
- **A body that reads a buffer at an offset takes it as an element offset in its arguments.** Vulkan caches
  a descriptor set by its binding tuple, so an offset bound per dispatch would be a set per offset; an offset
  in the arguments is one set. A kernel every site reads from element zero carries none.
- **The two emitters place the body differently.** The MSL emitter splices a called method flat into the
  kernel; the SPIR-V emitter keeps it a function the entry point calls. A kernel moved here therefore
  reads, on Vulkan, as its old words plus one call. Because the MSL emitter splices, a template method of
  more than one statement cannot sit in value position: a helper that returns a value - a workgroup
  reduction, a partial sum - is written as a statement method that lands its result in a `var` reference
  (`wg_max_into`, `wg_sum_into`, `value_sum_into` in `GkPkAttn`). The same splice names a parameter as
  many times as the body does, so an argument that is a computed expression is refused where the body
  names the parameter twice: the caller hoists it to a local first. A fixed-array local in a body hoists
  to a program-scope constant table only when every element is a scalar integer or float literal; a
  vector literal does not hoist, so a `float4` table is spelled as four scalars a word.
- **A home's lane primitive is reached by one name.** A template method resolves in the module that
  stamps it, so a shared reduction calls `gk_subgroup_add` / `gk_subgroup_max` (a histogram's
  subgroup aggregation `gk_subgroup_bcast0_u`), and the stamping module reaches a definition over its own
  primitive: the discrete file carries the one copy in SPIR-V's spelling (`subgroupAdd` / `subgroupShuffle`),
  which both discrete emitters lower, and a home defines a lane primitive of its own only where its
  spelling differs (`simd_sum` / `simd_shuffle` on Metal). A free function
  in the common module resolves in the common module and sees neither home, so a body that needs a
  home's primitive keeps it in a method.
- **A binding a stamp does not use is gated, and a stamp names its element types.** `@template_gate`
  erases the field at stamping (`daslib/typemacro_boost`, before either emitter reads the class): a gated
  f16 output or bias row is no parameter of that stamp's builder (`GkRestride`). A body writing one output
  in two precisions spells its element type as a `typedef` each stamp declares (`OT`), which both emitters take.
- **A number the drivers verify is a `let` beside the template, and a scale both homes must round
  alike is computed on the host.** A template constant's default is a literal, so the window a kernel is
  stamped for is spelled twice - the default and the `let` each driver checks against (`GK_G4A_ATTN_PAST`).
  A shape-derived scale (`gemma4a_attn_scales`) is passed in the arguments: in the kernel, the homes' intrinsics round it apart.
- **The discrete homes share a second layer.** `dasllama_gpu_kernels_discrete.das` holds the bodies
  Vulkan and CUDA run and Metal does not - the decode GEMV family over every `KqFmt` (`Q8GemvT`
  and its N-column, gate-up, residual-epilogue and per-layer-embedding twins, `Blk32GemvT` with
  the `[template(fmt)]` per-32 decodes, `KqGemvShellT` -> `KqGemvLeafT` -> the thirteen
  `<Fmt>GemvT` block decodes) and the decode helpers they splice; the token step's row families
  over the common templates (the residual step's `ArBase` -> `ArRqBase` ->
  `ArRqT` and `ArCombBase`, `ArCombG4T`, the requants `RqT` -> `RqPlainT` / `ActRqT`, `ActF16T`,
  `EmbGatherT`, the classifier pick's `ClsArgmaxBase` -> `ClsArgmaxPartT` / `ClsArgmaxFinT`, the
  decode attention `DaAttnT` over the token block `TokMeta`, the mirror stores `KvMirStoreT` ->
  `RopeKvStoreBT`, `RopeKvStoreT`, `QknRopeKvT`) with their args structs and the K/V block
  readers (`kv_blk_scale`, `kv_skew_word`, the quant unpackers); the prefill's batch GEMM tiles
  over the region rail `MoeCmBase` (`BatchArgs`, `SCHED_NONE`: `KqCm2BatchT` -> the twenty
  `<Fmt>Cm2T` block decodes, `MmBatchT`, `Blk32BatchT`, the KHR f16 shell
  `CmF16Base`), the small f16 x f16 GEMM on the cm2 tile (`F16GemmCm2T`: the E-series projection's
  window form on both homes, the MoE router's logits on Vulkan) and the flash attention (`FaT` -> the `FaH<hs>T` fragment shapes with the `facm2_*`
  per-element laws, `AtAttnTileT` -> the batched mirror pass `DaAttnBT` and the wide `DaAttnBWT`). Every class
  there is a `class template` under the same contract as
  this file's `Gk*` ones - but for the plain classes a chain roots in: `KqGemvBase`, the kq
  chain's root - the family's planes and the three abstract block-decode methods a host caller
  reaches through a `KqGemvBase?` (the CPU host twin, the kernel cells' oracle), nothing a kernel
  stamps, since a template cannot be a pointer type; and `MoeCmBase` / `CmF16Base`, the region
  rail the Vulkan home's sdot4 and KHR-f16 leaves derive from plainly (the dispatching lens's
  access walk follows a base's methods into the base's module). No kernel method, no
  dispatch annotation, the body a `[spirv_inline]` method (`gemv_body`, `leaf_body`, `b32_body`,
  ...) a home's leaf calls under its own `[spirv_kernel]` or `[cuda_kernel]` and dispatch line.
  A template, not a plain class, because a body's `gk_subgroup_*` calls and the dispatch lens's
  access walk both resolve in the module that stamps it. The Vulkan leaves keep their
  specialization-constant width (`local_size_x_id`) and their census names on the leaf's
  annotation. `dasllama_vulkan_classes.das` also holds two templates the Vulkan home alone
  stamps and no driver dispatches - `DaAttnBH128T` (the batched attention's h128 coopmat tile)
  and `F32RowsKhrT` (a file's f32 rows on the KHR tile) - held by their kernel cells
  (`test_vkd_da_attn_b_h128`, `test_vkd_f32_khr_batch`): a body one home runs lives in that
  home's classes file.
- **A home that must lower one method differently stamps a home-only template between the shared
  template and its leaf, never a second body.** The template is the shared one's child in the home's
  classes file, overriding the one method, and the leaf derives from it under its unchanged dispatch
  line; the body, the args and the census key stay the shared template's. The CUDA home's codebook
  GEMVs ride this shape (`Iq4xsGemvCudT`, `Iq4nlGemvCudT`, `Iq4nl32GemvCudT`, each overriding the one
  method `iq4_word_of` with `cud_iq4_word`): the shared `iq4_word` selects a codebook word by a dynamic
  vector index, which NVCC places in local memory, so the home's override takes each nibble's byte
  through `__byte_perm` instead, and the codebook GEMVs run several times faster on the CUDA home than
  under the shared `iq4_word`. A method a home will override is a method of the
  shared template, not a free function it calls: the shared template names the hook at the narrowest
  point the homes part (`KqGemvShellT` and `Blk32GemvT` name the codebook's word map, `iq4_word_of`,
  and spell the gather around it once), so an override is one line and the gather stays shared.
- **What stays per home:** the GEMM and attention tiles, which each home builds on its own matrix
  primitives. A pair whose two forms differ in shape (the TTS elementwise maps run one element a thread on
  Metal and four an invocation on Vulkan) folds onto one body when both homes profile the same on it, and
  until then shares its per-element function through `dasllama_gpu_math.das`. Which pairs ride a template, what each stamp
  reads on its home, and why the rest stay per home: `ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-kernel-pairs`.

### The kernel pairs of the two homes {#gpu-kernel-pairs}

**The row kernels of the qwen chains ride one template a pair.** Each template below holds the buffers, the argument
struct and the body; a stamp sets the template constants its home needs and maps its grid onto the body's method.

| Template | Metal stamps | Vulkan stamps | What the stamps set |
|---|---|---|---|
| `GkHcInit` | `MetalHcInit` | `HcInit` | nothing |
| `GkHcNorm` | `MetalHcNorm` (1024 lanes, `rsqrt`) | `HcNorm` (256 lanes, `1 / sqrt`) | `TG`, `RSQRT` |
| `GkHcMix` | `MetalHcMix` | `HcMix` | nothing; the CPU `hc_mix_tail`'s x . sigmoid(g) and 1 / hc |
| `GkHcCombine` | `MetalHcCombine` | `HcCombine` (`MOE`: the host-summed experts and the shared gate) | `MOE` |
| `GkPleGate` | `MetalPleGate` (the ring or the panel at `cin`) | `PleGate` (the ring slot off `tok`, or the panel) | `TG`, `RSQRT`, `cin_base` / `cin_store` |
| `GkPleConv` | `MetalPleConv` | `PleConv` | `row_pos` / `tap`; the CPU chain's (res + gated) + silu(conv) |
| `GkQkRms` | `MetalQkNorm` (the q and k heads of one buffer, `kwd` the no-wv V fill) | `QkRms` | the grid decode |
| `GkTopKRow` | `MetalMoeSelect` / `512` (32 lanes, DEPTH 8 / 16, `raw` the gpt-oss form), their `Ds` twins (`DSCALE`: gemma-4's per-expert down scales, the layer's row bound) | the discrete file's `TopKRecords`: `TopKN` / `TopKN2` / `TopKRows` / `TopKRows2` on Vulkan, `CuTopKN` / `CuTopKN2` / `CuTopKRows` / `CuTopKRows2` on CUDA (256 lanes, DEPTH 1 / 2, `DSCALE`, the row named at run time) | `LANES`, `DEPTH`, `DSCALE`, `DSOFF_SWITCH` |
| `GkMoeCombine` | `MetalMoeCombine`, `MetalMoeReduce` (`GATHERED`) | `MoeCombine` (`WINDOW`: the chunk window, the accumulate, the zero-weight skip), `MoeCombineRow` (`BYROW`: a bundle's region rows) | `GATHERED`, `WINDOW`, `BYROW`, `SLOT_X4` |
| `GkDnGate` | `MetalDnGate` / `Sig` (`rsqrt`) | `DnScanP3` / `Sig` | `ZSIG`, `RSQRT` |
| `GkQknRopeKv` | `MetalQknRope16` / `32` (one row), `MetalQknRopeB16` / `B32` (rows), `MetalPfQkn` / `HX` (the window, the q twin) | `QknRopeKv` / `F16` | `KT`, `CLAMP16`, `V_WG`, `LANES` / `EPT`, `HX` |
| `GkRouterGemv` | one Metal shell (`MetalRouterGemvT`: the grid onto the rows under `ROW_WG`) under two formats - the f32 rows (`MetalMoeRouterT`: `MetalMoeRouter`, a workgroup a row, the decode column; `MetalMoeRouterB`, a subgroup a row, `COL_CLAMP`, 8 columns) and the E-series side rows' projection's bf16 words (`MetalPleProjBf16T`: `MetalPleProjBf16One`, `MetalPleProjBf16`, the same two shapes; `MetalPleProjBf16HwT`: `MetalPleProjBf16HwOne`, `MetalPleProjBf16Hw`, a bf16 halfword a lane), and the f32 rows a lane an element (`MetalF32RowsT`: `MetalF32RowsOne`, `MetalF32RowsB`, the same two shapes) | `RouterGemv` (a workgroup a row, `NORM`), `RouterGemvBf16` (the bf16 words under `YSCALE`), `RouterGemvBf16Hw` (the same rows a bf16 halfword a lane) | `ROW_WG`, `VW`, `NC`, `COL_CLAMP`, `NORM`, `BIAS`, `BF16`, `YSCALE` |
| `GkPkGemv` | `MetalPkGemv*` (f32 rows), `MetalPkGemvQ8*` | `TtsPkGemv*` (`QUAD`) | the epilogues, `QUAD`, `RSQRT` |
| `GkMoeCombine` `SH` | `MetalMoeCombineSh`, `MetalMoeReduceSh` | the combine twins' `gated` arm | `SH`: the shared row under its gate logit, the router's row `ne` |
| `GkCat2` | `MetalCat2` | `Cat2` | nothing; position- or stream-major, the a row repeated over the streams |
| `GkDnConv` | `MetalDnConvN` / `N4` (`PB` 1 / 4, `RING` 8: a group of 16 rows or fewer) | `DnConv` (`PB` 32, `RING` 4) | `PB`, `RING` |
| `GkMoeCombG4` | `MetalMoeCombG4` / `Hx` (the selection rows' slots, the next norm in f32, `Hx` its f16 twin), `MetalMoeCombG4Pf` (the bucket inverse's slots, no norm out; 1024 lanes, `rsqrt`) | `ClsArCombG4` / `F16B` / `Rq` (the slot map, the next norm in f32, f16 or Q8_0 blocks; 256 lanes, `1 / sqrt`) | `TG`, `RSQRT`, `GATHERED`, `OUT_F32` / `OUT_F16` / `OUT_Q8`, `HX`, `PRESUM_SWITCH`; the stash and, on Metal, the norm rows' layout |
| `GkClsEpilogue` | `MetalClsEpilogue` | `ClsEpilogue` | nothing; every row in one dispatch |
| `GkWgScan` | (the selection's base) | the discrete file's `WgScanT` (the value-form `excl_scan` over the statement form) under `MoeSchedT`: `MoeSched` at 256 lanes and `MoeSched2` at 512 on Vulkan, `CuMoeSched` / `CuMoeSched2` on CUDA | `WG`, the `scan` slot a thread sized by it |
| `GkClsSelect` | `MetalClsSelect` (1024 lanes, no fast math) | `ClsSelect` (1024 lanes) | `WG` at 1024 over `GkWgScan`; the `SEL_*` layout as the template's constants, the max and sum `GkWgReduce`'s, the lane floor `sel_lane_val` of `dasllama_gpu_math`, the atomics the shader lingua franca's `atomicAdd` |
| `GkAddRmsRows` | `MetalAddRms` / `HX` (the plain step), `MetalPreAddRms` / `HX` (the sandwich seam in one dispatch: the branch normed under its post norm, the add, the next norm), `MetalPreAdd` (no norm out); 256 lanes the window and a decode row within the slab, 1024 a decode row past it | `ClsArAddRms` / `F16B`, `ClsArAddRmsRqB` and the `ArAddRmsRq*` stamps (Q8_0 out), the combines' `ArCombBase` (256 lanes) | `LANES`, `PRE`, `PRE_SWITCH`, `NORM_SWITCH`, `ARMS`, `X4`, `HX`, `OUT_*`; on Metal, the norm rows' layout |
| `GkPleAct` | `MetalGegluSide` (in place over the gate rows, 64 lanes) | `PleActF32` (`OUT`: the f32 projection's x, 256 lanes) | `OUT`; the side row's index is `gk_strided_at`, `ActRqT.up_at`'s |
| `GkPleFinish` | `MetalPleFinish` (`XSCALE`: the projection's 1 / sqrt(dim) in the finish, `rsqrt`) | `PleFinish` (the projection arrives scaled, `1 / sqrt`) | `XSCALE`, `RSQRT`; 256 lanes a slice on both |
| `GkRnntPred` / `GkRnntJoint` / `GkRnntPick` | `MetalRnntGates`, `MetalRnntProj` (`GATES` off), `MetalRnntJoint`, `MetalRnntPick` | `RnntGates`, `RnntProj`, `RnntJoint`, `RnntPick` | `GATES`; the pick's `WG` 256 on both (`ARCHITECTURE_MEDIA_ASR.md#rnnt-device-decode`) |
| `GkCtcPick` | `MetalCtcPick` | `CtcPick` | nothing; 256 lanes a frame on both, the argmax on `GkWgArgmax` (`ARCHITECTURE_MEDIA_ASR.md#gigaam-planes`) |
| `GkDwConvRows` | `MetalG4aDw` (`FORM` 1), `MetalCnDw` / `MetalPkDw` (2), `MetalGaDw` / `MetalGaDwH` (3, `TAP_F16`: the taps the carrier's halfwords at binding 3), `MetalSt2PoolDw` (0) | `TowerDwConv5`, `TowerCnDw` / `TowerPkDw`, `TowerGaDw` / `TowerGaDwH`, `TtsPoolDw` | `FORM`, `K`, `TAP_MAJOR`, `TAP_F16` |
| `GkVadEnc` | `MetalVadEnc` | `TowerVadEnc` | nothing; 128 lanes a chunk on both |
| `GkVadLstm` | `MetalVadLstm` | `TowerVadLstm` | `PTR_LOADS`: the Metal stamp reads the recurrent rows off a pointer, the SPIR-V stamp indexes them (SPIR-V takes no `addr`); one workgroup of 1024 on both, the recurrence's weights in registers |

**The VAD's recurrence reads no weight from memory a step.** Two lanes a gate row each hold their half of the row's W_hh
(64 floats) in registers for the whole walk, the plane laid k-major so the one load pass reads consecutive rows; a step
is the two half dots, a barrier, the first 128 lanes' cell, and a second barrier that publishes h to the next step's dots
and the decoder's per-subgroup partials (summed in the cell lanes' whole subgroups - 128 is a multiple of every subgroup
size) to lane 0, which lands the probability. The 64 held floats a lane would cap the pipeline at 512 lanes on the M1
family, so the Metal stamp declares its 1024 on the entry (`[metal_kernel(max_threads = 1024)]`, the
`[[max_total_threads_per_threadgroup]]` attribute) and the compiler fits them.

**The Metal E-series pre-step serves every table, ple width and model dim a blob model carries.** The gather reads the
table on its own format's block grid - a K-quant table at whole superblocks, a per-32 table at whole 32-blocks, the
per-32 pair bound on the 16-byte grid below it with the blocks past the bind in its arguments (`EmbedB32Args.b0`,
`b32_bind_rem`), the q8 copy at any element. The finish's lanes stride a slice, so a ple width past 256 runs in as many
passes as it takes and one off the 32-lane step leaves its last lanes idle. The projection's bf16 rows step word pairs where
the dim and the base are even and one halfword a lane where a row would start mid-word (`MetalPleProjBf16HwT`, picked by
`enc_ple_proj_bf16_rows`; Vulkan's `RouterGemvBf16Hw` the same; both homes pick through `gk_bf16_pairs`); the f32 rows step float4 words where the dim, the base and the column stride sit on that
lattice and one element a lane elsewhere (`MetalF32RowsT`, picked by `enc_f32_rows_gemv`). The narrow walks are stamps of
their own, so the word stamps' bodies and both homes' router words stay as they are. The window projects on the bf16 tile where the model dim is whole 32-element chunks and
layers x ple whole 64-row tiles (`pf_bf16_tile_ok`), else on the position-blocked bf16 GEMV. `metal_ple_pre_unserved`
names what is left: a planar load and a trimmed image, which serve no Metal step, and a table off its format's blocks or
in a format no gather stamp reads, which no load produces.

The gated FFN's activation is one function on both homes (`gk_act_mul`, `dasllama_gpu_math.das`): the Metal ew2 and
w13 combines and every Vulkan act class call it, the gelu its clamped tanh form.

The readings behind the templates - the instrument each figure comes from, the shapes the readings chose, and what each
stamp reads on its home against the per-home body before it: `ARCHITECTURE_GPU_SHARED_READINGS.md#gpu-shared-readings`.

**A gated model's q stays in its `[q | gate]` projection rows on both homes.** The Metal decode and verify rope stores
rope (tq4: and rotate) q at the head's `2 hs` stride, the attention stamps read it there and scale their stores by the
gate's sigmoid (`sq_gated`), and the split forms' partials carry the gate, since the combine is linear; under tq4 the
attention runs ungated and the un-rotation gates (`MetalFwhtUnsign`'s `qg`). A codec past the fused and block-codec rope
stores keeps the deinterleave and the epilogue - the two dispatches the row above retires a layer a step.

**The pairs that stay per home, a reason each:**

- `MetalDnScanT` / `DnScanT`: 32 against 16 rows a lane, the half rows, the decay order - each measured on its home
  (`ARCHITECTURE_GPU_PREFILL_WINDOW.md`, `ARCHITECTURE_GPU_VULKAN.md`).
- Metal's five-kernel deltanet step / `DnStepFusedT`: the whole step in one dispatch is the Vulkan resident command's seat;
  Metal steps a session's rows against its own mirror.
- `MetalDnConvHist` / `DnTailRingT`: not one computation - a shadow dual store against a zero-filled ring hand-off.
- `MetalDnBa` / `DnBaGemmT`: a row GEMV against a tile GEMM.
- Every GEMV and expert row (the deltanet projections, the routed and shared experts, the hc down and up, the n-gram
  projections, eh_proj, `MetalHcLo` / `HcLoRq`): Metal dots f32 x, Vulkan a requantized Q8 feed.
- The expert, router and dense GEMM tiles (`MetalMoeMulMm*` / `*Cm2*` and the KHR tiles, `pf_enc_router_mm` / `F16GemmCm2`):
  each home builds its tiles on its own matrix primitives.
- `MetalMoeCount` / `MetalMoeBucket` / `MoeSched`: an ordered atomics-free schedule against an atomic ladder; and with it the
  expert-row gather (`MetalMoeGatherX` gathers a bucket row off its slot, `MoeGatherF16` scatters a slot into its bucket row:
  the Vulkan schedule writes no bucket-to-slot map). Both clamp into the half range (`half_sat`).
- `MetalMoeMulMmQ8TensorT` off the kq scaffold, the ADAPT walks, `MetalG4RouterNorm`'s compensated sum, the gate-up fusion
  with its Q8 requant against the f32 act, the driver chains, and the hc head mixer's GEMV against its tile.
- `MetalMoeGemvMx4`: its measured lane map (sixteen blocks in flight, uint2 loads).
- The router's lane split parts by shape, not by home: a workgroup a row serves the decode column on both homes (M5 3.37
  against the subgroup form's 3.82 us), a subgroup a row the Metal window (144 against the workgroup form's 745 us at 512
  columns); the Vulkan window routes on its tile (`F16GemmCm2`), so no Vulkan stamp takes the subgroup split.
- The prefill shared gate: the window's tile GEMM wants `ne` a multiple of 64, so the gate logit is one GEMV over the router
  region's last row rather than a tile row.
- The deltanet conv + L2 past 16 rows a group on Metal (`MetalDnConv` then `MetalDnL2Norm`): the shared form's fold reads
  13% slower at a 512-row window on the M5 (83 against 75 us; PB 32 reads 108), and wins up to 16 rows (3.37 against 3.81
  us at 8, 4.77 against 5.00 at 16); `DN_CONV_FUSED_ROWS` is the knee. The Vulkan window keeps the fused form its own
  measurement chose (`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block`).
- `MetalHalfRows` + `MetalDnPrep` / the `DnScan` staging: each home's scan reads its own staged form (the pair above).
- The two MoE tile scaffolds keep one walk on Metal (`MetalMoeTileWalkT` under `MetalKqMulMmSplitTensorBase` and
  `MetalMoeMulMmKqTensorBase`), its stores the scaffold's hook; the tile lab holds the walk within 0.5% (two alternated rounds,
  best of three).

### The routed block's routing kernels both discrete homes stamp {#moe-routing-kernels}

**The routed block's routing kernels are the discrete file's templates, stamped by both discrete homes.** The batched
router GEMM `RouterGemmT` (`RouterGemm` / `CuRouterGemm`, its grid `router_gemm_wgs`), the select records `TopKRecords`
and the bucket schedule `MoeSchedT` (their rows in the pair table above), the f16 gather `MoeGatherT` (`MoeGatherF16` /
`CuMoeGatherF16`), the combine-folded residual step `ArCombBase` (`ClsArComb` / `CuClsArComb`, the f32 normed-row form on
both; the f16 and Q8_0 twins the Vulkan home's) the biased act `ActF16T` (`ActF16B` / `CuActF16B`) and the biased act + requant `ActRqBT` (`Q8ActRqB` on Vulkan;
`CuQ8ActRqB` and its Q8_K twin `CuQ8kActRqB` on CUDA) share one body
each; a stamp sets its lane count and output form and maps the grid onto the body's method. Their CPU oracles are one
module the two homes' kernel cells read (`tests/_moe_routing_oracles.das`). How each home chains the five stages into its
window and token step: `ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block`, `ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token`.

**The router GEMM is the decode router GEMV batched:** a 64 x 32 tile of positions by router rows per workgroup, each
invocation a 4 x 2 block whose two rows sit 16 apart, K in 64-wide steps through a float4 stage in shared memory at a row
stride of 17 float4, the next step's rows fetched into registers while the current step computes, and each output's four
products per float4 added in k order (the scalar loop's sums to the bit). The stride and the row split are the bank rule:
sixteen lanes reading sixteen rows land on all eight 16-byte bank groups, so a warp's two weight loads take two
wavefronts each and its four activation loads (two distinct rows) one; the scalar tile is bound by that shared-memory
traffic at about six FMAs per staged float4. The 64-wide K step is why the MoE seats ask for a 64-multiple row width. The
router plane holds every MoE layer's f32 rows, and a gated shared expert's gate vector rides as one more row past the
experts, so one dispatch writes the logits row `[ne | gate]` per position.

**The bucket schedule writes what the host fill writes for the per-op chain,** one workgroup: the experts' slot counts
are an atomic tally over the window (a thread an expert, 256 of them or the 512-thread stamp past 256 experts, stride the
picks, one workgroup atomic per slot), exclusive scans place each bucket's rows, its region indices (experts with rows, in
expert order) and its tile workgroups, and the three expert planes' schedules land as 4-word records plus per-workgroup
maps. A bucket is cut into at most two pieces by the tile ladder: a bucket within the s column (`SCHED_S_ROWS`, 32 rows)
is one s piece; a bigger bucket takes whole m columns (`SCHED_M_ROWS`, 128 rows) with the last one partial, unless the
remainder past the whole columns fits the s column, which then takes it (`sched_ladder_m_rows`); every piece dispatches
one workgroup per column tile per 128-row weight tile of the plane. The s pieces' records sit at `[0, ne)`, the m pieces'
at `[ne, 2 ne)`, and each dispatch's map at its own offset past the records (`PF_MOE_MAP_OFF` / `CUD_MOE_MAP_OFF`, 4096
words for up to 512 experts twice), the two tile counts scanned as one packed word (its halves stay under 65536 for any
admitted shape: 512 tokens by 64 slots over 512 experts). Every dispatch is sized for its worst case - one s tile per
expert; every expert's whole columns plus a partial one - and the map's tail past the real workgroup count carries the
sentinel (`SCHED_NONE`): a tile workgroup that reads it sees a zero-row region and returns before its first barrier. The
slot-to-bucket-row map hands every slot the next row of its expert's bucket through an atomic cursor, so the rows within
a bucket land in an order the schedule does not fix - which nothing downstream reads: the tiles compute rows apart and
the combine reads each slot's row through the map, so the sums are the CPU walk's to the bit (the kernel cells check the
map as a permutation of each bucket's rows). The per-expert slot walks this replaced - thread e scanning every slot twice,
staged through workgroup memory in 256-slot chunks - cost 163 us per layer on the 30B.

### The deltanet kernels both discrete homes stamp {#dn-discrete-kernels}

**A recurrent layer's device kernels are one body a kernel in the discrete file, stamped by the Vulkan and the CUDA
homes alike, and the two homes' drivers read one smalls layout.** The bodies: the fused decode step `DnStepFusedT` (one
workgroup a value head of a row - the conv preamble over the head's channels off the row's ring image, the q and k norms and
the raw q.k by subgroup adds, the delta rule with each of the 256 threads owning one state column's row part held in
registers through the update, the out-norm's sum by the shared fold, the o row as Q8_0 blocks or as floats under
`out_f32`; the row's region slot and ring parity off its token block, `TokMeta.dnslot` / `parity`), the window's scan
`DnScanT` (the plain per-token delta rule, a column of a head's state a lane cluster, 16 state rows a lane in registers, the
next token's k and q read ahead of the recurrence, a capture record writing the state after its row into the checkpoint
rows), the conv front's bindings `DnConvT` over the common `GkDnConv` (the taps and the history out of the layer's smalls,
the history per channel under `hist_mode` 1), the gate shell `DnScanP3T` over the common `GkDnGate` (a workgroup a row, its
subgroups walking the value heads), the ring hand-off `DnTailRingT` over `DnTailT` (the window's last `taps` projected rows
into the other ring image, a part shorter than the taps taking its leading rows off the entry image), the f32 beta/alpha
tile `DnBaGemmT` (a workgroup 16 positions by 16 of the `2 nvh` output rows, the x rows in the stamp's element type `XT`:
the Vulkan window's f32 normed rows, the CUDA window's f16 feed), and the router GEMV `RouterGemvT` over the common
`GkRouterGemv` (the f32 arm's beta and alpha rows in one dispatch: `ne` 2 nvh split at nvh between the beta base and the g
base). A home's leaf calls the body under its own kernel annotation and dispatch line (`dn_step_body`, `dn_scan_body`,
`dn_conv_body`, `dn_gate_body`, `tail_ring_body`, `dn_ba_body`, `router_body`); the Vulkan home's capture tail
`DnCapTail` stays its own leaf over `DnTailT`, since the CUDA home installs no snapshot seats.

**The smalls plane is one layout on both homes** (`DN_SM_*`, `DN_WINDOW`, `DN_MAX_*`, the per-step scratch sizes
`DND_*`): the beta and g rows a window deep at `DN_SM_BETA` / `DN_SM_G`, the conv taps at `DN_SM_TAPS`, the out-norm
weights at `DN_SM_WNORM`, a and dt at `DN_SM_A` / `DN_SM_DT`, then from `DN_SM_HIST` a ring pair (two images of
`cd x (dconv - 1)` floats) a region slot, `DN_SM_SLACK` floats past the last. The dispatch grids are the layout's too:
`dn_conv_wgs` (256 channels by `DN_CONV_PB` positions a workgroup), `dn_scan_wgs` (a workgroup `4 x 32 / (ds / 16)`
columns of one head), `dn_ba_wgs` (16-position by 16-row tiles). A kernel reads the offsets off its arguments, so a
driver that moves a row moves it for both homes in the one place.

**The chain's capacities are fixed constants, sized for the qwen35 family with headroom.** A window runs `DN_WINDOW`
(4096) positions at a time with the state carried on the device, and the `DN_MAX_*` maxima (state size 128, 64 value
heads, conv width 16384, q + k region 8192, 4 taps) cover the family through Qwen3.8-27B (state size 128, 48 value
heads, conv width 10240, 4 taps). The per-op drivers assert them, and each home's resident plan (`vk_rdec_prepare`,
`cud_rdec_prepare`) declines a larger deltanet geometry by name.

The ring region sits last in the smalls plane, so a window's re-stage of a layer's smalls (`record_dn_cmd`) copies only
the floats below `DN_SM_HIST` and never overwrites the conv history the previous window left on the device.

The decode step's own smalls (`DND_SMALLS_BYTES`) carry a second conv-ring image after the shared layout, beside the
step's scratch: the raw beta and g staging with the ring parity word (`DND_BG_BYTES`), and the o row's Q8_0 image the
out GEMV reads (`DND_OQ_BYTES`, `DND_OS_BYTES`). The fused step reads the image the parity word names and writes the
other one: a ring row's readers span workgroups, so an in-place update would race a workgroup still reading the row.

**Within the Vulkan home, `KqCm2BatchT.cm2_tile` keeps its unsplit k-loop beside the split one.** At `ksplit` 0 the split
arm computes the same sums, but the unsplit arm's loop bound off the push block and its store at element zero are what the
driver compiles best: with the split arm alone the k4 l and m stamps read 1.2 to 1.5% slower on the pod (`vk_gemm_probe`
`cm2:k4`, gate 0.190 -> 0.193 ms, down 0.184 -> 0.187 ms, two alternated rounds).

**Within the Vulkan home, each fused gate-up GEMV keeps its own `gu_store`.** The q8, Q4_K and mx4 gate-up classes share
no parent that carries the output planes, and the store's body is already the shared helpers (`gk_act_mul`,
`q8_lane_scale`, `q8_quant1`); what is left per class is the plane binding.
