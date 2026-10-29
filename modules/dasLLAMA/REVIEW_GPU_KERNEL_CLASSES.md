# dasLLAMA GPU Kernel Class Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_GPU.md`. Planned work: the `followup_*.md` ledgers beside this file.

Kernel twins are two classes with the same dispatch grid formula, the same mapping of threads to
work and the same index and guard arithmetic, whose compiled bodies differ only in expressions,
constants or unroll counts that one value selects: a template constant, a typedef, which base
shell's method they inherit, or a run-time count of live entries inside a fixed extent (a column
count, a row count). A base shell is the dispatch-less base class whose methods the emitter splices
flat into each deriving class. A stamp is a kernel class that compiles to a shader module -
standalone, a template instance or a base-shell derivative. A census key is a kernel name
`modules/dasLLAMA/tests/test_kernel_coverage.das` counts, that the engine's Metal or Vulkan driver
compiles at any point of a run, and that the pre-change tree does not compile. A crown is a kernel
family a box's tune sidecar enables. A lens is the `[metal_dispatch]`, `[vk_dispatch]` or
`[cuda_dispatch]` macro that generates a class's builder from its bindings; kargs is the
kernel-argument struct a dispatch hands its kernel. A home is one backend's own kernel files -
Metal's, Vulkan's or CUDA's - as against the shared files `dasllama/dasllama_gpu_kernels_common.das`
and `dasllama/dasllama_gpu_kernels_discrete.das`.

**A diff that adds a `class template` a GPU home stamps, or moves one between a home's file,
`dasllama/dasllama_gpu_kernels_discrete.das` and `dasllama/dasllama_gpu_kernels_common.das`,
applies `REVIEW_PLACEMENT.md` too.**

**A kernel twin that binds a different kargs (kernel-argument struct) type than its sibling twin
is a defect, even where one twin ignores a field of it.**

**Kernel twins keep every `@binding` number they both declare; a field a `@template_gate` omits on
one twin leaves a hole at its number and renumbers nothing.**

**Two stamps that declare the same field at different `@binding` numbers or kargs offsets never
share a host function that binds by number - each binds through its own generated builder.**

**A diff that makes two stamps of one class template or base shell declare the same field at
different `@binding` numbers or kargs offsets ledgers the pair at
`ARCHITECTURE_GPU.md#gpu-backends` in the same change.**

**Kernel twins are written once: as stamps of one `class template`, as classes deriving one base
shell, or - where the value that selects the difference is a run-time count - as one class whose
body reads the count from its kargs; a twin's own body holds only what the selecting value
changes, the shared guard and index arithmetic living in the template or the shell.**

**A diff that adds a bit-for-bit compare, or changes text on the path one covers - two kernel
bodies that compile to separate shader modules, which a cell under `modules/dasLLAMA/tests/`
holds bit for bit against each other - spells as `mad` every floating-point multiply on that path
that feeds an add, shared method or function text included.** A driver decides per shader module
whether to contract a multiply-add into one fma, so two bodies spelled alike round a ulp apart.

**A value that no model file and no request can change - a tile width, a math constant, a cap
fixed by the model architecture the class serves - which every dispatch site under `dasllama/`
passes identically to one stamp never reaches that stamp through a per-dispatch argument channel
(a uniform, a `@push_constant` field, a kargs field, an `@off` bind offset): stamp it into the
class as a `@template_constant` where the class's stamps differ on it, pass it as a literal
argument of the shared method the stamp's kernel calls, or write it as a literal or a module `let`
in the body - never a module `var`.**

**A stamp, or an intermediate template between a stamp and its class template or base shell, sets
only `@template_constant`s that at least one stamp built from it reads in its compiled body: a
`static_if` arm, a `@template_gate`, an expression, a loop bound, an array extent.** A constant
no such site reads is a defect - move it to the template whose body reads it, or make the body
read it. A read that sits only inside a `static_if` arm the stamp's own constants compile out is
not a read.

**A diff that changes a class body, a class template, a base shell, or a helper a stamp's body
splices diffs, for every stamp built from what it changed, that stamp's generated source against
the pre-change tree's (the files `DASLLAMA_METAL_MSL_DUMP=<dir>` writes, a disassembly diff of
the `.spv` files `DASLLAMA_VK_SPV_DUMP=<dir>` writes, or the CUDA C text global a `[cuda_kernel]`
method generates), and its PR body names each stamp with the kind of difference read there -
never the source text.**

