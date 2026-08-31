# dasLLAMA GPU Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_PREFILL.md`, `ARCHITECTURE_GPU_VULKAN.md`.

**Routed from `REVIEW.md`: a diff touching a GPU kernel, driver, dispatch class, or the K/V
mirrors applies this list together with `REVIEW.md`.**

**A diff touching the tower driver (`dasllama/dasllama_metal_tower.das`), a kernel class or
builder the tower dispatches, or the Metal ASR decoder applies `REVIEW_TOWER.md` too.**

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

**Never let a `matmul2d` left or right operand reach the op as `float` outside a kernel class
stamped `[metal_kernel(float_a_ok=true)]` - convert it in the pass that writes the operand's
buffer, or in the staging loop that reads it.** A float operand keeps the op off its native
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
without it, or monomorphized with none - shows that every address the form touches stays
inside its buffers' allocations.** One extent dividing evenly is not that showing. A padded
chunk's walk can run past the live extent, and one poisoned read in a shared tile corrupts
real rows; a deliberate tail over-read conforms only where the allocation carries the slack.

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
its previous write is encoded; a flip set smaller than the chain's encode-overlap depth is a
defect.** One shared scratch serializes the whole chain through its write-after-read hazards.

**A diff that adds dispatches to an encoder path to save bandwidth also gates that path on
work size, in the same change.** The gate's threshold is measured at both ends of the size
ladder. The small-work regression hides behind the big-work win.

**A diff that changes a tile, grid, threadgroup, or uniform constant shows the value at that
constant's authoritative site, in the same change.** An in-body tile constant is confirmed
literal in the generated `*_msl` global or the SPIR-V dump (`DASLLAMA_VK_SPV_DUMP=<dir>`
writes every class kernel's words). A grid constant is read off the
class's `[metal_dispatch]` / `[vk_dispatch]` `grid=` spec, whose `"n/c"` form is a
CEIL-divide; a threadgroup constant off Metal's `tg=` spec or Vulkan's
`[spirv_kernel(local_size_x=)]`; the spec alone decides. A uniform's value is read at the
single writer that fills its buffer.

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
differ on a single stamp axis.** The same diff names the axis that is gone, in the surviving
template's comment.

**A `[metal_dispatch]` / `[vk_dispatch]` field whose memory is load-once - a model plane, or
an `upload_region` upload never written after arming - is a defect unless it carries
`@role = "weight"`, even when the kernel compiles and passes parity.**

**`@role = "weight"` on per-encode data - a pooled buffer the host refills each encode - is a
defect; a per-encode field either omits `@role` or names the access its body performs.**
`weight` drops the hazard staging.

**A new kernel class declared in `dasllama/` carries `[metal_dispatch]` / `[vk_dispatch]`
with every annotation that backend's generated builder reads - per-field `@binding` /
`@role` / `@off` / `@default`.** A field carrying none of them is dropped from the bind list
with no error. (A source-text fixture a test spawns to compile is outside this rule.) The
`@workgroup`/`tgmem=` half is a lens compile refusal now - weakening that refusal, or its
gate `test_lens_tgmem_gate`, is a defect.

**A kernel field carries `@span` only when every caller binds whole output rows.** A caller
binding a column tile of a wider row passes the tile width as the kernel's n while its rows
stride the full output width, so a span computed from the tile width leaves the rest of
every row outside the tracked hazard range.

**A NEW hand-written `enc_*` body is a defect unless it is a wrapper - a format or twin pick, a
default-filling wrapper, or a composite over generated builders.**

**A hand-rolled bind list on a SERVED dispatch in `dasllama/` or `performance/` is a defect -
dispatch through the kernel's `enc_*` builder instead.** A race or knockout arm hand-binds by
construction and answers to the hand-binding-arm rules instead.

**A hand-binding arm that binds a field at another field's declared number is a defect - and
so is any bind in an arm the machine check cannot see.** The undeclared-number half is
`REVIEW.das`'s `check_race_bind_numbers` where the arm names its kernel class (a `kn_tgmem`
constant or an in-function `pipeline_from_source`); an arm binding through a pso or tgmem
passed as a function parameter is invisible to it, so BOTH halves of that arm stay the
reviewer's. A mis-numbered arm dispatches, reads the wrong buffer, and its timing crowns the
wrong kernel silently.

