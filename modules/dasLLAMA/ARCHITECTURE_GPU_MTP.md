# dasLLAMA Architecture - the Metal speculative round

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document
carries sections 2.28-2.29, 2.33-2.37a and 2.39: the speculative round on Metal, the box knob
that sets the depth a round drafts, the draft chains and the verify that commits them, and the
assistant drafter. `ARCHITECTURE_GPU_MTP_DECODE.md` beside it carries sections 2.30-2.32, 2.38,
2.38a, 2.39a and 2.39b - the decode driver's layer encoder, the kernel forms and row-buffer pad a
multi-row verify dispatches under, the single-row driver's greedy chain, the argument-alignment
contract a kernel declares on its `[metal_dispatch]`, and the rotated prefix a rope-store kernel
takes. The GPU backend role table these
sections build on, the assistant-drafter driver's role row included, stays in
`ARCHITECTURE_GPU.md#gpu-backends`.

### The speculative round on Metal {#mtp-round-one-join}

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
rewrites - and only the watermark keeps them from being read. The CPU depth-1 step
(`mtp_spec_eval`) takes a rejected draft on a model with no recurrent layer without a re-forward:
the verify's row 0 already holds the committed token's logits and post-norm hidden, so they stand
and the draft head is re-seeded from them; a recurrent model restores the pre-verify state and
re-forwards the committed token. `set_mtp_force_reject_every(n)` rejects every n-th draft of that
step's greedy walk whatever the verify said - the seam a test reaches the reject arm through on a
fixture that accepts every draft.

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
the argmax match (`PERF_LEDGER.md`, the MTP section, for the measured gap). A null `spec_params`,
or a sampler that is a bare argmax (temperature zero, no penalty), keeps the argmax walk bit for
bit; a penalized greedy sampler goes through the walk too, because its pick is the penalized
argmax and the raw-argmax match would accept drafts the penalty rejects.

### The depth a round drafts {#mtp-depth-knob}

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
text; no vehicle means depth 1). The NextN knob defaults to 1: every NextN carrier measured
best there on the M4 Pro, and the M5 Max's depth-3 point (1.31x against 1.23x at depth 1 on
Qwen3.8-27B, the ruler's `_depths` record) is a direction-grade reading the tuner does not yet
mint into the knob.

### The NextN draft chain reads only what its own chain wrote {#mtp-nextn-chain}

**A chained draft carries the head's own hidden and may read slab rows only above the round's
base.** The chain fills the verify batch: `vbatch[0]` is the committed token, `vbatch[i]` is draft
`d_i` decoded from `(d_{i-1}, the draft head's own h)`, and `d_1` alone comes from `(tok, the
trunk's stashed h)` - which the round saves in `s.mtp_xb_save` before the first draft, because the
chain overwrites `s.mtp_h`. Each step runs `metal_mtp_draft_forward` at `pos + i` against the trunk
mirror, and it is gated on the round's BASE position (`wm_pos`), not on `pos + i`: the trunk
watermark only has to cover the base, since a draft at `pos + i` reads exactly the slab rows its own
chain wrote above it, while the slab capacity must still cover the row being written. Gating on the
drafted position refuses every round past the first draft.

### The verify step re-warms the draft slab from committed history {#mtp-verify-draft-warm}

**The NextN verify encodes the draft head as one more layer and re-warms its K/V slab in the same
command buffer.** `encode_verify_step` builds `nrows` same-slab route entries for every trunk layer
PLUS `n_layer_nextn`, and the extra entry addresses the draft head's own slab. After the trunk rows
norm and classify - row 0 is the truth for the committed token, row i the truth for draft i - the
step assembles the head's inputs from committed history: `h_0` is the saved pre-draft `mtp_h` parked
at `bcat` row `nr`, and `h_i` is verify post-norm row `i-1`. It norms the embeds with `mtp_enorm`
and the hiddens with `mtp_hnorm`, interleaves `[enorm(embed_i) ; hnorm(h_i)]` per row into the cat
image, runs `mtp_ehproj`, and encodes the verify layer once more at `l == n_layers`. The head
therefore enters the next round on rows the trunk actually committed, not on the chain's own drafts.

