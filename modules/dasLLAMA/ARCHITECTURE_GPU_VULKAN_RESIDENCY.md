# dasLLAMA Architecture - the Vulkan tier's model residency

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This
document carries sections 2.2n-2.2o and 2.2an: the residency plan that sizes a whole model
before a byte uploads, the marks swap that lets one GPU slot serve many models, and the token
command's logits landing on the transfer queue. The N-row token command a batched step's rows go
through, and the residual step's two forms it holds bit for bit, are `ARCHITECTURE_GPU_VULKAN_NROW.md`
sections 2.2ao and 2.2ap. The prefill chain and byte
stores that run once a model is resident are `ARCHITECTURE_GPU_VULKAN.md` sections 2.2j, 2.2p,
2.2ab, 2.2ac and 2.2ad, and the cooperative-matrix GEMM tiles under them are
`ARCHITECTURE_GPU_VULKAN_GEMM.md` sections 2.2k-2.2m, 2.2q and 2.2ae; the per-op tier's decode era is
`ARCHITECTURE_GPU_VULKAN_DECODE.md` sections 2.2r-2.2v. The GPU backend role table these
sections build on stays in `ARCHITECTURE_GPU.md#gpu-backends`.

### The residency plan sizes a whole model before a byte uploads {#resident-plan}

The resident driver is all-or-nothing, so the plan IS the decision, and it is computed from
`Model` metadata alone. It sizes four numbers against the tier's weight budget: the dense weight
planes, the KV mirror at `seq_cap`, the driver's own device scratch, and the headroom the auto
arm leaves unfilled (zero when the user pins VRAM). The weight planes are ONE list
(`resident_planes`, a row length, a row count and a format per plane): the plan sums that list's
bytes and the upload reserves and places the same list, so the two cannot drift; the router rows,
the beta / alpha rows (f16 on the device) and the optional f32 embedding are the terms beside it. KV is reserved BEFORE weights and never
grows: on a discrete card the two compete directly, and evicting weights to grow KV would mean
re-uploading gigabytes. A decline carries a reason, and where the numbers allow one it carries
the remedy that works - a shorter context, because the weights are fixed and the KV is not. The
plan takes that remedy itself once: a mirror that does not fit is re-planned at seven eighths of
the room that is left after the weights, as long as that context clears the arming floor - the
built-in 4096 positions, `DASLLAMA_GPU_MIN_CTX` where set, and the caller's own context pin
(`set_gpu_ctx_max`, `DASLLAMA_GPU_CTX_MAX`) where that sits under either, since a caller that
pinned its context named the shape it serves (`resident_arm_floor`). The pin is what lets a
four-stream bench row home a 12B at 648 positions a region on a 16 GB card, where the binding
cap's 6238 a region asks for more K/V than the weights leave.

**The mirror's context is capped by the device's single-binding range before any byte is
counted.** Each K/V side binds as one SSBO range, so `seq_cap` is at most `maxStorageBufferRange`
over the side's bytes per position summed across the layers that hold rows of their own - a
shared-KV layer reads its donor's rows and adds none (`resident_binding_ctx`, over the mirror's own
row sum `resident_kv_row_bytes`). The sum is
what makes a hybrid right: a recurrent layer's KV width is zero, so the stride counts the
attention layers alone; read from layer 0 instead - recurrent on Qwen3.5 - the stride is zero,
the cap is skipped, the plan sizes a mirror the device prepare refuses, and the whole driver
declines to the per-op rails with the dense FFN on the CPU. The prepare keeps its own guard for
direct callers.

