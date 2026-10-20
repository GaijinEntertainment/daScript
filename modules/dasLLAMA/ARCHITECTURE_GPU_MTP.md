# dasLLAMA Architecture - the Metal speculative round

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document
carries the Metal seats of the common speculative round, the box knob that sets the depth a round drafts, the
draft chain and the verify that commits it, and the recurrent layer's stages over groups of rows
with the mirror pool they read; the assistant drafter on both homes is `ARCHITECTURE_GPU_MTP_ASSISTANT.md`,
and a sampled stream's three acceptances, the drafter distribution a rejection-sampling round records
and the chain cut are `ARCHITECTURE_GPU_MTP_ACCEPT.md`.
`ARCHITECTURE_GPU_MTP_DECODE.md` beside it carries the decode driver's layer encoder, the kernel forms and row-buffer pad a
multi-row verify dispatches under, the argument-alignment contract a kernel declares on its `[metal_dispatch]`, and the rotated
prefix a rope-store kernel takes; the single-row driver's greedy chain and the Metal selection landing are
`ARCHITECTURE_GPU_MTP_DECODE_PICK.md`. The GPU backend role table these
sections build on, the assistant-drafter driver's role row included, stays in
`ARCHITECTURE_GPU.md#gpu-backends`.

### The speculative round on Metal {#mtp-round-one-join}