### A recurrent verify writes a shadow region and replays into it {#mtp-dn-shadow-replay}

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

### One assistant-drafter step {#assistant-drafter-step}

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

### The batch driver's same-slab mode {#batch-same-slab}

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

### Every stream's round verifies in one pass {#mtp-joint-verify}

**A scheduler tick verifies every speculative stream's rows together.** `metal_mtp_spec_rounds`
takes the tick's streams: each drafts its k tokens on its own chain (a cold stream, or one whose
draft declines, plain-steps and leaves), then the warm streams' k+1 rows go through ONE verify -
`g_v_groups` names each stream's rows (`row0`, its slab's `koff`/`voff`/`cap`, its recurrent
mirror's live and shadow bases, its tape slots), the route table carries every row's own slab and
position, `recurrent_verify` scans each group's rows against that stream's mirror through the
kargs row index with the shadow-and-tape discipline of `ARCHITECTURE_GPU_MTP.md#mtp-dn-shadow-replay` per group, and the draft head's
inputs assemble per group (a group's saved pre-draft hidden parks at cat row `nr + its index`).
The landing scatters each group's K/V rows and logits into its own stream; the accept walk, the
replay (one command buffer per replaying group), the flip and the commit run per stream exactly as
the one-stream round's. The one-stream round is the rounds over one session. `mtp_spec_eval_batch`
is the engine seat the scheduler ticks through (`register_mtp_spec_batch_override`); without a
driver seat each stream steps alone through `mtp_spec_eval`, and the tick reports how many
streams' rows the joint verify carried so the batched bench row can refuse a per-stream tick.
A plain batched step on a NextN-headed model lands GPU logits like any other model's, and its
landing copies each row's post-norm hidden into that stream's `mtp_h` with the watermark moved
(`land_row_carry`), so a stream that decoded plain - the bench row's plain arm, a server slot
with speculation off - reaches its next speculative round warm instead of cold-forwarding; the
same-slab verify lands its rows into `mtp_hrows` instead and the walk sets the carry per group.
The drafts are rows steps too: draft i of every warm stream is one rows step (`mtp_draft_rows`)
whose row is that stream's previous draft at its own trunk position - the same route-table fill
as the verify (`group_routes`), the draft head's rows form (`encode_draft_rows_step`: the
enorm/hnorm rows into the cat, one eh_proj pass, the draft layer through the rows-form layer
encoder at every row's own slab, the head norm and ONE classifier pass) - and the landing writes
each row's draft-slab K/V row and carry hidden into its stream and takes the row's argmax as its
next draft, so a k-deep round reads the draft layer and the classifier plane k times for the
tick, never once per stream. The rows step always takes the four-row tile (`nrows = max(ng, 3)`,
the rows past the streams cloning row 0's inputs and route), because the two-row tile sums in
another order and a solo stream's drafts must round as they do beside others - the joint
invariance cell's claim. A stream whose mirror cannot take the row (its watermark or capacity
short) declines the whole rows step and every warm stream plain-steps.

### The verify encodes on the serial encoder {#verify-serial-encoder}

**The NextN round's verify builds its whole step on one serial compute encoder - the one decode
path that does not take the concurrent rail.** `encode_verify_step` opens a single encoder on the
round's command buffer, so every dispatch is implicitly barriered on the one before it. The
concurrent rail buys nothing here: measured through the round's profiler sections
(`lcpp_bench --mtp-ab --prof --for-debug-purposes` under `JOBQUE_PROFILING=1`, `-jit`, Qwen3.8-27B
on the M5 Max), the two forms show the same GPU time and the concurrent one doubles the host
encode, so the round keeps the serial encoder and pays no hazard-tracker work per dispatch.
