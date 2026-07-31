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

10. **The two fused QKV-GEMV twins are a D-family-shaped dedup that has not happened.**
    `MetalQ8GemvQkvRsF16` and `MetalQ8GemvQkvRsF32` are ~95 lines of near-identical body that
    differ only in the mirror's destination views — the same shape the split-K attention family
    had before it was split into `sqd_*` stages. Both are over the STYLE038 80-line cap and the
    kargs fold pushed them further (the seven unpacked fields are each used 3-19 times, so
    inlining `ka.field` instead would be noisier, not shorter). Done = the pair shares its GEMV,
    rope and store stages the way the D family shares `sqd_score_blk`/`sqd_vacc_blk`, and both
    bodies come back under the cap.

11. **Every cold mint streams; what is left is the vulkan flavor's SIZE, not its peak.**
    Peak `phys_footprint_peak` of one load, Meta-Llama-3.1-8B-Q8_0 (8 GB gguf → 9.6 GB image),
    M1 Max:

        cold planar (streamed)   5,380 MB   4.5 s
        cold metal  (streamed)   4,848 MB   3.1 s
        cold vulkan (streamed)   6,570 MB   15.5 s   (mints TWO images)
        warm, any flavor           651 MB

    A mapped serve lands at ~0.07x the image, and Metal wraps the mapping with
    `metal_new_buffer_no_copy_untracked` exactly as Vulkan and the CPU do. The metal blob comes out
    *under* planar because a blob forbids CPU repack and so never needs the resident `qscales`
    plane. The vulkan bake rides the GPU walk of the model that is already serving — the warm path
    always did this off a mapping, and the cold path now does too instead of loading eagerly.

    What remains is that a **vulkan cold mint writes ~22 GB to produce an 11.8 GB artifact**: the
    planar image lands first (10.0 GB), then the flavor image re-writes all of that content plus
    the 1.6 GB bake blob. The flavor is a superset of the planar image, so the second write copies
    9.6 GB of bytes that already exist on disk a few hundred milliseconds earlier — 8.3 s of the
    15.5 s total. Done = either the flavor image references the planar one for its shared planes,
    or a vulkan mint skips writing the planar image it is about to supersede. Neither is a peak
    problem (the peak is fine), so this is wall-clock and disk, and it wants a decision about
    whether the two files should stay independently mappable before anything is built.

    A second, smaller item: the eager rail's own peak sits ~1.7 GB above the model's heap
    (11,264 MB peak vs 9,636 MB heap on the 8B; ~1.9 GB on the 3B, so roughly constant rather than
    proportional). That looks like the mapping's resident page window during the read, i.e.
    reclaimable file cache, but it has not been confirmed — worth naming before anyone reads it as
    an allocation the loader holds.

    Operationally, regardless: run big-model cells one model per process, and never read peak
    from `ps` (`rss` shows ~1 MB here — `footprint -p <pid>` is the metric). The earlier 70 GB
    reading on qwen35moe-35b was `load_model_`, the direct gguf path every non-image suite takes
    on purpose (`ab37e6984`), plus `DASLLAMA_PIN_PREFILL`'s readahead — not the rail.

12. **The three tmm2d GEMM twins are unreachable on every box in the fleet, so that lane has no
    runtime coverage at all.** `MetalQ8GemmBT` / `MetalQ8GemmBSkT` / `MetalQ8Gemm64BT` are selected
    only when a tune manifest crowns their family (`metal_tensor_crowned("gemmb_q8")` /
    `"gemmb_sk_q8"` / `"gemm64b_q8"` — `dasllama_metal_kernels.das` §4440–4452, crowns read from
    the sidecar's `runtime.metal_tensor` field). No manifest sets them: the M1's sidecar is stale
    so no crowns apply at all, and `m4.tune.json` crowns exactly `mulmm_bf16`. Their PSOs are
    therefore never compiled and no suite dispatches them — a change to those bodies passes every
    gate we have. (Their MSL is still checkable: dump the `*_t_msl` global and run
    `xcrun metal -std=metal4.0 -c` on it, which is how the kargs fold was verified.) Done = either
    a manifest crowns them on a tensor-capable box so the decode suite exercises them, or the
    kernel-coverage census reports an uncrowned tensor family as a LOUD warning the way rule 17
    already requires for a kernel no run dispatches — silence is what let this sit.
