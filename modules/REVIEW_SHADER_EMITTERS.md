# Shader Emitters Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `dasMetal/ARCHITECTURE.md`, `dasSpirv/ARCHITECTURE.md`.

**Routed here by another checklist: a diff under either emitter module, or to any kernel body
or fixture either emitter compiles, applies this list together with its own folder's.**

**Never put anything that cannot run on the CPU into a kernel body or into a function a kernel
calls - keep both in ordinary das.** The CPU run of the same body is what the tests compare
against.

**A diff that adds or changes an emitter builtin - a declaration in
`daslib/shader_lingua_franca.das` or an emitter's builtin table - ships a CPU body that returns
what the emitted form returns, argument for argument.**

**Never let a construct the emitter cannot lower produce a kernel or a crash - the emitter
reports a compile error that names the construct.**

**Never pass a shape constant to a kernel as a runtime argument - pass it as a call-site
constant.** A shape constant is any value that fixes the kernel's tiling: a tile row count, a
tile column count, a cooperating-simdgroup count, a staged chunk depth.

**Never pass a matmul reduction width known only at run time into a kernel that loads its
operands with the emitter's tensor-load ops (`coopmatLoadTensor*`, `coopmatLoadTensorDecode`)
any way but through the emitter's runtime-extent descriptor - `dynamic_extent` on Metal, and on
SPIR-V a `tensorLayout2D` or `tensorLayout2DPad` whose dimension `tensorLayoutSetDimension`
sets.** The reduction width is the K dimension - the length of the loop the kernel accumulates
over; it does not fix tiling, so it is not a shape constant. A loop that stages its own tiles
and loads fragments with `coopmatLoad` takes K as a plain uniform.

**A diff that makes a kernel need a shape constant known only at run time ships a
specialization path, or records in the emitter's architecture doc - `dasMetal/ARCHITECTURE.md`
for a Metal kernel, `dasSpirv/ARCHITECTURE.md` for a SPIR-V kernel - that the kernel cannot
have one.** A specialization path is one compiled variant per constant shape.

**Never check a claim about emitted shape against the das source - check it in the emitted
words or text.** Emitted shape is the structure of the emitted kernel - its signature, its
parameter attributes, its statement forms - and its stamped shape values (tile, grid,
threadgroup sizes).

**A diff that adds a kernel-model capability to one emitter adds it to the other, or records
the asymmetry in the shared ledger (`dasMetal/ARCHITECTURE.md`).** A kernel-model capability
is anything a kernel author can write differently because of it - an annotation, a call form,
a declaration the emitter now accepts or refuses; a change to lowering alone is the folder's
own checklist's.

**A diff that puts a `daslib/shader_lingua_franca` declaration into a kernel body or fixture an
emitter compiles, where that emitter does not handle it, ships, in the same change, either
that emitter's lowering of the declaration or a test showing the emitter rejects the
declaration by name.** A declaration in that module is available to both emitters.

**A `?:`, `&&`, or `||` in a `[spirv_kernel]` or `[compute_shader]` body, or in any `def` that
body calls, whose skippable operand subscripts a global-rooted array - a module global, a
`@workgroup` array, or a `self.<member>` resource - is written as the language reads it: never
rewritten around the emitter with an if/else, a clamp or an unconditional read.** The emitter
lowers such an operand as a branch (`dasSpirv/ARCHITECTURE.md`, "Operand laziness follows the
language"); a branchless form where it matters waits for `select` (`plans/shader_emitter_followups.md`).
