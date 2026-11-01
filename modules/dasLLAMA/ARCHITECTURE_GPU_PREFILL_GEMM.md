# dasLLAMA Architecture - the Metal prefill driver's GEMM forms

Companion to `ARCHITECTURE_GPU_PREFILL.md`; a section is cited by its anchor. This document carries
the GEMM form ladder every weight site picks from, the short window's split-K form, the dev-W panel knee
map, the GEMV tail peel and the dense-KQ tensor mul_mm scaffold.

### The prefill GEMM form ladder {#prefill-gemm-ladder}

Every weight GEMM in `dasllama/dasllama_metal_prefill.das` splits its rows across two decisions.
First the GEMV tail peel (`ARCHITECTURE_GPU_PREFILL_GEMM.md#gemv-tail-peel`) takes up to `MM_TAIL_MAX` remainder rows off the padded tile.
The rows that remain pick one of five forms, in this order, per site per forward (a K-quant site tries the
split-K form ahead of dev-W; the q8 blob's dev-W floor sits far above the split-K knee):

1. **tall in-kernel-dequant (K-quant deep class)** - a K-quant site whose f16 panel would
   reach `TALLKQ_MIN_PANEL` (96 MiB) is DRAM-resident, and there the `KqMulMm*TH128` stamp
   wins: it dequants its own W tile in threadgroup memory and reads the quant plane
   (~0.56 B/element) once per 128-row tile, where a materialized f16 panel writes
   2 B/element and re-streams them per tile. The knee is panel SIZE, not tile count - a
   118 MiB panel wins 15-25% on the tall stamp while an 84 MiB one measures a small loss
   (`benchmarks/lcpp_bench.das -p 512 -n 0 -r 5 --ngl 99` A/B with the gate forced per run,
   gemma-4-12B ffn and Qwen3.8-27B qkv carriers, m5).
   The 32-row TH stamp covers the row remainder at its X/y offsets.
2. **dev-W all-device** - the weight plane is dequantized into a device f16 panel and multiplied
   half x half. No threadgroup staging and no barriers, so the staged-operand tax is gone; the
   dequant pass is paid once per site per forward against a GEMM that re-reads the f16 W panel
   once per M row tile (`mp/128` on the tall stamp, `mp/32` on the 32-row stamp, where `mp =
   ceil32(npos)` is the padded row count). `ARCHITECTURE_GPU_PREFILL_GEMM.md#devw-panel-knees` carries the panel size rules.
3. **tall 128-row M-tile** - the stamp streams W `M/128` times over a 128-row tile, taken on the
   row count's 128-floor with the 32-row stamp on the remainder. The remainder arm strides X by
   `kdim`, so a caller that passes no `kdim` takes the tall stamp only when its row count is
   already a multiple of 128, and otherwise stays whole-dispatch 32-tile.
4. **split-K 32-row tile** - a grid at or under the split-K knee divides each tile's K
   (`ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k`).
5. **32-row tile** - the default stamp.

**The dense staged stamps double-buffer their W staging** (`DASLLAMA_METAL_DBUF=0` is the
single-tile rail): two 64-deep ping-pong tiles hold 18432 B of threadgroup memory - probed at
the allocation law's edge, 0-2% tax by shape - and the loop runs ONE barrier per chunk, which
both publishes the tile staged last iteration and fences the previous `op.run` off the tile
about to be overwritten; staging chunk b+1 overlaps the tensor op on chunk b. Raced per
family (`benchmarks/lcpp_bench.das -p 512 -n 0` A/B with `DASLLAMA_METAL_DBUF` forced per
run, m5): the staged-q8 stamps +1.7% end-to-end on 8B pp512 (3435 -> 3492 tok/s), the kq
in-kernel-dequant stamps +1.8% on gemma-4-12B Q4_K_M (2040 -> 2077); the MoE q8 expert
stamps measured -5.0% (Qwen3-30B-A3B q8) - the expert z-grid's many small dispatches pay
the 18432 B occupancy cost the 2-D dense grids do not - so the MoE staged family stays
single-tile. Outputs are bit-equal to the single-tile forms because the staged f16 rounding
and chunk order are identical (the k6 stamp pins the factored `dsc*(q-32)` like its dequant
kernel, so the equality holds by source). The pattern is the one Apple ships in MLX's
fp-quantized NAX family at this exact tile size.

A threadgroup-memory REQUEST size moves the clock on its own, independent of how much of it
the kernel indexes: 10240-16384 B is a flat plateau, 9216 B a pothole, and 20480 B and above
degrade (`harness/p2_alloc_gap_probe.das`'s allocation ladder). A staging-depth race is therefore a 2x2 - tile stride against allocation - and the
deeper tile is judged at equal allocation.

**A swiglu epilogue folded into the gate GEMM's store is REFUTED on M5.** Any per-element
access of the cooperative C after `op.run` - the scattered-store walk, a modify-in-place
before the optimized `cT.store`, even a cooperative-load register combine of the up panel -
costs ~+340us on the 512x12288 gate GEMM (940us plain), 2.2x the standalone swiglu pass the
fold deletes (~154us, 8B end-to-end +11.9ms fused vs unfused). The stall is structural to the
tensor-op pipeline, not addressing: MPP tolerates elementwise epilogues only at that price,
which is consistent with MLX shipping its epilogue slot unused. Elementwise fusion pays only
kernel-to-kernel (rms_hx, swiglu_hx, add+rms), never inside a tensor-op store.

**Half operands ride an f16 activation panel.** One pass converts the f32 panel; the GEMM then
re-reads it at half the bytes `d/64` times, so the convert amortizes above a row floor
(`CVT_MIN_ROWS`, default 256) and loses under it. Two panels alternate, so one site's GEMM reads
panel A while the next site's convert writes panel B. A site whose producer kernel writes the
f16 copy alongside its f32 output takes its panel from `pf_twin_panel` directly instead of
`pf_cvt_panel`, skipping the row floor entirely, because there is no convert pass to amortize.

**The bf16-A stamp widens without dequantizing** - a bf16 row widens by an exact bit shift and
rounds to the f16 tile - so the E-series `per_layer_model_proj` GEMM serves straight off the
kept-bf16 blob with no resident f32 copy.

**The tall stamp is a per-box crown behind an occupancy floor.** Every tall arm (the dev-W
panel GEMM, the q8 and K-quant tensor twins, the tower's f16 route) opens only where the mint
crowned `hmm_tall`: the race (`race_hmm_tall`) times the 128-row half GEMM against its 32-row
form at a 1B-class site (512 rows, 2048 x 2048), and the M5 Max crowns it while the M4 Pro
declines - its tensor tiles idle in the tall grid and Llama-1B prefill reads 5% under the 32-row
form there, the 30B 1% (`PERF_LEDGER.md`, the M4 Metal pass). Crowned, a tall grid is taken only when
`rows/128 * (d/64) >= TALL_OCC_FLOOR` (default 64, a sidecar knob): an under-occupied tall grid
starves the GPU and small prompts regress hard without the floor.

### The short window's split-K form {#prefill-split-k}

A window of 9 to about 128 rows launches few 32-row tiles: `mp/32 * d/64` threadgroups, 16 to 48 at a 0.6B layer's sites
on a 40-core GPU, each walking its whole K serially under barriers, so the GEMM is latency-bound rather than bound by its
weights. The split-K form gives the grid a third axis of `nsk` threadgroups that share a tile's K, each walking `K/nsk` and
storing its own partial plane of the scratch; one reduce dispatch (`MetalSkReduce`, the batched decode's split-K reduce)
sums the planes into the output. Every 32-row tile family carries the axis as a template constant (`SK`) on its own stamp
body, so the split adds no per-format code: the q8 blob (`MetalQ8MulMmTensorT`'s `THSk` / `TSk`), every dense K-quant, grid
and per-32 format (the split scaffold's `db_shell`, stamped `kq_mm:<fmt>:thsk` / `tsk`), bf16 (`MetalBf16MulMmTensorT`'s
`THSk` / `TSk`), the dev-W half tile (`MetalHalfMulMmTHSk`, its partial planes a column tile's own width, summed into the
site's wider rows by the reduce's strided mode, `SkReduceArgs.cols4` / `ld4`), and the simdgroup base tiles a box with no
tensor crown serves (`MetalQ8MulMmSk`, `MetalKqMulMm<Fmt>Sk`, compiled and released by `dasllama_metal_kernels`, which
declares them, and picked through its per-format builder tables). The q8 split stamps hold 32 output columns a
threadgroup, the others 64: the narrower q8 tile doubles the grid again, and the K-quant stage16 decode is laid out for 64
rows. A shared K the planes sum in another f32 order than one walk, so the forms agree to rounding, not bit for bit.

The rule is `dasllama_gpu_knee`'s, the one the Vulkan home serves too: a site takes the form while its grid is at most half
its family's knee, and its split count doubles from one while the grid times the count stays at or under the knee, up to
eight, each share a whole number of 64-wide chunks and at least four 32-blocks of K (eight on the K-quant scaffold, whose
shorter shares lose to their reduce). Each family's knee is a box knob cached at init: `metal_split_k_tgs` (the q8 blob
and bf16), `metal_split_k_tgs_kq` (the K-quant scaffold), `metal_split_k_tgs_base` and `metal_split_k_tgs_kq_base` (the base
tiles), `metal_split_k_tgs_hmm` (the dev-W half tile); 0, or `DASLLAMA_METAL_SPLIT_K=0` for every knob, keeps the tiles
whole. The K-quant knee sits higher because its stage decodes more per element, so a wider grid still waits on its serial
K walk; past a knee the grid fills the GPU and the reduce is pure cost. The scratch is two alternating buffers sized for
the largest knob's planes. Past the split knee the q8 blob's 32-column stamp still takes a grid of few M tiles at one way -
no split, no reduce - while the padded rows are at most `metal_q8_n32_rows`: the narrower tile doubles a wide site's grid.
A window of 16 rows or fewer that one 32-row tile holds takes the 16-row split stamps (`THSk16` / `TSk16`, the same bodies at
`MT = 16`): the tensor op at 16 rows costs about 0.69 of the 32-row op the padding would pay.

**A mint races the knobs** (`metal_split_knee_race`, run by `harness/tune_kernels.das` ahead of its CPU benches and by a fat
exe's first start, which write them into the sidecar's runtime section): synthetic planes, no model. The race scaffold is
the kernels file's (`knee_split_rungs`): a 32-row tile over the shared rung tables (`KNEE_Q8_WIDTHS` at K 2048,
`KNEE_KQ_WIDTHS` at K 5120), each step two independent sites issued on a concurrent encoder - the way a window issues a
layer's co-running sites - their weights streamed from a ring of copies past the SLC, outputs alternating, the tile against
its split stamp at two, four and eight ways; `knee_pick` prices every candidate knob by the time the production rule gives
each grid and the cheapest total wins. Each knee races the kernels it serves: the q8 knee the q8 tensor tile and the bf16
plane's, priced together; the K-quant base knee a base arm a scale layout (k4, k6, iq4xs, q51); the half tile's knee its own
stamp. The one-way rows race the 8B down site (14336 x 4096) as a co-running pair at 32 to 128 rows and stop at the first
loss. Each race lives in the file declaring the classes it races (the base tiles' in `dasllama_metal_kernels`, the tensor,
bf16 and half tiles' in `dasllama_metal_prefill`). The measured knees, windows and served rows are `PERF_LEDGER.md`'s split-K
entry.

### The dev-W panel knee map {#devw-panel-knees}

Panel SIZE dominates the dev-W decision, not shape - each threshold is a knee, the panel size at which the dev-W win flips
sign. A panel at or under `DEVW_SMALL_PANEL` (32 MiB) engages at every row count while its output width is at or under
`DEVW_WIDE_N` (4096), and from `DEVW_WIDE_N_ROWS` (64 rows) when wider; a panel up to `DEVW_BIG_PANEL` (48 MiB) engages from
`DEVW_BIG_ROWS` (2048 rows); above that the panel either splits into N-column tiles or the site declines. The mechanism: the
f16 panel's W stream is 2 bytes an element against the q8 blob's 1.06 once it leaves cache, and a wide-N panel costs more
dequant work, which only a large row count repays. The raced evidence (32 MiB wins everywhere, 47 MiB only from 2048 rows, 112 MiB loses everywhere) is
`benchmarks/matmul/bench_metal_nax_probe.das`'s grid.

**Every dense site takes a scratch panel from its row floor.** The scratch pair is the window's working memory - a site's
panel dequantized from its own bytes inside the window, the pair reused by the next one - so it is no converted copy
(`REVIEW_KQ_FORMATS.md`); a STORED panel, baked into an image or resident across windows, is one, and only the lever builds
it (`set_metal_devw_panels`, `DASLLAMA_METAL_DEVW=1`; `ARCHITECTURE_IMAGE_GPU_PLANES.md#image-devw-plane`). The floor
(`devw_scratch_floor`) is `DEVW_SCRATCH_ROWS` (64 padded rows) where the site has no crowned tensor form - the base tiles
re-dequantize W every 32-row tile - and where its twin is crowned, the band the twin holds: `DEVW_KQ_TWIN_ROWS` (384) on a
K-quant site, `DEVW_Q8_TWIN_ROWS` (2048) on a Q8_0 one. Scratch / own-bytes window ratios (`benchmarks/prefill_window_probe.das
--cold --ab devw`, M5 Max, `DASLLAMA_METAL_TENSOR=0` uncrowned): Qwen3.6-35B-A3B IQ4_XS uncrowned 0.89 / 0.74 / 0.69 at 128
/ 256 / 512 rows and 0.62 at 2967, crowned 0.96 at 384-512 and 0.91 at 2967; Llama-3.2-3B Q4_K_M uncrowned 0.66 / 0.48 at
128 / 512, crowned 0.91 / 0.85 at 384 / 512; Llama-3.1-8B Q8_0 uncrowned 0.96 / 0.89 / 0.44 at 128 / 512 / 1024, crowned
1.00 / 0.96 / 0.93 at 1024 / 2048 / 4096. The IQ4_XS file's 2967-row window spends 6.6 ms in its dequant passes - 2.68 GB of
panel writes at the memory's rate - and the server's window line counts them (`devw_scratch=<sites>/<MB>`).

**A scratch site also needs a grid.** A big panel splits into column tiles the window runs one after another through the
scratch pair, and a tile's half GEMM launches `rows/32 x td/64` threadgroups over its whole K: a 27B's down (5120 x 17408, eight
640-wide tiles) launches 20 at 64 rows, walking 17408 deep. The half tile splits its K under its own knee
(`#prefill-split-k`), and a site whose tile grid stays under `metal_devw_min_tgs` keeps its quantized tiles
(`pf_devw_grid_short`). The floor race (`knee_race_devw`) times dense k4 sites at a 1B-to-27B model's gate and down shapes, 64
and 128 rows, each in the column tiles its panel takes, the quantized tile (split under the base knee) against the dev-W
route the window encodes - the tiles in turn, the two sites one after the other since they share the scratch pair - and
`knee_cut_pick` prices every rung's grid as the cutoff; a box where no rung's dev-W route wins sets the floor past every grid
(the route off). The floors and the windows they move are `PERF_LEDGER.md`'s split-K entry.

**Under the lever, resident panels remove the re-dequant** the scratch pair pays every window (8B, 512 rows: ~4.5 GB of
panel writes and quant reads, ~7.7 ms, `harness/p0_ko_probe.das`'s `devw_cvt` knockout). A resident site is one full-panel
GEMM at a `(buffer, offset)` seat - N-column tiling exists only for the scratch pair's capacity - keyed by its quant-plane
buffer plus its offset in the plane's own unit (q8: mblob bytes; kq: elem woff) with the format folded in, so the unit spaces
cannot alias, and a hit serves only at the seeded byte size; the cache is released on driver shutdown, weight refill and a
weights-epoch bump. An image-served model seeds it ZERO-COPY off its baked `devwf16` plane; an owned load dequantizes once
under the `DASLLAMA_METAL_DEVW_RESIDENT` MB budget (past it, scratch, warned once). 8B, 512 rows: 162.1 -> 153.8 ms, both
ways (`benchmarks/lcpp_bench.das -p 512 -n 0`, m5).

Clauses the isolated grid cannot see, because it races one site while production overlaps
sites:

- **A long-K (down-projection) dequant serializes** on the panel pair behind the up/gate GEMM chain at small M, so the
  long-K floor is 1024 rows on q8: the tiling that wins its isolated site race at 512 rows measures zero to negative end
  to end (`../benchmarks/lcpp_bench.das -p 512 -n 0` A/B with the form forced per run).
- **An over-knee panel runs as N-column TILES**, each under the small-panel knee, with the tile
  count bounded by `DEVW_MAX_TILES` (32), divisibility, the small-panel knee and the pool.
  Narrow tiles measure fine in the same A/B. Under the K-quant seat a site lowers the over-knee bar, the tiled-rows
  floor and the long-K floor (1024 rows to the wide-N floor), and leaves dev-W for the tall stamp past `TALLKQ_MIN_PANEL`.

`CVT_MIN_ROWS`, `TALL_OCC_FLOOR`, `DEVW_SMALL_PANEL` and `SPLIT_K_TGS` are box-raced and cached at init from the sidecar
(`metal_cvt_min_rows`, `metal_tall_floor`, `metal_devw_small_panel_mb`, `metal_split_k_tgs`); the routed block's gathered-route knee
(`metal_moe_gemv_max`, `ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-gemv-route`) is read per window; the other knees are fixed.

### The GEMV tail peel {#gemv-tail-peel}

A prefill panel pads to `mp` rows, so `npos % 32` rows of every GEMM are padding. Up to `MM_TAIL_MAX` (8) remainder rows peel
off the padded tile onto the fixed-B mv family instead; above that the padded tile is cheaper than three or more weight streams.
Where the split-K form takes the padded tile, the peel reaches `MM_TAIL_SPLIT_MAX` (1) row past whole tiles and
`MM_TAIL_SPLIT_WINDOW` (4) over a window of no whole tile - the rows one batched GEMV dispatch carries; a second dispatch is a
second weight stream, which the split tile's padding undercuts (the readings are `PERF_LEDGER.md`'s split-K entry).
One peeled row rides the reduction-split GEMV, two or more ride the b4 form only - the reduction-split GEMV walks per block and
needs `kdim % 32`, while the b4 form - the batched fixed-B mv stamp, up to four rows a dispatch (`enc_mv_b4_c`) - reads whole
128-quant rounds and needs `kdim % 128`. That peel is the q8 blob's. A dense K-quant, grid-format or per-32 site (`pf_enc_kq_site_mm`)
peels a whole window of `MM_TAIL_MAX` rows or fewer - `MM_TAIL_SPLIT_WINDOW` where its split-K form would take the window - onto
the decode's row-batched forms (`enc_kq_site_b`: the B2, B4 and B8 stamps, which stream the plane once for every row) on a
`kdim` of whole blocks (`kq_unit`), where the 32-row tile would walk K serially under barriers one threadgroup per core. Past
eight rows it peels the remainder past whole tiles onto the same row forms at the remainder's x and y offsets - one row under the
split-K knee, up to `MM_TAIL_KQ_PAST_KNEE` (2) past it - and the rest stays on the tile. Every row form binds x at a byte offset
(the shared x view `MetalKqXT`, the per-32 `MetalB32MvbT`), so a remainder needs no copy. Past the knee two rows win on the
row forms, four tie and eight lose (`PERF_LEDGER.md`'s split-K entry).

### The dense-KQ tensor mul_mm scaffold {#prefill-kq-tensor-scaffold}

Every dense format but q8 and the K-quant deep class shares ONE tensor mul_mm body,
`MetalKqMulMmSplitTensorBase`, the k6 tensor shell: the `tmm2d_tg_*` accumulate loop, the 6144-half `twb` W chunk, the store. It exposes
exactly one overridable stage, `stage16` - 16 elements per work item, decoded into `twb`. A
format derives, binds its own weight views, and overrides `stage16` alone. 16 is not an
arbitrary granularity: it is the base GEMV arm's own, so each format's decode ports into its
`stage16` verbatim, the arm's `va[]` store becoming a `twb` store - the MoE twins' shape
(`ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-buckets`) applied to the dense sites; as there, the q8 twin
stays its own template (a different staging).

Each format's `stage16` takes one of three forms, inherited from that format's base GEMV arm:

- **byte-parallel compose** (k3, k2, and the q5 arm of k45) - masked uint-wide shifts produce
  one `qv` per four elements, with the byte-position shift folded into exact power-of-two
  pre-scales. It measures 1.4x a per-element bit compose
  (`benchmarks/matmul/bench_metal_kq_race.das --tiers mm`, m5).
- **staged-grid slab** (iq3s, iq3xxs) - the 2 KB / 1 KB grid staged into threadgroup memory
  once per threadgroup.
- **direct constant-table gather** (iq2s, iq2xs, iq2xxs) - the u64 grid pair read straight off
  the hoisted tables, no slab. q40 is iq4nl's stamp (`Q40`) with the linear map in the LUT slot.

Every format stamps the same five forms - `T` (`XT = float`), `TH` (`XT = float16`), the `TH128`
tall stamp (form 1) and the double-buffered `THDb` / `THDb128` pair off the scaffold's one
`metal_kq_mulmm_db` kernel - the per-32 stage (`MetalKqMulMmB32TensorT`) indexing its 32-blocks
off `kdim`. Every stamp compiles only behind its own crown (`metal_tensor_crowned("kq_mulmm_<fmt>")`),
`pf_enc_kq_dense_site` dispatches a twin only when both the crown flag and the PSO are live, and
`pf_kq_dense_stamps` is the one table a format maps to its crown, psos and builders through, and
`check_kq_form_matrix` (`REVIEW.das`) holds every format to the same form set; a per-32 site takes a twin
or the dev-W arm only on a K of whole 64-wide chunks (`b32_twin_fits`). The crown is a RACE verdict (tensor stamp vs simdgroup base
at one dense shape); whether the box can run tensor kernels at all is a separate fact, the
toolchain probe `g_pf_tensor_ok` (one quiet compile of the dev-W GEMM at init). A box whose
Metal has no mpp kernels fails the probe and keeps every base form whatever its sidecar says.

The same classes carry the family's dev-W dequant pass (form 2): `MetalKqDequant<Fmt>` derives
from the format's tensor class, sets the base's `DEQW` axis (which gates the f16 panel binding
at 7) and takes the scaffold's `metal_kq_dequant` kernel, which runs the inherited `stage16`
over one 64x64 chunk per threadgroup into `twb`, then stores the tile into the panel; the
site dispatcher offers these formats the dev-W arm ahead of their staged stamps. The staged
stamp re-dequantizes W per 32-row tile, the whole gap on a small model: the 1B split-scale files
read 0.85-0.93 of llama.cpp's prefill staged, 1.05 on scratch dev-W, 1.23-1.24 resident
(`followup_metal.md` sec.7, the Llama-3.2-1B format matrix on the M5 Max).
The dev-W arm rides the toolchain probe alone, never a crown: the shared all-device half GEMM
and every format's dequant pass compile wherever `g_pf_tensor_ok` holds, and `pf_enc_kq_site_mm`
offers the arm before it reads any `kq_mulmm_<fmt>` crown. The q8 site takes the same arm through
`pf_q8_devw` while the q8 tensor twins stay behind their own crown. Dev-W's win is the one-time
dequant, not the tensor lane, so a box whose dense race crowned nothing still gets it.
