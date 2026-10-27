# dasLLAMA GPU Parity Evidence Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`, `ARCHITECTURE_GPU_TOWER.md`.
Planned work: `followup_metal.md`, `followup_vulkan.md`, `followup_general.md`.

- A GPU-served run is a run in which the GPU computed every output row the compare reads.
- Parity evidence is a compare with a GPU-served run on one side and, on the other, a side that is
  not GPU-served, where the CPU chain (the same model's forward on the CPU kernels) or an in-test
  scalar reference (the test's own CPU code for the same math) computed at least one row - one
  token's, frame's or stage's floats or ids - the compare reads, whatever the diff calls it; a
  kernel-unit cell's compare - a test cell that dispatches a `[vk_dispatch]`, `[metal_dispatch]` or
  `[cuda_dispatch]` class itself - counts.
- Driver-against-itself evidence is a compare of the outputs of two GPU-served runs of one model,
  whichever drivers served them.
- An external-reference compare holds a run's output to values another implementation recorded
  outside this engine (its ids or logits), whichever side computed the run.
- A diff cites parity, driver-against-itself or external-reference evidence when it changes what
  either side of a compare offered as that evidence computes, its bar or its assert, or adds or
  changes a citation of such a compare in a doc, a ledger, a commit message or the PR description.
- A serving call is a decode, a prefill, or an encoder or synthesis stage a GPU driver fills; a
  tower driver is the driver filling a family's encoder or synthesis stages
  (`dasllama/dasllama_metal_tower.das`, `dasllama/dasllama_vulkan_tower.das`,
  `dasllama/dasllama_vulkan_tts.das`).
- A bar is the largest difference a continuous compare accepts - a constant, or a constant times
  a statistic of the reference; for a tally of rows whose ids match, the most mismatched rows it
  accepts.
- A serving-tier line is `resident driver armed` for the whole-model driver (every layer of a step
  on the device); for the per-op tier (single operations on the device, the rest on the CPU) a
  `GPU MoE tier:` line reporting a rail (one operation family the tier serves) `resident` - the rail
  and its layers, with any text after them - never one saying the rail, or any layer of it, stays
  on the CPU: a line reading `declined`, `stopped at layer`, `leaves` or `on the CPU`.

**A diff that cites as parity evidence a compare whose GPU side is a `[vk_dispatch]`,
`[metal_dispatch]` or `[cuda_dispatch]` class the cell dispatches itself applies the `tests/`
subfolder's `REVIEW_KERNEL_CELLS.md` too.**

**A diff that cites as parity evidence a compare whose GPU-served run and its other side (the CPU
chain or the in-test scalar reference) did not take identical fixed inputs - the same tokens,
canvas, clip, or the same upstream rows for a stage - is a defect.**

**A diff that cites as parity evidence a within-bar compare whose GPU side is a serving call
shows a control from the same cell - or, for a `harness/parity.das` or `benchmarks/lcpp_bench.das
--parity` reading, from the same command over the same model file - an input the computation reads
changed on one side (a token, a frame, an upstream row, the input scaled by a small factor, a
weight region zeroed, a mechanism disabled) - landing outside the bar.** A value moved in the
output after the fact is not a control.

**A diff that cites as parity evidence a reading from any run but `harness/parity.das`,
`benchmarks/lcpp_bench.das --parity` (`performance/model_specs.das`'s fixed model list) or a cell
run through `tests/run.das` is a defect.**

**A diff that cites as parity evidence a `tests/run.das` cell that compares a continuous output
other than within a bar, or a discrete output other than exactly, is a defect.** A continuous
output is floats, float rows in an encoding that rounds them, or a tally of rows whose ids match;
a discrete output is ids, tokens or any other count, each held equal.

**A diff that adds or changes, in a `PERF_LEDGER.md`, `followup_metal.md`, `followup_vulkan.md` or
`followup_general.md` entry, a reading offered as parity evidence names in that entry the run
it came from - `harness/parity.das`, `benchmarks/lcpp_bench.das --parity` or a `tests/run.das`
run - and, for a `tests/run.das` run, the cell that read it; an entry crediting a reading to a
cell that does not produce it is a defect.**

**A diff that cites Metal parity evidence, or Metal driver-against-itself evidence, whose GPU side
is an LLM decode, prefill or draft step (a drafter's next-token step) is a defect unless a cell
assert or a logged before/after reading, across every GPU call whose reading it cites, shows one of
these counters rising - `metal_decode_stats`' `decodes`, `metal_batch_decode_stats`' `steps`,
`metal_prefill_stats`' `prefills`, or a `metal_kernel_coverage_of` count of a kernel only the
device-served step dispatches - or, for a draft step only, a cell assert that the drafter's GPU step
call (`gemma_draft_step_gpu` for gemma) returned true.** Without one of these the run may have
measured the CPU.

**A diff that cites parity or driver-against-itself evidence of a tower driver or an ASR-decoder
driver (`dasllama/dasllama_metal_asr_dec.das`, `dasllama/dasllama_vulkan_asr_dec.das`) with no cell
assert or logged before/after reading of a counter rising that the driver bumps only when it served
the stage the change touches is a defect.**

**A diff that cites as parity evidence a Vulkan run of a serving call armed by anything but
`DASLLAMA_GPU=1` is a defect; `--ngl` arms Metal alone.**

**A diff that cites as parity evidence a CUDA run of a serving call armed by anything but
`DASLLAMA_GPU=1` with `DASLLAMA_CUDA=1` is a defect.**

**A diff that cites CUDA parity evidence, or CUDA driver-against-itself evidence, whose GPU side is
a decode or prefill step, with no cell assert or logged before/after reading, across every GPU call
whose reading it cites, of a `cud_rdec_stats` counter rising - `tokens` for a decode step,
`windows` for a prefill - or of a `cud_kernel_coverage_of` count of a kernel only the
device-served step dispatches, is a defect.** Without one of these the run may have measured the
CPU.

**A diff that cites as parity or driver-against-itself evidence a compare either of whose Vulkan or
CUDA decode or prefill runs of the whole-model driver or the per-op tier logs no serving-tier line
for the changed path is a defect.**

**A diff that changes a K/V mirror codec's code - its store, its reader, or tq4's pass that undoes
the rotation of the attention row; a codec is the storage form (f16, f32, q8_0 or tq4) of the K/V
mirror, the device copy of the CPU's K/V cache - and cites as parity or driver-against-itself
evidence Vulkan or CUDA runs none of which armed that codec, is a defect.**
`set_gpu_kv_dtype`, the server's `--kv-dtype` and `DASLLAMA_GPU_KV` request f16, q8_0 or tq4, and
an f32 request through them arms f16; `DASLLAMA_VK_KV32=1` arms f32 over all three; f16 is the
default.

**A diff that cites as parity evidence, or as driver-against-itself evidence, a Vulkan or CUDA run
whose log carries a `resident override passed a call` line for a call the changed path serves on
the device is a defect.** That line is the whole-model driver naming a call it handed back to the
CPU path.

**A diff that changes the text of `resident driver armed`, of a `GPU MoE tier:` line, or of
`resident override passed a call` updates, in the same change, the serving-tier line definition
and every rule of this checklist that reads that text.**

**A diff that cites a driver-against-itself compare or an external-reference compare as parity
evidence is a defect - the diff calls it a driver-against-itself compare or an external-reference
compare in the cell's or harness's contract line, in every ledger row it adds or changes that
cites it, and in the PR description.** Each kind is evidence for a `PERF_LEDGER.md` row under its
own name.

**A diff that holds a model file's serving-call output, in a compare offered as parity,
driver-against-itself or external-reference evidence, to a bar no earlier such compare held that
file to, or changes the constant - or the statistic the constant multiplies - of such a bar, or
changes which rows, steps or positions the compare holds to such a bar, adds a `PERF_LEDGER.md`
row in the same change naming the bar (its named constant, where it has one, and its value), the
rows, steps or positions it holds, the reading it comes from and the box that read it.**

**A diff that renames or moves a bar's named constant updates, in the same change, every
`PERF_LEDGER.md` row that names that constant.**
