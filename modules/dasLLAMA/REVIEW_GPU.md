# dasLLAMA GPU Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md`.

**Routed from `REVIEW.md`: a diff touching a GPU kernel, driver, dispatch class, or the K/V
mirrors applies this list together with `REVIEW.md`.**

**A kernel body that emits a function pointer or a vtable into the shader is a defect - splice
the choice at compile time instead.** A `class template` / `def abstract` / `def override`
splice is compile-time and conforms - check the emission, not the das spelling.

**Never give a `*_decline_caps` predicate a parameter beyond the model and the call shape -
window-setup state is asked by `prefill_decline` / `decode_decline` instead.** A caps
parameter that reports the session's setup progress - rather than the CALL, its row count or
its span shape - is a defect however it is derived.

**A bounds or tail guard that branches per iteration in a kernel's main loop, where the host
already knows its answer as it picks the pipeline, is a defect - stamp the guard instead.**
Stamped means the guard rides a `@template_constant` - a `static_if` block, or a value select
on the constant. The guard-free instance is the one stamped without the guard, and the guard
is absent from that instance's generated `*_msl` global. A diff that edits an existing
kernel's loop answers to this exactly as a new kernel class does.

**Never let a `matmul2d` left or right operand reach the op as `float` outside a kernel class
stamped `[metal_kernel(float_a_ok=true)]` - convert it in the pass that writes the operand's
buffer, or in the staging loop that reads it.** The stamped set is ledgered in
`ARCHITECTURE.md` sec.2.2b, and a stamp that lands without its ledger line is a defect. A
float operand keeps the op off its native fast path.

**Never threadgroup-stage a `matmul2d` operand whose staged form matches its stored form -
stream it from device instead.** A dequant, a transpose, or a layout or element-type change
makes the forms differ. A staged pass-through costs the op more than the reads it saves.

**Never fill a `@workgroup` tile with a loop that gives each work item a stride - give each
work item a consecutive run of elements instead.** A device-to-device copy loop fills no tile
and is already coalesced. Per-element strided staging with div/mod addressing pays multiples
of what the contiguous form costs.

**Never decide a kernel row's validity or owner by scanning the per-bucket base and count
arrays - read the one per-row entry instead.** The bucket-building kernel stamps that per-row
entry. The scan repeats on every thread of every row's threadgroup, and it grows with the
bucket count.

**An encoder that picks a kernel's guard-free instance shows that every address the instance
touches stays inside rows holding real data.** The guard-free instance is the one stamped
without the loop's bounds or tail guard. One extent dividing evenly is not that showing. A
padded chunk's walk can run past the live extent, and one poisoned read in a shared tile
corrupts real rows.

**Never leave a pipeline of dispatches with fewer scratch buffers than it has dispatches in
flight - add buffers until it has at least that many, or give each dispatch site its own.**
One shared scratch serializes the whole chain through its write-after-read hazards.

**A diff that adds dispatches to an encoder path to save bandwidth also gates that path on
work size, in the same change.** The gate's threshold is measured at both ends of the size
ladder. The small-work regression hides behind the big-work win.

**A shape claim is settled at the one site that is authoritative for that kind of constant,
never by tracing the das that computes the value.** An in-body tile constant is confirmed
literal in the generated `*_msl` global or the SPIR-V dump. A grid or threadgroup constant is
read off the class's `[metal_dispatch]` / `[vk_dispatch]` `grid=`/`tg=` spec, whose `"n/c"`
form is a CEIL-divide; the spec alone decides, and no builder read is needed. A uniform's
value is read at the single writer that fills its buffer, and nowhere upstream of it.

**A kernel twin that binds a different kargs (kernel-argument struct) type than its sibling
twin, or shifts a shared field to a different binding number, is a defect - even where one
twin ignores that field.** Kernel twins are kernel classes whose bodies differ on one stamp
axis.

**A copy-pasted kernel twin, a kernel split into hand instances where a `static_if` on a
`@template_constant` serves, and a dummy-bound field where a gate serves are defects - kernel
twins stamp one `class template`, whatever the stamp axis is (single/batch, format,
single-pass/chunked).** Body divergence rides the stamp axis - a `@template_constant`, or an
overridden method spliced flat at emission. A stamp-varying binding rides `@template_gate`.

**A diff that forks a kernel class out of a shared template shows that the bodies no longer
differ on a single stamp axis.** The same diff names the axis that is gone, in the surviving
template's comment.