**The speculative round is the common one on every home (`mtp_spec_round_k`), and the Metal decode driver fills its
seats.** The round drafts its k tokens through the draft seat (`metal_mtp_draft_seat`: the single-row draft forward,
`metal_mtp_draft_forward`, the mirror held at the round's base), verifies them as k+1 same-slab rows through the verify
seat (`metal_mtp_verify_seat`: one group over the session's slab, `encode_verify_step`), walks and commits on the host as
every home does, and rejects through the rollback seat (`metal_mtp_rollback_seat`, `#mtp-dn-shadow-replay`). The cold seat
(`metal_mtp_cold_why`) names why a round steps plain before any seat is reached - the verify's shape gate, the recurrent
mirror's position, the slab's rows and reach - since a blob model has no CPU rails for a declined seat to fall to. The
verify seat lands every row's pick (`mtp_picks`, the device argmax), post-norm hidden (`mtp_hrows`) and logits - none under
the round's pick ask - the last row's as the session's, leaves the mirror's watermark at the round's full depth and parks its step for the rollback seat; the plain
step, the next seat and the reload prep release a parked step. The gemma assistant drafter's round is one command buffer
at every depth: the verify encodes every speculating stream's k steps itself, as a serial pass at the
head of its first command buffer (`set_batch_same_slab_chain`, the two passes ordered by the
buffers' tracked hazards; `metal_mtp_chained_rounds` counts the steps). A stream's draft i embeds token
row `row0 + i` (its seed at its group's first row), its argmax lands in the next row on the device,
and its post_projection is copied on the device into the next step's input; the batch driver then
gathers the rows' embeddings off that token list on its own encoder instead of the workspace's CPU
rows, lands every row's argmax (`enc_argmax_rows`) with the logits, and the host reads tokens and
winners only after the verify's join. The drafter's h input is the target's
post-output_norm hidden, carried in `s.mtp_h` by the decode on head-less models
(`set_metal_mtp_carry_hidden`); the pre-norm residual is a lever that measured worse. The two-row
verify's GPU cost over a one-row step (the rows' weight stream; the per-box margins are in
`PERF_LEDGER.md`'s MTP section) is the physics the round cannot recover; everything the host did
between the drafts and the verify was. The verify's rows name ONE session, so the batch driver's
arena reservation (`batch_rows_prepare`) counts that session's slice once, not once a row: a deep
session's slice is a 16384-row cap, and k+1 of it would pass the arena ceiling a single one fits.
The continuation window's seam - the draft head's row `start_pos - 1`, written ahead of the window
by the Metal seam seat (`metal_mtp_seam_row`, the draft forward with no logits) - prepares the
session's mirror to the seam first: a new session's mirror is adopted or built by that prepare,
an attached one stands at its whole pages, and the rows to the seam come up from the cache as a
step's would. A seam the seat declines on a blob model stays stale and dents the window's first
draft (`mtp_seam_declines` counts them).

**The accept walk and the commit.** Row i of the verify is the truth for draft i+1, so the walk
takes `a` = the length of the leading run where a row's argmax equals the draft it verifies. The
round commits rows 0..a: row a's logits become `s.logits` (the token sampled next), row a's
post-final-norm hidden becomes `s.mtp_h` (the next round's carry, and the assistant drafter's h
input), `s.mtp_h_pos1` moves to `pos + a + 1`, and `n_past` advances by `a + 1`. The rollback seat cuts
the mirror's watermark to `pos + a + 1` with the recurrent state; a dense model's reject reaches no rollback
seat and leaves the watermark at the round's full depth. The rows above the committed one are the rejected
drafts' - garbage the next round rewrites - and `n_past` is what the next step reads. The walk ends on a draft among
the caller's stop ids (`Session.spec_stops`, the scheduler's stream stop list, `mtp_accept_cut_at_stop`): the finishing
token's row is the committed row `a`, the rows past it roll back as a reject's, and no pre-drawn token is parked - a
stream emits nothing past its stop, so the round leaves no row the stream's history does not carry. The CPU depth-1 step
(`mtp_spec_eval`) takes a rejected draft on a model with no recurrent layer without a re-forward:
the verify's row 0 already holds the committed token's logits and post-norm hidden, so they stand
and the draft head is re-seeded from them (a verify seat that landed the rows' picks alone under
the caller's pick ask publishes row 0's pick in the logits' place,
`ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command`); a recurrent model restores the pre-verify state and
re-forwards the committed token, unless the verify seat that served the round registered a
rollback seat, which puts the state back on the device and leaves row 0 standing the same way
(`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback`). `set_mtp_force_reject_every(n)` rejects every n-th draft of that
step's greedy walk whatever the verify said - the seam a test reaches the reject arm through on a
fixture that accepts every draft. Past depth 1 the common round (`mtp_spec_round_k`) takes this
section's shape on the CPU round's seats: the draft chain (draft i + 1 from the head's own hidden
after draft i, at head row pos - 1 + i, cut where a draft's probability under the head falls under
`set_mtp_chain_min_p`), one verify over the k + 1 rows through the verify seat (the CPU prefill with
the batched classifier where none serves), the walk above, and the commit at row `a` - a reject
through the rollback seat with row `a`'s landed hidden as the carry, else the snapshot restored and
rows 0..a re-run - so both drivers round at the knob's depth through their seats, capped at the rows the verify seat
serves (`register_mtp_verify_rows_override`). The depth is a
measured choice per carrier (`#mtp-depth-knob`): the 0.8B on the Vulkan tier reads x1.25 / 1.26 /
1.28 / 1.15 at depths 1 to 4 (`debug-jit`; the absolute rates are the 0.8B depth entry in `PERF_LEDGER.md`).

**A sampled stream's walk draws instead of comparing argmaxes, and the same walk serves every
round - the common round at every depth and the assistant drafter's.** The caller points `s.spec_params` at its
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
assistant drafter's attach and detach select; a scheduler tick drafts an assistant's rounds at it
(`ARCHITECTURE_GPU_MTP_ASSISTANT.md#assistant-drafter-rounds`) and a NextN head's at depth 1. The per-position acceptance curve has the same shape
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
mirror (the draft seat passes the round's base, `s.n_past`, as `wm_pos`), and it is gated on that base, not on `pos + i`: the trunk
watermark only has to cover the base, since a draft at `pos + i` reads exactly the slab rows its own
chain wrote above it, while the slab capacity must still cover the row being written. Gating on the
drafted position refuses every round past the first draft.

**A depth-1 greedy round on any table the device gathers is one command buffer.** The draft seat parks the
draft step (`g_v_chain`: opened and poked, uncommitted) and answers the committed token as the draft's placeholder,
which the host prep embeds into row 1; the verify seat encodes the parked draft's body ahead of its rows on its own
serial encoder, lands the winner in the step's `btok` (`enc_argmax`), embeds that slot over row 1 of its rows and of the
head's cat image on the device (`enc_embed_dev`), and after the join writes the winner into `s.mtp_vbatch[1]` - the
depth-1 step reads the draft back from there - and lands the draft row's K/V to the host cache as the slow draft's
landing does; one commit, one join, no readback between the draft and the verify (`metal_mtp_chained_rounds` counts the
rounds, the assistant's with them). The seat parks only where the round asks the pick alone (greedy, no chain cut) at
depth 1, the table gathers on the device and no n-gram side input reads the host's tokens before the join; a parked
draft whose arena moved under the verify's acquire, or whose verify takes other than two rows, runs the slow draft
first. A verify that refuses or whose command buffer failed drops the parked draft unrun. On the 0.8B the two-command
round reads 0.93 of the one-command round at depth 1 (`debug-jit`), the host join between them the whole gap.

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
The prefill window warms the same slab from the prompt as one more layer past the trunk: the
shifted embeds and the trunk's final hiddens through the norms and eh_proj, then the head layer's
attention norm, K/V projections, k-norm, rope and store alone - its query, attention, output
projection and FFN (a routed FFN on the 35B) do not run, since a round reads only the K/V rows. The
warm reads every row's final hidden, so the trunk's last layer keeps every row
(`ARCHITECTURE_GPU_PREFILL.md#prefill-last-row-tail`).
On a hyper-connection model the carry is the trunk's WIDE residual row before the head mixer
(`hc_dim` floats, `StepRes.bhc_carry` / `bhc_save`, `Session.mtp_h` sized by `mtp_h_dim`): the
head's prelude (`hc_draft_prelude`) norms each carry's streams under `mtp_hnorm`, repeats the
`mtp_enorm` embed row per stream and runs one eh_proj GEMV over every (row, stream) pair, so the
head's block opens on a wide residual of its own; the head mixer's own weights
(`mtp_hc_head_{norm,down,up}`) stand where the shared head's would.

### A recurrent verify writes a shadow region and replays into it {#mtp-dn-shadow-replay}

**Recurrent layers under a verify never touch their live state, so a partial accept costs one
replay instead of a re-forward.** When `r.dn_split` is set, every deltanet store of the verify - the
conv-history store and the scan's final state - lands in the DnMirror's alias (shadow) region
(`dn_state_alt` / `dn_conv_alt`, the kernels' `split = 1`), and the live region keeps the pre-verify
state. The same pass tapes each recurrent layer's scan inputs into `btape` as `[cv rows | qkv rows |
beta rows | alpha rows]`, `tape_slot` bytes per taped layer. The verify seat flips the shadow live at
`pos + k + 1` as it lands (`dn_mirror_flip`) and parks the step; when the walk accepted a < k, the rollback seat's
`encode_dn_replay` re-runs the history and scan kernels over rows 0..a of the tape, reading the pre-verify state
in the region the flip left behind and writing the live region again - the same kernels over the same inputs, so the
boundary state is bit for bit what the verify produced for that prefix - and moves the live position to
`pos + a + 1` (`dn_mirror_set_pos`). An n-gram model's conv ring is copied whole before the verify's rows commit into
it and put back whole by the rollback seat, which commits rows 0..a again and re-advances the host window
(`mtp_ple_window_restore`, `mtp_ple_window_commit`). A verify whose dispatch failed mid-scan leaves the mirror
neither live nor re-derivable, so the seat drops the mirror and declines the round.

### The batch driver's same-slab mode {#batch-same-slab}

A multi-stream speculative tick on Metal runs each stream's round alone - every plane read once a
stream, no layer kind batched across streams - and the scheduler's census counts such a tick as
no batched step (the server arms speculation single-stream).

**In same-slab mode the batch rows are groups of ONE session's consecutive positions, so each
group's mirror is prepared once and each group lands on its session.** `verify_batch_groups` fills
the workspace with each group's session handle at its `pos .. pos+nr-1` (`verify_batch_step` is the
one group), names the groups (`set_batch_same_groups`) and arms `set_batch_same_slab(true)`; the
Metal batch driver then calls `mirror_prepare` only for a group's first row, at its base position,
its demand at the group's last, and the group's rows share that slab - the rows above the base are
exactly what this step writes, so row i attends the rows below it that the same step produced. A
hybrid's recurrent rows are one group over the session's one mirror, walked row after row in place
(`ARCHITECTURE_GPU_MTP.md#dn-group-stages`); every row commits, so the mirror lands at the last row's
position plus one. The landing side follows: the zero-copy per-session logits scatter stands down,
and `land_sameslab_rows` copies each group's logits into its session's `s.mtp_logits_b` (its row 0
also into `s.mtp_logits`, which only a NextN-headed session has sized) and its post-final-norm
hidden rows into `s.mtp_hrows`, and every row's GPU argmax into `g_sameslab_arg`.
A plain batched step on a NextN-headed model lands GPU logits like any other model's, and its
landing copies each row's post-norm hidden into that stream's `mtp_h` with the watermark moved
(`land_row_carry`), so a stream that decoded plain - the bench row's plain arm, a server slot
with speculation off - reaches its next speculative round warm instead of cold-forwarding; the
same-slab verify lands its rows into `mtp_hrows` instead and the walk sets the carry per group.

The batch driver's weight sites dispatch a tensor at a time by its own tag, so a layer mixing q8 and
K-quant planes serves. The planes a batched step reads more than once: past eight rows, a K-quant
plane whose shape misses the mul_mm tile (rows a multiple of 64 over the panel's 32-row pad) takes
the eight-row form and one pass a row past it, as the deepest verify does; a per-32 dense plane
(q51, iq4nl32) rides its small-batch form (`ARCHITECTURE_GPU_QUANT_PLANES.md#metal-b32-small-batch`).

### The verify encodes on the serial encoder {#verify-serial-encoder}

**The NextN round's verify builds its whole step on one serial compute encoder - the one decode
path that does not take the concurrent rail.** `encode_verify_step` opens a single encoder on the
round's command buffer, so every dispatch is implicitly barriered on the one before it. The
concurrent rail buys nothing here: measured through the round's profiler sections
(`lcpp_bench --mtp-ab --prof --for-debug-purposes` under `JOBQUE_PROFILING=1`, `-jit`, Qwen3.8-27B
on the M5 Max), the two forms show the same GPU time and the concurrent one doubles the host
encode, so the round keeps the serial encoder and pays no hazard-tracker work per dispatch.

### The recurrent stages run over groups of rows {#dn-group-stages}

**A recurrent layer's conv, history advance, q/k L2, scan and gate are one dispatch each, whatever the
rows.** A group is one session's rows against its mirror: the single row is one group of one row, the
batch step a group a row (a row a session), the same-slab step and the verify seat's rows a
group of its k+1 rows, the prefill window a group a segment. The rows sit back to back in the planes,
and a small table names each group's first row, its row count, its live and shadow state and
history bases, its capture count and the word its capture records start at (`dn_grp_push`, eight words a group; `dn_grp_cap`,
three words a capture past every group's eight: the row the state stands after, its state and history bases in the capture
arena the stages bind as `DnStages.capb`) - the prefill window's checkpoint stops inside it, which the scan writes its
register state to as it passes the row and the history advance writes the taps rows before the row to, each copied into its
snapshot once the window completes (`dn_captures_from_arena`); the conv grid spans the longest group's rows by the
group count, the history advance and the scan take the group count as their last axis, and each
group's scan walks its own rows in order against its own state. The L2 and the gate read no state, so
they run over every row as one window. `enc_dn_front` and `enc_dn_back`
(`dasllama/dasllama_metal_kernels.das`) encode the stages for every row shape; the verify tapes its
groups' operands between them and the replay runs the history and scan over one group's tape. The
layer's slice inside a region rides the kargs (`DnArgs.sslice`, `cslice`), so one table serves every
layer of a step. F32-on-disk beta and alpha planes (the 35B) run as one router GEMV over the two planes
concatenated, each row's [beta; alpha] landing in one plane the scan reads at a doubled row stride
(`DnArgs.bstr`, `goff`); a tagged pair keeps a site and a plane each.

### The recurrent mirrors are slots of one pool {#dn-mirror-pool}

**Every session's `DnMirror` is a slot of one device buffer, so one dispatch binds every group's
state.** A slot holds three regions and the n-gram ring: the live region, and the next one a step that
must leave the live state whole writes - a verify's shadow, the greedy chain's step - with a third for the
chained step behind it (`DN_REGIONS`, `dn_region_next`). The pool grows by doubling to the mirror cap
(`dn_mirror_room`) once every step that may still write a slot has landed or retired (`mirror_quiesce`, which an
eviction and a host write into a slot run too), copying the slots it held, and a slot size that changes
(another model) drops every mirror.
The cap is an LRU over the slots: the least recent mirror evicts into a host stash of its live
region and ring at the position it expects, and that session's next prepare at that position
rebuilds the slot from the stash (`dn_mirror_restores` counts them) - the state is forward-only and
a blob-only load has no CPU chain to rebuild it, so an eviction keeps it. The stash has no cap: an entry
leaves with its rebuild or with its session, whose `delete` - the facade's and the scheduler's stream release alike -
runs `SessionOwner`'s finalizer, which tells every `register_session_release_listener` subscriber the uid is gone, and
the Metal driver drops that uid's mirror and stash (`dn_mirror_released`). So every entry is a live session's and no
live session's state is ever dropped, however many sessions step in turn; a session never deleted keeps its entry. A
snapshot of an evicted session reads the stash into the host arrays.
A weights reload (`register_reload_prep`, `dn_pool_drop_idle`) frees the pool only when no mirror
and no stash is left in it: every model load bumps the epoch, a second model's included, so a live
session's entries stay and leave with the session, and a new session's uid, never reused, can
adopt no entry an earlier model wrote.
