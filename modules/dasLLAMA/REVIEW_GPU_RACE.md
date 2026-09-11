# dasLLAMA GPU Race Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_MEASUREMENT.md`. Planned work:
`followup_metal.md` for Metal, `followup_vulkan.md` for Vulkan.

A race times two implementations of one computation on one queue; a knockout skips a stage to
measure that stage's cost; an overhead arm times one chain with and without an interposed stage -
a timestamp, a barrier, a flush - to measure that stage, and is not a race. An arm's ranking is
decided when a checked-in document, box profile or sidecar records it. An A/B lab is a timing
script that picks between spellings of one compute.

**A diff another checklist routes here applies this list together with that checklist.**

**A hand-binding arm that binds a field at a position the class does not declare for that field
is a defect.** A hand-binding arm is a race or knockout timing arm that restates a bind order no
generated setter checks (Metal `kn_buffer(enc, n)` calls, a probe class redeclaring `@binding`
slots), instead of naming the class's fields. A mis-bound arm dispatches, reads the wrong buffer,
and its timing selects the wrong kernel silently.

**An ordered argument list into a generated setter (`set_...(bufs, sizes, gbits)`, the
`[vk_dispatch]` lens's set builder) whose entry count and order do not match the distinct
`@binding` numbers the class and its base declare - one entry per number, ascending; fields
sharing a binding share one entry, a number nothing declares gets none - is a defect.** The
setter checks the argument count against the class's layout, never which field each position
carries; such a list restates no binding number, so it is not a hand-binding arm.

**A hand-binding arm outside `dasllama/`, or one whose pipeline source or threadgroup-memory
size arrives as a function parameter rather than a literal global, states in the PR that its
binding order and push-constant layout were verified by hand against the class declaration.**
The `REVIEW.das` gate `check_race_bind_numbers` cannot read those arms, and an arm nothing
checked is where a mis-numbered bind reaches the board.

**A diff that changes a kernel's binding numbers, kargs (kernel-argument struct) layout,
threadgroup memory, staging shape (the operand tile a kernel copies into threadgroup memory
before it computes), or grid or threadgroup geometry resyncs or deletes, in the same change,
every arm that mirrors that kernel's binding order by hand or by an ordered setter list and
every arm ledgered as a retained reference in `ARCHITECTURE_GPU.md` sec.2.2b (Metal) or
`ARCHITECTURE_MEASUREMENT.md` sec.2.5 (Vulkan).** An arm left dispatching stale geometry
measures the wrong kernel silently.

**Race and knockout code inside the engine (`dasllama/`) sits in the file that owns the kernel
family it races, or - for a knockout - the file that owns the stage whose cost it removes.**

**Scaffolding that race sites in two DIFFERENT engine files share sits in
`dasllama/dasllama_<gpu>_common.das`.** Scaffolding two races in one file share stays in that
file.

**Race code sizes its operands past the device's last-level cache - never a slab small enough to
sit in it.** A cache-resident slab ranks the kernels by an effect production never sees, and the
race then picks the slower kernel.

**A timing arm with a decided ranking times its kernel at every value the engine dispatches that
kernel at, on every input the ranking branches on - a batch width, a row count, a lane split, a
tile's own width.** A ranking timed at one value alone is applied at values it was never ranked at.

**A timing arm for a prefill tile over a variable region, with a decided ranking, times its
kernel at one region whose token count is a whole multiple of that tile's token column - the
token extent one tile covers - and at one where it is not.** A token count that is not a whole
multiple is what makes the tile take its partial-tile store path.

**An `ARCHITECTURE_GPU.md` sec.2.2b entry for a kernel ranked on a power-of-two batch grid names
that grid.**

**A kernel A/B race arm with a decided ranking binds a different output buffer for consecutive
dispatches of its chain, never one shared output.** One shared output serializes the chain on
its write-after-write hazard while the served graph overlaps consecutive dispatches, so the race
ranks the arms on a shape production never runs.

**Every arm of a kernel A/B race with a decided ranking handles the hazard between its
dispatches the same way.** An arm serialized by a barrier races an arm that overlaps.

**Weakening the burn phase of `race_pair_ms` (`dasllama/dasllama_metal_common.das`) - the GPU
work it runs on both arms before the first timed round - is a defect.**

**A race with a decided ranking that does not go through `race_pair_ms` burns GPU work on both
arms before the first timed round.** The arms alternate, so a first round on a cold GPU clock
charges the clock's ramp-up to whichever arm ran first.

**A timed encoder in a race arm with a decided ranking issues its dispatches back to back.** An
encoder that leaves gaps between its dispatches times an idle clock.

**A diff that ports an A/B lab's winning variant into a kernel deletes, in the same change, that
variant's class and any `*_variants.das` code that exists only for it.**

**An arm that survives a port of an A/B lab's winning variant into a kernel dispatches the
shipped kernel class's generated source, or is ledgered as a retained reference in the
architecture doc that owns the kernel's
tier: `ARCHITECTURE_GPU.md` sec.2.2b for Metal, `ARCHITECTURE_MEASUREMENT.md` sec.2.5 (the
Vulkan GEMM probe's axes).** A decided arm that outlives its decision degrades into an
unmaintained duplicate of the kernel it seeded.

**A diff deletes an A/B lab's driver and its remaining arm, in the same change, once the lab
holds no undecided arm, no arm that dispatches the shipped kernel class's generated source and no
arm ledgered as a retained reference in the architecture doc that owns the kernel's tier.**
