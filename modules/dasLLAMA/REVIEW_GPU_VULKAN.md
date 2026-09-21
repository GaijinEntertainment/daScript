# dasLLAMA Vulkan Tier Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_VULKAN.md`, `ARCHITECTURE_GPU_VULKAN_ATTN.md`,
`ARCHITECTURE_GPU_VULKAN_DECODE.md`, `ARCHITECTURE_GPU_VULKAN_GEMM.md`,
`ARCHITECTURE_GPU_VULKAN_MOE.md`, `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`,
`ARCHITECTURE_GPU_VULKAN_NROW.md`, `ARCHITECTURE_GPU.md`. Planned work: `followup_vulkan.md`.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

**A diff that adds a Vulkan dispatch family - a `[vk_dispatch]` class and the `ensure_<family>` /
`set_<family>` pair generated from it - adds every piece of state the family keeps per model to
`vk_drop_model_state`'s sweep, in the same change.** A `make_device_buf` result and any field of
`RDec` - the resident decode driver's state struct - are swept already, a `*_ready` latch and a
profiler accumulator are not; pipelines are device-lifetime state that survives the drop and
rebuilds lazily.

**A diff that adds a host-side ensure/set/enc pick ladder for a new family of class stamps -
the stamps of one `[vk_dispatch]` class template, picked by a shape argument - to
`dasllama/dasllama_vulkan_classes.das` adds that family's stamp glob to the kernel-home row's
stamp list in `ARCHITECTURE_GPU.md` sec.1.5, in the same change.**

**Never size a buffer bound as one SSBO (shader storage buffer) range above
`vk_max_storage_range()` - check the size at the site that computes it, not at the site that
binds it.** The bind site cannot shrink a buffer that was sized wrong.

**Never cache a descriptor set or a host address - a pointer into CPU memory - across
dispatches in state that `vk_drop_model_state` does not clear** - hold it in
`dasllama/dasllama_vulkan_common.das` module state that `vk_drop_model_state` clears.

**Never take a quant byte out of an `unpack8` select in a cm2 decode body - the `decode` method
of a format's `<Fmt>Cm2T` class in `dasllama/dasllama_vulkan_classes.das` - or its four-wide twin
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
false branch or `continue` routes work off the path it armed, to the CPU path or to another
path inside the tier - is a defect when it does not log, at load, how many layers or planes it
routed off and why.** A silent decline is a fallback a user finds only by profiling.

**A Vulkan-tier serving gate that decides per call - a predicate or loop whose false branch or
`continue` routes work off the path it armed, to the CPU path or to another path inside the tier -
that does not log the concrete reason it declined, once per reason per armed model, is a defect.**

**A diff that changes what a device limit decides for the tier - which path serves, how much
it arms, whether it declines - adds that limit to `vk_ext_roster`
(`dasllama/dasllama_vulkan_common.das`) with what the tier does with it and what serves without
it, or adds the new route to the entry it already has, in the same change.** An extension or a
`*_supported` probe the roster omits is `check_vk_extension_roster`'s finding (`REVIEW.das`); the
device-init log prints the roster, so a box's log says which route each capability decided.

**The coordinate a k loop's counter feeds to `coopmatLoadTensor` / `coopmatLoadTensorDecode`
starts at a literal or at a value rounded down to the loop's step - never at a bare runtime
product or a buffer-read value.** That coordinate is `c0` where the contraction runs along
columns, `r0` where it runs along rows. The shader compiler vectorizes the decode-load only
where it can prove the coordinate's alignment, and an unproven start runs the same loop at half
the rate (`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2l).

**A per-loop hint on a kernel loop in `dasllama/dasllama_vulkan_classes.das` carries a name
`append_loop_hint_operand` (`modules/dasLLVM/daslib/llvm_jit.das`) knows.** A kernel body compiles
for the CPU oracle too, and the JIT fails a hint name it does not know.

**A cm2 tile - `cm2_tile` of `KqCm2BatchT` in `dasllama/dasllama_vulkan_classes.das`, the
NV_cooperative_matrix2 GEMM body stamped per weight format, token-column width (`BN`) and k step
(`BK`) - never loads weights through a clamped (`tensorLayout2DPad`) layout on its fast path - the
branch it takes when its 128 weight rows fit the plane, the plane's width is a whole number of k
steps, and its token column is whole or its stamp loads partial columns unclamped (`STILE`); every
other tile takes the edge path.** A clamped decode-load runs every tile at a third the speed
(`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2l).

