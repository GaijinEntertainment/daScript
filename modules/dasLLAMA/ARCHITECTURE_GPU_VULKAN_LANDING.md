# dasLLAMA Architecture - the Vulkan tier's logits landing

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This document carries the
token command's logits landing on the transfer queue - the compute-timeline signal the copy waits on and
the carry row every resident command lands beside - the selection a sampled stream's step lands in place of its rows, and the greedy chain that submits the next one-row step behind it. What a model has to fit on the card before the driver
runs is in `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`; the N-row token command a batched step's rows go
through is in `ARCHITECTURE_GPU_VULKAN_NROW.md`; the NextN draft head and its rollback are in
`ARCHITECTURE_GPU_VULKAN_MTP.md`.

### The token command's logits leave on the transfer queue {#logits-transfer-queue}

**A token command signals the compute timeline, and its logits copy follows on the transfer
queue.** On a box behind an IOMMU the compute queue's `vkCmdCopyBuffer` into cached host memory
moves a page a microsecond - the RTX PRO 4500 pod reads 4.3 GB/s at every size from 512 KB to
32 MB (`harness/vk_dma_probe.das`), so a four-row step's 2 MB logits plane cost 480 us of a 3.5 ms
command - while the transfer family's copy engine moves the same plane at 19-27 GB/s (108 us
with the submit and the wait; the RTX 5060 Ti reads 5.7 against 9.8). So a device with the
transfer family armed (`RDec.log_xfer`) records the token command without the copy: the submit
signals `g_gpu.cmp_sem`, the compute -> transfer timeline (`submit_signal`), the transfer queue
takes a recorded copy command a row count (`rd_xlog`, `RDec.xlog_cmd`) that waits for that value
at the transfer stage and signals the transfer timeline (`xfer_submit_after`), and the host spins
on the transfer timeline's counter (`xfer_wait` spinning) instead of the fence - a blocking wait
would pay the OS wake-up a step, as the fence wait's spin already knows. The logits planes are
CONCURRENT between the two families (`make_device_buf` / `make_host_buf` at `xfer_shared`), so no
ownership transfer sits on the path; the host orders the next step behind the copy, so the
command's next write of the plane never races its read - or, on the greedy chain, the chained step's prologue waits for the
copy on the transfer timeline (`#vk-greedy-chain`). `DASLLAMA_VK_XFERQ=0` leaves the device
without a transfer family; there the command carries the copy and the fence as before
(`rd_submit_land` / `rd_wait_land` choose) - the one in-process switch that puts the copy back
inside the command. The compute timeline (`cmp_sem`) stands under either setting: every chained
submit signals it (`submit_signal`, the streamed expert bundles and the hit chains wait on it), so
the timelines are created wherever the device has them, and only the transfer family and its
queue follow the knob. Pod, Llama-3.2-1B Q8_0 with the profiler on: the four-row step 3761 -> 3371
us, tg128@4 1019 -> 1136 summed, tg128 426 -> 451.

**A row whose sampler is a bare argmax lands its pick alone.** After the epilogue the command runs
the pick's two passes over every row, every step - `ClsArgmaxPart`, a workgroup a (row, chunk) over
a slice of the row (`CLS_ARGMAX_CHUNKS`, 64, compiled into both passes: a 4096-wide slice of a 262144
vocab, so a row's pass is a wave of small workgroups and not one workgroup's walk over a megabyte;
9-12 us a step from a 128k to a 262k vocab), then `ClsArgmaxFin`, a workgroup a row over the
chunks' partials - the first maximum's id, the lowest on a tie, as the host's `parallel_argmax`
reads (every lane reads through `lane_val`, NaN and -inf as the lowest float, and a thread's
first element seeds its candidate, so an all-equal row lands id 0, a row with non-finite lanes its
widest finite lane and an all-NaN row id 0; only an empty slice lands `0xFFFFFFFF`, which the
landing refuses as an engine bug, and a landed pick that reads non-finite panics naming the row -
`rdec_landed_finite`), into
`RDec.pick_dev`. The ask decides only what lands: the transfer command copies the picks behind the
logits, and a step whose every row asks for its pick takes the picks-only twin (`RdXlog.picks`, the landing kind `rd_xlog` records by),
so the logits plane never leaves the device and the host copies nothing (a device without the
transfer family copies both inside the command and skips the host copy alone); the one-row command's
guard steps (`rd_guard_step`: the first four tokens and every 256th) land the logits too, so the
over-commit check reads a row on a picking stream at its cadence. The seams carry the ask -
`RdecTokenFn`'s `pick_only`, a null `lrows` pointer of `RdecTokenNFn` - and answer the id, which
the driver hands to the session (`rdec_land_pick`: `Session.pick_ready`, `pick_tok`) for the next
`sample_` to return; a row-at-a-time step the driver declines after some rows landed takes their
picks back (`rdec_unland_picks`), since the CPU rails redo every row's logits. The scheduler asks it
a step for every stream whose parameters are a bare argmax (`sampler_is_argmax`: temperature at or
under zero, penalties off - the served default) and clears the ask after the step's sample; a
temperature or a penalty lands the logits as before, and so does every caller that never sets
`Session.pick_asked` (the tests read the rows). The speculative round's commands land the same
way: its draft lands the pick alone wherever the walk compares ids, its verify the rows' picks
alone under the ask, and the round then publishes the committed row's pick through the same
`Session.pick_ready` / `pick_tok` pair (`land_pick`, which the driver's `rdec_land_pick` counts
through; `ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command`); `benchmarks/lcpp_bench.das`'s
`--mtp-ab` arms ask the same way under a greedy temperature, so the A/B measures the served shape.
A stream's first token samples off its
prefill's logits inline, so a request of n tokens lands n - 1 picks. On the pod the host side of a four-row
step held the logits copy (252-266 us of 4 MB on the E-series) and four pool argmaxes; the pick
leaves a 16-byte landing, and its planes take 4 x (2 x 64 + 1) bytes a row of the plan. The figures in this section and the next are
the pod's (RTX PRO 4500, `-jit`, cm2): the `DASLLAMA_GPU_PROF=1` token profile of
`benchmarks/lcpp_bench.das` for the step times and rates, `harness/vk_dma_probe.das` for the copy
rates; `PERF_LEDGER.md`'s 2026-09-19 section is the record.

