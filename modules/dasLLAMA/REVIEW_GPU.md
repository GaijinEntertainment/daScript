# dasLLAMA GPU Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md`.

**Routed from `REVIEW.md`: a diff touching a GPU kernel, driver, dispatch class, or the K/V
mirrors applies this list with the master's.**

**The EMITTED shader contains no indirection.** No function pointers, no vtables. A
`class template` / `def abstract` / `def override` splice is compile-time and conforms —
check the emission, not the das spelling.

**The `*_decline_caps` predicates take only the model and the call shape; window-setup state
is asked by `prefill_decline` / `decode_decline`, never by a caps predicate.** A caps parameter that
reports the session's setup progress — rather than the CALL, its row count or its span
shape — is a defect however it is derived.

**A claim about a shape constant is checked against what the kernel is compiled and dispatched
with, never against the das that computes the value.** An in-body tile constant is confirmed
literal in the generated `*_msl` global or the SPIR-V dump; a grid or threadgroup constant is
confirmed in both the class's `[metal_dispatch]` / `[vk_dispatch]` `grid=`/`tg=` spec and the
generated `enc_*` builder.

**Kernel twins — kernel classes whose bodies differ on one stamp axis — bind the same kargs
(kernel-argument struct) type at the same binding numbers**, even where one twin ignores a
field; shifting the other twin's fields to different slots is a defect.

**Kernel twins share a template.** Two kernel classes whose bodies differ on one stamp axis
are twins, whatever the axis (single/batch, format, single-pass/chunked); they stamp one
`class template`: body divergence rides a stamp axis (`@template_constant`, or an overridden
method spliced flat at emission), a stamp-varying binding rides `@template_gate`. A
copy-pasted twin, or a dummy-bound field where a gate serves, is a defect.

**A `[metal_dispatch]` / `[vk_dispatch]` field carries `@role = "weight"` exactly when its
memory is load-once — a model plane, or an `upload_region` upload never written after
arming.** A load-once field with no `@role` is a defect even when the kernel compiles and
passes parity; `weight` on per-encode data — a pooled buffer the host refills each encode —
is one too, it drops the hazard staging. A per-encode field either omits `@role` or names the
access its body performs.

**A kernel declares its dispatch on the class; the builder is generated.** A new kernel class
carries `[metal_dispatch]` / `[vk_dispatch]` with per-field `@binding` / `@role` / `@off` /
`@span` / `@default`. A NEW `enc_*` body is hand-written only as a wrapper — a format or twin
pick, a default-filling wrapper, or a composite over generated builders; any other
hand-written `enc_*` body is a defect.

**A kernel is dispatched only through its `enc_*` builder** — a hand-rolled bind list in
`dasllama/` or `performance/` is a defect.

**No value reaches an encoder twice DEVICE-side.** A scalar the kernel receives twice — as a
uniform buffer and as a kargs field — or that the other bound scalars determine, is a defect;
a `params=` value the `grid=`/`tg=` spec consumes host-side never reaches the device and does
not count.

**A cache keyed by a host address carries the span and the form in its key.** A hit must cover
the request, and different upload forms live in separate tables.

**A backend-only capability goes in that backend's file for the matching role** — the
per-role files every backend has a twin of: the kernel home, `_common` (device state and
plumbing), `_decode`, `_prefill`, `_shapes` (portable servability gates), and the
kernel-access lens. A capability with no matching role gets its own role file; anything else
is a grab-bag, and a grab-bag file is a defect.

**A GPU family shares ONE device and queue from `dasllama/dasllama_<gpu>_common.das`'s
init.** A module creating its own is a defect.

**A Metal PSO serving the engine is compiled and released by the file that owns its kernel
class**, through its init/release pair — `metal_decode_init` / `metal_kernels_release` in
`dasllama/dasllama_metal_kernels.das`, `metal_prefill_init` / `metal_prefill_shutdown` in
`dasllama/dasllama_metal_prefill.das`. A kernel-unit gate's short-lived pipeline is its own.

**Race code — the in-engine base-vs-twin check that times both kernels on one queue and
compares their outputs — lives in the file that owns the kernel family**:
`dasllama/dasllama_metal_kernels.das` races its families, `dasllama/dasllama_metal_prefill.das`
its own; the shared scaffolding (`race_buf`, `race_envelope_ok`, `race_pair_ms`) is
`dasllama/dasllama_<gpu>_common.das`'s.

**A Metal decline reason is an enum value in `dasllama/dasllama_metal_shapes.das`, one enum
per driver.** A string-typed metal decline is a defect.

**Decline counting lives in `dasllama/dasllama_metal_common.das`.** A counter beside the
decline site is a defect.

**A diff that adds or removes a Metal-only or Vulkan-only hook, role, or served path lands its
`ARCHITECTURE.md` §1.5 edit in the same change — including when §1.5 already carries that class
of asymmetry, and including §1.5's per-driver lists of registered hooks and borrowed kernels.**
§1.5 is the closed list; an asymmetry it does not carry does not exist.

