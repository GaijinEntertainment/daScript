# dasLLAMA Vulkan Tier Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_VULKAN.md` and `ARCHITECTURE_GPU_VULKAN_DECODE.md`. Planned work:
`followup_vulkan.md`.

**Routed from `REVIEW_GPU.md`: a diff touching the Vulkan tier - `dasllama/dasllama_vulkan_*.das`,
`dasllama/dasllama_gpu_resident.das`, a `[vk_dispatch]` class, a `[spirv_decode]` callback, or a
cm2 tile class (an NV_cooperative_matrix2 GEMM class stamped per weight format and column) -
wherever the diff puts it - applies this list together with `REVIEW_GPU.md`'s and
`REVIEW.md`'s.**

**A hand-written Vulkan pipeline build anywhere in the engine is a defect - a Vulkan pipeline
is created only by a `[vk_dispatch]`-generated `ensure_*` and torn down by
`vk_drop_model_state`.**

**Never size a buffer bound as one SSBO (shader storage buffer) range above
`vk_max_storage_range()` - check the size where it is NEGOTIATED, not where it binds.** The
bind site cannot shrink a buffer that was sized wrong.

**Never cache a descriptor set across dispatches in state that `vk_drop_model_state` does not
clear** - put it in a `*_ready` latch, or in a holder that function already clears in
`dasllama/dasllama_vulkan_common.das`.

**Never read a `[spirv_decode]` callback's quant bytes by indexing `unpack8` of a 32-bit word
with a runtime value - read them as 16-bit lanes instead: an `int16[N]` block member selected
with `unpack8(w)[i & 1u]`, sub-fields pulled out by shift and mask.** The vendor driver's shader
compiler pattern-matches only the 16-bit spelling into its block-load path, and a runtime byte
select drops the whole kernel off it.

**A diff that changes when the resident prefill that takes token ids rather than embeddings
accepts a call updates the engine's GPU-embed probe in the same change.** That prefill is
`vk_rdec_prefill_ids` and the resident prefill override that routes to it; the probe is
`vulkan_embed_gpu_gate` in `dasllama/dasllama_gpu_resident.das`, registered through
`register_embed_gpu_gate`. The engine skips the CPU embed on a true probe, so a probe that is
true where that path declines hands the next consumer an unfilled residual stream.

**A diff that adds a cm2 tile format instance, changes a format's cm2 decode body, or changes a
format's four-wide twin (`decode_v4`) or its `DECV4` or `DECVEC` constant puts that format's
`cm2:<fmt>` probe rows (`harness/vk_gemm_probe.das`), both the `DASLLAMA_VK_DECVEC=1` and the
`=0` rows, in the PR body.** A cm2 tile is the NV_cooperative_matrix2 GEMM class stamped per
(weight format, column) pair in `dasllama/dasllama_vulkan_classes.das`.

**A cm2 tile format instance whose `DASLLAMA_VK_DECVEC=1` probe row is slower than its `=0` row
carries, in the same change, either a four-wide twin that wins - a hand-written `decode_v4`
under `override DECV4 = true` on that format's class (`dasllama/dasllama_vulkan_classes.das`) -
or the scalar callback: `override DECV4 = false` and `override DECVEC = false` together.** With
`DECV4 = true` the class never reads `DECVEC`, so `override DECVEC = false` alone leaves the
hand-written twin running.
