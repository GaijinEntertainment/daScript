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
   One hypothesis is ELIMINATED: the census is not blind to graph-captured dispatches —
   `kn_pipeline` counts on the immediate path and `gr_encode_node` counts again on replay
   (`dasllama_metal_common.das:1402`), so a captured dispatch is still tallied. The zero is real.
   Consequence for reviewers: these three kernels have NO runtime coverage, so a change to them
   (the PleArgs kargs fold is one) can only be verified structurally — emitted-MSL signature plus
   bind-list-vs-signature match — until this gap closes or a synthetic gate lands the way
   `moe_mulmm_q51_gate` does for the gathered q51 mul_mm.

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

6. **KqMv width merge RESOLVED by measurement (2026-08-08) — 9 classes → 3 templates + the B8
   trio.** The A/B (bench_metal_gemv_kernels `kq_mvb*` arms, 3 launches, cls3b the honest cell)
   ruled: a live colbase on B2 costs +2% (k4) / +0.5% (k6) — the specialization was
   load-bearing, and NOT via the per-b mul (a hoisted form didn't recover it; occupancy
   unchanged; mechanism unresolved at ISA level, the number is the ruling) — while k5's
   colbase B2 measured neutral-to-BETTER (−1.4% cls3b, occupancy 448→512). Prediction outcome
   (Boris: "accidental, will merge"): k5 hit, k4/k6 miss. The merge landed anyway via form
   choice (46a6252f7): K4T/K6T carry a `TILED` static_if branch-duplicating only the
   b-loop/writeback (B4 byte-identical, B2 = production text + a dead colbase decl, measured
   free); K5T keeps one colbase spelling. K4↔K5 format merge stays ruled out (the qh
   overlay + hq staging would duplicate most of the body). B8 trio stays hand-written
   (different algorithm). Standing tripwire: the lab's production arms reference the stamped
   globals.

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

8. **MoE mul_mm family: K6 JOINED 2026-08-08 (021fffd87) — Q8/Mx4 measured OUT.** K6's
   superblock-scalar cache (`sv`/`dall`, reloaded every 8th k-block) measured SLOWER than
   reload-per-kb (gmm6 lab section in bench_metal_moe_lab, qwen3moe pp512 shapes, 3 launches:
   −2.4% ms/mm), so the stateless `stage_a` override was both the join and a perf win — the
   joined kernel is −2.0% vs its old standalone form, bit-exact. The family is four of six
   (k4/k5/q51/k6). **Q8's stateless join is REFUTED by measurement** (gmm8 lab section,
   2026-08-08: index-math stage_a form +3.4–3.6% vs the production carried-pointer walk, 3/3
   launches, both arms bit-exact; occupancy identical 1024/1024 — the cost is in-loop
   addressing, two muls per k-block vs three pointer bumps, NOT registers; the dense twin's
   unquantified AGX comment now has a number). A pointer-preserving join is BLOCKED on msl_emit:
   kernel-class members must be @ssbo/@uniform/@workgroup (plain thread-local members are a
   compile error), so a rider cannot hold loop-carried pointer state across `stage_a` calls.
   Mx4's prologue hook (pre-loop vtab staging + bias-seeded accumulators) is blocked on the
   same missing mechanism. Both stay standalone until the msl_emit lead below lands — one
   design covers both: method-flattening WITH scope splicing (a `stage_init` hook whose locals
   stay live across the spliced stage_a = template-grade text from class-grade source). That
   flattening lead is also worth ~0.5% on ALL current riders (the method-call MSL shape
   measured that much shy of flat text, joined-vs-flat-reload). Side find from the gmm8 dump:
   the `addr()` escape for a pointer walk defeats the const analysis — production Q8's weight
   buffers emit as non-const `device half*` (and win anyway, so constness is not the term).
   ALSO: the MoE lab's per-site section (enc_lab_w13*/w2/pair arms) ROTTED at the kargs
   migration (c45724dae) — its encoders bind the old uniform slots and the w1 k4 check panics;
   the lab now runs gmm6 first so the rot doesn't block it. Repair = rebind those encoders to
   MoeGemvArgs kargs buffers and re-verify each arm.

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

