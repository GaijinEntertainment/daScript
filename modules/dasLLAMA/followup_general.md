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

3. **The pipelined batch-decode landing holds borrowed Session pointers across the caller's
   deletes.** `finish_pending_step` (dasllama_metal_common) lands the in-flight batch step by
   memcpy-ing KV rows and logits through `g_lp_sessions[i]` — raw borrowed pointers. Nothing
   quiesces on session death, `metal_decode_flush` (the public landing call) has zero callers,
   and every test helper deletes its sessions right after `eval_batch_` with a step potentially
   pending — a landing after those deletes writes into freed heap. Empirically quiet today
   (probed 2026-07-30: batch-only prelude showed no corruption), but it is a lifetime landmine,
   and the server would inherit it if a chat session ever dies mid-pipeline. Done = a session
   retirement seam the drivers hook (land or abandon pending steps that reference the dying
   session), or the landing rail stops holding raw session pointers; plus `metal_decode_flush`
   calls at the test helpers' teardown as the interim belt.

4. **The GPU-PLE prefill pre-step never engages on the E4B even though the model qualifies.**
   The census (family-scoped E4B row, metal prefill serving on device — `attn_qk_mm` counted)
   shows `metal_ple_gather_q8`, `metal_ple_finish`, and `metal_bf16_mulmm` at zero, yet every
   static leg of `metal_ple_pre_gpu_gate` holds for the E4B gguf: `per_layer_model_proj` is
   native BF16, `per_layer_token_embd` is Q8_0, ple=256 passes the shape gates, and the prefill
   itself did not decline. So the gate returns false on a dynamic leg (consult-time override /
   `prefill_decline` / `metal_prefill_init` ordering) — the PLE pre-step silently runs its CPU
   fallback (`ple_pre_prefill`) on every metal prefill window. Done = instrument the gate legs,
   find the refusing one, fix or document it, and the census E4B row counts all three kernels.

5. **The metal kernel zoo carries four different rope addressing schemes.** Factoring the
   sq_attn family showed the rope-store family does NOT share one skeleton the way attention
   did — its eleven kernels split four ways: pair-per-thread WITH partial rotation
   (`rope_store16/32` + the b twins: `rot`/`halfr` with an identity-cover tail),
   pair-per-thread WITHOUT it (`rope_store_q8` + `b_q8`: whole-head rotation, `half` only),
   element-per-thread (`rope_store_tq4`/`b_tq4`/`h16`, threadgroup-wide, and `h16` restores
   partial rotation), and the fused-GEMV form (`q8_gemv_qkv_rs16/32`, index derived from the
   weight row). Only ~60 lines are mechanically shared, so deduping them buys little AND would
   paper over the question worth answering: are four schemes load-bearing (each tuned to its
   store) or is this drift that one scheme could serve? A wrong answer changes each kernel's
   lane mapping, so it needs a measurement, not a refactor. Done = each scheme either justified
   in a comment at its kernel or collapsed into the one that measures as fast.

6. **The K-quant GEMV zoo is dedupable along its batch-width axis only.** Measured divergence
   over stripped bodies: the FORMAT axis is genuinely different math (`kq_gemv_k4` vs `_k5` =
   49% of lines differ; `kq_mvb2_k4` vs `_k6` = 43%) — those are separate block decodes and
   must stay separate. The WIDTH axis is near-identical (`kq_mvb2_k4` vs `kq_mvb4_k4` = 11%,
   and that 11% IS the unroll factor). So the nine `kq_mvb{2,4,8}_k{4,5,6}` kernels are three
   families of three, each family one kernel with a compile-time width. That needs the width to
   specialize at emission (an unrolled generic, not a value parameter — a runtime width would
   delete the tuning). Done = decide whether msl_emit should specialize a compile-time constant
   parameter, then collapse the nine to three or record why not.

7. **Pointer families — a language-level idea, parked.** (Boris, 2026-07-30: "interesting follow …
   it maybe good - we are just not there yet.") Everything above about address spaces exists
   because `float4?` says what a pointer points AT and nothing about where it lives. MSL needs
   that (`device` vs `threadgroup` are different types there and do not implicitly convert), so
   today a helper cannot take a pointer at all, and the near-term fix is a parameter annotation
   the emitter cross-checks against the argument's provenance. The deeper version is to give
   daslang pointer FAMILIES — the memory family as part of the pointer type, so the checking is
   the type system's job rather than an emitter's. It would pay off beyond shaders (the same
   information a CPU backend wants for aliasing, and what any target with more than one memory
   wants), but it reaches inference, mangling, and every cast, so it is a language design task
   and not a step in this arc. Parked deliberately, not forgotten.

8. **Two MoE mul_mm formats stayed out of the family, for the same reason.** The k4/k5/q51 kernels
   now share `MetalMoeMulMmBase` — one tile loop, two overrides (the k-block decode and the
   expert-plane origin it addresses from). K6 and Q8 could not join: both carry state ACROSS loop
   iterations that a per-iteration override cannot hold — k6 caches the superblock scalars
   (`sv`/`dall`, reloaded every 8th k-block), q8 walks advancing weight pointers (`scur`/`qp`).
   Both become expressible the moment a helper can take a pointer or a thread-space reference it
   may advance, which is the annotation work already ruled in (`@threadgroup p : float4?` and its
   `device` default). Done = k6 and q8 derive from the same base, and the family is five for five.

9. **Prefill compiles its own PSO for kernels decode already has.** `enc_qk_norm_pf` in
   dasllama_metal_prefill.das is `enc_qk_norm`'s body with `g_pf_pso_qknorm` in place of
   `g_pso_qknorm` and a buffer offset on the x bind; `enc_rope` there is the same story. The MSL
   source is one string in both cases, so the duplication is the PIPELINE object, not the shader —
   prefill and decode own separate lifecycles (`metal_prefill_init` / `metal_decode_init`) and
   each builds its own. The fix is not to merge the encoders but to decide who owns a PSO for a
   kernel both stages dispatch: either a shared pipeline registry keyed by MSL source, or an
   explicit rule that a stage-local PSO is the intended shape. Until that is settled, a lensed
   class whose builder both stages could use still gets a hand-written twin on the prefill side.
   Done = the rule is written in CODEREVIEW.md, and the twins either share a PSO or say why not.
