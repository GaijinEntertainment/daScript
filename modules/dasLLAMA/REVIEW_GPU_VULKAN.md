# dasLLAMA Vulkan Tier Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_VULKAN.md`, every `ARCHITECTURE_GPU_VULKAN_<CONCERN>.md` companion beside
it, `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_SHARED_GEMM.md` and `ARCHITECTURE_GPU_SHARED_ATTN.md`.
Planned work: `followup_vulkan.md`.

**A diff that changes a GEMM tile class or template - `KqCm2BatchT`, a `<Fmt>Cm2T`, a
`<Fmt>KhrBatch`, a stamp of one - or a kernel body that calls `coopmatLoadTensor`,
`coopmatLoadTensorDecode` or a `[spirv_decode]` method, or a prefill GEMM's k split
(`cm2_gemm_pick`, `F16G_SPLIT`, or an encode that splits k), or that changes the SPIR-V words a
GEMM tile stamp emits (its `DASLLAMA_VK_SPV_DUMP` output against the base branch's), applies
`REVIEW_GPU_VULKAN_TILES.md` (beside this file) too.**

**A diff that adds a Vulkan dispatch family - a `[vk_dispatch]` class and the `ensure_<family>` /
`set_<family>` pair generated from it - or that caches a descriptor set or a host address (a
pointer into CPU memory) a later dispatch reads, puts every piece of state the family keeps per model,
and every such cached set or address, where the model drop clears it, in the same change: in
`vk_drop_model_state`'s sweep, or in a listener registered through `register_vk_drop_hook`
(`dasllama/dasllama_vulkan_common.das`).** To clear is
to zero the state or leave it unreachable to every later read. A `make_device_buf` result and any
field of `RDec` - the resident decode driver's state struct - are swept already, a `*_ready` latch
and a profiler accumulator are not; pipelines are device-lifetime state that survives the drop.

**A diff that adds to `dasllama/dasllama_vulkan_classes.das` a host-side ensure/set/enc pick
ladder over a family's class stamps - the stamps of one `[vk_dispatch]` class template, picked by a
shape argument - including a ladder over a family that had one stamp and gains a second, adds that
family's stamp glob to the kernel-home row's stamp list in `ARCHITECTURE_GPU.md#gpu-backends`, in
the same change.**

**Never size a buffer bound as one SSBO (shader storage buffer) range above the device limit
`vk_max_storage_range()` - check the size with `vk_range_ok` at the site that computes it, or
once at the function that starts the encode chain binding the buffer, against a size no buffer of
that chain can exceed.** The bind site cannot shrink a buffer that was sized wrong.

**A diff that changes when `vk_rdec_prefill_ids` - the resident prefill that takes token ids
rather than embeddings - accepts a call, or when the override that routes to it
(`vulkan_resident_prefill`) accepts one, updates `vulkan_embed_gpu_gate` in the same
change** - the override and the gate live in `dasllama/dasllama_gpu_resident.das`,
`vk_rdec_prefill_ids` in `dasllama/dasllama_vulkan_prefill.das`. The engine skips the CPU
embed when that gate returns true, so a gate true where the prefill path declines sets up a
device gather for nothing, and the CPU embeds the rows after all (`embed_backfill_rows`).

**A Vulkan-tier serving gate that decides at load - a predicate or per-layer loop whose true branch
dispatches through a call into the tier (a function a `dasllama/dasllama_*vulkan*.das` or
`dasllama/dasllama_gpu*.das` file declares), wherever the diff puts it, and whose false branch or
`continue` routes work off the path it armed, to the CPU path or to another path inside the tier -
is a defect when it does not log, at load, how many layers or planes it routed off and why.** A
silent decline is a fallback a user finds only by profiling.

**A Vulkan-tier serving gate that decides per call - a predicate or loop whose true branch
dispatches through a call into the tier (a function a `dasllama/dasllama_*vulkan*.das` or
`dasllama/dasllama_gpu*.das` file declares), wherever the diff puts it, and whose false branch or
`continue` routes work off the path it armed, to the CPU path, to another path inside the tier, or
to a slower form inside the same path (a host upload in place of a device-to-device copy) because of
a model, session or device property (a decline driven by the work's size is not one) - that does
not log words
naming the condition that failed, once per reason per armed model, is a defect.**

**A `*_decline_words` return that a diff adds or changes, for a reason a `VK_DECLINE_WORDS_*`
constant in `dasllama/dasllama_vulkan_tower.das` already states, returns that constant or starts
its text with it - never a second wording of that reason.**

**A diff that adds or changes a function under `dasllama/` that calls `vk_moe_init()`, and that
is reachable other than through a hook `install_vk_moe_hooks` installs, makes that function test
`gpu_want_arms_tier()` first and decline without calling `vk_moe_init()` when the test returns
false.** `vk_moe_init` reads no knob, so a caller that skips the test serves on a box whose
`DASLLAMA_GPU` says no.

**A serving hook - a hook slot the CPU chain calls to hand work to the GPU - that keeps a
`VkDeclineCounter` counts a decline on a false `gpu_want_arms_tier()` under its decline enum's
`device` member, with `VK_DECLINE_WORDS_DEVICE` as the reason text.**

**A diff that changes what a device limit decides for the tier - which path serves, how much it
arms, whether it declines - adds that limit to `vk_ext_roster`
(`dasllama/dasllama_vulkan_common.das`) with what the tier does with it and what serves without
it; a diff that makes a limit or extension the roster already names decide a new route adds the
route to that entry, in the same change.** A new route is a caller family - the functions that
reach the tier through one serving hook - whose serving or declining the limit or extension now
decides. Weakening `check_vk_extension_roster` (`REVIEW.das`) is a defect.

**Never leave a K/V codec unserved by the kernels that read or write the whole-model driver's
`k_slabs`/`v_slabs` mirror slabs, or the decode block's per-layer `DatLayer.k_mir`/`v_mir` pair -
a K/V codec is a storage form those buffers take: a value of `RDec.kv_dt` on the driver's slabs,
an element type on the decode block's pair.** Three shapes serve a codec: stamps of one template
cover it; a single-codec kernel has a sibling behind an arming gate on the driver's codec
(`RDec.kv_dt`); or a kernel that reads f16 or f32 rows binds, in the mirror's place, the float
rows a block codec's store quantizes from (`RDec.kst_dev`/`vst_dev`, `pf_kst`/`pf_vst`) or its
f16 copy of one layer's rows (`RDec.ksh_dev`/`vsh_dev`). A codec no kernel covers silently drops
that codec's GPU path.

**A per-layer dispatch, or the upload of a session's CPU K/V rows into the mirror and their copy
back, reaches its layer's slab pair through `rd_slab(L)` - never through `rd_slab_at` or a slab
number of its own.** A slab holds a contiguous run of layers
(`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan`), so a site that names a slab number itself
binds the wrong rows for every layer in another slab, with no error.

**A diff that adds an array with one entry per slab adds the array's name to `REVIEW.das`'s
`check_mirror_slab_access` in the same change; weakening that check is a defect.**

**Never write a block codec's mirror bytes from any kernel but the codec's store kernel
(`KvQ8StoreT`, `KvTq4StoreT`).** When a session passes to the CPU chain, the tier copies the
mirror's bytes into the CPU K/V cache unchanged, so a second quantizer that rounds another way
hands the CPU chain rows its own store never writes.

**Under a tq4 mirror, never let anything read an attention output row before `KvTq4Unrot` has run
on it - the requant folded into the decode attention's store (`DaAttnArgs.rqk`), the head gate
(`DaAttnArgs.gated`), the f16 feed's conversion of the window tiles' f32 rows, or a reader the
diff adds: run it after the un-rotation instead.** The attention dispatches leave their rows in
tq4's rotated basis (`ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-tq4-basis`).

**A diff that retires a shape a fixture under `tests/spirv/` (repo root) declares replaces that
fixture's declaration in the same change.** The emitter suite validates the shapes it emits; a
fixture left on a retired shape validates nothing the stamp runs.

**A GPU timestamp in `dasllama/dasllama_vulkan_decode.das` recorded by a command whose stamps
`rdq_sample` reads goes through `rd_ts` with the name its interval is reported under - never a
bare `pfq_ts`; the command's first stamp is the anchor and takes the empty name.** The profiler
(`rdq_sample`) sums intervals by the recorder's own names, so a bare stamp records more stamps
than the recorder has names and the token's roles are not summed; a name's prefix (`a:` `d:` `m:`
`p:` `t:`) picks its table, and a name shared by two stamps sums them on purpose.

**A decode GEMV class - `KqGemvLeafT` or a `<Fmt>GemvT` template deriving from it in
`dasllama/dasllama_gpu_kernels_discrete.das`, or a stamp of one in
`dasllama/dasllama_vulkan_classes.das` - that stages a codebook into `@workgroup` memory reads it
from the family's grid buffer (`gridb`, filled by `kq_grid_dev` at the format's `KQ_GRID_<FMT>`
offset), never from a `*_grid_word` accessor.**
The accessor is a constant composite the driver reads lane-serially per index, while the grid
buffer is staged once per workgroup and read by every row that workgroup serves, whatever their
number (`ARCHITECTURE_GPU_VULKAN.md#kq-gemv-grid-buffer`).

**A diff that adds or changes a builder in `dasllama/dasllama_vulkan_decode.das` that asks
`set_<family>` for a class ensures `ensure_<family>` on every path that reaches it.** A set asked
of a class whose pipeline is not ensured is the null handle; `vkd_alloc_set` refuses it by the
class's family name, and the model's prepare fails on the path that skipped the ensure.

**A diff that adds a stamp, or adds, removes or retypes a binding on one stamp, of a
`[vk_dispatch]` class template whose set is picked at dispatch keeps every stamp's binding list
identical, in the same change; a field a `@template_gate` omits on a stamp is not a binding
change.** A set is picked at dispatch when a hand-written picker under `dasllama/` returns one
stamp's set by a stamp argument, or when a driver set is handed across stamps (`RLayer.s_attn`
handed to `DaAttnT`'s set). The picker asks one stamp's set and the encode
dispatches another, so the stamp the diff left behind reads the set's buffers in the wrong
slots, and nothing refuses it.

**A diff that changes how many GPU timestamps the resident prefill's window command records in
the window `pf_prof_report` reads - the call's last - through a `pfq_ts` call in `pf_run` or in
any function `pf_run` reaches, all in `dasllama/dasllama_vulkan_prefill.das`, updates
`pf_roles_per_layer` and that file's `pf_prof_report` in the same change.** Both index a fixed
count per layer, so one extra or missing timestamp reports every later stamp under the wrong role
name.

**A descriptor set the resident driver builds binds a buffer it allocated per row or per region -
one slot per batched row (`* RDec.nb`, the rows the per-row planes hold), or
one slot per region, a region being one session's share of the device K/V mirror and recurrent
state - over the whole buffer, never over one row's or one slot's range.** Each dispatch picks its
row or region at run time, from `TokMeta` or its push constants, so a set narrowed to one slot sends
every other row's or region's dispatch past its binding, with no error.

**A diff that fills a `TokMeta` block, wherever it puts it, writes every field but the pads with
the row's own value: `mirbase` the row's K/V mirror base in elements past the base the push
constants or launch parameters of the kernel carry (0 where they carry the own base of the row),
`dnslot` the row's slot in the deltanet state and conv-history buffers (0 where they hold one
slot); a field a zero-initialized literal leaves at 0 counts as written only where 0 is that row's
value.** A kernel reads each field from the row's block, so a field left unwritten sends the row
to whatever position, base or state the memory held, with no error.

