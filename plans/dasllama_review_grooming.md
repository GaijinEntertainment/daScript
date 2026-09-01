# dasLLAMA REVIEW.md grooming backlog (dragon findings, 2026-09-01)

A full-document dragon pass over `modules/dasLLAMA/REVIEW.md` during the metal-kq-race PR
surfaced findings on rules that PR did not touch. The diff-scoped ones were fixed in that PR
(the harness routing duplicate deleted, the bench-list rule moved to `harness/REVIEW.md`, the
reference-build carve complement re-paired). The rest is a grooming pass of its own - one
sitting, then a fresh dragon:

- SPLIT: the measured-number/servable-capability routing head; the `DASLLAMA_RELEASE` rule
  (two triggers); the tokenizer `--tok` rule (the superlinear verdict is its own rule); the
  override-announce rule (four criteria + a five-line definition - the longest in the file).
- WRONG DOCUMENT: the uniform/kargs dispatch-value ban -> `REVIEW_GPU.md`; the GPU-hook
  CPU-form rule -> `REVIEW_GPU.md`.
- MERGE: the two consecutive sidecar-exchange routing rules.
- DUPLICATE: the routed-file meta-rule (restates `REVIEW_COMMON.md`); the restore-check
  weakening clause (the general `REVIEW.das` weakening rule covers it).
- AMBIGUITY/undefined terms: "stocked model file", "a stated decision" (name the PR body),
  "records a run" (name the PR body), "beside one that has its own sec.1 charter line",
  "escaped file", "a parity or oracle rail" (define in place).
- REMOVE EXCEPTIONS: the STYLE037/038 ledger rule's dedup carve-out (move the boundary into
  the trigger).
- STATEMENT: the facade-def TAUGHT rule (keep only the overload residue the
  `check_tutorial_floor` gate cannot see).
- RECOMMEND LINT (gate candidates): bench-list-last as a `REVIEW.das` parse;
  `[EnvConfig]`-struct <-> `env_markdown()` pairing; `requires=` names vs
  `TUNE_KNOWN_FEATURES`; team-lane global reachability walk.

## tests/REVIEW.md (same dragon pass)

Diff-scoped fixes landed in the PR; the rest, one sitting: the suite-listing exception
absorbed into its trigger; the disarmed-file rule split; direct-dastest rule trimmed of its
statement tail; the -jit rule split from the compile-only-lane duty; the CMakeLists ban gains
its replacement clause; "re-lanes" defined; the box-decided-predicate and lane-pin carve
sentences dissolved into triggers; "silently vanishes"/"laundered"/"its grade" idioms
plained; the 70B batch-parity rule re-keyed on DASLLAMA_PARITY_FULL; the in-place-cell and
stocked-artifact ambiguities spelled; the establish-and-restore rule's ARCHITECTURE_GPU
sec.1.5 cross-cite and vulkan carve removed per the ledger handshake. STRUCTURAL: rename
tests/CLAUDE.md -> tests/ARCHITECTURE.md so the checklist's eight section cites stop being
rule-doc-to-rule-doc. RECOMMEND LINT (tests/REVIEW.das candidates): suite-listing glob vs
run.das lists; arm-name census vs CLAUDE.md's arm roster; the out-of-folder [test] ledger;
the CMakeLists registration ban.

## REVIEW_EXCHANGE.md (same dragon pass)

Diff-scoped fix landed (the moved consent rule's `../` path re-based). Backlog: drop the
cross-folder gate cite from the second-HTTP-path ban; the weakening rule reduces to the
test_exchange_client residue with "submission strip" defined in place; the tune-boot fallback
rule re-worded as its duty ("falls back to the local sidecar and the tune winners built into
the exe"); the consent-question trigger spelled to reading (b). RECOMMEND LINT: a REVIEW.das
byte-identity check over the three consent-notice copies; the strip/may_contact reachability
check over dasllama_exchange.das.

## performance/REVIEW.md (same dragon pass)

Diff-scoped fixes landed (the provenance head re-wrapped, the trailing blank dropped).
Backlog: the second-validator ban's trailing gate-cite sentence merges into a widened
"Weakening any of `REVIEW.das`'s checks is a defect" head, with the re-mint fix path split
out and "quiet, session-free box" spelled as `noise: ok` + `hardware.remote_desktop: off`;
`das` row defined at first use ("a row whose `engine` is `das`"); the no-`sha` reference-row
body re-worded as its duty; the board-cell rule's restating clause cut; the model-file
provenance rule compressed to the two-sentence form; the companion-artifact statement tail
reduced to the test_model_specs weakening residue; the `--tune` flag re-homed off
`fetch_models.das`. RECOMMEND LINT: widen make-pr `gate_stamp_reach`'s glob to every
checked-in `.json` under `*/performance/`; a `check_sidecar_archive` in performance/REVIEW.das
(filename sha12 == content hash; every row's `tune_sha` resolves).

## REVIEW_GPU_RACE.md (same dragon pass, all verdicts APPLIED in the PR)

RECOMMEND LINT residue only: extend `check_race_bind_numbers`'s walk beyond
`modules/dasLLAMA/dasllama` to `benchmarks/`, `harness/`, `performance/` (deletes the
"outside `dasllama/`" arm of the hand-verified rule); a `tests/` cell calling `race_pair_ms`
with counting blocks proving both arms ran before the first timed rep (reduces the
burn-weakening rule to the test residue).

## daslib/REVIEW.md (same dragon pass)

Diff-scoped fixes landed (the PERF023 doubled locator + all three wrap artifacts; the emit
entry-point gloss cut with its rewrap). Backlog, one sitting: the dedup-key rule names
`report_key`; the architecture-doc-entry rule DELETED (restates ARCHITECTURE_COMMON.md's
last rule); "collapse suggestion" defined inline; the residual-oracle criteria promoted to
the head; the ExprVar-string ban gains its replacement (`hash(expr.name)`); the Template
delete rule re-keyed as "Never add a `delete` for ... declared `inscope`"; the ast_verify
naming rule SPLIT (the post-infer placement duty its own rule); the `[|> name]` structure
macro rule's copula head promoted to duties. RECOMMEND LINT (utils/lint/REVIEW.das cells):
RULE_MODULES membership census over daslib emitters; the reverse census (fixture/alias/rst
id -> still-emitting module); message-literal starts with a registered id + colon. And one
style_lint candidate: raw string-loop char indexing a byte-class table without
`uint(uint8(ch))`. At 262/300 lines - when LINT027 fires, the split seams are the lint /
CppAot-emit / flatten_opt clusters.
