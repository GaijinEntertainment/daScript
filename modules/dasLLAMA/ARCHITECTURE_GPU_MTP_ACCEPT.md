# dasLLAMA Architecture - the sampled stream's acceptance

Companion to `ARCHITECTURE_GPU_MTP.md`; a section is cited by its anchor. This document carries
what a sampled stream's speculative round does with its drafts on both GPU homes: the three
acceptance modes, the seams the sampler installs for them, the drafter distribution a
rejection-sampling round records, and the chain cut. The round itself and the greedy walk every
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
