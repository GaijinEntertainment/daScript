# dasLLAMA GPU Race Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_RACE_SHAPES.md`,
`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md`, `ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md`. Planned
work: `followup_metal.md` for Metal, `followup_vulkan.md` for Vulkan.

A race times two candidates for one computation in one process, either of which the run could
adopt; a knockout skips a stage to measure that stage's cost; an overhead measurement times one
chain with and without an interposed stage - a timestamp, a barrier, a flush - to measure that
stage, and is not a race. A timing arm - arm below - is one timed run of a race, a knockout, or
an overhead measurement: code that dispatches a kernel to measure it rather than to serve a call.
An arm's chain is the dispatches it times. An arm's ranking is decided when a checked-in
document, box profile or sidecar records the arm's figure or names the arm as the shipped form. A
retained-reference arm is one ledgered as a retained reference in
`ARCHITECTURE_GPU_RACE_SHAPES.md` sec.2.2b (Metal) or `ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md`
sec.2.5a (Vulkan).

**A hand-binding arm that binds a field at a position the class does not declare for that field
is a defect.** A hand-binding arm restates a SHIPPED class's binding numbers instead of naming its
fields - a Metal `kn_buffer(enc, n)` call, or a probe class declaring the slots of a class it
mirrors. A mis-bound arm dispatches, reads the wrong buffer, and its timing selects the wrong
kernel silently.

**An ordered argument list into a generated setter (the `set_*(bufs, sizes, gbits)` function a
`[vk_dispatch]` class's family generates) carries one entry per distinct `@binding` number the
class and its base declare, in ascending order - fields sharing a binding share one entry, a
number nothing declares gets none.** The setter checks only how many arguments it got, never
which field each position carries; such a list restates no binding number, so it is not a
hand-binding arm.

**A hand-binding arm whose own function holds neither a `pipeline_from_source` over a literal
global nor a literal `kn_tgmem` constant - one outside `dasllama/`, one whose source arrives as
a parameter, one whose pipeline comes back from a shared shell - states in the PR that its
binding order and push-constant layout were verified by hand against the class declaration.**
The `REVIEW.das` gate `check_race_bind_numbers` cannot read those arms, so nothing but the PR
statement catches a mis-numbered bind before it decides a ranking.

**A diff that lifts race scaffolding into a shared shell keeps each race's `kn_kargs` beside the
`kn_dispatch` it binds, in one function.** The manual-dispatch census fails a function whose
kargs count differs from its dispatch count.

**A diff that changes a kernel's binding numbers, its kernel-argument struct or push-constant
layout, its threadgroup or workgroup memory, its staging shape (the operand tile a kernel copies
into that memory before it computes), or its grid, threadgroup or workgroup geometry resyncs or
deletes, in the same change, every arm that mirrors that kernel's binding order by hand or by an
ordered setter list and every retained-reference arm of that kernel.** An arm left dispatching stale
geometry measures the wrong kernel silently.

**Race and knockout code inside the engine (`dasllama/`) sits in the file that owns the kernel
family it races, or - for a knockout - the file that owns the stage whose cost it removes.**

**Race scaffolding that a race site in another engine file repeats moves to
`dasllama/dasllama_<gpu>_common.das` in the same change.** Scaffolding two races in one file share stays in that
file.

**A race whose ranking turns on weight-stream bandwidth sizes its operands past the device's
last-level cache or streams a ring of copies (`run_cold_shape`, `harness/vk_gemm_probe.das`).** A
cache-resident slab ranks the kernels by an effect production never sees, and the race then picks
the slower kernel.

**A race whose ranking turns on dispatch latency times its kernel at the shape production
dispatches.**

**A timing arm with a decided ranking times its kernel on every input the ranking branches on -
a batch width, a row count, a lane split, a tile's own width - at a value on each side of the
branch.** A ranking timed at one value alone is applied at values it was never ranked at.

**A timing arm for a prefill tile over a variable region, with a decided ranking, times its
kernel at one region whose token count is a whole multiple of that tile's token column - the
token extent one tile covers - and at one where it is not.** A token count that is not a whole
multiple is what makes the tile take its partial-tile store path.

**An `ARCHITECTURE_GPU_RACE_SHAPES.md` sec.2.2b or `ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md`
sec.2.5a entry for a kernel ranked only at power-of-two batch widths names those widths.**

**When production runs a kernel's dispatches independently of each other, each consecutive
dispatch in a race arm with a decided ranking binds its own output buffer.** One shared output
serializes the chain on its write-after-write hazard while production overlaps consecutive
dispatches, so the race ranks the arms on a shape production never runs.

**A race arm that holds one output across its chain - reading a dependent chain's cost, or one
dispatch's latency - is ledgered as that form in `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md`
sec.2.21 (Metal) or `ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md` sec.2.5a (Vulkan).**

**Every arm of a kernel A/B race with a decided ranking handles the hazard between its
dispatches the same way.** A barrier on one arm and overlap on the other price two different
chains, so the ranking is not a comparison.

**Weakening the burn phase of `race_pair_ms` (`dasllama/dasllama_metal_common.das`) - the GPU
work it runs on both arms before the first timed round - is a defect.**

**In a race with a decided ranking that does not go through `race_pair_ms`, every arm's first
timed round follows untimed GPU work on that arm.** A first round on a cold GPU clock charges the
clock's ramp-up to the arm that ran it.

**A timed encoder in a race arm with a decided ranking issues its dispatches back to back.** An
encoder that leaves gaps between its dispatches times an idle clock.

**A diff that ports an A/B lab's winning variant into a kernel - an A/B lab is a timing script
that picks between spellings of one compute - deletes, in the same change, that variant's class
and any `*_variants.das` code that exists only for it and is not a live arm.** A live arm is one
that is undecided, dispatches the shipped kernel class's generated source, is a
retained-reference arm, or is a kernel variant the shipped code picks between at run time. An arm
kept after its ranking is decided stops being maintained and duplicates the kernel it was ported
into.

**The diff that leaves an A/B lab with no live arm deletes the lab's driver and its remaining
arms in the same change.**
