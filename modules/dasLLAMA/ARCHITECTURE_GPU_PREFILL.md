# dasLLAMA Architecture - the Metal prefill driver

Companion to `ARCHITECTURE.md`; section numbers are that document's.

### 2.2c The prefill GEMM form ladder {#prefill-gemm-ladder}

Every weight GEMM in `dasllama_metal_prefill.das` picks one of four forms, in this order, per site
per forward:

1. **dev-W all-device** - the weight plane is dequantized into a device f16 panel and multiplied
   half x half. No threadgroup staging and no barriers, so the staged-operand tax is gone; the
   dequant pass is paid once per site per forward against a GEMM that re-reads the operand `d/64`
   times. sec.2.2d carries the panel size rules.
2. **tall 128-row M-tile** - the stamp streams W `M/128` times over a 128-row tile, taken on the
   row count's 128-floor with the 32-row stamp on the remainder. The remainder arm strides X by
   `kdim`, so a caller that passes no `kdim` stays whole-dispatch 32-tile.
3. **32-row tile** - the default stamp.
4. **GEMV tail peel** - sec.2.2e.

**Half operands ride an f16 activation panel.** One pass converts the f32 panel; the GEMM then
re-reads it at half the bytes `d/64` times, so the convert amortizes above a row floor
(`CVT_MIN_ROWS`, default 256) and loses under it. The panel is double-buffered so a consumer's
GEMM overlaps the next twin's producer, and a producer that dual-stores the f16 form takes a panel
ungated by rows because its twin is free.

**The bf16-A stamp widens without dequantizing** - a bf16 row widens by an exact bit shift and
rounds to the f16 tile - so the E-series `per_layer_model_proj` GEMM serves straight off the
kept-bf16 blob with no resident f32 copy.

**The staging form that wins is per format, not universal.** The gathered q8 mul_mm carries its
scale and quant pointers across k-blocks; the stateless index form measured 3.4-3.6% slower in the
gmm8 lab. The gathered Q6_K goes the other way: the superblock-scalar cache the standalone kernel
carried measured 2.4% slower per mm than reloading per k-block in the gmm6 lab, so its stage is
stateless.

**The occupancy floor guards the tall stamp.** A tall grid is taken only when
`rows/128 * (d/64) >= TALL_OCC_FLOOR` (default 64, a sidecar knob). An under-occupied tall grid
starves the GPU and small prompts regress hard without the floor; 32 adds nothing over 64, raced
at mid-M across three dense families.

### 2.2d The dev-W panel knee map {#devw-panel-knees}

Panel SIZE dominates the dev-W decision, not shape: a panel at or under 32 MiB wins at every row
count, 47 MiB wins only from 2048 rows, and 112 MiB loses everywhere - the f16 panel's W stream is
2 bytes an element against the q8 blob's 1.06 once it leaves cache. Wide-N panels pay a dequant
tax that only deep M repays. The grid these rules come from is
`benchmarks/matmul/bench_metal_nax_probe.das`.

Three clauses the isolated grid cannot see, because it races one site while production overlaps
sites:

- **A long-K (down-projection) dequant serializes** on the panel pair behind the up/gate GEMM
  chain at small M, so it loses end to end there despite winning its isolated site race. The
  long-K floor is 1024 rows; the isolated tiled win at 512 rows read flat-negative end to end.
- **An over-knee panel runs as N-column TILES**, each under the small-panel knee, with the tile
  count bounded only by divisibility, the knee and the pool. Deep-dense models (300 MB+
  up/gate/down planes) need far more tiles than the first knee map raced, and narrow tiles measure
  fine - a 20-threadgroup tile dispatch still beat the tg-staged fallback at 512 rows.
- **A k-quant tg fallback is about 1.28x the q8 half-panel form**, so a k-quant site lowers both
  the over-knee bar and the tiled-rows floor: a tiled read still beats THAT fallback.

The knee constants are box-raced and cached at init from the sidecar (`metal_cvt_min_rows`,
`metal_tall_floor`, `metal_devw_small_panel_mb`).

