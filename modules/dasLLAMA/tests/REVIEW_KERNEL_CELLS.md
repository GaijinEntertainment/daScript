# dasLLAMA tests - Kernel Cells Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE_GPU.md`, `../ARCHITECTURE_CPU_KERNELS.md`. Planned work:
`../followup_general.md`, `../followup_vulkan.md`, `../followup_metal.md`.

- A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest; a helper's
  asserts belong to every cell that calls it.
- A CPU kernel is a function in `../dasllama/dasllama_math*.das` that writes an output buffer from
  operand buffers.
- A CPU oracle is the same computation written in plain code in this checklist's folder - a cell,
  or a `_*.das` helper module here; only its operands may be built, quantized, widened or read
  through `../dasllama/` functions, and a kernel under `../dasllama/` is never one.
- A sentinel is a value the kernel cannot write from the cell's own inputs - a NaN where every
  input is finite, or a fixed bit pattern no finite input yields; a zero-filled or newly allocated
  buffer is not one.
- To loosen an assert is to let pass an input the old assert failed.
- A kernel class is a class that a GPU dispatch annotation - `[metal_dispatch]`, `[vk_dispatch]`
  or `[cuda_dispatch]` - compiles to a device kernel.
- A home is the GPU backend a stamp runs on - Metal, Vulkan or CUDA.
- A kernel-unit cell is a cell in which a kernel class is dispatched, or a CPU kernel is called,
  by a statement in test code outside a CPU oracle -
  the cell itself, or a helper in its own file or in a `_*.das` test-helper module - itself or
  through a router, a `../dasllama/` function whose only job is to pick which compiled form
  (stamp) of the dispatched class or the CPU kernel runs; a kernel run inside any other
  `../dasllama/` function does not make a cell a kernel-unit cell.
- A kernel run is a dispatch or a CPU kernel call.
- A gate is any call site that supplies a kernel's dispatch count or fills its kargs
  (kernel-argument) struct itself, other than a serving call in `../dasllama/` the kernel exists
  for - a cell, a probe, a harness.
- A stamp is a kernel class that compiles to its own shader module - a class no other class
  derives from; every rule here treats each stamp as its own class.
- A path of a kernel class or a CPU kernel is each branch a value read from its arguments, its
  kargs or any buffer it reads selects, and each trip-count regime of each loop whose bound such a
  value sets - read against the loop's stride: zero, under one stride, a whole number of strides,
  and past a whole number of strides (the tail). A branch or loop inside a function the kernel
  calls, other than a method of the class, is not its path - covering it is the job of that
  function's cells. A branch or regime is a path only where a serving call in `../dasllama/` can
  reach it on some shape that call does not decline, whether or not any model file the box holds
  has that shape.
- A bound assert is a compare of two computed sides, or a measured quality figure against a
  tolerance; a floor on a capacity, a configuration or a roster size is not one, a guard that keeps
  a later index or compare well-defined (a length past the index it reads) is not one, a counter
  asserted to show a route ran is not one, and an assert a rule of this list requires to show a
  compare can fail - the in-place cell's output-differs-from-input assert among them - is not a
  bound assert, and is not the control a bound assert needs.
- A sibling stamp is another stamp derived from the same class; it stands in for a stamp only where
  the code a rule asks about is the class's own, neither stamp overrides it - overriding a template
  constant that code reads overrides it - the sibling's cell compiles once the diff is applied, and
  the two stamps are in the same home or the stamp is CUDA's and the sibling Vulkan's.
- A control is an extra assert in the same cell proving a compare can fail - a poisoned input, or a
  poisoned expectation (the expected value with a known amount added), that must land outside the
  bar, a mechanism unhooked whose result must miss, or a second, independent computation the result
  must agree with; a cell's own reference is never its control.

**A diff that changes a kernel's dispatch grid (the count its dispatch needs, or its workgroup
size) or its kargs (the struct, any buffer binding, or the layout of a struct a bound buffer holds)
corrects or deletes, in the same change, every gate dispatching that kernel whose dispatch count,
workgroup size, buffer bindings, kargs values or bound-struct contents the change made wrong.** A
gate left on the old count dispatches the wrong shape with no error; a stale kargs gate reads the
wrong buffer and passes on garbage that happens to compare.

**A diff that changes a Metal kernel's `@workgroup` state or the size of a stamp's `@workgroup`
array updates, in the same change, every gate that dispatches that kernel and every gate that
reads the stamp's `*_tgmem` global for another stamp's threadgroup-memory length.** A gate that
sets no threadgroup memory for a kernel with `@workgroup` state reads garbage silently.

**A diff that changes the rows or elements a kernel's body writes updates, in the same change,
every kernel-unit cell expectation over that kernel's output.** An expectation over rows the kernel
no longer writes compares the sentinel to a sum.

