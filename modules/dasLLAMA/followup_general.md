# General follow-ups - CPU-side consistency items found during the reorg

> Sibling of `followup_vulkan.md`: things spotted while moving code that are NOT reorg work
> themselves. Each item names the code, the inconsistency, and what done looks like. Perf-only
> ideas without a consistency angle keep going to `PERF_LEDGER.md`; items here are "two ways of
> doing the same thing coexist - converge them".

1. **Some RoPE application paths still compute `sin`/`cos` directly instead of using tables.**
   `dasllama_math.das` has both forms: the direct `rope_scaled` / `rope_scaled_neox` /
   `rope_scaled_neox_part` (sec.2789-2924) pay `cos(angle)`/`sin(angle)` per pair per call, while
   the bit-identical table twins (`rope_scaled_tab`, `rope_scaled_neox_tab`,
   `rope_scaled_neox_tab_part`) read a prebuilt cos/sin row (angle generation now single-sourced
   in `dasllama_rope`). Done = audit every caller of the direct forms; each either (a) migrates
   to the `_tab` twin with a row from the decode/prefill rope tables it already has, or (b) gets
   a written reason it cannot hold a table (one-shot positions, no session). Direct forms that
   lose their last caller get deleted.

2. **Some activation loops pay libm `exp` per element where an `exp4` unroll exists next door.**
   `dasllama_math.das` carries `exp4` (vectorized expf, ~2 ulp, ~7x scalar - sec.2375) and uses it
   in softmax and the fast glu family (`swiglu4`, `geglu4`, `swiglu_oai4`, `silu4`-style bodies
   sec.2566-2654), but the scalar twins still loop libm `exp` per element: `silu` (sec.2453-2459), the
   scalar swiglu/swiglu_oai bodies (sec.2514, sec.2551), and the sigmoid gate multiply (sec.2684). Done =
   every per-element `exp` loop either gets the exp4 4-lane body + scalar tail (same shape as
   sec.2566), or carries a comment naming why it must stay scalar (bit-exactness contract with a
   parity oracle - the "NOT bit-exact" caveat on the fast forms is the known constraint; decide
   per call site which side of the contract it sits on).

3. **The pipelined batch-decode landing holds borrowed Session pointers across the caller's
   deletes.** `finish_pending_step` (dasllama_metal_common) lands the in-flight batch step by
   memcpy-ing KV rows and logits through `g_lp_sessions[i]` - raw borrowed pointers. Nothing
   quiesces on session death, `metal_decode_flush` (the public landing call) has zero callers,
   and every test helper deletes its sessions right after `eval_batch_` with a step potentially
   pending - a landing after those deletes writes into freed heap. Empirically quiet today
   (probed 2026-07-30: batch-only prelude showed no corruption), but it is a lifetime landmine,
   and the server would inherit it if a chat session ever dies mid-pipeline. Done = a session
   retirement seam the drivers hook (land or abandon pending steps that reference the dying
   session), or the landing rail stops holding raw session pointers; plus `metal_decode_flush`
   calls at the test helpers' teardown as the interim belt.

4. **The GPU-PLE prefill pre-step never engages on the E4B even though the model qualifies.**
   The census (family-scoped E4B row, metal prefill serving on device - `attn_qk_mm` counted)
   shows `metal_ple_gather_q8`, `metal_ple_finish`, and `metal_bf16_mulmm` at zero, yet every
   static leg of `metal_ple_pre_gpu_gate` holds for the E4B gguf: `per_layer_model_proj` is
   native BF16, `per_layer_token_embd` is Q8_0, ple=256 passes the shape gates, and the prefill
   itself did not decline. So the gate returns false on a dynamic leg (consult-time override /
   `prefill_decline` / `metal_prefill_init` ordering) - the PLE pre-step silently runs its CPU
   fallback (`ple_pre_prefill`) on every metal prefill window. Done = instrument the gate legs,
   find the refusing one, fix or document it, and the census E4B row counts all three kernels.
   One hypothesis is ELIMINATED: the census is not blind to graph-captured dispatches -
   `kn_pipeline` counts on the immediate path and `gr_encode_node` counts again on replay
   (`dasllama_metal_common.das:1402`), so a captured dispatch is still tallied. The zero is real.
   Consequence for reviewers: these three kernels have NO runtime coverage, so a change to them
   (the PleArgs kargs fold is one) can only be verified structurally - emitted-MSL signature plus
   bind-list-vs-signature match - until this gap closes or a synthetic gate lands the way
   `moe_mulmm_q51_gate` does for the gathered q51 mul_mm.

5. **The metal kernel zoo carries four different rope addressing schemes.** Factoring the
   sq_attn family showed the rope-store family does NOT share one skeleton the way attention
   did - its eleven kernels split four ways: pair-per-thread WITH partial rotation
   (`rope_store16/32` + the b twins: `rot`/`halfr` with an identity-cover tail),
   pair-per-thread WITHOUT it (`rope_store_q8` + `b_q8`: whole-head rotation, `half` only),
   element-per-thread (`rope_store_tq4`/`b_tq4`/`h16`, threadgroup-wide, and `h16` restores
   partial rotation), and the fused-GEMV form (`q8_gemv_qkv_rs16/32`, index derived from the
   weight row). Only ~60 lines are mechanically shared, so deduping them buys little AND would
   paper over the question worth answering: are four schemes load-bearing (each tuned to its
   store) or is this drift that one scheme could serve? A wrong answer changes each kernel's
   lane mapping, so it needs a measurement, not a refactor. Done = each scheme either justified
   in a comment at its kernel or collapsed into the one that measures as fast.

6. **KqMv width merge RESOLVED by measurement (2026-08-08) - 9 classes -> 3 templates + the B8
   trio.** The A/B (bench_metal_gemv_kernels `kq_mvb*` arms, 3 launches, cls3b the honest cell)
   ruled: a live colbase on B2 costs +2% (k4) / +0.5% (k6) - the specialization was
   load-bearing, and NOT via the per-b mul (a hoisted form didn't recover it; occupancy
   unchanged; mechanism unresolved at ISA level, the number is the ruling) - while k5's
   colbase B2 measured neutral-to-BETTER (-1.4% cls3b, occupancy 448->512). Prediction outcome
   (Boris: "accidental, will merge"): k5 hit, k4/k6 miss. The merge landed anyway via form
   choice (46a6252f7): K4T/K6T carry a `TILED` static_if branch-duplicating only the
   b-loop/writeback (B4 byte-identical, B2 = production text + a dead colbase decl, measured
   free); K5T keeps one colbase spelling. K4<->K5 format merge stays ruled out (the qh
   overlay + hq staging would duplicate most of the body). B8 trio stays hand-written
   (different algorithm). 2026-08-09: adoption closed the A/B - the K4T/K6T `TILED=false`
   stamps and K5T's single spelling ARE the previewed lab forms, so the three prod-vs-lab B2
   pairs compiled byte-identical and were deleted (the measured ruling above stands as the
   record; recreate the arm pattern if the question reopens). The tripwire stands: the lab's
   surviving production arms reference the stamped globals.