### 2.2e The GEMV tail peel {#gemv-tail-peel}

A prefill panel pads to `mp = ceil32(npos)`, so `npos % 32` rows of every GEMM are padding. Up to
`MM_TAIL_MAX` (8) remainder rows peel off the padded tile onto the fixed-B mv family instead;
above that the padded tile is cheaper than three or more weight streams. One peeled row rides the
reduction-split GEMV, two or more ride the b4 form only - the reduction-split GEMV walks per
block and needs `kdim % 32`, while the b4 stripe reads whole 128-quant rounds.

### 2.2f The prefill attention slab {#prefill-attn-slab}

Prefill attention is a three-kernel pipeline over one per-head f16 score slab padded to
`np32 = ceil32(npos)` columns: QK writes the raw scores half once, rowstat mints each row's max
and reciprocal sum, and AV applies `exp` while it stages P. The slab is written once and read
once, and no separate softmax pass touches it. Every stage and every stamp of every stage binds
ONE `AttnArgs` value, derived once per layer, and ignores the fields it does not read. Two forms
serve it: the tiled QK/AV GEMM pair (the default, needing `head_size % 64` on BOTH attention
classes) and the scalar 32x32 trio, which serves when that gate fails or `DASLLAMA_METAL_ATTN=0`
pins it.

- **A pad row of K or V stages as 0.** Pad rows carry recycled pool bytes and may hold NaN, and
  `0 * NaN` poisons a whole cooperative tile. `pf_p_weight` zeroes P columns past each row's live
  length exactly, so the P side needs no guard; the K and V sides do.
- **The PADFREE stamps drop that guard**, and an encoder may pick one only when the whole WALK
  stays inside live rows - `qoff + qrows == npos` as well as the divisibility. A padded query
  chunk walks pad-query tiles past `npos`, where a real row's read poisons the tile.
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

Routing is atomics-free: count (one threadgroup per expert) -> in-threadgroup prefix -> bucket
fill. Each expert's bucket PADS to a whole 32-row tile, every threadgroup computes the same padded
prefix, and threadgroup `e` publishes `basep[e]` for the mm and activation consumers. The bucket
fill splits the entry range into contiguous ascending per-lane chunks and scans the chunk counts,
which reproduces the serial entry order exactly, so the ordered weighted reduce is bit-stable
against the CPU park-and-accumulate. The selection is read GPU-side by the kernels; nothing reads
back to the CPU, so encode-ahead and speculation stay compatible.

Pad bucket rows carry a stamped sentinel and the reduce never references them. The gather-X pass
copies the bucket's token rows into a CONTIGUOUS f16 panel with pad rows zeroed, which lets the up
and gate sites ride the contiguous tensor twins instead of the in-kernel gather form; the panel is
minted once per layer and shared by both sites. A bkt-indirect X can never form a tensor view,
which is why every tensor twin of the MoE family serves contiguous rows only.

### 2.2h Pad rows and cooperative-op constraints {#prefill-pad-rows-and-coop}

Activation panels size to `mp = ceil32(npos)` rows because every kernel's M grid is `mp/32`; a
64-row pad would bill a dead 32-row GEMM block on every short prefill. The GEMM has no edge
masking, so pad rows are written with whatever the tile computes. That is safe because C-block
rows are independent and no pad row is read back: the norm, rope, attention and elementwise
kernels all bound at `npos`, and rowstat writes `[0, npos)` only.

A continuation chunk (`start_pos > 0`) attends the session's existing rows: the K/V panels hold
`[0, start_pos)` gathered rows plus the chunk at the `start_pos` offset, keys pad to the QK key
grid's 64-tile, and the score slabs widen to `nk64` columns while the rows stay the `mp` queries.

Cooperative matmul ops constrain the kernel bodies two ways. The accumulate loop is spelled ROLLED
over matrix arrays with pointer tile bumps: the hand-unrolled spelling hoists sixteen tile
addresses into loop-lifetime registers and costs an occupancy tier (measured `max_threads` 704).
And an early exit inside such a body must be threadgroup-uniform.

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
