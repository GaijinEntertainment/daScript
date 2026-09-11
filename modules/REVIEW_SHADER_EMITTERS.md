# Shader Emitters Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `dasMetal/ARCHITECTURE.md`, `dasSpirv/ARCHITECTURE.md`, `dasSpirv/ARCHITECTURE_COOPMAT.md`.

**Routed here by another checklist: a diff under `dasMetal/` or `dasSpirv/`, or to any kernel
body or fixture either emitter compiles, applies this list together with its own folder's.**

A device-side value is one whose storage exists only on the device: a tile or tensor, a layout
or view over one, a sampler, an image. A struct that stands for one on the CPU is a marker
struct when it has no storage of its own and a resource struct when it carries a device handle.

**Never put anything that cannot compile on the CPU into a kernel body or into a function a
kernel calls - keep both in ordinary das.** A kernel built from ordinary values is compared
against its own CPU run; a marker struct and the builtins over it compile on the CPU, and
their CPU bodies compute nothing.

**A diff that adds or changes an emitter builtin whose operands are all ordinary CPU values - a
declaration in `daslib/shader_lingua_franca.das` or an emitter's builtin table - ships a CPU
body that returns what the emitted form returns, argument for argument.** A builtin taking a
marker or resource struct is outside this trigger.

**Never let a construct the emitter cannot lower produce a kernel or a crash - the emitter
reports a compile error that names the construct.**

**Never pass a shape constant to a kernel as a runtime argument - pass it as a call-site
constant.** A shape constant is a value the kernel's index arithmetic treats as a fixed extent
of its tile or of its cooperating lane group.

**A SPIR-V kernel that loads its operands with `coopmatLoadTensor*` receives a run-time-only
matmul reduction width through a `tensorLayout2D` or `tensorLayout2DPad` whose dimension
`tensorLayoutSetDimension` sets, and no other way.** The reduction width is the K dimension -
the length of the loop the kernel accumulates over; it does not fix tiling, so it is not a
shape constant.

**A Metal kernel that loads its operands with the `tmm2d_*` family receives a run-time-only
matmul reduction width through a `matmul2d_descriptor` whose K extent is `dynamic_extent`, and
no other way.**

**A diff that makes a kernel need a shape constant known only at run time ships a
specialization path, or records in an `ARCHITECTURE*.md` at the root of the module the kernel
ships in that the kernel cannot have one.** A specialization path is one compiled variant per
constant shape.

**Never check a claim about emitted shape against the das source - check it in the emitted
words or text.** Emitted shape is the structure of the emitted kernel - its signature, its
parameter attributes, its statement forms - and its stamped shape values (tile, grid,
threadgroup sizes).

**A diff that adds a kernel-model capability to one emitter adds it to the other, or leaves the
shared ledger (`dasMetal/ARCHITECTURE.md`) naming that capability - covered by the row that
names its family, or by a row the diff adds.** A kernel-model capability is present on an
emitter when a kernel source that uses it compiles there, and a family is the set of constructs
one ledger row names. A diff that leaves every kernel source compiling exactly as it did before
changed lowering alone, and answers to that emitter folder's own checklist.

**A diff that puts a `daslib/shader_lingua_franca` declaration into a kernel body or fixture an
emitter compiles, where that emitter does not handle it, ships, in the same change, either
that emitter's lowering of the declaration or a test showing the emitter rejects the
declaration by name.** A declaration in that module is available to both emitters.

**A skippable read of a global-rooted array - a module global, a `@workgroup` array, or a
`self.<member>` resource - in a `[spirv_kernel]` or `[compute_shader]` body, or in any `def` that
body calls, stays skippable: a diff that makes it unconditional - both arms of an if reading it,
a clamped index, a bare read - is a defect.** The emitter lowers a `?:`, `&&` or `||` operand as
a branch (`dasSpirv/ARCHITECTURE.md`, "Operand laziness follows the language"), so the
short-circuit form needs no rewrite.
