# dasLLAMA GPU Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: the `ARCHITECTURE_GPU*.md` companions that `ARCHITECTURE.md` indexes.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with
it.**

**A diff touching a GPU kernel timing arm - code that dispatches a kernel to measure it
rather than to serve a call - or changing a kernel class such an arm mirrors (binding
numbers, kernel-argument (kargs) layout, threadgroup memory, staging shape, grid or
threadgroup geometry) - wherever the diff puts it - applies `REVIEW_GPU_RACE.md` too.**

**A diff touching the tower driver (`dasllama/dasllama_metal_tower.das`), a kernel class or
builder the tower dispatches, the `[metal_dispatch]` emission those builders are generated
from (`dasllama/dasllama_metal_lens.das`), or the Metal ASR decoder
(`dasllama/dasllama_metal_asr_dec.das`) applies `REVIEW_TOWER.md` too.**

**A diff touching the Vulkan tier - `dasllama/dasllama_*vulkan*.das`,
`dasllama/dasllama_gpu_resident.das`, a `[vk_dispatch]` class, a `[spirv_decode]` callback, or a
cm2 tile class (an NV_cooperative_matrix2 GEMM class stamped per weight format and column) -
wherever the diff puts it - applies `REVIEW_GPU_VULKAN.md` too.**

**A kernel body that emits a function pointer or a vtable into the shader is a defect - splice
the choice at compile time instead.** A `class template` / `def abstract` / `def override`
splice is compile-time and conforms - check the emission, not the das spelling.

**Never give a `*_decline_caps` predicate a parameter beyond the model, the row count, and
whether the call carries a uniform attention span - however that parameter is derived; window
readiness, whether this window's rope tables are staged, is asked by `prefill_decline` /
`decode_decline` instead.**

**A bounds or tail guard that branches per iteration in a kernel's main loop, where the host
already knows its answer as it picks the pipeline, is a defect - stamp the guard instead.**
Stamped means the guard is carried by a `@template_constant` - a `static_if` block, or a value
select on the constant. The instance stamped without the guard shows no guard in its generated
`*_msl` global.

**A `[metal_dispatch]` kernel whose main loop steps one fixed-size chunk at a time and never
checks for a partial last chunk declares each alignment it assumes on a value the builder
receives - a `params=` name or a kargs (kernel-argument struct) field - as one `<lhs> % N` item
in `requires =`, comma-separated.** The generated builder then trips on the first misaligned
dispatch instead of reading the next row.

**A driver that keeps misaligned shapes off a chunk-stepping kernel - one whose main loop
steps a fixed-size chunk and never checks for a partial last chunk - gates each dispatch site
of that kernel on that site's own K, the extent that site's loop steps along, never on one
gate covering every site.**

**A dispatch site's alignment gate whose divisor is not the chunk the kernel that site
dispatches steps is a defect.** A kernel that steps 128 behind a gate that checks 256 never
sees a shape it could serve; a kernel that steps 256 behind a gate that checks 128 silently
drops a tail.

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
one per-row entry instead.** The bucket-building kernel writes that per-row
entry. The scan repeats on every thread of every row's threadgroup, and it grows with the
bucket count.

**Never test the validity of a row in the bucket-ordered buffer - where each expert owns one
run of rows - against the pad sentinel `0xFFFFFFFF`; compare the row's per-row bucket entry,
the one the bucket-building kernel writes, with the live entry count (positions x experts per
token, `npos * nk`) instead.** Rows past the last expert's stamped
tail hold stale pool bytes, not the sentinel, and an equality test sends their token index
out of bounds.

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

**A prefill K/V panel is sized from the padded write extent, never from the live key count.**
The K/V GEMMs write full M-tile rows at the chunk's row offset, so a panel sized to the live
count is overrun silently into whatever the pool put next to it.

