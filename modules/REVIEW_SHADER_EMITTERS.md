# Shader Emitters Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `dasMetal/ARCHITECTURE.md`, `dasMetal/ARCHITECTURE_PARITY.md`, `dasSpirv/ARCHITECTURE.md`,
`dasSpirv/ARCHITECTURE_COOPMAT.md`, `dasCUDA/ARCHITECTURE.md`, `dasCUDA/ARCHITECTURE_EMIT.md`. The
emitters are `dasSpirv`'s (SPIR-V words), `dasMetal`'s (MSL text) and `dasCUDA`'s (CUDA C++ text);
this list is the one copy, and the rules below refer to it. A kernel is a function carrying
`[metal_kernel]`, `[spirv_kernel]`, `[cuda_kernel]`, or an annotation from `dasSpirv`, `dasVulkan`
or `dasMetal` whose name ends in `_shader`. A global-rooted-array access is a load or a store, in
a kernel body or any `def` it reaches through a chain of calls, of an element of a module global,
a `@workgroup` array or a `self.<member>` resource; slack is an allocation past a region's end
that the kernel's module-root `ARCHITECTURE*.md` names. A kernel-model capability is a construct
a kernel body may use - an op, a type, an attribute, a memory class - that an emitter lowers.
Emitted shape is the
structure of the emitted kernel - its signature, its parameter
attributes and binding numbers, its statement forms - and the constants that structure carries: a
tile, an unroll width, a SPIR-V kernel's local size (the `LocalSize` execution mode the SPIR-V
words carry), a CUDA kernel's block size and `min_blocks` (the `__launch_bounds__` arguments the
CUDA text carries), the extent of a local or `@workgroup` array; a grid and a Metal threadgroup
size are dispatch arguments, read at the encoder call site, and are not emitted shape. The
reduction width (K) is the length of the loop a matmul kernel accumulates over; it does not fix
tiling, so it is not a shape constant.

**A kernel body that computes on a marker struct with anything but the builtins declared over it
- the ops that take it as an operand or produce it, with the layout or descriptor values they
take - is a defect.** Arithmetic on what those ops produce is ordinary das, which the kernel's CPU
run checks. A marker struct is one that stands on the CPU for a value whose storage exists only on
the device (a tile, a tensor, a layout or view over one, a sampler, an image, a `subpassInput`)
and has no storage of its own; the builtins over it compile on the CPU, and their CPU bodies
compute nothing.

**A diff that adds or changes a declaration an emitter lowers by name whose operands are all
ordinary CPU values - in `daslib/shader_lingua_franca.das`, `dasSpirv/spirv/spirv_builtins.das`,
`dasMetal/metal/metal_builtins.das` or `dasCUDA/cuda/cuda_intrinsics.das` - ships a CPU body that
computes what the emitted form computes - its return value and every write through a reference
or pointer argument.** A kernel's CPU run is the reference its device output is compared against;
a body that computes nothing leaves that reference reading storage only the device fills.

**A diff that changes what an emitter lowers - an emitter's emit code, or
`daslib/shader_emit_common.das` - never lets a construct the emitter cannot lower, or would lower
to text or words that do not compute it, produce a kernel or a crash - the emitter reports a
compile error that names the construct instead.**

**Never pass a shape constant to a kernel as a runtime argument where a call-site constant can
carry it - pass it as a call-site constant.** A shape constant is a value the emitter requires as
a compile-time constant: the extent of a `@workgroup` or local array (not a buffer the host
sizes), an unrolled loop's trip count, a stride into such an array, or a `tmm2d` op's M, N, or
the K width of the one chunk it steps over; a run-time count of live entries inside such a fixed
extent is not one. A call-site constant is fixed where the kernel's source is generated - a
template constant or a typedef - so the emitter bakes it in; a kernel whose source is generated
once and dispatched for differently shaped inputs cannot carry a shape that varies per input that
way.

**A kernel that loads its operands with `coopmatLoadTensor*` and takes a run-time-only matmul
reduction width takes it through a `tensorLayout2D` or `tensorLayout2DPad` whose dimension
`tensorLayoutSetDimension` sets; the load never reads the width from anywhere else - only the
accumulation loop's bound may read it from the kernel's run-time arguments.**

**A Metal kernel that loads its operands with the `tmm2d_*` family and takes a run-time-only
matmul reduction width takes it through a `matmul2d_descriptor` whose K extent is
`dynamic_extent`; a run-time K reaches the load only as that extent or as the row stride of the A
operand's panel, and only the accumulation loop's bound may read it from the kernel's
arguments.**

**A diff that makes a kernel need a shape constant known only at run time ships a
specialization path - one code path per constant shape, a compiled variant or a branch whose shape
is a literal - or records the kernel as having none in an `ARCHITECTURE*.md` at the root of the
module it ships in.**

**A diff that makes a new claim, in a commit message, a PR body or a line of an
`ARCHITECTURE*.md`, about what a kernel's emitted words or text contain or leave out, or that a
kernel's emitted output did not change, reads the claim from the emitted artifact - the words or
text the opening names for the kernel's emitter - never from the das source, and names in its PR
body the artifact read and the value read there, or, for a no-change claim, that the artifacts
before and after the diff matched.** A claim about a das construct alone, or about where the
driver keeps a value (registers, spills), names nothing an emitted artifact shows; a section moved
between architecture docs unchanged makes no new claim.

**A diff that teaches one emitter a kernel-model capability another lacks, or adds a kernel body
that uses a capability not every emitter has, adds that capability to the emitters lacking it or
leaves the shared ledger (`dasMetal/ARCHITECTURE_PARITY.md`) naming it, under the row that names
its family or a row the diff adds.** A kernel-model capability is present on an emitter when the
emitted text or words carry its effect - an emitter that accepts the construct and emits nothing
for it does not have it - and a family is the set of constructs one ledger row names.

**A diff that puts a `daslib/shader_lingua_franca` declaration into a kernel body or fixture an
emitter compiles, where that emitter does not handle it, ships, in the same change, either
that emitter's lowering of the declaration or a test showing the emitter rejects the
declaration by name.** A declaration in that module is available to every emitter.

**A diff that adds `float_a_ok=true` to a kernel's `[metal_kernel]`, or adds a `tmm2d_*` or
`matmul2d` call to a kernel that carries it, names in the PR body each compiled variant (each
instantiation of a template kernel) whose A operand is float.** A float operand keeps the op off
its native fast path, and `float_a_ok=true` turns the MSL emitter's refusal of a float `matmul2d` A
operand off for every variant of the kernel.

**A diff that adds a global-rooted-array access, changes its index, moves it out from under a
guard that kept it inside its region, or moves it into a compiled variant it was not in, keeps
its index inside the region - the elements the code that allocates the buffer sizes for that
dispatch, not the dispatch's own bound - or inside slack.** A compiled variant is each stamp of a
kernel; a template several emitters compile is a variant under each, so an access moved onto a
shared template enters every emitter's variants, and a removed gate or a widened gate constant
moves an access into one. A guard on the access, a clamp into the region, a guard on the store
of a block the access loads whole, or a `static_if` or `@template_gate` that compiles the access
out of a variant satisfies the rule.

**A diff that changes the bound a dispatch hands its kernel, or the size a buffer a
global-rooted-array access indexes is allocated at, keeps every such access's index inside the
region the allocation sizes for that dispatch, or inside slack.**
