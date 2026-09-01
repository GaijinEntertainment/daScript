# dasLLAMA GPU Race / Lab / Hand-Binding Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_MEASUREMENT.md`.

**Routed from `REVIEW_GPU.md`: a diff touching a GPU kernel A/B race, knockout, or
hand-binding arm - or changing a kernel class such an arm mirrors (binding numbers, kargs
layout, threadgroup memory, staging shape, grid or threadgroup geometry) - wherever the diff
puts it, applies this list together with `REVIEW_GPU.md`.**

**A hand-binding arm - a race or knockout timing arm (a knockout attributes cost across
stages instead of selecting between implementations) that lists its bindings by number
instead of dispatching through the `enc_*` builder - that binds a field at another field's
declared number is a defect.** A mis-numbered arm dispatches, reads the wrong buffer, and its
timing selects the wrong kernel silently.

**A hand-binding arm outside `dasllama/`, or one whose pipeline source or threadgroup-memory
size arrives as a function parameter rather than a literal global, states in the PR that its
bind numbers were verified by hand against the class declaration.** The `REVIEW.das` gate
`check_race_bind_numbers` cannot read those arms, and an arm nothing checked is where a
mis-numbered bind reaches the board.

**A diff that changes anything a hand-binding arm must mirror to dispatch a kernel - binding
numbers, kargs (kernel-argument struct) layout, threadgroup memory, grid or threadgroup
geometry - fixes or deletes every such arm in the same change.** An arm left dispatching
stale geometry measures the wrong kernel silently.

**Race code inside the engine (`dasllama/`) sits in the file that owns its kernel family and
never in another engine file; the shared scaffolding (`race_buf`, `race_envelope_ok`,
`race_pair_ms`) sits in `dasllama/dasllama_<gpu>_common.das`.** Race code is the base-vs-twin
check that times both kernels on one queue and compares their outputs.

**Race code sizes its operands at a real model shape - never at a small square slab.** A slab
small enough to sit in cache ranks the kernels by an effect production never sees, and the
race then picks the slower kernel.

**A kernel A/B race arm whose verdict alone selects the kernel the engine ships - wherever
the race lives - binds a DIFFERENT output buffer for consecutive dispatches of its chain,
never one shared output.** One shared output serializes the chain on its write-after-read
hazard while the served graph overlaps consecutive dispatches, so the race ranks the arms on
a shape production never runs.

**Weakening the burn phase of `race_pair_ms` (`dasllama/dasllama_metal_common.das`) - the GPU
work it runs on both arms before the first timed round - is a defect.**

**A race arm that selects the kernel the engine ships and does not go through `race_pair_ms`
burns GPU work on its own arms before its first timed round.** The arms alternate, so a first
round on a cold GPU clock charges the clock's ramp-up to whichever arm ran first.

**A timed encoder in a race arm that selects the kernel the engine ships issues its
dispatches back to back.** An encoder that leaves gaps between its dispatches times an idle
clock.

**A diff that ports an A/B lab's winning variant into a kernel deletes, in the same change,
every arm of that lab that `ARCHITECTURE_GPU.md` sec.2.2b does not ledger as a retained
reference, together with the variant class and any `*_variants.das` code that exists only for
that arm.** An A/B lab is a timing script whose output selects between implementations of the
same compute; a decided arm that outlives its decision degrades into an unmaintained
duplicate of the kernel it seeded.

**A diff that leaves an A/B lab with no undecided arm deletes the lab's driver and its
remaining arm in the same change.**

**A diff that moves a shipped kernel's staging shape or its grid or threadgroup geometry
resyncs or deletes, in the same change, every arm `ARCHITECTURE_GPU.md` sec.2.2b ledgers as a
retained reference that reimplements that kernel.**
