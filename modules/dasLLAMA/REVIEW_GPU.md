# dasLLAMA GPU Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`. Planned work:
`followup_metal.md` for Metal, `followup_vulkan.md` for Vulkan.

**A diff that files GPU planned work in `followup_general.md` is a defect** - it goes to
`followup_metal.md` or `followup_vulkan.md`.

**A diff touching a GPU kernel timing arm - code that dispatches a kernel to measure it rather
than to serve a call - wherever the diff puts it, applies `REVIEW_GPU_RACE.md` too.**

**A diff changing a property of a kernel class that a timing arm or a gate restates rather than
reads - a binding number, the kargs (kernel-argument struct) layout, the layout of a struct a
bound buffer holds, threadgroup memory, a staging shape, the grid or threadgroup geometry -
applies the `tests/` subfolder's `REVIEW_KERNEL_CELLS.md` for the gates that hand-dispatch or
hand-bind the class.**

**A diff touching the tower driver (`dasllama/dasllama_metal_tower.das`), a kernel class or
builder the tower dispatches, the `[metal_dispatch]` emission those builders are generated
from (`dasllama/dasllama_metal_lens.das`), the Metal ASR decoder
(`dasllama/dasllama_metal_asr_dec.das`), a kernel class it dispatches or a builder it borrows,
or `dasllama/dasllama_metal_common.das` applies `REVIEW_TOWER.md` too.**

**A diff touching the Vulkan tier - `dasllama/dasllama_*vulkan*.das`,
`dasllama/dasllama_gpu_resident.das`, `dasllama/dasllama_gpu_tier.das`, a `[vk_dispatch]` class, a
`[spirv_decode]` callback, or a cooperative-matrix GEMM class stamped per weight format and
column width, on the NV cooperative-matrix-2 arm or the KHR cooperative-matrix arm - wherever
the diff puts it - applies `REVIEW_GPU_VULKAN.md` too.**

**A diff that adds or changes a GPU kernel class - a `[metal_kernel]` def, a class carrying
`[metal_dispatch]` or `[vk_dispatch]`, a base shell one derives from, or a class template one
stamps - or a dispatch's bind list, generated or hand-rolled, wherever the diff puts it,
applies `REVIEW_GPU_KERNEL_CLASSES.md` too.**

**A kernel body that emits a function pointer or a vtable into the shader is a defect - splice
the choice at compile time instead.** A `class template` / `def abstract` / `def override`
splice is compile-time and conforms - check the emission, not the das spelling.

**Never give a `*_decline_caps` predicate a parameter beyond the model, the row count, and
whether the call carries a uniform attention span - however that parameter is derived; window
readiness, whether this window's rope tables are staged, is asked by `prefill_decline` /
`decode_decline` instead.**

**A bounds or tail guard that branches per iteration in a kernel's main loop, where the guard's
answer is the same for every thread of the dispatch and the host knows it as it picks the
pipeline, is a defect - stamp the guard instead.**
Stamped means the guard is carried by a `@template_constant` - a `static_if` block, or a value
select on the constant. The instance stamped without the guard shows no guard in its generated
`*_msl` global or its SPIR-V dump.

**A chunk-stepping `[metal_dispatch]` kernel - one whose main loop steps one fixed-size chunk at
a time and never checks for a partial last chunk - declares each alignment it assumes on a value
the builder receives - a `params=` name or a kargs field - as one
`<lhs> % N` item in `requires =`, comma-separated.** The generated builder then trips on the
first misaligned dispatch instead of reading the next row.

**A driver that keeps misaligned shapes off a chunk-stepping kernel - its main loop steps
fixed-size chunks with no partial-last-chunk check - gates each dispatch site of that kernel on
that site's own K, the extent that site's loop steps along, never on one gate covering every
site.**

**A dispatch site's alignment gate whose divisor is neither the chunk the kernel that site
dispatches steps nor a multiple of that chunk the site forces by splitting its K extent across
dispatches is a defect.** A gate that checks less than the kernel's chunk silently drops a tail;
a gate that checks more than the site's own split forces never sees a shape the kernel could serve.

**Weakening the MSL emitter's refusal to compile an unlicensed float `matmul2d` A operand -
`[metal_kernel(float_a_ok=true)]` is the license - or its gate
`tests/test_metal_float_a_gate.das`, is a defect.** A float operand keeps the op off its native
fast path.

**A diff that stamps a kernel class `[metal_kernel(float_a_ok=true)]` outside the set
`ARCHITECTURE_GPU.md` sec.2.2b sanctions extends that section in the same change.** A class
the section already covers as a property needs no new line.

