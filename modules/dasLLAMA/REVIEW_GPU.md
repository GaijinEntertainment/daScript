# dasLLAMA GPU Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_MTP.md`, `ARCHITECTURE_GPU_MTP_DECODE.md`,
`ARCHITECTURE_GPU_VULKAN_NROW.md`.
Planned work: `followup_metal.md` for Metal, `followup_vulkan.md` for Vulkan.

**A diff that files GPU planned work in `followup_general.md` is a defect** - it goes to
`followup_metal.md` or `followup_vulkan.md`.

**A diff touching a GPU kernel timing arm - code that dispatches a kernel to measure it rather
than to serve a call - wherever the diff puts it, applies `REVIEW_GPU_RACE.md` too.**

**A diff changing a property of a kernel class that a timing arm or a test-side dispatcher
restates rather than reads - a binding number, the kargs (kernel-argument struct) layout, the
layout of a struct a bound buffer holds, threadgroup memory, a staging shape, the grid or
threadgroup geometry - or a kernel class's branch selection or the precision it computes a step
at, applies the `tests/` subfolder's `REVIEW_KERNEL_CELLS.md` for the test-side dispatchers that
dispatch or bind the class.**

**A diff that changes a kernel's binding numbers, its kernel-argument struct or push-constant
layout, its threadgroup or workgroup memory, its staging shape (the operand tile a kernel copies
into that memory before it computes), or its grid, threadgroup or workgroup geometry resyncs or
deletes, in the same change, every timing arm that mirrors that kernel's binding order by hand or
by an ordered setter list and every arm ledgered as that kernel's retained reference.** An arm
left dispatching stale geometry measures the wrong kernel silently.

**A diff that changes what a kernel's body computes resyncs or deletes, in the same change, every
timing arm that carries that body as a hand-written twin, and every arm ledgered as that kernel's
retained reference; a diff that routes a shape to a sibling kernel class resyncs, in the same
change, the arms that dispatch the old class at that shape.** An arm timing a body the shipped
kernel no longer runs at that shape measures the wrong kernel silently.

**A diff touching a tower driver (`dasllama/dasllama_metal_tower.das`,
`dasllama/dasllama_vulkan_tower.das`), a kernel class or builder a tower dispatches, the
`[metal_dispatch]` emission those builders are generated from (`dasllama/dasllama_metal_lens.das`),
the Metal ASR decoder (`dasllama/dasllama_metal_asr_dec.das`), a kernel class the ASR decoder
dispatches or a builder it borrows, or `dasllama/dasllama_metal_common.das` applies
`REVIEW_TOWER.md` too.**

**A diff touching the Vulkan tier - `dasllama/dasllama_*vulkan*.das`,
`dasllama/dasllama_gpu_resident.das`, `dasllama/dasllama_gpu_tier.das`, a `[vk_dispatch]` class, a
`[spirv_decode]` callback, or a cooperative-matrix GEMM class stamped per weight format and
column width, on the NV cooperative-matrix-2 arm or the KHR cooperative-matrix arm, or a kernel
cell or probe that fills or binds a `TokMeta` block - wherever the diff puts it - applies
`REVIEW_GPU_VULKAN.md` too.**

**A diff that adds or changes a GPU kernel class - a `[metal_kernel]` def, a class carrying
`[metal_dispatch]` or `[vk_dispatch]`, a base shell one derives from, or a class template one
stamps - or a dispatch's bind list, generated or hand-rolled, wherever the diff puts it,
applies `REVIEW_GPU_KERNEL_CLASSES.md` too.**

**A diff that changes what a kernel body does or the geometry a dispatch hands it - a main
loop with its guards and bounds, a cooperative op, a staged or padded operand, a workgroup tile
fill, a chunk step and its alignment gate, a bucket-ordered read - wherever the diff puts it,
applies `REVIEW_GPU_KERNEL_BODY.md` too.**

**A device buffer a dispatch writes is sized to every element the dispatch's full grid writes,
never to the live count of rows, keys or chunks.** A dispatch writes whole tiles past the live
rows, and a batched attention writes, for every row, as many key chunks as the row at the largest
position has, so a buffer sized to the live count is overrun silently into whatever the pool put
next to it.

