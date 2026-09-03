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

**A kernel body that emits a function pointer or a vtable into the shader is a defect - splice
the choice at compile time instead.** A `class template` / `def abstract` / `def override`
splice is compile-time and conforms - check the emission, not the das spelling.

**Never give a `*_decline_caps` predicate a parameter beyond the model, the row count, and
whether the call carries a uniform attention span - however that parameter is derived;
window-setup state is asked by `prefill_decline` / `decode_decline` instead.**

**A bounds or tail guard that branches per iteration in a kernel's main loop, where the host
already knows its answer as it picks the pipeline, is a defect - stamp the guard instead.**
Stamped means the guard is carried by a `@template_constant` - a `static_if` block, or a value
select on the constant. The instance stamped without the guard shows no guard in its generated
`*_msl` global.

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

**Never fill a `@workgroup` tile with a per-element loop whose addressing needs a div or mod
per element - give each work item a consecutive run of elements instead.** A lane-coalesced
stride (`i += 32`) and a device-to-device copy loop are already coalesced and conform.

**Never decide a kernel row's validity or owner by scanning the per-bucket base and count
arrays - read the one per-row entry instead.** The bucket-building kernel writes that per-row
entry. The scan repeats on every thread of every row's threadgroup, and it grows with the
bucket count.

**Never test a bucket row's validity against the pad sentinel `0xFFFFFFFF` - compare the row's
per-row bucket entry, the one the bucket-building kernel writes, with the live entry count
(positions x experts per token, `npos * nk`) instead.** Rows past the last expert's stamped
tail hold stale pool bytes, not the sentinel, and an equality test sends their token index
out of bounds.

**Never gate an early `return` in a kernel body that runs a cooperative op - a `barrier()`, a
simdgroup matrix op, or a cross-lane reduction - on a per-thread value; gate it on a
threadgroup-uniform value instead.** A per-thread exit leaves the threadgroup unable to
complete the op.

**An encoder that picks a kernel form whose loop carries no bounds or tail guard - stamped
without one, or generated from a template instance that has none - shows that every address
the form touches stays inside its buffers' allocations.** One extent dividing evenly is not
that showing. A padded chunk's walk can run past the live extent, and one poisoned read in a
shared tile corrupts real rows.

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

**A prefill GEMM dispatched at a nonzero start row never asks `cm2_split_k` for a split - it
encodes unsplit.** The split-k reduce sums partial planes counted from row 0, so a dispatch
starting above row 0 would reduce the wrong rows.

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

**A dummy-bound field where a gate serves is a defect - a stamp-varying binding is carried by
`@template_gate` instead.**

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
row can reach it - a class in neither place leaves `CENSUS_NEVER_DISPATCHED` claiming coverage
the census does not have.

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
default-filling wrapper, or a composite over generated builders.**

**A hand-rolled bind list on a dispatch that serves a user call, in `dasllama/` or
`performance/`, is a defect: dispatch through the kernel's `enc_*` builder instead.**

**A value that reaches the kernel twice device-side - a scalar bound both as a uniform buffer
and as a kargs field - is a defect.** A `params=` value that the `grid=`/`tg=` spec consumes
host-side never reaches the device, so it does not count.

**Never bind a scalar that the other bound scalars already determine - derive it in the
builder instead.** Binding it separately adds a second place to get it wrong.

**Never key a cache on a host address alone - carry the span and the form, the element type and
layout the upload produces, in the key too.** A hit must cover the request.

**A diff that lands a kernel class, driver arm, or backend capability in a file whose
`ARCHITECTURE_GPU.md` sec.1.5 role row does not sanction it extends that row's ledger in the
same change - or moves the code to the file whose row does.**

**A module that creates its own GPU device or queue is a defect - a GPU family shares the one
device and queue from `dasllama/dasllama_<gpu>_common.das`'s init.**

**Never compile or release a Metal PSO (pipeline state object) from an engine file
(`dasllama/`) other than the one that owns its kernel class** - it goes through that file's
own init/release pair.

**A string-typed Metal decline reason is a defect - a Metal decline reason is an enum value in
`dasllama/dasllama_metal_shapes.das`, one enum per driver.**

**A decline counter beside the decline site is a defect - decline counting lives in
`dasllama/dasllama_metal_common.das`.**

