# dasLLAMA GPU Kernel Class Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_GPU.md`. Planned work: `followup_metal.md` for Metal, `followup_vulkan.md`
for Vulkan.

**A kernel twin - one of two classes whose compiled bodies differ only on an axis one value
fixes: a template constant, a typedef, which base shell's method it inherits, or a run-time count
of live entries inside a fixed extent (a column count, a row count) - that binds a different
kargs (kernel-argument struct) type than its sibling twin, or shifts a shared field to a
different binding number, is a defect - even where one twin ignores that field; a field a
`@template_gate` omits on one twin is a shared field still, and keeps the number the other twin
binds it at.** A base shell is the dispatch-less base class whose methods the emitter splices
flat into each deriving class.

**Kernel twins are written once: as stamps of one `class template`, as classes deriving one base
shell, or - where the axis is a run-time count - as one class whose body reads the count from its
kargs.** Two classes with bodies of their own that share a base shell's method are not twins.

**A diff that adds a bit-for-bit compare, or changes text on the path one covers - two kernel
bodies that compile to separate shader modules, which a cell under `modules/dasLLAMA/tests/`
holds bit for bit against each other - spells as `mad` every multiply on that path that feeds an
add, shared method or function text included.** A driver decides per shader module whether to
contract a multiply-add into one fma, so two bodies spelled alike round a ulp apart.

**A value that no model file and no request can change - a tile width, a math constant, a cap
fixed by the model architecture the class serves - which every dispatch site under `dasllama/`
passes identically to one stamp never reaches that stamp through a per-dispatch argument channel
(a uniform, a `@push_constant` field, a kargs field, an `@off` bind offset): stamp it into the
class as a `@template_constant` where the class's stamps differ on it, or write it as a literal or
a module `let` in the body - never a module `var`.** A stamp is a kernel class that compiles to a
shader module - standalone, a template instance or a base-shell derivative.

**A stamp sets only `@template_constant`s its own compiled body reads: a `static_if` arm, a
`@template_gate`, an expression, a loop bound, an array extent.** A constant
no such site reads is a defect - move it to the template whose body reads it, or make the body
read it.

**A diff that changes a class body, a class template, a base shell, or a helper a stamp's body
splices carries in the PR body, for every stamp built from what it changed, that stamp's
generated source diffed against the pre-change tree (the `*_msl` global, or a disassembly diff
of the `.spv` files `DASLLAMA_VK_SPV_DUMP=<dir>` writes).**

**Each generated-source diff a PR body carries for a changed stamp takes only one of these forms:
an empty diff; a difference confined to whitespace, scoping braces, parentheses or identifier names,
with every changed line paired against its pre-change line in the PR body; the expression text
unchanged, moved into a named helper the stamp now calls; the difference named with the
compile-time choice that carries it; or the behaviour change named with the test cell that pins
it.**

**A kernel-family stamp - one stamp of a class template, or one of the classes deriving from a
base shell that carry a `[vk_dispatch]` / `[metal_dispatch]` - that binds a buffer to a
binding whose fields its compiled body, inherited code included, never reads or writes is a
defect: gate the field with `@template_gate` where a template constant decides it, and where the
family shares one set layout on purpose, name that case in `ARCHITECTURE_GPU.md#gpu-backends`'s
ledgered kernel-binding asymmetries.** A binding counts as used when the compiled body reads or
writes any field declared on it - fields in the stamp or in the shell may share a binding,
`@role = "alias"` marks such a view - including a field touched only under a run-time flag.

**A diff that moves a kernel class out of the class template its siblings stamp, or off the base
shell they derive from, gives the class a `//!` line above its `[metal_dispatch]` /
`[vk_dispatch]` declaration naming the body difference that keeps it out of that template or
shell.**

**A diff that adds or changes a `[metal_dispatch]` / `[vk_dispatch]` binding that no site writes
after arming - a binding filled before the first encode and never written again - puts
`@role = "weight"` on a field at that binding.**

