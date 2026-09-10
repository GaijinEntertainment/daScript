# dasLLAMA tests - Kernel Cells Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`,
`../followup_metal.md`.

**Routed here by another checklist: a diff that checklist routes here applies this list
together with it.**

**A diff that changes a kernel's dispatch geometry - a grid divisor, a threadgroup size, or a
`@workgroup` array the kernel gains or loses - updates every gate (a cell or probe that
dispatches a kernel) that hand-dispatches that kernel, in the same change.** A moved divisor
leaves the gate dispatching the wrong shape with no error, and a gate that sets no
threadgroup-memory length for a kernel with `@workgroup` state reads garbage silently; the
array's size reaches the gate through the generated `*_msl_tgmem` constant, so a size change
alone needs no gate edit.

**A diff that changes a kernel's kargs - the kernel-argument struct, or any buffer binding -
updates every gate that hand-binds that kernel, in the same change.** A stale hand bind reads
the wrong buffer and passes on garbage that happens to compare.

**A kernel-unit cell - a model-less cell that dispatches one kernel class and asserts on its
output - missing a compare against a CPU oracle that can witness the cell's property is a
defect.**

**A kernel whose branch selection changes - a branch added, or an existing branch's predicate
widened or narrowed, so that a different set of kargs values, or of sentinel values in a bound
buffer, reaches a path - ships a kernel-unit cell that dispatches it at a value the change
moved onto or off that path, in the same change.** At every value the change left where it was
the kernel computes what it did before, so a cell that dispatches only those values passes
whether the change is right or wrong.

**A kernel-unit cell fills a GPU output buffer the dispatch does not also read as input with a
sentinel before every dispatch whose output it then reads.** An unprefilled output can pass by
staying stale - the previous dispatch's values, or garbage that happens to sit inside the
tolerance bar.

**A cross-dispatch bit-identity compare - comparing the outputs of two dispatches - runs GPU
against GPU.** No CPU oracle can witness that property.

**A kernel-unit cell whose output buffer is its input buffer, and whose CPU oracle does not
differ from that input by construction, pairs its compare with an assert that the output
differs from the input at a known index.** An in-place kernel that never ran leaves the input,
which can wrongly satisfy a tolerant compare.

**A kernel-unit cell that dispatches a `[metal_dispatch]` or `[vk_dispatch]` class no cell
dispatched before ships a control that reds it in the same change.** A control is a run of
the same gate that must RED - a poisoned input, a poisoned expectation, a disconnected
mechanism, or a second independent lane; a gate's own reference is never its control.

**A kernel-unit cell whose kernel computes at a narrower precision than its oracle at any
step - operands, accumulator, or the stored result - bounds that step's error by construction
(f16-exact inputs, magnitude-bounded fixtures) or states in the cell how its bar follows from
that step's error.** A bar moved without that derivation is a loosening: the compare then
measures the narrowing until it no longer discriminates.

**A diff that changes a kernel's narrow step - the precision of its operands, its accumulator,
or its stored result - restates in every kernel-unit cell of that kernel what bounds that
step's error under the new step - the construction that bounds it, or how the cell's bar
follows from it - in the same change, whether or not the bar moves.**

**A gate for a kernel that attends inside a restricted horizon - a window, a sliding span, a
block-diagonal range - writes its CPU oracle to attend strictly inside that horizon.** A leak
then reds the ordinary compare, so the gate needs no separate leak control.

**A cell whose only compare is bit-identity between two kernel forms also compares one of the
two against a CPU oracle, in the same cell.** Two forms can be bit-equal and both wrong.