7. **Pointer families - a language-level idea, parked.** (Boris, 2026-07-30: "interesting follow ...
   it maybe good - we are just not there yet.") Everything above about address spaces exists
   because `float4?` says what a pointer points AT and nothing about where it lives. MSL needs
   that (`device` vs `threadgroup` are different types there and do not implicitly convert), so
   today a helper cannot take a pointer at all, and the near-term fix is a parameter annotation
   the emitter cross-checks against the argument's provenance. The deeper version is to give
   daslang pointer FAMILIES - the memory family as part of the pointer type, so the checking is
   the type system's job rather than an emitter's. It would pay off beyond shaders (the same
   information a CPU backend wants for aliasing, and what any target with more than one memory
   wants), but it reaches inference, mangling, and every cast, so it is a language design task
   and not a step in this arc. Parked deliberately, not forgotten.

8. **MoE mul_mm family: COMPLETE - all six formats ride MetalMoeMulMmBase.** K6 joined
   2026-08-08 (021fffd87, stateless stage_a, -2.0%). Q8 and Mx4 joined the same day via
   msl_emit's flatten/scope-splice arc (plans/msl-flatten.md): Q8 keeps the carried-pointer
   walk (plain `scur`/`qp` members bound in `stage_init`, advanced in stage_a - the
   stateless index form measured OUT +3.4-3.6%), -0.5..-0.9% vs its deleted standalone;
   Mx4 rides `stage_init` (vtab staging) + `stage_acc` (per-expert bias seed into the
   accumulator array), +0.02% flat vs its standalone (DRAM-bound - the join is free). Both
   binding contracts moved to the family numbers (xf@3 y@4 cnt@7 basep@8 bkt@9;
   `kn_moe_mm_family_tail` spells the tail once; tensor twins keep their compact layouts).
   Side find from the gmm8 dump: the `addr()` escape for a pointer walk defeats the const
   analysis - Q8's weight buffers emit as non-const `device half*` (and win anyway, so
   constness is not the term).
   The per-site rot from the kargs migration (c45724dae) was REPAIRED in the lens-arc review
   round (2026-08-09): production arms bind MoeGemvArgs kargs, the pre-kargs lab twins keep
   their historical layouts behind per-arm bind splits, the dead tail-duplicate
   run_gmm4/gmm6_lab calls are gone, and main runs end-to-end to the leak assert - all 19
   correctness checks at rel 0, run_gmx4_lab reachable again (prod 300.6 wGB/s vs gmx4_lcppe
   302.1, prod = the gmx4_lcppe arm + an untaken bias branch). Don't trust rounds=1 numbers from this
   lab - warm-up dominates.

9. **Prefill compiles its own PSO for kernels decode already has - HALF RESOLVED by the lens
   arc.** The qk_norm half closed in P1: `MetalQkNorm` gained `@off` on x, prefill rides
   decode's generated builder, and `enc_qk_norm_pf` + `g_pf_pso_qknorm` are gone. The rope
   half remains in the new shape: both stages' rope encoders are lens-generated now, but
   prefill's `MetalRope` instance names `g_pf_pso_rope` compiled from the same
   `metal_rope_msl` source decode compiles into its own PSO. The ownership rule is written
   (REVIEW.md: the class-owning file compiles/releases; instances may share a pso handle -
   the MetalRmsNorm x3 / tensor-twin precedent), so Done = prefill's rope instance either
   names decode's pso global the shared-handle way or a comment says why stage-local is
   intended.

10. **The two fused QKV-GEMV twins are a D-family-shaped dedup that has not happened.**
    `MetalQ8GemvQkvRsF16` and `MetalQ8GemvQkvRsF32` are ~95 lines of near-identical body that
    differ only in the mirror's destination views - the same shape the split-K attention family
    had before it was split into `sqd_*` stages. Both are over the STYLE038 80-line cap and the
    kargs fold pushed them further (the seven unpacked fields are each used 3-19 times, so
    inlining `ka.field` instead would be noisier, not shorter). Done = the pair shares its GEMV,
    rope and store stages the way the D family shares `sqd_score_blk`/`sqd_vacc_blk`, and both
    bodies come back under the cap.

11. **Every cold mint streams; what is left is the vulkan flavor's SIZE, not its peak.**
    Peak `phys_footprint_peak` of one load, Meta-Llama-3.1-8B-Q8_0 (8 GB gguf -> 9.6 GB image),
    M1 Max:

        cold planar (streamed)   5,380 MB   4.5 s
        cold metal  (streamed)   4,848 MB   3.1 s
        cold vulkan (streamed)   6,570 MB   15.5 s   (mints TWO images)
        warm, any flavor           651 MB

    A mapped serve lands at ~0.07x the image, and Metal wraps the mapping with
    `metal_new_buffer_no_copy_untracked` exactly as Vulkan and the CPU do. The metal blob comes out
    *under* planar because a blob forbids CPU repack and so never needs the resident `qscales`
    plane. The vulkan bake rides the GPU walk of the model that is already serving - the warm path
    always did this off a mapping, and the cold path now does too instead of loading eagerly.

    What remains is that a **vulkan cold mint writes ~22 GB to produce an 11.8 GB artifact**: the
    planar image lands first (10.0 GB), then the flavor image re-writes all of that content plus
    the 1.6 GB bake blob. The flavor is a superset of the planar image, so the second write copies
    9.6 GB of bytes that already exist on disk a few hundred milliseconds earlier - 8.3 s of the
    15.5 s total. Done = either the flavor image references the planar one for its shared planes,
    or a vulkan mint skips writing the planar image it is about to supersede. Neither is a peak
    problem (the peak is fine), so this is wall-clock and disk, and it wants a decision about
    whether the two files should stay independently mappable before anything is built.

    A second, smaller item: the eager rail's own peak sits ~1.7 GB above the model's heap
    (11,264 MB peak vs 9,636 MB heap on the 8B; ~1.9 GB on the 3B, so roughly constant rather than
    proportional). That looks like the mapping's resident page window during the read, i.e.
    reclaimable file cache, but it has not been confirmed - worth naming before anyone reads it as
    an allocation the loader holds.

    Operationally, regardless: run big-model cells one model per process, and never read peak
    from `ps` (`rss` shows ~1 MB here - `footprint -p <pid>` is the metric). The earlier 70 GB
    reading on qwen35moe-35b was `load_model_`, the direct gguf path every non-image suite takes
    on purpose (`ab37e6984`), plus `DASLLAMA_PIN_PREFILL`'s readahead - not the rail.

12. **The tensor (tmm2d) kernel twins are never SELECTED, and the one path that does exercise
    them is in no suite.** `MetalQ8GemmBT` / `MetalQ8GemmBSkT` / `MetalQ8Gemm64BT` (and the
    prefill `MetalAttnQKMmT` / `MetalAttnAVMmT` / the moe and kq `*T` forms) are chosen for
    serving only when a tune manifest crowns their family (`metal_tensor_crowned("gemmb_q8")` etc.
    - `dasllama_metal_kernels.das` sec.4440-4452, crowns read from the sidecar's
    `runtime.metal_tensor`). No manifest crowns them: the M1's sidecar is stale so no crowns apply,
    and `m4.tune.json` crowns exactly `mulmm_bf16`. So no decode/prefill parity cell ever runs one.

    They ARE compiled and dispatched by the tensor-race harnesses (`metal_tensor_race_decode` /
    `metal_tensor_race`), which build both PSOs through `pipeline_from_source` independent of the
    crowns and compare base-vs-twin output with `race_envelope_ok` - a real numeric check. But the
    races run only under `--tune`, so nothing in `tests/` calls them. Done = the kernel-coverage
    suite invokes both race entry points and asserts every family returns a non-empty `winner` with
    an empty `note` (that pair means "both PSOs built, both dispatched, outputs within envelope").
    That is a handful of lines and closes the whole tensor lane at once.

