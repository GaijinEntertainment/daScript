# dasLLAMA Architecture - the Vulkan resident driver's attention

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This document carries the
attention-side planes the resident driver uploads beside its norms - the q/k/v projection bias, gpt-oss's sink logits
and its output bias - with the flash tiles' sink stamps. The token command's attention key split
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
