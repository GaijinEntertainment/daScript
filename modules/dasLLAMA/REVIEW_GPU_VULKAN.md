# dasLLAMA Vulkan Tier Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_VULKAN.md` and the companions it routes to. Planned work:
`followup_vulkan.md`.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

**A hand-written Vulkan pipeline build is a defect - a Vulkan pipeline is created only by a
`[vk_dispatch]`-generated `ensure_*`.**

**A diff that adds a Vulkan dispatch family adds every piece of state the family keeps per
model - device buffers, descriptor-set caches, `*_ready` latches, profiler accumulators - to
`vk_drop_model_state`'s sweep, in the same change.** Pipelines are device-lifetime state that
survives the drop and rebuilds lazily.

**Never size a buffer bound as one SSBO (shader storage buffer) range above
`vk_max_storage_range()` - check the size at the site that computes it, not at the site that
binds it.** The bind site cannot shrink a buffer that was sized wrong.

**Never cache a descriptor set or a host address - a pointer into CPU memory - across
dispatches in state that `vk_drop_model_state` does not clear** - hold it in
`dasllama/dasllama_vulkan_common.das` module state that `vk_drop_model_state` clears.

**Never take a quant byte out of an `unpack8` select in a cm2 decode body or its four-wide twin
(`decode_v4`) - load the 16-bit lane from the `int16[N]` block member and shift the byte out,
and assemble a field that straddles two lanes from those lanes, never from selected bytes.**
Indexing `unpack8` of a 32-bit word with a runtime value drops the whole kernel off the
driver's block-load path, about 3x slower, and an `unpack8` lane select costs another 1.1x to
1.5x over the shift form.

**A diff that changes when `vk_rdec_prefill_ids` - the resident prefill that takes token ids
rather than embeddings - accepts a call, or when the override that routes to it
(`vulkan_resident_prefill`) accepts one, updates `vulkan_embed_gpu_gate` in the same
change** - the override and the gate live in `dasllama/dasllama_gpu_resident.das`,
`vk_rdec_prefill_ids` in `dasllama/dasllama_vulkan_prefill.das`. The engine skips the CPU
embed when that gate returns true, so a gate true where the prefill path declines hands the
next consumer an unfilled residual stream.

**A Vulkan-tier serving gate that decides at load - a predicate or per-layer loop whose true
branch dispatches through a `moe_gpu_*` or `vk_*` entry, wherever the diff puts it, and whose
false branch or `continue` routes work to the CPU path - that does not log at load how many
layers or planes it left on the CPU and the reason it left them is a defect.** A silent decline
is a fallback a user finds only by profiling.

**A Vulkan-tier serving gate that decides per call - a predicate or loop whose false branch or
`continue` routes work to the CPU path - that does not log the concrete reason it declined,
once per reason per armed model, is a defect.**

**A prefill GEMM dispatched at a nonzero start row never asks `cm2_split_k` for a split - it
encodes unsplit.** The split-k reduce sums partial planes counted from row 0, so a dispatch
starting above row 0 would reduce the wrong rows.

**Never leave a K/V codec unserved by the kernels that read or write the whole-model driver's
`k_mirror`/`v_mirror` slabs, or the decode block's per-layer `DatLayer.k_mir`/`v_mir` pair - a
K/V codec is the mirror's element type, f16 or f32.** Two shapes serve both: instances of one
template cover both codecs, or a single-codec kernel has a sibling that serves the other codec
behind an arming gate that keys on `kv16`. The whole-model driver serves both codecs, so a
codec no kernel covers silently drops that codec's GPU path.

