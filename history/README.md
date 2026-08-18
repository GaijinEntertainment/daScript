# history/

Design docs, plans, audits, findings, and session notes from **completed arcs** — kept for
the record, no longer load-bearing. A doc belongs here when its remaining value is the
record ("why is it built this way"), not operation. Living contracts and durable records
stay next to their code (e.g. `modules/dasSQLITE/PROVIDER_CONTRACT.md`, the `MASTERPLAN.md`
files, `modules/dasLLAMA/*_plan.md` specs still cited from code, `results.md` ledgers).
Process and criteria: `skills/internal/doc_archiving.md`.

- `compiler/` — shipped compiler-rework docs (fixed-array structural types, annotation-info
  rtti rework, quote lowering)
- `ci/` — process/coverage analyses (the local-vs-CI coverage gap plan; its stage 4 lives
  on as the nightly daspkg-index cron)
- `dasLLAMA/` — closed dasLLAMA arc docs (superseded hand-written AVX tier matrix, EPYC
  fleet measurements, the model-expansion wave plan; the 2026-07-29 doc-reorg sweep added
  the API_REWORK plan half — its living ledger split to `modules/dasLLAMA/PERF_LEDGER.md` —
  plus the public-bench, GEMM-generator, x64, KV-codec, lens and audio-wave plan/research
  docs; the vulkan class-kernel arc added its plan + the per-family walkthrough record);
  `dasLLAMA/benchmarks/` — retired model-speed bench scripts, all superseded by the
  one bench (`modules/dasLLAMA/benchmarks/lcpp_bench.das`, 2026-07-25 unification)
- `dasSQLITE/` — the typed-SQL API rework corpus (`API_*.md`, `TUTORIALS.md`, the
  `tutorial-mockup/` design artifacts); the living contract stays at
  `modules/dasSQLITE/PROVIDER_CONTRACT.md`
- `dasSpirv/` — the completed PHASE6–9 working plans (content captured in
  `modules/dasSpirv/MASTERPLAN.md`)
- `dasVulkan/` — the original boost-layer design plan, carried over when the module moved
  in-tree; the living docs are `modules/dasVulkan/CLAUDE.md` and its `ROADMAP.md`
- `linq_fold/` — the linq_fold arc's plans and audits; the living masterplan is
  `daslib/linq_fold.md`, results stay at `benchmarks/sql/results.md`
- `examples/` — plans, findings, and research notes behind shipped examples
- `tests/` — one-off test-migration audits

Source comments referring to these docs by bare name (e.g. `FIXED_ARRAY_REWORK.md`) resolve
here via search; path-qualified references were updated at move time. Every archived doc is
listed in the log below — search it first when hunting for a doc.

## Archive log

