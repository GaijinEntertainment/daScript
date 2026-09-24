# dasLLAMA GPU Parity Evidence Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`, `ARCHITECTURE_GPU_TOWER.md`.
Planned work: `followup_metal.md` for Metal, `followup_vulkan.md` for Vulkan.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

**Parity evidence counts only when a GPU-served run and the CPU chain take identical fixed inputs -
the same tokens, or the same canvas or clip for an encoder - and are compared against a bar that a
deliberately wrong control exceeds in the same run; an exact-match compare needs no control.** A
control is the input one token off, or one output element overwritten with a wrong value.

**Parity evidence counts only when it comes from `harness/parity.das`, `benchmarks/lcpp_bench.das
--parity` (`performance/model_specs.das`'s fixed model list), or a cell run through
`tests/run.das`.**

**A `tests/run.das` parity cell compares in one of these forms, and a cell comparing any other way
is not parity evidence: the logits within a tolerance; a counting prompt's tokens exactly; where the
changed path produces a token id and no logits row, the served ids token for token against the
host's `parallel_argmax` over the same logits; for a tower, the encoder's output rows against the
CPU chain's rows over one fixed canvas or clip, or its served transcript exactly equal to the CPU
chain's over the same clip.**

**A `PERF_LEDGER.md`, `followup_metal.md` or `followup_vulkan.md` entry that settles a parity
fix names the run its evidence came from, and that run is one this checklist admits as parity
evidence.**

**Parity or driver-against-itself evidence counts only when it shows the device path served the
changed call: a log line naming the serving tier, or a cell assert that the serving driver's own
counter rose across that call.**

**Metal parity or driver-against-itself evidence counts only when the Metal driver's counters rose
across the run - the step counters `metal_decode_stats`, `metal_batch_decode_stats` and
`metal_prefill_stats`, or `metal_tower_stats`'s encodes for the tower driver.** Metal arms by
`--ngl` on the command line, by `select_decode_override("metal")` /
`select_prefill_override("metal")` in process, or - for the tower driver - unless
`set_metal_tower(false)` / `DASLLAMA_METAL_TOWER=0` turns it off.

**A Vulkan run is armed by `DASLLAMA_GPU=1` - never `--ngl` - and shows it by a log line naming
the tier that serves the changed path: `resident driver armed` for the whole-model driver; for
the per-op tier a `GPU MoE tier:` line reporting a rail `resident` (the rail and its layers, a
timing or streamed-span tail allowed), never one reading `declined`, `stopped at layer` or
`stays on the CPU` - or, for the tower driver, a cell assert that `vulkan_tower_stats`'s encodes
rose.** A Vulkan log with neither measured the CPU.

**Vulkan parity or driver-against-itself evidence counts only when the run armed the mirror
codec - the K/V mirror's element type, f16 or f32 - that the changed path reads.**
`DASLLAMA_VK_KV32=1` arms f32; f16 is the default and needs no flag.

**Vulkan parity or driver-against-itself evidence whose log carries a `resident override passed
a call` line for a call the changed path serves on the device does not count.** That line is the
Vulkan driver naming a call it handed back to the CPU path.

**Driver-against-itself evidence - two GPU-served arms of one model compared against each other -
is evidence for a `PERF_LEDGER.md` row, never parity evidence.**

**A diff that sets or widens a bar held by `harness/parity.das`, `benchmarks/lcpp_bench.das
--parity` or a `tests/run.das` cell names, in a `PERF_LEDGER.md` row in the same change, the
reading the bar comes from and the box that read it.**