**A `TokMeta` buffer or binding is sized `TOK_META_BYTES`
(`dasllama/dasllama_gpu_kernels_discrete.das`) times the rows the block holds, never a literal or
a size constant of its own.** A literal stops matching when `TokMeta` gains a field, and every row
past the first then reads its fields outside the binding, with no error.

**A diff that adds a recorded form - a recorder that builds, into its own command buffer, a
command whose stamps `rdq_sample` reads (`dasllama/dasllama_vulkan_decode.das`) - gives that
form its own stamp-name list and stamp count.** The profiler sums intervals by the recorder's
own names.

**A diff that adds or changes a recorder that installs its own stamp names in
`g_rdq_stamp_names` (`dasllama/dasllama_vulkan_decode.das`) puts the one-row command's list back
before the recorder returns.** A form that leaves its list installed sends every later one-row
profile to another form's role names.

**A diff that submits a transfer copy with `xfer_submit_after`
(`dasllama/dasllama_vulkan_common.das`) submits no command that writes the buffer that copy reads
until the transfer timeline reaches the value it returned: the host waits on it (`xfer_wait`)
before the writing submit, or `g_gpu.pend_wait` holds at least that value - raised with `max`,
never overwritten - from the copy's submit to the writing submit with no other compute submit
between the two.**
Nothing else orders the copy's read before that write.

