# dasLLAMA Architecture - what the shared stamps read

Companion to `ARCHITECTURE_GPU_SHARED_KERNELS.md`; a section is cited by its anchor. This document carries the readings
behind the shared row templates: the instrument each figure comes from, the shapes the readings chose, and what each
stamp reads on its home against the per-home body before it.

### The shared stamps' readings {#gpu-shared-readings}

**Where the figures below come from** (the bullets and the table, every reading `debug-jit`, at under 3% cv). Metal:
`./bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/ew/bench_metal_gk_rows.das` on the M5
Max under `DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/m5.tune.json`, `DAS_TUNE_POLICY` unset - direction-grade, since the
box profile predates the binary; a race is its `race <kernel>` lines (the arms in one process, an arm's best round of at
least 9), any other row the median. Vulkan: `./bin/daslang -jit modules/dasLLAMA/harness/vk_gk_rows_probe.das` on the RTX PRO
4500 Blackwell pod, `DASLLAMA_COOPMAT` unset, the median of 9 rounds of 8 submits of 64 dispatches; the device clock steps by
2.05 us at these sizes, so a cell under 8 us moves by whole steps. A bullet's pair is raced in one process.

- **The inverse rms keeps each home's spelling.** The CPU rmsnorm and the Vulkan bodies spell `1 / sqrt`, the Metal bodies
  `rsqrt`, and a decode parity cell reads the two apart only past the bars it holds; a shared row template therefore carries
  the spelling as its `RSQRT` constant (`GkWgReduce.wg_rms_inv_into`, `GkDnGate`); the two bodies that spell one for every
  home are the layernorm seam's (`GkPostAddLn`, `1 / sqrt`) and the tower's `GkLnReluRows` (`rsqrt`), whose cells hold on
  either.
- **The gemma-4 MoE step sums the routed rows with the routed combine's own sum** (`GkMoeCombine.slot_sum_into`): `GkMoeCombG4`
  derives `GkMoeCombine`, which derives `GkQ8BlockStore` for the step's Q8_0 out; the `@workgroup` slab `GkWgReduce`
  declares sits on every combine stamp's class, the four reductions of the gemma-4 step reading it.
- **The routed sum keeps four slots' loads in flight off the window** (`GkMoeCombine.SLOT_X4`, the sum's order unchanged), and
  the gemma-4 window stamp (`MetalMoeCombG4Pf`) sums one slot a step. On against off on the M5, `debug-jit`: the decode combine
  at dim 2048 k 8 reads 1.28 against 1.82 us at one row and 1.31 against 1.91 at four (the gated one 1.42 against 1.98, 1.46
  against 2.02); the gemma-4 decode step at dim 2816 k 8 12.01 against 13.36 at one row and 12.06 against 13.50 at four (`Hx`
  11.98 against 13.33, 12.03 against 13.43); the window's reduce at 512 rows of 2048 38.10 against 38.86 (the gated one 41.83
  against 42.56); the gemma-4 window stamp 136.91 against 121.38.
