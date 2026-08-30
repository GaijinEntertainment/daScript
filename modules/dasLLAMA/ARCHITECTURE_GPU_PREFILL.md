# dasLLAMA Architecture - the Metal prefill driver

Companion to `ARCHITECTURE.md`; section numbers are that document's.

### 2.2c The prefill GEMM form ladder {#prefill-gemm-ladder}

Every weight GEMM in `dasllama/dasllama_metal_prefill.das` splits its rows across two decisions.
First the GEMV tail peel (sec.2.2e) takes up to `MM_TAIL_MAX` remainder rows off the padded tile.
The rows that remain pick one of four forms, in this order, per site per forward:

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
   ceil32(npos)` is the padded row count). sec.2.2d carries the panel size rules.
3. **tall 128-row M-tile** - the stamp streams W `M/128` times over a 128-row tile, taken on the
   row count's 128-floor with the 32-row stamp on the remainder. The remainder arm strides X by
   `kdim`, so a caller that passes no `kdim` takes the tall stamp only when its row count is
   already a multiple of 128, and otherwise stays whole-dispatch 32-tile.
4. **32-row tile** - the default stamp.

**The dense staged stamps double-buffer their W staging** (`DASLLAMA_METAL_DBUF=0` is the
single-tile rail): two 64-deep ping-pong tiles hold 18432 B of threadgroup memory - probed at
the allocation law's edge, 0-2% tax by shape - and the loop runs ONE barrier per chunk, which
both publishes the tile staged last iteration and fences the previous `op.run` off the tile
about to be overwritten; staging chunk b+1 overlaps the tensor op on chunk b. Raced per
family: the staged-q8 stamps +1.7% end-to-end on 8B pp512 (3435 -> 3492 tok/s), the kq
in-kernel-dequant stamps +1.8% on gemma-4-12B Q4_K_M (2040 -> 2077); the MoE q8 expert
stamps measured -5.0% (Qwen3-30B-A3B q8) - the expert z-grid's many small dispatches pay
the 18432 B occupancy cost the 2-D dense grids do not - so the MoE staged family stays
single-tile. Outputs are bit-equal to the single-tile forms because the staged f16 rounding
and chunk order are identical (the k6 stamp pins the factored `dsc*(q-32)` like its dequant
kernel, so the equality holds by source). The pattern is the one Apple ships in MLX's
fp-quantized NAX family at this exact tile size.

A threadgroup-memory REQUEST size moves the clock on its own, independent of how much of it
the kernel indexes: 10240-16384 B is a flat plateau, 9216 B a pothole, and 20480 B and above
degrade. A staging-depth race is therefore a 2x2 - tile stride against allocation - and the
deeper tile is judged at equal allocation.

**A swiglu epilogue folded into the gate GEMM's store is REFUTED on M5.** Any per-element
access of the cooperative C after `op.run` - the scattered-store walk, a modify-in-place
before the optimized `cT.store`, even a cooperative-load register combine of the up panel -
costs ~+340µs on the 512x12288 gate GEMM (940µs plain), 2.2x the standalone swiglu pass the
fold deletes (~154µs, 8B end-to-end +11.9ms fused vs unfused). The stall is structural to the
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

**The occupancy floor guards the tall stamp.** A tall grid is taken only when
`rows/128 * (d/64) >= TALL_OCC_FLOOR` (default 64, a sidecar knob). An under-occupied tall grid
starves the GPU and small prompts regress hard without the floor.

### 2.2d The dev-W panel knee map {#devw-panel-knees}

Panel SIZE dominates the dev-W decision, not shape - each threshold is a knee, the panel size at
which the dev-W win flips sign. A panel at or under `DEVW_SMALL_PANEL` (32 MiB) engages at every
row count while its output width is at or under `DEVW_WIDE_N` (4096), and from
`DEVW_WIDE_N_ROWS` (512 rows) when wider; a panel up to `DEVW_BIG_PANEL` (48 MiB) engages from
`DEVW_BIG_ROWS` (2048 rows); above that the panel either splits into N-column tiles or the site
declines. The mechanism: the f16 panel's W stream is 2 bytes an element against the q8 blob's
1.06 once it leaves cache, and a wide-N panel costs more dequant work, which only a large row
count repays. The raced evidence
(32 MiB wins everywhere, 47 MiB only from 2048 rows, 112 MiB loses everywhere) is
`benchmarks/matmul/bench_metal_nax_probe.das`'s grid.

**Resident panels remove the re-dequant entirely.** The dev-W scratch pair re-pays every
site's dequant every forward - at 8B geometry ~4.5 GB of panel writes and quant reads per
512-chunk, the whole `devw_cvt` knockout pool (~7.7 ms). A resident site reads as a single
full-panel GEMM at a `(buffer, offset)` seat (no N-column tiling - tiling exists only for the
scratch pair's capacity), keyed by the site's quant-plane buffer pointer plus its offset in
that plane's own unit (q8: mblob bytes; kq: elem woff), released on driver shutdown or weight
refill. Panels become resident two ways: an image-served model seeds them ZERO-COPY
from its baked `devwf16` plane (`ARCHITECTURE_IMAGE.md` sec.2.1h - no dequant ever, no
dedicated memory), and an owned load dequantizes once into persistent hazard-tracked buffers
under the `DASLLAMA_METAL_DEVW_RESIDENT` MB budget (past it, scratch, warned once).
Measured 8B npos=512: 162.1 -> 153.8 ms, both ways.

Clauses the isolated grid cannot see, because it races one site while production overlaps
sites:

- **A long-K (down-projection) dequant serializes** on the panel pair behind the up/gate GEMM
  chain at small M, so it loses end to end there despite winning its isolated site race. The
  long-K floor is 1024 rows on q8; the same tiling that wins its isolated site race at 512
  rows measures zero to negative end to end (`../benchmarks/lcpp_bench.das -p 512 -n 0` A/B
  with the form forced per run).
- **An over-knee panel runs as N-column TILES**, each under the small-panel knee, with the tile
  count bounded by `DEVW_MAX_TILES` (32), divisibility, the small-panel knee and the pool.
  Narrow tiles measure fine in the same A/B. A K-quant site whose panel reaches
  `TALLKQ_MIN_PANEL` leaves dev-W entirely for the tall in-kernel-dequant stamp (sec.2.2c
  form 1).
- **A k-quant tg fallback is slower than the q8 half-panel form**, so a k-quant site lowers the
  over-knee bar, the tiled-rows floor, and the long-K floor (1024 rows to 512): a tiled read
  still beats THAT fallback.

`CVT_MIN_ROWS`, `TALL_OCC_FLOOR` and `DEVW_SMALL_PANEL` are box-raced and cached at init from
the sidecar (`metal_cvt_min_rows`, `metal_tall_floor`, `metal_devw_small_panel_mb`); the other
knees are fixed.

### 2.2e The GEMV tail peel {#gemv-tail-peel}

A prefill panel pads to `mp` rows, so `npos % 32` rows of every GEMM are padding. Up to
`MM_TAIL_MAX` (8) remainder rows peel off the padded tile onto the fixed-B mv family instead;
above that the padded tile is cheaper than three or more weight streams. One peeled row rides the
reduction-split GEMV, two or more ride the b4 form only - the reduction-split GEMV walks per
block and needs `kdim % 32`, while the b4 form - the batched fixed-B mv stamp, up to four rows a
dispatch (`enc_mv_b4_c`) - reads whole 128-quant rounds and needs `kdim % 128`.

### 2.2f The prefill attention slab {#prefill-attn-slab}

Prefill attention is a three-kernel pipeline over one per-head f16 score slab padded to
`np32 = ceil32(npos)` columns: QK writes the raw scores half once, rowstat mints each row's max
and reciprocal sum, and AV applies `exp` while it stages P. The slab is written once and read
three times - rowstat's max pass, rowstat's sum pass, and AV. Every stage and every stamp of every stage binds
ONE `AttnArgs` value, derived once per layer, and ignores the fields it does not read. Two forms
serve it: the tiled QK/AV GEMM pair (the default, needing `head_size % 64` on BOTH attention
classes) and the scalar 32x32 trio, which serves when that gate fails or `DASLLAMA_METAL_ATTN=0`
pins it.

- **A pad row of K or V stages as 0.** `pf_p_weight` zeroes P columns past each row's live
  length exactly, so the P side needs no guard; the K and V sides do.
- **The PADFREE stamps drop that guard**; an encoder picks one only where the whole WALK stays
  inside live rows - `qoff + qrows == npos` as well as the divisibility. A padded query chunk
  walks pad-query tiles past `npos`, where a real row's read poisons the tile.
- **A block skip lifts to `uend` only where the tile holds span rows.** A causal-only tile above
  the uniform span keeps its short causal walk, and a tile below every row's sliding window is
  skipped whole because `pf_p_weight` zeroes P over the skipped region.
- **The softmax scale rides the staged K side, not Q**, because `(q*s).k == q.(k*s)` and Q streams
  from device raw.

**Two attention classes.** The config fields describe the GLOBAL class and the `_swa` twins the
SLIDING class; the loader guarantees at most two. Buffers size to the class maxima and each layer
binds its own class's uniform set - a uniform model leaves the sliding twins null and binds the
base set everywhere.

### 2.2g The prefill MoE bucket rail {#prefill-moe-buckets}

Routing is atomics-free: a router GEMV and a select pass, then a per-expert count kernel, then
one bucket kernel that computes the padded prefix and fills the buckets. Each expert's bucket
PADS to a whole 32-row tile, every threadgroup computes the same padded prefix, and threadgroup
`e` publishes `basep[e]` for the mm and activation consumers. The bucket fill splits the entry
range into contiguous ascending per-lane chunks and scans the chunk counts, which reproduces the
serial entry order exactly, so the ordered weighted reduce is bit-stable against the CPU path
that parks each routed expert's rows and reduces them in entry order. The selection is read
GPU-side by the kernels; nothing reads back to the CPU, so encode-ahead and speculation stay
compatible.

Pad rows inside each expert's padded bucket carry a stamped sentinel and the reduce never
references them; rows past the last expert's stamped tail are unstamped stale pool bytes, which
is why validity tests compare the per-row entry against the live count, never the sentinel.

The MoE tensor twins ride one scaffold: `MetalMoeMulMmKqTensorBase` carries the expert
prologue, the staged K walk with its barrier pair, and the store; a weight format derives,
owns its weight-view bindings, and overrides the staged decode (`stage_block`) - mx4 also the
store (its per-expert bias) and the chunk shape (32-deep, 128-item quota). The q8 twin is not
a copy of this scaffold: its whole body is the tuned `tmm2d_q8u_f32` staged helper, a
different staging mechanism, so it stays its own template. The gather-X pass
copies the bucket's token rows into a CONTIGUOUS f16 panel with pad rows zeroed, which lets the up
and gate sites ride the contiguous tensor twins instead of the in-kernel gather form; the panel is
minted once per layer and shared by both sites. An X read through the bucket index can never
form a tensor view, which is why every tensor twin of the MoE family serves contiguous rows
only.

**The staging form that wins inside the gathered mul_mm kernels is per format, not universal.**
The gathered q8 form carries its scale and quant pointers across k-blocks; the stateless index
form measures 3.4-3.6% slower (`benchmarks/matmul/bench_metal_moe_lab.das`, gmm8 section). The
gathered Q6_K is the opposite: a superblock-scalar cache measures 2.4% slower per mm than
reloading per k-block (same lab, gmm6 section), so its stage is stateless.

### 2.2h Pad rows and cooperative-op constraints {#prefill-pad-rows-and-coop}

Activation panels size to `mp` rows because every kernel's M grid divides `mp` by
its tile height - 32 for the default stamp, 128 for the tall stamps; a 64-row pad would bill a
dead 32-row GEMM block on every short prefill. The GEMM has no edge
masking, so pad rows are written with whatever the tile computes. That is safe because C-block
rows are independent and no pad row is read back: the norm, rope, attention and elementwise
kernels all bound at `npos`, and rowstat writes `[0, npos)` only.

A continuation chunk (`start_pos > 0`) attends the session's existing rows: the K/V panels hold
`[0, start_pos)` gathered rows plus the chunk at the `start_pos` offset, keys pad to the QK key
grid's 64-tile, and the score slabs widen to `nk64` columns - the key rows padded to that
64-tile - while the rows stay the `mp` queries.

Cooperative matmul ops shape the kernel bodies: the accumulate loop is spelled ROLLED over
matrix arrays with pointer tile bumps, because the hand-unrolled spelling hoists sixteen tile
addresses into loop-lifetime registers and costs an occupancy tier (measured `max_threads` 704).

### 2.2i Chunked submission and interleaved readback {#prefill-chunked-submit}

A prefill encodes into `DASLLAMA_METAL_NCB` command buffers (default about four layers each) and
commits each chunk as soon as it is encoded, so the scheduler analyzes chunk k while chunk k-1
executes; `DASLLAMA_METAL_UNRETAINED=1` drops per-dispatch retain/release on top. Inside a chunk
the `kn_*` calls record a step graph that `graph_flush_sched` replays on a CONCURRENT encoder,
which auto-schedules and inserts barriers only at real hazards (`DASLLAMA_METAL_SCHED=0` keeps
capture order, `DASLLAMA_METAL_PF_CAPTURE=0` is the serial-encode rollback).

Completion and readback interleave: chunks complete in commit order and each completed chunk's
roped-K and raw-V rows stream into the CPU K/V codec while later chunks keep the GPU busy. The
residual-stream copy waits for the LAST chunk.

### 2.2u The f16 twin dual-store {#prefill-twin-dual-store}

A producer kernel that already holds a panel's rows in registers writes the f16 twin beside its
f32 output, and the consumer GEMM reads that twin instead of running a conversion pass. Two
producers carry a dual-store stamp: the fused qk_norm+rope pass writes the q panel's twin, and
the residual-add+RMS and RMS stamps of the FFN spine write the FFN input's twin. The consumer
takes the twin directly through `pf_twin_panel`, so `pf_cvt_panel` and its row floor never run
for that site. The dual store is one conversion, not a second computation: the twin is bit-equal
to `float16` of the f32 panel at every element. Rows past the live row count are PAD - their f32
values come back untouched and their twin slots carry the raw f16 copy, so a consumer that walks
the padded tile reads the same numbers on both planes.

The fused qk_norm+rope pass is one panel rewrite per q and per k row - RMS by simdgroup
reduction, the rotation in register - where the split form runs a norm read-write, a rope
read-write and a conversion. It is picked per layer and needs an even head size and an even
rotary width; `DASLLAMA_METAL_QK_ROPE=0` restores the split dispatches.

The twin is written only where a dense FFN reads it. A routed-MoE layer feeds its gathered
expert panels instead, so no dual-store stamp is selected there and those layers pay no f16
write. Fusing the residual add into the FFN norm also lets the norm re-read its row from the
threadgroup slab rather than from device memory.

### 2.2v The last-layer FFN tail {#prefill-last-row-tail}

Past the final attention only the classifier's logits and the MTP head read the residual
stream, and both read one row. The last dense layer's FFN therefore runs that final row alone:
the add, the norm and the three FFN GEMMs dispatch at one row through the fixed-B GEMV forms,
not through `enc_gemm_mm`, so the mm-tail peel counters (sec.2.2e) stay an instrument of the
full-panel path only. The narrowing declines wherever another consumer reads every row - a
session keeping the hidden state, a warm or MTP forward, a recurrent, MoE or PLE layer, a
sandwich-norm or gated-query model - and `DASLLAMA_METAL_LASTROW=0` pins the full-panel tail.
A caller that consumes the whole `x_b` plane afterwards - embedding pooling, a plane-compare
probe - sets `Session.keep_hidden` and the prefill keeps every row; the flag is zero-init, so
narrowing is the default.
