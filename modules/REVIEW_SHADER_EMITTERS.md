# Shader Emitters Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `dasMetal/ARCHITECTURE.md`, `dasSpirv/ARCHITECTURE.md`, `dasSpirv/ARCHITECTURE_COOPMAT.md`.

**Never put anything that cannot compile on the CPU into a kernel body or into a function a
kernel calls - keep both in ordinary das.** A kernel built from ordinary values is compared
against its own CPU run; a marker struct - one that stands on the CPU for a value whose storage
exists only on the device (a tile, a tensor, a layout or view over one, a sampler, an image) and
has no storage of its own - and the builtins over it compile on the CPU, and their CPU bodies
compute nothing.

**A diff that adds or changes an emitter builtin whose operands are all ordinary CPU values - a
declaration in `daslib/shader_lingua_franca.das`, `dasSpirv/spirv/spirv_builtins.das` or
`dasMetal/metal/metal_builtins.das` - ships a CPU body that returns what the emitted form
returns, argument for argument.**

**Never let a construct the emitter cannot lower produce a kernel or a crash - the emitter
reports a compile error that names the construct.**

**Never pass a shape constant to a kernel as a runtime argument where a call-site constant can
carry it - pass it as a call-site constant.** A shape constant is a value that sizes a
`@workgroup` array, fixes an unrolled loop's trip count, or multiplies an index as a stride into a
fixed-extent array (a `@workgroup` or local array, not an ssbo the host sizes); a run-time count
of live entries inside such a fixed extent is not one. A call-site constant is fixed where the
kernel's source is generated - a template constant or a typedef - so the emitter bakes it in; a
kernel whose source is generated once and dispatched for differently shaped inputs cannot carry
a shape that varies per input that way.

**A SPIR-V kernel that loads its operands with `coopmatLoadTensor*` receives a run-time-only
matmul reduction width through a `tensorLayout2D` or `tensorLayout2DPad` whose dimension
`tensorLayoutSetDimension` sets - the load takes the width from that layout alone; the
accumulation loop's own bound may read it from the push constants.** The reduction width is the K
dimension - the length of the loop the kernel accumulates over; it does not fix tiling, so it is
not a shape constant.

**A Metal kernel that loads its operands with the `tmm2d_*` family receives a run-time-only
matmul reduction width through a `matmul2d_descriptor` whose K extent is `dynamic_extent` - the
load takes the width from that descriptor alone; the accumulation loop's own bound may read it
from the kernel's arguments.** The per-step chunk a `tmm2d_*` call takes is a shape constant.

**A diff that makes a kernel need a shape constant known only at run time ships a
specialization path - one compiled variant per constant shape - or records the kernel as having
none in an `ARCHITECTURE*.md` at the root of the module it ships in.**

**A claim about emitted shape checked against the das source is a defect - the check reads the
emitted artifact: the SPIR-V words `dasSpirv` builds, or the MSL text `dasMetal` writes.** Emitted
shape is the structure of the emitted kernel - its signature, its parameter attributes, its
statement forms - and the constants that structure carries: a tile, an unroll width, a SPIR-V
kernel's local size (the `LocalSize` execution mode the SPIR-V words carry), the extent of a local
or `@workgroup` array. A grid is not one, and neither
is a Metal threadgroup size - both are dispatch arguments, read at the encoder call site.

**A diff that claims an emitted shape value - in a commit message, a PR body, or an architecture
line - states in its PR body which emitted artifact was read, the SPIR-V words or the MSL text,
and the count or value read there.**

**A diff that adds a kernel-model capability to one emitter adds it to the other, or leaves the
shared ledger (`dasMetal/ARCHITECTURE.md` sec.5) naming that capability - covered by the row
that names its family, or by a row the diff adds.** A kernel-model capability is present on an
emitter when the emitted text or words carry its effect - an emitter that accepts the construct
and emits nothing for it does not have it - and a family is the set of constructs one ledger row
names.

**A diff that puts a `daslib/shader_lingua_franca` declaration into a kernel body or fixture an
emitter compiles, where that emitter does not handle it, ships, in the same change, either
that emitter's lowering of the declaration or a test showing the emitter rejects the
declaration by name.** A declaration in that module is available to both emitters.

**Weakening the MSL emitter's refusal to compile an unlicensed float `matmul2d` A operand -
`[metal_kernel(float_a_ok=true)]` is the license (`dasMetal/metal/msl_emit.das`) - is a defect.**
A float operand keeps the op off its native fast path.

**A global-rooted-array read - a module global, a `@workgroup` array or a `self.<member>`
resource read in a kernel body or in a `def` it calls - that a diff adds, makes reach an index it
did not reach before, or makes happen where a condition used to keep it out, is in range on every
dispatch it happens on: its index sits inside the region that dispatch's own bound defines, or
inside slack - an allocation past that region's end that the kernel's module-root
`ARCHITECTURE*.md` names.** A run-time condition keeps a read out when it is a guard on the read,
a clamp landing inside the region, or a guard on the store of a block the read loads whole.

**A global-rooted-array read - a module global, a `@workgroup` array or a `self.<member>` resource
read in a kernel body or in a `def` it calls - that a diff makes happen in a compiled
`[spirv_kernel]`, `[compute_shader]` or `[metal_kernel]` variant it did not appear in before -
through a removed `static_if` or `@template_gate`, or a widened constant the gate switches on - is
in range on every dispatch of that variant: inside the region the dispatch's own bound defines, or
inside slack - an allocation past that region's end that the kernel's module-root
`ARCHITECTURE*.md` names.** The gate that keeps the read out of the variant is its only range
check there.