- **The residual step's sandwich seam is one dispatch on both homes** (`GkAddRmsRows.PRE`: the branch normed under its post
  norm, the add and the next norm in one pass). The M5's 512-row window, `debug-jit`, at dim 2560 reads 27.51 us against the
  rms + add_rms pair's 36.56 (the no-norm seam 18.79 against 39.04), at dim 3840 40.69 against 55.80 (27.74 against 56.72).
  The Metal stamps stage a row's first 4096 columns through the threadgroup slab (`MetalAddRmsT.SLAB`) and re-read the
  columns past it from the stream rows the step wrote and the partner rows, so every width takes the one dispatch; the
  window's stamps run 256 lanes at every width, the decode builders 256 within the slab and 1024 past it (the `w` builders,
  `enc_residual`'s pick). The figures from here to the bullet's end are one run of the bench at 5677d5b4da
  (`DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_CPU_PREFILL=1 DASLLAMA_METAL_HAZARD_STRICT=1 DAS_LOG_LEVEL=info` beside the manifest).
  The lane race (`seam_lane_race`, one / four rows of the plain pipeline, 512 rows of the f16 twin's; 256 / 1024 / 128
  lanes, us): the decode rows at dim 1152 3.24 / 3.35, 5.66 / 5.72, 4.28 / 4.43; at 2560 4.96 /
  5.10, 6.19 / 6.25, 7.87 / 8.14; at 3840 6.75 / 6.91, 6.66 / 6.73, 11.38 / 11.66; at 5376 8.76 / 8.99, 7.48 / 7.59, 15.31
  / 15.68. The window at 1152 9.26, 46.17, 11.87; at 2560 27.65, 54.62, 35.23; at 3840 40.82, 63.44, 55.33; at 5376 57.22,
  75.24, 80.41. Past the slab the one dispatch against the split chain (the rms, the scaled add, the next norm; us, one /
  four decode rows, then the window): the seam at 5376 7.48 / 7.58 against 9.59 / 10.00, 58.94 against 125.3; at 8192 8.60 /
  8.72 against 10.84 / 11.39, 125.3 against 227.6; the no-norm seam at 5376 4.48 / 4.54 against 5.31 / 5.61, 39.46 against
  83.11, at 8192 5.20 / 5.28 against 5.96 / 6.39, 64.02 against 134.3; the plain step at 5376 4.30 / 4.39 against the add
  and the norm's 5.31 / 5.62, at 8192 5.01 / 5.14 against 5.96 / 6.38. The decode rows' one dispatch on the 256-lane
  builders past the slab reads 8.76 / 9.00 at 5376 and 12.62 / 12.94 at 8192 for the seam, slower than the split chain at
  8192. A bench stamp whose slab holds the 31B's whole row (`BenchPreAddRmsWide`, 5376 floats) reads with the re-read
  form at the 256-lane decode rows (8.76 / 8.98) and 55.35 against 58.94 on the window, under a tenth of a percent of a
  window, so the stamps keep the one slab. A second run of the bench at b4b207a287 (the same knobs) times the low end of the
  range past the slab, the window's plain step and the decode rows' f16-twin stamps (`enc_preaddrms_bhxw_c`,
  `enc_add_rms_bhxw_c`, which a rows site carrying the twin dispatches), one dispatch against the split chain, us: the seam at
  4608 7.13 / 7.21 against 9.26 / 9.52 and 49.10 against 101.4 on the window, at 5120 7.40 / 7.50 against 9.48 / 9.83 and
  55.05 against 116.0 (the four-row 7.50 a repeat run's, the first VOID at cv 4 %); the no-norm seam at 4608 4.23 / 4.30 against 5.09 / 5.37 and 33.83 against 70.17, at 5120 4.39 / 4.49
  against 5.26 / 5.54 and 37.62 against 78.30; the plain step at 4608 4.10 / 4.17 against 5.12 / 5.35, at 5120 4.27 / 4.33
  against 5.27 / 5.51; the window's plain step 43.90, 49.19, 52.60 and 114.9 at 4608, 5120, 5376 and 8192 against 73.88,
  83.07, 87.29 and 136.8. With the twin the seam reads 7.12 / 7.21 against 9.29 / 9.52 at 4608, 7.41 / 7.50 against 9.61 /
  9.93 at 5120, 7.48 / 7.58 against 9.66 / 9.98 at 5376 and 8.56 / 8.71 against 10.83 / 11.33 at 8192, and the plain step
  4.33 / 4.41 against 5.14 / 5.33, 4.54 / 4.60 against 5.31 / 5.53, 4.55 / 4.64 against 5.35 / 5.58 and 5.28 / 5.40 against
  5.97 / 6.38 at the same widths; the 256-lane builders past the slab read slower than the 1024-lane ones at every width
  and form, with the twin and without.
- **The Vulkan MoE step stores its next norm four elements in flight a lane** (`GkMoeCombG4.OUT_X4`, the residual step's
  `GkQ8BlockStore.store_x4` form, which the residual step's `ArBase` stores through): the pod reads 10.37 us with it against
  12.29 with the stride loop (`ProbeArCombG4X1`), `debug-jit`.
- **The select's down-scale arm is a stamp of its own on Metal.** With no row named the arm costs the plain stamps 0.20 us a
  dispatch on the M5, `debug-jit` (2.83 against 3.04 us at 256 experts k 8, 3.65 against 3.90 at 512 k 10), so a down-scale
  row selects the `Ds` stamps (`ARCHITECTURE_GPU_SHARED_ROWS.md#moe-wscale`), which bind the layer's row and scale every pick,
  and the plain stamps bind no down-scale buffer; the Vulkan top-k carries the arm on its one stamp, its row named or
  `GK_NO_DSCALE` (`DSOFF_SWITCH`).
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

**What each stamp reads on its home.** One dispatch alone, in us, the per-home body before the template against the stamp. A
before cell is the same instrument at the parent of the commit that brought its row in, `direction-grade`: `4459db710d` for the
rows the templates landed with (`PERF_LEDGER.md`, the 2026-10-13 entry on the shared row templates' stamps), `5d3db7852d` for
the router's one- and eight-column f32 rows, `1d357ad171` for the projection's Vulkan rows, `611775848f` for its fold onto the
router's Metal shell, `7f5c27186a` for the gemma-4 select, MoE tail (Vulkan) and classifier epilogue rows; the gemma-4 MoE tail's
M5 before cells are the unfused chain raced in one process. The projection's f32 rows are
its in-process alternate on both homes:

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
| E-series side rows' projection, 35 x 256 rows of 1536, bf16 rows, 1 column (the M5 before: the CPU; Vulkan before: an f16 copy of the rows) | - | 45.47 | 32.83 | 32.86 |
| the same, 4 columns | - | 79.98 | 74.15 | 74.07 |
| the same, the f32 rows (the alternate), 1 / 4 columns | - | 72.83 / 149.11 | - | 34.79 / 94.15 |
| the same over a ring of 8 weight copies, bf16 / f32 at 1 column, then at 4 | - | 56.08 / 103.30, 80.14 / 163.00 | - | 45.59 / 69.61, 83.86 / 108.44 |
| the same, the bf16 stamps on the router's Metal shell (`MetalRouterGemvT`; Metal before: the stamps off `GkRouterGemv` directly), the decode column's stamp / the blocked stamp at 1 and 4 columns | 46.00 / 77.40 / 79.91 | 45.14 / 77.90 / 80.49 | - | - |
| shared gate: its own GEMV, then the routed combine and the gated add | 3.08 + 2.93 | - | - | - |
| shared gate as the router's row `ne`, the combine's `SH` arm | - | 0 + 2.01 | - | - |
| Pocket row GEMV, 1024 x 1024 | 9.67 | 9.79 | 5.4-5.8 | 5.2-5.7 |
| Pocket row GEMV, LayerNorm + SiLU | 12.79 | 12.99 | 6.14 | 6.14 |
| gated attention's deinterleave + sigmoid epilogue, 8 heads of 256, 1 row (4 rows) | 2.19 (2.23) | 0 | - | - |
| select, 128 experts k 8, the down scales (Metal before: the select, then its scale pass) | 3.89 | 3.30 | 6.14 | 6.14 |
| gemma-4 MoE tail, dim 2816 k 8, 1 row (Metal before: the combine, three norms, the add, the add-norm) | 17.29 | 11.98 | 10.41 | 10.44 |
| the same, 512 rows (Metal before: the reduce, three norms, two adds) | 188.75 | 120.30 | - | - |
| classifier epilogue, vocab 262144 cap 30 two pins, 1 row (Metal before: the softcap row, then the suppress row) | 6.46 | 3.68 | 4.10 | 4.10 |
| the same, 4 rows (Metal before: the pair a row) | 25.75 | 10.65 | 6.15 | 6.15 |

The template stamps' Vulkan words keep their helpers' bodies in place: the router's methods, the Pocket GEMV's and the
`gk_subgroup_*` shims carry `[spirv_inline]` - the SPIR-V emitter visits such a helper's body at each call site, so the
stamp pays no function call for it - since the driver compiled the stamp 1.9 us slower with the subgroup reductions
behind calls. The router's 1-column 4.43 is the pre-template body's
own reading through the same probe (4.40); the 4.2 is that body's own probe.

No row parts the homes past a tenth of a percent of a decode step: the widest move is the 1-row out-norm's 0.33 us on the
M5 against the 512-row window's 5.6 us back.
