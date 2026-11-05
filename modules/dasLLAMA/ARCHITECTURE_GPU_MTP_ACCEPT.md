# dasLLAMA Architecture - the sampled stream's acceptance

Companion to `ARCHITECTURE_GPU_MTP.md`; a section is cited by its anchor. This document carries
what a sampled stream's speculative round does with its drafts on both GPU homes: the three
acceptance modes, the seams the sampler installs for them, the drafter distribution a
rejection-sampling round records, the DFlash2 block drafter's and the NextN head's drafts under each mode, and the chain cut. The round itself and the greedy walk every
stream shares are `ARCHITECTURE_GPU_MTP.md#mtp-round-one-join`; the sampler's own cuts and draws
are the sampling file's charter in `ARCHITECTURE_ENGINE.md`.

### The three acceptances of a sampled walk {#mtp-accept-modes}

**A greedy stream's walk is the argmax match; a sampled stream's walk is one of three
acceptances, a process-wide setting (`set_mtp_accept`, the `--mtp-accept` flag), rejection
sampling by default - the lossless walk that uses the head's whole distribution, where exact
stays the seeded decode's twin the parity cells compare against.** Each walks the verify's k+1
rows in order, judges draft i against row i, stops at
the first row that does not accept its draft, and parks that row's draw as the stream's next
token (`s.mtp_pre_tok`), so a round draws once and the sampler the caller runs afterwards returns
the parked token instead of drawing again. One walk (`mtp_walk_judged`) serves the three
acceptances: it judges every row through the one judge seam the sampling file installs at its
`[init]` (`register_mtp_judge`, the acceptance passed per row; the drafter's draw is the other
seam, `register_mtp_draft_draw`), so the engine file never reads `SamplingParams` past the argmax
test; a sampled stream reaching a walk with no seam installed panics, since the program forgot to
require the sampling module.

- **exact** draws each row through the session's sampler and accepts the draft where the draw
  equals it. The committed stream is a seeded sampled decode's twin token for token, and the
  acceptance rate is the chance a draw lands on the head's greedy pick: on the 35B at
  temperature 0.7 under a 0.9 nucleus, about half.
- **typical** accepts the draft where the sampler's cuts (penalties, top-k, the nucleus, min-p,
  at the temperature) admit it on its row, whatever the draw, and draws the first cut draft's row
  over its survivors, never landing on the cut draft. Every committed token is one the sampler
  would have admitted, but the stream leans toward the head's picks; a sampler with no cut admits
  every draft, so a typical stream carries a nucleus or a top-k.
- **rejection** is speculative rejection sampling. The head draws its draft through the
  sampler's chain over its own row and records that chain's distribution as q; the verify row's
  post-cut p accepts the draft with probability min(1, p[d] / q[d]), and a rejected row draws from
  the residual max(0, p - q) renormalized, which never lands on the rejected draft. The committed
  stream is distributed as the sampler's, with the head's whole distribution used instead of its
  argmax; the drafts themselves are sampled, so at depth 1 the round does not delegate to the
  greedy eval.

**The drafter distribution is recorded per draft row, capped.** `s.mtp_q` holds draft row i's q
at `i * MTP_Q_CAP`, `s.mtp_qn[i]` entries, id ascending, renormalized to sum to one. The
candidate-list path records its survivors whole; the vocab-wide path records its `MTP_Q_CAP`
largest survivors with the tail's mass renormalized into them, so a flat row's tail is folded into
its head rather than carried. A greedy or constrained sampler records its argmax as a point mass,
which the rejection judge accepts wherever the row gives it any mass. The residual draw walks
the row's survivors with q subtracted, and where the residual is empty - the row holds q whole -
it falls back to the row's own draw. A round whose drafts recorded no q at all - the assistant
drafter's, picked on the device by argmax - walks typical under the rejection knob
(`mtp_walk_by_accept`): the greedy draft is what typical acceptance judges, and there is no q row to
read.