**A row-splitting GEMM encoder - one that dispatches a subset of a site's output rows at an
offset - is called only where the width it is given equals the row stride the site writes with,
so a wider-row site passes the full stride as that width or dispatches the padded tile instead
of splitting.** A split row writes at `row x dispatched-width`, so a wider-row caller lands its
split rows on the row beside them.

**A scratch buffer a dispatch writes is never rebound for a new write before the reader of
its previous write is encoded - rotate through as many buffers as the chain has dispatches in
flight between a write and its read.** One shared scratch serializes the whole chain through
its write-after-read hazards.

**A diff that divides one op's work across two or more dispatches - on a new path or on one that
had a single dispatch - gates the path in the same change on the quantity the split divides (its
K, key span or row count), or on the path's work size when it divides none of these. The gate's
threshold, or the decision to ship no gate, comes from measurements at the smallest and the
largest value the quantity takes on the path, both in the PR body; no gate ships only where the
split wins at both ends.** The small-work regression hides behind the big-work win.

**A diff that changes a tile, grid, threadgroup, or uniform constant shows the value at every
authoritative site its kind has, in the same change.** The sites per kind: the generated `*_msl`
global for a Metal in-body tile, the `@workgroup` declaration or `local_size_x` for a Vulkan one;
a `@template_constant`'s default and each stamp's `override` for a constant a template stamps;
the `grid=` spec (a CEIL divide), or the `wgs` decode plus its host helper, for a grid; the
`tg=` / `local_size_x` spec for a threadgroup; the single writer for a uniform.

**A diff that changes how a `grid = "wgs"` kernel body decodes its workgroup index, or how the
host computes that class's `wgs`, shows in the same change that the host's count still covers
exactly the indices the body's decode reads - by changing both, by a decode that permutes the
same index set, or - where the body's decode and the host's count both read one shared
function - by showing that for every shape the encoder dispatches that class on, the stamp it
picks is the one that function's value names.** The `grid=` spec carries no number for these
classes, so nothing else ties the two.

**A key that decides whether uploaded bytes or encoded GPU work may be reused compares every
input that content was built from - every span, element type, layout, position, offset and model
shape, the row count, each row's session by its `uid`, and, for every device buffer the content
reads that a grow can free and allocate again, a counter bumped on each such rebuild - never a
session pointer, and never a host address, an offset or a handle alone.** An address, offset or
handle names whatever occupies it now, and the scheduler moves sessions in memory as it admits and
erases streams, so a key without the other inputs, or with a session pointer in place of the
`uid`, reuses stale content silently.

**A `dasllama/` file that creates its own GPU device or queue is a defect - a GPU family shares
the one device and queue from `dasllama/dasllama_<gpu>_common.das`'s init.**

**Weakening `REVIEW.das`'s device-creation check is a defect - a new name in
`DEVICE_CREATION_FILES` or `DEVICE_CREATION_FUNCTIONS`, or a device- or queue-creating call
spelling absent from `DEVICE_CREATION_CALLS`, weakens it.**

**Never compile or release a Metal PSO (pipeline state object) from an engine file
(`dasllama/`) other than the one that owns its kernel class** - it goes through that file's
own init/release pair.

**A string-typed decline reason in a GPU driver file - `dasllama/dasllama_metal_*.das`,
`dasllama/dasllama_*vulkan*.das` and `dasllama/dasllama_gpu_resident.das` - is a defect - a decline
reason is an enum value, one enum per driver: in `dasllama/dasllama_metal_shapes.das` for Metal,
and for Vulkan, in the driver's own file.**

**A decline in a GPU driver file - `dasllama/dasllama_metal_*.das`, `dasllama/dasllama_*vulkan*.das`
and `dasllama/dasllama_gpu_resident.das` - is counted only through a `DeclineCounter`
(`dasllama/dasllama_metal_common.das`) or `VkDeclineCounter` (`dasllama/dasllama_vulkan_common.das`)
and the `note_decline` / `note` call that file declares on it; a count storage type, or a
hand-rolled count, outside those two files is a defect.**

**Never give a `*_decline_caps` predicate a parameter beyond the model, the row count, and
whether the call carries a uniform attention span - however that parameter is derived; window
readiness, whether this window's rope tables are staged, is asked by `prefill_decline` /
`decode_decline` instead.**

