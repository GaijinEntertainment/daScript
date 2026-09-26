# dasLLAMA Architecture - the Vulkan resident driver's attention

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This document
carries sections 2.2al and 2.2am: the token command's attention key split, and the attention-side
planes the resident driver uploads beside its norms - the q/k/v projection bias, gpt-oss's sink
logits and its output bias - with the flash tiles' sink stamps. The window chain the flash tiles
serve is `ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain`; the token command the decode pass sits in is
`ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command`.

### The token command's attention splits a head's keys across workgroups {#vk-decode-attn-split}

**The decode attention dispatches a workgroup per (kv head, slab of its q heads, key split), and
the group's last piece combines.** A workgroup reads its kv head's K and V rows once and scores
them against the slab's query heads of the GQA group that share them - `DA_G` (four), or two on a
group of one or two heads (`da_slab_is_g2`: the `g2` stamps of `DaAttnT`, whose template constant
`G` sizes every score and V loop, so a group of one or two heads - gemma-2, gemma-4's sliding
layers, any MHA carrier - scores no dead heads; `da_slab_heads` is the one expression the kernel's
`G` and the host's workgroup count derive from); a group wider than the slab takes several slabs,
a narrower one leaves dead heads whose q rows are zero and whose reductions and stores are skipped
(`da_attn_row_wgs` counts a row's workgroups, the same count on either slab). The token command
picks the slab a layer (`rd_enc_da_attn`); the per-op seam (`dasllama_vulkan_seams.das`) and the
attention-tower chain dispatch the four-head slab whatever the group. The pass is a chain
of latencies, not a stream of bytes: a workgroup a head walking two keys a step behind a subgroup
reduction each read Llama-3.2-1B's sixteen layers at 28 us a four-row step and 8.5 a one-row step on
the RTX PRO 4500, the same whatever the split, so the pass (`DaAttnT`) spends its threads on
independent work. The device figures in this section are read by `harness/vk_attn_probe.das` under
`-jit` in cm2 mode (device timestamps a layer) and by the `DASLLAMA_GPU_PROF=1` token profile of
`benchmarks/lcpp_bench.das`; `PERF_LEDGER.md`'s 2026-09-19 section is the record.

**A pass is one chain: the scores, the softmax, the V accumulate, the fold and the slab finish.** A
256-key chunk's scores take a thread a key: the K row's words load eight at a time (`KV16`, one
round trip a 128 dims), the slab's dots - one a head - read the q rows from shared memory, and no subgroup
reduction sits between the keys; the chunk's first V words load in the same round trip, since they
wait on nothing the scores compute. The softmax's max and sum a head are one subgroup reduction
each, the subgroups' values folded by every thread from shared memory (`sgp`, `ml`). The V
accumulate keeps a thread a (key group, eight dims) with a head's eight sums in registers, eight
rows' words in flight. The finish folds the key groups by subgroup shuffles, then across the
subgroups through the q slab (`qrow`, spent by then), a round of heads at a time. The driver serves
a 32-multiple head up to 256 and the 512 head; the resident gate declines 288..480, where the wide
arm addresses a second element at dim + 256. The shuffle fold across key groups runs where the
row's word count (`hs / 8`) is a power of two - 32, 64, 128, 256, 512; at 96, 160, 192 and 224 the
pass keeps every key group's partial in the slab, the finish sums them, and the threads past the
last whole key group hold no keys.

**The split ladder follows the attended span, and the last piece to land finishes.** The pass
cuts the attended span into `nsplit` 32-aligned pieces, each running the online softmax over its
piece into an unnormalized piece a head (max, denominator, accumulators; an empty piece weighs
nothing). The piece count is the span's: one piece under
`RD_UNSPLIT_POS` (512), `RD_SPLIT_PIECES` (four) to `RD_WIDE_POS` (3072), `RD_SPLIT_WIDE_PIECES`
(eight) past it for the one-row command alone, each capped by the partials plane's `attn_nsplit`
(`da_nsplit`: enough (head, split) workgroups to cover the SM count twice, at most
`DA_NSPLIT_MAX`); a sliding-window layer attends at most its window, so its count is its
window's span's under the command's; and a layer's count is capped so its (kv head, slab)
workgroups over the plan's rows (`RDec.nb`) fit one wave of the SMs (`rd_layer_pieces`: the
SM count over the row's workgroups times the rows, one at least) - the pass is a chain of
latencies a workgroup, and the ruler reads a second wave as a second chain: at 640 positions
and four rows gpt-oss's shape (64 heads, eight a kv head: sixteen workgroups a row) reads 16.4
us a layer unsplit and 34.9 at four pieces, Llama-1B's (eight a row) 14.5 and 22.6, the 30B's
(eight a row, head 128) 22.6 at one and 20.6 at two, where E4B's (two a row) wants its four
pieces - 22.7 against 34.4 unsplit - and every shape climbs past about thirty-two
workgroups a layer. The cap reads the plan's rows for both commands, so a one-row step on a
plan of four rows takes the four-row count and the two commands' rows sum in one order (the regions
files' bit-for-bit claim); the flat rows of a `--npl 4` bench run are that shape, and they held
through the cap (gpt-oss 209.1 -> 209.2, Llama-1B 459.2 -> 465.0, E4B 112.5 unchanged; `PERF_LEDGER.md`'s
2026-09-21 section). Where the cap reads one, the unsplit and wide twins record the same chain as
the split command (`unsplit_on` and `wide_on` read `attn_nsplit` alone), a duplicate the form switch
pays in recording, never in a step. The ruler (`harness/vk_attn_probe.das`
under `-jit` on the pod, `PERF_LEDGER.md`'s 2026-09-20 section) read the rule at the Qwen2.5-0.5B
geometry on the RTX PRO 4500, us a layer: one piece 10.4 at 384 positions and 14.5 at 640 against
four pieces' 16.5 at both, four pieces 16.5 at 1024 against one piece's 18.6 and 18.6 at 1536
against 26.7 (the edge sits between 640 and 1024; the qwen3 0.6B geometry's four pieces win from
640 at one row), four pieces 20.6 at 2048 and 27 at 4096 against sixteen's 33 and 34; past 4096
eight pieces pay for one row alone - 33 against four's 41 at 8192, 47 against 72 at 16384 - and
lose at four rows (51 against 44, 81 against 74), so the N-row command stays at four pieces at
every span past 512. The old rule, covering the card twice whatever the span, recorded sixteen
pieces for a one-row step and eleven for a four-row step and paid double. One count for every
row count also keeps the one-row and N-row forms summing in one order, so a batched row is the
row alone bit for bit on any SM count while every row of the batch sits in one band; the batch's
furthest row picks the form, and a row batched across a band edge takes the batch's. The
pieces land in the partials plane; past them sit the arrival counters (`partu`, a word a (row, kv
head, slab) at `cntoff`), each atomically bumped after a device-scope release. The piece that reads
its group's count last aligns each head's pieces by their maxes, normalizes, gates and stores the
row, and rearms the counter to zero (unsplit, the pass stores it), so no combine dispatch follows
and the split costs no second launch. The store quantizes the row for the `wo` plane (`rqk`: Q8_0
blocks by the 32-lane group's amax, Q8_K superblocks by the workgroup's on a head of 256 or 512), so
no requant dispatch follows either. A split device records the token command as a ladder of
forms over the same sets - the split chain and an unsplit twin at the epoch's record, the wide
twin on the first step at `RD_WIDE_POS` (a device whose `attn_nsplit` stays within the split
form's pieces has no wide twin) - and the profiler keeps each form's stamp names and restarts
its averages when a run crosses a band edge. Under `RD_UNSPLIT_POS` a head's whole row is at
most two of the pass's 256-key chunks, so the unsplit twin serves there.

**The flash tiles stay unsplit.** The flash tile's cm2 arm (`FaT` at `KHR = false`) runs one
workgroup a (head, 64-row q tile) unsplit, its KHR arm one a (head, 16-row q tile): a key split there
costs more in partial stores and a combine than the shorter key loop returns on every gemma shape
measured (`followup_vulkan.md` item 50). A model that softcaps its attention logits (gemma-2) takes
the tile's `CAP` leaves at head size 256: every scaled score through `cap * tanh(s / cap)` before the
mask, the 8-row tile serving any other capped shape. Every kernel that stages a head does it in a
256-thread workgroup, 32 dims to a lane and a second element a thread past 256;
`resident_layer_decline` names a layer outside the served head sizes.

**A sink model (gpt-oss) runs the pass's `SINK` twins.** The head's sink logit is a phantom key with
no value row, read off the sink plane (`ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-attn-planes`) bound past the pass's eight bindings: the unsplit
store folds it into the row's max and denominator once, a split piece carries none, and the last
piece's combine (the pass's own `SINK` stamp) seeds its aligned max and denominator with it.

### The attention-side planes: the q/k/v bias, the sink logits and the output bias {#vk-attn-planes}

**A q/k/v projection bias (qwen2) folds into the rope stage.** The biased models' bias rows
upload once as one row per layer in the projection buffer's own `[q | k | v]` layout
(`vk_rdec_upload_bias`, a recurrent layer's row zero) - every layer's rows one after another, each
at its own attention class's widths, so a layer's base is the sum of the widths below it
(`resident_bias_off`) - bound at the last binding of the three rope kernels - the decode rope+store,
the prefill's batched twin and the fused qk-norm+rope - which add the bias to each element as they
read it, before the rotation (or the norm) and before the v copy, so no dispatch is added: the CPU
chain adds the bias between the projection and the norm, and so does this. A model without a bias
binds the norms buffer in that slot as a placeholder the kernel never reads (`hasb` 0). The seat
installs separately (`install_moe_gpu_resident_bias`), so a tier without it names the bias in its
decline instead of serving the model unbiased. The per-op tier carries the same rows through its
hooks: the decode block binds the layer's row to the same rope kernels
(`ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block`), and the prefill chain's `AtPrep` stage adds the q
and k rows before its norm and rope and runs a third pass over the raw v window - `AtPrep` with no
rope and no norm is a copy plus bias, in place - so the attention and the v rows that come home both
carry it (`ARCHITECTURE_GPU_VULKAN_GEMM.md`, the per-op chain).

**A sink model's attention (gpt-oss) takes the flash tiles' `SINK` stamps, the 64-wide head's
alone, on either arm.** The head's sink logit is a phantom key with no value row: the stamp seeds
each query row's running max with it and its running sum with one, the phantom key's own mass, and
the pass runs unchanged - the final divide carries the sink's share, the CPU flash form's own seed.
The stamps sit in their own families (`fa_cm2_sink_cls`, `fa_khr_sink_cls`) for their fifth
binding, the sink plane. The chunked pair has no sink arm, so a sink model declines at the sink
upload wherever no flash arm serves the f16 mirrors, before any weight lands.

**The sink plane is the bias plane's twin.** `vk_rdec_upload_sinks` uploads every layer's
`[n_heads]` sink row once; its seat installs separately (`install_moe_gpu_resident_sinks`) so a tier
without it names the sinks in its decline, and the upload itself declines - naming the reason -
where no flash arm serves the f16 mirrors or the head is not 64 wide, the sink stamps' one size,
pre-flighting the sink stamps the way the prepare pre-flights the plain ones. The token command's
sink twins (`ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-decode-attn-split`) and the flash tiles' sink stamps read the plane at the layer's row
(`sinkoff`).

**An attention output bias (gpt-oss) rides the norms plane, and the residual step adds it.** The
plane carries `RDEC_NORM_ROWS` rows a layer; the last (`RD_NORM_OBIAS`) is the layer's output
bias, a zero row on every other model, so no binding changes: the residual step's `abias_on` /
`aboff` (`ArArgs`, `GemvArArgs`) add the row to the add partner - the `wo` GEMV's row - before the
add and before any post-attention norm, in the plain `cls_ar`, the fused requant twins and the
`wo` GEMV's epilogue alike, the CPU chain's order.