**A diff that dispatches an existing kernel class at a workgroup size no kernel-unit cell runs it
at, or with a grid formula (the expression that computes its dispatch count from the call's sizes)
no kernel-unit cell computes its count with, ships a kernel-unit cell that dispatches it at that
workgroup size, or whose dispatch count equals what that formula computes from the cell's sizes, in
the same change.**

**A kernel-unit cell's compare of kernel output against its reference logs, whether it passes or
fails, the largest difference it measured with that element's row and column, or, for an output
with one value per row (a max, a denominator), the largest difference and its row; an exact
compare - a sentinel scan, a per-row flag - logs its mismatch count and first mismatching index
(for a per-row output, its row) instead.** A failure, or a pass that looks wrong, must say how far
and where, not only how many.

**A kernel-unit cell holds a CPU-oracle compare of every kernel run's output at that run's
shapes - every size a dispatch grid or loop bound of that run reads, the row count included - or
compares the run against a second kernel form that a cell of the same file holds to a CPU
oracle.** Two forms can be bit-equal and both wrong.

**A kernel-unit cell that compares a kernel run against a second kernel form has, in the same
file, a cell that runs one of the two forms - directly or through any `../dasllama/` call - at the
same shapes against a CPU oracle, and that runs on every box and every build the comparing cell
runs on.** A companion that skips where the comparing cell runs leaves that run held to nothing
but another kernel form.

**A diff that changes an existing kernel class's or CPU kernel's branch selection - a branch added,
or a predicate widened or narrowed, so that a different set of kargs or argument values, or of
marker values a bound or operand buffer holds, reaches a path - ships a kernel-unit cell that runs
that kernel at a value the change moved onto or off that path, or names the existing cell that
already does.** A sibling stamp's cell counts. At every value the change left in place the kernel
computes what it did before, so a cell that runs only those values passes whether the change is
right or wrong.

**A diff that adds a kargs field to a kernel class, or changes what an existing field means, so that
a stamp reads or writes different elements, ships for each such stamp a kernel-unit cell that
dispatches it, or a sibling stamp, with the field at a value a serving call in `../dasllama/` can
pass and at which the elements read or written differ from the ones before the change - for an
added field, never 0 and never the value a serving call that does not use the field passes.** At
the value where nothing moved, a cell passes whether the new indexing is right or wrong.

**A diff that adds a parameter to a CPU kernel, or changes what one means, so that the kernel
reads or writes different elements or scales them differently, ships a kernel-unit cell that calls
it with that parameter at a value a serving call can pass and at which the result differs from the
one before the change - never 0, 1, or the value a serving call that does not use it passes.** At a
value where nothing moved, the cell passes whether the change is right or wrong.

**A diff that adds a kernel class or a CPU kernel ships a kernel-unit cell that runs it on every path
its body has - a sibling stamp's cell counts, and for a stamp whose override changes only
`static_if` arms, none holding a branch or loop a kargs or bound-buffer value sets, a cell counts
that dispatches it at the arguments of sibling cells that together reach every sibling path.** A
merge can drop a path no cell reaches, and every cell still passes.

**Before every kernel run whose output a kernel-unit cell reads in any assert, directly or through
a later run in the same cell, the cell fills with a sentinel every range of that run's output
buffers that the run writes without first reading what the range held before the run; a range the
run writes and then reads back is one of them.** A stale value is the previous run's, or garbage
that happens to sit inside the tolerance bar; an unprefilled output can pass by keeping it.

**A compare of a kernel against a CPU oracle asserts bit-identity only where the cell fixes the
floating-point operation order on both sides, or builds operands that keep every intermediate
exact; any other kernel-vs-oracle compare holds a tolerance bar.** With the order fixed on both
sides, or the operands exact by construction, the oracle's result is the kernel's by
construction; an exact compare of indices or schedule words against a CPU oracle is not a
bit-identity assert.

**A kernel-unit cell whose output buffer is its input buffer, and whose compare passes within a
nonzero tolerance, asserts, at a known index, that the output differs from the input or that its
reference - its CPU oracle, or the second kernel form it compares against - differs from that
input by more than that tolerance.** An in-place kernel that never ran leaves the input, which
can wrongly satisfy a tolerant compare.

**A diff that adds or loosens, in a kernel-unit cell, a bound assert ships a control for each such
assert, in the same change - a control that runs with that assert on every call.**

**A size, depth, row count or block offset that a kernel-unit cell's name, a comment inside the
cell, or an assert's text claims, and that the cell passes as a literal argument to the kernel it
dispatches or calls, is asserted in that cell - by an assert that a figure the run produces (the
count it wrote, the offset it read or wrote at) equals that value, or by comparing against the
oracle every output element the value sizes or places.**

**A kernel-unit cell whose dispatch covers more lanes or rows than the size the cell claims sizes
its output buffer to the dispatch's full lane or row count, fills the elements past the claimed
size with a sentinel, and asserts each of them still holds the sentinel after the run - or, for a
kernel whose contract writes those elements (a class that zeroes a padded panel), asserts each
holds the value that contract names, and holds the elements past the dispatch's own extent at the
sentinel.**

**A kernel-unit cell whose kernel computes at a narrower precision than its oracle at any
step - operands, accumulator, or the stored result - bounds that step's error by construction
(f16-exact inputs, magnitude-bounded fixtures), or states how its bar follows from that step's
error, in the cell or at the shared bar helper the cell calls.** A bar with no derivation admits
whatever result it was set to pass.

**A diff that changes where a kernel computes at a narrower precision than its oracle - the
operands, the accumulator, or the stored result - restates, in every kernel-unit cell of that
kernel and in the same change, what holds that cell's compare: the construction that bounds the
new step's error, or how its bar follows from that error, whether or not the bar moves.**

**A kernel-unit cell for a kernel that attends inside a restricted horizon - a window, a
sliding span, a block-diagonal range - writes its CPU oracle to attend strictly inside that
horizon.** A leak then fails the ordinary compare, so the cell needs no separate leak control.

**A kernel-unit cell that binds a buffer its kernel indexes by a slot id or an element offset read
from a field of the row's `TokMeta` block sizes that buffer to cover every row's range - through
the highest slot id plus one slot, or through the highest offset plus the span the kernel reads or
writes past it - never to the dispatch's row count.** A one-row dispatch whose row sits in slot 1
writes past a one-slot buffer, and no error says so.