13. **The kargs census checks one direction only, and the obvious fix does NOT work - measured.**
    `MetalManualDispatchCensus` (`dasllama_metal_lens.das` sec.557) errors when
    `nkargs > 0 && nkargs != ndispatch` - a builder that binds kargs on some dispatch paths but not
    all. It is silent on `nkargs == 0 && ndispatch > 0`, which is what the tensor-race harnesses
    were: `race_gemmb_family` and `race_attn_pair` hand-bound their families' old scalar uniforms
    instead of going through `enc_*`, so folding those families left them binding 4-byte buffers
    where the kernels read a struct. Both compiled clean; only `--tune` runs them.

    The natural fix - derive a PSO->class->has-kargs map from `g_pso_X = compile_pso(metal_Y_msl, ...)`
    plus `[metal_kernel(name=...)]`, then error on a function that pipelines a kargs-bearing PSO and
    dispatches without binding - was PROTOTYPED and **does not catch this bug**. It builds a good
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
    turns an invisible class of bug into an enumerated, reviewable list. Note this is ADVISORY - it
    flags "this function hand-binds", not "this function is wrong". Until it exists the rule is
    structural: nothing dispatches a kernel except its `enc_*` builder (REVIEW: "Nothing dispatches a kernel except its enc_* builder").

14. **`dasllama_blocks` / `dasllama_moe` / `dasllama_ple` have no direct tests.** All three
   are pointer-dispatched forward-loop internals moved out of `dasllama_common` during the reorg;
   today their only coverage is the forward/family matrix - exactly the "the model still runs"
   posture REVIEW's Tests section calls out. Their siblings from the same wave have direct
   suites (`test_batch_decode`/`test_batch_grid`, `test_prefill`/`test_flash`, `test_sampling`).
   Done = each of the three gets a feed-the-function suite (a block kernel against a reference
   forward step; `moe_ffn_core` routing against a hand-checked top-k case; the PLE pre-step
   against a fixture), or a written carve-out on the rule for pointer-dispatched move-outs.

15. **Accelerate (+AMX) arm for the audio-tower tables (Boris 2026-08-05; explicitly NOT the
   quant-lane arc).** Once the towers are q8-forced, the `--accel` flavor rows should serve the
   tower mms through the accelerate tier the way the LLM float-batch slots already do - the
   override hook (`g_mm_fp_batch_override`) and the tier plumbing exist, so the wiring is
   expected to be crazy cheap. Metal tower offload is explicitly deferred ("not metal (yet)").
   Done = accel-flavor audio cells dispatch tower mms through the accelerate driver with the
   crossover verdict recorded, default cpu rows unchanged.

