# dasLLAMA GPU Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_ASYMMETRIES.md`. Planned work: `followup_metal.md`,
`followup_vulkan.md`.

**A diff touching a GPU kernel timing arm - code that dispatches a kernel to measure it rather
than to serve a call: a timing race between kernels, a knockout, an overhead measurement -
wherever the diff puts it, applies `REVIEW_GPU_RACE.md`, the timing-race checklist, too.** A data
race between serving dispatches is not a timing race.

**A diff changing a restated property of a kernel class - a binding number, a kargs
(kernel-argument struct) or push-constant layout, a bound struct's layout, threadgroup or workgroup
memory, a staging shape (the operand tile the kernel copies into that memory before it computes)
or the dispatch geometry, each as a timing arm or test-side dispatcher writes it out rather than
reads it - or the rows or elements its body writes, its branch selection or the precision it
computes a step at, or dispatching an existing class at a workgroup size or dispatch grid no
kernel-unit cell (a test cell that dispatches the class itself, not through a serving call) runs
it at, or adding a `[vk_dispatch]` or `[metal_dispatch]` class, applies the `tests/` subfolder's
`REVIEW_KERNEL_CELLS.md` too.**

**A diff touching a tower driver (`dasllama/dasllama_metal_tower.das`,
`dasllama/dasllama_vulkan_tower.das`, `dasllama/dasllama_vulkan_tts.das`), a kernel class or
builder a tower dispatches, or a kernel-argument struct a tower fills for a dispatch, wherever the
struct is declared, applies `REVIEW_TOWER.md` too.**

**A diff touching the `[metal_dispatch]` emission that tower builders are generated from
(`dasllama/dasllama_metal_lens.das`), a module-level `g_tw_*` seat outside the three tower driver
files, or `dasllama/dasllama_metal_common.das` applies `REVIEW_TOWER.md` too.**

**A diff touching an ASR decoder (`dasllama/dasllama_metal_asr_dec.das`,
`dasllama/dasllama_vulkan_asr_dec.das`), a kernel class an ASR decoder dispatches or a builder it
borrows applies `REVIEW_TOWER.md` too.**

**A diff touching a `register_*` function that `dasllama_metal_tower_register`,
`dasllama_vulkan_tower_register` or `dasllama_vulkan_tts_register` calls with a hook - a stage
seat (a hook slot the CPU chain calls to hand a model family's encoder or synthesis stage to the
GPU), a drop hook, or a reload or weights-epoch listener - or a site that calls the hook such a
function stores, applies `REVIEW_TOWER.md` too.**

**A diff touching the Vulkan tier - `dasllama/dasllama_*vulkan*.das`,
`dasllama/dasllama_gpu_math.das`, `dasllama/dasllama_gpu_kernels_common.das`,
`dasllama/dasllama_gpu_resident.das`, `dasllama/dasllama_gpu_tier.das`, a `[vk_dispatch]` class,
a `[spirv_decode]` callback, or a
cooperative-matrix GEMM class stamped per weight format and column width, on the NV
cooperative-matrix-2 arm or the KHR cooperative-matrix arm, or a kernel cell or probe that fills
or binds a `TokMeta` block - wherever the diff puts it - applies `REVIEW_GPU_VULKAN.md` too.**

**A diff that adds or changes a GPU kernel class - a `[metal_kernel]` def, a class carrying
`[metal_dispatch]` or `[vk_dispatch]`, a base shell one derives from, or a class template one
stamps - or a dispatch's bind list, generated or hand-rolled, wherever the diff puts it,
applies `REVIEW_GPU_KERNEL_CLASSES.md` too.**

**A diff that changes what a kernel body does or the geometry a dispatch hands it - a main
loop with its guards and bounds, a cooperative op, a staged or padded operand, a workgroup tile
fill, a chunk step and its alignment gate, a bucket-ordered read - wherever the diff puts it,
applies `REVIEW_GPU_KERNEL_BODY.md` too.**

