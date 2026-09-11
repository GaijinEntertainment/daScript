# dasLLAMA tests - Kernel Cells Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`,
`../followup_metal.md`.

**A diff another checklist routes here applies this list together with that checklist.**

**A diff that changes a kernel's dispatch geometry - a grid divisor or a threadgroup size -
updates every gate (a cell or probe that dispatches a kernel) that hand-dispatches that kernel,
in the same change.** A moved divisor leaves the gate dispatching the wrong shape with no error.

**A diff that gives a `[metal_dispatch]` kernel `@workgroup` state, or takes it away, updates
the threadgroup-memory length in every gate that hand-dispatches that kernel, in the same
change.** A gate that sets none for a kernel with `@workgroup` state reads garbage silently; a
change to the `@workgroup` array's own size needs no gate edit, and a Vulkan kernel's shared
memory is compiled in.

**A diff that changes a kernel's kargs - the kernel-argument struct, or any buffer binding -
re-checks every gate that hand-binds that kernel and updates each bind the change made stale,
in the same change.** A stale hand bind reads the wrong buffer and passes on garbage that
happens to compare.

**A kernel-unit cell - a model-less cell that dispatches one kernel class and asserts on its
output - missing a compare against a CPU oracle that can witness the cell's property is a
defect.**

**A kernel whose branch selection changes - a branch added, or an existing branch's predicate
widened or narrowed, so that a different set of kargs values, or of sentinel values in a bound
buffer, reaches a path - ships a kernel-unit cell that dispatches it at a value the change
moved onto or off that path, in the same change.** At every value the change left where it was
the kernel computes what it did before, so a cell that dispatches only those values passes
whether the change is right or wrong.

**Before every dispatch whose output a kernel-unit cell reads - directly, or through a later
dispatch in the same cell - the cell fills with a sentinel each of that dispatch's output
buffers that the dispatch does not also read as input.** An unprefilled output can pass by
staying stale - the previous dispatch's values, or garbage that happens to sit inside the
tolerance bar.

**A cross-dispatch bit-identity compare - comparing the outputs of two dispatches - runs GPU
against GPU.** No CPU oracle can witness that property.

**A kernel-unit cell whose output buffer is its input buffer, and whose CPU oracle does not
differ from that input by construction, pairs its compare with an assert that the output
differs from the input at a known index.** An in-place kernel that never ran leaves the input,
which can wrongly satisfy a tolerant compare.

**A kernel-unit cell that dispatches a `[metal_dispatch]` or `[vk_dispatch]` class no cell
dispatched before ships a control for that class, in the same change.** A control is a run of
the same gate that must RED - a poisoned input, a poisoned expectation, a disconnected
mechanism, or a second independent lane; a gate's own reference is never its control.

**A kernel-unit cell whose kernel computes at a narrower precision than its oracle at any
step - operands, accumulator, or the stored result - bounds that step's error by construction
(f16-exact inputs, magnitude-bounded fixtures) or states, in the cell or at the shared bar
helper the cell calls, how its bar follows from that step's error.** A bar moved without that
derivation is a loosening: the compare then measures the narrowing until it no longer
discriminates.

**A diff that changes a kernel's narrow step - the precision of its operands, its accumulator,
or its stored result - restates in every kernel-unit cell of that kernel what bounds that
step's error under the new step - the construction that bounds it, or how the cell's bar
follows from it - in the same change, whether or not the bar moves.**

**A gate for a kernel that attends inside a restricted horizon - a window, a sliding span, a
block-diagonal range - writes its CPU oracle to attend strictly inside that horizon.** A leak
then reds the ordinary compare, so the gate needs no separate leak control.

**A cell whose only compare is bit-identity between two kernel forms also compares one of the
two against a CPU oracle, in the same cell.** Two forms can be bit-equal and both wrong.