12. **The tensor (tmm2d) kernel twins are never SELECTED, and the one path that does exercise
    them is in no suite.** `MetalQ8GemmBT` / `MetalQ8GemmBSkT` / `MetalQ8Gemm64BT` (and the
    prefill `MetalAttnQKMmT` / `MetalAttnAVMmT` / the moe and kq `*T` forms) are chosen for
    serving only when a tune manifest crowns their family (`metal_tensor_crowned("gemmb_q8")` etc.
    — `dasllama_metal_kernels.das` §4440–4452, crowns read from the sidecar's
    `runtime.metal_tensor`). No manifest crowns them: the M1's sidecar is stale so no crowns apply,
    and `m4.tune.json` crowns exactly `mulmm_bf16`. So no decode/prefill parity cell ever runs one.

    They ARE compiled and dispatched by the tensor-race harnesses (`metal_tensor_race_decode` /
    `metal_tensor_race`), which build both PSOs through `pipeline_from_source` independent of the
    crowns and compare base-vs-twin output with `race_envelope_ok` — a real numeric check. But the
    races run only under `--tune`, so nothing in `tests/` calls them. Done = the kernel-coverage
    suite invokes both race entry points and asserts every family returns a non-empty `winner` with
    an empty `note` (that pair means "both PSOs built, both dispatched, outputs within envelope").
    That is a handful of lines and closes the whole tensor lane at once.

13. **The kargs census checks one direction only, and the obvious fix does NOT work — measured.**
    `MetalManualDispatchCensus` (`dasllama_metal_lens.das` §557) errors when
    `nkargs > 0 && nkargs != ndispatch` — a builder that binds kargs on some dispatch paths but not
    all. It is silent on `nkargs == 0 && ndispatch > 0`, which is what the tensor-race harnesses
    were: `race_gemmb_family` and `race_attn_pair` hand-bound their families' old scalar uniforms
    instead of going through `enc_*`, so folding those families left them binding 4-byte buffers
    where the kernels read a struct. Both compiled clean; only `--tune` runs them.

    The natural fix — derive a PSO→class→has-kargs map from `g_pso_X = compile_pso(metal_Y_msl, …)`
    plus `[metal_kernel(name=…)]`, then error on a function that pipelines a kargs-bearing PSO and
    dispatches without binding — was PROTOTYPED and **does not catch this bug**. It builds a good
    map (140 classes, 72 kargs-bearing, 133 PSO globals, 67 kargs-bearing) and the tree is green
    under it, but it reports ZERO at the broken commit: `race_gemmb_family` takes
    `base_pso, twin_pso : MetalComputePipeline?` as PARAMETERS, and their identity arrives through
    two levels of argument passing (`race_decode_family`'s `base_src` string parameter, bound at the
    top-level call site to `metal_q8_gemm_b_msl`). A per-function census cannot see that. Catching
    it precisely needs interprocedural constant propagation through direct calls.

    What IS checkable locally: **a dispatch site must NAME its PSO global.** `kn_pipeline(enc, X)`
    where X is not a `g_*pso*` global is an opaque dispatch the census cannot map to a class, so it
    cannot verify the kargs contract. Measured today: **17 of 89 dispatch sites are opaque, and 16
    are race harnesses** (`base_pso`/`twin_pso`/`q_pso`/`qt_pso`/`a_pso`/`at_pso`); the seventeenth
    is `enc_ew2`, which legitimately shares one builder across swiglu/geglu/add. Done = the census
    errors on an opaque dispatch unless the function carries an explicit opt-out annotation, which
    turns an invisible class of bug into an enumerated, reviewable list. Note this is ADVISORY — it
    flags "this function hand-binds", not "this function is wrong". Until it exists the rule is
    structural: nothing dispatches a kernel except its `enc_*` builder (CODEREVIEW: "Nothing dispatches a kernel except its enc_* builder").

14. **`dasllama_blocks` / `dasllama_moe` / `dasllama_ple` have no direct tests.** All three
   are pointer-dispatched forward-loop internals moved out of `dasllama_common` during the reorg;
   today their only coverage is the forward/family matrix — exactly the "the model still runs"
   posture CODEREVIEW's Tests section calls out. Their siblings from the same wave have direct
   suites (`test_batch_decode`/`test_batch_grid`, `test_prefill`/`test_flash`, `test_sampling`).
   Done = each of the three gets a feed-the-function suite (a block kernel against a reference
   forward step; `moe_ffn_core` routing against a hand-checked top-k case; the PLE pre-step
   against a fixture), or a written carve-out on the rule for pointer-dispatched move-outs.

15. **Accelerate (+AMX) arm for the audio-tower tables (Boris 2026-08-05; explicitly NOT the
   quant-lane arc).** Once the towers are q8-forced, the `--accel` flavor rows should serve the
   tower mms through the accelerate tier the way the LLM float-batch slots already do — the
   override hook (`g_mm_fp_batch_override`) and the tier plumbing exist, so the wiring is
   expected to be crazy cheap. Metal tower offload is explicitly deferred ("not metal (yet)").
   Done = accel-flavor audio cells dispatch tower mms through the accelerate driver with the
   crossover verdict recorded, default cpu rows unchanged.