**A Vulkan pipeline is created only by a `[vk_dispatch]`-generated `ensure_*` and torn down by
`vk_drop_model_state`.** A hand-written pipeline build anywhere else in the engine is a defect.

**A buffer bound as one SSBO range stays under `vk_max_storage_range()`, checked where its size
is NEGOTIATED, not where it binds.** The bind site cannot shrink a buffer that was sized wrong.

**A change to a GPU decode or prefill driver — `dasllama/dasllama_metal_decode.das`,
`dasllama/dasllama_metal_prefill.das`, `dasllama/dasllama_vulkan_decode.das`,
`dasllama/dasllama_vulkan_prefill.das`, the servability gate that admits models to them
(`dasllama/dasllama_metal_shapes.das`), or the resident rail in
`dasllama/dasllama_gpu_resident.das` (`resident_upload`, `resident_plan`, and the decode,
batch-decode and prefill overrides it registers) — ships with `harness/parity.das` GPU-vs-CPU
runs on one q8 and one kq model, with `--kv` matching the armed mirror codec.** The Metal arm is
`--ngl`; the vulkan arm is `DASLLAMA_GPU=1`, never `--ngl`, and its driver declines
codec-mismatched sessions silently, so that log must show `resident driver armed`.

**A change to the tower driver `dasllama/dasllama_metal_tower.das` — or to the `AttnArgs`
kargs struct, or to any kernel class dispatched through `enc_qk_mm`, `enc_softmax`,
`enc_av_mm`, `pf_enc_bf16_mm`, or `enc_add_bias_rows` — ships two runs: the per-family GPU
oracle gates `tests/test_gemma4uv.das`, `tests/test_gemma4v.das`, and `tests/test_gemma3v.das`,
for every family the changed code is reachable from and all three when the change touches
state or setup the whole driver shares rather than one family's path (any module-level
`g_tw_*` variable in that file, `metal_tower_init`, or `dasllama_metal_tower_register`); and
a `tests/test_model_image.das` run with the `mtower` arm, with `metal_tower_stats()`'s encode
count rising across the run.** The image run is the parity instrument for the families no
per-family gate file covers.

**A diff that adds a tower family the Metal tower driver serves — one whose bring-up
registers a `*_gpu` hook in `dasllama/dasllama_metal_tower.das` — or gives the tower driver a
newly borrowed kernel class or kargs struct, adds that family's per-family GPU oracle gate
file, or that borrowed name, in the same change, to the trigger of the rule in this
checklist that requires the per-family GPU oracle gates and the `mtower` image run.**

**A change to the bake-trim path in `dasllama/dasllama_gpu_resident.das` (`trim_model_planes`)
ships a `dasllama-convert --trim` bake plus a serve of the trimmed image, on one q8 and one kq
model.** Parity runs never reach it.

**A change to `dasllama/dasllama_metal_asr_dec.das` ships a `tests/test_model_image.das` run
with the `mtower` arm** — its CPU-vs-GPU transcript cells are that driver's parity instrument.

**A kernel that reads or writes the residency rail's `k_mirror`/`v_mirror` slabs leaves
neither codec unserved: it is stamped from a `[|> template_struct_instance]` codec template
(`typedef KT`) with both f32 and f16 instances, or it is single-codec and a sibling stamped
from that template serves the other codec behind an arming gate that keys on `kv16`.** The
rail serves both codecs, so a codec no kernel covers silently drops that codec's GPU path.

**An f16 store into any GPU-resident K/V clamps to the f16 finite range (±65504).**

**Every resident override — a decode/prefill hook the whole-model residency rail registers in
common's override registries — gates sessions on the armed mirror codec and on the flat
(non-paged) cache before touching the mirror.** Mirror bytes move only between same-codec
session rows and mirror rows; an override that byte-copies across codecs corrupts the host
authority.

**A descriptor set cached across dispatches lives in state `vk_drop_model_state` clears** — a
`*_ready` latch, or a field inside `g_gpu` or the weight arena in
`dasllama/dasllama_vulkan_common.das`.

**A diff that changes a kernel's binding numbers or the layout of a kargs struct it binds
fixes or deletes, in the same change, every arm of a hand-binding lab that binds it** — a
hand-binding lab is a kernel A/B or knockout timing script, wherever it lives (`benchmarks/`,
`harness/`), that hand-lists its bindings instead of dispatching through the `enc_*` builder.
A lab left dispatching stale bindings measures the wrong kernel silently.

**A diff that ports an A/B lab's winning variant into a kernel deletes that lab in the same
change — its bench driver, both of its arm variants, and any variants-module code that exists
only for it.** An A/B lab is a timing script whose output SELECTS between two implementations
of the same compute, wherever it lives (`benchmarks/`, `harness/`); one that outlives its
decision degrades into an unmaintained one-off measurement script.
