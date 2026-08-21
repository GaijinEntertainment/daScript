# Pre-PR Checklist

Complete every step in order; fix a failure before proceeding.

**The mechanical gates are one command:** `daslang utils/internal/make-pr/main.das --` runs
sync → review-md walk → dupes → ast-verify → jit-smoke (the last two auto-skip when the diff
has no macro/AST or JIT surface), then chains `preflight --full` (`skills/internal/preflight.md`
maps each gate to its CI lane). `--only <gate>` reruns one; `--no-preflight` skips the chain; exit 2 names the red gate. This
file is the authority on fix policy and on what the tool prints as STILL YOURS. Commit, run
it, push once (one batched PR).

**The full preflight runs ONCE per PR — never a second full run.** On failure fix everything,
validate each fix with the **targeted** gate or an isolated repro (`--only <gate>`, the
failing test slice, a scratch probe — minutes, not tens of minutes), say so in your summary,
and let CI validate the tip. Later fix commits — its own findings, Copilot/CI rounds
(`skills/internal/babysit.md`) — do NOT re-trigger it.

## The checklist

| Step | Gate / tool | Fix policy |
|---|---|---|
| 0 Sync | make-pr `sync` | Red = behind origin/master: rebase (never onto local `master`), re-run. A listed PR-set file you did not edit = the rebase went wrong; a conflict on a file also changed upstream keeps origin/master's. Squash only AFTER the rebase — `git reset --soft master` on a stale `master` bakes other PRs in; already pushed: rebase + `git push --force-with-lease`. Re-read any `skills/*.md` / `REVIEW*.md` the rebase changed |
| 0 Untracked | preflight `untracked` gate | Empty at PR time — commit, delete, or ignore each (`.gitignore`; `.git/info/exclude` for box-local) |
| 0a REVIEW audit | make-pr `review-md` | Red = a discovered `REVIEW.das` gate failed — fail-fix, no agents until green. Then one `review-md-auditor` per checklist. Discovered rules bind on top of this file; checklist defects fixed in the same batch |
| 0a TDD audit | one `tdd-auditor` over the whole diff, REVIEW.md folders or not (`skills/tdd_audit.md`) | UNTESTED branch → test in the same change, never a follow-up promise. UNPROVEN → run its named settling gate or state the claim in the PR body. RETUNED/WEAKENED test edit → restore the expectation/instrument or state the reason |
| 0a2 Style hygiene | `style-hygiene-auditor` (`skills/comment_style_hygiene.md`) | Mandatory run, non-blocking findings: fix each or consciously decline it |
| 0a3 Woodpecker | external codex round (`skills/internal/woodpecker.md`), background Bash at final branch shape; keep working the checklist while it runs | Every arc, trivial or not; non-trivial arcs re-round on the fixed tip. Verify each finding; harvest before step 6 |
| 0b Build drift | nuke `build/` only on the three symptoms below | No proactive clean build. Never run full preflight on a Debug host |
| 1 Lint | preflight lint gate (debug one file: MCP `lint`) | **Zero warnings** — CI runs the same utility on the changed set. Fix it (idiom table in `CLAUDE.md`), or `// nolint:CODE` **with the reason**, for a known false positive only (handled types like `xml_node` need `var`) |
| 1.5 Dupes | make-pr `dupes` (scoping + report; modes: `skills/internal/detect_dupe.md`) | Triage is yours: reuse an exact match, justify the sibling, or extract a helper. Widen the corpus by `daslib` for a new generic helper. Skip for tests/fixtures/generated-only PRs |
| 1.6 AST verify | make-pr `ast-verify` | **Zero** `AST verify` lines and no crash — the tree is clean tree-wide, so any report is a bug in what built the node. A compile error is not a failure (many tests assert one). Plain `--ast-verify` on ONE file names the pass that broke it (`skills/das_macros.md`) |
| 1.7 Workarounds | `git diff origin/master..HEAD` — read every changed file | A smell (below) is a STOP-and-decide: surface fix-vs-workaround and **ask the user** |
| 2 Tests | preflight tests gate (debug one file: MCP `run_test`) | Failures (assertions) and errors (compilation) both count. Fix yours and obvious pre-existing ones; **ask the user** about non-obvious — never call one pre-existing without checking the affected `tests/dasX/` against master's count. Changed `modules/X/daslib/`? Run that module's tests even if your build disables it (CI enables all of `ci/release_modules.txt`) |
| 2.5 JIT smoke | make-pr `jit-smoke` | Empty verifier output = pass. Widen to `tests/soa/test_soa_basic.das` + `tests/language/typeAlias.das` for generic-instance or capture-frame changes. Windows `clang-cl` "program not executable" at the `.dll` link is linker discovery, not codegen — ignore it; end-to-end needs WSL (`skills/internal/wsl_ci_repro.md`) |
| 2.7 Type system | sequence smoke (commands in `skills/internal/preflight.md`) + externals sweep (`skills/internal/abi_break_sweep.md`) | Only when the PR changes the type system, generic binding rules, AST node layout, or widely-instantiated daslib generics (`builtin.das`, `safe_addr.das`, …); neither gate overlaps the test suite |
| 3 AOT build | kill by path first (below), then `cmake --build build --config Release --target test_aot -j 64 -- /nodeReuse:false` with `timeout: 0` (2-25 min) | Doesn't build: register new test directories in `tests/aot/CMakeLists.txt` (`skills/internal/aot_testing.md`); `error[50101]` is a hash desync (`skills/internal/aot_hash_desync_debugging.md`) |
| 3 AOT tests | `bin/Release/test_aot.exe -use-aot dastest/dastest.das -- --use-aot --color --failures-only --timeout 1800 --test tests` (the `-use-aot` / `--use-aot` doubling matches CI) | Same triage as step 2. PR CI builds only `test_aot_subset` — this run and the nightly cron are the only full-AOT checks |
| 4 Docs | see below | Skip when the PR only changes examples, tests, or non-public code |
| 5 Format | MCP `format_file` on all changed `.das` in ONE batched call | Only files in the PR, every era — it handles gen1 and `.das_project`, and CI fails on unformatted gen1; comment stripping follows the folder's `.lint_config` `[format]` policy (teaching folders opt out, swept trees opt in — no per-call flag). Verify they still compile. CI's `utils/das-fmt/dasfmt.das -- --path ./ --verify` wraps the same engine and the same policy |
| 5 `.md` stop | `git diff --name-only origin/master..HEAD \| grep '\.md$'` | Any match: STOP, list the changes, ask the user to review BEFORE push |
| 6 PR | GitHub MCP `create_pull_request` or `gh pr create` | Body follows the two-layer template below. On a squashed branch every later fix is `git commit --amend --no-edit` + force-push, never a new commit |
| 6a Babysit | continue into `skills/internal/babysit.md`; triage every comment per `skills/internal/review_triage.md` | Creating the PR does not end the workflow; the stop rule and merge gate are babysit §0's |
| 7 Post-land sweep | `git ls-files --others --exclude-standard` | Babysit rounds mint debris after the `untracked` gate — sweep it per Workspace Hygiene in `CLAUDE.md` (probe scripts, dumps, `__pycache__`, ad-hoc logs, typically `_`-prefixed) |