- 2026-07-24 `FIXED_ARRAY_REWORK.md` → `history/compiler/FIXED_ARRAY_REWORK.md` — the tFixedArray structural-type rework plan (shipped 0.6.3)
- 2026-07-24 `ANNOTATION_INFO_REWORK.md` → `history/compiler/ANNOTATION_INFO_REWORK.md` — rtti self-contained annotation-info rework
- 2026-07-24 `QUOTE_LOWERING.md` → `history/compiler/QUOTE_LOWERING.md` — quote/qmacro lowering under AOT + JIT
- 2026-07-24 `COVERAGE_GAP.md` → `history/ci/COVERAGE_GAP.md` — local-vs-CI coverage gap analysis (stage 4 lives on as the nightly daspkg-index cron)
- 2026-07-24 `examples/flatten/scalar_packing/PLAN.md` → `history/examples/scalar_packing/PLAN.md` — scalar-packing loop re-roll design
- 2026-07-24 `examples/flatten/scalar_packing/FINDINGS.md` → `history/examples/scalar_packing/FINDINGS.md` — scalar-packing re-roll findings
- 2026-07-24 `examples/daStrudel/sfx_lab/PLAN.md` → `history/examples/sfx_lab/PLAN.md` — SFX-lab implementation plan
- 2026-07-24 `examples/daStrudel/sfx_lab/NOTES_drum_session.md` → `history/examples/sfx_lab/NOTES_drum_session.md` — SFX-lab drum-session notes
- 2026-07-24 `examples/daStrudel/sfx_lab/research/metric_design.md` → `history/examples/sfx_lab/research/metric_design.md` — SFX-lab fit-metric design
- 2026-07-24 `examples/daStrudel/sfx_lab/research/seeding_from_analysis.md` → `history/examples/sfx_lab/research/seeding_from_analysis.md` — SFX-lab analysis-seeding research
- 2026-07-24 `examples/games/sequence/PLAN.md` → `history/examples/sequence/PLAN.md` — sequence board-game live-coding plan
- 2026-07-24 `examples/telegram/dictation/CADMUS_PLAN.md` → `history/examples/dictation/CADMUS_PLAN.md` — Cadmus assistant product decisions
- 2026-07-24 `tests/REVIEW.md` → `history/tests/REVIEW.md` — daScriptTest-migration review checklist
- 2026-07-24 `modules/dasSQLITE/API_REWORK.md` → `history/dasSQLITE/API_REWORK.md` — typed-SQL API rework design record
- 2026-07-24 `modules/dasSQLITE/API_MISSING.md` → `history/dasSQLITE/API_MISSING.md` — typed-SQL deferred-feature list
- 2026-07-24 `modules/dasSQLITE/API_CHECKED.md` → `history/dasSQLITE/API_CHECKED.md` — typed-SQL equivalence-harness plan
- 2026-07-24 `modules/dasSQLITE/API_MIGRATION.md` → `history/dasSQLITE/API_MIGRATION.md` — typed-SQL migration design
- 2026-07-24 `modules/dasSQLITE/TUTORIALS.md` → `history/dasSQLITE/TUTORIALS.md` — typed-SQL tutorial coverage plan
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/01-version.das.mockup` → `history/dasSQLITE/tutorial-mockup/01-version.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/02-insert_data.das.mockup` → `history/dasSQLITE/tutorial-mockup/02-insert_data.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/03-last_row_id.das.mockup` → `history/dasSQLITE/tutorial-mockup/03-last_row_id.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/04-select_all.das.mockup` → `history/dasSQLITE/tutorial-mockup/04-select_all.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/05-parametrized.das.mockup` → `history/dasSQLITE/tutorial-mockup/05-parametrized.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/14-transaction.das.mockup` → `history/dasSQLITE/tutorial-mockup/14-transaction.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/15-update.das.mockup` → `history/dasSQLITE/tutorial-mockup/15-update.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/16-delete.das.mockup` → `history/dasSQLITE/tutorial-mockup/16-delete.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/17-upsert.das.mockup` → `history/dasSQLITE/tutorial-mockup/17-upsert.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/19-group_by.das.mockup` → `history/dasSQLITE/tutorial-mockup/19-group_by.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/23-joins.das.mockup` → `history/dasSQLITE/tutorial-mockup/23-joins.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/24-subqueries.das.mockup` → `history/dasSQLITE/tutorial-mockup/24-subqueries.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/25-null_handling.das.mockup` → `history/dasSQLITE/tutorial-mockup/25-null_handling.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/26-foreign_keys.das.mockup` → `history/dasSQLITE/tutorial-mockup/26-foreign_keys.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/27-indexes.das.mockup` → `history/dasSQLITE/tutorial-mockup/27-indexes.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/28-defaults_computed.das.mockup` → `history/dasSQLITE/tutorial-mockup/28-defaults_computed.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/29-custom_types.das.mockup` → `history/dasSQLITE/tutorial-mockup/29-custom_types.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/30-migrations.das.mockup` → `history/dasSQLITE/tutorial-mockup/30-migrations.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/31-views.das.mockup` → `history/dasSQLITE/tutorial-mockup/31-views.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/32-sql_functions.das.mockup` → `history/dasSQLITE/tutorial-mockup/32-sql_functions.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/33-pragma.das.mockup` → `history/dasSQLITE/tutorial-mockup/33-pragma.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/34-backup_vacuum.das.mockup` → `history/dasSQLITE/tutorial-mockup/34-backup_vacuum.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/36-attach.das.mockup` → `history/dasSQLITE/tutorial-mockup/36-attach.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/37-json.das.mockup` → `history/dasSQLITE/tutorial-mockup/37-json.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/39-fts5.das.mockup` → `history/dasSQLITE/tutorial-mockup/39-fts5.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSQLITE/tutorial-mockup/_error_handling.das.mockup` → `history/dasSQLITE/tutorial-mockup/_error_handling.das.mockup` — pre-implementation tutorial design artifact
- 2026-07-24 `modules/dasSpirv/PHASE6_VISITOR_PORT.md` → `history/dasSpirv/PHASE6_VISITOR_PORT.md` — completed phase working plan (content captured in MASTERPLAN.md)
- 2026-07-24 `modules/dasSpirv/PHASE7_TEXTURES.md` → `history/dasSpirv/PHASE7_TEXTURES.md` — completed phase working plan (content captured in MASTERPLAN.md)
- 2026-07-24 `modules/dasSpirv/PHASE8_LANGUAGE.md` → `history/dasSpirv/PHASE8_LANGUAGE.md` — completed phase working plan (content captured in MASTERPLAN.md)
- 2026-07-24 `modules/dasSpirv/PHASE9_TUTORIALS.md` → `history/dasSpirv/PHASE9_TUTORIALS.md` — completed phase working plan (content captured in MASTERPLAN.md)
- 2026-07-24 `modules/dasLLAMA/avx_kernel_matrix.md` → `history/dasLLAMA/avx_kernel_matrix.md` — hand-written AVX tier matrix (superseded by the generated `[tune]` GEMM family)
- 2026-07-24 `modules/dasLLAMA/epyc9654_measurements.md` → `history/dasLLAMA/epyc9654_measurements.md` — EPYC 9654 campaign measurements
- 2026-07-24 `modules/dasLLAMA/model_expansion_plan.md` → `history/dasLLAMA/model_expansion_plan.md` — model-expansion wave plan (waves shipped 0.6.4)
- 2026-07-24 `benchmarks/sql/LINQ.md` → `history/linq_fold/LINQ.md` — linq_fold arc plan (shipped 0.6.3)
- 2026-07-24 `benchmarks/sql/LINQ_TO_DECS.md` → `history/linq_fold/LINQ_TO_DECS.md` — linq-to-DECS plan (shipped 0.6.3)
- 2026-07-24 `benchmarks/sql/LINQ_TO_TABLE.md` → `history/linq_fold/LINQ_TO_TABLE.md` — linq-to-table plan (shipped 0.6.3)
- 2026-07-24 `benchmarks/sql/M4_DECS_EXPANSION.md` → `history/linq_fold/M4_DECS_EXPANSION.md` — M4 DECS expansion plan (shipped 0.6.3)
- 2026-07-24 `benchmarks/sql/linq_fold_chain_audit.md` → `history/linq_fold/linq_fold_chain_audit.md` — linq_fold chain audit (shipped 0.6.3)
- 2026-07-24 `benchmarks/sql/sqlite_linq_gaps.md` → `history/linq_fold/sqlite_linq_gaps.md` — closed SQLite-linq gap audit (shipped 0.6.3)
- 2026-07-25 `modules/dasLLAMA/benchmarks/perf.das` -> `history/dasLLAMA/benchmarks/perf.das` - CPU-era end-to-end perf bench (superseded by lcpp_bench)
- 2026-07-25 `modules/dasLLAMA/benchmarks/prefill_perf.das` -> `history/dasLLAMA/benchmarks/prefill_perf.das` - prefill-phase bench (superseded by lcpp_bench pp reps)
- 2026-07-25 `modules/dasLLAMA/benchmarks/llama3_perf.das` -> `history/dasLLAMA/benchmarks/llama3_perf.das` - decode-table bench (superseded by lcpp_bench tg reps)
- 2026-07-25 `modules/dasLLAMA/benchmarks/batch_decode_perf.das` -> `history/dasLLAMA/benchmarks/batch_decode_perf.das` - batch-decode speed bench (decode arc shipped)
- 2026-07-25 `modules/dasLLAMA/benchmarks/decode_real_bench.das` -> `history/dasLLAMA/benchmarks/decode_real_bench.das` - real-prompt decode bench (decode arc shipped)
- 2026-07-25 `modules/dasLLAMA/benchmarks/decode_prof.das` -> `history/dasLLAMA/benchmarks/decode_prof.das` - decode per-phase profiler driver (GPU_PROF env + lcpp_bench covers it)
- 2026-07-25 `modules/dasLLAMA/benchmarks/decode_metal_chase.das` -> `history/dasLLAMA/benchmarks/decode_metal_chase.das` - Metal decode chase instrument (arc shipped)
- 2026-07-25 `modules/dasLLAMA/benchmarks/prefill_metal_chase.das` -> `history/dasLLAMA/benchmarks/prefill_metal_chase.das` - Metal prefill chase instrument (arc shipped)
- 2026-07-25 `modules/dasLLAMA/benchmarks/bench_metal_framing.das` -> `history/dasLLAMA/benchmarks/bench_metal_framing.das` - Metal framing-overhead bench (arc shipped)
- 2026-07-25 `modules/dasLLAMA/benchmarks/gguf_perf.das` -> `history/dasLLAMA/benchmarks/gguf_perf.das` - gguf load-time bench (the .dlim rail made it moot)
- 2026-07-27 `modules/dasLLVM/TUNE_PROGRESS_PLAN.md` -> `history/compiler/TUNE_PROGRESS_UX.md` - tune progress-display design (events + single renderer; arc shipped, operative content in skills/tune.md)
- 2026-07-29 `modules/dasLLAMA/API_REWORK.md` → `history/dasLLAMA/API_REWORK.md` — API + backend-seam plan (Phases 1–7 shipped); the living perf ledger split out to `modules/dasLLAMA/PERF_LEDGER.md`
- 2026-07-29 `modules/dasLLAMA/PUBLIC_BENCH_PLAN.md` → `history/dasLLAMA/PUBLIC_BENCH_PLAN.md` — public-bench build plan (shipped); record schema now in `modules/dasLLAMA/METHODOLOGY.md`
- 2026-07-29 `modules/dasLLAMA/gemm_generator_plan.md` → `history/dasLLAMA/gemm_generator_plan.md` — [llvm_code] GEMM generator plan + M0–M4 measurement record (shipped; open items → PERF_LEDGER.md)
- 2026-07-29 `modules/dasLLAMA/x64_arch.md` → `history/dasLLAMA/x64_arch.md` — x64 port seam map (self-marked HISTORICAL; durable ground rules → modules/dasLLAMA/ARCHITECTURE.md)
- 2026-07-29 `modules/dasLLAMA/get_x64_going.md` → `history/dasLLAMA/get_x64_going.md` — x64 bring-up runbook (steps describe deleted hand backends; gotcha ledger transferred to skills + tune_for_this_box.md)
- 2026-07-29 `modules/dasLLAMA/kernel_access_lens_metal.md` → `history/dasLLAMA/kernel_access_lens_metal.md` — @role lens design postmortem (implemented #3570; macro lessons → skills/das_macros.md)
- 2026-07-29 `modules/dasLLAMA/kv_cache_compression.md` → `history/dasLLAMA/kv_cache_compression.md` — KV codec design (f16/q8_0/tq4 shipped; layout invariants + verdicts → modules/dasLLAMA/ARCHITECTURE.md)
- 2026-07-29 `modules/dasLLAMA/turboquant_research.md` → `history/dasLLAMA/turboquant_research.md` — tq4 research briefs (verdicts → modules/dasLLAMA/ARCHITECTURE.md)
- 2026-07-29 `modules/dasLLAMA/audio_models_plan.md` → `history/dasLLAMA/audio_models_plan.md` — audio-wave tracker, sessions A–F all shipped (findings transferred; open Mistral [INST] item → PERF_LEDGER.md)
- 2026-07-29 `modules/dasLLAMA/box_profile.epyc9654.json` → `history/dasLLAMA/box_profile.epyc9654.json` — EPYC 9654 tune sidecar, beside its measurements doc
- 2026-07-30 `modules/dasLLAMA/ARCHITECTURE.md` (extraction-ledger section) → `history/dasLLAMA/reorg_extraction_ledger.md` — the reorg's per-move record; ARCHITECTURE.md now carries placement rules only (no historical data, per its header note)
- 2026-08-02 `modules/dasLLAMA/audio_image_plan.md` → `history/dasLLAMA/audio_image_plan.md` — audio .dlim borrowed-plane/map/stream plan (Phases 1–3 shipped; whisper streamed mint landed, gemma4a/canary mints deferred by measurement — the sizing lives in PERF_LEDGER.md)
- 2026-08-02 `modules/dasLLAMA/performance/results_metal_3b.md` → `history/dasLLAMA/performance/results_metal_3b.md` — pre-board 3B metal profiling record (superseded by records/*.json + boards; "living doc" banner had no generator)
- 2026-08-02 `modules/dasLLAMA/performance/results_metal_gemma.md` → `history/dasLLAMA/performance/results_metal_gemma.md` — pre-board gemma metal profiling record (same supersession)
- 2026-08-02 `modules/dasLLAMA/performance/results_metal_qwen35.md` → `history/dasLLAMA/performance/results_metal_qwen35.md` — pre-board qwen3.5 metal profiling record (same supersession)
- 2026-08-02 `modules/dasLLAMA/performance/results_metal_qwen3_4b.md` → `history/dasLLAMA/performance/results_metal_qwen3_4b.md` — pre-board qwen3-4B metal profiling record (same supersession)
- 2026-08-02 `modules/dasLLAMA/performance/results_metal_qwen3moe.md` → `history/dasLLAMA/performance/results_metal_qwen3moe.md` — pre-board qwen3moe metal profiling record (same supersession)
- 2026-08-02 `modules/dasLLAMA/performance/baseline_metal_{3b,gemma,qwen35,qwen3_4b,qwen3moe}_m1.tsv` → `history/dasLLAMA/performance/` — hand-authored companions of the results_metal set, moved with their docs
- 2026-08-06 `plans/vulkan_class_kernels.md` → `history/dasLLAMA/vulkan_class_kernels_plan.md` — the vulkan class-kernel arc plan (`[spirv_kernel]`/`[vk_dispatch]`; shipped — serving is 100% class kernels, the module-global kernel world deleted)
- 2026-08-06 `modules/dasLLAMA/vulkan_family_walkthrough.md` → `history/dasLLAMA/vulkan_family_walkthrough.md` — per-family das-vs-llama.cpp vulkan walkthrough record (debug-grade rows; verdicts live on as `followup_vulkan.md` items 11/13/14)
- 2026-08-07 `modules/dasLLAMA/INVENTORY.md` → `history/dasLLAMA/INVENTORY.md` — the 2026-07-29 as-is census that scoped the reorg (self-dated snapshot at b0049dc52; the reorg + class-kernel + cm2 arcs rebuilt what it describes)
- 2026-08-16 `ORIGINAL_PLAN.md` (dasVulkan standalone repo root) → `history/dasVulkan/ORIGINAL_PLAN.md` — the boost-layer design plan that drove the raw→boost build-out (all 9 phases shipped; landed here rather than under `modules/dasVulkan/` when the module moved in-tree)
- 2026-08-17 `modules/dasLLAMA/metal_media_plan.md` → `history/dasLLAMA/metal_media_plan.md` — the metal-media arc's plan (chunks 1-3: tower, media, the whisper decoder + conv frontends on Metal; shipped via #3754/#3763)
