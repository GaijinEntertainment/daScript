# dasLLAMA Architecture - the kernels both GPU homes stamp

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries the kernels more than one
home runs - the Metal, Vulkan and CUDA homes: the template mechanism of `dasllama_gpu_kernels_common.das`, and the
ledger of which kernels ride one template and what each stamp reads there, and which stay per home and why.

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
  `<Fmt>Cm2T` block decodes, `F32RowsKhrT`, `MmBatchT`, `Blk32BatchT`, the KHR f16 shell
  `CmF16Base`) and the flash attention (`FaT` -> the `FaH<hs>T` fragment shapes with the `facm2_*`
  per-element laws, `AtAttnTileT` -> the batched mirror pass `DaAttnBT` and the wide `DaAttnBWT`,
  `DaAttnBH128T`). Every class there is a `class template` under the same contract as
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
  annotation.
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
| `GkTopKRow` | `MetalMoeSelect` / `512` (32 lanes, DEPTH 8 / 16, `raw` the gpt-oss form), their `Ds` twins (`DSCALE`: gemma-4's per-expert down scales, the layer's row bound) | `TopKRecords`' `TopKN` / `TopKN2` / `TopKRows` / `TopKRows2` (256 lanes, DEPTH 1 / 2, `DSCALE`, the row named at run time) | `LANES`, `DEPTH`, `DSCALE`, `DSOFF_SWITCH` |
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
| `GkWgScan` | (the selection's base) | `WgScanT` (`MoeSched` at 256 lanes, `MoeSched2` at 512: the value-form `excl_scan` over the statement form) | `WG`, the `scan` slot a thread sized by it |
| `GkClsSelect` | `MetalClsSelect` (1024 lanes, no fast math) | `ClsSelect` (1024 lanes) | `WG` at 1024 over `GkWgScan`; the `SEL_*` layout as the template's constants, the max and sum `GkWgReduce`'s, the lane floor `sel_lane_val` of `dasllama_gpu_math`, the atomics the shader lingua franca's `atomicAdd` |
| `GkAddRmsRows` | `MetalAddRms` / `HX` (the plain step), `MetalPreAddRms` / `HX` (the sandwich seam in one dispatch: the branch normed under its post norm, the add, the next norm), `MetalPreAdd` (no norm out); 256 lanes the window and a decode row within the slab, 1024 a decode row past it | `ClsArAddRms` / `F16B`, `ClsArAddRmsRqB` and the `ArAddRmsRq*` stamps (Q8_0 out), the combines' `ArCombBase` (256 lanes) | `LANES`, `PRE`, `PRE_SWITCH`, `NORM_SWITCH`, `ARMS`, `X4`, `HX`, `OUT_*`; on Metal, the norm rows' layout |
| `GkPleAct` | `MetalGegluSide` (in place over the gate rows, 64 lanes) | `PleActF32` (`OUT`: the f32 projection's x, 256 lanes) | `OUT`; the side row's index is `gk_strided_at`, `ActRqT.up_at`'s |
| `GkPleFinish` | `MetalPleFinish` (`XSCALE`: the projection's 1 / sqrt(dim) in the finish, `rsqrt`) | `PleFinish` (the projection arrives scaled, `1 / sqrt`) | `XSCALE`, `RSQRT`; 256 lanes a slice on both |

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

- `MetalDnScanT` / `DnScan`: 32 against 16 rows a lane, the half rows, the decay order - each measured on its home
  (`ARCHITECTURE_GPU_PREFILL_WINDOW.md`, `ARCHITECTURE_GPU_VULKAN.md`).
- Metal's five-kernel deltanet step / `DnStepFusedT`: the whole step in one dispatch is the Vulkan resident command's seat;
  Metal steps a session's rows against its own mirror.
- `MetalDnConvHist` / `DnTail`: not one computation - a shadow dual store against a zero-filled ring hand-off.
- `MetalDnBa` / `DnBaGemm`: a row GEMV against a tile GEMM.
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

**Within the Vulkan home, `KqCm2BatchT.cm2_tile` keeps its unsplit k-loop beside the split one.** At `ksplit` 0 the split
arm computes the same sums, but the unsplit arm's loop bound off the push block and its store at element zero are what the
driver compiles best: with the split arm alone the k4 l and m stamps read 1.2 to 1.5% slower on the pod (`vk_gemm_probe`
`cm2:k4`, gate 0.190 -> 0.193 ms, down 0.184 -> 0.187 ms, two alternated rounds).

**Within the Vulkan home, each fused gate-up GEMV keeps its own `gu_store`.** The q8, Q4_K and mx4 gate-up classes share
no parent that carries the output planes, and the store's body is already the shared helpers (`gk_act_mul`,
`q8_lane_scale`, `q8_quant1`); what is left per class is the plane binding.
