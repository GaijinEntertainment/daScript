# dasLLAMA Architecture - the kernel pairs of the two GPU homes

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries the ledger of the kernels
the Metal and the Vulkan home both run: which ride one template of `dasllama_gpu_kernels_common.das` and what each stamp
reads there, and which stay per home and why. The template mechanism itself stays in `ARCHITECTURE_GPU.md#gpu-shared-kernels`.

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
| `GkTopKRow` | `MetalMoeSelect` / `512` (32 lanes, DEPTH 8 / 16, `raw` the gpt-oss form) | `TopKRecords`' `TopKN` / `TopKN2` / `TopKRows` / `TopKRows2` (256 lanes, DEPTH 1 / 2) | `LANES`, `DEPTH` |
| `GkMoeCombine` | `MetalMoeCombine`, `MetalMoeReduce` (`GATHERED`) | `MoeCombine` (`WINDOW`: the chunk window, the accumulate, the zero-weight skip) | `GATHERED`, `WINDOW` |
| `GkDnGate` | `MetalDnGate` / `Sig` (`rsqrt`) | `DnScanP3` / `Sig` | `ZSIG`, `RSQRT` |
| `GkQknRopeKv` | `MetalQknRope16` / `32` (one row), `MetalQknRopeB16` / `B32` (rows), `MetalPfQkn` / `HX` (the window, the q twin) | `QknRopeKv` / `F16` | `KT`, `CLAMP16`, `V_WG`, `LANES` / `EPT`, `HX` |
| `GkRouterGemv` | `MetalMoeRouter` (a workgroup a row, the decode column), `MetalMoeRouterB` (a subgroup a row, `COL_CLAMP`, 8 columns) | `RouterGemv` / `F16` (a workgroup a row, `NORM`) | `ROW_WG`, `VW`, `NC`, `COL_CLAMP`, `NORM`, `BIAS` |
| `GkPkGemv` | `MetalPkGemv*` (f32 rows), `MetalPkGemvQ8*` | `TtsPkGemv*` (`QUAD`) | the epilogues, `QUAD`, `RSQRT` |
| `GkMoeCombine` `SH` | `MetalMoeCombineSh`, `MetalMoeReduceSh` | the combine twins' `gated` arm | `SH`: the shared row under its gate logit, the router's row `ne` |
| `GkCat2` | `MetalCat2` | `Cat2` | nothing; position- or stream-major, the a row repeated over the streams |
| `GkDnConv` | `MetalDnConvN` / `N4` (`PB` 1 / 4, `RING` 8: a group of 16 rows or fewer) | `DnConv` (`PB` 32, `RING` 4) | `PB`, `RING` |

The gated FFN's activation is one function on both homes (`gk_act_mul`, `dasllama_gpu_math.das`): the Metal ew2 and
w13 combines and every Vulkan act class call it, the gelu its clamped tanh form.

- **The inverse rms keeps each home's spelling.** The CPU rmsnorm and the Vulkan bodies spell `1 / sqrt`, the Metal bodies
  `rsqrt`, and a decode parity cell reads the two apart only past the bars it holds; a shared row template therefore carries
  the spelling as its `RSQRT` constant (`GkWgReduce.wg_rms_inv_into`, `GkDnGate`) and no body picks one for both homes.
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
per-home body before the template against the stamp, in us:

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
| router, 256 experts dim 2048, 1 column, f32 rows (f16 rows) | 3.82 | 3.37 | 4.2 (4.10) | 4.43 (4.11) |
| router, 8 columns, f32 rows (f16 rows) | - | - | 14.34 (10.24) | 14.34 (10.24) |
| router, 512 columns (the window) | 141.6 | 144.4 | - | - |
| shared gate: its own GEMV, then the routed combine and the gated add | 3.08 + 2.93 | - | - | - |
| shared gate as the router's row `ne`, the combine's `SH` arm | - | 0 + 2.01 | - | - |
| Pocket row GEMV, 1024 x 1024 | 9.67 | 9.79 | 5.4-5.8 | 5.2-5.7 |
| Pocket row GEMV, LayerNorm + SiLU | 12.79 | 12.99 | 6.14 | 6.14 |
| gated attention's deinterleave + sigmoid epilogue, 8 heads of 256, 1 row (4 rows) | 2.19 (2.23) | 0 | - | - |

The Vulkan clock reads in steps of 2.05 us at these sizes, so a Vulkan cell under 8 us moves by whole steps.
The template stamps' Vulkan words keep their helpers' bodies in place: the router's methods, the Pocket GEMV's and the
`gk_subgroup_*` shims carry `[spirv_inline]` (`../dasSpirv/ARCHITECTURE.md#emit-time-inline`), since the driver compiled
the stamp 1.9 us slower with the subgroup reductions behind calls. The router's 1-column 4.43 is the pre-template body's
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
- `MetalMoeMulMmQ8TensorT` off the kq scaffold, the ADAPT walks, `MetalG4RouterNorm`'s compensated sum, `MetalMoeWScale`
  against the top-k fold, the gate-up fusion with its Q8 requant against the f32 act, the driver chains, and the hc head
  mixer's GEMV against its tile.
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
