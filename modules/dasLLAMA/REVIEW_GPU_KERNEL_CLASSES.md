# dasLLAMA GPU Kernel Class Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_GPU.md`. Planned work: `followup_metal.md` for Metal, `followup_vulkan.md`
for Vulkan.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

Two kernel classes are twins when one body serves both: their compiled bodies differ only on an
axis one value fixes - a template constant, a typedef, which base shell's method they inherit, or
a run-time count of live entries inside a fixed extent (a column count, a row count). Two classes
with bodies of their own that share a base shell's method are not twins. A base shell is the
dispatch-less base class whose methods the emitter splices flat into each deriving class.

**A kernel twin that binds a different kargs (kernel-argument struct) type than its sibling
twin, or shifts a shared field to a different binding number, is a defect - even where one
twin ignores that field; a field a `@template_gate` omits on one twin is a shared field still,
and keeps the number the other twin binds it at.**

**A kernel class whose body differs from a sibling's only on such an axis is a defect: twins
stamp one `class template`, derive from one base shell, or - where the axis is a run-time count -
share one class whose body reads the count from its kargs.** Body divergence is carried by a
`@template_constant`, by an overridden method spliced flat at emission, or by a run-time value
the builder passes.

**Two kernel bodies that compile to separate shader modules and that any cell under
`modules/dasLLAMA/tests/` holds bit for bit against each other spell as `mad` in both, on the path
the compare covers, every multiply that feeds an add and that is not one product both bodies take
from one shared method text.** A driver decides per shader module whether to contract a
multiply-add into one fma, so two bodies spelled alike round a ulp apart on a driver that
contracts one and not the other; `mad` is the fused instruction by definition and leaves the
driver nothing to choose. The shared text is one source in both modules, and a `mad` there would
be a fused multiply-add the CPU oracle's `mad` - a multiply, then an add - does not reproduce.

**A value that no model file and no request can change - a tile width, a math constant, a cap
fixed by the model architecture the class serves - never reaches a kernel class through a
per-dispatch argument channel (a uniform, a `@push_constant` field, a kargs field, an `@off` bind
offset): stamp it into the class as a `@template_constant` where the class's stamps differ on it,
or write it as a literal in the body.** A Metal kernel body reads no module constant - the MSL
emitter refuses a global name.

**A stamp - a kernel class that compiles to a shader module, standalone, a template instance or
a base-shell derivative - sets only `@template_constant`s its own body resolves at compile time: a
`static_if` arm, a `@template_gate`, a value select, an array extent.** A constant no such site
reads is a defect - move it to the template whose body reads it, or make the body read it.

**A diff that changes an existing stamp's generated source - through the class's own body, the
template or base shell it stamps, or a helper its body splices - carries in the PR body, for each
affected stamp, its generated source diffed against the pre-change tree (the `*_msl` global, or the
`.spv` files `DASLLAMA_VK_SPV_DUMP=<dir>` writes).** The evidence is one of four: an empty diff; a
difference confined to whitespace, scoping braces, parentheses or identifier names, with every
changed line paired against its pre-change line in the PR body; the difference named with the
compile-time choice that carries it; or the behaviour change named with the test cell that pins it.

**A kernel-family stamp - one stamp of a class template, or one of the classes deriving from a
base shell that carry a `[vk_dispatch]` / `[metal_dispatch]` - that binds a buffer to a
binding whose fields its compiled body, inherited code included, never reads or writes is a
defect: gate the field with `@template_gate` where a template constant decides it, and where the
family shares one set layout on purpose, name that case in `ARCHITECTURE_GPU.md` sec.1.5's
ledgered kernel-binding asymmetries.** A binding counts as used when the compiled body reads or
writes any field declared on it - fields in the stamp or in the shell may share a binding,
`@role = "alias"` marks such a view - including a field touched only under a run-time flag.

**A diff that moves a kernel class out of the class template its siblings stamp, or off the base
shell they derive from, gives the class a `//!` line above its `[metal_dispatch]` /
`[vk_dispatch]` declaration naming the body difference that keeps it out of that template or
shell.**

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

**A diff that adds a GPU kernel class under `dasllama/` that no census row of the class's backend
dispatches, where such a row could dispatch it - a model the census file loads for that backend,
run the way the census runs it - adds that row, or that model, to
`modules/dasLLAMA/tests/test_kernel_coverage.das` in the same change.**

**A diff that adds a GPU kernel class under `dasllama/` that no census row of the class's backend
could dispatch - the model, the quant or the load shape sits outside what the census runs -
names it in the blind-spot list of `modules/dasLLAMA/tests/test_kernel_coverage.das` -
`CENSUS_NEVER_DISPATCHED` for Metal, `VK_CENSUS_NEVER_DISPATCHED` for Vulkan - with the reason no
census row reaches it, the stocked model that dispatches it where one does - one the `stocked`
suite runs on a box that has it - and the model-free test cell that dispatches it.** The list is
asserted: a row that dispatches a listed class reds the census.

**Weakening the blind-entry asserts in `modules/dasLLAMA/tests/test_kernel_coverage.das` - that
an entry matches a compiled census key, and that it matches no dispatched one - is a defect.**

**Weakening any refusal the `[metal_dispatch]` / `[vk_dispatch]` lens makes at compile time, or
any `test_lens_*` / `test_vkd_lens_*` cell that holds one, is a defect** - the cells sit in
`modules/dasLLAMA/tests/test_metal_misc_kernels.das` for Metal and
`modules/dasLLAMA/tests/test_vulkan_kernels.das` for Vulkan.

**A diff that adds a refusal to the `[metal_dispatch]` / `[vk_dispatch]` lens lands the cell that
holds it in the same change** - a `test_lens_*` cell in
`modules/dasLLAMA/tests/test_metal_misc_kernels.das` for Metal, a `test_vkd_lens_*` cell in
`modules/dasLLAMA/tests/test_vulkan_kernels.das` for Vulkan.

**A diff that replaces a refusal with a derivation - the lens computing the value it used to
demand, so no class can compile with that value missing - points that refusal's cell at the
derived path in the same change.** Deriving the value is not a weakening.

**A kernel field carries `@span` only when every caller binds whole output rows.** A caller
binding a column tile of a wider row would leave the rest of each row outside the tracked
hazard range.

**A hand-written encode or descriptor-set helper, or a hand-rolled bind list on a dispatch, that a
diff adds anywhere - a buffer or kargs field bound by literal number instead of through the builder
the `[metal_dispatch]` / `[vk_dispatch]` lens generates for that class - whose PR body does not
state why the generated builder cannot serve that site is a defect.** A body that only picks,
defaults or composes generated builders binds nothing.

**A value that reaches the kernel twice device-side - a scalar bound both as a uniform buffer
and as a kargs field - is a defect: bind it once, as a kargs field.** A `params=` value consumed
only host-side - by the `grid=` / `tg=` spec, a `requires=` item, or an `@span` - never reaches
the device, so it does not count.

**Never bind a scalar that the other bound scalars already determine by integer arithmetic -
derive it in the kernel body from them instead.** Binding an integer count separately adds a
second place to get it wrong.

**A kernel-class method whose returned value is used anywhere but as the whole right-hand side
of a `let` or an assignment - an argument, an operand, a subscript, a `return`'s value -
returns its value in one statement after compile-time folding: an arrow form (`=>`), or a
`static_if` whose every arm is one `return`; a method that needs more than one statement hands
its value back through a `var T&` parameter instead.** A kernel class compiles on both emitters
and as its own CPU oracle, and only a one-statement body means the same thing on all three when
spliced into a larger expression.
