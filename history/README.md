# history/

Design docs, plans, audits, findings, and session notes from **completed arcs** — kept for
the record, no longer load-bearing. A doc belongs here when its remaining value is the
record ("why is it built this way"), not operation. Living contracts and durable records
stay next to their code (e.g. `modules/dasSQLITE/PROVIDER_CONTRACT.md`, the `MASTERPLAN.md`
files, `modules/dasLLAMA/*_plan.md` specs still cited from code, `results.md` ledgers).
Process and criteria: `skills/doc_archiving.md`.

- `compiler/` — shipped compiler-rework docs (fixed-array structural types, annotation-info
  rtti rework, quote lowering)
- `ci/` — process/coverage analyses (the local-vs-CI coverage gap plan; its stage 4 lives
  on as the nightly daspkg-index cron)
- `dasLLAMA/` — closed dasLLAMA arc docs (superseded hand-written AVX tier matrix, EPYC
  fleet measurements, the model-expansion wave plan)
- `dasSQLITE/` — the typed-SQL API rework corpus (`API_*.md`, `TUTORIALS.md`, the
  `tutorial-mockup/` design artifacts); the living contract stays at
  `modules/dasSQLITE/PROVIDER_CONTRACT.md`
- `dasSpirv/` — the completed PHASE6–9 working plans (content captured in
  `modules/dasSpirv/MASTERPLAN.md`)
- `linq_fold/` — the linq_fold arc's plans and audits; the living masterplan is
  `daslib/linq_fold.md`, results stay at `benchmarks/sql/results.md`
- `examples/` — plans, findings, and research notes behind shipped examples
- `tests/` — one-off test-migration audits

Source comments referring to these docs by bare name (e.g. `FIXED_ARRAY_REWORK.md`) resolve
here via search; path-qualified references were updated at move time. Every archived doc is
listed in the log below — search it first when hunting for a doc.

## Archive log

2026-07-24:

- `FIXED_ARRAY_REWORK.md` → `history/compiler/` — the tFixedArray structural-type rework plan (shipped 0.6.3)
- `ANNOTATION_INFO_REWORK.md` → `history/compiler/` — rtti self-contained annotation-info rework
- `QUOTE_LOWERING.md` → `history/compiler/` — quote/qmacro lowering under AOT + JIT
- `COVERAGE_GAP.md` → `history/ci/` — local-vs-CI coverage gap analysis (stage 4 lives on as the nightly daspkg-index cron)
- `examples/flatten/scalar_packing/PLAN.md`, `FINDINGS.md` → `history/examples/scalar_packing/` — scalar-packing loop re-roll design + findings
- `examples/daStrudel/sfx_lab/PLAN.md`, `NOTES_drum_session.md`, `research/metric_design.md`, `research/seeding_from_analysis.md` → `history/examples/sfx_lab/` — SFX-lab plan, drum-session notes, fit-metric research
- `examples/games/sequence/PLAN.md` → `history/examples/sequence/` — sequence board-game live-coding plan
- `examples/telegram/dictation/CADMUS_PLAN.md` → `history/examples/dictation/` — Cadmus assistant product decisions
- `tests/REVIEW.md` → `history/tests/` — daScriptTest-migration review checklist
- `modules/dasSQLITE/API_REWORK.md`, `API_MISSING.md`, `API_CHECKED.md`, `API_MIGRATION.md`, `TUTORIALS.md` → `history/dasSQLITE/` — the typed-SQL API rework corpus (design record, deferred-feature list, equivalence-harness plan, migration design, tutorial coverage)
- `modules/dasSQLITE/tutorial-mockup/` → `history/dasSQLITE/tutorial-mockup/` — pre-implementation tutorial design artifacts (all features since shipped)
- `modules/dasSpirv/PHASE6_VISITOR_PORT.md`, `PHASE7_TEXTURES.md`, `PHASE8_LANGUAGE.md`, `PHASE9_TUTORIALS.md` → `history/dasSpirv/` — completed phase working plans (content captured in MASTERPLAN.md)
- `modules/dasLLAMA/avx_kernel_matrix.md` → `history/dasLLAMA/` — hand-written AVX tier matrix (superseded by the generated `[tune]` GEMM family)
- `modules/dasLLAMA/epyc9654_measurements.md` → `history/dasLLAMA/` — EPYC 9654 campaign measurements
- `modules/dasLLAMA/model_expansion_plan.md` → `history/dasLLAMA/` — model-expansion wave plan (waves shipped 0.6.4)
- `benchmarks/sql/LINQ.md`, `LINQ_TO_DECS.md`, `LINQ_TO_TABLE.md`, `M4_DECS_EXPANSION.md`, `linq_fold_chain_audit.md`, `sqlite_linq_gaps.md` → `history/linq_fold/` — linq_fold arc plans and closed-out audits (shipped 0.6.3)