16. **Profiling framework refactor — zero-overhead-when-off (Boris 2026-08-05, during the
   audio-tower probe work).** The current rails are runtime-gated hand sites: every
   `prof_add`/`asr_prof_add` call site takes `ref_time_ticks()` unconditionally and branches on
   a global inside the callee — "cute", but each new probe sprinkles more always-on ticks
   through hot loops (today's `g4a.enc.*` sites included), two parallel rails exist
   (`forward_profile` buckets vs `asr_prof` string table) with different arming and reporting,
   and the generic-path `mm_gemm` double-count chore is a symptom of sites living at the wrong
   level. Wanted: ONE macro-based rail where a profile site **compiles to nothing** unless
   profiling is armed at COMPILE time — under `-jit` every run compiles at launch, so the
   `--prof` decision can reach the macro (compile-time option/env read by the pass macro, or a
   lens-module require, the `dasllama_metal_lens` pattern), and the released bench app bakes
   the sites OUT of its timed paths entirely while a probe launch bakes them IN. Done = one
   site syntax for both rails, `--prof`-off builds contain zero profiling instructions
   (verified in emitted IR), the double-count chore resolved by construction, and the census +
   report surface preserved.

17. **Vision stills — when, not if (Boris 2026-08-05).** Image-in/text-out on the existing
   mmproj carriers (the `v.*` towers the audio work already ships past unread). Explicitly
   SEQUENCED: after the vulkan side comes together AND dasllama.io serves sidecars + hosts
   ladders — not before. Scope expectation from the audio comparison: first family ≈ a
   gemma4a-sized arc (vanilla ViT / MobileNet blocks, no DSP frontend, no realtime bar; the
   soft-token splice, q8 tower rail, .dlim lane, and mtmd oracle harness all transfer;
   qwen3vl's M-RoPE + deepstack are pre-registered and merely activate). One-time plumbing:
   image decode (dasStbImage) + per-family resize/patchify conventions. VIDEO stays out:
   not reasonable without GPU — "maybe hybrid mode, maybe one day"; its real dependency is
   the deep-prefill context wall (50k-token splices), not new kernels.

18. **Tutorials + documentation resync sweep (Boris 2026-08-05).** The ASR/audio tutorials and
   module documentation have desynced from the API as the q8/KV/lever work landed — a full
   pass over `tutorials/` (`.das` and `.rst`) and the dasLLAMA doc pages against the current
   facade surface (`set_asr_kv`, `--kv`, `--fp32-tower`, exec_fmt spellings, the q8 defaults).
   Going forward the new Documentation section in CODEREVIEW.md makes this a per-change check;
   this item is the one-time catch-up.

19. **Oracle-suite hardening round 2 (PR #3653's review round, deferred by ruling).** The
    kernels suite is census-complete but the bug-scan agents mapped axis-coverage holes where
    a targeted kernel mutation stays green: the Part/PartB split-K softcap path (hardcoded 0.0
    in those gates — distinct from the fixed single/batched arms), the SqAttnB q8 K-vs-V base
    aliasing (rt.y = rt.x in the quant branch), the D-family khoff ≡ 0 under kv_mul=2/nheads=2,
    gemm ndim=64 hiding the nBase column-tile term, gemv output buffers sized exactly (row
    guards unobservable — pair with sentinel-padded planes), the argmax tie pair never sharing
    a simdgroup (the simd_shuffle_xor tie-break unwitnessed), the misc softcap fixture spanning
    only ±0.3·cap, rope bias planes zero-filled where production dummy-binds arbitrary bytes
    (fill non-zero when hasbias=0), the neox×hasbias confound in the rope-store quant mirrors,
    kq gemv weight reads past exactly-sized buffers (blocks MTL_SHADER_VALIDATION=1 runs), the
    copy_row gate's tg=64 vs production 256, the dead b4 pad-column clamp cell, the q8 region
    fixture's sel collapsing to a k-slot function (routing untested, padding only), and the 12
    prefill-file gates still on dump-less tag-less local compares. Also latent: the two rope
    oracles index tcos/tsin at [0, hs/2) while sizing rot/2 — reads OOB and ropes the
    pass-through tail the moment a cell pairs neox=false with rot < hs (the flat gate's
    identity-pad pattern is the fix). Also: migrate the prefill file's tag-less mismatch trio
    to the shared dumping compares (~25 call sites).

20. **`vk_moe_would_accept` / resident probes vs the wg-cap decline.** The resident decline
    for an over-cap batched-attention class now lives in `vk_rdec_prepare` (live path only).
    The offline plan cannot see the cap (followup_vulkan item 19) and `vk_moe_would_accept`
    answers only format/geometry — a would-accept probe that consults device caps would let
    schedulers plan without a live ensure_ round-trip.
