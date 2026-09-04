# dasLLAMA Architecture - the Metal speculative round

Companion to `ARCHITECTURE_GPU.md`; section numbers are `ARCHITECTURE.md`'s. This document
carries sections 2.28-2.39: the speculative round on Metal, the box knob that sets the depth a
round drafts, and the argument-alignment contract a kernel declares on its `[metal_dispatch]` -
the contract the batch driver's fixed-B mul_mv forms carry. The GPU backend role table these
sections build on, the assistant-drafter driver's role row included, stays in
`ARCHITECTURE_GPU.md` sec.1.5.

### 2.28 The speculative round on Metal {#mtp-round-one-join}

**The speculative round on Metal is one queue, one join.** A round drafts k tokens and verifies them
as k+1 same-slab rows of the batch driver. The gemma assistant drafter encodes its k steps into ONE
command buffer committed without a wait: draft i embeds `bvtok[i]` (the seed at 0), its argmax lands
in `bvtok[i+1]` on the device, and its post_projection is copied on the device into the next step's
input; the batch driver then takes that token buffer (`set_batch_same_slab_tokens`) and dequantizes
the rows' embeddings on its own encoder instead of the workspace's CPU rows, lands every row's
argmax (`enc_argmax_rows`) with the logits, and the host reads tokens and winners only after the
verify's join - queue order completes the chain first. The drafter's h input is the target's
post-output_norm hidden, carried in `s.mtp_h` by the decode on head-less models
(`set_metal_mtp_carry_hidden`); the pre-norm residual is a lever that measured worse. The two-row
verify's GPU cost over a one-row step (the rows' weight stream; the per-box margins are in
`PERF_LEDGER.md`'s MTP section) is the physics the round cannot recover; everything the host did
between the drafts and the verify was.

**The accept walk and the commit.** Row i of the verify is the truth for draft i+1, so the walk
takes `a` = the length of the leading run where a row's argmax equals the draft it verifies. The
round commits rows 0..a: row a's logits become `s.logits` (the token sampled next), row a's
post-final-norm hidden becomes `s.mtp_h` (the next round's carry, and the assistant drafter's h
input), `s.mtp_h_pos1` and the mirror watermark move to `pos + a + 1`, and `n_past` advances by
`a + 1`. The rows above the new watermark are the rejected drafts' - garbage the next round
rewrites - and only the watermark keeps them from being read.

**A sampled stream's walk draws instead of comparing argmaxes, and the same walk serves every
round - the two Metal rounds and the CPU depth-1 step.** The caller points `s.spec_params` at its
own sampler before the round; at a temperature above zero the walk draws row i through that
sampler (`mtp_walk_sampled`, the row sampler installed by `dasllama_sampling`), with the
recent-token window advanced per accepted draft so penalties see the tokens the stream will have
committed, and accepts while the draw equals the draft it verifies. Every committed token is a draw
from its own target row, so the stream's distribution is the plain sampled decode's, and a seeded
run reproduces it token for token - one RNG draw per emitted token, in order. The first miss, or the
draw from the row past the last draft, is the next token: the walk parks it in `s.mtp_pre_tok`
(`mtp_pre_drawn`), and the caller's next `sample_` returns it instead of drawing from `s.logits`
again. The acceptance rate becomes the target's probability of the draft token, so it sits below
the argmax match (`PERF_LEDGER.md`, the MTP section, for the measured gap); a null `spec_params` or
a zero temperature keeps the argmax walk bit for bit.

### 2.29 The depth a round drafts {#mtp-depth-knob}

**The depth a round drafts is a box knob per round kind, not a controller.** `get_mtp_depth()`
serves an explicit setting (`set_mtp_depth`, the `--mtp-depth` flag) when one was made, else the
box profile's `runtime.mtp_depth_assistant` or `runtime.mtp_depth_nextn` by the `MtpRoundKind` the
assistant drafter's attach and detach select. The per-position acceptance curve has the same shape
on every task and on both carriers - of the rounds at depth 4, position 1 is accepted in about
three quarters, position 2 in half to two thirds, position 3 in a third and position 4 in a
fifth (the `_depths` ruler records under `performance/records/mtp/`), so a fourth draft adds
about a tenth of a token per round - and nothing a controller could observe changes the depth ranking;
what does is the box's k-row verify cost - on the M5 depth 2 ties depth 1, on the M4 Pro depth 2
loses (`PERF_LEDGER.md`, the MTP section) - so the tuner mints the assistant knob as a
serving race of depth 1 against depth 2 on the SpecBench chat corpus with a gemma-4 vehicle and its
`mtp-` head (depth 2 must beat by 2% - its downside is asymmetric and a tie is not worth the longer
round; a synthetic 32-token prompt is not the site shape, the drafter accepts less of incoherent
text; no vehicle means depth 1). The NextN knob defaults to 1, the depth every
NextN carrier measured best.

### 2.30 The kernel argument-alignment contract {#metal-dispatch-requires}