**A weight tile the plane cannot fill starts at the plane's last whole 128 rows, never past the
plane's end.** The whole dispatch already runs at its partial workgroups' rate, and a load past
the plane reads memory the plane does not own (`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2l).

**A scale cache a cm2 tile stages indexes a row by its offset from the tile's first row
(`wg_m0`), never by `bc.x & 127`.** `bc.x` is the plane's absolute row, and a last-window tile
does not start on a 128 multiple.

**Never split k on a prefill GEMM whose output rows start above row 0 - encode it whole, whatever
splitter it uses (`cm2_gemm_pick`, or the small f16 GEMM's `F16G_SPLIT` chunks).** The split-k
reduce sums partial planes counted from row 0, so a dispatch starting above row 0 would reduce
the wrong rows.

**Never leave a K/V codec unserved by the kernels that read or write the whole-model driver's
`k_mirror`/`v_mirror` slabs, or the decode block's per-layer `DatLayer.k_mir`/`v_mir` pair - a
K/V codec is the mirror's element type, f16 or f32.** Two shapes serve both: instances of one
template cover both codecs, or a single-codec kernel has a sibling that serves the other codec
behind an arming gate that keys on `kv16`. The whole-model driver serves both codecs, so a
codec no kernel covers silently drops that codec's GPU path.

**A diff that changes the shared `KqCm2BatchT` - its `cm2_tile`, its `run`, or a
`@template_constant` default declared there - puts every format's probe rows in the PR body, both
the `DASLLAMA_VK_DECVEC=1` and the `=0` rows.**

**A diff that changes a format's own cm2 tile - its `<Fmt>Cm2T`, a `@template_constant`
`override`n on a stamp, a stamp's tile typedefs or instance set, or its decode body or four-wide
twin (`decode_v4`) - puts that format's probe rows in the PR body, both the `=1` and the `=0`
rows.**

**A diff that owes a cm2 stamp's probe rows takes them from that stamp's `harness/vk_gemm_probe.das`
arm - `cm2:<fmt>` or `cm2g:<fmt>` for the l and m stamps (`<Fmt>Cm2LBatch`, `<Fmt>Cm2MBatch`),
`moe:<fmt>` or `moesk:<fmt>` for the s and e stamps (`<Fmt>Cm2SBatch`, `<Fmt>Cm2EBatch`) - never
from another arm and never from a whole-model sweep; a stamp the probe admits no arm for answers
with its tile cell's run in `tests/test_vulkan_kernels.das` instead.**

**A diff that answers a probe-row or kernel-cell duty with a claim that a stamp's emitted words
did not move carries that stamp's `DASLLAMA_VK_SPV_DUMP` words diffed against master's.**

**A diff that changes the shared `khr_tile` or `run` of `KqCm2BatchT` runs every format's kernel
cell (its test block in `tests/test_vulkan_kernels.das`) on the KHR arm and puts the runs in the PR
body.**

**A diff that changes what a format's own KHR tile emits - its `khr_stage16` override, or a
constant or a gated member the KHR stamp emits - runs that format's kernel cell on the KHR arm and
puts the run in the PR body.** A KHR tile is the `<Fmt>KhrBatch` class stamped per weight format in
`dasllama/dasllama_vulkan_classes.das`; a member a `@template_gate` admits on the stamp is emitted
whether or not its body reads it.

**A diff that owes a KHR tile's kernel-cell run carries with it either the `khrx` probe rows
(`harness/vk_gemm_probe.das`) or a `tests/test_gpu_resident_hybrid.das` run on a model in that
format.**

**A diff that retires a shape a fixture under `tests/spirv/` (repo root) declares replaces that
fixture's declaration in the same change.** The emitter suite validates the shapes it emits; a
fixture left on a retired shape validates nothing the stamp runs.

**A diff that adds a `<Fmt>KhrBatch` stamp adds that format's KHR arm to its kernel cell in
`tests/test_vulkan_kernels.das`, in the same change.**

**A kernel body that calls a `[spirv_decode]` method directly passes the plane element itself
(`decode(wq[i], ...)`), never a local copy of it (`let blk = wq[i]` then `decode(blk, ...)`).**
Both compile: on the element the emitter passes the index and the callee chains through the
plane; on a copy it loads and spills the whole block per call
(`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2k).

**A diff whose probe rows show a format's `DASLLAMA_VK_DECVEC=1` row slower than its `=0` row
ships one of two fixes in the same change: a hand-written `decode_v4` under `override DECV4 = true`
on that format's class (`dasllama/dasllama_vulkan_classes.das`), re-measured so its `=1` row now
beats its `=0` row; or `override DECV4 = false` and `override DECVEC = false` together, which puts
the format back on the scalar callback.** With `DECV4 = true` the class never reads `DECVEC`, so
`override DECVEC = false` alone leaves the hand-written twin running.

**A GPU timestamp the resident decode's token command records goes through `rd_ts` with the name
its interval is reported under - never a bare `pfq_ts` - in `dasllama/dasllama_vulkan_decode.das`;
the command's first stamp is the anchor and takes the empty name.** The profiler (`rdq_sample`)
sums intervals by the recorder's own names, so a bare stamp records more stamps than the recorder
has names and the token's roles are not summed; a name's prefix (`a:` `d:` `m:` `p:` `t:`) picks
its table, and a name shared by two stamps sums them on purpose.