**A diff that changes the bound a dispatch hands a kernel (a row, key or element count a record
or kargs field carries) or the size a buffer the kernel reads is allocated at, wherever the diff
puts it, applies `modules/REVIEW_SHADER_EMITTERS.md` (repo root) too.** A kernel read's range
depends on both, and a host-only change reaches no kernel file.

**A diff that names GPU-vs-CPU parity evidence or driver-against-itself evidence - a run, a log,
a claim - or changes the text of a Vulkan serving log line a parity rule reads - the resident
driver's `resident driver armed (...)` line, the per-op tier's `GPU MoE tier:` report, the
`resident override passed a call` line - applies `REVIEW_GPU_PARITY.md` too.**

**A diff that hands a GPU decode or prefill call the device served to the CPU path; lands a model
family, backend arm or session shape with a batched decode step; calls `create_device_session`,
`set_device_kv` or `moe_gpu_drop_model`; calls a `set_gpu_*` or `reset_gpu_*` function; adds an
`RdecPass` value; changes `trim_model_planes`; stores into, gathers from or releases a device buffer
holding K or V rows; adds or changes a resident override (a decode/prefill hook the resident driver
registers) or a byte copy of mirror rows (the device copy of the K/V cache); or adds or changes a
GPU path that runs a recurrent forward, applies `REVIEW_GPU_RESIDENT.md` too.**

**A diff that changes a restated property of a kernel class - a binding number, a kargs or
push-constant layout, a bound struct's layout, threadgroup or workgroup memory, a staging shape or
the dispatch geometry - resyncs or deletes, in the same change,
every timing arm that mirrors that kernel's binding order by hand or by an ordered setter list and
every arm ledgered as that kernel's retained reference.** An arm left dispatching stale geometry
measures the wrong kernel silently.

**A diff that changes what a kernel's body computes resyncs or deletes, in the same change, every
timing arm that carries that body as a hand-written twin, and every arm ledgered as that kernel's
retained reference; a diff that routes a shape to a sibling kernel class resyncs, in the same
change, the arms that dispatch the old class at that shape.** An arm timing a body the shipped
kernel no longer runs at that shape measures the wrong kernel silently.

**A change that can alter what a GPU decode or prefill call on a session computes or selects ships
GPU-vs-CPU parity on one q8 model, one K-quant model, and one model of a format outside both, for
each of the three whose path the change reaches - or names both compares in the PR body: its emitted
kernels byte-identical before and after (the `*_msl` globals or the AIR, Metal's compiled shader IR,
they build into; the SPIR-V words `DASLLAMA_VK_SPV_DUMP` writes) and the kernel variant the host
picks, its dispatch selection and every bind argument (buffer, offset, kargs value) unchanged on
every input.** Such a change is anything the call executes or that selects what it executes; only
both compares together show the change cannot alter what the path computes or selects.

**Never size a device buffer a dispatch writes to the live count of rows, keys or chunks - size
it to every element the dispatch's full grid writes.** A dispatch writes whole tiles past the live
rows, and a batched attention writes, for every row, as many key chunks as the row at the largest
position has, so a buffer sized to the live count is overrun silently into whatever the pool put
next to it.

**Never call a row-splitting GEMM encoder - one that dispatches a subset of a site's output rows
at an offset - where the width it is given differs from the row stride the site writes with; a
wider-row site passes the full stride as that width or dispatches the padded tile instead of
splitting.** A split row writes at `row x dispatched-width`, so a wider-row caller lands its
split rows on the row beside them.

**Never rebind a scratch buffer a dispatch writes for a new write before the reader of its
previous write is encoded - rotate through as many buffers as the chain has dispatches in flight
between a write and its read.** One shared scratch serializes the whole chain through its
write-after-read hazards.

**A diff that makes a path split one op - dividing an op's K, key span, row count or other work
size across two or more dispatches whose partial outputs combine - gates the split on the quantity
it divides, or ships it ungated, in the same change, at every work size at which one dispatch's
scratch fits under the path's byte ceiling (the largest scratch buffer the path lets one dispatch
allocate), and at every work size on a path with no ceiling.** Whole steps of an op run one after
another are not a split.

