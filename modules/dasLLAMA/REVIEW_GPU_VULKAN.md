# dasLLAMA Vulkan Tier Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_VULKAN.md` and `ARCHITECTURE_GPU_VULKAN_DECODE.md`. Planned work:
`followup_vulkan.md`.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

**A hand-written Vulkan pipeline build is a defect - a Vulkan pipeline is created only by a
`[vk_dispatch]`-generated `ensure_*`.**

**A diff that adds a Vulkan pipeline tears it down in `vk_drop_model_state`, in the same
change.**

**Never size a buffer bound as one SSBO (shader storage buffer) range above
`vk_max_storage_range()` - check the size at the site that computes it, not at the site that
binds it.** The bind site cannot shrink a buffer that was sized wrong.

**Never cache a descriptor set or a host address - a pointer into CPU memory - across
dispatches in state that `vk_drop_model_state` does not clear** - hold it in that function's
own state in `dasllama/dasllama_vulkan_common.das`, the way the `*_ready` latches are.

**Never read a `[spirv_decode]` callback's quant bytes by indexing `unpack8` of a 32-bit word
with a runtime value - read them as 16-bit lanes instead: load the lane `w` from an `int16[N]`
block member, select the byte with `unpack8(w)[i & 1u]`, and pull sub-fields out by shift and
mask.** The vendor driver's shader compiler pattern-matches only the 16-bit spelling into its
block-load path, and a runtime byte select loses that path for the whole kernel.

**A diff that changes when `vk_rdec_prefill_ids` - the resident prefill that takes token ids
rather than embeddings - accepts a call, or changes the override that routes to it
(`vulkan_resident_prefill`), updates `vulkan_embed_gpu_gate` in the same change** - the
override and the gate live in `dasllama/dasllama_gpu_resident.das`, `vk_rdec_prefill_ids` in
`dasllama/dasllama_vulkan_prefill.das`. The engine skips the CPU embed when that gate returns
true, so a gate true where the prefill path declines hands the next consumer an unfilled
residual stream.

**A prefill GEMM dispatched at a nonzero start row never asks `cm2_split_k` for a split - it
encodes unsplit.** The split-k reduce sums partial planes counted from row 0, so a dispatch
starting above row 0 would reduce the wrong rows.

**Never leave a K/V codec unserved by the kernels that read or write the whole-model driver's
`k_mirror`/`v_mirror` slabs, or the decode block's per-layer `DatLayer.k_mir`/`v_mir` pair - a
K/V codec is the mirror's element type, f16 or f32.** Two shapes serve both: instances of one
template cover both codecs, or a single-codec kernel has a sibling that serves the other codec
behind an arming gate that keys on `kv16`. The whole-model driver serves both codecs, so a
codec no kernel covers silently drops that codec's GPU path.

**A diff that adds a cm2 tile instance for a kq superblock format - one `kq_sb`
(`dasllama/dasllama_kqformat.das`) accepts - changes such a format's cm2 decode body, or
changes its four-wide twin (`decode_v4`) or its `DECV4` or `DECVEC` constant puts that
format's `cm2:<fmt>` probe rows (`harness/vk_gemm_probe.das`), both the
`DASLLAMA_VK_DECVEC=1` and the `=0` rows, in the PR body.** A cm2 tile is the
NV_cooperative_matrix2 GEMM class stamped per weight format and token-column width (the
class's `BN`) in `dasllama/dasllama_vulkan_classes.das`.

**A cm2 tile format instance whose `DASLLAMA_VK_DECVEC=1` probe row is slower than its `=0` row
ships one of two fixes in the same change: a hand-written `decode_v4` under
`override DECV4 = true` on that format's class (`dasllama/dasllama_vulkan_classes.das`),
re-measured so its `=1` row now beats its `=0` row; or `override DECV4 = false` and
`override DECVEC = false` together, which puts the format back on the scalar callback.** With
`DECV4 = true` the class never reads `DECVEC`, so `override DECVEC = false` alone leaves the
hand-written twin running.
