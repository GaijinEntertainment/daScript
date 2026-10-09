# dasLLAMA tests - Kernel Cells Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE_GPU.md` for a `[vk_dispatch]` or `[metal_dispatch]` kernel,
`../ARCHITECTURE_CPU_KERNELS.md` for a CPU kernel in `../dasllama/dasllama_math*.das`. Planned
work: `../followup_general.md`, `../followup_vulkan.md`, `../followup_metal.md`.

A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest; a helper's asserts
belong to every cell that calls it. A CPU kernel is a function that writes an output buffer from
operand buffers; one that answers a size, a block or a layout is not. To loosen an assert is to let
pass an input the old assert failed. A kernel-unit cell is a cell in which a `[metal_dispatch]` or
`[vk_dispatch]` class is dispatched, or a CPU kernel in `../dasllama/dasllama_math*.das` is called,
by a statement in test code - the cell itself, or a helper in its own file or in a `_*.das`
test-helper module - itself or through a router, a `../dasllama/` function whose only job is to
pick which compiled form (stamp) of the dispatched class or the CPU kernel runs; a kernel run
inside any other `../dasllama/` function does not make a cell a kernel-unit cell. A kernel run is a
dispatch or a CPU kernel call. A gate is any call site that supplies a kernel's dispatch count or
fills its kargs (kernel-argument) struct itself, other than a serving call in `../dasllama/` the
kernel exists for - a cell, a probe, a harness. A stamp is a `[vk_dispatch]` or
`[metal_dispatch]` class that compiles to its own shader module - a class no other class derives
from; every rule here treats each stamp as its own class. A CPU oracle is the same computation
written in plain code and run on the CPU. A path of a class is each branch a value read from the
kargs or from any bound buffer selects, and each trip-count regime of each loop whose bound such a
value sets - the regimes read against the loop's stride: zero, under one stride, a whole number of
strides, and past a whole number of strides (the tail). A branch or loop inside a function the
class's methods call, other than a method of the class, is not a path of the class - covering it
is the job of that function's cells. A branch or regime is a path only where a serving call in
`../dasllama/` can reach it on some model the engine serves. A bound assert is an assert whose pass
means two sides agree or a figure is good enough - a difference between two computed sides, a
rate, an error, or a count the run decides, within a nonzero tolerance or past a floor or ceiling;
a guard that keeps a later index or compare well-defined (a length past the index it reads) is not
one, and a counter asserted to show a route ran is not one. A control is an extra assert
in the same cell proving a compare can fail - a poisoned input, or a poisoned expectation (the
expected value with a known amount added), that must land outside the bar, a mechanism unhooked
whose result must miss, or a second, independent computation the result must agree with; a cell's
own reference is never its control.

**A diff that changes a kernel's dispatch grid (the count its dispatch needs, or its workgroup
size) or its kargs (the struct, any buffer binding, or the layout of a struct a bound buffer holds)
updates every gate that dispatches that kernel, in the same change.** A gate left on the old count
dispatches the wrong shape with no error; a stale kargs gate reads the wrong buffer and passes on
garbage that happens to compare.

**A diff that changes a Metal kernel's `@workgroup` state or the size of a stamp's `@workgroup`
array updates, in the same change, every gate that dispatches that kernel and every gate that
reads the stamp's `*_tgmem` global for another stamp's threadgroup-memory length.** A gate that
sets no threadgroup memory for a kernel with `@workgroup` state reads garbage silently.

**A diff that changes the rows or elements a kernel's body writes updates, in the same change,
every kernel-unit cell expectation over that kernel's output.** An expectation over rows the kernel
no longer writes compares the sentinel to a sum.

**A diff that dispatches an existing `[vk_dispatch]` or `[metal_dispatch]` class at a workgroup
size or dispatch grid no kernel-unit cell runs it at ships a kernel-unit cell at that shape, in the
same change.**

**A kernel-unit cell's compare of kernel output against its reference logs, whether it passes or
fails, the largest difference it measured with that element's row and column; an exact compare
logs its mismatch count and first mismatching index instead.** A failure, or a pass that looks
wrong, must say how far and where, not only how many.

**A kernel-unit cell with a kernel run whose output no CPU-oracle compare covers at that run's
shapes is a defect: the compare sits in that cell, or - where the cell compares two kernel forms
against each other - in a cell of the same file that dispatches one of those two forms at the same
shapes.** Two forms can be bit-equal and both wrong.

**A diff that changes an existing `[vk_dispatch]` or `[metal_dispatch]` class's branch
selection - a branch added, or a predicate widened or narrowed, so that a different set of kargs
values, or of sentinel values in a bound buffer, reaches a path - ships a kernel-unit cell that
dispatches that class at a value the change moved onto or off that path, or names the existing
cell that already does.** A cell that dispatches a sibling stamp - another stamp derived from the
same class - counts only when no constant or method either stamp overrides appears in the moved
path. At every value the change left where it was the kernel computes what it did before, so a
cell that dispatches only those values passes whether the change is right or wrong.

**A diff that adds a `[vk_dispatch]` or `[metal_dispatch]` class ships a kernel-unit cell that
dispatches it on every path its body has.** A path no cell reaches is one a merge can drop and
every cell still passes.

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

**A kernel-unit cell whose output buffer is its input buffer asserts, at a known index, that the
output differs from the input or that its reference - its CPU oracle, or the second kernel form
it compares against - differs from that input by more than the compare's bar.** An in-place kernel
that never ran leaves the input, which can wrongly satisfy a tolerant compare.

**A diff that adds or loosens, in a kernel-unit cell, a bound assert that is not itself a control -
a compare on a class or at a kargs value, a rate, an error, a count - ships a control for each such
assert, in the same change.**

**A size, depth, or row count that a kernel-unit cell's name, a comment inside the cell, or an
assert's text claims, and that the cell passes as a literal argument to the kernel it dispatches
or calls, is asserted in that cell by an assert on the count or by comparing every output element
that count covers against the oracle.** A count the dispatch pads, clamps or derives from that
literal gets its own assert.

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
writes past it - never to the dispatch's row count.** A one-row
dispatch whose row sits in slot 1 writes past a one-slot buffer, and no error says so.