**A diff that sets or changes the threshold of a gate on a path that splits one op's work size
across dispatches whose partial outputs combine, or ships such a path ungated, takes the threshold
- or the evidence that the split wins at both ends - from measurements at the smallest and the
largest value the gated quantity takes on the workloads the path serves, both in the PR body.**
The small-work regression hides behind the big-work win.

**A diff that changes a tile, grid, threadgroup, or uniform constant sets the new value at every
authoritative site its kind has, in the same change.** The sites per kind: the generated `*_msl`
global for a Metal in-body tile, the `@workgroup` declaration or `local_size_x` for a Vulkan one;
a `@template_constant`'s default and each stamp's `override` for a constant a template stamps;
the `grid=` spec (a CEIL divide), or the `wgs` decode plus its host helper, for a grid; the
`tg=` / `local_size_x` spec for a threadgroup; the single writer for a uniform.

**A diff that adds a `grid = "wgs"` kernel class, changes how such a body decodes its workgroup
index, or changes how the host computes that class's `wgs`, shows in the same change that the
host's count covers exactly the indices the body's decode reads - by changing the decode and the
count together, by a decode that permutes the same index set, or, where the decode and the count
both read one shared function, by showing that for every shape the encoder dispatches that class
on, the stamp it picks is the one that function's value names.** The `grid=` spec carries no number for these classes, so nothing else ties the two.

**Never let a key that decides whether uploaded bytes, a cached descriptor set or bind list, or
encoded GPU work may be reused skip an input that content was built from - it compares every
span, element type, layout, position, offset and model shape, the row count, and each row's
session, or any other state the content was built for, by a stable id (a session's `uid`) or a
generation, never by its address; a plane whose source holds still from its load to its release - a weight
region, a loaded table - may key by address when the key covers every piece and the model's
release drops it.** The scheduler moves sessions in memory as it admits and erases
streams, so an address key reuses stale content silently.

**Never key reuse of content that reads a device buffer that growing frees and allocates again
on a host address, an offset or a handle alone - also compare a counter bumped on each
reallocation, or drop every cached entry built over the old buffer at the reallocation.** An
address, offset or handle names whatever occupies it now.

**Never compile or release a Metal PSO (pipeline state object) from an engine file (`dasllama/`)
other than the one that owns its kernel class** - it goes through that file's own init/release pair.

**A function that decides whether a GPU seat or driver declines a call, wherever the diff puts
it, counts a decline - where it counts one - only through a `DeclineCounter`
(`dasllama/dasllama_metal_common.das`) or `VkDeclineCounter` (`dasllama/dasllama_vulkan_common.das`)
and the `note_decline` / `note` call its common file declares on it, or, in
`dasllama/dasllama_gpu_resident.das`, through `note_gpu_cpu_pass` (`dasllama/dasllama_gpu_tier.das`)
over an `RdecPass` value, and builds the counter key - `note_decline`'s `why`, `note`'s `reason` -
from a value of the decline enum of the driver it declines for, and from nothing else; a key held
as a string literal is a defect.**

**Never give a `*_decline_caps` predicate a parameter beyond the model, the row count, and
whether the call carries a uniform attention span - however that parameter is derived; window
readiness, whether this window's rope tables are staged, is asked by `prefill_decline` /
`decode_decline` instead.**

**A diff that adds a seat in `dasllama/dasllama_gpu_tier.das` that only the Vulkan family fills -
a seat is a module-level variable that holds a function and has a public setter, whatever the
setter is named - names the seat, by its setter's name, in the `dasllama_gpu_tier.das` entry of
`ARCHITECTURE_GPU.md#gpu-backends`; a diff that removes one deletes that name; in the same change.**

**A diff that adds a hook a GPU driver registers in a model family's registry (the hook table
`dasllama/dasllama_common.das` declares for that family), or a prefill
builder a tower driver borrows, names it - the hook by the seat's register function, the builder
by its name - in that driver's row of the role table in `ARCHITECTURE_GPU.md#gpu-backends`; a
diff that removes one deletes its name from that row; in the same change.**

