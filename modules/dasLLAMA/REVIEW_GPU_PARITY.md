# dasLLAMA GPU Parity Evidence Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`, `ARCHITECTURE_GPU_TOWER.md`.
Planned work: `followup_metal.md`, `followup_vulkan.md`.

Parity evidence is a compare of a GPU-served run - a run whose output the GPU computed - against
the CPU chain, cited to show the GPU path computes what the CPU does; a kernel-unit cell's compare
counts. Driver-against-itself evidence is a compare of two GPU-served arms of one model against
each other.

**A diff that cites as parity evidence a compare whose GPU-served run and CPU chain did not take
identical fixed inputs - the same tokens, canvas, clip, or the same upstream rows for a stage - is
a defect.**

**A diff that cites as parity evidence a within-bar compare whose GPU side is a serving call - a
decode, a prefill, a tower or a stage seat - with no control in the same run that changes an
input the computation reads on one side (a token, a frame, an upstream row, the input scaled by a
small factor, a weight region zeroed, a mechanism disabled) and lands outside the bar is a
defect.** A value moved in the output after the fact is not a control.

**A diff that cites as parity evidence a compare whose GPU side is a `[vk_dispatch]` or
`[metal_dispatch]` class the cell dispatches itself applies the `tests/` subfolder's
`REVIEW_KERNEL_CELLS.md` too.**

**A diff that cites as parity evidence a reading from any run but `harness/parity.das`,
`benchmarks/lcpp_bench.das --parity` (`performance/model_specs.das`'s fixed model list) or a cell
run through `tests/run.das` is a defect.** A scratch probe's reading may sit in a ledger's prose
marked as a probe's, never as evidence.

**A diff that cites as parity evidence a `tests/run.das` cell that compares a continuous output
(logits, a stage's rows, a waveform) other than within a bar, or a discrete output (a counting
prompt's tokens, served ids against the host's `parallel_argmax` over the same logits, a
transcript, a frame count) other than exactly, is a defect.**

**A diff that adds or changes a `PERF_LEDGER.md`, `followup_metal.md` or `followup_vulkan.md`
entry offering a GPU-vs-CPU parity reading as evidence names in it the run it came from -
`harness/parity.das`, `benchmarks/lcpp_bench.das --parity` or a `tests/run.das` run - and, for a
`tests/run.das` run, the cell that read it; an entry crediting a reading to a cell that does not
produce it is a defect.**

**A diff that cites Metal parity evidence, or Metal driver-against-itself evidence, with no cell
assert or logged before/after reading of the Metal driver's own step counters rising across each
changed call - `metal_decode_stats`, `metal_batch_decode_stats` and `metal_prefill_stats` - is a
defect.** Without the counter the run may have measured the CPU.

**A diff that cites Metal parity or driver-against-itself evidence of the tower driver (the
driver serving a family's encoder and synthesis stages through hook seats - stage slots the CPU
chain calls and a GPU driver fills) or the ASR-decoder driver with no cell assert or logged
before/after reading of that driver's counter rising is a defect - `metal_tower_stats`' counter
of the stage the change touches (`encodes` or `blocks` for the blocks, `convs` for a front),
`metal_wdec_stats`' windows.**

**A diff that cites as parity evidence a Vulkan run armed by anything but `DASLLAMA_GPU=1`
(`--ngl` included) is a defect.**

**A diff that cites as parity evidence a Vulkan decode or prefill run of the whole-model driver
or the per-op tier whose log names no serving tier for the changed path is a defect.** The line
is `resident driver armed` for the whole-model driver (every layer of a step on the device); for
the per-op tier (single operations on the device, the rest
on the CPU) a `GPU MoE tier:` line reporting a rail (one operation family the tier serves)
`resident` - the rail and its layers, with any text after them - never one reading `declined`,
`stopped at layer` or `stays on the CPU`.

**A diff that cites as parity evidence a Vulkan run of the tower driver or the ASR-decoder driver
whose cell asserts no rise in that driver's counter is a defect - for the tower driver
`vulkan_tower_stats`'s counter of the stage the change touches (`encodes` for the blocks, `convs`
for a front or the stem, `mels` for the mel), for the ASR-decoder driver `vulkan_wdec_stats`'s
windows.**

**A diff that cites as parity evidence, or as driver-against-itself evidence, a Vulkan run that
did not arm the mirror codec the changed path reads - the element type, f16 or f32, of the K/V
mirror (the device copy of the CPU's K/V cache) - is a defect.** `DASLLAMA_VK_KV32=1` arms f32;
f16 is the default and needs no flag.

**A diff that cites as parity evidence, or as driver-against-itself evidence, a Vulkan run whose
log carries a `resident override passed a call` line for a call the changed path serves on the
device is a defect.** That line is the Vulkan driver naming a call it handed back to the CPU path.

**A diff that changes the text of `resident driver armed`, of a `GPU MoE tier:` line, or of
`resident override passed a call` updates, in the same change, the rules of this list that read
that text.**

**A diff that cites as parity evidence a driver-against-itself compare is a defect; such a
compare is evidence for a `PERF_LEDGER.md` row only.**

**A diff that sets or widens the bar a parity run holds a GPU-served stage to - a stage the run
reaches through a hook seat (a stage slot the CPU chain calls and a GPU driver fills) or a
session step, the run one of `harness/parity.das`, `benchmarks/lcpp_bench.das --parity` or a
`tests/run.das` cell - adds a `PERF_LEDGER.md` row in the same change naming the bar's constant
and value, the reading it comes from and the box that read it.**
