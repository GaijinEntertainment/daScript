# dasLLAMA Architecture - the Metal decode driver's token pick and selection landing

Companion to `ARCHITECTURE_GPU_MTP_DECODE.md`; a section is cited by its anchor.

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
`-jit`) where a hit saves the turnaround, about 2% of a step. The adaptive mode (`DASLLAMA_METAL_SPEC` -1)
backs off exponentially after a miss and re-arms on the next hit; under a sampler that agrees with the argmax
two steps in three it still costs 63% of the step (26.0 against 15.9 ms), because the re-arm attempts far
more often than a 2% upside pays for. So the sampler decides: `sample_` marks
the session `sampled` whenever its pick can differ from the raw argmax - a temperature above zero,
or a penalty at temp 0 (`sampler_is_argmax` is the negation) - and the driver never chains such a
session, because the chain predicts the raw argmax the penalty then rejects. Only a bare argmax
sampler (temp 0, penalties off) and a caller feeding the argmax directly keep the adaptive chain.
The winner's embed gather reads the table the host's `embed_row` would (`enc_embed_dev`): the Q8
linear copy (`MetalEmbedQ8`), the table's K-quant plane in its own format, tied or untied (the
`MetalEmbedKqT` stamp of that format), or the fp32 token table in fblob (one row through
`MetalRowGather` under the embed scale, the plane wrapped zero-copy through `plane_of`). The same gather opens every unchained
single-row step, off the token `forward()` pokes, and the MTP round's depth-1 chain takes every table
(`ARCHITECTURE_GPU_MTP.md#mtp-nextn-chain`). A recurrent model chains on its mirror's three regions: a chained step reads the
region the step in flight writes and writes the next, so a miss lands the region it read live again (`dn_mirror_land`) and
the rerun steps from the state the chain never touched. An n-gram model's step is encoded ahead and held, never chained
(`StepRes.png`): its heads are the host's hash gather, poked with the token like the embed row, and the held step writes
its one ring slot on the confirmed token alone, so the ring needs no second copy. The held step binds the session's ring
at the mirror's base (`dn_mirror_ple_off`) exactly as the slow path does: every step a session takes, held or slow,
reads the conv taps the previous step wrote. A device gather off the picked token would open the chain to it
(`followup_metal.md`).

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

The GPU argmax reproduces the CPU sampler's tie-break exactly: a lane keeps the earliest of equal values
(strict `>`), and the cross-lane fold takes the lower index on a tie, so the lowest index wins - what lets
the chain predict a bare-argmax sampler's pick bit for bit.

### A sampled caller chains on the device's draw {#sampled-chain}

**A sampled session chains like a greedy one when the device can draw its token.** The chained
step opens on `MetalSelDraw` over the step in flight's selection landing instead of the argmax
(`r.spec_draw`): the kernel reproduces the host sampler's candidate path over the landing - the
k-th largest survivor's cut with its ties, the temperature, the softmax summed in id order, the
nucleus walk over the numerators largest first, the min-p floor, one step of the session's LCG
(`daslib/random`'s `random_int` on the first component) and the cdf walk in id order - element by
element in the host's float order, so the id it lands is the one `sample_` draws over the same
landing, and the host's own draw is the verdict: `finish_step`'s compare against the caller's token
stands as it does for the greedy chain, and a miss - a row the landing cannot serve (an overflow,
more ties at the cut than the id sort's half) or an `exp` a unit apart between the two libraries -
reruns the step through the slow path. The LCG state rides a per-step buffer (`r.brng`) poked at
the pre-encode, which runs before the host draws for the step in flight, so both sides draw from one
state. `sampled_chain_ok` admits the sampler the landing's candidate path serves - a temperature, a
top-k within the landing's survivors and under the vocabulary, no penalty, no constraint - on a
session whose step in flight landed a selection; the penalized and the constrained keep the
unchained path. The single row's landing alternates between two slots (`GkClsSelArgs.row0`,
`g_msel_slot_next`), because the chained step's tail lands its row while the host still reads the
step in flight's, and `metal_sel_landing` answers the landed slot's planes.

### A sampled caller's step lands its selection beside its rows {#metal-selection-landing}

**A sampled stream's step runs the selection over its logits and the host sampler reads the survivors.** The
common round's ask (`sel_before`, `ARCHITECTURE_GPU_VULKAN_LANDING.md#logits-selection-landing`) reaches the
driver's seats (`install_rdec_select`: `metal_sel_ask` pokes the rows' sampler words into a persistent
host-visible block, `metal_sel_landing` answers pointers into the landing's four planes, `metal_sel_land_rows`
moves nothing). The classifier tail that follows - the single row's, the parked draft's, the verify's rows' -
dispatches `MetalClsSelect` over its logits buffer where the ask's row count is the tail's (`enc_sel_tail`), and
the step's landing records the rows it covered, so a step encoded with no ask reports none. Every Metal step
still lands its rows into the session on unified memory - the selection saves the host sampler's passes over a
248k-float row, not a copy - which is why the late landing is a no-op and the planes outlive the step. The
seats install at the driver's first step, so the home in use holds them where both modules are built.

**A bare-argmax caller's step lands its pick in the logits' place.** Under the caller's pick ask
(`Session.pick_asked`) the step runs the chain's argmax over its own logits (`pick_arm`, `bpick`) and the
landing publishes the id through `land_pick` instead of copying the row; the verify seat lands every row's
pick the same way (`enc_argmax_rows` into `bvarg`) and copies no row under the round's ask. A step encoded
under the ask for a caller that no longer asks lands its row, and a mispredicted chained step's pick goes
with the step. A zero-copy step's logits still reach the session's row: on unified memory the saving is the
host's pass over the row, not a copy.
