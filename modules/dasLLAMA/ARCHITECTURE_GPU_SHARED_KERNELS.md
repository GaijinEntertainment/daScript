# dasLLAMA Architecture - the kernels both GPU homes stamp

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries the kernels the Metal and
the Vulkan home both run: the template mechanism of `dasllama_gpu_kernels_common.das`, and the ledger of which kernels
ride one template and what each stamp reads there, and which stay per home and why.

### Kernels both homes stamp {#gpu-shared-kernels}

A kernel the Metal and the Vulkan home both run is one `class template` in
`dasllama_gpu_kernels_common.das`: its `@ssbo` buffers with their `@role`s, its argument struct and its
body. The module requires no backend and no shader vocabulary - a template's body resolves in the module
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
  stamps it, so a shared reduction calls `gk_subgroup_add` / `gk_subgroup_max`, and each kernel home
  defines the pair over its own primitive (`subgroupAdd` on Vulkan, `simd_sum` on Metal). A free function
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
| `GkMoeCombine` | `MetalMoeCombine`, `MetalMoeReduce` (`GATHERED`) | `MoeCombine` (`WINDOW`: the chunk window, the accumulate, the zero-weight skip) | `GATHERED`, `WINDOW`, `SLOT_X4` |
| `GkDnGate` | `MetalDnGate` / `Sig` (`rsqrt`) | `DnScanP3` / `Sig` | `ZSIG`, `RSQRT` |
| `GkQknRopeKv` | `MetalQknRope16` / `32` (one row), `MetalQknRopeB16` / `B32` (rows), `MetalPfQkn` / `HX` (the window, the q twin) | `QknRopeKv` / `F16` | `KT`, `CLAMP16`, `V_WG`, `LANES` / `EPT`, `HX` |
| `GkRouterGemv` | one Metal shell (`MetalRouterGemvT`: the grid onto the rows under `ROW_WG`) under two formats - the f32 rows (`MetalMoeRouterT`: `MetalMoeRouter`, a workgroup a row, the decode column; `MetalMoeRouterB`, a subgroup a row, `COL_CLAMP`, 8 columns) and the E-series side rows' projection's bf16 words (`MetalPleProjBf16T`: `MetalPleProjBf16One`, `MetalPleProjBf16`, the same two shapes) | `RouterGemv` (a workgroup a row, `NORM`), `RouterGemvBf16` (the bf16 words under `YSCALE`) | `ROW_WG`, `VW`, `NC`, `COL_CLAMP`, `NORM`, `BIAS`, `BF16`, `YSCALE` |
| `GkPkGemv` | `MetalPkGemv*` (f32 rows), `MetalPkGemvQ8*` | `TtsPkGemv*` (`QUAD`) | the epilogues, `QUAD`, `RSQRT` |
| `GkMoeCombine` `SH` | `MetalMoeCombineSh`, `MetalMoeReduceSh` | the combine twins' `gated` arm | `SH`: the shared row under its gate logit, the router's row `ne` |
| `GkCat2` | `MetalCat2` | `Cat2` | nothing; position- or stream-major, the a row repeated over the streams |
| `GkDnConv` | `MetalDnConvN` / `N4` (`PB` 1 / 4, `RING` 8: a group of 16 rows or fewer) | `DnConv` (`PB` 32, `RING` 4) | `PB`, `RING` |
| `GkMoeCombG4` | `MetalMoeCombG4` / `Hx` (the selection rows' slots, the next norm in f32, `Hx` its f16 twin), `MetalMoeCombG4Pf` (the bucket inverse's slots, no norm out; 1024 lanes, `rsqrt`) | `ClsArCombG4` / `F16B` / `Rq` (the slot map, the next norm in f32, f16 or Q8_0 blocks; 256 lanes, `1 / sqrt`) | `TG`, `RSQRT`, `GATHERED`, `OUT_F32` / `OUT_F16` / `OUT_Q8`, `HX`, `PRESUM_SWITCH`; the stash and, on Metal, the norm rows' layout |
| `GkClsEpilogue` | `MetalClsEpilogue` | `ClsEpilogue` | nothing; every row in one dispatch |
| `GkAddRmsRows` | `MetalAddRms` / `HX` (the plain step), `MetalPreAddRms` / `HX` (the sandwich seam in one dispatch: the branch normed under its post norm, the add, the next norm), `MetalPreAdd` (no norm out; 1024 lanes a decode row, 256 the window) | `ClsArAddRms` / `F16B`, `ClsArAddRmsRqB` and the `ArAddRmsRq*` stamps (Q8_0 out), the combines' `ArCombBase` (256 lanes) | `LANES`, `PRE`, `PRE_SWITCH`, `NORM_SWITCH`, `ARMS`, `X4`, `HX`, `OUT_*`; on Metal, the norm rows' layout |
| `GkPleAct` | `MetalGegluSide` (in place over the gate rows, 64 lanes) | `PleActF32` (`OUT`: the f32 projection's x, 256 lanes) | `OUT`; the side row's index is `gk_strided_at`, `ActRqT.up_at`'s |
| `GkPleFinish` | `MetalPleFinish` (`XSCALE`: the projection's 1 / sqrt(dim) in the finish, `rsqrt`) | `PleFinish` (the projection arrives scaled, `1 / sqrt`) | `XSCALE`, `RSQRT`; 256 lanes a slice on both |

The gated FFN's activation is one function on both homes (`gk_act_mul`, `dasllama_gpu_math.das`): the Metal ew2 and
w13 combines and every Vulkan act class call it, the gelu its clamped tanh form.

- **The inverse rms keeps each home's spelling.** The CPU rmsnorm and the Vulkan bodies spell `1 / sqrt`, the Metal bodies
  `rsqrt`, and a decode parity cell reads the two apart only past the bars it holds; a shared row template therefore carries
  the spelling as its `RSQRT` constant (`GkWgReduce.wg_rms_inv_into`, `GkDnGate`) and no body picks one for both homes.
- **The gemma-4 MoE step sums the routed rows with the routed combine's own sum** (`GkMoeCombine.slot_sum_into`): `GkMoeCombG4`
  derives `GkMoeCombine`, which derives `GkQ8BlockStore` for the step's Q8_0 out; the `@workgroup` slab the store declares
  sits on every combine stamp's class, read by the Q8_0 out alone.
- **The routed sum keeps four slots' loads in flight off the window** (`GkMoeCombine.SLOT_X4`, the sum's order unchanged): the
  M5's decode combine reads 1.33 against 1.81 us at dim 2048 k 8 (the gated one 1.45 against 2.01) and the gemma-4 step 12.3
  against 13.3 us, while the gemma-4 window stamp (`MetalMoeCombG4Pf`) reads 11% slower with it (137.4 against 122.6 us at
  512 rows, the two stamps raced in one process by `bench_metal_gk_rows`) and sums one slot a step.
- **The residual step's sandwich seam is one dispatch on both homes** (`GkAddRmsRows.PRE`: the branch normed under its post
  norm, the add and the next norm in one pass). The M5's 512-row window at dim 2560 reads 32.9 us against the rms + add_rms
  pair's 41.9 (the no-norm seam 22.5 against 33.4), at dim 3840 40.7 against 59.1 (28.2 against 53.0); the pod's rows read
  the same clock steps as the per-home bodies (4.14 / 6.14 / 8.19 us at dim 2560 and 3840, one and four rows). The window's
  stamps run 256 lanes: raced in one process the seam reads 27.7 us at 256 against 35.0 at 128 and 52.9 at 1024 (dim 2560),
  40.7 against 55.1 and 61.1 (dim 3840); the decode rows keep 1024.