**`@role = "weight"` on per-encode data the kernel reads - a pooled buffer the host refills
each encode - is a defect; a per-encode field either omits `@role` or names the access its body
performs.** The Metal builder records no hazard for a `weight` field, so nothing orders the
kernel's read after the encode that refilled the buffer; neither lens refuses a `weight` field
the body only reads.

A new census key is a name `modules/dasLLAMA/tests/test_kernel_coverage.das` counts a compiled
kernel under that the pre-change tree does not compile; a `[metal_kernel]` def, a `[vk_dispatch]`
declaration or a new instance of a template carrying one, under `dasllama/`, adds one.

**A diff that adds a new census key a census row dispatches names, in the PR body, that row and
its nonzero count for the key, read from a census run on the key's backend.**

**A diff that adds a new census key which no census row of its backend dispatches where such a
row could dispatch it - a model the census file loads for that backend, run the way the census
runs it - adds that row, or that model, to `modules/dasLLAMA/tests/test_kernel_coverage.das` in
the same change.**

**A diff that adds a new census key which no census row of its backend could dispatch - the
model, the quant or the load shape sits outside what the census runs - names it in the blind-spot
list of `modules/dasLLAMA/tests/test_kernel_coverage.das`: `CENSUS_NEVER_DISPATCHED` for Metal,
`VK_CENSUS_NEVER_DISPATCHED` for Vulkan.**

**A blind-spot entry a diff adds carries the reason no census row reaches the key, the stocked
model that dispatches it where one does - one the `stocked` suite runs on a box that has it - and
the model-free test cell that dispatches it.**

**Weakening the blind-entry asserts in `modules/dasLLAMA/tests/test_kernel_coverage.das` - that
an entry matches a compiled census key, and that it matches no dispatched one - is a defect.**

**Weakening any refusal the lens - the macro behind `[metal_dispatch]` / `[vk_dispatch]` that
derives each field's access and generates the builder - makes at compile time, or any
`test_lens_*` / `test_vkd_lens_*` cell that holds one, is a defect.**

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

**A diff that adds or changes an ordered argument list into a generated setter (the
`set_*(bufs, sizes, gbits)` function a `[vk_dispatch]` class's family generates) gives it one
entry per distinct `@binding` number the stamp keeps - the class and its base declare it and no
template gate drops it for that stamp - in ascending order; fields sharing a binding share one
entry, a number nothing keeps gets none.** The setter checks only how many arguments it got,
never which field each position carries.

**A hand-written encode or descriptor-set helper, or a hand-rolled bind list on a dispatch, that a
diff adds anywhere - a buffer or kargs field bound by literal number instead of through the builder
the `[metal_dispatch]` / `[vk_dispatch]` lens generates for that class - whose PR body does not
state why the generated builder cannot serve that site is a defect.** A body that only picks,
defaults or composes generated builders binds nothing; an ordered argument list into a generated
`set_*` setter restates no binding number and is not one.

**A value that reaches the kernel twice device-side - a scalar bound both as a uniform buffer
and as a kargs field - is a defect: bind it once, as a kargs field.** A `params=` value consumed
only host-side - by the `grid=` / `tg=` spec, a `requires=` item, or an `@span` - never reaches
the device, so it does not count.

**Never bind a scalar that a stamp's other bound scalars already determine by integer
arithmetic - derive it in that stamp's body from them instead.** Binding an integer count
separately adds a second place to get it wrong.

**A diff that adds or changes a kernel-class method whose returned value is used anywhere but
as the whole right-hand side of a `let` or an assignment - an argument, an operand, a subscript,
a `return`'s value - has it return its value in one statement after compile-time folding: an
arrow form (`=>`), or a `static_if` whose every arm is one `return`; a method that needs more
than one statement hands its value back through a `var T&` parameter instead.** A kernel class
compiles on its backend's emitter and as its own CPU oracle (the class body run on the CPU as the
reference), and only a one-statement body means the same thing on both when spliced into a larger
expression.