**Never threadgroup-stage a `matmul2d` operand whose staged form matches its stored form -
stream it from device instead.** A dequant, a transpose, or a layout or element-type change
makes the forms differ. A staged pass-through costs the op more than the reads it saves.

**Never fill a `@workgroup` tile with a loop whose per-element address needs a div or mod of
anything but the lane's own slot index (the index that steps by one from lane to lane); give
each lane a consecutive run of elements, or a lane-coalesced stride (`i += 32`), instead.** A
device-to-device copy loop is already coalesced and conforms.

**Never decide a kernel row's validity or owner by scanning the per-bucket base and count
arrays - a bucket is the run of rows one expert owns in the bucket-ordered buffer - read the
one per-row entry instead.** The bucket-building kernel writes that per-row entry. The scan
repeats on every thread of every row's threadgroup, and it grows with the bucket count.

**Never test the validity of a row in the bucket-ordered buffer - where each expert owns one
run of rows - against the pad sentinel `0xFFFFFFFF`; compare the row's per-row bucket entry,
the one the bucket-building kernel writes, with the live entry count (positions x experts per
token, `npos * nk`) instead.** Rows past the last expert's stamped tail hold stale pool
bytes, not the sentinel, and an equality test sends their token index out of bounds.

**Never gate an early `return` in a kernel body that runs a cooperative op - a `barrier()`, a
simdgroup matrix op, or a cross-lane reduction - on a per-thread value; gate it on a
threadgroup-uniform value instead.** A per-thread exit leaves the threadgroup unable to
complete the op.

**An encoder that picks a kernel form whose loop carries no bounds or tail guard - stamped
without one, or generated from a template instance that has none - shows that every address
the form touches stays inside its buffers' allocations.** A `requires =` contract on the class
is that showing for the dimension it names; an unchecked claim that an extent divides evenly is
not. A padded chunk's walk can run past the live extent, and one poisoned read in a shared tile
corrupts real rows.

**Never let a prefill pad output row reach a `matmul2d` or a staged cooperative tile as its B
operand - stage it as zero, or bound the walk at the live row count.** Pad rows hold recycled
pool bytes, so a pad row used as B multiplies stale values (NaN included) into every real row
of the tile.

**A prefill K/V panel - the per-layer device K/V slab the prefill GEMMs write; the Vulkan
resident mirror is one - is sized from the padded write extent, never from the live key count.**
The K/V GEMMs write full M-tile rows at the chunk's row offset, so a panel sized to the live
count is overrun silently into whatever the pool put next to it.

**A row-splitting GEMM encoder - one that dispatches a subset of a site's output rows at an
offset - is called only where the width it is given equals the row stride the site writes with,
so a wider-row site passes the full stride as that width or dispatches the padded tile instead
of splitting.** A split row writes at `row x dispatched-width`, so a wider-row caller lands its
split rows on the row beside them.

**A scratch buffer a dispatch writes is never rebound for a new write before the reader of
its previous write is encoded - rotate through as many buffers as the chain has dispatches in
flight between a write and its read.** One shared scratch serializes the whole chain through
its write-after-read hazards.

**A diff that turns one dispatch on an encoder path into two or more also gates that path in the
same change - on the extent the added dispatch divides (the site's own K, key span or row
count), or on the path's work size when the split divides no extent - and the threshold comes
from a measurement at the smallest and at the largest value the gated quantity takes on the
path, both measurements in the PR body.** The small-work regression hides behind the big-work
win.

**A diff that changes a tile, grid, threadgroup, or uniform constant shows the value at that
constant's authoritative site, in the same change.** The site per kind: the generated `*_msl`
global or SPIR-V dump (`DASLLAMA_VK_SPV_DUMP=<dir>`) for an in-body tile; the `grid=` spec (a
CEIL divide), or the `wgs` decode plus its host helper, for a grid; the `tg=` /
`local_size_x` spec for a threadgroup; the single writer for a uniform.

**A diff that changes how a `grid = "wgs"` kernel body decodes its workgroup index, or how the
host computes that class's `wgs`, changes both in the same change.** The `grid=` spec carries no
number for these classes, so nothing else ties the two.

**A cache key covers every input the cached result depends on: a host address, an offset, or a
handle alone is not a key - carry the span and the form, the element type and layout the upload
produces, in the key too.**

**A diff that gives a `dasllama/` file code outside the role its `ARCHITECTURE_GPU.md` sec.1.5
entry names - a kernel class, a driver arm, a backend capability, a dispatch-support macro -
extends that entry in the same change, or moves the code to the file whose entry holds that
role.** A driver arm is host code that ensures, binds, or encodes a dispatch; a backend
capability is a function a driver registers in a hook or capability registry.

**A `dasllama/` file that creates its own GPU device or queue is a defect - a GPU family shares
the one device and queue from `dasllama/dasllama_<gpu>_common.das`'s init.**

