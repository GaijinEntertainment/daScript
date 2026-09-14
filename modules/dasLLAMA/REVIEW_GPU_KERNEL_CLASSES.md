# dasLLAMA GPU Kernel Class Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_VULKAN.md`. Planned work: `followup_metal.md`
for Metal, `followup_vulkan.md` for Vulkan.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

**A kernel twin that binds a different kargs (kernel-argument struct) type than its sibling
twin, or shifts a shared field to a different binding number, is a defect - even where one
twin ignores that field.** Kernel twins are kernel classes whose bodies differ on one stamp
axis - one compile-time choice, such as single/batch, format, or single-pass/chunked.

**A copy-pasted kernel twin, or a kernel split into hand instances where a `static_if` on a
`@template_constant` serves, is a defect - kernel twins stamp one `class template`.** Body divergence is carried by a `@template_constant`, or by an overridden
method spliced flat at emission.

**A kernel-family stamp - one stamp of a class template, or one of the classes deriving from a
base shell that carry a `[vk_dispatch]` / `[metal_dispatch]` - that binds a real buffer to a
binding whose fields its compiled body, inherited code included, never reads is a defect: gate
the field with `@template_gate` where a template constant decides it, and where the family
shares one set layout on purpose, name that case in `ARCHITECTURE_GPU.md` sec.1.5's ledgered
kernel-binding asymmetries, for either backend.** A binding counts as read when the compiled
body reads any field declared on it - fields in the stamp or in the shell may share a binding,
`@role = "alias"` marks such a view - including a field read only under a run-time flag.

**A diff that forks a kernel class out of a shared template shows, in the forked class's
generated source (its `*_msl` global, or the SPIR-V dump), that its body no longer differs
from its former siblings' on the compile-time choice the template carried, and names that
choice on the forked class's `[metal_dispatch]` / `[vk_dispatch]` declaration.**

**A `[metal_dispatch]` / `[vk_dispatch]` binding whose memory is never written after arming at
every site that binds it is a defect unless a field at that binding carries `@role = "weight"`.**
A field the kernel reads under a run-time flag takes the role of its read arm.

**`@role = "weight"` on per-encode data the kernel reads - a pooled buffer the host refills
each encode - is a defect; a per-encode field either omits `@role` or names the access its body
performs.** `weight` tells the generated builder the buffer needs no per-encode hazard tracking.

**A diff that adds a GPU kernel class under `dasllama/` - a `[metal_kernel]` def, a
`[vk_dispatch]` declaration, or a new instance of a template carrying one - either shows a
census row in `tests/test_kernel_coverage.das` that dispatches it, adding the row or the census
model when none does, or names it in that file's blind-spot list for its backend -
`CENSUS_NEVER_DISPATCHED` for Metal, `VK_CENSUS_NEVER_DISPATCHED` for Vulkan - with the reason no
stocked model reaches it and the model-less test cell that dispatches it.**

**Weakening a refusal the `[metal_dispatch]` / `[vk_dispatch]` lens makes at compile time - an
`@ssbo` field with no `@binding`, an unaccessed `@ssbo` field declaring no `@role`, a
`[vk_dispatch]` `@readonly` field on a binding a kernel of its class writes, a `@workgroup` field
with no `tgmem=` spec - or weakening any test cell that holds such a refusal
(`test_lens_tgmem_gate` and `test_lens_requires_gate` in `tests/test_metal_misc_kernels.das`,
`test_vkd_lens_readonly_gate` in `tests/test_vulkan_kernels.das`), is a defect.**

**A kernel field carries `@span` only when every caller binds whole output rows.** A caller
binding a column tile of a wider row would leave the rest of each row outside the tracked
hazard range.

**A NEW hand-written `enc_*` body that binds a buffer or a kargs field itself is a defect -
declare the class so the `[metal_dispatch]` / `[vk_dispatch]` lens generates the builder.** A
body that only picks, defaults or composes generated builders binds nothing.

**A hand-rolled bind list on a dispatch that serves a user call, in `dasllama/` or
`performance/`, is a defect: dispatch through the kernel's `enc_*` builder instead.**

**A value that reaches the kernel twice device-side - a scalar bound both as a uniform buffer
and as a kargs field - is a defect: bind it once, as a kargs field.** A `params=` value that the `grid=`/`tg=` spec consumes
host-side never reaches the device, so it does not count.

**Never bind a scalar that the other bound scalars already determine - derive it in the
builder instead.** Binding it separately adds a second place to get it wrong.
