# dasLLAMA GPU Parity Evidence Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`, `ARCHITECTURE_GPU_TOWER.md`.
Planned work: `followup_metal.md` for Metal, `followup_vulkan.md` for Vulkan.

**Parity evidence counts only when a GPU-served run and the CPU chain take identical fixed inputs -
the same tokens, canvas, clip, or the same upstream rows for a stage - and are compared against a
bar that a deliberately wrong control exceeds in the same run; an exact-match compare needs no
control.** A control is a perturbation no larger than the error the bar must catch: one input token
changed, or one output element moved by a small multiple of the bar times the output's rms.

**Parity evidence counts only when it comes from `harness/parity.das`, `benchmarks/lcpp_bench.das
--parity` (`performance/model_specs.das`'s fixed model list), or a cell run through
`tests/run.das`.**

**A `tests/run.das` parity cell compares a continuous output (logits, a stage's rows, a waveform)
against the CPU chain's output for the same stage over identical fixed inputs within a bar, and a
discrete output (a counting prompt's tokens, served ids against the host's `parallel_argmax` over
the same logits, a transcript, a frame count) exactly; a cell comparing any other way is not
parity evidence.**

**A `PERF_LEDGER.md`, `followup_metal.md` or `followup_vulkan.md` entry that settles a parity
fix names the run its evidence came from, and that run is one this checklist admits as parity
evidence.**

**Metal parity or driver-against-itself evidence counts only when a cell assert or a logged
before/after reading shows the Metal driver's own counters rose across each changed call: the
step counters `metal_decode_stats`, `metal_batch_decode_stats` and `metal_prefill_stats`, or
`metal_tower_stats`'s encodes for the tower driver.**

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

**A diff that sets or widens a parity bar - one a GPU-served stage is held to against the CPU
chain by `harness/parity.das`, `benchmarks/lcpp_bench.das --parity` or a `tests/run.das` cell -
names, in a `PERF_LEDGER.md` row in the same change, the reading the bar comes from and the box
that read it.**
