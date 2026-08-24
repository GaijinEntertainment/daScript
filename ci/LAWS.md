# LAWS.md - Boris's rulings

Append-only intent provenance for rule-document edits in this folder's documents (the
mechanism: CLAUDE.md sec. "Boris's rulings get a `LAWS.md` sidecar"). Never groomed,
compacted, or cited as rules.

| Date | Document | The ask |
|---|---|---|
| 2026-08-24 | smoke_test_bundle.sh (no-REVIEW gate) | "CMake install skips REVIEW.md and REVIEW.das - its 100% internal" - the dasllama-server glob leak fixed, the bundle smoke gained the check; REVIEW_COMMON.md stays the deliberate exception (adopting repos vendor from it) |
| 2026-08-24 | CMakeLists.txt (install-manifest check) | "REVIEW.md and REVIEW.das is in more places than dasllama - and should never install anywhere. should probably make it part of CMake install check" - a terminal install(CODE) scans the full install manifest and FATAL_ERRORs on any REVIEW*.md / REVIEW*.das outside root REVIEW_COMMON.md |
