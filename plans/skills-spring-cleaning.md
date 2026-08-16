# Skills spring cleaning — map of record

Census: 6 parallel readers over 69 root skills + the daslang bundle + CLAUDE.md + infra.
Charter: `skills/internal/skill_taxonomy.md` (rule 0: shorten and simplify, a lot).
This file is the ratified classification map; the census detail lives in the session that
produced it, the durable decisions live here.

## Rulings (Boris, 2026-08-16)

1. CLAUDE.md keeps a ~20-line silent-miscompile digest; everything else moves or dies.
2. Repo-only skills move to `skills/internal/`; shipped set stays at root.
3. `install/skills.list` is deleted; the folder is the shipping decision.
4. The SDK ships `skills/daslang/` wholesale; getting-started pages mention it; auto-pickup
   via CMake install into `<prefix>/.claude/skills/` + a documented one-liner for user
   projects.
5. detect_dupe family = internal. decs, dashv = external root.
6. tune/llvm_tune: fold llvm_tune's user half into `tune.md` (root, path-frozen RST
   include); maintainer residue → `skills/internal/llvm_tune_internals.md`.
7. dasImgui SHIPS with the SDK (follow-up arc, right after this one, adds the install
   rules). imgui skills classify to that end state: application/ui_debugging/migration =
   root; playwright/recording = internal.
8. `comment_style_hygiene.md` ships. The "How this guide is applied" trim was tried by a
   wave-3 agent and REVERTED (Boris: not sold on agent work there, for now) — the file is
   byte-identical to pre-arc; the applied-block's repo references are a known phase-4 gate
   question. The review machinery ships with it: `.claude/agents/dragon.md` installs to
   `<prefix>/.claude/agents/`, `REVIEW_COMMON.md` ships to the SDK root, `review_md.md`
   ships.
9. `tdd_audit.md` ships; the reliably-failing-fixture rule was folded in (done).
10. Governing line for reference absorption: daslib/builtin-backed reference skills go
    into `skills/daslang/references/`; module-backed skills stay at root.
11. Doubts go to Boris directly, one at a time. No dragon review of the mass reshuffle;
    the two rule docs (taxonomy, writing_skills) were dragon-passed separately.

## Destinations

**Absorb into `skills/daslang/references/`** (rewritten to bundle purity — no lint IDs,
no MCP, no repo paths; stripped lint material folds into CLAUDE.md policy or dies):
- `strings.md` + `regex.md` → new `references/strings.md`
- `filesystem.md` + `glob.md` → new `references/files-and-paths.md`
- `json.md` → new `references/json.md`
- `linq.md` → new `references/queries.md` (+ ~20-line fusion digest from
  linq_fold_patterns)
- `clargs_usage.md` → new `references/cli-and-config.md` ([EnvConfig] mechanics included;
  the repo knob inventory stays in `environment_variables.md`)

