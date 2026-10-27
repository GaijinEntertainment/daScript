# dasLLAMA GPU Timing-Race Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_RACE_SHAPES.md`, `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md`,
`ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md`, `ARCHITECTURE_MEASUREMENT.md`. Planned work:
`followup_metal.md`, `followup_vulkan.md`, `followup_general.md` (the CUDA rows).

This list reviews timing arms - code that dispatches a kernel to measure it rather than to serve
a call: a race (two candidates for one computation, timed in one process), a knockout (a timing
that skips a stage to measure its cost; the branch that reads a stage-drop knob - a knob whose
runs do not serve correct output - is arm code, whoever sets it), or an overhead measurement (one
chain timed with and without an interposed stage). A same-build knob A/B - two runs under an
environment override, both serving correct output - is not an arm; its knob answers to
`REVIEW_OVERRIDES.md`, and a tower's knob to `REVIEW_TOWER.md` too. A two-candidate figure is a
recorded timing comparing two candidates
for one computation - from a race, a same-build knob A/B, or one run on each of two builds; the
knob itself still answers to its engine's checklist. A data race between
serving dispatches is not this list's subject. An arm's ranking is decided once a checked-in document or a committed box profile records
the arm's figure or names the arm as the shipped form, and always where the race's winner is a
crown - a verdict each box's tune sidecar records. A ranking input is any value the ranking's
selection reads - a shape dimension, a layer kind, or a device or driver feature such as an
extension, a subgroup size, or the decode path the driver runs. An A/B lab is a timing script that picks between spellings of
one compute. A live arm is one that is undecided, dispatches the shipped kernel class's generated
source, is a knockout whose stage-drop constant or knob sits in a shipped class or a lab, is a
retained-reference arm (one ledgered as a retained reference in
`ARCHITECTURE_GPU_RACE_SHAPES.md#tensor-gemm-shapes-that-measured-out-m5` or the Vulkan arm
ledger - `ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md#vk-gemm-probe`), or is a kernel variant the shipped code picks
between at run time.

**Weakening `check_race_bind_numbers` (`REVIEW.das`) - the gate that reads a `kn_buffer(enc, n)`
arm's slots against the class declaration - is a defect.** A hand-binding arm is a timing arm that
passes a literal binding slot for a SHIPPED class's field instead of naming the field - a Metal
`kn_buffer(enc, n)` or raw `metal_set_buffer` call, or a probe class declaring the slots of a class
it mirrors. A mis-bound arm dispatches, reads the wrong buffer, and its timing selects the wrong
kernel silently.

**A diff that adds a hand-binding arm, or changes which shipped class an existing one binds, states
in the PR that the arm's binding order and push-constant layout were checked by hand against that
class's declaration.** `check_race_bind_numbers` flags only a slot the class never declares, and only
under `dasllama/`; a swapped pair of declared slots dispatches and ranks the kernels on the wrong data.

**A diff that adds race or knockout code inside the engine (`dasllama/`) puts it in the file that
owns the kernel family it races, or for a knockout, the file that owns the stage whose cost it
removes.**

**Race or knockout scaffolding (the setup a timing of two candidates, or of a skipped stage,
repeats per arm) that an arm site in another engine file repeats moves to
`dasllama/dasllama_<gpu>_common.das` in the same change.** Scaffolding two arms in one file share
stays in that file.

**A diff that decides the ranking of a race whose code the tree holds, or adds or changes a
race, whose ranking turns on the bandwidth of an operand the kernel streams once a dispatch -
weights, a K/V mirror - sizes that operand past the device's last-level cache or streams a ring
of copies (`run_cold_shape`, `harness/vk_gemm_probe.das`).** A cache-resident slab ranks the
kernels by an effect production never sees, and the race then picks the slower kernel.

**A diff that decides the ranking of a race whose code the tree holds, or adds or changes a
race, whose ranking turns on dispatch latency times its kernel at the shape production
dispatches.**

**A diff that decides a timing arm's ranking times the arm's kernel at a value on each side of
every ranking-input branch - or names, in the document that records the figure, the production run
whose served rows measure the untimed side.** A ranking timed at one value alone is applied at
values it was never ranked at.