**A `[metal_dispatch]` / `[vk_dispatch]` field whose memory is load-once - a model plane, or
an `upload_region` upload never written after arming - is a defect unless it carries
`@role = "weight"`, even when the kernel compiles and passes parity.** `weight` on per-encode
data - a pooled buffer the host refills each encode - is a defect too: it drops the hazard
staging. A per-encode field either omits `@role` or names the access its body performs.

**A new kernel class carries `[metal_dispatch]` / `[vk_dispatch]` with every annotation the
generated builder reads - per-field `@binding` / `@role` / `@off` / `@span` / `@default`,
`@workgroup` state with its `tgmem=` dispatch key.** A NEW hand-written `enc_*` body is a
defect unless it is a wrapper - a format or twin pick, a default-filling wrapper, or a
composite over generated builders.

**A hand-rolled bind list that dispatches a kernel in `dasllama/` or `performance/` is a
defect - dispatch through the kernel's `enc_*` builder instead.**

**A value that reaches the kernel twice device-side - a scalar bound both as a uniform buffer
and as a kargs field - is a defect.** A `params=` value that the `grid=`/`tg=` spec consumes
host-side never reaches the device, so it does not count.

**Never bind a scalar that the other bound scalars already determine - derive it in the
builder instead.** Binding it separately adds a second place to get it wrong.

**Never key a cache on a host address alone - carry the span and the form in the key too.** A
hit must cover the request, and different upload forms live in separate tables.

**Never put a backend-only capability outside its backend's file for the matching role - a
capability with no matching role gets its own role file.** The `dasllama_metal_*` and
`dasllama_vulkan_*` files partition into these roles: the kernel home (`_kernels` on Metal,
`_classes` on Vulkan), `_common` (device state and plumbing), `_decode`, `_prefill`, `_gemm`
(the Metal batch-GEMM donor backend), `_shapes` (portable servability gates), `_tower` (the
encoder-tower driver), `_asr_dec` (the ASR-decoder driver), `_seams` (the Vulkan single-op
resident-driver seams), and the kernel-access lens (`_lens` on Metal, `_dispatch` on Vulkan).
Vulkan's backend entry is `dasllama/dasllama_math_vulkan.das`, which carries the capability
probe, the arm, the `.dlim` identity source, and the `[init]` that installs every hook. Never
add a role's file to a backend that does not have the capability.

**A module that creates its own GPU device or queue is a defect - a GPU family shares the one
device and queue from `dasllama/dasllama_<gpu>_common.das`'s init.**

**Never compile or release a Metal PSO serving the engine outside the file that owns its
kernel class** - it goes through that file's init/release pair: `metal_decode_init` /
`metal_kernels_release` in `dasllama/dasllama_metal_kernels.das`, `metal_prefill_init` /
`metal_prefill_shutdown` in `dasllama/dasllama_metal_prefill.das`. A pipeline that a
per-kernel unit test under `tests/` builds for its own run is that test's to compile and
release.

**Never put race code outside the file that owns the kernel family - the shared scaffolding
(`race_buf`, `race_envelope_ok`, `race_pair_ms`) belongs to
`dasllama/dasllama_<gpu>_common.das`.** Race code is the in-engine base-vs-twin check that
times both kernels on one queue and compares their outputs.

**A string-typed Metal decline reason is a defect - a Metal decline reason is an enum value in
`dasllama/dasllama_metal_shapes.das`, one enum per driver.**

**A decline counter beside the decline site is a defect - decline counting lives in
`dasllama/dasllama_metal_common.das`.**

**A diff that adds or removes a Metal-only or Vulkan-only hook, role, served path, or
backend-only capability - anything that changes what one backend can serve and the other
cannot - lands its `ARCHITECTURE.md` sec.1.5 edit in the same change - including when
sec.1.5 already carries that class of asymmetry, and including sec.1.5's per-driver lists of
registered hooks and borrowed kernels.** sec.1.5 is the closed list; an asymmetry it does not
carry does not exist.

**A hand-written Vulkan pipeline build anywhere in the engine is a defect - a Vulkan pipeline
is created only by a `[vk_dispatch]`-generated `ensure_*` and torn down by
`vk_drop_model_state`.**

**Never size a buffer bound as one SSBO range above `vk_max_storage_range()` - check the size
where it is NEGOTIATED, not where it binds.** The bind site cannot shrink a buffer that was
sized wrong.