**A diff that adds a function or export that `dasllama/` code outside the backend's own files
calls, a hook registration, kernel annotation or setter only one GPU backend (Metal or Vulkan) has,
or a decode or prefill behavior or a step's precision only one has, lands its own entry, naming
the operation the other backend's API or device lacks, in
`ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries`; a diff that removes one deletes its entry; in
the same change.** A hook a tower driver registers in a model family's registry and a seat (a hook
slot `dasllama/dasllama_gpu_tier.das` declares) only the Vulkan backend's files fill land in the
backend role table at `ARCHITECTURE_GPU.md#gpu-backends` instead of an entry of their own; the same
path served faster or slower is not a behavior.

**A diff that adds an environment variable only one GPU backend reads, and that changes what that
backend computes or serves, lands its own entry, naming the operation the other backend's API or
device lacks, in `ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries`; a diff that removes one deletes
its entry; in the same change.** A variable that changes only how fast the backend serves the same
output is not one; one entry serves this rule and the rule above.

**A diff that adds or extends, on one GPU backend (Metal or Vulkan), a prefill or decode path or
a K/V cache feature for a model family the other backend also serves - a capability being any
behavior a served call shows, or that a test shows by driving a setter or an environment variable
the engine exposes -
lands the same capability on the other backend
in the same change, wherever the other backend's API offers the operations it
uses on any device it supports; a PR body that defers it is a defect.** A capability one backend
has makes a served model's behavior depend on the box it runs on.

**A module-level variable in a GPU driver file whose value derives from a loaded model's weights, or
names, sizes or points at device content built from a loaded model (a served model, a draft or tower
model, a TTS voice), and which a later call reads before that call writes it - a record of the
decline reasons the driver has logged for that model is one - gets a model-swap discharge in the
same change that adds it: a path the unload or reload of that model runs that returns the variable
to its no-model value.** The discharge paths: for a Vulkan driver file, a reset
`moe_gpu_model_marks_restore_` or `moe_gpu_drop_model_` runs - in its body, or in a listener the
file registers with `register_vk_drop_hook` (`dasllama/dasllama_vulkan_common.das`), which the
drop's sweep runs; for every Metal driver file, the tower driver included, `register_reload_prep`
(`dasllama/dasllama_metal_common.das`). A global with no discharge survives a model swap and routes
the next model's dispatches at the old model's planes.

**A module-level variable in a Vulkan driver file whose value derives from, or names, sizes or
points at device content built from, a TTS model (a StyleTTS2 or Pocket model) also discharges
in a listener the file registers with `register_weights_epoch_listener`
(`dasllama/dasllama_common.das`), in the same change that adds it.** A TTS model's reload runs
no Vulkan reset, only `bump_weights_epoch`, which every Metal reload prep already runs under.

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

**A diff that gives a per-format pick that maps a weight format to a Metal pipeline or builder - a
dispatch ladder, a table row, a picker function - an arm for a weight format adds that format to
`kq_fmt_gpu_supported`, or, for an expert plane, to `moe_fmt_metal_served` (both
`dasllama/dasllama_metal_shapes.das`), in the same change; an arm whose condition matches more than
one format (a range or family test) also tests `kq_fmt_gpu_supported` - for an expert plane,
`moe_fmt_metal_served` - in that condition.** Those predicates are what declines an unserved format,
so an unlisted format decodes under whatever the pick's default arm holds.

**A diff that gives the Metal PLE (per-layer embedding) token-table gather a weight format, or
changes where a format's table sits so its base or row stride moves on or off the format's block
grid, makes `metal_ple_pre_unserved` decline the format wherever the table's base or row stride is
not a whole number of the format's blocks, and never decline it on block-grid grounds where both
are.** A base off the format's own block grid is gathered
under a layout that is not its own.

**A diff that adds a call that creates a GPU device or a command queue, under a spelling
`DEVICE_CREATION_CALLS` in `REVIEW.das` does not list, adds that spelling to the list in the same
change.** The gate finds only the spellings it lists, so a new one passes it silently.