**A diff that adds or removes a hook seat only the Vulkan family fills (an `install_*` or
`set_*_hook(s)` slot in `dasllama/dasllama_gpu_tier.das`), a hook a GPU driver registers in a
model family's registry, or a prefill builder a tower driver borrows names it - a seat by its
`install_*` / `set_*` name, a registered hook by the seat's register function, a builder by its
name - in the same change, in the row of `ARCHITECTURE_GPU.md` sec.1.5's role table for the file
that fills, registers or borrows it.**

**A diff that adds or removes a registered override only one GPU backend files
(`register_*("metal", ...)` or `register_*("vulkan", ...)`), a `dasllama/dasllama_gpu_tier.das`
seat Metal fills, a function one backend exports with no counterpart under the other backend's
prefix - the same name after the prefix, the same role - called by code outside that backend's
files, a `[metal_dispatch]` or `[vk_dispatch]` argument or field annotation the other lens lacks,
or a decode or prefill behavior only one backend's drivers provide lands its own entry in
`ARCHITECTURE_GPU.md` sec.1.5's closed asymmetry list in the same change, even when the list
already carries one of the same class.** One backend serving the same path faster or slower is
not such a change.

**A change that can alter what a served GPU decode or prefill path computes or selects ships
GPU-vs-CPU parity on one q8 model, one K-quant model, and one model of a format outside both,
for each of the three that the changed path serves.** That is anything a served GPU decode or
prefill call executes or that selects what it executes - a driver, a kernel class it dispatches,
that class's builder, a servability gate, a race that picks which kernel serves, a forwarder
default, a weight-region or residency path, the tier forwarders and the Vulkan tier-dispatch
seams (`dasllama/dasllama_vulkan_seams.das`) the call routes through.