**Stay at root (shipped), notable notes:**
- `xml.md`, `sql.md` (module-backed; bundle's modules-and-stdlib gets one-row pointers)
- `tune.md` (RST include target — path-frozen; absorbs llvm_tune's user half; light-touch
  rule-0 only, every edit is a doc edit)
- `linq_fold_patterns.md` (RST include target, path-frozen; exempt from the no-history
  rule as a published doc page)
- `decs.md`, `dashv.md`, `daslang_live.md`, `daspkg.md`, `mcp_tools.md`,
  `external_module_debugging.md`, `strudel_port.md`, `das_formatting.md`,
  `das_macros.md` (cut to ~220 — the ~380-line overlap with references/macros.md dies),
  `daslib_modules.md` (cut to ~35 — conventions only), `writing_tests.md`,
  `memory_leak_detection.md`, `jobque_debugging.md`, `profiler.md`,
  `project_overview.md`, `design_philosophy.md` (RST include), `perf_lint.md` +
  `style_lint.md` (split: interpretation half stays root — keeps the link closure —
  authoring half internal), `tdd_audit.md`, `comment_style_hygiene.md` (trimmed),
  `review_md.md`, `imgui_application.md`, `imgui_ui_debugging.md`, `imgui_migration.md`,
  `dynamic_modules.md` (split: .das_module authoring root, CMake/static plumbing
  internal), `cpp_integration.md` (split: embedding root, codebase-notes + repo-MCP
  sections internal), `writing_benchmarks.md` (split per census line).
  `gc_migration.md`: DELETED outright (Boris, wave 3 — migration is complete; history
  lives in git), both halves; inbound pointers dropped in memory_leak_detection (skill +
  RST), visitor_gen_bind, sql.md, both CLAUDE.md tables, skills.list.

**Move to `skills/internal/`:** abi_break_sweep, aot_hash_desync_debugging, aot_testing,
babysit, build_and_debug, clang_bind_build, daslang_lsp, doc_archiving, doc_sweep,
documentation_rst, environment_variables (RST include — retarget
`doc/source/reference/environment_variables.rst:12`), imgui_playwright, imgui_recording,
install_instructions (rewrite: fold surviving CMake/CI facts into taxonomy +
writing_skills, archive the manifest model per doc_archiving), make_pr, preflight,
review_round, review_triage, tests_in_repo, tutorial_prose, tutorials, version_update,
visitor_gen_bind, writing_cpp_tests, writing_skills, wsl_ci_repro, detect_dupe,
detect_dupe_reference (RST include — retarget
`doc/source/reference/utils/detect_dupe.rst:17`), find_dupe, + new
`daslang_internals.md` (CLAUDE.md's D-bucket: collect-banner gating, `/*option*/` marker
rule, fixedDim internals, mangling-collision defect, etc.) + new `llvm_tune_internals.md`
+ the internal halves of the splits above.

## CLAUDE.md language migration (census B)

Buckets: ~88 claims delete (bundle already carries them), 21 move into the bundle
(destinations recorded per-claim in the census), ~37 stay as house policy (the idiom/lint
table), 12 → `internal/daslang_internals.md`. The ~20-line digest replaces the deleted
mass (10 traps; the `-jit` comprehension trap is DEAD — fixed by literal lowering — do
not resurrect). `install/CLAUDE.md:103-343` is a twin: same treatment, bundle becomes the
SDK's language reference.

Divergences to reconcile (newer probe wins): generator syntax (bundle's `generator<T> {}`
wins; CLAUDE teaches the older `$()` form), literal-adaptation context list (bundle is a
superset), `-const` on concrete targets is a no-op (probe 2026-08-16 — rewrite the
type-modifiers bullet; STYLE036 is right), `delete array<T?>` symptom (CLAUDE's richer
telling moves into the bundle), reserved-token list (add range64/urange64 + the
annotation-arg case to the bundle), table double-lookup (bundle right: it is REJECTED,
not a silent hazard), the duplicated `:=` string rule inside CLAUDE.md, the blank line
splitting the idiom table at ~line 425.

## Infra (census F — the spec of record for phase 4)

- CMake: replace the `file(STRINGS)` block with two `install(DIRECTORY)` calls (PATTERN
  matches last component only — daslang/README.md exclusion needs REGEX; trailing-slash
  semantics; the ASCII caveat dies). Add `.claude/skills/daslang` + `.claude/agents/dragon.md`
  installs (precedent: the LSP manifest block). Ship REVIEW_COMMON.md to the SDK root.
- `ci/check_shipped_skills.py`: os.walk; folder-derived shipped-set; explicit
  `skills/internal/` link ban (the current SKILL_REF regex is BLIND to internal/ links —
  fix); daslang/ purity checks (repo paths, bin/, MCP, CI, lint IDs; no repo-only escape
  there); dead-link resolution gains dirname-first (or references/ links all report
  dead); trigger-row presence check vs install/CLAUDE.md; extend the dead-link walk to
  skills/internal/ (dragon recommendation). KNOWN HAZARD: today, a missing list arg
  silently disables the closure check — the rework must not inherit that shape.
  REPO_PATH gains `history/`, `utils/daScript/`, `utils/lint/tests/` (leaks found).
- `ci/smoke_test_bundle.sh`: drop the list arg; add presence asserts for
  SKILL.md + references/ (replaces the deleted CMake FATAL_ERROR); fix the
  `GETTING_STARTED.md` `bin/Release/daslang.exe` lines while there.
- `ci/reinstall_bundle.sh`: configure-time comment + `ls skills/*.md | wc -l` undercount.
- `install/CLAUDE.md`: language half → digest + bundle pointer; detect_dupe rows out;
  bundle trigger row in; formatter/converter trap is INVERTED today — fix.
- Getting started: `install/README.md` "What's in the Box" + AI-assistants section;
  `GETTING_STARTED.md` after the LSP section; `doc/source/getting_started.rst` mirror.
- `install_instructions.md` phantom `ci/check_shipped_skill_refs.awk` (×2) — dies with
  the rewrite.

## Retarget traps (census C)

- `.gitignore` has `install/` — ripgrep-based tools (Grep tool, MCP grep) SILENTLY SKIP
  tracked `install/CLAUDE.md`. Retarget sweeps must use `grep`/`rg --no-ignore` there.
- Runtime strings: `modules/dasImgui/widgets/imgui_lint.das:213,218` print
  `skills/imgui_migration.md` — stays valid (migration stays root).
- Hard RST includes: environment_variables.rst:12, utils/detect_dupe.rst:17 (retarget on
  move); design_philosophy/linq_fold_patterns/tune/profiler includes unaffected (files
  stay put).
- `.claude/agents/*.md` reference comment_style_hygiene (stays root — fine), review_md
  (stays root — fine), writing_skills (→ internal: retarget dragon.md:39).
- Tools with `skills/` strings: utils/preflight/main.das, utils/review-md/main.das,
  utils/pr-babysit/main.das (all → internal paths), utils/mcp/cpp_search_config.das:15
  (stale `install/skills/` path — fix), env_cfg.h:24, dasllama_env.das,
  tests/language/inline_never_module.das:21.
- The full per-file link inventory was enumerated in the census; move agents re-derive
  per file with `grep --no-ignore` and cross-check against the trap list above.

## Wave-2 follow-ups (for wave 3 owners)

- `linq_fold_patterns.md` "What falls back" bullet is STALE (probe-resolved against
  `daslib/linq_fold_common.das:6090` + adapter `emit_join_hook`s): array/table/JSON/XML
  equi-`_join` DOES splice; only left/right/full-outer/cross joins + non-primitive keys
  cascade. Fix the bullet in the light-touch pass (RST include — doc edit).
- ~~`get_this_module_dir()` → add to `daspkg.md`~~ RESOLVED wave 3: the fact never lost its
  home — daspkg.md already carried the full *Runtime asset paths* section; a cross-link from
  Key Details was added. Residual idea (not this arc): `daslib/fio.das` `parse_file_list`'s
  param is NAMED `file` though it is argument text — a rename would kill the trap at the root.
- New probe (2026-08-16): one-line enum body takes commas — `enum E { A; B }` is
  error[30151] → one line for `references/types.md`.
- `filesystem.md`'s `parse_file_list(file, ...)` param is argument TEXT, not a path —
  carried with annotation; fix the root source too if it survives.

## Wave-2c residuals

- LINT016 (`:=` on a string) lost its only rule-ID mention in the shrink — the digest
  carries the semantics; add an idiom-table row for the rule ID.
- `install/README.md:33` lists `das-fmt` as a `bin/` binary — same inversion fixed in
  install/CLAUDE.md; phase 4 owns README.
- Transient until phase 4: install/CLAUDE.md points at `skills/daslang/` before the CMake
  rules ship it; the old gate may flag it until the rework lands.

## Wave-3 residuals (homeless facts from the daslib_modules/das_macros cuts)

Listed by agent C, deleted from nowhere yet — each needs a probe before entering the bundle
(bundle rule) or a placement edit (root skill). Place during wave 4/5:
- `notify` vs `notify_and_release` (lambda-captured channel holds an extra ref) +
  child-script channel ops need `require daslib/jobque_boost` and `compile_file` +
  `make_file_access("")` → home: `skills/jobque_debugging.md` (root, no probe needed —
  behavioral claims carried from the deleted text, verify against daslib/jobque_boost).
- `logger_install_hook()` — process-wide debug agent diverting print/to_log; mandatory for
  stdio-transport processes; one install covers `new_thread` workers → bundle
  modules-and-stdlib logger row (probe: grep daslib/log.das + a compile probe).
- `move(dest, src)` = refcount-aware smart_ptr move vs `<-` memcpy+memset → bundle
  references/memory.md (probe).
- `return <- expr` on a `&` ref parameter zeroes the CALLER's variable → candidate for the
  CLAUDE.md fails-silently digest + references/memory.md (probe REQUIRED before either).
- Module existence rows with no bundle mention: `daslib/flat_hash_table.das`
  (`TFlatHashTable`), `daslib/soa.das` (`[soa]`), `daslib/ansi_colors.das`,
  `daslib/temp_strings.das` → bundle modules-and-stdlib table (grep-verify).
- Lint-message audit implied by the no-catalog doctrine: the deleted tables carried fix
  guidance (e.g. PERF030's three escape routes) possibly richer than the current error
  strings — sweep `daslib/*_lint.das` messages against the deleted tables (git history) and
  upgrade any message that lost information. Boris also wants a LINT LOG follow-up: per-run
  capture of findings on freshly written code, to judge the idiom table empirically.

## Phases

1. ✅ Charter + census + rulings (this file).
2. ✅ Language migration (bundle = truth; twins shrank; daslang_internals.md born).
3. ✅ Moves + splits + absorptions + retargets (opus fleet + day rulings: lint catalogs
   deleted everywhere, idiom table trimmed, gc_migration deleted, hygiene trim reverted).
4. ✅ Infra: install(DIRECTORY) + .claude auto-pickup + dragon/REVIEW_COMMON ship;
   gate rewritten; skills.list dead; getting-started x3.
5. ✅ Verification + PR #3755: install+smoke 19/19, gate fixture suite 12/12
   (negative-controlled), sphinx -W clean, lint/AST/format clean, module registry tests
   green; 7-auditor round applied. OPEN: babysit #3755; the ledgered pre-existing
   checklist findings below go to Boris one at a time.

## Checklist round outcomes (2026-08-16, rulings 1-4 EXECUTED + dragon-converged)

- benchmarks/REVIEW.md MOVED to modules/dasLLAMA/benchmarks/ + regroomed (3 rules; the two
  event-triggered lab rules moved to REVIEW_GPU.md where their triggers fire).
- simulate REVIEW.md: hot-path rule is folder-wide property-triggered; pin obligation folded
  into the debug_info rule; NEW tests-cpp/REVIEW.md (routing) + tests-cpp/small/REVIEW.md
  (the pin rule, timeless form).
- dasImgui REVIEW.md: executable-content trigger, Windows parenthetical deleted per the
  no-code-on-top-of-bug ruling (libhv root fix = Boris, own session, memory-tracked),
  tree-sitter rule deleted; NEW modules/dasImgui/tests/REVIEW.md (multiplatform rule).
- dasLLVM REVIEW.md: two scanner-enforced env rules deleted, version rule split (emitter
  enumeration → ARCHITECTURE.md §1.2), llvm_tune_internals routing line.
- REVIEW_GPU.md: gained the two lab rules + full definition-in-place/path-qualification
  pass. ALL files exited dragon all-OK.

**Lint/test candidates harvested by the dragons** (for the lint backlog, Boris rules):
twin-binding-stability dastest; device/queue-creation call-site check; raw Vulkan
pipeline-create call-site lint; raw encoder-bind outside enc_* lint; f16 mirror-clamp
dastest cell (deletes its rule); debug_info PR-verdict-lines CI assertion; pin-watched-set
touch assertion; JIT timed-phase inventory dastest; sidecar mint-and-validate round-trip
dastest; env-knob §3-inventory scanner extension; computed-name env-read non-literal-arg
scanner extension; preflight changed-set auto-escalation for the imgui gate; machine-local
path-literal style lint for test files; hot-path entry-annotation perf_lint rule (reuses the
PERF026-028 reachability walk); .das_module completeness diff vs dasllama/*.das (verified NO
current drift, 85 = 85); audio/stbimage require-graph single-file assertion; dlim_wipe
single-call-site assertion.

Ruling 5 (dasLLAMA master) EXECUTED same day, then largely SUPERSEDED at the rebase:
master had its own parallel grooming of the same rules (the version split, a concrete
`*_decline_caps` rule, the EMITTED-shader indirection form, the entry-point split) —
master's text won wholesale; what survives from our round is the mechanical layer
(21 engine-filename `dasllama/` qualifications + the routing-block count drop). Same at
the checklist-round commit for REVIEW_GPU's role-file and decline rules. The
ledgered-findings queue is CLOSED — all five rulings executed, by one hand or the other.

## Ledgered checklist findings (pre-existing, NOT this PR — Boris rules each)

- benchmarks/REVIEW.md: structurally homeless (all four rules' triggers live under
  modules/dasLLAMA/benchmarks/ — the ancestor walk can never fire them from root
  benchmarks/); "the one sanctioned" phrase REVIEW_COMMON bans; A/B-lab rule = four
  undefined terms; "today that is X alone" census; tune_gate rule duplicates
  performance/REVIEW.md's sharper copy. Proposed fix: move to
  modules/dasLLAMA/benchmarks/REVIEW.md + the five rewrites.
- include/daScript/simulate/REVIEW.md: eval* trigger names runtime_table.h which has no
  SimNode (property-vs-locator ambiguity); the new layout-pin clause cannot fire from
  this folder (pin lives in tests-cpp/).
- modules/dasImgui/REVIEW.md: behavioral-vs-file-kind trigger ambiguity (comment-only
  .das change: suite or no suite?); exclude-passthrough impossible on Windows as
  written; tree-sitter routing rule can never change an outcome; `tests/dasImgui`
  missing "(repo root)".
- modules/dasLLVM/REVIEW.md: two env rules fully scanner-enforced (automated =>
  deleted); LLVM_JIT_CODEGEN_VERSION rule = 3 rules in one paragraph.
- modules/dasLLAMA/REVIEW.md: can-run/is-ready rule names no API; version-bump and
  hot-path-coverage rules over the one-paragraph limit.
- comment_style_hygiene.md GUIDE gap: header-that-is-also-__doc__-output — map rule vs
  message rule undecidable (hygiene file frozen per Boris; ledger only).
