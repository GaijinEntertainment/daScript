# dasLLAMA Vulkan Tier Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_VULKAN.md`, `ARCHITECTURE_GPU_VULKAN_GEMM.md`,
`ARCHITECTURE_GPU_VULKAN_DECODE.md`, `ARCHITECTURE_GPU_VULKAN_MOE.md`. Planned work:
`followup_vulkan.md`.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

**A hand-written Vulkan pipeline build is a defect, and so is weakening the `REVIEW.das` check
that reports a `vkCreateComputePipelines(` call in `dasllama/`, `harness/` and `tests/` - a
Vulkan pipeline is created only by a `[vk_dispatch]`-generated `ensure_*`.**

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

**A diff that keys a route of the tier on a Vulkan capability it did not read before - a
device or instance extension by name, a feature bit a `*_supported` probe of
`modules/dasVulkan/daslib/vulkan_boost.das` reads, or a device limit - adds that capability to
`vk_ext_roster` (`dasllama/dasllama_vulkan_common.das`) with what the tier does with it and what
serves without it, in the same change.** The device-init log prints the roster, so a box's log
says which route each capability decided.

**A prefill GEMM whose output rows start above row 0 encodes unsplit, whatever splitter it
uses - `cm2_split_k` or the small f16 GEMM's `F16G_SPLIT` chunks.** The split-k reduce sums
partial planes counted from row 0, so a dispatch starting above row 0 would reduce the wrong
rows.

**Never leave a K/V codec unserved by the kernels that read or write the whole-model driver's
`k_mirror`/`v_mirror` slabs, or the decode block's per-layer `DatLayer.k_mir`/`v_mir` pair - a
K/V codec is the mirror's element type, f16 or f32.** Two shapes serve both: instances of one
template cover both codecs, or a single-codec kernel has a sibling that serves the other codec
behind an arming gate that keys on `kv16`. The whole-model driver serves both codecs, so a
codec no kernel covers silently drops that codec's GPU path.

**A diff that changes what a kq superblock format's cm2 tile emits - a `@template_constant`
value its stamped kernel reads (on `KqCm2BatchT`, on the format's `<Fmt>Cm2T`, or `override`n
on a stamp), a stamp's tile typedefs or instance set, its decode body or four-wide twin
(`decode_v4`), or the shared `cm2_tile` or `run` of `KqCm2BatchT` - puts probe rows in the PR
body, both the `DASLLAMA_VK_DECVEC=1` and the `=0` rows: every format's when the change is to
`KqCm2BatchT` or to a constant's default there, that format's alone when it is to a format's
own template or one of its stamps.** A cm2 tile is the NV_cooperative_matrix2 GEMM class
stamped per weight format, token-column width (`BN`) and k step (`BK`) in
`dasllama/dasllama_vulkan_classes.das`.

**A diff that owes a cm2 stamp's probe rows takes them from the arm that dispatches that stamp,
all in `harness/vk_gemm_probe.das`: `cm2:<fmt>` for the l and m stamps (`<Fmt>Cm2LBatch`,
`<Fmt>Cm2MBatch`), `moe:<fmt>` or `moesk:<fmt>` for the s and e stamps (`<Fmt>Cm2SBatch`,
`<Fmt>Cm2EBatch`).**

**A diff that answers a probe-row or kernel-cell duty with a claim that a stamp's emitted words
did not move carries that stamp's `DASLLAMA_VK_SPV_DUMP` words diffed against master's.**

**A diff that changes what a kq superblock format's KHR tile emits - its `khr_stage16`
override, a constant or a gated member the KHR stamp emits, or the shared `khr_tile` or `run`
of `KqCm2BatchT` - puts that format's kernel cell - that format's test block in
`tests/test_vulkan_kernels.das` - run on its KHR arm in the PR body.** A KHR tile is the
`<Fmt>KhrBatch` class stamped per weight format in `dasllama/dasllama_vulkan_classes.das`; a
member a `@template_gate` admits on the stamp is emitted whether or not its body reads it.

**A diff that owes a KHR tile's kernel-cell run carries with it either the `khrx` probe rows
(`harness/vk_gemm_probe.das`) or a `tests/test_gpu_resident_hybrid.das` run on a model in that
format.**

**Weakening `check_cm2_ladder_sets` in `REVIEW.das` (beside this file) is a defect.**

**Weakening `check_cm2_stamp_tiles` in `REVIEW.das` (beside this file) is a defect.**

**A diff that retires a shape a fixture under `tests/spirv/` (repo root) declares replaces that
fixture's declaration in the same change.** The emitter suite validates the shapes it emits; a
fixture left on a retired shape validates nothing the stamp runs.

**A `kq_sb` format that ships a KHR instantiation runs its KHR arm in that format's kernel cell,
in the same change.**

**Weakening `check_khr_stage16_abstract` in `REVIEW.das` (beside this file) is a defect.**

**A kernel body that calls a `[spirv_decode]` method directly passes the plane element itself
(`decode(wq[i], ...)`), never a local copy of it (`let blk = wq[i]` then `decode(blk, ...)`).**
Both compile: on the element the emitter passes the index and the callee chains through the
plane; on a copy it loads and spills the whole block per call
(`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2k).

**A diff that puts a format's probe rows in the PR body whose
`DASLLAMA_VK_DECVEC=1` row is slower than its `=0` row ships one of two fixes in the same
change: a hand-written `decode_v4` under `override DECV4 = true` on that format's class
(`dasllama/dasllama_vulkan_classes.das`), re-measured so its `=1` row now beats its `=0` row;
or `override DECV4 = false` and `override DECVEC = false` together, which puts the format back
on the scalar callback.** With `DECV4 = true` the class never reads `DECVEC`, so
`override DECVEC = false` alone leaves the hand-written twin running.

**A GPU timestamp the resident decode's token command records goes through `rd_ts` with the name
its interval bills - never a bare `pfq_ts` - in `dasllama/dasllama_vulkan_decode.das`; the
command's first stamp is the anchor and takes the empty name.** The profiler (`rdq_sample`) sums
intervals by the recorder's own names, so a bare stamp leaves the stamp count past the names and
the token's roles unaggregated; a name's prefix (`a:` `d:` `m:` `p:` `t:`) picks its table, and a
name shared by two stamps (`a:kv`) sums them on purpose.

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

**Weakening `check_ar_max_dim_triple` in `REVIEW.das` (beside this file) is a defect.**