**A decode GEMV class - a `KqGemvBase` leaf in `dasllama/dasllama_vulkan_classes.das` - that
stages a codebook into `@workgroup` memory reads it from the family's grid buffer (`gridb`,
filled by `kq_grid_dev` at the format's `KQ_GRID_<FMT>` offset), never from a `*_grid_word`
accessor.** The accessor is a constant composite the driver reads lane-serially per index,
while the grid buffer is staged once per workgroup and read by every row that workgroup serves,
whatever their number (`ARCHITECTURE_GPU_VULKAN.md` sec.2.2ab).

**Never let a reduce write a workgroup slot while the previous reduce's partials still occupy it -
pass the other slot, or put a `barrier()` between the two reduces.** A reduce sums a value across
the workgroup through a `@workgroup` staging array (`wg_rms_inv` of `RmsWgBase`,
`dasllama/dasllama_vulkan_classes.das`, takes it as the `slot` argument, 0 or 1); a slot is the run
of partials one reduce writes into that array. A reduce carries one barrier, so a thread still
summing the first reduce's partials would read the second's writes out of the same slot.

**A builder in `dasllama/dasllama_vulkan_decode.das` that asks `set_<family>` for a class ensures
`ensure_<family>` on every path that reaches it.** A set asked of a class whose pipeline is not
ensured is the null handle; `vkd_alloc_set` refuses it by the class's family name, and the model's
prepare fails on the path that skipped the ensure.

**A diff that adds a stamp to, or adds, removes or retypes a binding on one stamp of, a
`[vk_dispatch]` class template whose stamps are dispatched with a set one stamp's `set_<family>`
built - through a hand-written picker in `dasllama/dasllama_vulkan_classes.das` (`gemv_cls_set_n`,
`q8_gemv_gu_n_set`, `fa_stamp_set`) or a driver set handed across stamps (`RLayer.s_attn` to
`DaAttnT`'s) - keeps every stamp's binding list identical, in the same change; a field a
`@template_gate` omits on a stamp is not a binding change.** The picker asks one stamp's set and
the encode dispatches another, so the stamp the diff left behind reads the set's buffers in the
wrong slots, and nothing refuses it.

**A diff that changes how many GPU timestamps the resident prefill's window command records - a
`pfq_ts` call in `pf_run` or in any function `pf_run` reaches, all in
`dasllama/dasllama_vulkan_prefill.das` - updates `pf_roles_per_layer` and that file's
`pf_prof_report` in the same change.** Both index a fixed count per layer, so one extra or
missing timestamp reports every later stamp under the wrong role name.

**A descriptor set the N-row token command - the resident decode command that runs `RDec.nb`
rows in one dispatch (`dasllama/dasllama_vulkan_decode.das`) - dispatches binds its plane's whole
`RDec.nb`-row extent, never one row's.** A per-row plane is a buffer the resident decode driver
sizes to one slot per batched row (`* RDec.nb`); a one-row binding makes the N-row command read
past its binding on every row but the first.

**Every `TokMeta` block a diff fills - in the resident driver, a seam, or a test - writes
`mirbase`, the row's mirror base in elements; a site with one mirror region writes 0.** The
attention and the mirror store add that field to every K/V address they touch, so a block left
unwritten sends a row at whatever base the memory held.

**A diff that adds a recorded form - a recorder that builds the resident token command into its
own command buffer (`dasllama/dasllama_vulkan_decode.das`) - gives that form its own stamp-name
list and stamp count.** The profiler sums intervals by the recorder's own names.

**A recorder that installs its own stamp names in `g_rdq_stamp_names`
(`dasllama/dasllama_vulkan_decode.das`) puts the one-row command's list back before it returns.** A
form that leaves its list installed sends every later one-row profile to another form's role names.

**A diff that submits a transfer copy with `xfer_submit_after`
(`dasllama/dasllama_vulkan_common.das`) calls `xfer_spin_wait` on the value it returned before it
submits any command that writes the buffer that copy reads.** The host's wait is the only order
between the copy's read and that write.

**A kernel body in `dasllama/dasllama_vulkan_classes.das` that divides or takes a modulo by a
divisor that is not a literal or a template constant - a push-constant field, bare or computed
from - clamps it to at least one (`max(1u, ...)`) before it divides, unless an enclosing `if` the
zero case cannot enter guards the division; a `?:` select on the field is not a guard.** Some
drivers evaluate both arms of a select, and an integer division by zero is undefined in SPIR-V,
so the selected arm can carry the undefined result.

**A path under `dasllama/` that re-records the one-row token command's split form - the chain
recorded with the attention at `RD_SPLIT_PIECES` key pieces - or replaces a descriptor set it
dispatches, clears that region's wide-twin recorded flag (`RDec.tok_wide_recorded`) in the same
path.** The wide twin (the same chain at `RD_SPLIT_WIDE_PIECES` pieces) dispatches the same sets,
so a twin left marked recorded runs sets the new record replaced.

**A twin's availability flag (`RDec.unsplit_on`, `RDec.wide_on`) is written where the twin's
command buffers are allocated, in `vk_rdec_prepare`, and nowhere else.** A path that writes
availability outside `vk_rdec_prepare` can turn a form on after the recording pass ran, and
`rd_form_at` then picks a form whose command buffer nothing recorded.
