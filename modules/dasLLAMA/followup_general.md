# General follow-ups — CPU-side consistency items found during the reorg

> Sibling of `followup_vulkan.md`: things spotted while moving code that are NOT reorg work
> themselves. Each item names the code, the inconsistency, and what done looks like. Perf-only
> ideas without a consistency angle keep going to `PERF_LEDGER.md`; items here are "two ways of
> doing the same thing coexist — converge them".

1. **Some RoPE application paths still compute `sin`/`cos` directly instead of using tables.**
   `dasllama_math.das` has both forms: the direct `rope_scaled` / `rope_scaled_neox` /
   `rope_scaled_neox_part` (§2789–2924) pay `cos(angle)`/`sin(angle)` per pair per call, while
   the bit-identical table twins (`rope_scaled_tab`, `rope_scaled_neox_tab`,
   `rope_scaled_neox_tab_part`) read a prebuilt cos/sin row (angle generation now single-sourced
   in `dasllama_rope`). Done = audit every caller of the direct forms; each either (a) migrates
   to the `_tab` twin with a row from the decode/prefill rope tables it already has, or (b) gets
   a written reason it cannot hold a table (one-shot positions, no session). Direct forms that
   lose their last caller get deleted.

2. **Some activation loops pay libm `exp` per element where an `exp4` unroll exists next door.**
   `dasllama_math.das` carries `exp4` (vectorized expf, ~2 ulp, ~7x scalar — §2375) and uses it
   in softmax and the fast glu family (`swiglu4`, `geglu4`, `swiglu_oai4`, `silu4`-style bodies
   §2566–2654), but the scalar twins still loop libm `exp` per element: `silu` (§2453–2459), the
   scalar swiglu/swiglu_oai bodies (§2514, §2551), and the sigmoid gate multiply (§2684). Done =
   every per-element `exp` loop either gets the exp4 4-lane body + scalar tail (same shape as
   §2566), or carries a comment naming why it must stay scalar (bit-exactness contract with a
   parity oracle — the "NOT bit-exact" caveat on the fast forms is the known constraint; decide
   per call site which side of the contract it sits on).
