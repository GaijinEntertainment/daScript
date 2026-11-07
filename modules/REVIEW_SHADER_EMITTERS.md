# Shader Emitters Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `dasMetal/ARCHITECTURE.md`, `dasMetal/ARCHITECTURE_PARITY.md`, `dasSpirv/ARCHITECTURE.md`,
`dasSpirv/ARCHITECTURE_COOPMAT.md`, `dasCUDA/ARCHITECTURE.md`, `dasCUDA/ARCHITECTURE_EMIT.md`,
`dasCUDA/ARCHITECTURE_EMIT_TILES.md`.

A kernel, in every rule below, is a function carrying `[metal_kernel]`, `[spirv_kernel]`,
`[cuda_kernel]`, or a `dasSpirv`, `dasVulkan` or `dasMetal` annotation whose name ends in `_shader`;
an emitter is the one in `dasSpirv` (SPIR-V words), `dasMetal` (MSL text) or `dasCUDA` (CUDA C++ text).

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
`daslib/shader_emit_common.das` - never lets a construct in a kernel body, or a value of a kernel
annotation argument, that the emitter has no lowering for produce a kernel or a crash - the
emitter reports a compile error that names the construct or the argument instead.**

**A diff that adds another lowering form for a construct an emitter already lowers - a different
emitted shape for the same construct - admits the new form only where it computes what the
construct's existing lowering computes; everywhere else the construct keeps its existing
lowering.**

**Never pass a shape constant to a kernel as a runtime argument where a call-site constant can
carry it - pass it as a call-site constant.** A shape constant is a value the emitter requires as
a compile-time constant: the extent of a `@workgroup` or local array (not a buffer the host
sizes), an unrolled loop's trip count, a stride into such an array, or a `tmm2d` op's M, N, or
the K width of the one chunk it steps over; a run-time count of live entries inside such a fixed
extent is not one. A call-site constant is fixed where the kernel's source is generated - a
template constant or a typedef - so the emitter bakes it in; a kernel whose source is generated
once and dispatched for differently shaped inputs cannot carry a shape that varies per input that
way.

**A diff that makes a matmul kernel's reduction width run-time-only, or changes an operand load
of a kernel whose width already is, passes the width to the loads only as a layout value -
`tensorLayoutSetDimension` on the `tensorLayout2D` or `tensorLayout2DPad` a `coopmatLoadTensor*`
load takes, or, for a `tmm2d_*` call, the `dynamic_extent` K of its `matmul2d_descriptor` or the A
operand's row stride (`lda`); only the accumulation loop's bound reads it from the kernel's
arguments.** The reduction width (K) is the length of the loop the kernel accumulates over; it
does not fix tiling, so the emitter needs no compile-time value for it.

**A diff that makes a kernel need a shape constant known only at run time ships a
specialization path - one code path per constant shape, a compiled variant or a branch whose shape
is a literal - or records the kernel as having none in an `ARCHITECTURE*.md` at the root of the
module it ships in.**

**A diff that makes a new claim, in a commit message, a PR body or a line of an
`ARCHITECTURE*.md`, about what a kernel's emitted words or text contain or leave out, or that a
kernel's emitted output did not change, reads the claim from the emitted artifact - `dasSpirv`'s
SPIR-V words, `dasMetal`'s MSL text, `dasCUDA`'s CUDA C++ text - never from the das source, and
names in its PR body the artifact read and the value read there, or, for a no-change claim, that
the artifacts before and after the diff matched.** A claim about a das construct alone, or about
a later compiler's or the driver's output - registers, spills, occupancy - names nothing an
emitted artifact shows; the `@workgroup` memory the text declares is in the artifact; a section
moved between architecture docs unchanged makes no new claim.

**A diff that teaches one emitter a kernel-model capability another lacks, or adds a kernel body
that uses a capability not every emitter has, adds that capability to the emitters lacking it or
leaves the shared ledger (`dasMetal/ARCHITECTURE_PARITY.md`) naming it, under the row that names
its family or a row the diff adds.** A kernel-model capability is a construct a kernel body may
use - an op, a type, an attribute, a memory class - that an emitter lowers; it is present on an
emitter when the emitted text or words carry its effect - an emitter that accepts the construct
and emits nothing for it does not have it - and a family is the set of constructs one ledger row
names.

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
its index inside the region - the elements the buffer's allocation sizes for that dispatch, not
the dispatch's own bound - or inside slack, an allocation past the region's end that the kernel's
module-root `ARCHITECTURE*.md` names.** A global-rooted-array access loads or stores an element of
a module global, a `@workgroup` array or a `self.<member>` resource, in a kernel body or any `def`
it reaches through calls; a tensor load or store accesses every element it covers. A compiled
variant is a kernel's instantiation under one emitter; adding a variant, moving an access onto a
shared template, or removing or widening a `static_if` or `@template_gate` moves accesses into new
variants.

**A load or store of an element of a module global, a `@workgroup` array or a `self.<member>`
resource counts as inside its region, with no index checked against the allocation, when a guard
on it, a clamp into the region, a guard on the store of a block it loads whole, or a `static_if`
or `@template_gate` that compiles it out keeps it there - or when it sits in a new variant of a
template, under an emitter that compiled no variant of it, whose buffers are sized through the
same size function at the same arguments, and whose dispatch hands it the same bound, as an
existing variant of that template whose accesses stay inside their regions.**

**A diff that changes what an emitter lowers so the emitted text reads an element the kernel
source does not name keeps that element inside the extent the kernel source's tensor layout or
guard sets.** A speculative window a lowering adds is a read the source never guarded.

**A diff that changes the bound a dispatch hands its kernel, or the size a buffer is allocated
at, keeps every load or store of an element of a module global, a `@workgroup` array or a
`self.<member>` resource - by that kernel, or of that buffer, in a kernel body or any `def` it
reaches through a chain of calls - inside the region the allocation sizes for that dispatch, or
inside slack.** A tensor load or store accesses every element it covers; slack is an allocation
past a region's end that the kernel's module-root `ARCHITECTURE*.md` names.
