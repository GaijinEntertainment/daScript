# dasLLAMA GPU Race Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_GPU.md`. Planned work: `followup_metal.md`.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with it.**

**A hand-binding arm - a race or knockout timing arm (a knockout attributes cost across
stages instead of selecting between implementations) that lists its bindings by number
instead of dispatching through the `enc_*` builder - that binds a field at a number the
class does not declare for that field is a defect.** A mis-numbered arm dispatches, reads
the wrong buffer, and its timing selects the wrong kernel silently.

**A hand-binding arm outside `dasllama/`, or one whose pipeline source or threadgroup-memory
size arrives as a function parameter rather than a literal global, states in the PR that its
bind numbers and kargs word layout were verified by hand against the class declaration.**
The `REVIEW.das` gate `check_race_bind_numbers` cannot read those arms, and an arm nothing
checked is where a mis-numbered bind reaches the board.

**A diff that changes anything a hand-binding arm must mirror to dispatch a kernel - binding
numbers, kargs (kernel-argument struct) layout, threadgroup memory, staging shape, grid or
threadgroup geometry - fixes or deletes every such arm in the same change.** An arm left
dispatching stale geometry measures the wrong kernel silently.

**Race and knockout code inside the engine (`dasllama/`) sits in the file that owns the kernel
family or the stage it measures, and never in another engine file.**

**Scaffolding that race sites in two DIFFERENT engine files share sits in
`dasllama/dasllama_<gpu>_common.das`.** Scaffolding two races in one file share stays in that
file.

**Race code sizes its operands at a real model shape - never at a small square slab.** A slab
small enough to sit in cache ranks the kernels by an effect production never sees, and the
race then picks the slower kernel.

**A timing arm whose ranking is recorded as a decision - a runtime crown, a tune-sidecar row,
or an `ARCHITECTURE_GPU.md` sec.2.2b entry - times its kernel at every verify width, every row
count from 2 to `MTP_MAX_ROWS - 1` (`dasllama/dasllama_common.das`), when the batched decode
driver (`dasllama/dasllama_<gpu>_decode.das`) dispatches that kernel or the decision concerns
those widths.** A ranking timed at one width alone is applied at widths it was never ranked at.
A kernel dispatched at one fixed width per tile (a two-row tile walked over pairs, a tensor
tile of fixed M) is timed at that width; an instrument built on a power-of-two batch grid times
the powers of two it spans, and the entry names the grid.

**A kernel A/B race arm that mints a runtime crown or a tune-sidecar row binds a DIFFERENT
output buffer for consecutive dispatches of its chain, never one shared output.** One shared
output serializes the chain on its write-after-read hazard while the served graph overlaps
consecutive dispatches, so the race ranks the arms on a shape production never runs.

**Weakening the burn phase of `race_pair_ms` (`dasllama/dasllama_metal_common.das`) - the GPU
work it runs on both arms before the first timed round - is a defect.**

**A race arm that mints a runtime crown or a tune-sidecar row and does not go through
`race_pair_ms` burns GPU work on its own arms before its first timed round.** The arms
alternate, so a first round on a cold GPU clock charges the clock's ramp-up to whichever arm
ran first.

**A timed encoder in a race arm that mints a runtime crown or a tune-sidecar row issues its
dispatches back to back.** An encoder that leaves gaps between its dispatches times an idle
clock.

**A diff that ports an A/B lab's winning variant into a kernel deletes, in the same change,
that variant's class and any `*_variants.das` code that exists only for it; an arm that
survives the port dispatches the shipped kernel class's generated source, or is ledgered as
a retained reference in `ARCHITECTURE_GPU.md` sec.2.2b.** An A/B lab is a timing script
whose output selects between implementations of the same compute; a decided arm that
outlives its decision degrades into an unmaintained duplicate of the kernel it seeded.

**A diff that leaves an A/B lab with no undecided arm deletes the lab's driver and its
remaining arm in the same change.**

**A diff that moves a shipped kernel's staging shape or its grid or threadgroup geometry
resyncs or deletes, in the same change, every arm `ARCHITECTURE_GPU.md` sec.2.2b ledgers as a
retained reference that reimplements that kernel.**
