# dasLLAMA Architecture - the Metal decode driver's kernel forms and layer encoder

Companion to `ARCHITECTURE_GPU_MTP.md`; a section is cited by its anchor. This document
carries the argument-alignment contract a kernel declares on
its `[metal_dispatch]` - the contract the batch driver's fixed-B mul_mv forms carry - the
K-quant small-batch form and the row-buffer pad a multi-row verify dispatches under, the
single-row driver's greedy chain, the batch driver's pre-encoded step, the decode layer encoder,
and the rotated prefix a rope-store kernel takes. The speculative round these
forms serve is `ARCHITECTURE_GPU_MTP.md`. The GPU backend role table these sections build on
stays in `ARCHITECTURE_GPU.md#gpu-backends`.

### The kernel argument-alignment contract {#metal-dispatch-requires}

**A kernel's argument-alignment contract is declared on its `[metal_dispatch]` and enforced at
every dispatch.** `requires = "lhs % N, ..."` (lhs a `params=` name or a kargs field) makes the
generated builder check each item before it binds and call `metal_requires_failed` on a miss -
a panic naming the builder and the broken contract, or the test hook installed with
`set_metal_requires_hook`. The check is one integer modulo per dispatch, so the kernel keeps its
tail-guard-free main loop and the DRIVER does the shape routing: the fixed-B mul_mv forms stripe
K in 256 (B2) / 128 (B4) element chunks, and the batched decode driver gates each mv site on its
own K (`mv_kdim`, `mv_wo`, `mv_w2`), falling to the tail-exact GEMV form where the alignment
fails (gemma-4-26B-A4B's dense hidden 2112 on the w2 site). Every `[metal_dispatch]` GEMM form
carries its grid divisors the same way - the production mul_mm `mp % 32, d % 64`, the non-tensor
K-quant mul_mm builders `mp % 32`, the 32- and 64-wide GEMM-B forms and their tensor twins `ka.ndim % 32|64` with the q8 block `ka.kdim % 32`,
the split-K pair `d % 32` - so a grid that would have truncated silently now names the site.
The non-tensor `enc_kq_mm_*_c` family (`dasllama/dasllama_metal_kernels.das`) declares `mp % 32`
alone: its `rows % 64` half stays undeclared until the site census (followup 83) settles which
sites clear it and which take a driver gate, and the drivers' own shape routing carries it
meanwhile - the one contract these forms hold that the builder does not yet check. The prefill
tensor twins and the MoE split stamps declare their `rows % 64` beside their own mp divisor.
A contract on a value that reaches the builder only as a bound uniform BUFFER (the mul_mm's K)
stays with the caller: the dispatch never pays a readback.

### The K-quant small-batch form is a per-box crown {#kq-rows-crown}

**A verify row on a K-quant plane costs what the box says, not what the kernel comment says.**
At two to eight rows `enc_kq_site_b` dispatches either the small-batch twin (one weight pass per
row group) or B single-row GEMV passes. The K-quant GEMV is ALU-bound, so sharing the weight pass
pays only where ALU is plentiful: on the M5 Max the two-row twin costs 0.52-0.89 of two passes,
on the M4 Pro 1.35-1.5 for k4 while k6's twin still wins. The mint races the production wrappers
per format (`race_kq_rows`, twin against two passes) and crowns `kq_rows_<fmt>` where the passes
win; `metal_decode_init` reads the crowns once into `g_kq_rows_crowned` and the dispatcher takes
the passes exactly there. An unraced box keeps the twin. The M4 Pro's Qwen3.8-27B round paid
about two steps for its two verify rows under the twin (the round clocks, `PERF_LEDGER.md`'s MTP
section) - the reason its depth-1 speculation lost.

The k4 twin itself has two forms, and the form is the box's too. The ext twin gives a thread one
weight row and two x columns; the two-row register tile (`MetalKqMvB2K4R2`) gives it two weight
rows and both columns, so every x float4 load feeds two rows - fewer loads per FMA, twice the
live registers. The tile wins where ALU is short (M4 Pro: 0.74-0.96 of two passes against the ext
twin's 1.36-1.49) and loses where it is not (M5 Max: 0.63-1.07 against 0.52-0.89). The mint races
the two production twins first (`race_kq_k4_form`) and crowns `kq_mvb2_k4_r2` where the tile
wins; `enc_kq_mvb` takes the tile at two rows exactly there, and the k4 rows race that follows
meets the box's twin form. An unraced box keeps the ext twin. Where the tile is crowned, three to
eight k4 rows never take the four- or eight-column forms either: `enc_kq_mvb_k4_pairs` walks the
tile over the column pairs and gives an odd tail one single-row pass. The wide forms are the ext
algorithm at more columns, and on the box that crowned the tile they cost more than B single
passes while two tiles do four rows in well under the four-column kernel's time; a third verify
row under the four-column form cost that box's Qwen3.8-27B round more than two extra steps (the
round clocks and the row ladder, `PERF_LEDGER.md`'s MTP section). In both races the
first arm is the incumbent and the second is the "tensor" side whose win crowns: `race_kq_rows`
times the twin against B single passes (the passes are "tensor"), `race_kq_k4_form` the ext twin
against the tile (the tile is "tensor").

Past eight rows the form is the panel's. The kq mul_mm twins dispatch `mp / 32` threadgroups
along M, so `enc_kq_site_b` takes them only over a panel padded to that tile (`mp` a multiple of
32) - the batch driver's at nine rows and up. The verify's panel is padded to the GEMV forms'
4-row tile (`ARCHITECTURE_GPU_MTP_DECODE.md#verify-row-pad`), so its nine rows (`MTP_MAX_ROWS`, depth 8) ride the small-batch forms: the
first eight as the eight-row dispatch, each row past eight as a single pass at its own x and y row
offsets. An unpadded nine-row panel handed to the mul_mm dispatches no threadgroup at all; the
twins' `mp % 32` contract names that site instead of leaving the output unwritten.

### The verify's row buffers are padded to the GEMV form's row tile {#verify-row-pad}

**A multi-row verify sizes every row buffer to a whole 4-row tile, and the pad rows are owned
scratch.** `acquire_step` takes `mp = ceil(nrows / 4) * 4` (a single-row step stays at 1) and sizes
`bx`, `bxb`, `bqkv`, `bh12`, `blog` and the deltanet rows to `mp` rows; the rope tables stay at the
live `nrows`. The fixed-B GEMV forms write a full tile - `enc_gemv_rows` dispatches the B2 form at
one or two rows, the B4 form at three or four, two B4 tiles at five to eight and three at nine to
twelve, each tile past the first offsetting x by its first row's count of n floats and y by the
same rows of the site's y stride - so the spare rows of the last tile compute garbage that must
land inside an allocation this step owns and nobody reads. A row buffer sized to the live count puts that garbage on whatever the pool put next to it.

The attention partials follow the same ownership rule along the position axis, and so does the
attention form: `acquire_step` sizes `bpart` for the DEEPEST row's chunk count -
`ceil((deepest + nrows) / 64)` chunks per (row, head), where `deepest` is the largest position
any of the step's rows reaches (a joint round's groups sit at different positions, so the callers
pass it) - and picks the chunked form off that same depth, never off row 0's. The layer chain
dispatches its chunks off the deepest group's position too (`encode_verify_step` passes it as the
chain position), so every row's partials are written before the combine reads them; the combine
kernel's batched arm clamps a row's chunk count to the dispatched plane besides, so a row deeper
than the plane reads a prefix rather than its neighbours' partials. A buffer sized for the first
row's count is short by one chunk per (row, head) whenever the rows straddle a 64-row boundary,
and the chunked attention then writes past its end into the pool's neighbour - a corruption that
surfaces rounds later, on whatever the heap put there. Each group's layer bases inside its mirror
slice follow that group's own cap (`group_routes`): a deeper stream's slice is taller than group
0's, and a base computed from another group's cap addresses the wrong layer.

### The single-row driver's greedy chain {#greedy-chain}

**The single-row driver pre-encodes the next step on the GPU's own argmax, and only a greedy
caller can afford it.** `pre_encode_next` encodes step `pos + 1` while step `pos` runs. Armed,
the new command buffer opens with an argmax over the running step's logits and the embed gather
of the winner (`r.spec`), commits at once, and runs the moment the previous one drains - the CPU
tail, the sampler and the next `forward()` all overlap GPU execution. Unarmed, the step is encoded
but held, and `forward()` pokes the caller's token and commits it - the encode still overlaps, the
commit turnaround does not. `finish_step` then compares the GPU's pick with the caller's token: a
miss waits the chained step out, discards it, and reruns the step through the slow path, so a miss
costs about two steps (30 ms against a 15 ms step on gemma-4-12B, M5 Max; every figure in this
section is `harness/batch_rows_probe.das` with `--feed text|greedy|second:K --spec -1|0|1`,
`-jit`) where a hit saves the turnaround, about 2% of a step. The adaptive mode
(`DASLLAMA_METAL_SPEC` -1) backs off exponentially after a miss and re-arms on the next hit; under
a sampler that agrees with the argmax two steps in three it still costs 63% of the step (26.0
against 15.9 ms), because the
re-arm attempts far more often than a 2% upside pays for. So the sampler decides: `sample_` marks
the session `sampled` whenever its pick can differ from the raw argmax - a temperature above zero,
or a penalty at temp 0 (`sampler_is_argmax` is the negation) - and the driver never chains such a
session, because the chain predicts the raw argmax the penalty then rejects. Only a bare argmax
sampler (temp 0, penalties off) and a caller feeding the argmax directly keep the adaptive chain.

**The step wait spins on the GPU end time, then blocks.** `finish_step` polls the command
buffer's `GPUEndTime` until it is set and only then enters `waitUntilCompleted`, because a
blocking wake returns late on a box whose performance cores the jobque lanes fill: on the M4
Pro (ten lanes on ten P-cores) the bench process pays 0.35 ms per token on every step and, with
the greedy chain, a whole step before every third one (the GPU idle 3.3-3.6 ms with the next
command buffer already queued, Llama-1B Q8 at 5.7 ms of GPU work per 6.4-6.7 ms token -
`benchmarks/decode_step_trace.das -o`, M4 Pro; `PERF_LEDGER.md`, the M4 Metal pass), while a
process with idle lanes wakes in time on either box. The calling thread owns the step and has
nothing else to do while it runs, and the jobque's own workers spin between jobs by design, so
the spin costs a core's idle and buys the wake's latency. `DASLLAMA_METAL_WAIT_SPIN=0` is the
blocking wait alone, the A/B rail. The spin is capped at 200 ms so a stalled step still reaches
the blocking wait that reports it.

The GPU argmax reproduces the CPU sampler's tie-break exactly: a lane keeps the earliest of equal
values (strict `>`), and the cross-lane fold takes the lower index on a tie, so the lowest index
wins - what lets the chain predict a bare-argmax sampler's pick bit for bit.

### The batch driver's pre-encoded step {#batch-pre-encode}

**The batch driver encodes the next step under the current one's GPU run and commits it when the
caller arrives with the tokens.** A batched step's landing state is a `BatchLanding` (the pooled
buffers, the command buffers, the rows' sessions, positions and slice offsets, and the poke
handles) in one of two slots: the pending step's and the pre-encoded step's, never the same slot.
After a step commits, `batch_pre_encode_next` builds the step at every row's position plus one into
the free slot with every command buffer ended and none committed, and only while each row's mirror
slice is on `mirror_prepare`'s fast path (the slice holds the position, the codec matches, the
watermark covers it) - a grow, an eviction or an upload would touch arena slices the in-flight
step is writing; the watermark must sit exactly at the row's position, since below it a prepare
uploads and above it a rewind re-uploads from row 0. The next call matches the pre-encoded step
against its rows - the same sessions by uid (a session pointer is not identity: the scheduler's reap
moves its streams) at the same positions, the model shape and KV codec, the arena epoch, every
slice still where the row table names it, and every row's zero-copy logits wrap where the scatter
was encoded - pokes the token-dependent inputs the caller's CPU pre-step produced - the rows'
embeds, their rope rows, the E-series' per-layer side rows - commits the command buffers in order
and lands the step like a freshly built one; a step that does not match is retired unrun. The
same-slab verify, the recurrent rows (their state buffer is one per session) and a knockout run
stay on the build-at-call path. The single-row driver, the speculative round's draft and verify
steps, a knob setter that changes what a step encodes, a dispatch failure's CPU rerun and
`metal_decode_flush` all retire the pre-encoded step: each advances the rows past the positions it
was built for or moves the slices under it. The rail ships off - `DASLLAMA_METAL_BATCH_PRE=1` arms
it: on the E4B four-row step it times the same as building at the call, and it holds a second
step's pooled buffers (`PERF_LEDGER.md`, the server-MTP section).