**The kind a PR body names for a changed stamp is one of these: no difference; a difference
confined to whitespace, scoping braces, parentheses or identifier names; the expression text
unchanged, moved into a named helper the stamp now calls; the difference named with the
compile-time choice that carries it; or the behaviour change named with the test cell that pins
it.**

**A kernel-family stamp - any stamp of a class template or derivative of a base shell that a
lens-generated builder dispatches - that binds a buffer to a binding whose fields its compiled
body, inherited code included, never reads or writes is a defect: gate the field with
`@template_gate` where a template constant decides it, and where the family shares one binding
layout for the whole family on purpose, name that case in `ARCHITECTURE_GPU.md#gpu-backends`'s
ledgered kernel-binding asymmetries.** A binding counts as used when the compiled body reads or
writes any field declared on it - fields in the stamp or in the shell may share a binding,
`@role = "alias"` marks such a view - including a field touched only under a run-time flag.

**A diff that moves a kernel class out of the class template its siblings stamp, or off the base
shell they derive from, gives the class a `//!` line above its `[metal_dispatch]`,
`[vk_dispatch]` or `[cuda_dispatch]` declaration naming the body difference that keeps it out of
that template or shell.**

**A diff that adds a kernel class stamping or deriving from - through an intermediate template
too - a template more than one home stamps (`dasllama/dasllama_gpu_kernels_common.das`,
`dasllama/dasllama_gpu_kernels_discrete.das`), or makes an existing class do so, gives the class a
`//!` line above its own declaration naming the nearest template in its chain that more than one
home stamps.**

**A diff that adds a name to `DISCRETE_ONE_HOME_STAMPS` in `REVIEW.das` - the license for a
`class template` in `dasllama/dasllama_gpu_kernels_discrete.das` that only one discrete home
stamps - names the template, and the family of the home that does not stamp it, in the
`followup_general.md` row of stamps that wait on their families, in the same change; a diff that
removes a name from the list removes it from that row too.**

**Weakening `check_shared_template_homes` in `REVIEW.das` is a defect.** It holds every
`class template` in `dasllama/dasllama_gpu_kernels_discrete.das` to a stamp from the Vulkan and
the CUDA home or a `DISCRETE_ONE_HOME_STAMPS` license, and every one in
`dasllama/dasllama_gpu_kernels_common.das` to a stamp from the Metal and the Vulkan home, through
any chain of intermediate templates.

**A diff that adds or changes a binding of a class a lens declares that no site writes
after arming - a binding filled before the first encode and never written again - puts
`@role = "weight"` on a field at that binding.**

**`@role = "weight"` on data written during a step - a pooled buffer the host refills each encode,
or one another kernel rewrites - is a defect, judged per stamp by the buffer its dispatch sites
bind; such a field either omits `@role` or names the access its body performs.** Neither the
Metal nor the Vulkan lens orders a `weight` read after the write that refilled the buffer, and
neither refuses the role on a field the body only reads.

**A diff that adds a `[metal_dispatch]`, `[vk_dispatch]` or `[cuda_dispatch]` class under
`dasllama/` that the engine's Metal, Vulkan or CUDA driver never compiles at any point of a run is
a defect - the class moves into the file under `modules/dasLLAMA/benchmarks/` or
`modules/dasLLAMA/tests/` that compiles it, and a class nothing compiles is deleted.**

**A diff that deletes a GEMM form `DASLLAMA_CUDA_MMQ` selects between, on the ground that another form
wins on every card measured, is a defect - move the knob's default instead and keep the loser
compiled.** The winner
is the card's and the driver's, not the kernels' - the same silicon favors the f16 tile under its
Vulkan driver - so a card that flips the pick turns the pair into a per-box race, never a deletion.

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

**A blind-spot entry a diff adds, and a blind-spot entry that names a new census key by exact name
or covers it by prefix, carries for each key it covers the reason no census row reaches it, the
stocked model that dispatches it where one does - one the `stocked` suite runs on a box that has
it - and the model-free test cell that dispatches it.**