**A change to a served GPU decode or prefill path that ships no parity runs names both compares
in the PR body: its emitted kernels byte-identical before and after - the `*_msl` globals or the
AIR (Metal's compiled shader IR) they build into, the SPIR-V words `DASLLAMA_VK_SPV_DUMP` writes -
and the host's stamp and dispatch selection unchanged on every input.** Only both compares
together show the change cannot alter what the path computes or selects.

**A diff that names GPU-vs-CPU parity evidence - a run, a log, a claim - applies
`REVIEW_GPU_PARITY.md` (beside this file) for what counts as evidence, together with this list.**

**A diff that adds a call site handing a whole served GPU decode or prefill call to the CPU
path is a defect - it ships the device path in the same change.** A call that runs on the CPU
gives the user who selected the GPU a fraction of its speed.

**A diff that lands a model family, a backend arm or a session shape whose batched decode step
reads a weight plane once per row rather than once for all the step's rows, for any model it
serves, is a defect: it ships that plane's batched read - one read serving every row of the
step - in the same change, or the arm declines that model by name and that backend's
architecture doc - `ARCHITECTURE_GPU_VULKAN_NROW.md` sec.2.2ao for Vulkan,
`ARCHITECTURE_GPU_MTP.md` sec.2.37a for Metal - gains the kinds of layer the batched step does
not serve.** The weight stream is what the batch amortizes.

**A diff in `dasllama/` that calls `create_device_session`, or turns a scheduler's device mode
on (`set_device_kv`), shows at that call site that the live device-home sessions - a device-home
session keeps its K/V only in a device K/V region and has no host cache - stay within
`gpu_device_sessions()`, a scheduler counting as its own `max_streams` of them.** The driver
panics on a device-home session that finds no region.

**A diff that sets the resident regions (`set_gpu_resident_regions`) or the context cap
(`set_gpu_ctx_max`, `DASLLAMA_GPU_CTX_MAX`) sets it before the load it governs.** The loader plans
the mirror from the regions and the cap, so a value set after the load never reaches the plan.

**A diff that adds an `RdecPass` value gives it its own reader-facing sentence in
`rdec_pass_words`, in plain words that name no part of the engine - no pass, region, mirror,
resident driver, tier or rails - in the same change.** A status page prints those words; a
value with none prints its machine name instead.

**A diff that drops a model (`moe_gpu_drop_model`) a device-mode scheduler serves turns that
scheduler's device mode off (`set_device_kv(sch, false)`) before its next step, in the same
change.** The drop releases the regions, and a scheduler left in device mode admits the next
stream as device-home onto none.

**A change to the bake-trim path in `dasllama/dasllama_gpu_resident.das` (`trim_model_planes`)
ships a `dasllama-convert --trim` bake plus a serve of the trimmed image, on one q8 model, one
K-quant model, and one model of a format outside both, for each of the three the trim path
bakes.** Parity runs never reach it.

**An f16 store into any device buffer holding K or V rows that does not clamp to the f16 finite
range (+/-65504) is a defect.**

**A per-layer K/V panel (the per-layer device K/V slab the prefill GEMMs write; the Vulkan
resident mirror is one) or mirror slab that aliases another layer's is gathered, stored and
released only through its source layer.** An aliasing layer that gathers, stores or releases
a second time double-frees the panel or overwrites the source's rows.

**A resident override that touches the mirror before gating the session on the armed mirror
codec and on the flat (non-paged) cache is a defect** - a resident override is a
decode/prefill hook `dasllama/dasllama_gpu_resident.das` registers in
`dasllama/dasllama_common.das`'s override registries.

**An override that byte-copies mirror bytes across codecs is a defect - bytes move only between
same-codec session rows and mirror rows.** A cross-codec copy corrupts the host's authoritative
cache.

**A GPU path that runs a recurrent (deltanet) forward itself zeroes the deltanet state on a
call at position zero, and declines every call at a nonzero position other than the next
position the state it advances expects - `Session.dn_pos` for the host state, `DnMirror.pos`
for a device mirror.** It runs the whole forward itself, so the engine's own forward-only guard
never runs.

**A module-level variable in a GPU driver file whose value depends on the installed model gets a
model-swap discharge in the same change that adds it - a discharge is a path the model's drop runs
that returns the variable to its no-model value.** The vulkan tier files discharge through
`moe_gpu_model_marks_save_` / `moe_gpu_model_marks_restore_` / `moe_gpu_drop_model_`, a Vulkan
driver file the `moe_gpu_model_marks_*` pair does not cover through a listener it registers with
`register_vk_drop_hook` (`dasllama/dasllama_vulkan_common.das`), which the drop's sweep runs, and
the Metal tier through `register_reload_prep` (`dasllama/dasllama_metal_common.das`). A global
with no discharge survives a model swap and routes the next model's dispatches at the old model's
planes.

**A diff that changes how a dev-W resident panel's cache key is built - a dev-W panel is a
weight plane dequantized once into a device f16 panel - changes both the seed site and the
lookup site in the same change** - `pf_devw_seed_baked` and `pf_devw_resident_panel` in
`dasllama/dasllama_metal_prefill.das`. A seed keyed differently from the lookup never hits,
and every baked site silently re-dequantizes.

**A servability gate in `dasllama/dasllama_metal_shapes.das` never reads process-global
runtime state - the active kernel backend, a mode toggle - on its mint-time path: such a read
runs only where the gate's `mint_time` flag is false, and the mint-time verdict tests the
model's own fields.** The load selects the repacking CPU backend before the GPU backend is
decided, so a mint-time read bakes a verdict the drivers do not share.

**A diff that gives a per-format pick that maps a weight format to a Metal pipeline or
builder - a dispatch ladder, a table row, a picker function - an arm for a weight format adds that
format to `kq_fmt_gpu_supported`, or, for an expert plane, to `moe_fmt_metal_served` (both
`dasllama/dasllama_metal_shapes.das`), in the same change.** Those predicates are what declines
an unserved format, so an unlisted format decodes under whatever the pick's default arm holds.

**A diff that gives the Metal PLE (per-layer embedding) token-table gather a weight format lands,
in the same change, the format's compiled pipeline (its `g_pf_pso_ple_gather*` global with its
compile and release lines), its arm in the PSO pick `ple_gather_pso_of` and its arm in the
encode ladder `pf_enc_ple_gather_fmt` (all in `dasllama/dasllama_metal_prefill.das`), and
`metal_ple_pre_gpu_gate` admits the format only where the table's base and row stride are whole
superblocks of that format's own block size.** A format with a pick and no pipeline declines to
the CPU gather silently; one with a pipeline and no encode arm, or a base off its own lattice,
is gathered under a layout that is not its own.
