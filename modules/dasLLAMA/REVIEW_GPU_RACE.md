# dasLLAMA GPU Timing-Race Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_RACE_SHAPES.md`, `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md`,
`ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md`, `ARCHITECTURE_MEASUREMENT.md`. Planned work:
`followup_metal.md` for Metal, `followup_vulkan.md` for Vulkan.

This list reviews timing arms - code that dispatches a kernel to measure it rather than to serve
a call: a race (two candidates for one computation, timed in one process), a knockout (a timing
that skips a stage to measure its cost; the branch that reads a stage-drop knob - a knob whose
runs do not serve correct output - is arm code, whoever sets it), or an overhead measurement (one
chain timed with and without an interposed stage). A same-build knob A/B - two runs under an
environment override, both serving correct output - is not an arm; its knob answers to
`REVIEW_GPU_VULKAN.md` or `REVIEW_TOWER.md`. A data race between serving dispatches is not this
list's subject.

**A hand-binding arm that binds a field at a position the class does not declare for that field
is a defect.** A hand-binding arm is a timing arm (code that dispatches a kernel to time it, not
to serve a call) that restates a SHIPPED class's binding numbers instead of naming its fields - a
Metal `kn_buffer(enc, n)` call, or a probe class declaring the slots of a class it mirrors. A
mis-bound arm dispatches, reads the wrong buffer, and its timing selects the wrong kernel
silently.

**A hand-binding arm whose own function holds neither a `pipeline_from_source` over a literal
global nor a literal `kn_tgmem` constant - one outside `dasllama/`, one whose source arrives as
a parameter, one whose pipeline comes back from a shared shell - states in the PR that its
binding order and push-constant layout were verified by hand against the class declaration.**
The `REVIEW.das` gate `check_race_bind_numbers` cannot read those arms, so nothing but the PR
statement catches a mis-numbered bind before the arm's figure ranks the kernels.

**Weakening the manual-dispatch census in `dasllama/dasllama_metal_lens.das` is a defect.**

**A diff that adds race or knockout code inside the engine (`dasllama/`) puts it in the file that
owns the kernel family it races, or for a knockout, the file that owns the stage whose cost it
removes.**

**Race or knockout scaffolding (the setup a timing of two candidates, or of a skipped stage,
repeats per arm) that an arm site in another engine file repeats moves to
`dasllama/dasllama_<gpu>_common.das` in the same change.** Scaffolding two arms in one file share
stays in that file.

**A diff that adds or changes a race whose ranking turns on the bandwidth of an operand the
kernel streams once a dispatch - weights, a K/V mirror - sizes that operand past the device's
last-level cache or streams a ring of copies (`run_cold_shape`, `harness/vk_gemm_probe.das`).**
A cache-resident slab ranks the kernels by an effect production never sees, and the race then
picks the slower kernel.

**A diff that adds or changes a race whose ranking turns on dispatch latency times its kernel at
the shape production dispatches.**

**A diff that decides a timing arm's ranking - it records the arm's figure, or names the arm as
the shipped form, in a checked-in document, box profile or sidecar - times the arm's kernel at a
value on each side of every ranking-input branch (a ranking input is a shape dimension or layer
kind the ranking's selection reads), and on a layer of each kind the ranking covers - or names, in
the document that records the figure, the production run whose served rows measure the untimed
value.** A ranking timed at one value alone is applied at values it was never ranked at.

**A diff that widens the gate admitting ranking inputs (the shape dimensions and layer kinds a
ranking's selection reads) to a kernel whose ranking is decided - a checked-in document, box
profile or sidecar records its figure or names it as the shipped form - times the ranked arms at
a newly admitted input, at each end when the new admission spans a range, in the same change.**
The widened gate otherwise applies the ranking at values it was never ranked at.

**A diff that adds or changes a timing arm for a prefill tile over a variable region, with a
decided ranking (a checked-in document, box profile or sidecar records its figure or names it as
the shipped form), times its kernel at one region whose token count is a whole multiple of that
tile's token column - the token extent one tile covers - and at one where it is not.** A token
count that is not a whole multiple is what makes the tile take its partial-tile store path.

**A diff that adds or changes an
`ARCHITECTURE_GPU_RACE_SHAPES.md#tensor-gemm-shapes-that-measured-out-m5` or Vulkan arm ledger
(`ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md#vk-gemm-probe`,
`ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig`) entry for a kernel ranked only at power-of-two
batch widths names those widths in it.**

**A race arm whose ranking is decided (a checked-in document, box profile or sidecar records its
figure or names it as the shipped form) binds each dispatch it times its own output buffer, or is
ledgered in `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity` (Metal) or the Vulkan
arm ledger (`ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md#vk-gemm-probe`,
`ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig`) as holding one output across its chain, naming
the chain it reads.** One shared output serializes the chain on its write-after-write hazard, so
where production overlaps those dispatches the race ranks a shape production never runs.

**A diff that adds or changes an arm of a kernel A/B race with a decided ranking (a checked-in
document, box profile or sidecar records its figure or names it as the shipped form) handles the
hazard between its dispatches the same way every other arm does.** A barrier on one arm and
overlap on the other price two different chains, so the ranking is not a comparison.

**Weakening the burn phase of `race_pair_ms` (`dasllama/dasllama_metal_common.das`) - the GPU
work it runs on both arms before the first timed round - is a defect.**

**A diff that adds or changes a race with a decided ranking (a checked-in document, box profile
or sidecar records its figure or names it as the shipped form) that does not go through
`race_pair_ms` runs untimed GPU work on every arm before that arm's first timed round.** A first
round on a cold GPU clock charges the clock's ramp-up to the arm that ran it.

**A diff that adds or changes a timed encoder in a race arm with a decided ranking (a checked-in
document, box profile or sidecar records its figure or names it as the shipped form) issues the
encoder's dispatches back to back.** An encoder that leaves gaps between its dispatches times an
idle clock.

**A diff that ports an A/B lab's winning variant into a kernel - an A/B lab is a timing script
that picks between spellings of one compute - deletes, in the same change, that variant's class
and any `*_variants.das` code that exists only for it and is not a live arm.** A live arm is one
that is undecided, dispatches the shipped kernel class's generated source, is a
retained-reference arm (one ledgered as a retained reference in
`ARCHITECTURE_GPU_RACE_SHAPES.md#tensor-gemm-shapes-that-measured-out-m5` or the Vulkan arm
ledger - `ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md#vk-gemm-probe`,
`ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig`), or is a kernel variant the shipped code picks
between at run time. An arm kept after its
ranking is decided stops being maintained and duplicates the kernel it was ported into.

**The diff that leaves an A/B lab (a timing script that picks between spellings of one compute)
with no live arm deletes the lab's driver and its remaining arms in the same change.**