**A diff that adds a new Metal census key that is dispatched only under a crown adds, in the same
change, either a census row that pins the crown (`pin_metal_tensor_crowns`), or a
`CENSUS_NEVER_DISPATCHED` entry plus a `CENSUS_CROWN_CONDITIONAL` entry naming the crown (both in
`modules/dasLLAMA/tests/test_kernel_coverage.das`); both entries spell the key exactly.** The
blind-entry asserts look the row up by the entry's exact text, and they count the dispatches of every
key the entry is a prefix of.

**Weakening the blind-entry asserts in `modules/dasLLAMA/tests/test_kernel_coverage.das` - that
an entry matches a compiled census key, and that it matches no dispatched one - is a defect.**

**Weakening any refusal a lens - the macro behind `[metal_dispatch]`, `[vk_dispatch]` or
`[cuda_dispatch]` that derives each field's access and generates the builder - makes at compile
time, or any cell that holds one, is a defect.**

**Weakening any refusal the manual-dispatch check in `dasllama/dasllama_metal_lens.das` makes in a
module whose name starts with `dasllama` and contains `metal` - a direct
`metal_dispatch_threadgroups` call, an `@ssbo` field with no `@role`, a function binding
`kn_kargs` on fewer paths than it calls `kn_dispatch` - is a defect; a module that adds
`options _metal_manual_dispatch = true` weakens it.**

**A diff that adds a refusal to a lens lands the cell that holds it in the same change** - a
`test_lens_*` cell in `modules/dasLLAMA/tests/test_metal_misc_kernels.das` for Metal, a
`test_vkd_lens_*` cell in `modules/dasLLAMA/tests/test_vulkan_kernels.das` for Vulkan, a cell in
`modules/dasLLAMA/tests/test_cuda_kernels.das` for CUDA.

**A diff that replaces a refusal with a derivation - the lens computing the value it used to
demand, so no class can compile with that value missing - points that refusal's cell at the
derived path in the same change.** Deriving the value is not a weakening.

**A kernel field carries `@span` only when the range it declares covers every byte a dispatch
writes through that field from its bound offset, and ends within the bound buffer.** A write
outside the tracked range escapes the hazard check, and a range past the buffer tracks memory the
dispatch does not own.

**A diff that adds or changes an ordered argument list into a generated setter (the
`set_*(bufs, sizes, gbits)` function a `[vk_dispatch]` class's family generates) gives it one
entry per distinct `@binding` number the stamp keeps - the class or its base declares it and no
template gate drops it for that stamp - in ascending order; fields sharing a binding share one
entry, a number nothing keeps gets none.** The setter checks only how many arguments it got,
never which field each position carries.

**A diff that adds a hand-written encode or descriptor-set helper, or a hand-rolled bind list on a
dispatch - a buffer or kargs field bound by literal number instead of through the builder a
lens generates for that class; code the diff only moves or renames is not added - says why the
generated builder cannot serve that site, in a comment at the site or in the PR body.** A body
that only picks, defaults or composes generated builders binds nothing; an ordered argument list
into a generated `set_*` setter restates no binding number and is not one.

**A value that reaches the kernel twice device-side - a scalar bound both as a uniform buffer
and as a kargs field - is a defect: bind it once, as a kargs field.** A `params=` value consumed
only host-side - by the `grid=` / `tg=` spec, a `requires=` item, or an `@span` - never reaches
the device, so it does not count.

**Never bind a scalar that a stamp's other bound scalars already determine by integer arithmetic at
every dispatch site of that stamp under `dasllama/` - derive it in the stamp's body from them
instead.** Binding an integer count separately adds a second place to get it wrong.

**A diff that adds or changes a kernel-class method whose returned value is used anywhere but
as the whole right-hand side of a `let` or an assignment - an argument, an operand, a subscript,
a `return`'s value - has it return its value in one statement after compile-time folding: an
arrow form (`=>`), or a `static_if` whose every arm is one `return`; a method that needs more
than one statement hands its value back through a `var T&` parameter instead.** A kernel class
compiles on its backend's emitter and as its own CPU oracle (the class body run on the CPU as the
reference), and only a one-statement body means the same thing on both when spliced into a larger
expression.