**The chain cut stops drafting at the first draft the head holds under a probability floor**
(`set_mtp_chain_min_p`, the `--mtp-min-p` flag). The head's own softmax over its row gives a
greedy draft's probability (off the landed selection where the draft command landed one); a
sampled draft's is its q - the draw consumed the head's row, and the sampler's chain over it is
the distribution the draft came from. A draft under the floor still enters the verify (the row past it is the
round's draw), but no draft is chained behind it, so a round spends its verify rows where the head
is confident. The floor is read on every acceptance; on the exact walk at depth 3 on the 35B it
buys a few percent where the uncut depth-3 round loses to depth 2 (`PERF_LEDGER.md`, the MTP
section).

### The block drafter's drafts under the three acceptances {#dflash-sampled-drafts}

**The DFlash2 round judges its block's drafts with the same walk, and only rejection sampling changes what the drafter
does.** The verify's rows are the trunk's, so row i's p is the sampler's post-cut distribution (temperature, top-k, the
nucleus, min-p, penalties) whatever drafted; the drafter decides only which d each row is judged against and what q the
judge reads.

- **greedy** (a bare argmax sampler): the draft is accepted while it equals the row's argmax - the walk the device
  selector and the device accept serve.
- **exact** (`set_mtp_accept(MtpAccept.exact)`): the drafter walks greedy and records no q; each row is drawn through
  the sampler with the session's rng and the draft accepted while the draw equals it, the first miss's draw committed.
  The draws are one a committed token in stream order, the drafter draws none, so a seeded speculative stream is the seeded
  plain sampled stream (`generate_mtp_sampled` against `generate_`) token for token wherever the verify's rows and the plain
  steps' rows give the draw the same distribution. Acceptance is p(d) for the greedy d.
- **rejection** (the default): draft d is accepted with probability min(1, p(d) / q(d)); a miss draws from the normalized
  max(0, p - q) over the row's survivors, which never lands on d. The drafter's temperature (`set_dflash_draft_temp`)
  defaults to the stream's own sampler temperature; pinned at 0 the drafts stay greedy and the land records each as a
  point mass q = 1 at d, so a row accepts with probability p(d) - the exact walk's rate - and the residual is p with d
  removed. Above 0 the drafter samples: position r's q is the softmax at that temperature over its top-k candidates'
  selector scores
  (`dflash_scores`: logit + sum_i A[pred][i] H[r][i] B[cand][i]), the draft is one draw of the session's rng from it
  (`dflash_soft_pick`, the host's; `MetalDflashWalkSoft`, the device's), and the pick is the next position's predecessor as on
  the greedy walk (`dflash_walk_sampled`).
  q is nonzero on the top-k candidates alone, so the residual is p with those masses reduced and renormalized. A row
  accepts with probability sum_x min(p(x), q(x)), which beats the point mass's p(d) where q spreads its mass over the
  candidates p holds and loses where q leans on tokens p does not; the drafter's temperature is the lever. The stream is
  distributed as the sampler's under every temperature, but its draws are not the plain stream's, so a seeded rejection
  stream is not token-equal to the plain one.

**The device selector draws the sampled drafts; the judge stays the host's.** A sampled round asks no device accept
(`mtp_seat_accept_asked`), so its verify rows land - their logits, or the selection landing - and the judge reads them on
the host; its pass holds for the verify as the greedy one does (`ARCHITECTURE_GPU_MTP_ROUND.md#verify-ahead`). A
round drafting above temperature 0 ends its pass in the sampled walk stamp (`MetalDflashWalkSoft`, under
`DASLLAMA_METAL_DFLASH_SOFT_PICK`): the session's LCG state rides in a word buffer, each drawn position steps it once as
`random_float` does, and the round lands the picks, every position's candidates and q, and the stepped state, so the host
records q exactly as its own walk does and the judge's draws follow on the same stream. The scores are the greedy walk's
bit for bit and the draw is the host's integer arithmetic, so the device and host drafts are the same ids from the same
state; q parts from the host's by the transcendental's ulp alone. The knob off takes the pass with no selector on the device
(`DflashDeviceSeams.draft_pass`, its rows' logits landed), the top-k and the walk on the host - the reference the device draw
is held to. A round at 0 keeps the greedy selector, whose picks land before the walk and are recorded as point masses then.

### The NextN head's drafts under the three acceptances {#nextn-draft-temp}

**The NextN head drafts as the block drafter does under greedy and exact; under rejection its temperature is its own
knob, defaulting to the stream's.** Under greedy and exact the head's chain is its argmax and records no q - exact
draws each verify row and accepts while the draw equals the draft. Under rejection every draft goes through the
drafter's draw seam (`mtp_draft_sample`), which records q, so the walk never meets a sampled stream's draft without
one. `set_mtp_draft_temp` picks q: 0 drafts the head's penalized argmax as a point mass, the exact walk's acceptance
p(d) with the residual draw; above 0 the stream's chain (penalties, top-k, the nucleus, min-p) at that temperature
over the head's row; below 0, the default, at the stream's own temperature - the rejection round of
`#mtp-accept-modes`. The block drafter's default is the point mass instead (`set_dflash_draft_temp`). A constrained
stream's draft records a point mass whatever the knob; the scheduler keeps a constrained stream out of the round.

### The block drafter's captures off a hyper-connection trunk {#dflash-hc-capture}

**A hyper-connection trunk hands the block drafter the plain mean of its streams, because that is the layer output its
draft was trained on.** The drafter's context is the concatenation of its target layers' output rows through `fc`; a
trunk with one residual stream captures the row as it stands after the layer's FFN add (`s.x`, the window's `s.x_b`, the
device step's `bx`). Under hyper-connections a layer's output is its wide residual - `hc_count` streams of `dim` after the
FFN's scatter (`hc_res`, `hc_res_b`, the device's `bhc`), before the next layer's side input and mixer - and the row the
drafter reads is the arithmetic mean over the streams, the sum scaled once by 1 / hc_count: on the CPU `stream_mean_row`,
on the device `MetalHcStreamMean` into the capture image's slot at the decode step's and the window's layer-end seams.
The gated means the mixers form (`GkHcMix`, the head mixer) are not it: they weight each stream by its sigmoid gate, and
the Flash-Next draft (`Qwen3.8-Flash-Next-DFlash2`) was distilled against the unweighted mean. The drafter's width stays
the trunk's `dim`, so the same `fc`, ring and selector serve; the carry the NextN head reads stays the wide row
(`mtp_h_dim`).
