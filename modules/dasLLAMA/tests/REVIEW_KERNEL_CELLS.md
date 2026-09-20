# dasLLAMA tests - Kernel Cells Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`,
`../followup_metal.md`.

**A diff that changes a kernel's dispatch grid - the count its dispatch needs, or its workgroup
size - updates every gate that dispatches that kernel, in the same change; a gate is a cell or
probe that dispatches or binds a kernel by hand rather than through the generated builders.** A
gate left on the old count dispatches the wrong shape with no error.

**A diff that gives a `[metal_dispatch]` kernel `@workgroup` state, or takes it away, updates
the threadgroup-memory length in every gate that hand-dispatches that kernel, in the same
change.** A gate that sets none for a kernel with `@workgroup` state reads garbage silently.

**A diff that changes the size of a `[metal_dispatch]` stamp's `@workgroup` array - a stamp
being one leaf class of a kernel, whose overridden constants set that size - updates, in the
same change, every gate that hand-dispatches a different stamp while reading this stamp's
`*_tgmem` global for its threadgroup-memory length.**

**A diff that changes a kernel's kargs - the kernel-argument struct, any buffer binding, or the
layout of a struct a bound buffer holds - updates every hand-bind of that kernel the change made
stale, in the same change.** A stale hand bind reads the wrong buffer and passes on garbage that
happens to compare.

**A kernel-unit cell - a model-less cell that dispatches one or more kernel classes and asserts on
their output - missing a compare against a CPU oracle in the cell itself is a defect; where the
cell compares two kernel forms against each other, the oracle compare targets one of those two
forms, in that cell or in a cell of the same file that dispatches that form at the same shapes.**
A cell is a `t |> run` block, or a helper that asserts on `t`; a CPU oracle is the same
computation written in plain code and run on the CPU. Two forms can be bit-equal and both wrong.

**A kernel whose branch selection changes - a branch added, or an existing branch's predicate
widened or narrowed, so that a different set of kargs values, or of sentinel values in a bound
buffer, reaches a path - has a kernel-unit cell that dispatches it at a value the change moved
onto or off that path: an existing cell that already does discharges it and the change says
which; otherwise the cell ships in the same change.** At every value the change left where it was
the kernel computes what it did before, so a cell that dispatches only those values passes
whether the change is right or wrong.

**Before every dispatch whose output a kernel-unit cell reads - directly, or through a later
dispatch in the same cell - the cell fills with a sentinel every range of that dispatch's output
buffers the dispatch writes without reading.** An unprefilled output can pass by staying stale -
the previous dispatch's values, or garbage that happens to sit inside the tolerance bar.

**A bit-identity assert whose result each side computes with floating-point arithmetic, in an
operation order the cell does not fix on both sides, compares two GPU dispatches - never a
dispatch against a CPU oracle.** With the order fixed on both sides the oracle's result is the
kernel's by construction; an exact compare of indices or schedule words against a CPU twin is not
that assert.

**A kernel-unit cell whose output buffer is its input buffer, and whose CPU oracle does not
differ from that input by construction, pairs its compare with an assert that the output
differs from the input at a known index.** An in-place kernel that never ran leaves the input,
which can wrongly satisfy a tolerant compare.

**A kernel-unit cell that dispatches a `[metal_dispatch]` or `[vk_dispatch]` class no cell
dispatched before ships a control for that class, in the same change.** A control is an extra
assert in the same cell proving the compare can fail - a poisoned input or a poisoned expectation
that must land outside the bar, a mechanism unhooked whose result must miss, or a second
independent lane the result must agree with; the cell's own reference is never its control.

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