**The mirror watermark moves at commit, not at landing.** `batch_mark_committed` marks every
row's mirror at its position plus one the moment the step's command buffers are committed. The
queue is in order, so the step writes each row before any later-committed work reads the mirror,
and the next step's `mirror_prepare` therefore finds the position covered and stays on its fast
path. A watermark moved only when the step lands would put every pre-encode behind a re-upload of
rows from a CPU cache the landing has not written back yet - the one thing the fast path exists
to avoid.

The rail is neutral on the board rows because the step's command buffers already commit
progressively (`DASLLAMA_METAL_BATCH_NCB`), so the GPU runs under the encode either way; what the
rows pay is the GPU's idle between the landing and the next commit (`followup_metal.md` row 23).
The rail is the scaffold for the rows twin of the greedy chain (`ARCHITECTURE_GPU_MTP_DECODE.md#greedy-chain`): a pre-encoded step
that opens with the GPU's argmax over the running step's logits and the rows' gathers commits at
once, and the poke disappears.

### The decode driver's layer encoder {#metal-layer-enc}

**A layer is one chain at every row shape, written once.** `LayerEncT`
(`dasllama/dasllama_metal_decode.das`) carries the phase order - QKV, the norms, rope plus the KV
store, attention, wo, the dense or routed FFN, the post norms and the residual adds - as one
`encode` method over a `StepRes` the stamp's driver owns. A row shape is a stamp that binds its own
adapters into the template's call slots: the weight site, the row totals, add_rms, rms and add, the
expert args, rope plus store, attention, the recurrent layer, the fused gate+up form and the FFN
panels. Three stamps exist - `SingleLayerEnc` (one row), `VerifyLayerEnc` (the MTP verify's k+1 rows
over one slab) and `BatchLayerEnc` (the batch step's B rows). `SINGLE` is the template's one
`@template_constant`: the single row alone serves the fused QKV form, the QK-norm prepass skip on an
f16 mirror, the fused pre-norm and PLE. What differs between row shapes is how a weight site
dispatches and which kernel family serves a phase, never the phase order, so the order cannot drift
between them.

The row shapes share their adapters. `VerifyLayerEnc` and `BatchLayerEnc` bind one generic per
slot - `rms_rows`, `add_rows`, `add_rms_rows`, `moe_args_rows` and `total_rows` - each written
over `auto(ET)` and reading nothing but the stamp's `StepRes`, so a new row shape binds the set
instead of copying it. Their weight site is the batch ladder under `rows_tier`, one `BatchTier`
with every arm off (`use_mm` and `use_gemm` false, `mp = nrows`, no split-K buffer): a row chain
takes the K-quant plane GEMV or the row GEMVs, never the batch's tensor, mv, split-K or tile
forms.

**A dense q8 weight site picks its GEMV form by reduction width.** `enc_q8_site_gemv`
(`dasllama/dasllama_metal_kernels.das`) takes the row-per-simdgroup form (`MetalQ8GemvSg`, 8 rows
per 256-thread threadgroup, no cross-simdgroup reduce) at `n <= GEMV_SG_MAX_N` (1536) and the
reduction-split `MetalQ8Gemv` above it; the fused W1|W3 twin (`enc_gemv_w13sw`) follows the same
cut through its own `Sg` stamp. At a short reduction the split form's four simdgroups each see a
handful of blocks and its two barriers per row pair dominate: on the M4 Pro the row form wins
every short shape, and on the M5 Max it wins the projection shapes and loses the classifier by 6%
(`benchmarks/matmul/bench_metal_gemv_kernels.das` at the Qwen2.5-0.5B shapes, both boxes;
`PERF_LEDGER.md`, the M4 Metal pass). At 2048, classifier width, the split form is back ahead on
both parts, so `GEMV_SG_MAX_N` is a constant, not a crown. The pick reads n alone while the
crossover moves on n and d, which is why the M5 classifier takes the slower form
(`followup_metal.md` sec.12).

### A rope-store kernel rotates a partial carrier's prefix alone {#metal-rope-store-rot}

**A rope-store kernel takes the rotated prefix `rot`, and `rot` alone rotates.** `rot` is 0 or the
head size for a full head; a partial carrier (qwen3.5: 64 rotated dims of a 256-wide head) rotates
the NEOX pairs `(j, j + rot/2)` of the head's first `rot` dims and passes `[rot, hs)` through
unchanged - still adding the bias there - which is the CPU leaf `rope_scaled_neox_tab_part`'s rule,
and a partial carrier takes the pair form whatever `neox` says. The cos/sin table row is `rot / 2`
wide on a partial carrier and `hs / 2` otherwise, so a batched kernel strides its per-row table by
that width. The f16 mirror carried the arm first; the q8_0 and tq4 mirrors take the same one.