**Every integer division or modulo a diff adds or changes in kernel code of
`dasllama/dasllama_vulkan_classes.das`, `dasllama/dasllama_gpu_kernels_discrete.das` or
`dasllama/dasllama_gpu_kernels_common.das` - a kernel body, any method or
`dasllama/dasllama_gpu_math.das` helper it calls - whose divisor is not a literal or a template
constant clamps the divisor to at least one (`max(1u, ...)`) before the division, or sits inside
an `if` whose condition compares that divisor expression with zero.** Editing the divisor,
the values feeding it, or its guard changes a division; moving one unchanged between these files,
with the values feeding its divisor, does not add it. An integer division by zero is undefined in
SPIR-V, and some drivers evaluate both arms of a `?:` select, so neither a select nor a condition
on another expression guards it.

**A diff that adds or changes a path under `dasllama/` that re-records the split form of the
one-row token command in `RDec.cmd[region]` - the chain recorded there with the attention at
`RD_SPLIT_PIECES` key pieces - or replaces a descriptor set it dispatches, makes it clear
`RDec.tok_wide_recorded` for the region whose split form it re-recorded or whose set it replaced,
in the same path.** The wide twin (the same chain at `RD_SPLIT_WIDE_PIECES` pieces) dispatches the
same sets, so a twin left marked recorded runs sets the new record replaced.

**A twin's availability flag (`RDec.unsplit_on`, `RDec.wide_on`) is written where the twin's
command buffers are allocated, in `vk_rdec_prepare`, and nowhere else.** A path that writes
availability outside `vk_rdec_prepare` can turn a form on after the recording pass ran, and
`rd_form_at` then picks a form whose command buffer nothing recorded.

**A diff that adds or changes a buffer the host refills, from a `[cold_path]` function or from the
buffer's allocating function, on a later call after a device command read it allocates it per
call - `make_call_buf` / `try_call_buf` / `soft_call_buf`, a driver's `*_call_buf`, or
`make_stack_shell`'s `per_call` (`ARCHITECTURE_GPU_VULKAN_ROOM.md#vk-per-call-buffers`) - never
through an allocator that may hand out ReBAR memory (device memory the host maps directly).**
`REVIEW.das`'s `check_per_call_buffers` covers a refill from any other function; weakening
`check_per_call_buffers` is a defect.

**Never write through the host mapping of a buffer in `g_gpu.dev_mapped` after a device command
has touched it - finish the write before the first touch, or allocate the buffer per call.** The
NVIDIA driver shadows a large mapped allocation and migrates it at the device's first touch, and a
write-migrate-write cycle hangs the machine.
