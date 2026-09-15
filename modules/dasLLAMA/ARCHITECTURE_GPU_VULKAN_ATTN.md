# dasLLAMA Architecture - the Vulkan resident driver's attention

Companion to `ARCHITECTURE_GPU_VULKAN.md`; section numbers are `ARCHITECTURE.md`'s. This document
carries sections 2.2al and 2.2am: the token command's attention key split, and the attention-side
planes the resident driver uploads beside its norms - the q/k/v projection bias, gpt-oss's sink
logits and its output bias - with the flash tiles' sink stamps. The window chain the flash tiles
serve is `ARCHITECTURE_GPU_VULKAN.md` sec.2.2j; the token command the decode pass sits in is
`ARCHITECTURE_GPU_VULKAN_DECODE.md` sec.2.2v.

### 2.2al The token command's attention splits a head's keys across workgroups {#vk-decode-attn-split}

**The decode attention dispatches a workgroup per (head, key split) and a combine per head.** One
workgroup a head leaves a low-head model's attention on a few SMs (four heads of eighty-two), so the
decode pass (`DaAttnT`) cuts the attended span into `nsplit` 32-aligned pieces (`da_nsplit`: enough
workgroups to cover the SM count twice, at most `DA_NSPLIT_MAX`, one where the count is unknown or the
heads alone cover it), each running the online softmax over its piece into an unnormalized partial
(max, denominator, accumulators; an empty piece's weighs nothing); `DaAttnComb` aligns a head's
partials by their maxes, normalizes, gates and stores the row (unsplit, the pass stores it), and the
store quantizes the row for the `wo` plane (`rqk`: Q8_0 blocks by the 32-lane group's amax, Q8_K
superblocks by the workgroup's on a head of 256 or 512), so no requant dispatch follows. A split
device records the token command twice - the split chain and an unsplit twin over the same sets,
so the once-per-epoch record costs double there and the profiler keeps each form's stamp names and
restarts its averages when a run crosses between them - and submits the twin while the position is
under `RD_UNSPLIT_POS` (512): there a head's whole row
is at most two of the pass's 256-key chunks, less than the combine's own chain, so the split only
adds a dispatch a layer (gpt-oss-20b on the RTX PRO 4500 at three splits, the `DASLLAMA_GPU_PROF=1`
token profile of `benchmarks/lcpp_bench.das` under `-jit` in cm2 mode with `DASLLAMA_ALLOW_UNTUNED=1`:
the combine 147 us a token, the pass no shorter for the split). The scores
go a subgroup two keys a step, lanes across the dims (one coalesced K row, the dot a subgroup add; on
the f16 mirror a lane's eight halves are one 16-byte word, `KV16`, and both keys' words are in flight
before either dot - a piece holds a few keys a subgroup, so the pass is the memory round trips it
chains); the V pass keeps a thread a dim, eight keys' loads issued before their adds (four a key pair
on a 512 head) for the same reason - on the RTX PRO 4500 the two together read gemma-3-1b's attention
384 -> 178 us a token at 128 tokens. The flash tile's cm2 arm (`FaT` at `KHR = false`) runs one
workgroup a (head, 64-row q tile) unsplit, its KHR arm one a (head, 16-row q tile): a key split there
costs more in partial stores and a combine than the shorter key loop returns on every gemma shape
measured (`followup_vulkan.md` item 50). A model that softcaps its attention logits (gemma-2) takes
the tile's `CAP` leaves at head size 256: every scaled score through `cap * tanh(s / cap)` before the
mask, the 8-row tile serving any other capped shape. Every kernel that stages a head does it in a
256-thread workgroup, 32 dims to a lane and a second element a thread past 256, so the driver serves
head sizes that are 32-multiples up to 512 and `resident_layer_decline` names any other by layer.

**A sink model (gpt-oss) runs the pass's `SINK` twins.** The head's sink logit is a phantom key with
no value row, read off the sink plane (sec.2.2am) bound past the pass's eight bindings: the unsplit
store folds it into the row's max and denominator once, a split piece carries none, and the combine's
sink twin seeds its aligned max and denominator with it.

### 2.2am The attention-side planes: the q/k/v bias, the sink logits and the output bias {#vk-attn-planes}

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
(`ARCHITECTURE_GPU_VULKAN_DECODE.md` sec.2.2r), and the prefill chain's `AtPrep` stage adds the q
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
sink twins (sec.2.2al) and the flash tiles' sink stamps read the plane at the layer's row
(`sinkoff`).

**An attention output bias (gpt-oss) rides the norms plane, and the residual step adds it.** The
plane carries `RDEC_NORM_ROWS` rows a layer; the last (`RD_NORM_OBIAS`) is the layer's output
bias, a zero row on every other model, so no binding changes: the residual step's `abias_on` /
`aboff` (`ArArgs`, `GemvArArgs`) add the row to the add partner - the `wo` GEMV's row - before the
add and before any post-attention norm, in the plain `cls_ar`, the fused requant twins and the
`wo` GEMV's epilogue alike, the CPU chain's order.
