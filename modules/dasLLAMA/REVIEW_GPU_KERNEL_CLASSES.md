# dasLLAMA GPU Kernel Class Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_VULKAN.md`. Planned work: `followup_metal.md`
for Metal, `followup_vulkan.md` for Vulkan.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

Two kernel classes are twins when one body serves both: their compiled bodies differ only on an
axis one value fixes - a template constant, a typedef, which base shell's method they inherit, or
a run-time count of live entries inside a fixed extent (a column count, a row count). A base
shell is the dispatch-less base class whose methods the emitter splices flat into each deriving
class.

**A kernel twin that binds a different kargs (kernel-argument struct) type than its sibling
twin, or shifts a shared field to a different binding number, is a defect - even where one
twin ignores that field.**

**A kernel class whose body differs from a sibling's only on such an axis is a defect: twins
stamp one `class template`, derive from one base shell, or - where the axis is a run-time count -
share one class whose body reads the count from its kargs.** Body divergence is carried by a
`@template_constant`, by an overridden method spliced flat at emission, or by a run-time value
the builder passes.

**Two kernel bodies that compile to separate shader modules and that a test cell
(`tests/test_vulkan_kernels.das`) or a regions file (`tests/test_gpu_resident_regions_*.das`)
holds bit for bit against each other spell every multiply that feeds an add as `mad` in both, on
the path the compare covers.** A driver decides per shader module whether to contract a
multiply-add into one fma, so two bodies spelled alike round a ulp apart on a driver that
contracts one and not the other; `mad` is the fused instruction by definition and leaves the
driver nothing to choose.

**A value that is the same on every dispatch a compiled kernel's pipeline serves - a tile width
the class fixes is, a dimension the model sets is not - never reaches that kernel as a uniform, a
kargs field, or an `@off` bind offset: stamp it into the class as a `@template_constant`.**

**A stamp - a kernel class that compiles to a shader module, standalone, a template instance or
a base-shell derivative - sets only `@template_constant`s its own body resolves at compile time: a `static_if` arm, a
`@template_gate`, a value select, an array extent.** A constant no such site reads is a defect -
move it to the template whose body reads it, or make the body read it.

**A diff that changes a stamp's generated source - through the class's own body, the template or
base shell it stamps, or a helper its body splices - carries in the PR body, for each affected
stamp, its generated source diffed against the pre-change tree (the `*_msl` global, or the `.spv`
files `DASLLAMA_VK_SPV_DUMP=<dir>` writes).** The evidence is one of three: an empty diff; the
difference named with the compile-time choice that carries it; or the behaviour change named with
the test cell that pins it.

**A kernel-family stamp - one stamp of a class template, or one of the classes deriving from a
base shell that carry a `[vk_dispatch]` / `[metal_dispatch]` - that binds a buffer to a
binding whose fields its compiled body, inherited code included, never reads is a defect: gate
the field with `@template_gate` where a template constant decides it, and where the family
shares one set layout on purpose, name that case in `ARCHITECTURE_GPU.md` sec.1.5's ledgered
kernel-binding asymmetries.** A binding counts as read when the compiled body reads any field
declared on it - fields in the stamp or in the shell may share a binding, `@role = "alias"` marks
such a view - including a field read only under a run-time flag.

**A forked kernel class carries a `//!` line above its `[metal_dispatch]` / `[vk_dispatch]`
declaration naming the body difference that keeps it out of its former siblings' template.**

**A `[metal_dispatch]` / `[vk_dispatch]` binding that no site writes after arming - a binding
filled before the first encode and never written again - carries `@role = "weight"` on a field at
that binding.**

**`@role = "weight"` on per-encode data the kernel reads - a pooled buffer the host refills
each encode - is a defect; a per-encode field either omits `@role` or names the access its body
performs.** `weight` tells the generated builder the buffer needs no per-encode hazard tracking.

