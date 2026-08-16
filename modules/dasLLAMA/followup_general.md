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
   (different algorithm). 2026-08-09: adoption closed the A/B — the K4T/K6T `TILED=false`
   stamps and K5T's single spelling ARE the previewed lab forms, so the three prod-vs-lab B2
   pairs compiled byte-identical and were deleted (the measured ruling above stands as the
   record; recreate the arm pattern if the question reopens). The tripwire stands: the lab's
   surviving production arms reference the stamped globals.

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

8. **MoE mul_mm family: COMPLETE — all six formats ride MetalMoeMulMmBase.** K6 joined
   2026-08-08 (021fffd87, stateless stage_a, −2.0%). Q8 and Mx4 joined the same day via
   msl_emit's flatten/scope-splice arc (plans/msl-flatten.md): Q8 keeps the carried-pointer
   walk (plain `scur`/`qp` members bound in `stage_init`, advanced in stage_a — the
   stateless index form measured OUT +3.4–3.6%), −0.5..−0.9% vs its deleted standalone;
   Mx4 rides `stage_init` (vtab staging) + `stage_acc` (per-expert bias seed into the
   accumulator array), +0.02% flat vs its standalone (DRAM-bound — the join is free). Both
   binding contracts moved to the family numbers (xf@3 y@4 cnt@7 basep@8 bkt@9;
   `kn_moe_mm_family_tail` spells the tail once; tensor twins keep their compact layouts).
   Side find from the gmm8 dump: the `addr()` escape for a pointer walk defeats the const
   analysis — Q8's weight buffers emit as non-const `device half*` (and win anyway, so
   constness is not the term).
   The per-site rot from the kargs migration (c45724dae) was REPAIRED in the lens-arc review
   round (2026-08-09): production arms bind MoeGemvArgs kargs, the pre-kargs lab twins keep
   their historical layouts behind per-arm bind splits, the dead tail-duplicate
   run_gmm4/gmm6_lab calls are gone, and main runs end-to-end to the leak assert — all 19
   correctness checks at rel 0, run_gmx4_lab reachable again (prod 300.6 wGB/s vs lcppe
   302.1, prod = lcppe + an untaken bias branch). Don't trust rounds=1 numbers from this
   lab — warm-up dominates.

9. **Prefill compiles its own PSO for kernels decode already has — HALF RESOLVED by the lens
   arc.** The qk_norm half closed in P1: `MetalQkNorm` gained `@off` on x, prefill rides
   decode's generated builder, and `enc_qk_norm_pf` + `g_pf_pso_qknorm` are gone. The rope
   half remains in the new shape: both stages' rope encoders are lens-generated now, but
   prefill's `MetalRope` instance names `g_pf_pso_rope` compiled from the same
   `metal_rope_msl` source decode compiles into its own PSO. The ownership rule is written
   (REVIEW.md: the class-owning file compiles/releases; instances may share a pso handle —
   the MetalRmsNorm ×3 / tensor-twin precedent), so Done = prefill's rope instance either
   names decode's pso global the shared-handle way or a comment says why stage-local is
   intended.

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
    structural: nothing dispatches a kernel except its `enc_*` builder (REVIEW: "Nothing dispatches a kernel except its enc_* builder").

14. **`dasllama_blocks` / `dasllama_moe` / `dasllama_ple` have no direct tests.** All three
   are pointer-dispatched forward-loop internals moved out of `dasllama_common` during the reorg;
   today their only coverage is the forward/family matrix — exactly the "the model still runs"
   posture REVIEW's Tests section calls out. Their siblings from the same wave have direct
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

17. **Vision stills — DELIVERED for the first family (the vision arc, 2026-08-14).** gemma-4
    dense via `gemma4uv` shipped end to end: preprocessing, non-causal span, chat/scheduler/
    server splice, the `.dlim` rail, `ask --image`, and the `lcpp_bench --image` cell. What
    remains of this item folded into #23 (uniform-bound GPU prefill kernels) and the v2 ledger
    in `vision_plan.md` (the E-series `gemma4v` ViT, multi-image, remote fetch, video).
18. **Tutorials + documentation resync sweep (Boris 2026-08-05).** The ASR/audio tutorials and
   module documentation have desynced from the API as the q8/KV/lever work landed — a full
   pass over `tutorials/` (`.das` and `.rst`) and the dasLLAMA doc pages against the current
   facade surface (`set_asr_kv`, `--kv`, `--fp32-tower`, exec_fmt spellings, the q8 defaults).
   Going forward the new Documentation section in REVIEW.md makes this a per-change check;
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

