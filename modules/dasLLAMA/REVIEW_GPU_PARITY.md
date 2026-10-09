# dasLLAMA GPU Parity Evidence Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`, `ARCHITECTURE_GPU_TOWER.md`.
Planned work: `followup_metal.md`, `followup_vulkan.md`.

Parity evidence is a compare of a GPU-served run - a run whose output the GPU computed - against
the CPU chain (the same model's forward on the CPU kernels), cited to show the GPU path computes
what the CPU does; a kernel-unit cell's compare - a test cell that dispatches a `[vk_dispatch]` or
`[metal_dispatch]` class itself - counts. Driver-against-itself evidence is a compare of two
GPU-served sides of one model against each other. A diff cites parity or driver-against-itself
evidence when it changes what either side of a compare offered as that evidence computes, its bar
or its assert, or adds or changes a citation of such a compare in a doc, a ledger, a commit message
or the PR description. A serving call is a decode, a prefill, or an encoder or synthesis stage a
GPU driver fills; a tower driver is the driver filling a family's encoder or synthesis stages
(`dasllama/dasllama_metal_tower.das`, `dasllama/dasllama_vulkan_tower.das`,
`dasllama/dasllama_vulkan_tts.das`). A bar is the largest difference a continuous compare accepts -
a constant, or a constant times a statistic of the reference.

**A diff that cites as parity evidence a compare whose GPU side is a `[vk_dispatch]` or
`[metal_dispatch]` class the cell dispatches itself applies the `tests/` subfolder's
`REVIEW_KERNEL_CELLS.md` too.**

**A diff that cites as parity evidence a compare whose GPU-served run and CPU chain did not take
identical fixed inputs - the same tokens, canvas, clip, or the same upstream rows for a stage - is
a defect.**

**A diff that cites as parity evidence a within-bar compare whose GPU side is a serving call
shows a control from the same run - an input the computation reads changed on one side (a token, a
frame, an upstream row, the input scaled by a small factor, a weight region zeroed, a mechanism
disabled) - landing outside the bar.** A value moved in the output after the fact is not a control.

**A diff that cites as parity evidence a reading from any run but `harness/parity.das`,
`benchmarks/lcpp_bench.das --parity` (`performance/model_specs.das`'s fixed model list) or a cell
run through `tests/run.das` is a defect.** A scratch probe's reading may sit in a ledger's prose
marked as a probe's, never as evidence.

**A diff that cites as parity evidence a `tests/run.das` cell that compares a continuous output
other than within a bar, or a discrete output other than exactly, is a defect.** A continuous
output is floats, or float rows in an encoding that rounds them; a discrete output is ids, tokens
or counts.

**A diff that adds or changes, in a `PERF_LEDGER.md`, `followup_metal.md` or `followup_vulkan.md`
entry, a reading offered as parity evidence names in that entry the run
it came from - `harness/parity.das`, `benchmarks/lcpp_bench.das --parity` or a `tests/run.das`
run - and, for a `tests/run.das` run, the cell that read it; an entry crediting a reading to a
cell that does not produce it is a defect.**

**A diff that cites Metal parity evidence, or Metal driver-against-itself evidence, whose GPU
side is a decode or prefill step, with no cell assert or logged before/after reading, across every
GPU call whose reading it cites, of one of these counters rising - `metal_decode_stats`' `decodes`,
`metal_batch_decode_stats`' `steps`, `metal_prefill_stats`' `prefills`, or a
`metal_kernel_coverage_of` count of a kernel only the device-served step dispatches - is a
defect.** Without the counter the run may have measured the CPU.

**A diff that cites Metal parity or driver-against-itself evidence of the Metal tower driver or
the ASR-decoder driver (`dasllama/dasllama_metal_asr_dec.das`) with no cell assert or logged
before/after reading of that driver's counter rising is a defect - the `metal_tower_stats` counter
of the stage the change touches (`convs` for a front, the conv stage ahead of the blocks; `encodes`
for any other stage, the blocks and a TTS stage included), `metal_wdec_stats`' `windows`.**

**A diff that cites as parity evidence a Vulkan run of a serving call armed by anything but
`DASLLAMA_GPU=1` is a defect; `--ngl` arms Metal alone.**

**A diff that cites as parity evidence a Vulkan decode or prefill run of the whole-model driver
or the per-op tier whose log names no serving tier for the changed path is a defect.** The line
is `resident driver armed` for the whole-model driver (every layer of a step on the device); for
the per-op tier (single operations on the device, the rest on the CPU) a `GPU MoE tier:` line
reporting a rail (one operation family the tier serves) `resident` - the rail and its layers,
with any text after them - never one saying the rail, or any layer of it, stays on the CPU - a
line reading `declined`, `stopped at layer`, `leaves` or `on the CPU`.

**A diff that cites as parity evidence a Vulkan run of a Vulkan tower driver or the ASR-decoder
driver (`dasllama/dasllama_vulkan_asr_dec.das`) with no cell assert or logged before/after reading
of that driver's counter for the stage the change touches rising - `vulkan_tower_stats`: `encodes`
for the blocks, `convs` for a front (the conv stage ahead of the blocks) or the stem, `mels` for
the mel spectrogram; `vulkan_tts_stats`: `encodes` for a TTS stage; `vulkan_wdec_stats`: `windows`
- is a defect.**

**A diff that cites as parity evidence, or as driver-against-itself evidence, a Vulkan run that
did not arm the mirror codec the changed path reads - the storage form (f16, f32, q8_0 or tq4) of
the K/V mirror, the device copy of the CPU's K/V cache - is a defect.** `set_gpu_kv_dtype`, the
server's `--kv-dtype` and `DASLLAMA_GPU_KV` request f16, q8_0 or tq4, and an f32 request through
them arms f16; `DASLLAMA_VK_KV32=1` arms f32 over all three; f16 is the default.

**A diff that cites as parity evidence, or as driver-against-itself evidence, a Vulkan run whose
log carries a `resident override passed a call` line for a call the changed path serves on the
device is a defect.** That line is the Vulkan driver naming a call it handed back to the CPU path.

**A diff that changes the text of `resident driver armed`, of a `GPU MoE tier:` line, or of
`resident override passed a call` updates, in the same change, the rules of this list that read
that text.**

**A diff that cites a driver-against-itself compare as parity evidence is a defect - such a
compare is evidence for a `PERF_LEDGER.md` row.**

**A diff that, in a cell or run whose compare is offered as parity evidence, other than a
kernel-unit cell, holds a model file's serving-call output to a bar no earlier such compare held
that file to, or changes the constant - or the statistic the constant multiplies - of such a bar,
adds a `PERF_LEDGER.md` row in the same change naming the bar (its named constant, where it has
one, and its value), the reading it comes from and the box that read it.**