**A diff that changes what one backend can serve and the other cannot - a Metal-only or
Vulkan-only hook, role, served path, or backend-only capability, added or removed - lands its
`ARCHITECTURE_GPU.md` sec.1.5 edit in the same change.** One backend serving the same path
faster or slower is not such a change. The duty holds when sec.1.5 already carries that class
of asymmetry, and it covers sec.1.5's per-driver lists of registered hooks and borrowed
kernels. sec.1.5 is the closed list of backend asymmetries; an asymmetry it does not
carry does not exist.

**A hand-written Vulkan pipeline build anywhere in the engine is a defect - a Vulkan pipeline
is created only by a `[vk_dispatch]`-generated `ensure_*` and torn down by
`vk_drop_model_state`.**

**Never size a buffer bound as one SSBO (shader storage buffer) range above
`vk_max_storage_range()` - check the size where it is NEGOTIATED, not where it binds.** The
bind site cannot shrink a buffer that was sized wrong.

**A change to code that a served GPU decode or prefill path executes ships GPU-vs-CPU parity
on one q8 and one kq (K-quant) model with the armed mirror codec.** That code is anything a
served GPU decode or prefill call executes OR that selects what it executes - a driver, a
kernel class it dispatches, that class's builder, a servability gate, a race that picks which
kernel serves, a forwarder default, a weight-region or residency path, the tier forwarders
the call routes through; never the bake paths, never a comment. The parity run is
`harness/parity.das` on either backend, `benchmarks/lcpp_bench.das --parity`
(`performance/model_specs.das`'s fixed model list) on either backend, or - on Metal only -
the in-suite instruments `tests/test_metal_decode_parity.das` /
`tests/test_metal_prefill_parity.das` through `tests/run.das`.

**Parity evidence counts only when its backend was armed: the Metal arm ran with `--ngl`; the
Vulkan arm ran with `DASLLAMA_GPU=1` - never `--ngl` - and its log shows the tier that serves
the changed path armed (`resident driver armed` for the whole-model driver, `GPU MoE tier: ...
resident` for the per-op tier).** The Vulkan driver declines codec-mismatched sessions silently.

**A change to the bake-trim path in `dasllama/dasllama_gpu_resident.das` (`trim_model_planes`)
ships a `dasllama-convert --trim` bake plus a serve of the trimmed image, on one q8 and one kq
(K-quant) model.** Parity runs never reach it.

**Never leave a K/V codec unserved by the kernels that read or write the whole-model driver's
`k_mirror`/`v_mirror` slabs, or the decode block's per-layer `DatLayer.k_mir`/`v_mir` pair - a
K/V codec is the mirror's element type, f16 or f32.** Two shapes serve both: instances of one
template cover both codecs, or a single-codec kernel has a sibling that serves the other codec
behind an arming gate that keys on `kv16`. The whole-model driver serves both codecs, so a
codec no kernel covers silently drops that codec's GPU path.

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

**A module-level variable in a GPU driver file whose value depends on the installed model
gets a model-swap discharge in the same change that adds it** - the vulkan tier files
discharge through `moe_gpu_model_marks_save_` / `moe_gpu_model_marks_restore_` /
`moe_gpu_drop_model_`; the Metal prefill and tower through `register_reload_prep`
(`dasllama/dasllama_metal_common.das`). A global with no discharge survives a model swap and
routes the next model's dispatches at the old model's planes.

**A diff that changes how a dev-W resident panel's cache key is built - a dev-W panel is a
weight plane dequantized once into a device f16 panel - changes both the seed site and the
lookup site in the same change** - `pf_devw_seed_baked` and `pf_devw_resident_panel` in
`dasllama/dasllama_metal_prefill.das`. A seed keyed differently from the forward never hits,
and every baked site silently re-dequantizes.

**A servability gate in `dasllama/dasllama_metal_shapes.das` never reads process-global
runtime state - the active kernel backend, a mode toggle - on its mint-time path: such a read
sits behind the gate's `mint_time` flag, and the mint-time verdict tests the model's own
fields.** The load selects the repacking CPU backend before the GPU backend is decided, so a
mint-time read bakes a verdict the drivers do not share.

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
