# dasLLAMA Architecture - the Vulkan tier's logits landing

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This document carries the
token command's logits landing on the transfer queue - the compute-timeline signal the copy waits on and
the carry row every resident command lands beside. What a model has to fit on the card before the driver
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
command's next write of the plane never races its read. `DASLLAMA_VK_XFERQ=0` leaves the device
without a transfer family; there the command carries the copy and the fence as before
(`rd_submit_land` / `rd_wait_land` choose) - the one in-process switch that puts the copy back
inside the command. Pod, Llama-3.2-1B Q8_0 with the profiler on: the four-row step 3761 -> 3371
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
logits, and a step whose every row asks for its pick takes the picks-only twin (`RDec.xlog_pick_cmd`),
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
