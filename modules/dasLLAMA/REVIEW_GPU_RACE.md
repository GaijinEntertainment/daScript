# dasLLAMA GPU Race Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_MEASUREMENT.md`. Planned work:
`followup_metal.md` for Metal, `followup_vulkan.md` for Vulkan.

**Routed here by another checklist: a diff that checklist routes here applies this list
together with it.**

**A hand-binding arm that binds a field at a position the class does not declare for that field
is a defect.** A hand-binding arm is a race or knockout timing arm - a race times two
implementations of one computation on one queue and compares their outputs, a knockout skips a
stage to measure that stage's cost - that mirrors a class's binding order by hand, with a
literal bind number or a positional buffer and size array, instead of naming the class's fields.
A mis-bound arm dispatches, reads the wrong buffer, and its timing selects the wrong kernel
silently.

**A hand-binding arm outside `dasllama/`, or one whose pipeline source or threadgroup-memory
size arrives as a function parameter rather than a literal global, states in the PR that its
binding order and push-constant layout were verified by hand against the class declaration.**
The `REVIEW.das` gate `check_race_bind_numbers` cannot read those arms, and an arm nothing
checked is where a mis-numbered bind reaches the board.

**A diff that changes a kernel's binding numbers, kargs (kernel-argument struct) layout,
threadgroup memory, staging shape (the operand tile a kernel copies into threadgroup memory
before it computes), or grid or threadgroup geometry resyncs or deletes, in the same change,
every arm that mirrors that kernel's binding order by hand and every arm ledgered as a
retained reference in `ARCHITECTURE_GPU.md` sec.2.2b (Metal) or `ARCHITECTURE_MEASUREMENT.md`
sec.2.5 (Vulkan).** An arm left dispatching stale geometry measures the wrong kernel silently.

**Race and knockout code inside the engine (`dasllama/`) sits in the file that owns the kernel
family it races, or - for a knockout - the file that owns the stage whose cost it removes.**

**Scaffolding that race sites in two DIFFERENT engine files share sits in
`dasllama/dasllama_<gpu>_common.das`.** Scaffolding two races in one file share stays in that
file.

**Race code sizes its operands at a real model shape - never at a small square slab.** A slab
small enough to sit in cache ranks the kernels by an effect production never sees, and the
race then picks the slower kernel.

**A timing arm whose ranking a checked-in document, box profile or sidecar records as decided
times its kernel at every fixed width it is dispatched at: every width one speculative-verify
batch runs at; every row count from 2 to `MTP_MAX_ROWS - 1` (`dasllama/dasllama_common.das`)
where the batched decode driver (`dasllama/dasllama_<gpu>_decode.das`) dispatches it per row
count; the tile's own width where it is dispatched at one fixed width; each power of two it
spans on a power-of-two batch grid.** A ranking timed at one width alone is applied at widths it
was never ranked at.

**A timing arm for a prefill tile over a variable window, whose ranking a checked-in document,
box profile or sidecar records as decided, times its kernel at one window whose token count is
a whole multiple of the tile's row count and at one where it is not.** The short last tile is
what makes the tile take its partial-tile store path.

**An `ARCHITECTURE_GPU.md` sec.2.2b entry for a kernel ranked on a power-of-two batch grid names
that grid.**

**A kernel A/B race arm whose ranking a checked-in document, box profile or sidecar records as
decided binds a different output buffer for consecutive dispatches of its chain, never one
shared output.** One shared output serializes the chain on its write-after-read hazard while the
served graph overlaps consecutive dispatches, so the race ranks the arms on a shape production
never runs.

**Every arm of a kernel A/B race whose ranking a checked-in document, box profile or sidecar
records as decided handles the hazard between its dispatches the same way.** An arm serialized
by a barrier races an arm that overlaps.

**Weakening the burn phase of `race_pair_ms` (`dasllama/dasllama_metal_common.das`) - the GPU
work it runs on both arms before the first timed round - is a defect.**

**A race whose ranking a checked-in document, box profile or sidecar records as decided and that
does not go through `race_pair_ms` burns GPU work on both arms before the first timed round.**
The arms alternate, so a first round on a cold GPU clock charges the clock's ramp-up to
whichever arm ran first.

**A timed encoder in a race arm whose ranking a checked-in document, box profile or sidecar
records as decided issues its dispatches back to back.** An encoder that leaves gaps between
its dispatches times an idle clock.

**A diff that ports an A/B lab's winning variant - an A/B lab is a timing script that picks
between spellings of one compute - into a kernel deletes, in the same change, that variant's
class and any `*_variants.das` code that exists only for it.**

**An arm that survives a port of the winning variant of an A/B lab - a timing script that picks
between spellings of one compute - into a kernel dispatches the shipped kernel class's generated
source, or is ledgered as a retained reference in the architecture doc that owns the kernel's
tier: `ARCHITECTURE_GPU.md` sec.2.2b for Metal, `ARCHITECTURE_MEASUREMENT.md` sec.2.5 (the
Vulkan GEMM probe's axes).** A decided arm that outlives its decision degrades into an
unmaintained duplicate of the kernel it seeded.

**A diff that leaves an A/B lab - a timing script that picks between spellings of one compute -
with no undecided arm - every arm's ranking recorded as decided by a checked-in document, box
profile or sidecar - deletes the lab's driver and its remaining arm in the same change.**