**A NextN-headed model's steps land the speculative carry beside their logits.** The speculative
round drafts from `Session.mtp_h`, the post-final-norm hidden of the last evaluated row, and runs
cold unless `mtp_h_pos1` names the position it starts at (`ARCHITECTURE_GPU_MTP.md#mtp-round-one-join`);
the CPU tail stashes both after every decode and prefill, and the resident overrides return before
that tail. So a driver prepared with `carry` - set where the model has a NextN head and the tier
installed the landing (`install_rdec_carry`) - takes the row-storing form of the classifier feed's
stamp at the token command's final site (`storex`: the normed float rows stay in `xb_dev` beside
the Q8_0 blocks), and every step copies those rows home on the logits' path - in the transfer
command, the picks-only twin included, or inside the token command on a device without the
transfer family; the window chain's last window copies its last row's normed hidden out of the
prefill plane beside its logits. The overrides then write the row into `mtp_h` and move
`mtp_h_pos1` as the CPU tail does - a step's row to `pos + 1`, a prompt's last row to
`start_pos + npos`, each batched row into its own session (`rdec_stash_carry`). A device-home
session takes no carry: the CPU round has no host rows to draft or verify it against. The
carry costs `dim x 4` bytes a row over the bus (4 KB a token on Qwen3.5-0.8B) and the last
layer's fused down epilogue, which writes Q8_0 blocks and no float row, so that layer's residual
step runs as its own dispatch; a model without a head records the command it always did.

### A sampled stream's step lands its rows' selection in place of the rows {#logits-selection-landing}

**A row whose sampler draws lands its survivors, not its logits.** A sampled stream's sampler
reads a 248k-float row to draw one token, and every row of a speculative round's verify, every
draft a rejection-sampling round samples, and every plain step of a sampled stream paid that
row's trip and the host's passes over it. So a step whose stream wants the selection
(`sel_wanted`: a sampler pointed at that draws at a temperature above zero, no constraint, no pick
ask, and a penalty that only lowers logits - a repetition penalty at or above one, presence and
frequency penalties at or above zero, since a raised unlanded logit could outrank the survivors -
over a window the ask can list) asks the driver before
its command (`sel_before` in the common round, around every seat's and the plain step's command on either home -> `vk_rdec_sel_ask`), poking a row's sampler words - the
temperature's bits and the recent window's unique ids with the round's drafts - into a
host-visible block (`RDec.sel_words`, `SEL_WORDS` a row, as `TokMeta` is poked). An ask no
driver can serve - no home installed the seats, or the home has no selection pipeline
(`metal_sel_ask` answers false) - is declined, and the command lands its rows as it would
unasked: the landing read after it reports no rows. The submit then
runs the selection command behind the step's on the compute queue (`rd_sel_cmd`: a full barrier,
then `ClsSelect`, the Vulkan stamp of the template every home stamps (`GkClsSelect`, `ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-kernel-pairs`; `CuClsSelect` on the CUDA home, launched on the stream after the command's segments with its ask words and four planes in mapped host memory - `cud_rdec_sel_ask`, `cud_rdec_sel_landing` - and the rows the selection stood in for read late off the device's logits plane; `MetalClsSelect` on Metal), a workgroup of 1024 threads a row over the logits plane - a radix select over the
float's order-preserving key in three histogram passes to the exact 1024th largest logit (each
pass's bin counts are subgroup-aggregated: a row's logits cluster into a few bins, and a thread an
atomic serializes the workgroup on them), the
compaction of every logit at or above it with the ties, capped at 2048 with the overflow flagged,
the row's max and the softmax denominator at the row's temperature, and the logits of the listed
recent ids) and the transfer queue's landing twin (`RdXlog.sel`) copies the picks, the carry and
the selection's four planes into the packed host landing (`RDec.sel_host`) - the logits plane
never leaves the device. A guard step drops the ask, since the over-commit check reads a row. The
common glue hands the engine pointers into that landing (`sel_after` -> `g_sel_landing`, the session's
row in `Session.sel_row1`), and every sampler entry - the row draw, the typical and rejection
judges, the rejection drafter, the chain cut's probability - serves off it where the stream's cuts
confine the draw's support to the survivors (`landed_row_`: a top-k within the cap whose k-th
penalized logit stays at or above the landing's floor, or a nucleus reached inside the landing
and a min-p whose floor sits above the lowest survivor's probability), the committed token the
row path's token for token since the survivors hold the row's whole post-cut support and the
denominator is the row's within a float sum's rounding. A sampler the landing cannot serve - no
cut at all, a flat row whose nucleus reaches past the survivors, a top-k past the cap, an
overflowing row - lands the rows late (`sel_land_rows` -> `vk_rdec_sel_land_rows`: the transfer
queue copies the intact logits plane down) and takes the row path. Every step opens with the
landing cleared (`sel_reset`), so no session reads another step's. On the RTX 5060 Ti the
selection over three 248320-wide rows runs in about 170 us behind the submit's fixed cost against
1.8 ms a row for the host's passes (`debug-jit`, the selection entry in `PERF_LEDGER.md`); `set_select_landing` is the knob, on by default, and
`benchmarks/lcpp_bench.das --mtp-select=false` lands the rows for the A/B.