**A value that reaches the kernel twice device-side - a scalar bound both as a uniform buffer
and as a kargs field - is a defect.** A `params=` value that the `grid=`/`tg=` spec consumes
host-side never reaches the device, so it does not count.

**Never bind a scalar that the other bound scalars already determine - derive it in the
builder instead.** Binding it separately adds a second place to get it wrong.

**Never key a cache on a host address alone - carry the span and the form, the element type and
layout the upload produces, in the key too.** A hit must cover the request.

**A diff that lands a kernel class, driver arm, or backend capability in a file whose
`ARCHITECTURE_GPU.md` sec.1.5 role row does not sanction it extends that row's ledger in the
same change - or moves the code to the file whose row does.** The role table is the
criterion; a placement the table does not carry does not exist.

**A module that creates its own GPU device or queue is a defect - a GPU family shares the one
device and queue from `dasllama/dasllama_<gpu>_common.das`'s init.**

**Never compile or release a Metal PSO (pipeline state object) from an engine file
(`dasllama/`) other than the one that owns its kernel class** - it goes through that file's
own init/release pair.

**Never put race code outside the file that owns the kernel family - the shared scaffolding
(`race_buf`, `race_envelope_ok`, `race_pair_ms`) belongs to
`dasllama/dasllama_<gpu>_common.das`.** Race code is the in-engine base-vs-twin check that
times both kernels on one queue and compares their outputs.

**Race code sizes its operands at a real model shape - never at a small square slab.** A slab
small enough to sit in cache ranks the kernels by an effect production never sees, and the
race then picks the slower kernel.

**A string-typed Metal decline reason is a defect - a Metal decline reason is an enum value in
`dasllama/dasllama_metal_shapes.das`, one enum per driver.**

**A decline counter beside the decline site is a defect - decline counting lives in
`dasllama/dasllama_metal_common.das`.**

**A diff that adds or removes a Metal-only or Vulkan-only hook, role, served path, or
backend-only capability - anything that changes what one backend can serve and the other
cannot - lands its `ARCHITECTURE_GPU.md` sec.1.5 edit in the same change - including when
sec.1.5 already carries that class of asymmetry, and including sec.1.5's per-driver lists of
registered hooks and borrowed kernels.** sec.1.5 is the closed list; an asymmetry it does not
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
kernel class it dispatches, that class's builder, a servability gate, a kernel crown race, a
forwarder default, a weight-region or residency path, the tier forwarders and engine seams
the call routes through; never the bake paths, never a comment. The parity
run is `harness/parity.das` on either backend, or - on Metal only - the in-suite instruments
`tests/test_metal_decode_parity.das` / `tests/test_metal_prefill_parity.das` through
`tests/run.das`.

**Parity evidence counts only when its backend was armed: the Metal arm ran with `--ngl`; the
Vulkan arm ran with `DASLLAMA_GPU=1` - never `--ngl` - and its log shows the tier that serves
the changed path armed (`resident driver armed` for the whole-model driver, `GPU MoE tier: ...
resident` for the per-op tier).** The Vulkan driver declines codec-mismatched sessions silently.

**A change to the bake-trim path in `dasllama/dasllama_gpu_resident.das` (`trim_model_planes`)
ships a `dasllama-convert --trim` bake plus a serve of the trimmed image, on one q8 and one kq
(K-quant) model.** Parity runs never reach it.

**Never leave a K/V codec unserved by the kernels that read or write the residency rail's
`k_mirror`/`v_mirror` slabs, or the decode block's per-layer `DatLayer.k_mir`/`v_mir` pair - a
K/V codec is the mirror's element type, f16 or f32.** Two
shapes serve both: instances of one template cover both codecs, or a single-codec kernel has
a sibling that serves the other codec behind an arming gate that keys on `kv16`. The rail
serves both codecs, so a codec no kernel covers silently drops that codec's GPU path.

