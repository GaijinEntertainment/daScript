# dasLLAMA GPU Parity Evidence Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`. Planned work:
`followup_metal.md` for Metal, `followup_vulkan.md` for Vulkan.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

**Parity evidence compares a GPU-served run against the CPU chain over the same fixed tokens,
and counts only when it comes from `harness/parity.das`, `benchmarks/lcpp_bench.das --parity`
(`performance/model_specs.das`'s fixed model list), or an in-suite parity instrument run through
`tests/run.das` that feeds both sides the same fixed tokens and compares the logits against a
tolerance a one-token-off control reads past - or, for a path that lands a token id and no logits
row (the device argmax pick), compares the served ids token for token against the host's
`parallel_argmax` over the same logits.**

**Parity evidence counts only when its backend was armed: the Metal arm ran with `--ngl`; the
Vulkan arm ran with `DASLLAMA_GPU=1` - never `--ngl` - and its log shows the tier that serves
the changed path armed (`resident driver armed` for the whole-model driver, `GPU MoE tier: ...
resident` for the per-op tier).** A log showing neither arming line measured the CPU.

**Vulkan parity evidence counts only when the run armed the mirror codec - the K/V mirror's
element type, f16 or f32 - that the changed path reads.** `DASLLAMA_VK_KV32=1` arms f32; f16
is the default and needs no flag.

**Vulkan parity evidence whose log carries a `resident override passed a call` line for the
changed path does not count.** That line is the Vulkan driver naming a call it handed back to
the CPU path.

**Driver-against-itself evidence - two GPU-served arms of one model compared against each other:
a batched row against the same session stepped alone, a device-home stream against a host-cached
one, the regions files' shapes - is rows evidence, never parity, and the arming, codec
and pass-through rules above bind it the same.**

**A diff that widens a bar an instrument holds names, in the same change, the reading the new
bar comes from and the box that read it.**