21. **`harness/tq4_probe.das` helpers have no tests.** `rotate_row` / `unrotate_row` /
    `row_dot` were rewritten to zip loops in the lint sweep (PR #3733) with no coverage
    before or after - feed each a known row, check the bytes. Note the zip form stops at
    the shorter operand where the old indexed form panicked on mismatched lengths;
    current callers pass equal lengths, so the test should pin the equal-length results,
    not the mismatch behavior.

22. **Tune-sidecar rejection under `--gpu metal` — RESOLVED (2026-08-15): the staleness rail
    working correctly behind an uninformative message.** Forensics: the refused
    `main.tune.json` was an Aug-10 mint whose mtime predated the Aug-13 rebuilt binary
    (stale reads as absent), while `ask.tune.json` was minted fresh by that binary — and
    `--gpu metal` was a pure confound: the demand set is arm-invariant (one compiled
    program; verified by an identical per-kernel stamp-line diff across cpu/metal server
    arms). Neither reading (a) nor (b) held. Fixed in llvm_tune: every untuned refusal now
    names its reason (stale-binary with both dates / foreign-box / version / missing
    entries / absent / unreadable), `DAS_TUNE_MANIFEST` at an untuned-reading file warns
    loudly instead of silently stamping fallbacks, and `[tune_scope] version_of=` pins the
    scope to `DASLLAMA_VERSION` so a copy-restored old sidecar (fresh mtime, old winners —
    the trap the forensics surfaced) refuses by name. Metal-arm measurement is unblocked;
    bringup owes the standing fresh quiet-box mint, which also refreshes
    `performance/last_known_good_sidecar.json` and the `performance/<box>.tune.json` rig
    sidecars — the checked-in and on-box copies predate the version pin, so until then they
    serve fallbacks with the named warning.

23. **Uniform-bound non-causal prefill kernels for Metal and Vulkan.** The image span
    currently serves on the CPU loop by decline: `attn_gpu_prefill_ready`,
    `prefill_decline` (`MetalPrefillDecline.non_causal_span`) and `vulkan_resident_prefill`
    all refuse `s.attn_uniform_end != 0`, and metal-blob models refuse a vision arm outright
    at create (no CPU weights to fall back on). Done = the GPU prefill chains take the
    uniform bound, the three declines and the blob refusals are deleted, and the kernel
    coverage suite dispatches the non-causal arm. Scheduled for the Metal arc's tail —
    pulled forward only if CPU-fallback vision proves a blocker (Boris 2026-08-14).

24. **Stream the over-line audio-carrier mints (the 1 GiB staging rule, 2026-08-14).**
    `REVIEW.md` caps `cache_via_image_staged` at a 1 GiB source file; six carriers on the
    fleet stage over it today — mmproj-omni-3b-f32 (5.0 GB), canary-qwen-2.5b-encoder-f32
    (3.1 GB), mmproj-ultravox-1b-f32 (2.6 GB), voxtral-mini-mmproj-f32 (2.5 GB),
    qwen2audio-mmproj-f32 (2.4 GB), mmproj-Qwen3-Omni-30B bf16 (2.1 GB) — each paying a
    staged peak of roughly source + built image. Done = the audio-family mints size the
    image up front and write planes as produced (the decoder rail's shape), the staged
    entry points refuse a source over the line, and the mint-form-differs conversions
    restructure into per-plane producers. The vision carriers stay staged (gemma-4 12B
    mmproj 0.2 GB, E2B 0.9 GB).

25. **OpenAI-standard audio/image API surface — conformance, decisions, hygiene (2026-08-16).**
    The server implements `image_url`/`input_image` chat parts (data:-only) and
    `/v1/audio/transcriptions` (`json`/`text`/`verbose_json`, `language`, VAD segments).
    The rest of the standard surface, tiered:
    - CONFORMANCE DEBT (plumbing, no new capability): `input_audio` chat parts plus the
      ecosystem `audio_url` twin (accept both — audio-in-chat rides the existing
      `render_prompt_media` splice); `srt`/`vtt` transcription response formats;
      `timestamp_granularities[]` where the family has word timestamps; `prompt` biasing;
      `stream=true` transcription events; `/v1/audio/translations` for the whisper family
      (native decoder mode; other families decline); accept-and-ignore `image_url.detail`.
    - CAPABILITY DECISIONS (Boris's call, each a new model class): TTS — `/v1/audio/speech`
      and chat `modalities:["text","audio"]`. No served artifact can speak; the two Omni
      families have Talkers upstream but the GGUF ecosystem carries only thinker + audio
      encoder, and a talker conditions on thinker HIDDEN STATES (not a bolt-on). The
      reference-backed route if wanted: a dedicated small TTS family (llama.cpp's tts
      example — OuteTTS + WavTokenizer ggufs). Realtime API (WebSocket voice, barge-in) is
      the end-state the smaller audio choices point at; name it before choosing them.
    - DECLINE/PARK: `/v1/images/generations` (+edits/variations) — no roster model
      generates images even upstream; diffusion is a disjoint class (DiT/UNet + conv2d VAE,
      no reference in llama.cpp; the GGML reference is stable-diffusion.cpp, which shares
      our quants/GGUF/GEMMs but not the graph). `file`/`video_url` parts likewise parked.
    - HYGIENE: our `/v1/images` is the dlim-inventory/bake endpoint — a name squat on the
      standard image-API prefix; rename ours or accept the squat deliberately.