**A diff that changes what a kq superblock format's cm2 tile emits - its instance set, its
decode body or four-wide twin (`decode_v4`), its `DECV4` or `DECVEC` constant, or the shared
`cm2_tile` or `run` of `KqCm2BatchT` - puts that format's `cm2:<fmt>` probe rows
(`harness/vk_gemm_probe.das`), both the `DASLLAMA_VK_DECVEC=1` and the `=0` rows, in the PR
body, or the claim that the format's cm2-stamped kernels are byte-identical to master's.** A
cm2 tile is the NV_cooperative_matrix2 GEMM class stamped per weight format and token-column
width (the class's `BN`) in `dasllama/dasllama_vulkan_classes.das`.

**A diff that changes what a kq superblock format's KHR tile emits - its `khr_stage16`
override, a constant or typedef the KHR arm's emitted code reads, or the shared `khr_tile` or
`run` of `KqCm2BatchT` - puts that format's kernel cell - that format's test block in
`tests/test_vulkan_kernels.das` - run on its KHR arm in the PR body, with either the `khrx`
probe rows (`harness/vk_gemm_probe.das`) or a `tests/test_gpu_resident_hybrid.das` run on a
model in that format, or the claim that the format's KHR-stamped kernels are byte-identical to
master's.** A KHR tile is the `<Fmt>KhrBatch` class stamped per weight format in
`dasllama/dasllama_vulkan_classes.das`.

**A `kq_sb` format (`dasllama/dasllama_kqformat.das`) that joins the cm2 template - a
`<Fmt>Cm2T` format template in `dasllama/dasllama_vulkan_classes.das` - ships its KHR
instantiation (`<Fmt>KhrBatch`, the `kq_batch_<fmt>_khr_cls` dispatch) and its arm in each of
`khr_cls_ensure`, `khr_cls_set` and `khr_cls_enc` (`dasllama/dasllama_vulkan_prefill.das`), in
the same change.** `pf_f16_feed` admits every `kq_sb` format, so a format with no KHR
class panics in `khr_cls_ensure`, `khr_cls_set` or `khr_cls_enc` on a card whose
cooperative-matrix mode is KHR.

**A `kq_sb` format that ships a KHR instantiation runs its KHR arm in that format's kernel cell,
in the same change.**

**`khr_stage16` stays abstract on `KqCm2BatchT` (`dasllama/dasllama_vulkan_classes.das`) - a
diff that gives it a default body is a defect.**

**A kernel body that calls a `[spirv_decode]` method directly passes the plane element itself
(`decode(wq[i], ...)`), never a local copy of it (`let blk = wq[i]` then `decode(blk, ...)`).**
Both compile: on the element the emitter passes the index and the callee chains through the
plane; on a copy it loads and spills the whole block per call
(`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2k).

**A diff that puts a format's `cm2:<fmt>` probe rows in the PR body whose
`DASLLAMA_VK_DECVEC=1` row is slower than its `=0` row ships one of two fixes in the same
change: a hand-written `decode_v4` under `override DECV4 = true` on that format's class
(`dasllama/dasllama_vulkan_classes.das`), re-measured so its `=1` row now beats its `=0` row;
or `override DECV4 = false` and `override DECVEC = false` together, which puts the format back
on the scalar callback.** With `DECV4 = true` the class never reads `DECVEC`, so
`override DECVEC = false` alone leaves the hand-written twin running.

**A diff that changes how many GPU timestamps the resident decode's token command records - the
`pfq_ts` calls in `dasllama/dasllama_vulkan_decode.das` - updates the stamp count `rdq_sample`
expects and, for every layer kind whose count moved, that file's role-name tables
(`rdq_role_names`, `RDQ_DN_NAMES`, `rd_moe_tail_names`) and the matching accumulators in
`dasllama/dasllama_vulkan_common.das` (`g_rdq_role`, `g_rdq_dn`, `g_rdq_moe`), in the same
change.** `rdq_sample` indexes a fixed count per layer, so one extra or missing timestamp
reports every later stamp under the wrong role name.

**A decode GEMV class - a `KqGemvBase` leaf in `dasllama/dasllama_vulkan_classes.das` - that
stages a codebook into `@workgroup` memory reads it from the family's grid buffer (`gridb`,
filled by `kq_grid_dev` at the format's `KQ_GRID_<FMT>` offset), never from a `*_grid_word`
accessor.** The accessor is a constant composite the driver reads lane-serially per index,
while the grid buffer is staged once per workgroup and read by every row that workgroup serves,
whatever their number (`ARCHITECTURE_GPU_VULKAN.md` sec.2.2ab).

**A diff that changes how many GPU timestamps the resident prefill's window command records - a
`pfq_ts` call in `pf_run` or in any function `pf_run` reaches, all in
`dasllama/dasllama_vulkan_prefill.das` - updates `pf_roles_per_layer` and that file's
`pf_prof_report` in the same change.** Both index a fixed count per layer, so one extra or
missing timestamp reports every later stamp under the wrong role name.

**A diff that changes `AR_MAX_DIM` (`dasllama/dasllama_vulkan_common.das`) changes the `row`
`@workgroup` slab of `ArBase` (`dasllama/dasllama_vulkan_classes.das`) and the `c.dim` cap of
the Vulkan servability gate (`attn_dec_shape_ok`, `dasllama/dasllama_blocks.das`) to the same
number, in the same change.** The add+rms kernels stage a whole row in that slab, so a slab
shorter than the cap writes past its end.