## 0a-0a3. Agent topology

**0a** — ONE `review-md-auditor` (`.claude/agents/review-md-auditor.md`) per discovered checklist, each auditing only its own under the self-review rule, plus ONE `tdd-auditor` (`.claude/agents/tdd-auditor.md`) for the whole diff; merge the reports. **Registry caveat: agent definitions snapshot at session start — a just-pulled or just-edited definition only exists in the NEXT session.** A non-trivial change runs the full round (`skills/internal/review_round.md`) on top; these instances are its surfacing phase, not a repeat.

**0a2** — `style-hygiene-auditor` (`.claude/agents/style-hygiene-auditor.md`): one instance over the whole changed set for a small diff, one per file cluster (directory or language) for a large one, reports merged. Same registry caveat.

**0a3** — no agent: the damper, the verify-before-believing loop, and the no-codex-on-PATH fallback are all in `skills/internal/woodpecker.md`.

## 0b. Build-config drift

**No per-PR clean-build step** — `skills/internal/build_and_debug.md` repairs every other build failure. Nuke and reconfigure only on these three symptoms:

- **MSVC `LNK2038` `/MT` vs `/MD`** (canonically `dasModuleHV`) — a cache var (e.g. `DAS_USE_STATIC_STD_LIBS`) toggled across configures leaves `.vcxproj`s on one CRT and the cache on another.
- **Stale `ExternalProject` byproducts** — libhv's `hv_static.lib` freezes at the CRT it first built with.
- **Build/link errors that survive a full rebuild** of the affected target and vanish in a fresh build dir.