### The greedy chain submits the next one-row step behind the running one {#vk-greedy-chain}

**A greedy session's next step runs on the device's own pick, submitted before the running step lands.** Asked
(`vk_rdec_chain_ask`, which the resident decode makes for a session whose last draw was a bare argmax - `Session.sampled`
clear - with the next position's rope row), the one-row step submits its command and then, before waiting its landing, the
next step (`rd_chain_submit`): a prologue (`rd_chain_prologue`, a command a region and staging slot) that waits on the transfer
timeline for the running step's landing - the chained step rewrites the logits and pick planes that copy reads - and then,
fenced off both neighbours, copies the running step's pick into the gather's token id, the next position's staged TokMeta row
and rope row into the live ones, and on a recurrent model the region's state slots into the chain's snapshot; then the same
recorded one-row command (recorded for simultaneous use) and a landing copy into the slot's own host planes. The staging and
the landing alternate two slots, so the host stages the step after while the chained one still reads its rows. The next call
takes the chained step when it steps the same region at that position with the token the chain gathered and asks no landing
the chain did not make: it chains the step after before waiting, and lands off the slot.
The same step with another token is a miss; a chain dropped at a session switch or a
settle predicted nothing and is neither hit nor miss. Either way the step is waited out,
the region's state slots copied back from the snapshot - the ring images alternate by
parity, so the rerun reads
the image the chained step read and writes the one it wrote - and the call runs plain; the K/V row the chained step stored is
the row the rerun stores again. Every other driver entry - a prefill, the N-row and in-order rows commands, a draft, a verify,
a rollback, a hydrate or upload of rows, a checkpoint, a region switch - settles a chained step the same way first
(`rd_chain_settle`), as does the model drop. The chain needs the transfer-queue landing, the device gather of the token id,
and a command with no per-layer-embedding rows, carry, host experts or hyper-connection seats (`vk_rdec_chain_wanted`); it
never chains into a guard step or across the wide twin's first record. `DASLLAMA_VK_CHAIN` picks the mode - 0 off, 1 every
greedy step, -1 adaptive (a miss leaves the next 1, 2, 4 .. 64 steps unchained, a hit
resets the run, a chain dropped at a switch or a settle backs off nothing) - and
`vk_rdec_chain_stats` counts the steps taken and the misses.
The plan charges the snapshot - one region's recurrent states - where the chain is asked
(`rdec_chain_snap_bytes`).

The CUDA home runs the same chain on its one stream (`crd_chain_submit`), where stream order is every fence: the running step's
graph is launched and left running, the step's pick (the out block's first word) and, where it landed logits, its logits plane are
copied aside into a landing slot with an event after them, the prologue copies the pick into the in block's token id word and the
staged TokMeta block and rope row of the next position into the in block (and the region's recurrent state slots into the
snapshot), the next step's captured graph is launched behind, and an event marks it done. The host waits the landing event alone
and reads the running step off its slot; a chained step the next call takes is waited at its own event, its results off the out
block, or off the slot the chain after it copied them into. The chain needs the row and the side row gathered off the token id,
no live carry and no host experts (`cud_rdec_chain_wanted`), a step past the first and both steps' graphs captured (the chained
step launches a graph and captures none), one segment and no selection asked (`crd_chain_ready`); `DASLLAMA_CUDA_CHAIN` picks
the mode as the Vulkan knob does, `cud_rdec_chain_mode_set` pins it, and `cud_rdec_chain_stats` counts the hits and misses.