**The mirror is `regions` consecutive copies of the per-layer layout, and a region is one
session's whole history.** A host asks for the count before the load (`set_gpu_resident_regions`, the facade's verb,
one by default; a server's stream count), the plan sizes every region's K/V at `seq_cap`, and
the binding cap divides by the count, since both sides stay one buffer and one binding. A region
is an element offset (`rd_mir_base`: the layer's base plus the selected region's stride), folded
on the host into the `layerbase` push constant of every kernel that stores or reads a mirror row
and into the sync, readback and hydrate offsets, so no kernel knows regions exist. The push
constant is baked as the token command records, so the driver records one command per region
(and one unsplit twin) and `vk_rdec_select_region` names the region the next call resolves
against; the window chain encodes per call and reads the same accessor. The resident module owns
who sits where (`RdecRegion`: rows, the owning claim's generation, a last-bind tick): a session
binds its own region while its claim stands, else the least recently used one, which its next
claim takes. Two sessions in two regions step between each other, or in one batched step, with no
K/V crossing the bus - `tests/test_gpu_resident_regions.das` holds each bit for bit to the
session alone. A session past the region count takes another's region and brings its history up
from the host, as every session did on the single mirror. An owner whose region another session
took is served from the host or not at all: where its rows never came down, its next prefill or
decode panics rather than read another session's history. A batched step over a
per-layer-embedding model gives each row its own side input (`rdec_batch_ple_row`): the table
row gathered on device where the driver holds the projection, else that row of the pre-step
`eval_batch` already ran.

**A device-home session's region is its only copy.** `create_device_session` makes a session
with scratch and no host cache (`Session.device_kv`), so nothing is allocated per request and
nothing crosses the bus per token: the overrides bring no history up for it, read no row back
after a batched step and hydrate nothing down. The CPU rails cannot take its call, so a pass
that reaches one panics with the pass's reason (`rdec_device_home_guard`) - the prefill pin
is the one gate it walks past, because the pin guards host-cached sessions whose device-only
rows another session's claim would strand. Its region is pinned while it lives
(`RdecRegion.pinned`): no other session is handed it, and a host-cached outsider that finds
every region pinned passes as `busy`. A finished session parks (`gpu_device_kv_park`): the
pin drops, and the claim it answers keeps naming the rows until another session takes the
region; `gpu_device_kv_adopt` hands the first `npos` of them to a fresh session under a claim
of its own, the next prefill rewriting whatever lay past. `gpu_device_sessions` answers how
many a host may hold - the region count - and `gpu_device_prefill_continues` whether a
session's prefill may start past zero: not on a recurrent model, whose window chain starts the
deltanet state from zero.

**The scheduler's device mode is that contract over streams.** `set_device_kv` switches an
idle `Scheduler` (one batched step takes one kind of session): admitted streams are
device-home, their prefill quantum is at least the device's window (`gpu_device_prefill_window`),
a media request is refused at `submit` (a device-home session serves no media span),
and a reaped stream parks its claim under the token list its rows hold - one parked claim a
stream slot - so the next request adopts the longest opening it shares, short of its last
token: a conversation's next turn prefills its new suffix alone, with no copy. A recurrent
model's stream parks nothing: its prompt prefills whole in one quantum, from zero, and adopts nothing. `dasllama-server` turns the mode on
per slot before each step (`sync_slot_device_kv`) while the slot is served whole from the device,
has a region per stream and no media tower or self-speculation, and sizes the slot's context
to a region's. `tests/_resident_regions.das` holds the sessions, the pin, the park and the
mode against host-cached twins on the same device, on a qwen2 and on the E-series carrier.

**The auto arm sizes against the room the OS reports on the adapter, where the OS reports
it.** Vulkan cannot see the desktop: `VK_EXT_memory_budget` reads a flat 16024 MB on the 16 GB
reference card with 3 GiB of another process's memory resident, and so does the process's own
WDDM budget (`D3DKMTQueryVideoMemoryInfo`) - on the NVIDIA driver that number is the card minus
the OS reserve, whatever anyone else holds. What does carry the desktop is the adapter's
system-wide dedicated usage, the "GPU Adapter Memory / Dedicated Usage" performance counter
summed over the adapter's segments. The boost helper `os_video_memory` returns all three -
the ceiling, this process's usage, the adapter's usage - for the physical device's LUID, and
the tier installs it as the resident plan's `moe_gpu_os_memory` source. The plan's usable
bytes are then the tier's cap, or the ceiling minus what every other process holds minus a
1 GiB reserve, whichever is smaller; the headroom share is zero. The reserve is not slack: the
process's dedicated footprint runs about 0.7 GB past the plan's own tally (arena slab tails,
descriptor pools, pipelines, the transfer queue's staging - a 14.0 GB plan held 14.66 GB), and
a decline names what the other processes hold. Measured untagged on that card with a 0.63 GB desktop: plans of 13.35, 14.0 and 14.6 GB
(the card at 14.7, 15.3 and 15.9 GB dedicated) all decoded the 27B at full rate; with 2 GiB
more held by another process the same 13.35 GB plan lost 1.7 GB to system memory and decoded
at 4.35 tok/s, and with 4 GiB more the device returned all-zero logits with no error. The
manager never demotes the idle other process; ours is the victim, so the room left is the only
honest input. A pinned `VRAM_MB` takes no headroom and reads no room: the number is the user's
contract, and the plan fills it. Every rate in this section is a `benchmarks/lcpp_bench.das`
tg128 reading (`-jit --for-debug-purposes -r 5 -p 512 -n 128`, `DASLLAMA_IMAGE=0`,
`DASLLAMA_GPU=1`, 16 threads) on the zen2 box's RTX 5060 Ti 16 GB, driver 616.56.

**Where no OS answers, the auto arm's headroom is the larger of 2 GiB and 27% of the tier's
cap on Windows, and the 2 GiB floor alone elsewhere (`plan_headroom`).** On Windows that share
keeps a 16 GB card's plan near 10.7 GB and leaves an 8 GB card's plan where the fixed term
already put it; it declines every 27B file on a 16 GB card, and the ladder behind it (the 9B
hybrid at 6.8, 3.4 and 49.8 tok/s across 12.5, 12 and 11 GB plans) was taken with every
allocation tagged priority 1.0, the arm that made the planes pageable. The OS query is the
Windows memory manager's, so on Linux no OS ever answers; its Mesa drivers fill `heapBudget`
from the kernel's system-wide accounting and the NVIDIA driver fails an allocation past the
card out loud, so there is no demotion knee for the share to guard, and the share alone would
decline the 35B-A3B hybrid on a 16 GB card whose floor arms it at a reduced context. A decline
taken with no OS answer names the headroom it stood on and `DASLLAMA_GPU_VRAM_MB` as the pin
that reads no room.

**Two guards stand whatever the plan decided.** The resident decode panics on an all-zero
logits row (the first four tokens and every 256th are scanned) naming the over-commit, and at
its eighth token it reads the demotion tell and measures the token's wall against the weights
streamed at 40 GB/s, the PCIe class; either past its line logs the remedy once. Every resident
prefill reads the tell the same way. The tell is the adapter's shared usage - system memory the
GPU maps - grown past the desktop's baseline at tier init plus every live host-visible buffer
of ours (`make_host_buf`'s tally) by more than 512 MB: a demoted plane lands exactly there
(the 27B beside a 2 GiB hog read 3.07 GB shared against 0.67 GB of its own host buffers), while
the dedicated-usage counters can sit a few MB over the soft ceiling with nothing demoted.

**Device allocations carry no memory-priority tag.** The tier enables
`VK_EXT_memory_priority` and `VK_EXT_pageable_device_local_memory` on the device (the boost
creator does, whenever the card reports them) but chains no priority into an allocation
unless `DASLLAMA_VK_MEMPRIO=1` asks. On the NVIDIA WDDM driver the priority-1.0 tag is what
makes an allocation pageable: the 27B UD-IQ4_XS at a 13.35 GB plan on the 16 GB card held
12.26 GB dedicated with 1.7 GB demoted and decoded at 4.2 tok/s tagged, and held 14.66 GB
dedicated at 23.5 tok/s untagged, same run shape, same desktop (llama.cpp, which tags only
behind its own opt-in, sat at 13.7 GB dedicated and 24.1). The tag is an A/B arm.

An OPTIONAL plane rides only the room left under the budget at THIS context - what remains of
`budget_bytes - headroom_bytes` after weights, KV and scratch; the reserved headroom itself
stays unfilled. It never shrinks any of the three, and it reports zero bytes when it does not
fit - so the same model plans the plane in at a short context and out at a long one. Two planes are
optional: the raw f32 embed table, and after it the gemma-4 E-series' per-layer-embedding token
table (`ResidentPlan.ple_tbl_bytes`, a q8 plane [vocab x layers*ple]) - placed first, into the empty
arena, in pieces of 65536 rows the host gather's 2 GB array admits, which must land back to back in
one slab because the window chain's gather (`EmbGather` at the row width layers*ple, scaled by
sqrt(ple)) reads them off one block base. With it placed the ids prefill serves a per-layer-embedding
model too - the embed and the side rows gathered on device, no host row upload - and the token
command keeps its one-row host gather.

**A MoE is planned like a dense model with bigger FFN planes.** Its weight planes are the
attention quads, every MoE layer's expert triple (`[ne x nfe x dim]` twice and `[ne x dim x
nfe]`, sliced per expert as the per-op walk gathers them), its shared expert's triple where it
has one - in the file's K-quant format where the loader kept those planes beside the q8
transcode the CPU chain reads (`wshk*_offs`; it keeps them on a load with a GPU tier armed, so
a CPU-only load carries the transcode alone), the transcode otherwise - the classifier, and the
router plane - every MoE layer's f32 rows with a gated shared expert's gate row beside them. The scratch adds the window's routed planes: the gathered f16
rows, the gate and up rows, the f16 hidden rows and the routed down rows over `PF_WINDOW x k`
bucket rows plus `TILE_READ_SLACK` rows of tile slack (128 - the m column's unclamped partial
load, `dasllama/dasllama_gpu_tier.das`), and the routing smalls. The dense planes size at the shared
expert's width, or the expert width where no layer has one. The plan is all-or-nothing as ever:
a MoE whose stacks do not fit takes the per-op rails, which stream what the card cannot hold.
The plan is sized BEFORE the per-op reserves (the streamed slot, the decode mirrors), and a
fitting plan forgoes them - they would only shrink its room - so a decline past the plan (a
placement, a class rail) leaves the per-op rails without a streamed slot, said out loud. The
tile family the routed block rides is the f16-fed cm2 tiles (`ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block`), and `DASLLAMA_GPU_RESIDENT=0` keeps the per-op rails for any model, the A/B lever.
The driver is attempted only when asked for (`gpu_resident_requested`): the measured-best set
asks (`DASLLAMA_GPU=1`, or `auto_tier` on the want), and so does a want that spells its rails
out one by one and sets `resident` - the server's serving shape, where `gpu_dn = false` must
still turn one rail off. Rails alone, by env or by want, keep the per-op tier.

### One GPU slot, many models: the marks swap {#gpu-slot-marks}

A multi-model host runs one device tier under several loaded models, and the tier's per-model
state is offset-keyed - two models' marks installed together route one model's dispatches at
the other's planes. `GpuModelMarks` is that state WHOLE: the loader-contract marks plus every
resident-driver per-model global (the activation, the mirror count, the mirror cap, the mirror
codec, the device-embed arm, and the per-layer-embedding projection arm). The save moves the installed state out and leaves the globals
reading as no-model; the restore is its exact inverse. The whole-model drop clears the same set
and deselects the `"vulkan"` overrides, so a dropped model's prefill and decode take the plain
CPU path and a later re-arm passes `resident_upload`'s no-active-override gate. The three carry
the same set, which is why a model's device state never survives into the next. The upload
rail enforces it from its own side: a load that finds marks still installed - a model deleted
without the drop, the shape every test process and single-model tool takes - drops that
model's device state before uploading its own, carrying the load's MoE layer request across
the drop (the one mark the drop's reset would otherwise zero before the rail reads it).
Without that drop the second model's stacks
land beside the first's, and the offset-keyed stack lookup serves whichever model's plane
registered that offset first: the decode attention block asserts on the geometry change, and a
model whose geometry matches decodes the earlier model's weights.

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
reads (a thread's first element seeds its candidate, so an all-equal or non-finite row lands id
0, and only an empty slice lands `0xFFFFFFFF`, which the landing refuses as an engine bug), into
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
`Session.pick_asked` (the tests read the rows). A stream's first token samples off its prefill's
logits inline, so a request of n tokens lands n - 1 picks. On the pod the host side of a four-row
step held the logits copy (252-266 us of 4 MB on the E-series) and four pool argmaxes; the pick
leaves a 16-byte landing, and its planes take 4 x (2 x 64 + 1) bytes a row of the plan. The figures in this section and the next are
the pod's (RTX PRO 4500, `-jit`, cm2): the `DASLLAMA_GPU_PROF=1` token profile of
`benchmarks/lcpp_bench.das` for the step times and rates, `harness/vk_dma_probe.das` for the copy
rates; `PERF_LEDGER.md`'s 2026-09-19 section is the record.

### A call that passes to the CPU rails hydrates first {#resident-pass-hydrate}

Every pass of a host-cached session's call to the CPU rails - whatever the reason - brings the
region's device-only rows down to the host cache first (`rdec_pass_hydrated`), so the CPU
computes over the rows the mirror holds: an image turn's media rows attend the head the device
prefilled, and a decode past the region's rows reads them beside the CPU-served ones. A
device-home session has no host cache to fill, so its pass panics with the reason. The pass
still answers false, and the CPU rails serve the call.