**A diff that decides a timing arm's ranking times the arm's kernel on a layer of each kind the
ranking covers - or names, in the document that records the figure, the production run whose
served rows measure each untimed kind.** A layer kind is one weight matrix a layer multiplies by -
qkv, w13, w2, an expert stack - whose shape differs from the others'.

**A diff that applies a decided ranking to a kernel no arm of that race dispatches - gives it a
setting that race chose - adds an arm dispatching that kernel to the race, or names,
in the document that records the figure, the production run whose served rows measure it, in the
same change.** A ranking holds only for the kernels its arms timed.

**A diff that widens the gate admitting ranking inputs to a kernel whose ranking is decided times
the ranked arms at a newly admitted input in the same change - at both ends of a newly admitted
range, and for a range with no upper bound at its lowest value and at the largest value any model
`performance/model_specs.das` lists runs it at.** The widened gate otherwise applies the ranking at values it was never ranked
at.

**A diff that decides the ranking of a prefill tile's timing arm over a variable region, or adds
or changes such an arm whose ranking is decided, for a tile some dispatch can run at a token count
off a whole multiple of its token column (the token extent one tile covers), makes the deciding arm
time its kernel at one region on such a multiple and at one off it.** Only the count off a multiple
takes the tile's partial-tile store path.

**A diff that adds or changes an
`ARCHITECTURE_GPU_RACE_SHAPES.md#tensor-gemm-shapes-that-measured-out-m5` or Vulkan arm ledger
(`ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md#vk-gemm-probe`) entry for a kernel ranked only at power-of-two
batch widths names those widths in it.**

**A diff that decides the ranking of a race whose code the tree holds, or adds or changes an
arm of a race whose ranking is decided, gives every dispatch an arm times an output buffer the
chain's previous dispatch does not write - two buffers alternated between consecutive dispatches
qualify - or ledgers the arm in
`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity` (Metal) or the Vulkan arm ledger
(`ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md#vk-gemm-probe`) as holding one output across its chain, naming
the chain it reads, in the same change.** One shared output serializes the chain on its
write-after-write hazard, so where production overlaps those dispatches the race ranks a shape
production never runs.

**A diff that decides the ranking of a race whose code the tree holds, or adds or changes an
arm of a race whose ranking is decided, makes every arm order its consecutive dispatches the
same way - a barrier between each pair on every arm, or on none.** A barrier on one arm and
overlap on the other price two different chains, so the ranking is not a comparison.

**Weakening the burn phase of `race_pair_ms` (`dasllama/dasllama_metal_common.das`) - the GPU
work it runs on both arms before the first timed round - is a defect.**

**A diff that decides the ranking of a race whose code the tree holds and that does not go
through `race_pair_ms`, or adds or changes such a race whose ranking is decided, makes it run
untimed GPU work on every arm before that arm's first timed round.** A first round on a cold GPU
clock charges the clock's ramp-up to the arm that ran it.

**A diff that decides the ranking of a race whose code the tree holds, or adds or changes a
timed encoder in an arm of a race whose ranking is decided, makes each timed encoder issue its
dispatches back to back.** An encoder that leaves gaps between its dispatches times an idle clock.

**A diff that records a two-candidate figure lands in the tree, in the same change, the program
that timed it, and writes beside the figure the command line that ran it and the commit of each
build it ran on.** A figure that cannot be re-run cannot be checked.

**A diff that replaces or removes a race whose figure a document carries re-measures that figure
with a race the tree holds, deletes the figure, or moves the old race under `harness/`, in the
same change.** No reviewer can re-check a figure whose race is gone.

**A diff that ports an A/B lab's winning variant into a kernel deletes, in the same change, that
variant's class and any `*_variants.das` code that exists only for it and is not a live arm.** An
arm kept after its ranking is decided stops being maintained and duplicates the kernel it was
ported into.

**The diff that leaves an A/B lab with no live arm deletes the lab's driver and its remaining
arms in the same change.**
