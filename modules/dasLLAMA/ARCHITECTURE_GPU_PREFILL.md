# dasLLAMA Architecture - the Metal prefill driver

Companion to `ARCHITECTURE.md`; a section is cited by its anchor. This document carries the
attention slab, the pad-row and cooperative-op constraints, chunked submission, the f16 twin
dual-store, the last-layer FFN tail and the hyper-connection window. Its companions:

- `ARCHITECTURE_GPU_PREFILL_GEMM.md` - the GEMM form ladder, the dev-W panel knee map, the GEMV tail peel and the dense-KQ tensor mul_mm scaffold.
- `ARCHITECTURE_GPU_PREFILL_MOE.md` - the routed block: the MoE bucket rail, its tensor-twin scaffold, the split-format expert twins, the K-quant twins' pick, the short window's gathered route and the gemma-4 MoE tail as one step (`#metal-g4-moe-step`).
- `ARCHITECTURE_GPU_PREFILL_WINDOW.md` - the device attention pair, the mirror-fed window, the segmented window, the recurrent scan and hot-load addressing.

### The prefill attention slab {#prefill-attn-slab}

Prefill attention is a three-kernel pipeline over one per-head f16 score slab padded to
`np32 = ceil32(npos)` columns: QK writes the raw scores half once, rowstat mints each row's max
and reciprocal sum, and AV applies `exp` while it stages P. The slab is written once and read
three times - rowstat's max pass, rowstat's sum pass, and AV. Every stage and every stamp of every stage binds
ONE `AttnArgs` value, derived once per layer, and ignores the fields it does not read. Two forms
serve it: the tiled QK/AV GEMM pair (the default, needing `head_size % 64` on BOTH attention
classes) and the scalar 32x32 trio, which serves when that gate fails or `DASLLAMA_METAL_ATTN=0`
pins it. A third form over the same slab is `ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-attn-device`.

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

### Pad rows and cooperative-op constraints {#prefill-pad-rows-and-coop}

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

### Chunked submission and interleaved readback {#prefill-chunked-submit}

A prefill encodes into `DASLLAMA_METAL_NCB` command buffers (default about four layers each) and
commits each chunk as soon as it is encoded, so the scheduler analyzes chunk k while chunk k-1
executes; `DASLLAMA_METAL_UNRETAINED=1` drops per-dispatch retain/release on top. Inside a chunk
the `kn_*` calls record a step graph that `graph_flush_sched` replays on a CONCURRENT encoder,
which auto-schedules and inserts barriers only at real hazards (`DASLLAMA_METAL_SCHED=0` keeps
capture order, `DASLLAMA_METAL_PF_CAPTURE=0` is the serial-encode rollback).

Completion and readback interleave: chunks complete in commit order and each completed chunk's
roped-K and raw-V rows stream into the CPU K/V codec while later chunks keep the GPU busy. The
residual-stream copy waits for the LAST chunk.

### The f16 twin dual-store {#prefill-twin-dual-store}

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

### The last-layer FFN tail {#prefill-last-row-tail}

Past the final attention only the classifier's logits and the MTP head read the residual stream, and both read
one row. The last dense layer's FFN therefore runs that final row alone: the add, the norm and the three FFN
GEMMs dispatch at one row through the fixed-B GEMV forms, not through `enc_gemm_mm`, so the mm-tail peel
counters (`ARCHITECTURE_GPU_PREFILL_GEMM.md#gemv-tail-peel`) stay an instrument of the full-panel path only. On a
sandwich-norm model the row's attention branch takes its post norm before the add and its FFN branch its own before
the residual's, both at the one row. The narrowing declines wherever another consumer reads every row - a session
keeping the hidden state, a warm or MTP forward (`n_layer_nextn` present), a recurrent, MoE or PLE layer, a
gated-query model, a deepstack-tapped layer - and it engages only at `npos > 1` with `dim` and the layer's hidden width both %32
and every FFN weight on a plane with a one-row form - the q8 plane's GEMV, a per-32 plane's, or a superblock format's
row-batched decode form at one row (`pf_enc_narrow_gemv`; the site's K must be whole blocks of its unit, so a K-quant
carrier whose `dim` or hidden width is off 256 keeps the full panel) - whatever the mm-tail lever
(`DASLLAMA_METAL_MM_TAIL`) says. `DASLLAMA_METAL_LASTROW=0` pins the full-panel tail. A caller that reads
`x_b` rows afterwards - embedding pooling, whichever rows its pooling picks, a plane-compare probe - sets
`Session.keep_hidden` and the prefill keeps every row; the flag is zero-init, so narrowing is the default.

### The hyper-connection window {#prefill-hc-window}

A hyper-connection model (qwen4exp) prefills with the decode's seams over the window's rows (`PfHc`):
`enc_hc_init` makes the wide residual (`hc` copies of every embed row) at layer 0; each layer opens with
the attention site's mixer (`pf_hc_mix_site`: the grouped norm over `npos x hc` stream rows, the scatter
logits as a rows GEMV, the down and up q8 mul_mm over the padded panel, the gated stream mean into `bxb`),
the attention output scatters under those logits (`pf_hc_combine`), the ffn site's mixer makes the FFN
rows and the FFN output scatters under its own; the head mixer runs the single-row chain over the last
wide row for the logits. The n-gram side input (`pf_ple_side`, before layer `ple_layer`'s mixer) gates
every row into a window panel, runs the conv over the panel and the ring (`PleConvArgs.direct`), and
copies the window's last `rows` panel rows into the ring, one slot each, for the rows after the window.
