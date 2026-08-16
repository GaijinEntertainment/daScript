# dasLLAMA GPU rules

**Routed from `REVIEW.md`: a diff touching a GPU kernel, driver, dispatch class, or the K/V
mirrors applies this list with the master's.** `REVIEW_COMMON.md` (repo root) binds this file
too. Architecture doc: `ARCHITECTURE.md`.

**A claim about a shape constant is checked against the emitted shader, not the das source.**
Read the generated `*_msl` global or the SPIR-V dump and confirm the constant is literal there.

**Twins of a kernel family bind the same kargs type at the same binding**, even where one twin
ignores a field; shifting the other's fields to different slots is a defect.

**Kernel twins share a template.** Same-body single/batch or format twins stamp one
`class template`: body divergence rides a stamp axis (`@template_constant`, or an overridden
method spliced flat at emission), a stamp-varying binding rides `@template_gate`. A
copy-pasted twin, or a dummy-bound field where a gate serves, is a defect.

**Every weight, bias, or lookup-table field on a `[metal_dispatch]` / `[vk_dispatch]` kernel
class declares `@role = "weight"` explicitly.** An un-roled weight-shaped field is a defect even
when the kernel compiles and passes parity.

**A kernel declares its dispatch on the class; the builder is generated.** A new kernel class
carries `[metal_dispatch]` / `[vk_dispatch]` with per-field `@binding` / `@role` / `@off` /
`@span` / `@default`. A NEW hand-written `enc_*` body is a defect unless it is a wrapper shape:
a format or twin pick, a default-filling wrapper, or a composite over generated builders.

**A kernel is dispatched only through its `enc_*` builder** — a hand-rolled bind list in
`dasllama/` or `performance/` is a defect.

**No value reaches an encoder twice DEVICE-side.** A scalar uniform buffer passed alongside the
identical value as a parameter is a defect, as is a kargs field the fields beside it determine;
a builder parameter the `grid=`/`tg=` spec consumes host-side never arrives at the device.

**A cache keyed by a host address carries the span and the form in its key.** A hit must cover
the request, and different upload forms live in separate tables.

**A backend-only capability goes in that backend's matching role file.** A capability with no
matching role gets its own role file; anything else is a grab-bag, and a grab-bag file is a
defect.

**A GPU family shares ONE device and queue from `<gpu>_common`'s init.** A module creating its
own is a defect.

**A PSO serving the engine is compiled and released by the file that owns its kernel class**,
through its init/release pair — `metal_decode_init` / `metal_kernels_release` in
`dasllama_metal_kernels.das`, `metal_prefill_init` / `metal_prefill_shutdown` in
`dasllama_metal_prefill.das`. A kernel-unit gate's short-lived pipeline is its own.

**Race code for a kernel family lives beside the family** — kernels races its families, prefill
races its own; the shared scaffolding (`race_buf`, `race_envelope_ok`, `race_pair_ms`) is
`<gpu>_common`'s.

**A Metal decline reason is an enum value in `dasllama_metal_shapes.das`, one enum per
driver.** A string-typed metal decline is a defect.

**Decline counting lives in `dasllama_metal_common.das`.** A counter beside the decline site
is a defect.

**A diff that changes how Metal and Vulkan differ — adding or removing an asymmetry — lands its
`ARCHITECTURE.md` §1.5 edit in the same change.** §1.5 is the closed list; an asymmetry it does
not carry does not exist.

**A pipeline is created only by a `[vk_dispatch]`-generated `ensure_*` and torn down by
`vk_drop_model_state`.** A hand-written pipeline build anywhere else in the engine is a defect.

**A buffer bound as one SSBO range stays under `vk_max_storage_range()`, checked where its size
is NEGOTIATED, not where it binds.** The bind site cannot shrink a buffer that was sized wrong.

**A change to `dasllama_metal_decode.das`, `dasllama_metal_prefill.das`,
`dasllama_gpu_resident.das`, `dasllama_vulkan_decode.das`, or `dasllama_vulkan_prefill.das`
ships with `harness/parity.das` GPU-vs-CPU runs on one q8 and one kq model, with `--kv`
matching the armed mirror codec.** The Metal arm is `--ngl`; the vulkan arm is
`DASLLAMA_GPU=1`, never `--ngl`, and its driver declines codec-mismatched sessions silently,
so that log must show `resident driver armed`.

**A kernel that reads or writes the K/V mirrors is stamped from a
`[|> template_struct_instance]` codec template (`typedef KT`) with both f32 and f16
instances.** A single-codec mirror kernel is legal only when a codec-templated sibling serves
the other codec and its arming gate keys on `kv16`.

**An f16 instance's mirror stores clamp to the f16 finite range.**

**Every resident override gates sessions on the armed mirror codec and on the flat (non-paged)
cache before touching the mirror.** Mirror bytes move only between same-codec session rows and
mirror rows; an override that byte-copies across codecs corrupts the host authority.

**A descriptor set cached across dispatches lives in state `vk_drop_model_state` clears** — a
`*_ready` latch or a field inside `g_gpu` / the arena.