**A kernel's argument-alignment contract is declared on its `[metal_dispatch]` and enforced at
every dispatch.** `requires = "lhs % N, ..."` (lhs a `params=` name or a kargs field) makes the
generated builder check each item before it binds and call `metal_requires_failed` on a miss -
a panic naming the builder and the broken contract, or the test hook installed with
`set_metal_requires_hook`. The check is one integer modulo per dispatch, so the kernel keeps its
tail-guard-free main loop and the DRIVER does the shape routing: the fixed-B mul_mv forms stripe
K in 256 (B2) / 128 (B4) element chunks, and the batched decode driver gates each mv site on its
own K (`mv_kdim`, `mv_wo`, `mv_w2`), falling to the tail-exact GEMV form where the alignment
fails (gemma-4-26B-A4B's dense hidden 2112 on the w2 site). Every `[metal_dispatch]` GEMM form
carries its grid divisors the same way - the production mul_mm `mp % 32, d % 64`, the K-quant
mul_mm twins `mp % 32`, the 32- and 64-wide GEMM-B forms and their tensor twins `ka.ndim % 32|64` with the q8 block `ka.kdim % 32`,
the split-K pair `d % 32` - so a grid that would have truncated silently now names the site.
A contract on a value that reaches the builder only as a bound uniform BUFFER (the mul_mm's K)
stays with the caller: the dispatch never pays a readback.

### 2.31 The K-quant small-batch form is a per-box crown {#kq-rows-crown}

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
4-row tile (sec.2.32), so its nine rows (`MTP_MAX_ROWS`, depth 8) ride the small-batch forms: the
first eight as the eight-row dispatch, each row past eight as a single pass at its own x and y row
offsets. An unpadded nine-row panel handed to the mul_mm dispatches no threadgroup at all; the
twins' `mp % 32` contract names that site instead of leaving the output unwritten.

### 2.32 The verify's row buffers are padded to the GEMV form's row tile {#verify-row-pad}

**A multi-row verify sizes every row buffer to a whole 4-row tile, and the pad rows are owned
scratch.** `acquire_step` takes `mp = ceil(nrows / 4) * 4` (a single-row step stays at 1) and sizes
`bx`, `bxb`, `bqkv`, `bh12`, `blog` and the deltanet rows to `mp` rows; the rope tables stay at the
live `nrows`. The fixed-B GEMV forms write a full tile - `enc_gemv_rows` dispatches the B2 form at
one or two rows, the B4 form at three or four, two B4 tiles at five to eight and three at nine to
twelve, each tile past the first offsetting x by its first row's count of n floats and y by the
same rows of the site's y stride - so the spare rows of the last tile compute garbage that must
land inside an allocation this step owns and nobody reads. A row buffer sized to the live count puts that garbage on whatever the pool put next to it.

### 2.33 The NextN draft chain reads only what its own chain wrote {#mtp-nextn-chain}

**A chained draft carries the head's own hidden and may read slab rows only above the round's
base.** The chain fills the verify batch: `vbatch[0]` is the committed token, `vbatch[i]` is draft
`d_i` decoded from `(d_{i-1}, the draft head's own h)`, and `d_1` alone comes from `(tok, the
trunk's stashed h)` - which the round saves in `s.mtp_xb_save` before the first draft, because the
chain overwrites `s.mtp_h`. Each step runs `metal_mtp_draft_forward` at `pos + i` against the trunk
mirror, and it is gated on the round's BASE position (`wm_pos`), not on `pos + i`: the trunk
watermark only has to cover the base, since a draft at `pos + i` reads exactly the slab rows its own
chain wrote above it, while the slab capacity must still cover the row being written. Gating on the
drafted position refuses every round past the first draft.

### 2.34 The verify step re-warms the draft slab from committed history {#mtp-verify-draft-warm}

**The NextN verify encodes the draft head as one more layer and re-warms its K/V slab in the same
command buffer.** `encode_verify_step` builds `nrows` same-slab route entries for every trunk layer
PLUS `n_layer_nextn`, and the extra entry addresses the draft head's own slab. After the trunk rows
norm and classify - row 0 is the truth for the committed token, row i the truth for draft i - the
step assembles the head's inputs from committed history: `h_0` is the saved pre-draft `mtp_h` parked
at `bcat` row `nr`, and `h_i` is verify post-norm row `i-1`. It norms the embeds with `mtp_enorm`
and the hiddens with `mtp_hnorm`, interleaves `[enorm(embed_i) ; hnorm(h_i)]` per row into the cat
image, runs `mtp_ehproj`, and encodes the verify layer once more at `l == n_layers`. The head
therefore enters the next round on rows the trunk actually committed, not on the chain's own drafts.

### 2.35 A recurrent verify writes a shadow region and replays into it {#mtp-dn-shadow-replay}

**Recurrent layers under a verify never touch their live state, so a partial accept costs one
replay instead of a re-forward.** When `r.dn_split` is set, every deltanet store of the verify - the
conv-history store and the scan's final state - lands in the DnMirror's alias (shadow) region
(`dn_state_alt` / `dn_conv_alt`, the kernels' `split = 1`), and the live region keeps the pre-verify
state. The same pass tapes each recurrent layer's scan inputs into `btape` as `[cv rows | qkv rows |
beta rows | alpha rows]`, `tape_slot` bytes per taped layer. After the accept walk the shadow holds
the state as of row k; when the walk accepted a < k, `encode_dn_replay` re-runs the history and scan
kernels over rows 0..a of the tape, reading the LIVE pre-verify state and writing the shadow again -
the same kernels over the same inputs, so the boundary state is bit for bit what the verify produced
for that prefix - and `dn_mirror_flip` then makes the shadow live at `pos + a + 1`. A verify whose
dispatch failed mid-scan leaves the mirror neither live nor re-derivable, so the round drops it.

### 2.36 One assistant-drafter step {#assistant-drafter-step}

**The drafter is a Q-only head on the target's cache at ONE frozen anchor.** A step takes
`(tok, h)`: it concatenates `embed(tok) * sqrt(tdim)` with the target hidden `h` into a `2 x tdim`
row, projects it down with `pre_projection`, runs the drafter's layers, applies `output_norm`, and
reads the normed hidden twice - through the token table for logits (the draft is their argmax) and
through `post_projection` for the `tdim`-wide hidden the NEXT step consumes in `h`'s slot.

A layer is rms -> Q -> per-head q_norm -> rope at the anchor -> attention -> wo -> post-norm ->
residual, then rms -> gate/up -> geglu -> down -> post-norm -> (residual + branch) * layer_scale.
The drafter owns no K/V projection: layer `l` attends the TARGET's layer `kv_layer[l]` rows
`[0, anchor)`, GQA-broadcast over that layer's kv heads at scale 1.0, and a sliding layer masks
`anchor - key >= window` (low bound `anchor - window + 1`). The anchor is the seed's own position -
the target's `n_past` - so the seed row itself is unfed and every draft of a round ropes at the same
position. Each attention class ropes on its own terms: the full class over `hs` at the target's
`rope_freq_base` with the p-RoPE frequency table, the sliding class over its own head size at
`rope_freq_base_swa` with none.

### 2.37 The batch driver's same-slab mode {#batch-same-slab}

**In same-slab mode every batch row is the SAME session at consecutive positions, so the mirror is
prepared once and the whole step lands on one session.** `verify_batch_step` fills the workspace
with `nrows` copies of one session handle at `pos .. pos+nrows-1` and arms
`set_batch_same_slab(true)`; the Metal batch driver then calls `mirror_prepare` only for row 0, at
the base position, and every row shares that slab - the rows above the base are exactly what this
step writes, so row i attends the rows below it that the same step produced. The landing side
follows: the zero-copy per-session logits scatter stands down, because all rows would scatter to one
session, and `land_sameslab_rows` copies every row's logits into `s.mtp_logits_b` (row 0 also into
`s.mtp_logits`, which only a NextN-headed session has sized), the post-final-norm hidden rows into
`s.mtp_hrows`, and the GPU per-row argmax into `g_sameslab_arg`.

### 2.38 The single-row driver's greedy chain {#greedy-chain}

**The single-row driver pre-encodes the next step on the GPU's own argmax, and only a greedy
caller can afford it.** `pre_encode_next` encodes step `pos + 1` while step `pos` runs. Armed,
the new command buffer opens with an argmax over the running step's logits and the embed gather
of the winner (`r.spec`), commits at once, and runs the moment the previous one drains - the CPU
tail, the sampler and the next `forward()` all overlap GPU execution. Unarmed, the step is encoded
but held, and `forward()` pokes the caller's token and commits it - the encode still overlaps, the
commit turnaround does not. `finish_step` then compares the GPU's pick with the caller's token: a
miss waits the chained step out, discards it, and reruns the step through the slow path, so a miss
costs about two steps (30 ms against a 15 ms step on gemma-4-12B, M5 Max) where a hit saves the
turnaround, about 2% of a step. The adaptive mode (`DASLLAMA_METAL_SPEC` -1) backs off
exponentially after a miss and re-arms on the next hit; under a sampler that agrees with the
argmax two steps in three it still costs 63% of the step (26.0 against 15.9 ms), because the
re-arm attempts far more often than a 2% upside pays for. So the sampler decides: `sample_` marks
the session `sampled` whenever it draws at a temperature above zero, and the driver never chains
such a session; a greedy sampler (temp 0) and a caller feeding the argmax directly keep the
adaptive chain.

### 2.39 The verify encodes on the serial encoder {#verify-serial-encoder}

**The NextN round's verify builds its whole step on one serial compute encoder - the one decode
path that does not take the concurrent rail.** `encode_verify_step` opens a single encoder on the
round's command buffer, so every dispatch is implicitly barriered on the one before it. The
concurrent rail buys nothing here: measured through the round's profiler sections on Qwen3.8-27B
(M5 Max), the two forms show the same GPU time and the concurrent one doubles the host encode, so
the round keeps the serial encoder and pays no hazard-tracker work per dispatch.