- **The Vulkan MoE step stores its next norm four elements in flight a lane** (`GkMoeCombG4.OUT_X4`, the residual step's
  `GkQ8BlockStore.store_x4` form, which the residual step's `ArBase` stores through): with the stride loop the pod reads 12.29 against the per-home body's 10.41 us, with it 10.44.
- **The select's down-scale arm is a stamp of its own on Metal.** The arm costs the plain stamps 0.27 us a dispatch on the M5
  with no row named, so a down-scale row selects the `Ds` stamps (`ARCHITECTURE_GPU_PREFILL_MOE.md#moe-wscale`), which bind the
  layer's row and scale every pick, and the plain stamps bind no down-scale buffer; the Vulkan top-k carries the arm on its one
  stamp, its row named or `GK_NO_DSCALE` (`DSOFF_SWITCH`).
- **A value one home fixes and the other varies is a run-time read on the varying home alone.** A `*_SWITCH` constant compiles
  the read in where a site varies the field (`PRE_SWITCH`: the Vulkan sandwich arm off a layer's `pre_post`; `PRESUM_SWITCH`:
  the window step's down-GEMV route; `DSOFF_SWITCH`: the Vulkan top-k's row) and out where every site passes one value; a row
  offset the Metal home's own upload fixes (`seam_norms`, `moe_g4_norms`) is that stamp's accessor over its width, the args'
  field read only on Vulkan.
- **A one-subgroup stamp skips the workgroup stage.** `GkTopKRow` at `LANES` 32 takes its max, its sum and every pick off the
  subgroup's own reduction, the Metal select's register form; at 256 lanes the subgroups' partials cross the workgroup slab.
- **The Metal row kernels' value-form reduces sit on `GkWgReduce`'s slab** (`MetalTgReduceBase`), so a Metal row kernel and a
  shared one read the one workgroup scratch.
- **The deltanet out-norm is one subgroup a head on both homes**, the head's elements strided by the subgroup's width; the
  Vulkan form summed a head serially in one lane.
- **The grid-codebook decode GEMVs ride one shell on Metal** (`MetalKqGridGemvT`: iq3s, iq3xxs, iq2s, iq2xs, iq2xxs, dense and
  routed); a format supplies its block's term (`blk_sum`), and the iq2xxs F4 stamp its staged grid (`grid_prologue`).

**What each stamp reads on its home.** One dispatch alone, the kernel bench (`benchmarks/ew/bench_metal_gk_rows.das`, M5 Max)
and the Vulkan probe (`harness/vk_gk_rows_probe.das`, RTX PRO 4500 Blackwell), median of the rounds at under 3% cv, the
per-home body before the template against the stamp, in us (`PERF_LEDGER.md`, the 2026-10-13 entry on the shared row
templates' stamps, carries the run):

| Kernel, shape | M5 before | M5 stamp | Vulkan before | Vulkan stamp |
|---|---:|---:|---:|---:|
| hc norm, dim 2048 hc 4, 1 row | 3.87 | 3.87 | 4.19 | 4.19 |
| hc mix, 1 row | 1.75 | 1.63 | 4.06 | 4.06 |
| hc combine, 1 row | 1.45 | 1.50 | 2.50 (MoE arm) | 2.16 |
| ple gate, the ring row | 11.05 | 11.13 | 6.17 | 6.50 |
| ple conv, the ring row | 2.38 | 2.25 | 4.10 | 4.09 |
| q/k norm, hs 256 q 16 k 2 | 1.57 | 1.71 | 2.40 | 2.40 |
| select, 256 experts k 8 | 3.05 | 2.81 | 6.14 | 6.14 |
| select, 512 experts k 10 | 5.51 | 3.67 | - | 6.14 |
| routed combine, dim 2048 k 8 | 1.70 | 1.92 | 4.12 | 4.11 |
| deltanet out-norm, 32 heads of 128, 1 row | 1.28 | 1.61 | 10.24 | 6.14 |
| deltanet out-norm, 512 rows | 34.6 | 29.0 | 22.5 | 12.1 |
| q/k norm + rope + store, hs 128 q 32 k 8, 1 row f32 (the split pair before) | 3.27 | 1.89 | 4.09 | 4.09 |
| the same, 4 rows f16 | 3.66 | 1.95 | - | - |
| the same, 1 row f16, against the half-twin rope store it replaced | 1.79 | 1.93 | - | - |
| the window's pass with its q twin, 512 rows | 26.9 | 26.5 | - | - |
| router, 256 experts dim 2048, 1 column, f32 rows | 3.82 | 3.37 | 4.2 | 4.43 |
| router, 8 columns, f32 rows | - | - | 14.34 | 14.34 |
| router, 512 columns (the window) | 141.6 | 144.4 | - | - |
| E-series side rows' projection, 35 x 256 rows of 1536, bf16 rows, 1 column (the M5 before: the CPU; Vulkan before: an f16 copy of the rows) | - | 68.8 | 32.83 | 32.86 |
| the same, 4 columns | - | 79.9 | 74.15 | 74.07 |
| the same, the bf16 stamps on the router's Metal shell (`MetalRouterGemvT`; Metal before: the stamps off `GkRouterGemv` directly), the decode column's stamp / the blocked stamp at 1 and 4 columns | 46.00 / 77.40 / 79.91 | 45.14 / 77.90 / 80.49 | - | - |
| shared gate: its own GEMV, then the routed combine and the gated add | 3.08 + 2.93 | - | - | - |
| shared gate as the router's row `ne`, the combine's `SH` arm | - | 0 + 2.01 | - | - |
| Pocket row GEMV, 1024 x 1024 | 9.67 | 9.79 | 5.4-5.8 | 5.2-5.7 |
| Pocket row GEMV, LayerNorm + SiLU | 12.79 | 12.99 | 6.14 | 6.14 |
| gated attention's deinterleave + sigmoid epilogue, 8 heads of 256, 1 row (4 rows) | 2.19 (2.23) | 0 | - | - |
| select, 128 experts k 8, the down scales (Metal before: the select, then its scale pass) | 3.89 | 3.30 | 6.14 | 6.14 |
| gemma-4 MoE tail, dim 2816 k 8, 1 row (Metal before: the combine, three norms, the add, the add-norm) | 17.5 | 12.3 | 10.41 | 10.44 |
| the same, 512 rows (Metal before: the reduce, three norms, two adds) | 182.4 | 122.5 | - | - |
| classifier epilogue, vocab 262144 cap 30 two pins, 1 row (Metal before: the softcap row, then the suppress row) | 6.46 | 3.68 | 4.10 | 4.10 |
| the same, 4 rows (Metal before: the pair a row) | 25.75 | 10.65 | 6.15 | 6.15 |

The Vulkan clock reads in steps of 2.05 us at these sizes, so a Vulkan cell under 8 us moves by whole steps.
The template stamps' Vulkan words keep their helpers' bodies in place: the router's methods, the Pocket GEMV's and the
`gk_subgroup_*` shims carry `[spirv_inline]` - the SPIR-V emitter visits such a helper's body at each call site, so the
stamp pays no function call for it - since the driver compiled the stamp 1.9 us slower with the subgroup reductions
behind calls. The router's 1-column 4.43 is the pre-template body's
own reading through the same probe (4.40); the 4.2 is that body's own probe.

**A gated model's q stays in its `[q | gate]` projection rows on both homes.** The Metal decode and verify rope stores
rope (tq4: and rotate) q at the head's `2 hs` stride, the attention stamps read it there and scale their stores by the
gate's sigmoid (`sq_gated`), and the split forms' partials carry the gate, since the combine is linear; under tq4 the
attention runs ungated and the un-rotation gates (`MetalFwhtUnsign`'s `qg`). A codec past the fused and block-codec rope
stores keeps the deinterleave and the epilogue - the two dispatches the row above retires a layer a step.

No row parts the homes past a tenth of a percent of a decode step: the widest move is the 1-row out-norm's 0.33 us on the
M5 against the 512-row window's 5.6 us back.

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