16. **Profiling framework refactor - zero-overhead-when-off (Boris 2026-08-05, during the
   audio-tower probe work).** The current rails are runtime-gated hand sites: every
   `prof_add`/`asr_prof_add` call site takes `ref_time_ticks()` unconditionally and branches on
   a global inside the callee - "cute", but each new probe sprinkles more always-on ticks
   through hot loops (today's `g4a.enc.*` sites included), two parallel rails exist
   (`forward_profile` buckets vs `asr_prof` string table) with different arming and reporting,
   and the generic-path `mm_gemm` double-count chore is a symptom of sites living at the wrong
   level. Wanted: ONE macro-based rail where a profile site **compiles to nothing** unless
   profiling is armed at COMPILE time - under `-jit` every run compiles at launch, so the
   `--prof` decision can reach the macro (compile-time option/env read by the pass macro, or a
   lens-module require, the `dasllama_metal_lens` pattern), and the released bench app bakes
   the sites OUT of its timed paths entirely while a probe launch bakes them IN. Done = one
   site syntax for both rails, `--prof`-off builds contain zero profiling instructions
   (verified in emitted IR), the double-count chore resolved by construction, and the census +
   report surface preserved.

17. **Vision stills - DELIVERED for the first family (the vision arc, 2026-08-14).** gemma-4
    dense via `gemma4uv` shipped end to end: preprocessing, non-causal span, chat/scheduler/
    server splice, the `.dlim` rail, `ask --image`, and the `lcpp_bench --image` cell. What
    remains of this item folded into #23 (uniform-bound GPU prefill kernels) and the v2 ledger
    in `vision_plan.md` (the E-series `gemma4v` ViT, multi-image, remote fetch, video).
18. **Tutorials + documentation resync sweep (Boris 2026-08-05).** The ASR/audio tutorials and
   module documentation have desynced from the API as the q8/KV/lever work landed - a full
   pass over `tutorials/` (`.das` and `.rst`) and the dasLLAMA doc pages against the current
   facade surface (`set_asr_kv`, `--kv`, `--fp32-tower`, exec_fmt spellings, the q8 defaults).
   Going forward the new Documentation section in REVIEW.md makes this a per-change check;
   this item is the one-time catch-up.

19. **Oracle-suite hardening round 2 (PR #3653's review round, deferred by ruling).** The
    kernels suite is census-complete but the bug-scan agents mapped axis-coverage holes where
    a targeted kernel mutation stays green: the Part/PartB split-K softcap path (hardcoded 0.0
    in those gates - distinct from the fixed single/batched arms), the SqAttnB q8 K-vs-V base
    aliasing (rt.y = rt.x in the quant branch), the D-family khoff == 0 under kv_mul=2/nheads=2,
    gemm ndim=64 hiding the nBase column-tile term, gemv output buffers sized exactly (row
    guards unobservable - pair with sentinel-padded planes), the argmax tie pair never sharing
    a simdgroup (the simd_shuffle_xor tie-break unwitnessed), the misc softcap fixture spanning
    only +/-0.3*cap, rope bias planes zero-filled where production dummy-binds arbitrary bytes
    (fill non-zero when hasbias=0), the neoxxhasbias confound in the rope-store quant mirrors,
    kq gemv weight reads past exactly-sized buffers (blocks MTL_SHADER_VALIDATION=1 runs), the
    copy_row gate's tg=64 vs production 256, the dead b4 pad-column clamp cell, the q8 region
    fixture's sel collapsing to a k-slot function (routing untested, padding only), and the 12
    prefill-file gates still on dump-less tag-less local compares. Also latent: the two rope
    oracles index tcos/tsin at [0, hs/2) while sizing rot/2 - reads OOB and ropes the
    pass-through tail the moment a cell pairs neox=false with rot < hs (the flat gate's
    identity-pad pattern is the fix). Also: migrate the prefill file's tag-less mismatch trio
    to the shared dumping compares (~25 call sites).

20. **`vk_moe_would_accept` / resident probes vs the wg-cap decline.** The resident decline
    for an over-cap batched-attention class now lives in `vk_rdec_prepare` (live path only).
    The offline plan cannot see the cap (followup_vulkan item 19) and `vk_moe_would_accept`
    answers only format/geometry - a would-accept probe that consults device caps would let
    schedulers plan without a live ensure_ round-trip.

21. **`harness/tq4_probe.das` helpers have no tests.** `rotate_row` / `unrotate_row` /
    `row_dot` were rewritten to zip loops in the lint sweep (PR #3733) with no coverage
    before or after - feed each a known row, check the bytes. Note the zip form stops at
    the shorter operand where the old indexed form panicked on mismatched lengths;
    current callers pass equal lengths, so the test should pin the equal-length results,
    not the mismatch behavior.

22. **Tune-sidecar rejection under `--gpu metal` - RESOLVED (2026-08-15): the staleness rail
    working correctly behind an uninformative message.** Forensics: the refused
    `main.tune.json` was an Aug-10 mint whose mtime predated the Aug-13 rebuilt binary
    (stale reads as absent), while `ask.tune.json` was minted fresh by that binary - and
    `--gpu metal` was a pure confound: the demand set is arm-invariant (one compiled
    program; verified by an identical per-kernel stamp-line diff across cpu/metal server
    arms). Neither reading (a) nor (b) held. Fixed in llvm_tune: every untuned refusal now
    names its reason (stale-binary with both dates / foreign-box / version / missing
    entries / absent / unreadable), `DAS_TUNE_MANIFEST` at an untuned-reading file warns
    loudly instead of silently stamping fallbacks, and `[tune_scope] version_of=` pins the
    scope to `DASLLAMA_RELEASE` so a copy-restored old sidecar (fresh mtime, old winners -
    the trap the forensics surfaced) refuses by name. Metal-arm measurement is unblocked;
    the owed quiet-box mint landed with the release-scope re-mint
    (`performance/last_known_good_sidecar.json` and the rig sidecars are fresh under it).

23. **Uniform-bound non-causal prefill kernels for Metal and Vulkan.** The image span
    currently serves on the CPU loop by decline: `attn_gpu_prefill_ready`,
    `prefill_decline` (`MetalPrefillDecline.non_causal_span`) and `vulkan_resident_prefill`
    all refuse `s.attn_uniform_end != 0`, and metal-blob models refuse a vision arm outright
    at create (no CPU weights to fall back on). Done = the GPU prefill chains take the
    uniform bound, the three declines and the blob refusals are deleted, and the kernel
    coverage suite dispatches the non-causal arm. Scheduled for the Metal arc's tail -
    pulled forward only if CPU-fallback vision proves a blocker (Boris 2026-08-14).
    **Metal half DONE (metal-media chunk 1, 2026-08-16):** the trio takes the bound as
    `AttnArgs.uend` (no separate kernel - the census criterion is moot), the declines and
    blob refusals are deleted, and the span parity arm + support-matrix engage cell gate it.
    REMAINING: the Vulkan half (`vk_moe_attn` signature surgery, device KV pre-seed,
    `AT_CTX`), on the PC box.

24. **Stream the over-line audio-carrier mints (the 1 GiB staging rule, 2026-08-14).**
    `REVIEW.md` caps `cache_via_image_staged` at a 1 GiB source file; six carriers on the
    fleet stage over it today - mmproj-omni-3b-f32 (5.0 GB), canary-qwen-2.5b-encoder-f32
    (3.1 GB), mmproj-ultravox-1b-f32 (2.6 GB), voxtral-mini-mmproj-f32 (2.5 GB),
    qwen2audio-mmproj-f32 (2.4 GB), mmproj-Qwen3-Omni-30B bf16 (2.1 GB) - each paying a
    staged peak of roughly source + built image. Done = the audio-family mints size the
    image up front and write planes as produced (the decoder rail's shape), the staged
    entry points refuse a source over the line, and the mint-form-differs conversions
    restructure into per-plane producers. The vision carriers stay staged (gemma-4 12B
    mmproj 0.2 GB, E2B 0.9 GB).

25. **OpenAI-standard audio/image API surface - conformance, decisions, hygiene (2026-08-16).**
    The server implements `image_url`/`input_image` chat parts (data:-only) and
    `/v1/audio/transcriptions` (`json`/`text`/`verbose_json`, `language`, VAD segments).
    The rest of the standard surface, tiered:
    - CONFORMANCE DEBT (plumbing, no new capability): `input_audio` chat parts plus the
      ecosystem `audio_url` twin (accept both - audio-in-chat rides the existing
      `render_prompt_media` splice); `srt`/`vtt` transcription response formats;
      `timestamp_granularities[]` where the family has word timestamps; `prompt` biasing;
      `stream=true` transcription events; `/v1/audio/translations` for the whisper family
      (native decoder mode; other families decline); accept-and-ignore `image_url.detail`.
    - CAPABILITY DECISIONS (Boris's call, each a new model class): TTS - `/v1/audio/speech`
      and chat `modalities:["text","audio"]`. No served artifact can speak; the two Omni
      families have Talkers upstream but the GGUF ecosystem carries only thinker + audio
      encoder, and a talker conditions on thinker HIDDEN STATES (not a bolt-on). The
      reference-backed route if wanted: a dedicated small TTS family (the upstream tts
      example - OuteTTS + WavTokenizer ggufs). Realtime API (WebSocket voice, barge-in) is
      the end-state the smaller audio choices point at; name it before choosing them.
    - DECLINE/PARK: `/v1/images/generations` (+edits/variations) - no roster model
      generates images even upstream; diffusion is a disjoint class (DiT/UNet + conv2d VAE,
      no upstream reference; the GGML reference is stable-diffusion.cpp, which shares
      our quants/GGUF/GEMMs but not the graph). `file`/`video_url` parts likewise parked.
    - HYGIENE: our `/v1/images` is the dlim-inventory/bake endpoint - a name squat on the
      standard image-API prefix; rename ours or accept the squat deliberately.

26. **ASR perf follow-ups after the Metal tower - MOSTLY RESOLVED by metal-media chunk 3
    (2026-08-16, `history/dasLLAMA/metal_media_plan.md` slices M-R).** The decoder half SHIPPED (cross-KV +
    the q8-native decode step on Metal, part/comb attention, f16 KV - decode beats the
    q8-CPU rail 1.6x and serves by default above the `n_text_state >= 1024` floor); J-qwen3a
    SHIPPED (the conv2d frontend pads into the tile GEMM - encode 6.3x on the stage probe
    (`harness/asr_stage_probe.das`, no board cell), the q8 serving default included); the
    whisper conv frontend rides the GPU too (37x, same probe). gb1 large-v3-turbo lands
    4.04 s vs whisper-cli's 5.54 (both celled, `records/m1.json`). What remains:
    - **The q8 tower lane**: transform-vs-upload-dequant, measurement-driven - the encoder
      blocks still ride the f32 lane under `--ngl` (the one remaining fp32-forced half).
    - **Audio mmproj provenance**: `qwen2audio-mmproj-f32.gguf` and
      `voxtral-mini-mmproj-f32.gguf` are locally-converted fixtures with no
      `performance/fetch_models.das` entry - the tests that load them skip honestly without
      them, but the files are not re-fetchable from the manifest. Mint the entries (or a
      documented convert recipe) rather than inventing provenance.
    - **The whisper decode step's small-model floor**: tiny (d=384) loses ~3x to the
      per-dispatch latency floor and keeps the CPU rail by policy - a graph/ICB replay or
      megafusion round would move the floor; measured, ledgered, not urgent.

27. **The metal suites should catch a raw (runner-less) invocation (2026-08-17).** Running
    `dastest -- --test modules/dasLLAMA/tests` directly - without `tests/run.das` - drops the
    runner's env (`DASLLAMA_CPU_PREFILL=1`, `DASLLAMA_METAL_HAZARD_STRICT=1`,
    `DAS_TUNE_MANIFEST=m1.tune.json`) and produces phantom reds that read like real
    regressions: the CPU-truth A/B halves trip the CPU-prefill panic, untuned identities
    collide with the dlim flavor-lane fixtures, and the combined run even segfaulted. The
    prohibition exists in `tests/CLAUDE.md` but nothing ENFORCES it - a session that skips
    the doc burns an hour on fake failures. Add a guard test (or a `_model_tier` setup
    check) that detects the missing runner markers and fails IMMEDIATELY with "run through
    modules/dasLLAMA/tests/run.das", instead of letting the suite die deep. Design choice
    for implementation: refuse outright vs loud-warn; and keep single-file dastest runs
    OUTSIDE the metal-suite set (test_whisper.das etc.) unaffected.

28. **CPU fallback on anything but tiny should be LOUD (2026-08-17).** The review round's
    pattern: the GPU drivers decline silently by design (best-effort inside knobs), which is
    right for tiny - the floor genuinely serves it better on CPU - but on any above-floor
    model a silent CPU fallback is a performance cliff the user only finds by profiling.
    Discussion to have: a single loudness policy for the serving paths - e.g. every decline
    of a model the driver WOULD normally serve (above the floor, right quant, registered
    family) logs one line naming the reason, while policy-class declines (floor, knob-off)
    stay counter-only. Touches the tower conv frontends' bare-false contract, the wdec
    decline notes (partly landed in the review round), and the required-mode asymmetry the
    round ledgered. Decide the policy once, then sweep the drivers to it.

29. **The f32-fallback REVIEW rule's form defects - parked for REVIEW.das (2026-08-17).**
    The rule as landed has a stale name census (misses `matmul`, `gemm_f32_jo`, `mm_fblob*`,
    `enc_f32_mm`), a twin-existence clause not decidable from a diff, and a self-exempting
    inline `unless`. The dragon's proposed repair - a `// f32: <reason>` marker on every
    f32-matmul call line, mechanically checkable like `// clock: control` - is sound but
    parked: the coming executable-checklist mechanism (REVIEW.das) is the better home for a
    rule that is really a lint. Revisit when that lands.

30. **REVIEW_GPU's derived-uniform clause - parked for REVIEW.das (2026-08-17).** The
    "no value reaches an encoder twice" rule condemns the file's own standing shape (the
    grid extent re-bound as the device-side tail guard) and cannot say so. Once REVIEW.das
    lands, the check is mechanical: the [metal_dispatch] lens knows every grid= param and
    binding, so "an argument other arguments determine is a defect, except the grid extent
    as the bounds guard" becomes an auto-test; the rule text then shrinks to the sanction.

31. **`pin_kernel_backend` is not load-sticky (2026-08-17).** A model load's internal
    selection overrode an `arm64-gen` pin mid-cell (portable pins survive - the asymmetry is
    availability-gated re-selection), which corrupted the wdec attach until the driver moved
    to the pure read. Decide the contract: the pin survives loads, or its doc states it pins
    only until the next load - then sweep the A/B seats that assume the stronger reading.

32. **`daspkg --quick` silently downgrades on an incomplete sidecar (2026-08-17).** Finding
    the scope incomplete, --quick re-mints in NORMAL mode - a quick rebuild can quietly strip
    the record-grade rig of its paranoid winners (observed: 5 tie-class winner flips, oracle
    INCOMPARABLE until the LKG restore). Safer contract: refuse and name the LKG restore, or
    re-mint in the sidecar's previous mode.

33. **Lint/test-cell candidates from the metal-asr review round (2026-08-17).**
    - `[hot_path]` region contracts are blind across `invoke`d function-pointer seams (the
      four new GPU hooks) - the PERF026-028 scan cannot traverse them.
    - The tests/REVIEW CMakeLists rule is a pure grep: a model-free cell walking every
      CMakeLists.txt for `modules/dasLLAMA/tests/` basenames.
    - Three `test_metadata.py` cells for site-dasllama: page metadata (title/description/
      OG/Atom), news-region idempotence (regenerate into tmp, byte-compare), and the `dl-*`
      prefix ban.
    - The site dl-* shared-selector parity check (parse both files, intersect selectors,
      assert identical bodies).

34. **ModelSpec unification - the staged README rendering (2026-08-18, remainder).** The
    catalog move SHIPPED: one `ModelSpec` table (`performance/model_specs.das`, re-exported
    by `profile_common`) carries catalog fields + `official` flag/note + provenance +
    companions + parity evidence as data; `llm_catalog` / `official_catalog` /
    `models_provenance` are views, `asr_catalog` specs carry their own provenance,
    `test_parity` runs one generic loop over evidence, and the board pregate
    (`lcpp_bench --parity`) refuses a model whose fixture does not reproduce, stamping
    `parity` into das rows. STAGED BEHIND: the README support-matrix table becomes a
    generated rendering of the table (gen_env_doc pattern; authored arch-prose rides as a
    field). Found while scoping it: the README matrix claims frozen `test_parity.das`
    fixtures for GLM-4.5-Air, Phi-3.5, Mistral-Medium-3.5 and Gemma-4-31B that do not exist
    in the tree - the generation arc starts by reconciling those rows (re-mint or re-word).

35. **`@exact_size` - DONE as a lint contract, not a runtime mode (2026-08-18).** The
    declaration-site annotation landed on variables, fields and by-ref parameters (locals gained
    `@` metadata grammar for it); it changes nothing at runtime - PERF032 holds every
    `resize`/`resize_no_init` on an annotated array to a `reserve`/`ensure_capacity` earlier in
    the same function, helpers like `reserve_resize`/`grow_resize`/`zeroed_resize` staying
    transparent. Annotated: the deltanet state pair, `moe_gout`/`moe_eout`, every T-scaled
    `EncoderState` buffer, `VisionImage.rgb`, the k6 scale-split staging local, the ASR requant
    helper parameters, and `embq`/`embs`. The runtime exact-growth mode was judged not doable
    (an argument passed by reference, a copy-initialized variable - the flag has no home).

36. **DONE (2026-08-18) under #35:** the vulkan bake-trim `embq`/`embs` bare resizes in
    `dasllama_gpu_resident.das` were the first PERF032 hits - now `reserve_resize`.

37. **The Mistral-7B-v0.3 parity fixture is red on clean master (2026-08-18).** Verified by a
    detached-master run (54 tests, that one red); the arm now runs as
    `pinned-greedy parity: Mistral-7B-Instruct-v0.3-Q8_0.gguf` inside `test_parity_specs`.
    PARITY_FULL-gated, so per-PR CI never sees it. The eyeball rail now shows the shape:
    38/40 match, then "...interest in the technology" (oracle) vs "...in the field" (das) -
    a prose-tail near-tie of the "Once upon a time" prompt, the exact class the fixture
    header disallows. Needs its own session: regression vs upstream re-upload vs stale
    expectation (stash-and-rerun, then the model-file sha against the manifest); the likely
    resolution is re-freezing on the counting prompt like every other carrier.

38. **Large-tier parity fixtures have no CI lane (2026-08-18; narrowed).** The official
    set's evidence is now exercised by every board sweep (the parity pregate - the de facto
    liveness lane), and the invariants test pins official => evidence. What remains
    CI-blind: the non-official large-tier fixtures (the kq trios, the 26B Q8, the
    plain-stem 35B, Coder-Next) still run only under `DASLLAMA_PARITY_FULL` on dev boxes.
    Decide whether a nightly PARITY_FULL job on a model-stocked runner is worth standing up
    for that residue.

39. **The ASR reference receipt collapses a clip ladder to one argv (2026-08-18).** Reference
    ASR tools take one clip per invocation, and `gen_bench_records.das` stores only the LAST
    clip's command line ("they differ only in -f"), so the site renders hp0x2's receipt under
    every clip row of that run - a reader reproducing jfk gets the hp0x2 number. Fix is a
    per-clip `cmd` (schema addition) or a receipt template with the `-f` slot marked; the das
    rows are unaffected (one process serves the whole ladder).

40. **Batched PLE on the GPU - the E-series multi-stream Metal arm (long-term).** The Metal
    batch-decode path clears `MetalNeed.ple` from its implemented set (`BATCH_NEEDS_OK` in
    `dasllama/dasllama_metal_shapes.das`), so E-series models decline `feature` on batched
    steps and the scheduler falls back to per-row decode - correct, but the multi-stream
    serving tier loses its batching win exactly on the edge models built for it. The prefill
    driver already carries batched PLE kernels (gather + finish, `dasllama_metal_prefill.das`),
    so the shape exists; the batch-decode arm needs the per-stream token-id gather plumbed
    through the batch kargs. Upstream has no batched-PLE serving path either, so this is
    differentiation, not parity catch-up. Done = `BATCH_NEEDS_OK` carries `ple`, a batch-decode
    E-series cell in the support matrix, and a scheduler-level A/B showing the batched step
    beats N per-row steps on an E-series carrier.

- **`check_committed_records` gate in `performance/REVIEW.das`** (fused-image-span review round):
    machine-check the two records-provenance rules - every `runs[]` row's
    `hardware.remote_desktop == "off"`, every row `sha` and sidecar `provenance.engine_sha`
    an ancestor of HEAD, and `provenance.dasllama_version` equal to `DASLLAMA_RELEASE` at that
    commit. When it lands, the two prose rules in `performance/REVIEW.md` collapse to the
    "weakening that gate is a defect" residue form. Also owed: a re-mint of the gemma-3-4b cpu
    image row (hand-minted in the gemma3v arc; still stamps `parsec` + a pre-branch sha, and
    the image sweep's catalog does not reach it).

41. **Audio-in-chat serves the whisper-class `AudioTower` families only - the qwen3a conformer
    (Qwen3-Omni) and gemma4a have no chat splice.** Surfaced by the omni showcase test (the
    vision arc): `add_user_audio_`/`create_chat_(model, tower)` ride `AudioTower`
    (qwen2a/ultravox/voxtral); `load_audio_tower` reads the legacy `clip.projector_type` key
    the dual-tower Omni mmproj does not carry, and the qwen3a conformer is a different type
    (`Qwen3aTower`) served through the ASR route. Done = an audio-encoder seam the chat layer
    can hold for either type (the `VisionEmbedder` union pattern), `add_user_audio_` routed
    by family, and the Omni showcase upgraded to the one-session three-modality form.
    OWNED BY the qwen3vl arc (`qwen3vl_plan.md` sec.Arc followups) - it lands there, not here.

42. **The CPU decoder prefill chain carries no `[hot_path]` region entry, so its mid-runtime
    loops ride uncovered by the perf-lint contracts.** Surfaced by the qwen3vl review round:
    the region-entry rule now names the CPU decoder's `forward_*` entries as region entries,
    but annotating them (`forward_prefill_body` / `forward_prefill_embd` / `generate_`) arms
    the allocation contracts down a large existing call graph - a lint-burn-down of its own,
    not a ride-along on a feature arc. Done = `[hot_path]` (or the narrower contracts) on the
    CPU decoder entries with the findings triaged, covering the mrope/deepstack loops
    (`build_rope_tabs_imrope`/`_mrope`, `mrope_span_positions`, `ds_add_slice`,
    `prefill_rope_tables`, `eval_embd_span_mrope_`) the vision arc added.

43. **The tier-1 tower gates (test_qwen3v, test_qwen25v, and the gemma siblings) silently
    serve a prepared `.dlim` beside the mmproj, bypassing the gguf load path they claim.**
    Surfaced by the qwen3vl TDD round: dropping the conv-pair fold left both qwen tier-1
    gates bit-identically green on a box holding the prepared images; the same mutation reds
    at 10.1 under `DASLLAMA_IMAGE=0`. No in-process lever exists (`g_env_engine` loads once
    at context init, immutably - by design). Done = a `DASLLAMA_IMAGE=0` leg (runner or
    per-file env plumbing) or a cold-mint arm in the tier-1 gates, so a loader mutation reds
    on any box.

44. **The qwen25v (Qwen2.5-Omni/VL window ViT) CPU encode stays ~4.6x behind mtmd's clip
    (released `lcpp_bench --image` vs patched llama-mtmd-cli, CPU, --image-think, r=3, t=8,
    M1 Max) -
    the tower is ruled exact-only (ARCHITECTURE_MEDIA.md 1.7b: per-32-block activation requant
    cannot represent its outlier rows; a q8q8 lane measured 2.0 x rms where a deleted layer
    measures less).** Two honest paths if that encode ever matters: the Metal tower for the
    qwen ViT families (the same slice the qwen3v towers await), or outlier-aware activation
    quant (SmoothQuant-style per-channel folds baked at stage - needs a calibration set and
    its own gate design). Done = either path serving the Omni-3B encode with a
    poison-discriminating tier-1 gate.

45. **RESOLVED (2026-08-30, the release-remint arc) - the fixture/assert sync is a
    `modules/dasMetal/REVIEW.das` cell.** `check_fail_closed_sync` walks
    `tests/msl/_fail_closed/*.das` and the `check_rejects` calls both directions; either
    drift is a finding (negative-controlled both ways at landing). The module's REVIEW.das
    already existed (the RP law) by the time this landed.

46. **The `// clock: control` marker (dasllama REVIEW.md's ad-hoc-profiling split) has no
    mechanical check.** Done = a `REVIEW.das` cell or lint: every `ref_time_ticks()` /
    `get_time_usec()` occurrence under `dasllama/` whose line carries neither a print of the
    elapsed interval nor the literal `// clock: control` is a finding; the four marked sites
    (`dasllama_prefix.das` x2, `dasllama_image.das`, plus the harness ramp probe) are the
    fixture.

47. **`REVIEW.das` run with a cwd outside the repo root reports OK with zero findings even
    when its checks should fire** (observed 2026-08-24 from a detached git worktree: the
    root-guard `fexist` passes there, the folder walk silently returns nothing). A gate that
    looks green off-root is a hazard for worktree-based auditors. Done = the walk either
    resolves against the repo root explicitly or the run REFUSES (exit 2) when the walk
    yields zero files - an empty scan is never a pass.

48. **The cross-module template-base reifier defect is unrecorded outside one code comment.**
    `class template X : Base` with `Base` in another module fails reification -
    `error[30915]: can't initialize field __finalize` (the stamped instance keeps the base
    module's `self` type; `daslib/typemacro_boost.das`'s `__finalize` rewrite only covers the
    same-module autoinfer-cast shape). `MetalAttnAVMmSgT` (dasllama_metal_prefill.das)
    hand-inlines `MetalMmTileBase`'s tiles + `acc_quad` because of it - a hot primitive with
    a second unsynced copy. Done = fix the reifier to re-point `__finalize` at the stamped
    instance type for a cross-module parent (25-line repro: base class in module A,
    `[template_struct_instance]` template extending it in module B; the same-module control
    compiles), then collapse the inline copy back onto the base; until then any drift between
    the copies is a review hazard.

49. **The MoE kq tensor twins (K4/K5/K6) are gated by the DENSE kq crowns - a race that never
    measured them.** `pf_compile_moe_kq_twins` keys on `metal_tensor_crowned("kq_mulmm_k4")`
    etc. (dense 512x2048x1024 race), but the kernels it arms add the per-expert plane fold,
    cnt/basep indirection and the tg-uniform early exit; the q8/mx4 MoE twins already have
    their own MoE-shaped race families (`race_moe_mulmm_q8` / `_mx4`, ne=4, 32 rows/expert).
    On a box where the dense K4 twin wins at 512 rows while the MoE K4 twin loses at
    32 rows/expert, every routed K4 expert site takes the slower kernel for the life of the
    sidecar, and the sidecar cannot express the split. Done = own race families
    (`moe_mulmm_k4/k5/k6`) beside the q8/mx4 racers, PSO gates moved onto their crowns.

50. **`float_a_ok` licenses the whole kernel, so the float-A gate is disarmed on twin-stamped
    templates.** `ctx.float_a_ok` is one per-emit bool; a `[metal_kernel(float_a_ok=true)]`
    on a template method covers every stamp - including the `XT = float16` twins the gate
    exists to protect (a half family regressing to an f32 panel compiles clean). Nine of the
    ten production license sites sit on such templates, and the q8b form (no half twin by
    construction) FORCES the blanket switch onto any kernel that carries it. Done = make the
    license a property of the call (a call-site argument the emitter reads beside m/n/sgs, or
    a distinct builtin spelling for deliberate f32 forms) and exempt q8b by construction;
    test_metal_float_a_gate's arms then split per-stamp.

51. **`pool_release` cannot enforce its own postcondition - the handle survives the release.**
    The reviewer's point stands: after a release the caller still holds the pointer, and a
    double-release pushes a live buffer into the free list twice (two later frees of one
    buffer; the new null-guard cannot see it). The suggested in-place fix is a das no-op -
    `var buf : MetalBuffer?` is a by-value pointer copy, so nulling it never reaches the
    caller (the LINT023 class). Done = the signature becomes `var buf : MetalBuffer?&` and
    the body nulls after the push, with every call site audited for lvalue-ness in the same
    change - an API pass over das_metal_boost, not a babysit patch.

52. **`build_image`'s staging plane-group arm deletes nested fields unguarded** - the
    `image_planes` arm frees with `if (consume)` alone while its top-level and `image_map`
    siblings guard on `lock_count(field) == 0`. Unreachable today: `AudioTowerPlanes` embeds
    only in the mint-side staging structs (`Qwen3aStaging.core`, `WhisperStaging.enc`),
    which are always fresh-built and never parse targets, so no borrowed view can reach the
    arm. Done = the arm carries the same `lock_count` guard as its siblings, added when that
    arm is next touched for real work.

53. **RESOLVED (2026-08-28) - the MoE tensor twins ride one scaffold template.**
    `template_struct_instance` learned template CHAINS (a template deriving a template; the
    instance reifies the whole chain in one pass), and `MetalMoeMulMmKqTensorBase` now
    carries the shared shell with k45/k6/q51/mx4 deriving - kernel, PSO and global names all
    unchanged, the MSL audit read statement-identical (scoping, a folded pointer alias, and
    mx4's x/y renumber to the scaffold's 3/4). q8 stays its own template: its body IS the
    tuned `tmm2d_q8u_f32` helper, a different mechanism, not a copy. Original entry:
    five hand-split copies of one scaffold -
    `MetalMoeMulMmQ8/K45/K6/Mx4/Q51TensorT` share the expert prologue, the `while (work < 256u)`
    staging shell, the barrier pair, and the `tmm2d_tg_begin/step/store` epilogue verbatim,
    diverging only on the weight-format decode block and its buffer views (`REVIEW_GPU.md`:
    "kernel twins stamp one `class template`, whatever the stamp axis is"; the q5_1 twin made
    the count five). Done = one base class template
    carrying the scaffold with the decode behind an abstract method spliced flat at emission -
    the `MetalMoeMulMmBase` pattern - with every format's bit-exact gate green and the MSL of
    the pre-existing four stamps unchanged.

54. **The hardware probe stamps `remote_desktop` on daemon presence, not session activity.**
    The stored-run ban means an ACTIVE remote-desktop session (its encoder load taints the
    measurement); a dormant autostart daemon is harmless, yet the probe stamps its name,
    which is why 47 historical m1/zen2 rows read `parsec` and were hand-corrected to `off`
    (dormant by the box discipline). Done = the probe stamps `off` unless a session is live
    (a connected-client check per product), with the value naming the product only then.

55. **The kernel-coverage census has no tower carrier.** `tests/test_kernel_coverage.das`'s
    zoo is text-decoder-only, so the tower/ASR kernel classes (the tower flash, win-attn,
    kv_hc, row_gather, the g4a/cn/q3a chain families, the q3v stem) ride
    `CENSUS_NEVER_DISPATCHED` with reasons instead of census rows. Done = a tower carrier in
    the zoo dispatches them and the never-dispatched entries move onto real rows.

56. **Four servable capabilities have no board cell.** The canary Q8_0 decoder lane, the
    canary Metal FastConformer tower, the gemma4a/qwen3a chat-decoder Metal rail, and the
    qwen25v Metal tower shipped cell-less; `gen_bench_records.das`'s
    `is_metal_served_family` still keys on Whisper alone, and `records/m1.json` /
    `records/m4.json` still carry canary rows pinning the retired
    `canary-qwen-2.5b-decoder-f16.gguf`. Done = metal-ASR rows spawn for the four (minted at
    the arc-end re-mint on the b10659 ref pin, fresh tune manifest), and the stale m1/m4
    canary rows re-mint or retire.

57. **RESOLVED (2026-08-30, verified no-work) - the Plane `length` overloads already return
    `int64`.** `dasllama_plane.das` sec.108-112: `def length(pl : PlaneF) : int64 => pl.n`
    (all five Plane types), so `uint64(length(t.blob) * 4l)` is int64 arithmetic end to end
    and no plane caps at 2^31. The ledger entry's premise ("return `int`") was wrong when
    written; the das-array staging twins cannot exceed 2^31 elements by the array limit.

58. **Per-entry kernel-identity hashes in llvm_tune - the fine invalidation the release
    split trades away.** With tune sidecars scoped to `DASLLAMA_RELEASE` (bumped only on a
    declared release), a kernel changed mid-release keeps its stale sidecar winner - accepted
    as perf-only drift (crowns pick among envelope-verified twins; races re-verify at mint).
    The principled hardening: each sidecar kernel entry carries its kernel's identity hash
    (the per-function AOT semantic hash the JIT dll cache already folds - in-tree precedent),
    and a mismatching entry alone re-races or falls back while the rest of the sidecar stays
    valid. A generic llvm_tune feature, useful beyond dasLLAMA. Done = entry-level staleness
    by kernel hash, the release scope untouched, and the refusal-reason rail naming
    "kernel-hash" beside the existing reasons.

59. **daslang's AST serializer could restore function-typed globals by name - retiring the
    boot-restore class outright.** A serialized exe restores globals as data, so function
    values arrive null; today every module carrying `var g_x = @@fn` declaration defaults
    owes a boot-restore `[init]` (the REVIEW.das restore check + test_exe_smoke guard the
    dasLLAMA tree, minted after the vulkan decode-mirror claim shipped crashing every exe
    forward). The root fix is daslang-core: serialize a function value as its mangled name
    and re-resolve at restore - the whole hazard class disappears language-wide, and the
    restore inits + the gate collapse to history. das-core scope, its own arc.

60. **Crowned-lane strict parity: decide whether the tensor twins owe token-exactness, or
    the strict gates own the simdgroup lane only.** The release re-mint armed the box's
    crowns in the test suites for the first time in a while (the pre-remint sidecar was
    stale, so the crown-consuming gates had been silently measuring fallback), and two
    strict cells crossed their bars under crowned kernels: whisper's cross-lane
    token-identity (attn_qkmm - 10/25 flips on jfk) and gemma4uv's tier-1 rung (mulmm_bf16 -
    6e-05 past a scale-relative bar on one canvas). Both cells now pin the simdgroup lane
    via `pin_metal_tensor_crowns` (dasllama_common - a latch `apply_box_profile_runtime`
    respects, since every load re-applies profile crowns over a bare setter); the crowned
    lane's serving quality stays covered by the board's parity pregate (word-level). The
    open question: should the tensor twins' race envelope bar be tightened until the strict
    gates pass crowned, or is per-lane gating the honest end state? Also ride-along: the
    trio's crown flags cache at prefill init, so a crown pin needs
    tower+wdec+prefill shutdowns - a single `metal_crown_flags_reset()` seam would retire
    that triple-shutdown incantation.

60b. **Per-variant dispatch witnesses for the crowned word-level cell.** The cell's engage
    witnesses prove tower + wdec service, not WHICH variant served: a garble control on the
    tensor QK body did not move whisper-tiny's transcripts, so tiny's hs=64 attention likely
    never takes that route and the cell gates the crowned PIPELINE, not each twin.
    `metal_kernel_coverage()` rows (the kcov census) are the per-variant witness - assert
    the expected crowned entries' counts moved, once the cell picks a carrier whose shapes
    actually ready the tensor variants.

61. **An in-process image-off seam for facade-knob test cells.** A cell whose subject is a
    facade lane knob (`load_asr_model` under `set_asr_tower_fp32`) must keep the facade
    loader, and on a cold box that load BAKES the pinned lane's `.dlim` and GC-purges the
    serving lane's flavor (`dlim_gc_stale`) - the class that broke `test_audio_embedder`
    once already. The tests checklist ledgers the residue; the fix is an in-process
    equivalent of `DASLLAMA_IMAGE=0` (`g_env_engine.image` is a `let` read at load), so such
    cells can run image-free instead of risking the purge.

57. **Plane types have no `long_length`.** `length(PlaneF)` / `length(PlaneU16)` return
    `int`, so every `uint64(length(t.blob) * 4l)` spelling caps a plane at 2^31 elements
    before the widening - headroom-only today (whisper large-v3's twin is ~632M elements).
    Done = `long_length` overloads in `dasllama_plane` and the buffer-sizing call sites
    moved onto them.

58. **The M5 pass on the IQ4_XS Metal kernels (Boris, 2026-08-30: "we'll ledger M5 pass on
    new kernels for later").** The format's Metal set is correctness-first: `MetalKqGemvIq4xs`,
    the `MetalKqMvIq4xsT` B2/B4 pair, `MetalKqMvB8Iq4xs` and the `IQ4XS` arm of
    `MetalKqMulMmK45T` copy the k4/k6 lane maps with a per-element `iq4_lut` (four packed
    words, select + shift + sign trick) and no measurement behind them; the prefill site takes
    the base mul_mm only - no tensor (`_t`), tall (`_th128`), double-buffered (`_thdb`) or dev-W
    dequant twins, and no MoE GEMV / mul_mm trio for the format. Done = the twins stamped on
    the existing templates, the LUT cost measured against a `constant` table and against a
    byte-pair decode on the M5, and `bench_metal_gemv_kernels` / `bench_metal_kq_mm_lab` rows
    for the format beside k4's. The same gap now covers every format this arc added: `k3`,
    `iq3s`, `iq3xxs`, `iq4nl`, `k2`, `iq2s`, `iq2xs` and `iq2xxs` take the base `mul_mm` at the
    prefill site - no tensor (`_t`), tall (`_th128`), double-buffered (`_thdb`) or dev-W dequant
    twins - and `pf_devw_panel_kq` declines all nine formats outright. Done = the twins stamped
    on the existing templates for whichever formats measure worth it, and that decline list
    shortened to match.

59. **DONE (2026-08-31, the unquirk pass): `--tune-only <family>` re-mints one family into the
    existing sidecar, and the shipped defaults profiles adopt-then-race only the residue.**
    Original ask - partial mint (Boris, 2026-08-30, for after the formats arc): "takes existing mint and
    mints new kernels only. at least in debug-only mode." Today a sidecar missing ANY demanded
    kernel re-tunes the whole scope (the completeness rule), so every new `[tune]` family - one
    per format in this arc - re-mints every application sidecar on the box on its next start,
    minutes each (HOW_TO_ADD_A_FORMAT.md QUIRK 17), and the same rule keeps a stub-era
    `"reference"` pin alive after the emitter lands (QUIRK 11). The partial mint keeps the
    existing entries and races only the families the sidecar lacks (or names as reference),
    validating the merged file as one. Debug-only is the acceptable first form: a
    `DAS_TUNE_PARTIAL=1` (or `--tune-partial`) rail that the auto policy does not take, so a
    shipped box still mints whole. Done = the rail exists, a fresh family on a minted box
    costs one family's race, and the tuner's status line names the partial mint as such.

60. **Retroactive llama.cpp CPU-kernel audit for the arc's earlier formats (Boris,
    2026-08-30: "lets make sure we do for all new CPU kernels, and if we skipped for previous
    ones - lets ledger towards the end of this arc").** The IQ3_S CPU arm is the first written
    against llama.cpp's arch kernel (signs-on-activation, sllv index compose, scalar grid
    loads beating hardware gather); IQ4_XS and Q3_K were written from the disk format alone.
    Toward the end of the arc: read the reference exe's vec-dot and the reference exe's vec-dot
    (x86 + arm), list every technique ours does not measure, land the missing ones as
    `[tune_perm]` spellings and let the probe judge. Done = a per-format note naming what was
    raced and what won, beside the existing bench rows.

61. **Grid-format CPU decode: the panel round trip, one shared cost (kernel-level 0.43x-0.91x).**
    Measured 2026-09-01 at one thread, m=4096 k=14336 (`benchmarks/matmul/kq_kernel_bench.das` vs
    the reference exe's `test-backend-ops perf`): iq3s 11406 us vs 10340 (0.91x), iq3xxs 11578 vs
    6590 (0.57x), iq2s 11732 vs 5074 (0.43x), iq2xs 11490 vs 5386 (0.47x), iq2xxs 11061 vs 5124
    (0.46x) - all five at 48-51 ns per superblock against k4/k2's 8.6-8.8, a flat cost independent
    of the format. Every one of the five gathers ends each decoded dword with a 4-byte store into the
    alloca panel and the dot reloads 32 bytes spanning eight such stores - a wide load over narrow
    stores, which x86 does not forward; llama.cpp composes grid words into registers on both arches
    (`plans/kernel_parity_research_cpu.md`). Signs-on-activation does NOT port to our 8-rows-per-vector
    layout (llama.cpp's own arm64 arms sign the weights for the same reason). The spellings, in
    order: `gather="reg"` (group-major compose, insertelement per row, no panel), `psign="mask"` (a
    vector sign mask replacing GPR sign math + abs), a `vpdpbssd` seat for AVX-VNNI-INT8 hosts, a
    repack-baked parity sign byte. Done = each of the five at or past the reference row at one thread,
    crowned by the probe; plan and fact base: `plans/kernel_parity_pass.md`.
    2026-09-01: `gather="reg"` measured 1.85x SLOWER (insertelement chains, 5x the code) and was
    dropped; `sign="vec"` landed for iq3s (7732 us = 1.34x the reference) and iq2s (7076 us =
    0.72x). iq3xxs/iq2xs/iq2xxs join once the repack bakes their sign bytes into the plane.

62. **IQ3_S Metal decode: the ~140 GB/s compose ceiling (tg 0.95x).** Eight GEMV forms raced
    at n=2048 d=8192 - gather placement x3, gather deleted, signs deleted, llama.cpp's exact
    1-lane-per-block geometry, row width, f4 magnitude slab - all land in 127-141 GB/s while
    k4 does 204 and k6 287 in the same harness; the 9-bit-index + per-nibble-sign compose is
    the cost, not any one instruction class (deleting the gather OR the signs moves it under
    10%). The f4 slab ships (+9%, tg128 0.82x -> 0.95x same-run). Candidates beyond kernel
    shape: emitted-MSL diffs vs llama.cpp's compiled kernel (half math, function constants),
    or fusing the sign flip into the staged slab per SITE via a second indexed table. Done =
    a form that clears 180 GB/s in the dispatch-loop probe (QUIRK 22's harness), or a note
    proving the ceiling is shared by llama.cpp's own kernel when isolated the same way.