```bash
rm -rf build                                            # sanctioned only on the symptoms above
# Set DASLANG_OPENSSL_DIR first, or this rebuilds OpenSSL from source (build_and_debug.md)
cmake -S . -B build -DDAS_HV_DISABLED=OFF -DDAS_LLVM_DISABLED=OFF -DDAS_AUDIO_DISABLED=OFF \
  -DDAS_PUGIXML_DISABLED=OFF -DDAS_SQLITE_DISABLED=OFF -DDAS_GLFW_DISABLED=OFF   # ci/release_modules.txt
cmake --build build --config Release -j 64              # timeout: 0
```

After a rebase pulling in C++ changes, rebuild incrementally before trusting test or AOT results — a stale `bin/test_aot` fails AOT where interp passes.

**Never run full preflight with a Debug host** — preflight rejects Debug before starting. When `LNK1104` on `bin/Release/libDaScriptDyn_runtime.dll` blocks the Release build, terminate the MCP host holding it (its watcher restarts it) and retry — never fall back to Debug to spare the host.

## 1.7. Workaround smells

Read the whole diff: *is this change compensating for something that should already work?*

- **Redundant step** — a close/reset/refresh after an action that already does it; a `sleep`/retry to line timing up.
- **Divergence compensation** — a synthetic or programmatic path behaves differently from the real user action and you papered over the gap. **Invariant: synthetic input MUST equal real input, side effects included** — synth mouse == real mouse, injected / L2 click == real click, remote value-set == real edit. The divergence is a framework bug to fix at the source layer, never route around in app, test, or recording.
- **Special-case branch** — a button for an action that already has a key; an injection rail because "the gesture doesn't fire"; a flag toggled only to dodge a misbehavior.
- **Copied-from-here justification** — "the codebase already does it this way" is not validation; in AI-assisted code you may be copying a *past* workaround. Validate against upstream semantics, not local precedent.

The call is the user's: name the divergence, offer (1) fix at source vs (2) keep the workaround, **ask before shipping the compensating code**. After a root-cause fix in a dependency, note the now-redundant compensations elsewhere.

## 3. Kill by path, never by image name

`taskkill /IM daslang.exe` kills every daslang on the box, including the dasHerd watcher that owns other sessions' PTYs — they die with a silent exit 1 and no log.

```powershell
# Only processes running from THIS tree
Get-Process daslang,daslang-live,mcp -ErrorAction SilentlyContinue |
  Where-Object { $_.Path -like "$(Get-Location)\*" } | Stop-Process -Force
```

## 4. Documentation

Run when any changed: `daslib/*.das` publics (added, removed, renamed, re-signed) or their `//!` comments; C++ bindings in `modules/*/src/*.cpp` or `src/builtin/*.cpp` adding public functions, types, or fields; **fields or enum values added, REMOVED, or reordered** in a C++ type documented under `doc/source/stdlib/handmade/` — das2rst validates those **positionally** (line 1 = type description, line N+1 = Nth field/value) — a removal is as CI-fatal as an addition; RST under `doc/source/`; `das2rst.das` or `daslib/rst.das` / `rst_comment.das`.

