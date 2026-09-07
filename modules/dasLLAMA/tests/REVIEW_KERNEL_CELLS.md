# dasLLAMA tests - Kernel Cells Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`,
`../followup_metal.md`.

**Routed from `REVIEW.md` (beside this file): a diff that checklist routes here applies this
list together with it.**

**A diff that changes a kernel's dispatch geometry - its grid divisor, threadgroup size, or
threadgroup-memory length - updates every gate that hand-dispatches that kernel, in the same
change.** A hand-dispatched gate encodes the geometry itself, so a moved divisor leaves the
gate dispatching the wrong shape with no error.

**A diff that changes a kernel's kargs - the kernel-argument struct, or any buffer binding -
updates every gate that hand-binds that kernel, in the same change.** A stale hand bind reads
the wrong buffer and passes on garbage that happens to compare.

**A kernel that gains a kargs field whose non-default value changes what it computes or which
elements it reads or writes - a branch selector, a row or element base, a stride - ships, in
the same change, a gate cell that sets that field to a non-default value.** At the default the
new field has no visible effect: a CPU oracle that ignores it and the kernel that honors it
agree.

**A kernel-unit cell - a model-less cell that dispatches one kernel class and asserts on its
output - missing a compare against a CPU oracle that can witness the cell's property is a
defect.**

**A kernel-unit cell fills a GPU output buffer with a sentinel before every dispatch whose
output it then reads.** An unprefilled output can pass by staying stale - the previous
dispatch's values, or garbage that happens to sit inside the tolerance bar.

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

**A kernel-unit cell whose kernel reads f16 operands and whose oracle is wider-precision
feeds inputs that are exact in f16.** Otherwise the compare measures input rounding, and the
bar has to be loosened until it no longer discriminates.

**A gate for a kernel that attends inside a restricted horizon - a window, a sliding span, a
block-diagonal range - writes its CPU oracle to attend strictly inside that horizon.** A leak
then reds the ordinary compare, so the gate needs no separate leak control.

**A cell whose only compare is bit-identity between two kernel forms also compares one of the
two against a CPU oracle, in the same cell.** Two forms can be bit-equal and both wrong.
