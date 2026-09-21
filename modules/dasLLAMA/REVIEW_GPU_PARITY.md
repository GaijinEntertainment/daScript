# dasLLAMA GPU Parity Evidence Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`. Planned work:
`followup_metal.md` for Metal, `followup_vulkan.md` for Vulkan.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

**Parity evidence - a run comparing what the GPU path computes against what the CPU path
computes on the same input - counts only when it comes from `harness/parity.das`,
`benchmarks/lcpp_bench.das --parity` (`performance/model_specs.das`'s fixed model list), or an
in-suite parity instrument run through `tests/run.das` that feeds both sides the same fixed
tokens and compares the logits, or the greedy tokens, against a fixed bar.** A harness probe
that prints a difference and asserts nothing localizes a defect; the entry that settles the
fix names the sanctioned run.

**Parity evidence counts only when its backend was armed - the run's log or gate output shows
the device path served the changed call.** The Metal arm arms through `--ngl` on the command
line or `select_decode_override("metal")` / `select_prefill_override("metal")` in process; the
Vulkan arm through `DASLLAMA_GPU=1` - never `--ngl` - with the log showing the tier that serves
the changed path armed (`resident driver armed` for the whole-model driver, `GPU MoE tier: ...
resident` for the per-op tier). A log showing neither arming line measured the CPU.

**Vulkan parity evidence counts only when the run armed the mirror codec - the K/V mirror's
element type, f16 or f32 - that the changed path reads.** `DASLLAMA_VK_KV32=1` arms f32; f16
is the default and needs no flag.

**Vulkan parity evidence whose log carries a `resident override passed a call` line for the
changed path does not count.** That line is the Vulkan driver naming a call it handed back to
the CPU path.
