# LAWS.md - Boris's rulings

Append-only intent provenance for this folder's rule documents (the mechanism: CLAUDE.md
sec. "Boris's rulings get a `LAWS.md` sidecar" at the repo root). Never groomed, compacted,
or cited as rules.

| Date | Document | The ask |
|---|---|---|
| 2026-08-22 | REVIEW.md, ARCHITECTURE.md | "we produce REVIEW.md and ARCHITECTURE.md for the sample - based on distillation" - the distillation being the analysis-bot -> rescue-bot pass over this comment-free sample (written by grok as Boris's model test; the game shipped to the examples page) |
| 2026-08-22 | REVIEW.md (the test-residue rules), test_cave_rules.das, test_sfx_gen.das | "we test\fix the potential bugs" - the four suspected defects the distillation surfaced: magic wall converting resting objects, ASCII-loaded exit never opening, GL_BLEND left to leftover state, sine sweeps sounding twice their named span (fixed audio-identically) |
| 2026-08-22 | (context for both) | The sample's known asked-for changes, per Boris: "make the controls snappier" and - confirmed by blind guess from retrofit scars - "stop killing me at the start of the cave" |
| 2026-08-28 | REVIEW.md, ARCHITECTURE.md | Port convergence: "i'd like to get to the point where playground and web versions are identical"; for boulder-dash, adopt the port's `sfx_gen.das` extraction desktop-side ("1. yes") and move the port's test suites desktop-side, one merged set in both places ("2. yes"). live_stub.das deleted - the wasm runtime resolves `live/*` natively - and the placement block and architecture opening updated to match |
