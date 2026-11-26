# dasLLAMA Architecture - the Vulkan resident driver's attention

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This document carries the
attention-side planes the resident driver uploads beside its norms - the q/k/v projection bias, gpt-oss's sink logits
and its output bias - with the flash tiles' sink stamps, and the far row a one-row step takes on the decode tile form
(`#decode-far-row`). The token command's attention key split
(`ARCHITECTURE_GPU_SHARED_ATTN.md#vk-decode-attn-split`) and the K/V mirror's block codecs, q8_0 and tq4
(`ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs`), with tq4's attention in the rotated basis
(`ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-tq4-basis`), are in `ARCHITECTURE_GPU_SHARED_ATTN.md`. The segmented window and
the short window's peel are in `ARCHITECTURE_GPU_VULKAN_WINDOW.md`. The window chain the flash
tiles serve is `ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain`; the token command the decode pass
sits in is `ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command`.

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
upload wherever no flash arm serves the prefill's f16 rows (an f16 mirror's, or a block codec's
shadow), before any weight lands.

**The sink plane is the bias plane's twin.** `vk_rdec_upload_sinks` uploads every layer's
`[n_heads]` sink row once; its seat installs separately (`install_moe_gpu_resident_sinks`) so a tier
without it names the sinks in its decline, and the upload itself declines - naming the reason -
where no flash arm serves those f16 rows or the head is not 64 wide, the sink stamps' one size,
pre-flighting the sink stamps the way the prepare pre-flights the plain ones. The token command's
sink twins (`ARCHITECTURE_GPU_SHARED_ATTN.md#vk-decode-attn-split`) and the flash tiles' sink stamps read the plane at the layer's row
(`sinkoff`).

**An attention output bias (gpt-oss) rides the norms plane, and the residual step adds it.** The
plane carries `RDEC_NORM_ROWS` rows a layer; the last (`RD_NORM_OBIAS`) is the layer's output
bias, a zero row on every other model, so no binding changes: the residual step's `abias_on` /
`aboff` (`ArArgs`, `GemvArgs`) add the row to the add partner - the `wo` GEMV's row - before the
add and before any post-attention norm, in the plain `cls_ar`, the fused requant twins and the
`wo` GEMV's epilogue alike, the CPU chain's order.

### The far row: one row's flat layers on the decode tile form {#decode-far-row}

The token command's attention has two forms. The row form (`da_attn_*`) walks a row's keys a group of query heads at a
time, in the device's pieces, so where a K/V head serves eight query heads its K and V rows are read once a group. The
tile form (the `fa_khr_dec_*` stamps) scores every query head of a K/V head as one tile over a piece of the keys, each
K and V row read once, and a combine dispatch of the row form's stamp sums the pieces (`comb`, a workgroup a head: a
thread a piece shares the piece's weight, so a thread's walk is one load a piece - `combine_pieces`). Both homes take
it over an f16 mirror on a layer with no softcap, a same-slab verify's rows and the far row alike.

A launch of the tile form is a workgroup a (K/V head, tile of sixteen (row, query head) pairs, key piece).
`fa_dec_groups` counts a piece's workgroups, and `fa_dec_pieces` takes the pieces that bring the launch to a home's
budget of workgroups - its wave or SM count under the home's own factor - one piece at least, the home's cap at most,
and never past the 64-key steps of a sliding layer's window. Both homes' decode drivers and the block drafter's dense
layers dispatch off the two; a home's policy is the budget and the cap it passes.

On the CUDA home a verify takes the tile form from four rows on a flat layer and from six on a windowed one
(`crd_tile_stamps`), at a head of 512 or 256 (`fa_khr_dec_h512_cls`, `fa_khr_dec_h256_cls`). A one-row step takes it
on a flat layer from the far count (`crd_far_row`), and there the heads of 128 and 64 have their stamps too
(`fa_khr_dec_h128_cls`, `fa_khr_dec_h64_cls`), which serve the far row alone; a sink model's far row combines on the
sink stamp. The far count is the device's - 128 keys from 64 SMs up, 2048 under, unless `DASLLAMA_CUDA_ATTN_TILE_FAR`
or a test's `set_cuda_attn_tile_far_pin` names one - and the pieces are a tier of the keys an SM serves (`CrdFar`,
`crd_tile_pieces`): a block every second SM under 32 keys an SM, a block an SM from there, two from 384 (64 under a
head narrower than 256), 128 pieces at most. The tier is a field of the command's graph key (`crd_graph_key`), so the
greedy chain launches the step's own graph on either side of a tier's edge; the witness is `cud_rdec_far_tokens`.

On the Vulkan home the tile form has the 512 and 256 stamps alone, wants KHR cooperative matrices and serves no sink
model (`rd_attn_tile_stamps`). A verify takes it past the unsplit position at any row count - the home has no floor
of rows - where `DASLLAMA_VK_ATTN_TILE` asks, or, unset, on a device whose prefill flash is the KHR arm
(`rd_attn_tile_ok`). The far row is the wide twin's record on a flat layer (`RdForm.wide`, `RD_WIDE_POS`;
`rd_attn_tile_far_ok`, `DASLLAMA_VK_ATTN_TILE_FAR`), two waves of workgroups, 64 pieces at most (`rd_tile_pieces`).

On the CUDA home of an RTX PRO 6000 Blackwell the tile form leads the row form on the Qwen3.6-35B's one-row step from
1024 keys on, and by more the longer the context (`PERF_LEDGER.md`, the entry on the far row). The same entry holds the
far row's readings on the RTX PRO 4500's two homes and on the Radeon 8060S, where the tile form leads too.
