# daslang Project Instructions

## Project Overview

This is the [daslang](https://daslang.io/) programming language repository (GaijinEntertainment/daScript). daslang (formerly daScript) is a high-performance statically-typed scripting language for games and real-time applications. The repository and many C++ API names still use the old "daScript" spelling.

For the **why** — design principles, three-tier execution model, the macros-as-design-lens rule — read `skills/project_overview.md`. The full long-form rationale lives in `skills/design_philosophy.md`, which is also the single source `doc/source/reference/design_philosophy.rst` includes for daslang.io — edit the `.md`, never a copy.

## Build & Run

CMake-based build, supported on Windows / Linux / macOS / iOS / Android / WASM (CI runs the full matrix). Quick start: `cmake --build build --config Release -j 64`, then run `bin/Release/daslang path/to/script.das` (Windows MSVC layout) or `build/daslang ...` (single-config Make/Ninja). Builds are slow (15-25 min clean, 2-10 min incremental) — always pass `timeout: 0` to long `cmake --build` commands, do NOT assume "no output" means failure.

Full reference (per-platform generator commands, build flags, AOT debugging, exit code meanings, runtime crash diagnostics): **`skills/internal/build_and_debug.md`**.

**Build all of your own modules.** A session runs its tree's binary with shared_modules built
from THAT tree — never `-load_module` another checkout's build. A module compiled against
different daScript headers can load cleanly, pass tests, and still be subtly wrong: ABI drift
surfaces as behavior or performance skew, not necessarily a crash — and it poisons every
measurement made through it. External-module repos (dasImguiNodeEditor etc.) get a detached
worktree under `<tree>/modules/<name>` with `_build` configured `-DDASLANG_DIR=<tree>`.

## Workspace Hygiene

Clean up files created for diagnostics, staging, and one-off tests before handing work back. This
includes temporary release trees, formatter backups, extracted audio, generated `__pycache__`
directories, and ad-hoc stdout/stderr logs. Keep intentional application logs, databases, configs,
rollback bundles, and user-owned artifacts. In deployed application directories, do not leave
unrelated diagnostic files beside the program.

## GitHub Operations

- **Use GitHub MCP tools** (`mcp__github__*`) for all GitHub operations (creating PRs, listing issues, reading PRs, etc.) — they avoid shell escaping issues entirely
- **Fallback:** If MCP tools are unavailable, use `gh` CLI with `--body-file` for any text containing backticks (they're shell escape characters in every supported shell)

## MCP-first search

Before reaching for `Bash`/`Grep`/`Read` to find a symbol or trace usages in this repo, ToolSearch the daslang MCP tool that answers the question and call it. The tools are deferred (schemas not in turn-start context) so the workflow is two calls: `ToolSearch select:mcp__daslang__<tool>` → invoke.

| Question | Tool |
|---|---|
| Where is symbol X defined? (`.das`) | `find_symbol` — pass `with_cpp_source=true` for builtins / handled types to bridge daslang→C++ in one call |
| Where is symbol X defined? (`.cpp`) | `cpp_find_symbol` |
| Where is X used? (`.das`) | `grep_usage` |
| Where is X used? (`.cpp`) | `cpp_grep_usage` |
| What's in this file? | `outline` (`.das`) / `cpp_outline` (`.cpp`) |
| Goto definition at cursor | `goto_definition` (`.das`) / `cpp_goto_definition` (`.cpp`) |
| All references to X (`.das`) | `find_references` |

Same applies to lint/format: `mcp__daslang__lint` / `format_file`, not shell `bin/daslang` invocations. Same applies to GitHub operations (see above): `mcp__github__*`, not `gh` shell.

Fall back to `Bash`/`Grep`/`Read` only when the MCP tool reports an error or the question is genuinely outside MCP coverage (RST prose, CMake, Python tooling).

**Cross-tree guard:** when you work in a git worktree, MCP tool results and LSP diagnostics prepend a `CROSS-TREE WARNING` if you query a file **outside the server's own tree** — that answer used the *wrong* tree's module sources AND that binary's compiled-in C++ bindings, so it's likely stale (missing symbols, outdated signatures). Heed it, don't trust cross-tree output: run the session *inside* the worktree (its checked-in `.claude/skills/daslang-lsp` gives correct diagnostics; `daslang utils/mcp/setup.das -- --root <worktree>` bootstraps its MCP tools). A main-tree-rooted session can never give correct diagnostics for a worktree file.

## Skill Files (REQUIRED)

Task-specific instructions are split into skill files under `skills/`. You MUST read the relevant skill file(s) before performing the corresponding task.

| Skill file | Read BEFORE... |
|---|---|
| `skills/project_overview.md` | First significant task — design philosophy, three execution tiers, macros-as-design-lens |
| `skills/internal/build_and_debug.md` | Build flags, AOT build commands, exit-code/crash diagnosis, `options log_infer_passes` |
| `skills/mcp_tools.md` | Full MCP tool table + live-API reference |
| `skills/das_formatting.md` | Creating or modifying any `.das` file |
| `skills/comment_style_hygiene.md` | Writing or reviewing comments, names, or local code shape in ANY language — the house rulebook the `style-hygiene-auditor` agent applies to every PR's new code |
| `skills/writing_tests.md` | Writing or editing any dastest test (ships in the SDK; applies everywhere) |
| `skills/internal/tests_in_repo.md` | Adding/moving tests **in this repo** — AOT registration for a new `tests/` dir, the `tests/.das_test` gating filter, deep-engine model-test rules. Repo-only; deliberately not shipped |
| `skills/internal/writing_cpp_tests.md` | Writing or editing C++ tests under `tests-cpp/` (doctest, leak guards, ctest wiring) |
| `skills/internal/documentation_rst.md` | Editing RST in `doc/source/`, `//!` doc-comments in `daslib/*.das`, tutorial RST pages |
| `skills/internal/tutorials.md` | Anything that looks like a tutorial — they live under `/tutorials/<area>/`, NEVER `modules/<X>/tutorial/` |
| `skills/internal/tutorial_prose.md` | WRITING or revising any general-reader doc/tutorial prose — the voice/vocabulary/structure rules; `documentation_rst.md` is mechanics, this is the words |
| `skills/cpp_integration.md` | Embedding daslang in C++; binding types/functions/enums |
| `skills/internal/cpp_codebase_notes.md` | Working on daslang's own C++ — where inference/builtins/errors/parser live, AST function flags, the `cpp_*` MCP search workflow |
| `skills/internal/clang_bind_build.md` | Enabling `dasClangBind` / bumping the libclang SDK / running any `bind_*.das` self-binder |
| `skills/daslib_modules.md` | Working with `daslib/` modules or extending the stdlib |
| `skills/das_macros.md` | Compile-time macros, AST manipulation, qmacro/quote, gc_node patterns |
| `skills/internal/daslang_internals.md` | Reasoning about how the compiler implements something no user-facing doc covers — the collect banner, the `/*option*/` marker, known mangling defects |
| `skills/writing_benchmarks.md` | Writing/running benchmark `.das` files (`--bench`, `run`, `Asserter`) |
| `skills/internal/benchmarks_in_repo.md` | Adding or moving a file under `benchmarks/` — layout, the README index rule, output capture |
| `skills/daspkg.md` | Running daspkg commands, `.das_package` manifests |
| `skills/dynamic_modules.md` | `.das_module` descriptors, `register_native_path`, module resolution |
| `skills/internal/dynamic_modules_plumbing.md` | Adding a module under this repo's `modules/` tree — static-binary resolution, the `ADD_MODULE_*` CMake macros |
| `skills/external_module_debugging.md` | Working on an external daslang module locally (dasImguiNodeEditor, dasCards, …) — run/lint/test from a standalone daslang or MCP before push-to-CI; the junction pattern + `project_root` MCP arg |
| `skills/internal/install_instructions.md` | Updating `install/CLAUDE.md` or `install/skills/` for the shipped SDK |
| `skills/internal/skill_taxonomy.md` | Deciding where a skill lives and whether it ships — the folder taxonomy (root/daslang/internal), one-fact-one-home, and the move/merge rules |
| `skills/internal/writing_skills.md` | Adding a `skills/*.md` file, moving content between skills, or reviewing a skill change — the content craft: claims verified, commands run, the gate mechanics, the review checklist |
| `skills/internal/aot_testing.md` | AOT test files, `test_aot` binary, `Module::aotRequire()`, AOT hash mismatches |
| `skills/tune.md` | The `[tune]` kernel-tuning framework — `[tune_perm]`/`[tune_scope]`/`[tune_policy]`/`--tune`, sidecars, the tune-and-re-exec model, adding a kernel family |
| `skills/internal/llvm_tune_internals.md` | Maintaining `modules/dasLLVM/daslib/llvm_tune.das` itself — macro-context traps, the AOT/`-exe` gate split, the dasLLAMA tuner |
| `skills/internal/visitor_gen_bind.md` | Adding `Visitor` virtual methods / `canVisit*` gates / `gen_bind.das` regen |
| `skills/daslang_live.md` | `daslang-live`, live-reload lifecycle, `[live_command]`, `[before_reload]`/`[after_reload]` |
| `skills/internal/daslang_lsp.md` | Working on `utils/lsp/` (the LSP server) — locked architecture, coordinate conventions, protocol tests |
| `utils/internal/das-herd/dasherder.md` | Running INSIDE a dasHerd-managed agent session (any `DASHERD_SESSION_ID` env var set) — the session cooperation contract: inbox/outbox mailbox, declaring participating repositories, Review Bundles, the `dasherd.ps1` CLI |
| `skills/imgui_ui_debugging.md` | **CRITICAL UI SKILL** — ANY dasImgui UI/interaction bug: reproduce → make it observable in `imgui_snapshot` → fix → prove via snapshot + test. Never claim a UI fix from logic or a screenshot alone |
| `skills/imgui_application.md` | Building any dasImgui application — harness lifecycle (`init`/`update`/`shutdown`, `harness_*` frame calls), the headless arm, heap/GC ownership contract |
| `skills/imgui_migration.md` | Migrating v1 `imgui_boost` code to the v2 boost layer — the v1→v2 mapping table (`imgui_lint` IMGUI002 points here) |
| `skills/internal/imgui_playwright.md` | Writing/editing `modules/dasImgui/tests` playwright tests — the async rule (gate on the effect), the `wait_*` family, one-host-per-9090 |
| `skills/internal/imgui_recording.md` | Recording tutorial videos — `record_*.das` drivers, prepare → record → convert pipeline, `docs-assets` release upload |
| `skills/internal/vulkan_recording.md` | Recording dasVulkan tutorial videos — `record_*.das` drivers under `modules/dasVulkan/tutorials/*/recording/`, the capture → voiceover → mux pipeline, `docs-assets` release upload |
| `skills/internal/environment_variables.md` | Reading or ADDING any environment variable — the full daslang set, the rules a new one must follow, the C++ `env_cfg` rail |
| `skills/perf_lint.md` | Running the lint suite or suppressing a finding — **and before declaring any hot path off-limits to allocation**: `[hot_path]`/`[no_alloc]`/`[no_env]`/`[no_io]`, `[cold_path]`, `@scratch` |
| `skills/style_lint.md` | Suppressing a `STYLE*` finding (the per-module `options` knobs) or applying the STYLE037/038 suppress-vs-split policy |
| `skills/internal/perf_lint_authoring.md` | Adding or changing a rule in `daslib/perf_lint.das` — visitor hooks, the shared enablement policy, fixture + RST registration |
| `skills/internal/style_lint_authoring.md` | Adding or changing a rule in `daslib/style_lint.das` — detection patterns, fixture + RST registration, the facts that live outside the module |
| `skills/daslang/references/strings.md` | Any `.das` string operation — `find`/`replace`/`split`/parsing/`build_string`/`peek_data` |
| `skills/daslang/references/strings.md` | Writing regular expressions in `.das` code |
| `skills/daslang/references/files-and-paths.md` | Writing or reviewing any glob/wildcard pattern handling — file selection, include/exclude masks, pattern-match-on-paths (`*` / `?` / `**` / `[abc]`) |
| `skills/internal/version_update.md` | Bumping the daslang version number |
| `skills/internal/doc_archiving.md` | Archiving a completed arc's design/plan/audit docs into `/history` — the archive-vs-stay test, reference-update discipline, area-index notes, the `history/README.md` ledger |
| `skills/internal/doc_sweep.md` | Running the each-release authored-RST doc sweep, editing `.. das-doc:` markers, or extending `utils/internal/doc-verify/` |
| `skills/jobque_debugging.md` | Channel/LockBox/JobStatus/Feature leaks (`--track-job-status`, `DumpJobQueLeaks`) |
| `skills/memory_leak_detection.md` | Any leak report at exit — master index of the six leak-detection mechanisms and which to reach for |
| `skills/internal/make_pr.md` | Creating a pull request (lint, test, AOT, format checklist) |
| `skills/internal/review_round.md` | Running the multi-agent deep review of a branch/diff (the pre-PR round, or any "review this") |
| `skills/review_md.md` | Creating or editing any module `REVIEW.md`, or reviewing a diff to one — the opening contract, the template, the followability finding classes |
| `skills/tdd_audit.md` | Auditing any diff for test coverage — the branch-test rule: every new/changed reachable branch has a test that fails without it |
| `skills/internal/preflight.md` | Pushing a non-trivial branch or reproducing a red CI lane — maps every PR-triggered CI lane to its exact local mirror command (or an honest "not mirrorable") |
| `skills/internal/abi_break_sweep.md` | Changing public C++ API, AST node layout, or daslib generic signatures that external module repos compile against |
| `skills/internal/wsl_ci_repro.md` | Reproducing a Linux-only CI failure (sanitizers, POSIX divergence, headless timing) in the WSL CI-mirror distro — verbatim-CI recipe and its traps |
| `skills/internal/babysit.md` | Babysitting an open PR through CI failures and Copilot/human review feedback after the PR is created (the post-open loop) |
| `skills/internal/review_triage.md` | Triaging ANY review comment on a PR (Copilot, bot, or human) — the verdict tests, the fix-now vs ledgered disposition, what the user is asked versus told |
| `skills/strudel_port.md` | Porting strudel.cc patterns into daslang |
| `skills/daslang/references/cli-and-config.md` | Writing or editing any tool that parses command-line flags — declarative argv parsing via `daslib/clargs` |
| `skills/daslang/references/json.md` | Reading/writing JSON in `.das` code (`sprint_json`/`sscan_json`, `JV`, manual `JsonValue?`) |
| `skills/xml.md` | XML via `dasPUGIXML`/`PUGIXML_boost` (RAII parsing, builder, XPath, struct round-trip) |
| `skills/sql.md` | Any SQL in `.das` — `daslib/sql_linq` + the dasSQLITE/dasDuckDB/dasPostgreSQL providers, `[sql_table]`, the `_sql(...)` LINQ bridge, migrations |
| `skills/dashv.md` | Any `.das` that serves HTTP, streams SSE, makes outbound requests, or tests a server — `HvWebServer`, the client API, `with_test_server` |
| `skills/daslang/references/files-and-paths.md` | Any `.das` path/filename/filesystem op — must use `fio` helpers, never `rfind`/`slice` |
| `skills/internal/detect_dupe.md` | Duplicate-function detection (corpus build, MCP tools `export_corpus`/`detect_duplicates`, CLI under `utils/detect-dupe/`) |
| `skills/internal/find_dupe.md` | AI-judging a detect-dupe report via Claude (MCP tools `judge_duplicates`/`find_dupe`, CLI under `utils/find-dupe/`); cost guardrails (`--dry-run`, `--max-clusters`, `--positives-only`) |
| `skills/daslang/references/queries.md` | Filter/map/sort/group/aggregate transforms — preference order: comprehension → linq_boost → plain `for`. Avoid `daslib/functional` for new code |
| `skills/decs.md` | Programming with `daslib/decs` / `decs_boost` — entities, components, queries, `[decs_template]`, stages, bulk creation, `from_decs` linq bridge |
| `skills/internal/aot_hash_desync_debugging.md` | `error[50101]: AOT link failed` — semantic-hash desync diagnostics |

Multiple skill files may apply to a single task. For example, creating a new daslib module requires reading `skills/das_formatting.md`, `skills/daslib_modules.md`, and possibly `skills/internal/documentation_rst.md`.

**Formatter reminder:** Use the MCP `format_file` tool to format `.das` files. It calls `daslib/das_source_formatter` directly. Do NOT use `utils/gen1-to-gen2/` (that is the v1→v2 syntax converter — the `gen1_to_gen2` binary — not a code formatter).

### Updating Instructions with New Knowledge

When you discover something new about daslang syntax, semantics, or conventions — whether through compiler errors, user corrections, or experimentation — **update this file** with the new knowledge. If it relates to a specific skill area, update the relevant `skills/*.md` file instead.

**Lint opportunities are reportable findings — don't hold back.** When a bug, review note, or probe reveals a pattern a lint rule could catch (especially one that has bitten more than once), SAY SO in the end-of-turn summary and propose the rule. The lint suite keeps paying for itself precisely because rules keep getting added; candidates go to Boris (rules land in `daslib/*_lint.das`; the authoring rail is `skills/internal/perf_lint_authoring.md` / `skills/internal/style_lint_authoring.md`, and the finding's error text must stand on its own — no per-rule doc catalog exists).

**Syntax and factual corrections are fix-in-place, always.** If a compiler error, probe, or user correction shows that a claim in CLAUDE.md or `skills/*.md` is wrong, incomplete, or stale, fix it in the same session and flag the edit in the end-of-turn summary — never defer it to a proposal. Verify the corrected claim before writing it (grammar truth is `src/parser/ds2_parser.ypp`; behavior truth is a probe-compile with the current binary).

**Rule files carry rules, not history.** CLAUDE.md files, `skills/*.md`, and per-module rule
docs (REVIEW.md-style checklists) state the CURRENT contract only — no incident anecdotes,
no PR/issue numbers, no dated rulings, no "as of" changelog entries. History lives in git,
`/history`, and memory. When a rule changes, replace the old text outright instead of
appending a dated entry; state the timeless WHY, and if the motivating incident is worth
keeping, archive it in `/history`. (Carve-out: `probe-verified <date>` tags on syntax/behavior
claims are verification provenance, not history — they stay.)

**Every REVIEW.md reviews itself.** The contract all checklists share — diff-checkable
criteria only, the self-review rule (a rule a reviewer cannot apply as written is a defect
of the checklist, marked like any other finding), the Form hard limits — lives ONCE in
`REVIEW_COMMON.md` at the repo root; every checklist opens with a pointer to it from its
first commit, never a restatement (`skills/review_md.md` carries the opening template). When
a checklist audit runs for a PR, its scope is every REVIEW.md discovered from the changed set
(the `skills/internal/make_pr.md` step-0a walk, tool: `utils/internal/review-md/main.das`): each discovered
checklist is itself audited under the self-review rule, not
just applied. The walk tool first EXECUTES each discovered checklist's `REVIEW.das` gate
(the mechanical half — fail-fix like dastest, no agents until green; contract in
`REVIEW_COMMON.md`). The implementer for the prose half is the shared **`review-md-auditor` agent**
(`.claude/agents/review-md-auditor.md`): one instance owns exactly one checklist, so with
several in scope the ORCHESTRATOR launches one instance per checklist in parallel and merges
the findings into one report. Self-review includes **followability** — the finding classes
(dense multi-clause rules, enumerations standing in for criteria, mechanism prose,
structurally homeless rules, overlap) are in `skills/review_md.md`; checklists stay slim
because every round applies them, not because someone notices. Beside the checklist auditors
runs the **`tdd-auditor` agent** (`.claude/agents/tdd-auditor.md`): ONE instance for the
whole diff, auditing the constitutional branch-test clause (`skills/tdd_audit.md`) —
including in folders no REVIEW.md covers. (The agent registry snapshots at session
start — a freshly pulled or edited definition is live in the NEXT session, not the current
one.)

**Doc improvements at stopping points.** Propose-first applies only to what's left: restructuring, removing existing guidance, **or proposing a new skill file when you see a recurring pattern that no existing skill covers**. Doc edits direct future Claude behavior, so structural diffs still get review — but factual drift must be self-healing, not queued behind it.

### Writing a new skill

Read `skills/internal/writing_skills.md` first — it carries the full checklist. The three things that matter most:

1. **The folder IS the audience decision.** `skills/*.md` ships with the SDK — where `src/`, `tests/`, `benchmarks/`, `doc/source/` and `modules/*/src` **do not exist**; `skills/internal/` never ships; `skills/daslang/` is the standalone language skill. Decide before writing (`skills/internal/skill_taxonomy.md`); a file that mixes SDK-usable content with repo plumbing serves neither audience.
2. **Never fix an audience mismatch by shipping `src/` or `tests/`.** Mark the line `repo-only` instead (works on a line, or on a heading to cover a whole section). `python3 ci/check_shipped_skills.py <bundle>` enforces this per-PR via `ci/smoke_test_bundle.sh`; it also catches `bin/Release/…` paths, `daslang.exe` invocations, machine-local paths, dead relative links, references to skills the bundle doesn't carry, and `skills/daslang/` purity (no marker escape there).
3. **Register it.** Every skill gets a row in the top-level `CLAUDE.md` table; a shipped skill additionally gets one in `install/CLAUDE.md` (the gate fails the bundle without it). A skill with no trigger row is a skill nobody opens.

## daslang Language

Language, runtime and stdlib truth lives in **`skills/daslang/`** — `SKILL.md` plus thirteen
references (`types`, `functions`, `structs-and-classes`, `closures`, `memory`, `generics`,
`macros`, `modules-and-stdlib`, `strings`, `files-and-paths`, `json`, `queries`,
`cli-and-config`). Read the one that covers what you are about to write or review, before you
write it. Compiler internals no user-facing doc carries — the container collect banner, the
`/*option*/` policy marker, known mangling defects — are in
`skills/internal/daslang_internals.md`.

What follows is only what the bundle deliberately does not carry: the shapes that fail
silently, and this repo's house policy.

### Fails silently

Each of these compiles clean and then gives a wrong answer or corrupts memory, with no
diagnostic in any tier.

- **A continuation line starting `+` or `-` is its own statement**, and unary plus is pure, so
  the optimizer deletes it: `let x = a` ⏎ `+ b` ⏎ `+ c` leaves `x == a`. Always wrap a
  multi-line arithmetic RHS in `(...)`.
- **Stripping `const` in order to write.** The original const type already licensed
  readonly / `noalias` / DCE, so the write vanishes in interp, JIT and AOT alike. Declare the
  parameter `var T?`; never `reinterpret` the const away.
- **`delete` on `array<T?>` frees the POINTEES**, not just the buffer. On borrowed pointers the
  interpreter reports `deleting <ptr>, which is not a chunk pointer`, while Release+JIT corrupts
  the heap silently and crashes later at an unrelated allocation. `clear()` the container first,
  or mark the field `@do_not_delete`.
- **A lambda copy aliases one capture frame** — deleting a container that holds two copies of
  the same lambda is a double free.
- **`:=` on a string copies the pointer** unless the source is a temp (`#`) string or the
  program declares `options multiple_contexts`. Across a context boundary that is a dangling
  read; the reliable copy is `clone_string(s)`.
- **`is` / `as` on handled and AST types is EXACT-type**: `expr is ExprField` is false when
  `expr` is an `ExprSafeField`, and `as` on the wrong type crashes. Enumerate every concrete
  type, or use a matcher.
- **`tab[k]` on a mutable table INSERTS on read.** Read with `tab?[k] ?? d`.
- **`to_int` / `to_float` return `0` on garbage** (`to_int("12abc")` is `12`). Anything from a
  user, a file, or the environment goes through `try_to_int` / `try_to_float`.
- **`<-` into a table slot ZEROES the source.** Registering one value under two keys takes a
  fresh clone per registration, not the same variable passed twice.
- **`new WithCtor(field = v)` skips the user constructor** — it is plain field-init, so
  inherited fields stay zero. Write `new WithCtor(args)` when the constructor must run.

### Code style — prefer idiomatic forms

| Don't write | Write instead | Why |
|---|---|---|
| `string(x.__rtti) == "ExprFoo"` | `x is ExprFoo` | `is` works directly on AST pointers |
| `get_ptr(x) == null` / `get_ptr(x).field` | `x == null` / `x.field` | AST pointers auto-dereference |
| `string(das_str) == "lit"`, `!empty(string(das_str))` | drop the `string(...)` cast | `das_string` compares with `string` directly; `empty()` works on it |
| `let v = string(x.name); $i(v)` / `var copy = val; $v(copy)` | `$i(x.name)` / `$v(val)` | qmacro tags accept `das_string`, `let` vars, loop vars directly |
| N qmacro arms differing only in the call target | `let fname = ...; qmacro($c(fname)(…))` | `$c(stringVar)` splices a function name. `_::$c(…)` is a parse error — drop `_::` |
| `if (true) { ... }` | `{ ... }` | bare blocks create lexical scope in gen2 |
| `var inscope r <- expr; return <- r` | `return <- expr` | |
| `unsafe { (reinterpret<ExprBlock?>(blk)).list }` / `unsafe(reinterpret<T?>(x))` | make param `var` + plain `x.list` | `var` param gives non-const field access without reinterpret |
| `if (cond) { return X }` (or `{ break }` / `{ continue }`) | `if (cond) return X` or postfix `return X if (cond)` | STYLE005 |
| `for (i in range(length(arr))) { ... arr[i] ... }` where `i` is used only as `arr[i]` | `for (c in arr) { ... c ... }` | PERF018 |
| `from_JV(v, type<int>, 13)` | `v ?? 13` | STYLE020 |
| per-key `insert("k1", JV(v1))` into `table<string; JsonValue?>` | `var args = JV((k1=v1, k2=v2))` | STYLE021 |
| `int(BfT.a) \| int(BfT.b)` (same bitfield, or enum with `operator \|`) | `int(BfT.a \| BfT.b)` | PERF019 |
| `int64(a)` where `a : int64` (any same-type workhorse cast) | `a` | PERF020 |
| `foo \|= BfT.m` / `foo &= ~BfT.m` (single named bit) | `foo.m = true` / `foo.m = false` | STYLE022 |
| `uint(bf & BfT.m) != 0u` / `int(bf & BfT.m) == 0` (single named bit) | `bf.m` / `!bf.m` | STYLE023 |
| `unsafe(x + y)` / `unsafe { ... }` where nothing inside requires unsafe | drop the wrap | STYLE024 |
| `unsafe { stmt1; stmt2; stmt_needing_unsafe }` (only ONE needs it) | `stmt1; stmt2; unsafe(<sub-expr>)` | STYLE025 |
| `unsafe { }` nested inside another `unsafe { }` | drop the inner wrap | STYLE026. Closure/lambda/generator bodies DO need their own wrap — the outer one does not propagate into them |
| `for (s in A) { B \|> push(s) }` / `push_clone(s)` (iter-var only) | `B \|> push_from(A)` / `push_clone_from(A)` | PERF022: reserves the combined capacity once |
| a for-loop that only pushes/inserts into a fresh-empty array/table | a comprehension: `var a <- [for (x in SRC); EXPR; where COND]` / `\{for (...); k => v; ...\}` | STYLE027. `emplace` loops stay loops — move semantics differ |
| ≥ 2 `insert`s / `t[k] = v` right after an empty table decl | a literal: `var t <- { k1 => v1, k2 => v2 }` (set: `{ k1, k2 }`) | STYLE031. Duplicate const keys: inserts overwrite, the literal is a compile error — don't rewrite those |
| `var w : array<T>; w \|> push_from(SRC)` (fresh-empty target) | `var w := SRC`; if then returned, `return clone_to_move(SRC)` | STYLE032 + PERF009. NOT `return <- src` — that would move out of the source |
| `var X = clone_expression(E); ... $e(X) ...` (only splice uses) | inline `$e(E)` at each splice site | PERF023: the template machinery clones every substitution already |
| hand-rolled `is X` / `as X` / null-guard ladders in macro code | `qmatch(...)` for source-syntax shapes; `match (e) { if (ExprField(...)) }` for node-class shapes | division of labor: `skills/das_macros.md` |
| `unsafe(reinterpret<T?>(unsafe(addr(x))))` (pointer target) | `unsafe(addr<T?>(x))` | STYLE034 |
| `b == T('(')` where `b` is a non-`int` numeric scalar | make `b : int`, compare `b == '('` | STYLE035: character literals are `int` |
| `dst := src` / `push_clone(src)` on a plain `string` | `dst = src` / `push(src)`; cross-context copy = `clone_string(src)` | LINT016: the clone spelling copies only the pointer |
| `int64(length(x))` / `uint64(capacity(x))` (also `count`, `find_index`, `fread`, `fwrite`) | `long_length(x)` / `long_capacity(x)` / … | LINT017: the inner call returns `int` — 2^31 is hit before the cast |
| `memcpy(d, s, int(n))` / `arr \|> resize(int(n64))` with a 64-bit `n` | `memcpy(d, s, n)` / `resize(n64)` — the 64-bit overloads exist | LINT018 |
| `range(int(n64))` / `urange(uint(u64))` | `range64(n64)` / `urange64(u64)` | LINT020. Vector components and `string` index stay 32-bit |
| a 64-bit local whose every use sits under `int(...)` | narrow once at the declaration, or lift the sinks to 64-bit | LINT021 |
| `def f(var why : string)` written on a path but never read | `var why : string&` — a by-value copy's write never reaches the caller | LINT023 |
| bare `resize(need)` on an input-scaled buffer (frames, pixels, vocab) | declare it `@exact_size`; then `reserve`/`ensure_capacity` before every `resize` (or size it through `reserve_resize`-style helpers) | PERF032 — the annotation is a lint contract; the guard panics only when the big input arrives |
| `-const` `-&` `-[]` `-#` `==const` `==&` on a **concrete** cast target | drop the contract | STYLE036: substitution contracts act only while a generic binds — inert on concrete targets |
| `slice(s, i, j)` / `chop(s, i, n)` in a loop over an outer string | `peek_data(s) $(d)` and slice the view | PERF031: each call re-strlens the whole source — O(n²); every haystack op has a byte-view twin |

For path/filename ops use `fio` helpers (`base_name`/`dir_name`/`path_join`/etc.) — see `skills/daslang/references/files-and-paths.md`. Never hand-roll `rfind("/")` / slice — misses Windows separators.

**Inline literals over temp-var-and-push:** for a short array consumed in one expression, write `stack([a, b, c])` rather than `var xs : array<T>; xs |> emplace(a); xs |> emplace(b); stack(xs)`. Faster interpreted and easier to read; same for table literals and other bracketed constructors. Threshold: while it stays readable.

**`print` is for user-facing scripts only.** In `tests/`, `daslib/`, `utils/` use `to_log(LOG_INFO|LOG_WARNING|LOG_ERROR)` — same stdout, but level-tagged and filterable. Canonical example: `utils/detect-dupe/main.das`.

**Minimize `unsafe`:** Most `unsafe(reinterpret<T?>)` in macro code exists to strip `const` from raw-pointer field access. Fix the root cause: make the function parameter `var` so field access returns non-const pointers. Reserve `unsafe` for genuinely unsafe operations (pointer arithmetic, `reinterpret` across unrelated types).

**Complexity/length lint (STYLE037/STYLE038): new code meets both limits from the start.** On a hit in existing code, the suppress-vs-split resolution policy is `skills/style_lint.md` — never force a split on an honest shape.

**Comment and style hygiene.** Do not narrate code with comments — the test for every comment: does it add information the reader would otherwise have to go look for? 1–2 lines preferred, 3 the enforced cap (STYLE014 fires at 4+). No banner comments above documented functions, no multi-paragraph architectural preambles, no `//!` docstrings on private symbols; when in doubt, delete. The full rulebook — comments, naming, and code shape, language-agnostic — is **`skills/comment_style_hygiene.md`**; the `style-hygiene-auditor` agent applies it to every PR's new code (mandatory run in `skills/internal/make_pr.md`, findings persuade rather than block).

## Key Directories

Most layout is obvious from `ls`. Non-obvious ones worth knowing:

- `skills/daslang/` — the **distributable, SDK-free daslang language skill** for third-party AI agents (Claude-skill format: `SKILL.md` + `references/`), NOT a repo task skill. Every example is probe-verified; its `README.md` carries the editing rules. When grammar/stdlib/defaults change, update it in the same arc — it replaces a scraped bundle that rotted precisely because nobody owned it

- `daslib/aot_cpp.das` — the AOT C++ emitter lives here, NOT in C++
- `tests/aot/CMakeLists.txt` — register new test directories here for AOT compilation. Two AOT binaries: `test_aot_subset` (tests/language only, in ALL — the per-PR CI compile gate) and full `test_aot` (`EXCLUDE_FROM_ALL`, ~1080 AOT TUs — nightly CI + `preflight --full` only, via `--target test_aot`/`run_tests_aot`)
- `dastest/` — test framework (used by both `tests/` and external repos)
- `utils/detect-dupe/` (in-repo dupe finder) and `utils/find-dupe/` (Claude-based judge that needs `daspkg install --root utils/find-dupe` + `ANTHROPIC_API_KEY`) — both also exposed as MCP tools
- `utils/mcp/`, `utils/daslang-live/`, `utils/daspkg/` — in-tree tools (most also have skills under `skills/`)
- `tutorials/language/` (language tour) vs `tutorials/<area>/` (per-area) — never put tutorials in `modules/<X>/tutorial/`
- **Array/dim/vector indexing lives across 5 tiers** — bug fixes (bounds checks, neg-index detection, width-aware bounds) usually need parallel edits in all of them: AOT C++ ([include/daScript/simulate/aot.h](include/daScript/simulate/aot.h)), interpreter non-fused ([include/daScript/simulate/runtime_array.h](include/daScript/simulate/runtime_array.h) + [include/daScript/simulate/simulate_nodes.h](include/daScript/simulate/simulate_nodes.h)), interpreter fused ([src/simulate/simulate_fusion_at_array.cpp](src/simulate/simulate_fusion_at_array.cpp) + [src/simulate/simulate_fusion_at.cpp](src/simulate/simulate_fusion_at.cpp)), JIT ([modules/dasLLVM/daslib/llvm_jit.das](modules/dasLLVM/daslib/llvm_jit.das)), and AST const-fold ([src/ast/ast_simulate.cpp](src/ast/ast_simulate.cpp)). Debug builds bypass the fused permutations — a fix that lands only in the fused path passes Release CI and trips Debug-ARM. Bump `LLVM_JIT_CODEGEN_VERSION` in `modules/dasLLVM/daslib/llvm_jit_run.das` after JIT emitter/ABI changes to invalidate cached `.dll`s (the cache hash also folds per-function AOT hashes, so AST-level changes self-invalidate without a bump)

## MCP Server (AI Tool Integration)

The daslang MCP server (`utils/mcp/main.das`) exposes compiler diagnostics, program introspection, and live-reload control. **Prefer MCP tools** over manual compilation and grep — `grep_usage` is parse-aware (tree-sitter), `find_references` resolves cross-module symbols, and `live_*` tools talk to `daslang-live` directly instead of curl.

**Fresh worktree/clone with no daslang MCP tools?** `.mcp.json`, `sgconfig.yml`, `bin/`, and the tree-sitter grammar lib are all gitignored, so a `git worktree add` (or clone) starts with zero daslang tools. Bootstrap it from any existing daslang binary: `daslang utils/mcp/setup.das -- --root <worktree>` builds a worktree-local binary (+ grammar), copies the platform `sgconfig.yml`, and merges a `daslang` entry into `.mcp.json` (writes no secrets, preserves `github` etc.). `--no-build` wires config to an already-built binary. Restart the session in the worktree to pick it up. Details: **`skills/mcp_tools.md`**.

**Adding a file under `utils/mcp/` that the server requires? Add it to the `install(FILES …)` block in `CMakeLists.txt` in the same commit.** The installed SDK's MCP server runs with no `-project`, so it must be self-contained: every `.das` reachable from `main.das` has to be installed, or the SDK's server dies at startup on `error[20605] missing prerequisite '<mod>'` while the in-tree server keeps working. `tools/` and `subtools/` are globbed, but the top-level files are listed one by one — that list is the trap.

Full tool table (including `detect_duplicates`/`judge_duplicates`/`find_dupe`), live-API caveats, and `.mcp.json` configuration: **`skills/mcp_tools.md`**.