**An f16 store into any GPU-resident K/V that does not clamp to the f16 finite range
(+/-65504) is a defect.**

**A per-layer K/V panel or mirror slab that aliases another layer's is gathered, stored and
released only through its source layer.** An aliasing layer that gathers, stores or releases
a second time double-frees the panel or overwrites the source's rows.

**A resident override that touches the mirror before gating the session on the armed mirror
codec and on the flat (non-paged) cache is a defect** - a resident override is a
decode/prefill hook the whole-model residency rail registers in
`dasllama/dasllama_common.das`'s override registries.

**An override that byte-copies mirror bytes across codecs is a defect - bytes move only between
same-codec session rows and mirror rows.** A cross-codec copy corrupts the host's authoritative
cache.

**Never cache a descriptor set across dispatches in state that `vk_drop_model_state` does not
clear** - put it in a `*_ready` latch, or in a holder that function already clears in
`dasllama/dasllama_vulkan_common.das`: `g_rd`, `g_gpu`, the weight arena.

**A diff that changes anything a hand-binding arm must mirror to dispatch a kernel - binding
numbers, kargs layout, threadgroup memory, grid or threadgroup geometry - fixes or deletes,
in the same change, every arm of a hand-binding lab that binds it** - a hand-binding lab is a
kernel A/B or knockout timing script, wherever it lives, that hand-lists its bindings
instead of dispatching through the `enc_*` builder. A lab left dispatching stale geometry
measures the wrong kernel silently.

**A diff that ports an A/B lab's winning variant into a kernel deletes the ported arm in the
same change - its variant class and any variants-module code that exists only for it; when
the lab exists only for that decision, its driver and remaining arm go too.** An A/B lab is a
timing script whose output SELECTS between implementations of the same compute, wherever it
lives (`benchmarks/`, `harness/`); a decided arm that outlives its decision degrades into an
unmaintained duplicate of the kernel it seeded. An arm `ARCHITECTURE_GPU.md` sec.2.2b ledgers
as a retained reference is the sanctioned survivor - and a retained arm that reimplements a
shipped kernel is resynced or deleted in the same change that moves that kernel's staging
shape or geometry.

**Never read a `[spirv_decode]` callback's quant bytes by indexing `unpack8` of a 32-bit word
with a runtime value - read them as 16-bit lanes instead: an `int16[N]` block member selected
with `unpack8(w)[i & 1u]`, sub-fields pulled out by shift and mask.** The vendor driver's shader
compiler pattern-matches only the 16-bit spelling into its block-load path, and a runtime byte
select drops the whole kernel off it.

**A diff that changes when the resident prefill that takes token ids rather than embeddings
(`vk_rdec_prefill_ids` and the resident prefill override that routes to it) accepts a call
changes the engine's GPU-embed probe - the gate registered through `register_embed_gpu_gate`,
`vulkan_embed_gpu_gate` in `dasllama/dasllama_gpu_resident.das` - in the same change.** The
engine skips the CPU embed on a true probe, so a probe that is true where that path declines
hands the next consumer an unfilled residual stream.

**A module-level variable in a GPU driver file whose value depends on the installed model
gets a model-swap discharge in the same change that adds it** - the vulkan tier files
discharge through `moe_gpu_model_marks_save_` / `moe_gpu_model_marks_restore_` /
`moe_gpu_drop_model_`; the Metal prefill and tower through a registered reload prep. A global
with no discharge survives a model swap and routes the next model's dispatches at the old
model's planes.

**A diff that changes how a dev-W resident panel's cache key is built changes both the seed
site and the lookup site in the same change** - `pf_devw_seed_baked` and
`pf_devw_resident_panel` in `dasllama/dasllama_metal_prefill.das`. A seed keyed differently
from the forward never hits, and every baked site silently re-dequantizes.

**A servability gate in `dasllama/dasllama_metal_shapes.das` never reads process-global
runtime state - the active kernel backend, a mode toggle - on its mint-time path: such a read
sits behind the gate's `mint_time` flag, and the mint-time verdict tests the model's own
fields.** The load selects the repacking CPU backend before the flavor is decided, so a
mint-time read bakes a verdict the drivers do not share.
