# dasLLAMA GPU Parity Evidence Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`. Planned work:
`followup_metal.md` for Metal, `followup_vulkan.md` for Vulkan.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

**Parity evidence - a run comparing what the GPU path computes against what the CPU path
computes on the same input - counts only when it comes from `harness/parity.das`,
`benchmarks/lcpp_bench.das --parity` (`performance/model_specs.das`'s fixed model list), or an
in-suite parity instrument run through `tests/run.das`, in the compare form the `tests/`
folder's checklist binds.** A probe that prints a difference and asserts nothing is not parity
evidence.

**A `PERF_LEDGER.md`, `followup_metal.md` or `followup_vulkan.md` entry that settles a parity
fix names the run its evidence came from, and that run is one the rule above admits.**

**Parity evidence counts only when its backend was armed - the run's log, or the cell's report
line, shows the device path served the changed call.** A Metal run is armed by `--ngl` on the
command line or by `select_decode_override("metal")` / `select_prefill_override("metal")` in
process, and shows it by the driver's step counters (`metal_decode_stats`,
`metal_batch_decode_stats`, `metal_prefill_stats`) rising across the run; a Vulkan run is armed
by `DASLLAMA_GPU=1` - never `--ngl` - and shows it by a log line naming the tier that serves the
changed path (`resident driver armed` for the whole-model driver; a `GPU MoE tier:` line ending
in `resident` for the per-op tier). A Vulkan log with no such line measured the CPU.

**Vulkan parity evidence counts only when the run armed the mirror codec - the K/V mirror's
element type, f16 or f32 - that the changed path reads.** `DASLLAMA_VK_KV32=1` arms f32; f16
is the default and needs no flag.

**Vulkan parity evidence whose log carries a `resident override passed a call` line for the
changed path does not count.** That line is the Vulkan driver naming a call it handed back to
the CPU path.