Substeps by what changed: handwritten RST only → **4f, 4g** (das2rst regenerates daslib, not handwritten pages); `//!` comments / new daslib publics / C++ bindings → **4a-4g**; both, or `das2rst.das` / `rst.das` itself → **all**. The five CI doc gates: `skills/internal/preflight.md`.

**4a** group the new publics (`group_by_regex` in `das2rst.das`) → **4b** `bin/Release/daslang.exe -documentation doc/reflections/das2rst.das` → **4c** fill every `// stub` under `doc/source/stdlib/handmade/` → **4d** re-run 4b → **4e** `grep -c Uncategorized doc/source/stdlib/generated/*.rst | grep -v ':0$'` must be empty → **4f** delete `doc/sphinx-build`, then `sphinx-build -b html -d doc/sphinx-build doc/source site/doc` → **4g** stage it all. Mechanics: `skills/internal/documentation_rst.md`. On top of them:

- das2rst **stops at the FIRST validation panic** — one CI round hides N−1 issues, so loop 4b locally until clean.
- CI runs `sphinx-build -W`: **any** warning fails, including classes only handwritten RST produces — never skip 4f. Two non-obvious ones: a title's over/underline must be at least as long as the title in SOURCE characters (backticks and inline-code spans count); a page not in a `.. toctree::` warns "document not in any toctree".
- 4g stages every changed and new file under `doc/` and `doc/reflections/` (amend on a squashed branch): `git ls-files --others --exclude-standard doc/source/stdlib/` must be empty — CI fails on untracked generated files.

## 6. The PR body — two layers, one artifact

**Top — the reviewer document.** Plain prose, no headers, ESL-plain (short sentences, common words, no idioms, one clause per thought).

- **Line 1 = consumer impact, bold** whenever there is any — ABI break, required rebuild, behavior change; never buried in a bullet.
- One paragraph per topic, ~150 words total, the largest arcs ~300.
- Never narrate machinery (agents, review rounds, gate lists) — state conclusions; a finding's origin is at most a tag ("negative-controlled").

**Fold — the ledger.** One `<details>` block. Heading names are FIXED — later sessions grep for them. Omit an empty section; never rename one.

- `### Validation` — exceptions only; a full green run is not news, CI re-proves it. Report (a) evidence CI cannot produce — a local-only gate (full AOT sweep, external shared_module rebuild after an ABI break, a WSL/platform repro) — and (b) deviations: a gate skipped, a partial result, a known-red cell with its control, each with its reason. Numbers only where a result is partial.
- `### Claims — stated, not tested` — every UNPROVEN claim (the TDD audit's state-it-in-the-PR resolution): the claim, how it was verified instead, what a break would look like.
- `### Not done` — deliberate omissions, residuals, ledgered follow-ups.

```markdown
**ABI break: <impact> — <who> must <do what>.**   ← only when true

<problem → change → why, 1–3 paragraphs>

Where to look: <entry points, risky spots>.

<details>
<summary>Validation, claims, ledger</summary>

### Validation
- …

### Claims — stated, not tested
- …

### Not done
- …

</details>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
```

**`#N` is GitHub reference syntax, never ledger numbering.** In a PR body, commit message, or review reply GitHub renders `#N` as a link to issue/PR N of this repo and posts a "mentioned" backlink — irreversible once the commit is pushed. Cite an internal ledger entry (`followup_*.md`, `PERF_LEDGER.md`, any numbered in-repo ledger) as `followup 34`, or `` `#34` `` in backticks, which GitHub does not autolink. Bare hash = real issues and PRs only. Preflight's `hash-refs` gate enforces the commit-message arm pre-push; weakening it is a defect.