**Never compile or release a Metal PSO (pipeline state object) from an engine file
(`dasllama/`) other than the one that owns its kernel class** - it goes through that file's
own init/release pair.

**A string-typed Metal decline reason is a defect - a Metal decline reason is an enum value in
`dasllama/dasllama_metal_shapes.das`, one enum per driver.**

**A Metal decline counter beside the decline site is a defect - decline counting lives in
`dasllama/dasllama_metal_common.das`.**

**A diff that adds or removes a Metal-only or Vulkan-only hook, role, served path, or
backend-only capability - a hook in sec.1.5's per-driver registered-hook or borrowed-kernel
lists included - lands its own entry in `ARCHITECTURE_GPU.md` sec.1.5's closed asymmetry list
in the same change, even when that list already carries an asymmetry of the same class.** One
backend serving the same path faster or slower is not such a change.

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

**Parity evidence counts only when it comes from `harness/parity.das`,
`benchmarks/lcpp_bench.das --parity` (`performance/model_specs.das`'s fixed model list), or an
in-suite parity instrument run through `tests/run.das` that feeds both sides the same fixed
tokens and compares the logits against a fixed tolerance.**

**Parity evidence counts only when its backend was armed: the Metal arm ran with `--ngl`; the
Vulkan arm ran with `DASLLAMA_GPU=1` - never `--ngl` - and its log shows the tier that serves
the changed path armed (`resident driver armed` for the whole-model driver, `GPU MoE tier: ...
resident` for the per-op tier).** A log showing neither arming line measured the CPU.

**Vulkan parity evidence counts only when the run armed the mirror codec - the K/V mirror's
element type, f16 or f32 - that the changed path reads.** `DASLLAMA_VK_KV32=1` arms f32; f16
is the default and needs no flag.

**Vulkan parity evidence whose log carries a `resident override passed a call` line for the
changed path does not count.** That line is the Vulkan driver naming a call it handed back to
the CPU path.

**A diff that adds a call site handing a whole served GPU decode or prefill call to the CPU
path is a defect - it ships the device path in the same change.** A call that runs on the CPU
gives the user who selected the GPU a fraction of its speed.

**A diff that lands a model family, a backend arm or a session shape whose batched decode step
dispatches its rows one at a time - one command per row, rather than all the step's rows in one
dispatch - for any model it serves is a defect: it ships the batched dispatch for that model in
the same change, or the arm declines that model by name and `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`
sec.2.2ao gains the kinds of layer the batched command does not serve.** A batched dispatch reads
each weight plane once for all the step's rows; the per-row step reads it once per row.

**A diff in `dasllama/` that calls `create_device_session`, or turns a scheduler's device mode
on (`set_device_kv`), shows at that call site that the live device-home sessions - a device-home
session keeps its K/V only in a device K/V region and has no host cache - stay within
`gpu_device_sessions()`, a scheduler counting as its own `max_streams` of them.** The driver
panics on a device-home session that finds no region.

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

**An f16 store into any GPU-resident K/V that does not clamp to the f16 finite range
(+/-65504) is a defect.**

**A per-layer K/V panel or mirror slab that aliases another layer's is gathered, stored and
released only through its source layer.** An aliasing layer that gathers, stores or releases
a second time double-frees the panel or overwrites the source's rows.

**A resident override that touches the mirror before gating the session on the armed mirror
codec and on the flat (non-paged) cache is a defect** - a resident override is a
decode/prefill hook `dasllama/dasllama_gpu_resident.das` registers in
`dasllama/dasllama_common.das`'s override registries.

**An override that byte-copies mirror bytes across codecs is a defect - bytes move only between
same-codec session rows and mirror rows.** A cross-codec copy corrupts the host's authoritative
cache.

**A resident override that serves a recurrent (deltanet) model zeroes the deltanet state on a
call at position zero, and declines every call at a nonzero position other than the session's
next deltanet position (`Session.dn_pos`).** The override runs the whole forward itself, so the
engine's own forward-only guard never runs.

**A module-level variable in a GPU driver file whose value depends on the installed model
gets a model-swap discharge in the same change that adds it** - the vulkan tier files
discharge through `moe_gpu_model_marks_save_` / `moe_gpu_model_marks_restore_` /
`moe_gpu_drop_model_`; the Metal tier through `register_reload_prep`
(`dasllama/dasllama_metal_common.das`). A global with no discharge survives a model swap and
routes the next model's dispatches at the old model's planes.

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

**A diff that gives a per-format pick that maps a weight format to a Metal pipeline or builder
- a dispatch ladder, a table row, a picker function - an arm for a weight format adds that
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