**A diff that adds a GPU kernel class under `dasllama/` - a `[metal_kernel]` def, a
`[vk_dispatch]` declaration, or a new instance of a template carrying one - that a census row of
`modules/dasLLAMA/tests/test_kernel_coverage.das` dispatches names in the PR body that row and its
nonzero count for the new census key, from a census run of the class's backend.**

**A diff that adds a GPU kernel class under `dasllama/` that no census row dispatches, where a
census row could dispatch it - a model the census file can load, run the way the census runs it -
adds that row, or that model, to `modules/dasLLAMA/tests/test_kernel_coverage.das` in the same
change.**

**A diff that adds a GPU kernel class under `dasllama/` that no census row could dispatch - the
model, the quant or the load shape sits outside what the census runs - names it in the blind-spot
list of `modules/dasLLAMA/tests/test_kernel_coverage.das` for its backend -
`CENSUS_NEVER_DISPATCHED` for Metal, `VK_CENSUS_NEVER_DISPATCHED` for Vulkan - with the reason no
census row reaches it, the stocked model that dispatches it where one does - one the `stocked`
suite runs on a box that has it - and the model-free test cell that dispatches it.** The list is
asserted: a row that dispatches a listed class reds the census.

**Weakening the blind-entry asserts in `modules/dasLLAMA/tests/test_kernel_coverage.das` - that
an entry matches a compiled census key, and that it matches no dispatched one - is a defect.**

**Weakening a refusal the `[metal_dispatch]` / `[vk_dispatch]` lens makes at compile time - an
`@ssbo` field with no `@binding`, an unaccessed `@ssbo` field declaring no `@role`, a
`[vk_dispatch]` `@readonly` field on a binding a kernel of its class writes, a `[metal_dispatch]`
`@workgroup` field with no `tgmem=` spec, a `[metal_dispatch]` `requires=` item that is not
`<lhs> % <int>`, a `stamp =` naming no family and form, a `compile_stamp` / `race_pso_pair_stamp`
naming a source other than the class's `*_msl` global, an empty `release_handles` - or weakening
any test cell that holds such a refusal (`test_lens_tgmem_gate`, `test_lens_requires_gate`, `test_lens_stamp_gate` and
`test_lens_call_macro_gates` in `modules/dasLLAMA/tests/test_metal_misc_kernels.das`,
`test_vkd_lens_readonly_gate` in `modules/dasLLAMA/tests/test_vulkan_kernels.das`), is a
defect.** A refusal replaced by a derivation that leaves no such configuration compiling unbound -
the `stamp =` form's threadgroup-memory global - is not a weakening, and the test cell then holds
the derived path.

**A kernel field carries `@span` only when every caller binds whole output rows.** A caller
binding a column tile of a wider row would leave the rest of each row outside the tracked
hazard range.

**A hand-written encode or descriptor-set helper, or a hand-rolled bind list on a dispatch, that a
diff adds anywhere - a buffer or kargs field bound by literal number instead of through the
builder the `[metal_dispatch]` / `[vk_dispatch]` lens generates for that class - whose PR body does not
state why the generated builder cannot serve that site is a defect.** A body that only picks,
defaults or composes generated builders binds nothing.

**A value that reaches the kernel twice device-side - a scalar bound both as a uniform buffer
and as a kargs field - is a defect: bind it once, as a kargs field.** A `params=` value that the
`grid=`/`tg=` spec consumes host-side never reaches the device, so it does not count.

**A `params=` value that no `grid=`/`tg=` spec and no `requires=` item consumes is dropped from
the `params=` spec and from every call site, in the same change.** The value then reaches neither
the host nor the device, so nothing reads it.

**Never bind a scalar that the other bound scalars already determine - derive it in the
builder instead.** Binding it separately adds a second place to get it wrong.

**A kernel-class method a compiled body calls in value position - inside an expression rather
than as its own statement - returns its value in one statement after compile-time folding: an
arrow form (`=>`), or a `static_if` whose every arm is one `return`; a method that needs more
than one statement hands its value back through a `var T&` parameter instead.** The emitter
splices a value-position method as one expression, so a body that folds to more than one
statement reaches the kernel as a statement and its value never arrives.