**A change to code that a served GPU decode or prefill path executes ships GPU-vs-CPU parity
on one q8 and one kq model with the armed mirror codec.** That code is a driver
(`dasllama/dasllama_metal_decode.das`, `dasllama/dasllama_metal_prefill.das`,
`dasllama/dasllama_vulkan_decode.das`, `dasllama/dasllama_vulkan_prefill.das`), a kernel
class one of them dispatches, that class's builder, the servability gates
(`dasllama/dasllama_metal_shapes.das`), the weight-region cache and residency paths
(`dasllama/dasllama_metal_common.das`), or the residency rail's serving paths
(`dasllama/dasllama_gpu_resident.das`); never the bake paths, never a comment. The parity run
is `harness/parity.das`, or the in-suite instruments `tests/test_metal_decode_parity.das` /
`tests/test_metal_prefill_parity.das` through `tests/run.das`.

**A `harness/parity.das` run arms its backend: the Metal arm is `--ngl`; the Vulkan arm is
`DASLLAMA_GPU=1`, never `--ngl`, and its log shows `resident driver armed`.** The Vulkan
driver declines codec-mismatched sessions silently.

**A change to `dasllama/dasllama_metal_tower.das`, to the `AttnArgs` kargs struct, to any
kernel class the tower dispatches or builder the tower borrows, or to state the whole
driver shares (a module-level `g_tw_*` variable, `metal_tower_init`,
`dasllama_metal_tower_register` - reachable from every hook) runs the gate of every
registered tower hook the changed code is reachable from.** The gates are the family gates
`tests/test_gemma4uv.das`, `tests/test_gemma4v.das`, `tests/test_gemma3v.das`, and
`test_qwen3v_tier1_metal` in `tests/test_qwen3v.das`; the encoder-blocks leg's
`tests/test_whisper.das`; the conv legs' `tests/test_audio.das` and
`tests/test_audio_embedder.das`; plus a `tests/test_model_image.das` run with the `mtower`
arm, with `metal_tower_stats()`'s encode count rising across the run. A hook registered in
`dasllama_metal_tower_register` that this rule's gates do not cover is the rule's defect to
fix in the same change.

**A change to the bake-trim path in `dasllama/dasllama_gpu_resident.das` (`trim_model_planes`)
ships a `dasllama-convert --trim` bake plus a serve of the trimmed image, on one q8 and one kq
model.** Parity runs never reach it.

**A change to `dasllama/dasllama_metal_asr_dec.das`, to `dasllama/dasllama_metal_common.das`,
or to any kernel class the ASR decoder dispatches or builder the ASR decoder borrows, ships
a `tests/test_model_image.das` run with the `mtower` arm** - its CPU-vs-GPU transcript cells
are the ASR-decoder driver's parity instrument, and the shared common paths and borrowed
kernels reach that driver with no line of its own file touched.

**Never leave a K/V codec unserved by the kernels that read or write the residency rail's
`k_mirror`/`v_mirror` slabs - a K/V codec is the mirror's element type, f16 or f32.** Two
shapes serve both: instances of one template cover both codecs, or a single-codec kernel has
a sibling that serves the other codec behind an arming gate that keys on `kv16`. The rail
serves both codecs, so a codec no kernel covers silently drops that codec's GPU path.

**An f16 store into any GPU-resident K/V that does not clamp to the f16 finite range
(+/-65504) is a defect.**

**A resident override that touches the mirror before gating the session on the armed mirror
codec and on the flat (non-paged) cache is a defect** - a resident override is a
decode/prefill hook the whole-model residency rail registers in
`dasllama/dasllama_common.das`'s override registries. An override that byte-copies mirror
bytes across codecs is a defect too: bytes move only between same-codec session rows and
mirror rows, and a cross-codec copy corrupts the host authority.

**Never cache a descriptor set across dispatches in state `vk_drop_model_state` does not
clear** - put it in a `*_ready` latch, or in a field inside `g_gpu` or the weight arena in
`dasllama/dasllama_vulkan_common.das`.

**A diff that changes anything a hand-binding arm must mirror to dispatch a kernel - binding
numbers, kargs layout, threadgroup memory, grid or threadgroup geometry - fixes or deletes,
in the same change, every arm of a hand-binding lab that binds it** - a hand-binding lab is a
kernel A/B or knockout timing script, wherever it lives (`benchmarks/`, `harness/`), that
hand-lists its bindings instead of dispatching through the `enc_*` builder. A lab left
dispatching stale geometry measures the wrong kernel silently.

**A diff that ports an A/B lab's winning variant into a kernel deletes the ported arm in the
same change - its variant class and any variants-module code that exists only for it; when
the lab exists only for that decision, its driver and remaining arm go too.** An A/B lab is a
timing script whose output SELECTS between implementations of the same compute, wherever it
lives (`benchmarks/`, `harness/`); a decided arm that outlives its decision degrades into an
unmaintained duplicate of the kernel it seeded.