**A row-splitting GEMM encoder - one that dispatches a subset of a site's output rows at an
offset - is called only from a site whose output row stride equals the width it dispatches; a
wider-row site passes the full stride or dispatches the padded tile.** A split row writes at
`row x dispatched-width`, so a wider-row caller lands its split rows on top of the row beside
them.

**A scratch buffer a dispatch writes is never rebound for a new write before the reader of
its previous write is encoded - rotate through as many buffers as the chain has dispatches in
flight between a write and its read.** One shared scratch serializes the whole chain through
its write-after-read hazards.

**A diff that adds dispatches to an encoder path to save bandwidth also gates that path on
work size, in the same change.** The gate's threshold is measured at the smallest and the
largest work size the path serves. The small-work regression hides behind the big-work win.

**A diff that changes a tile, grid, threadgroup, or uniform constant shows the value at that
constant's authoritative site, in the same change.** An in-body tile constant is confirmed
literal in the generated `*_msl` global or the SPIR-V dump (`DASLLAMA_VK_SPV_DUMP=<dir>`
writes every class kernel's words). A grid constant is read off the class's
`[metal_dispatch]` / `[vk_dispatch]` `grid=` spec, whose `"n/c"` form is a
CEIL-divide; a threadgroup constant off Metal's `tg=` spec or Vulkan's
`[spirv_kernel(local_size_x=)]`. A uniform's value is read at the single writer that fills
its buffer.

**A kernel twin that binds a different kargs (kernel-argument struct) type than its sibling
twin, or shifts a shared field to a different binding number, is a defect - even where one
twin ignores that field.** Kernel twins are kernel classes whose bodies differ on one stamp
axis - one compile-time choice, such as single/batch, format, or single-pass/chunked.

**A copy-pasted kernel twin, or a kernel split into hand instances where a `static_if` on a
`@template_constant` serves, is a defect - kernel twins stamp one `class template`, whatever
the stamp axis is.** Body divergence is carried by a `@template_constant`, or by an
overridden method spliced flat at emission.

**A kernel class template that binds a real buffer to a field the stamp's own body never
reads - a dummy bind that exists only to fill the slot - is a defect: gate that field with
`@template_gate` so the stamps whose bodies do not read it do not carry it.**

**A diff that forks a kernel class out of a shared template shows that the bodies no longer
differ on the compile-time choice the template carried, and names that choice in the
surviving template's comment.**

**A `[metal_dispatch]` / `[vk_dispatch]` field whose memory is load-once - a model plane, or
an `upload_region` upload never written after arming - is a defect unless it carries
`@role = "weight"`, even when the kernel compiles and passes parity.**

**`@role = "weight"` on per-encode data - a pooled buffer the host refills each encode - is a
defect; a per-encode field either omits `@role` or names the access its body performs.**
`weight` tells the generated builder the buffer needs no per-encode hazard tracking.

**A diff that adds a GPU kernel class under `dasllama/` - a `[metal_kernel]` def, a
`[vk_dispatch]` declaration, or a new instance of a template carrying one - covers that class
in `tests/test_kernel_coverage.das`, one of two ways.** Either a census row there dispatches
the class, or the diff names it in that file's `CENSUS_NEVER_DISPATCHED` with the reason no
row can reach it - the two lists together are the file's coverage claim, and a class in
neither makes that claim false.

**Every field of a new kernel class declared in `dasllama/` carries at least one of the
annotations its `[metal_dispatch]` / `[vk_dispatch]` builder reads - `@binding`, `@role`,
`@off`, `@default`.** A field carrying none of them is dropped from the bind list with no error.

**Weakening `[metal_dispatch]`'s refusal to compile a `@workgroup` field with no `tgmem=` spec,
or its gate `test_lens_tgmem_gate` (`tests/test_metal_misc_kernels.das`), is a defect.**

**A kernel field carries `@span` only when every caller binds whole output rows.** A caller
binding a column tile of a wider row passes the tile width as the kernel's n while its rows
stride the full output width, so a span computed from the tile width leaves the rest of
every row outside the tracked hazard range.

**A NEW hand-written `enc_*` body is a defect unless it is a wrapper - a format or twin pick, a
default-filling wrapper, or a composite over generated builders - declare the class so the
`[metal_dispatch]` / `[vk_dispatch]` lens generates the builder instead.**

**A hand-rolled bind list on a dispatch that serves a user call, in `dasllama/` or
`performance/`, is a defect: dispatch through the kernel's `enc_*` builder instead.**

**A value that reaches the kernel twice device-side - a scalar bound both as a uniform buffer
and as a kargs field - is a defect.** A `params=` value that the `grid=`/`tg=` spec consumes
host-side never reaches the device, so it does not count.

**Never bind a scalar that the other bound scalars already determine - derive it in the
builder instead.** Binding it separately adds a second place to get it wrong.

**Never key a cache on a host address alone - carry the span and the form, the element type and
layout the upload produces, in the key too.** A hit must cover the request.

**A diff that lands a kernel class, driver arm, or backend capability in a `dasllama/` file
whose `ARCHITECTURE_GPU.md` sec.1.5 role row does not sanction it extends that row's ledger in
the same change - or moves the code to the file whose row does.**

**A `dasllama/` file that creates its own GPU device or queue is a defect - a GPU family shares
the one device and queue from `dasllama/dasllama_<gpu>_common.das`'s init.**

**Never compile or release a Metal PSO (pipeline state object) from an engine file
(`dasllama/`) other than the one that owns its kernel class** - it goes through that file's
own init/release pair.

**A string-typed Metal decline reason is a defect - a Metal decline reason is an enum value in
`dasllama/dasllama_metal_shapes.das`, one enum per driver.**

**A decline counter beside the decline site is a defect - decline counting lives in
`dasllama/dasllama_metal_common.das`.**

**A diff that adds or removes a Metal-only or Vulkan-only hook, role, served path, or
backend-only capability - a hook in sec.1.5's per-driver registered-hook or borrowed-kernel
lists included, a seat of the `dasllama_gpu_tier` cooperation SPI excluded (the closed list's
standing entry sends those to the tier's role row) - lands its own entry in
`ARCHITECTURE_GPU.md` sec.1.5's closed asymmetry list in the same change, even when that list
already carries an asymmetry of the same class, and even when the diff also extends the file's
sec.1.5 role row.** One backend serving the same
path faster or slower is not such a change.

**A change to code that a served GPU decode or prefill path executes ships GPU-vs-CPU parity
on one q8 and one kq (K-quant) model the changed path serves, with the mirror codec armed
where the changed path reads a K/V mirror.** That code is anything a served GPU decode or prefill call executes OR that
selects what it executes - a driver, a kernel class it dispatches, that class's builder, a
servability gate, a race that picks which kernel serves, a forwarder default, a weight-region
or residency path, the tier forwarders and the Vulkan tier-dispatch seams
(`dasllama/dasllama_vulkan_seams.das`) the call routes through; never the bake paths, never a
comment. The parity run is `harness/parity.das` on either backend, `benchmarks/lcpp_bench.das
--parity` (`performance/model_specs.das`'s fixed model list) on either backend, or - on Metal
only - an in-suite `tests/test_metal_*_parity.das` instrument run through `tests/run.das`.

**Parity evidence counts only when its backend was armed: the Metal arm ran with `--ngl`; the
Vulkan arm ran with `DASLLAMA_GPU=1` - never `--ngl` - and its log shows the tier that serves
the changed path armed (`resident driver armed` for the whole-model driver, `GPU MoE tier: ...
resident` for the per-op tier).** The Vulkan driver declines codec-mismatched sessions silently.

**A change to the bake-trim path in `dasllama/dasllama_gpu_resident.das` (`trim_model_planes`)
ships a `dasllama-convert --trim` bake plus a serve of the trimmed image, on one q8 and one kq
(K-quant) model.** Parity runs never reach it.

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
