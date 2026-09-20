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
`@workgroup` array, fixes an unrolled loop's trip count, or multiplies an index as a stride; a
run-time count of live entries inside such a fixed extent is not one.

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

**Never check a claim about emitted shape against the das source - check it in the emitted
words or text, the SPIR-V words one emitter builds and the MSL text the other writes.** Emitted
shape is the structure of the emitted kernel - its signature, its parameter attributes, its
statement forms - and its stamped shape values (tile, grid, threadgroup sizes).

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

**A global-rooted-array read a diff adds, makes happen at an index it did not reach before, or
makes happen on a dispatch it did not happen on before, is in range on every dispatch it happens
on, or the `ARCHITECTURE*.md` at the root of the module the kernel ships in names slack past
that range and the read stays inside the slack.** A global-rooted array is a module global, a
`@workgroup` array, or a `self.<member>` resource, read in a `[spirv_kernel]`,
`[compute_shader]` or `[metal_kernel]` body or in any `def` that body calls; a diff makes that
read happen on a dispatch it did not happen on before when it drops the condition that kept the
read inside the region this dispatch's own bound defines - a read in both arms of an `if`, a
clamp landing outside that region, a bare read - or loads a fixed-size block whose only guard is
on its store.

**A compile-time gate (`static_if`, `@template_gate`) that keeps a global-rooted-array read out of
a compiled `[spirv_kernel]`, `[compute_shader]` or `[metal_kernel]` variant keeps it out: a diff
that removes the gate, or widens the constant the gate switches on so the read reaches variants
it did not reach, is a defect.**
