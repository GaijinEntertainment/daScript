# find_dupes — duplicate-function detection

Read this skill before doing duplicate-detection work in this repo, or before editing the tool itself. Two audiences are covered:

- **Using the tool** — building a corpus, asking "does this function already exist?", wiring a CI gate. Lead sections.
- **Extending the tool** — adding patterns, modifying the canonicalizer, wiring new MCP parameters, refactoring helpers. See [Maintainer notes](#maintainer-notes) at the bottom.

The full user-facing reference lives at `doc/source/reference/utils/find_dupes.rst`; this skill is the operational guide.

## When to use this

- A function you're writing feels familiar — you suspect it already exists somewhere in `daslib/`, `tests/`, or `utils/`.
- You're reviewing a PR and want a "is this a copy-paste of an existing helper?" check on the diff.
- You want a CI gate that flags new structural duplicates introduced by a PR (B1 baseline mode).

What it finds: functions whose **canonical token stream** (identifiers, types, and literals all alpha-renamed and collapsed) is byte-identical or close. It is intentionally a structural matcher — `add(a, b : int)` and `add(a, b : float)` look identical to it; that's the point.

What it does **not** find: semantic duplicates with different control flow, different called helpers, or significantly different shape. Two implementations of "the same idea" written from scratch will not match.

## Workflow via MCP (preferred)

The MCP server exposes the entire pipeline — no shelling out:

| MCP tool | Purpose |
|---|---|
| `export_corpus` | Scan paths/dirs/globs, compile each `.das` file, write a `corpus.json` |
| `find_duplicates` | Compare candidate file(s) against a `corpus.json`, return per-candidate matches |

### Step 1 — build a corpus

```
mcp__daslang__export_corpus(paths="daslib,utils,tests", out="corpus.json")
```

Build the corpus once over the body of code you want to compare against. Re-run when the code drifts enough that stale matches become a problem.

Envelope:

```json
{ "out": "...", "files_scanned": 86, "files_skipped": 0, "records_written": 612 }
```

`files_skipped` counts `.das` files containing `expect ` directives — intentionally-broken fixtures, not part of the corpus.

### Step 2 — query against a candidate

```
mcp__daslang__find_duplicates(
  paths="path/to/new_helper.das",
  corpus="corpus.json"
)
```

Pass `paths` as comma- or newline-delimited (the latter lets you pipe `git diff --name-only` through). Records in the corpus whose `file` matches a `paths` entry are dropped first, then the candidate is freshly compiled — so the file is compared against the rest of the world, never against its own stale copy in the corpus.

Envelope:

```json
{
  "corpus_records": 612,
  "corpus_dropped_for_override": 4,
  "candidate_files_compiled": 1,
  "candidate_files_failed": 0,
  "candidate_functions": 7,
  "candidate_functions_pre_filter": 7,
  "exact_clusters_kept": 1,
  "fuzzy_pairs_kept": 2,
  "patterns_skipped": {},
  "report": { "candidates": [ ... ] }
}
```

`report.candidates[]` is the actionable output: each candidate with its top-N `exact_matches` (similarity 1.0) and `fuzzy_matches` (similarity in `[threshold, 1)`), each with `file`, `line`, `name`, `is_lambda`.

### Reading the envelope

| Field | What it tells you |
|---|---|
| `candidate_functions_pre_filter` | How many candidates the file had before pattern filtering |
| `candidate_functions` | How many survived the filter and were clustered |
| `patterns_skipped` | Per-pattern count of records dropped (corpus + candidates). `{}` means the filter was a no-op |
| `exact_clusters_kept` / `fuzzy_pairs_kept` | Aggregate match counts after the candidate filter |

When `candidate_functions == 0` but `candidate_functions_pre_filter > 0`, the filter consumed every candidate. The report is empty by design; if you want those records anyway, retry with `keep="all"` (or with a specific name from `patterns_skipped`).

## Pattern filter

A "pattern" is a structural shape that signals "this function is boilerplate, not real code" — a wrapper or dispatcher whose canonical token stream contains zero unique signal beyond its repetition count. Pattern-matched functions are dropped from clustering by default.

Currently shipped:

| Name | Detects | Why it's boilerplate |
|---|---|---|
| `dispatch` | Body is N >= 2 byte-identical top-level statement chunks | dastest's `t \|> run("X") @(t) {…}` lists, `t \|> bench(…)` lists, repeated-init blocks. Lambda bodies collapse to `ADDR` upstream, so two `run` calls look identical regardless of what the lambdas do |

Override per-pattern with `keep="<name>"` (comma-separated for multiple), or disable filtering wholesale with `keep="all"`. Default (omit `keep`) skips every known pattern.

The filter applies to **both** corpus records and freshly-compiled candidates — if a corpus is full of dispatch shapes and you don't filter, every candidate dispatcher will fuzzy-match every other one (pure noise).

## CLI workflow (advanced)

The CLI at `utils/find_dupes/main.das` supports modes the MCP tools don't expose. Use it for:

- **B1 baseline / CI gate** — `--baseline <corpus.json> --check`. Records absent from the baseline are tagged candidates; non-zero exit when any cluster/pair survives.
- **`--baseline-strict`** — drops clusters whose canonical was already in the baseline; only fully-new canonicals survive.
- **`--against-from-stdin`** — read newline-delimited candidate paths from stdin, e.g. piped from `git diff --name-only`.
- **`-L / --lambdas-only`** — cluster lambda bodies instead of top-level functions; useful for finding duplicated dastest `run` lambdas.
- **`--min-tokens N`** — drop trivial wrappers (default 8).
- **`--no-fuzzy`** — exact clusters only, faster on large corpora.

Quick recipes:

```sh
# one-off corpus build (commit this)
bin/Release/daslang.exe utils/find_dupes/main.das -- -p tests --export-functions tests_baseline.json

# CI gate: flag any new structural duplicates introduced by a PR
bin/Release/daslang.exe utils/find_dupes/main.das -- -p tests --baseline tests_baseline.json --check

# git pipeline against the baseline
git diff --name-only master | grep '\.das$' | \
    bin/Release/daslang.exe utils/find_dupes/main.das -- \
        --import-functions tests_baseline.json --against-from-stdin
```

Full flag reference: `bin/Release/daslang.exe utils/find_dupes/main.das -- -?` or `doc/source/reference/utils/find_dupes.rst`.

## Limitations

- **Structural matcher.** Same shape, different semantics → match. Different shape, same semantics → no match. Report is "candidates worth investigating", not a definitive verdict.
- **No auto-fix.** Discovery only.
- **Compile-required.** Candidate files must compile. If `candidate_files_failed > 0`, fix the compile error and rerun.
- **Macro-expanded.** Canonicalization runs after macro expansion. Two functions that differ in source but collapse to the same shape after macros will match — usually what you want, but can surprise you with macro-heavy daslib code.

## Iteration tip

Work top-down: most-similar matches first.

1. Walk every `exact_matches` entry — similarity 1.0 means structurally identical, almost always worth a close read.
2. Then `fuzzy_matches` sorted by `similarity` descending. Above 0.9 is usually a real near-duplicate; 0.7-0.9 is "investigate"; below the default threshold the tool already filtered them out.
3. If the report is dominated by one cluster of obviously-similar functions you don't care about (e.g. all your visitor methods have the same shape), check whether they fit the `dispatch` pattern; if not, that's a candidate for a new pattern in the upstream tool.

---

## Maintainer notes

The remainder of this skill is **only relevant when editing the find_dupes tool itself** — adding patterns, modifying the canonicalizer, wiring new MCP parameters, refactoring helpers. SDK users don't need any of it; the install version of this skill (`install/skills/find_dupes.md`) deliberately omits these sections.

### Architecture at a glance

`utils/find_dupes/` is an in-process compile-and-canonicalize pipeline:

1. **Scan** — `scan_das_files` (in `pipeline.das`) walks paths/dirs/globs; skips `builtin.das`, `daslib/debugger.das`, `daslib/profiler.das`, `_*` dirs.
2. **Compile** — `compile_and_collect` compiles one file at a time. Compile policy mirrors `utils/lint`: `ignore_shared_modules`, `export_all`. Optimisations stay ON so dastest macros (e.g. `unroll`) compile.
3. **Canonicalize** — `CanonicalVisitor` (in `canonical.das`) emits the alpha-renamed token stream.
4. **Sign** — `minhash.das` computes 64-slot MinHash over 5-grams (skipped when `want_sigs=false`, e.g. export mode).
5. **Filter** — `apply_pattern_filter` drops records matching shapes in `patterns.das`.
6. **Cluster** — `exact_buckets` for byte-identical groups; `fuzzy_pairs` for all-pairs MinHash + length-gate scoring (`cluster.das`).
7. **Report** — flat or per-candidate JSON + stdout summary (`report.das`).

The pipeline is **shared** between the CLI (`main.das`) and the MCP tools — the MCP tools `require pipeline.das` and call the same functions. Don't duplicate logic; make pipeline helpers `def public` and call them from both.

### Adding a new pattern

Three-step change in `patterns.das` + `test_find_dupes.das`:

1. **Predicate.** Add a `try_<name>(canonical : string; var hit : PatternHit) : bool` in `patterns.das`. Return `true` and populate `hit.name` (the user-visible identifier) and `hit.note` (a short human-readable summary, shown in `--verbose`) when the pattern matches.
2. **Wire it.** Add the call to `classify()` in priority order — *more specific shapes first*. The first match wins.
3. **Tests.** Add at least one positive test and one negative test in `test_find_dupes.das` under the `── patterns / classify ──` section. The negative test should be a real-looking canonical that should NOT match (mixed statements, nested blocks, single statement) — guard against the predicate over-firing.

Pattern names are the user-visible contract: they appear in `--keep`, the `--verbose` skip log, the summary line, and the MCP envelope's `patterns_skipped` map. Pick a short, stable identifier.

When matching against tokenized canonicals, prefer `tokenize_canonical` + `split_top_level_stmts` (already in `patterns.das`) — these are BLK-depth aware. Don't roll your own string scan.

### When to extract a helper into `pipeline.das`

If more than one entry point uses it (the CLI in `main.das`, the MCP `find_duplicates` tool, the MCP `export_corpus` tool, and the test suite all count), it lives in `pipeline.das` as `def public`. Examples that already follow this rule: `compile_and_collect`, `apply_pattern_filter`, `scan_das_files`, `is_skip_file`, `has_expect_directive`.

If you're tempted to copy-paste a helper from `main.das` into a new MCP tool: stop, move it into `pipeline.das` first, then call it from both.

### MCP wiring

Two MCP tools, both in `utils/mcp/tools/`:

- `export_corpus.das` — scans paths, compiles, writes the corpus JSON.
- `find_duplicates.das` — wraps B2 mode. Loads a corpus, compiles candidates, applies pattern filter, returns a per-candidate JSON envelope.

Adding parameters to either tool is a four-site change in `utils/mcp/protocol.das`:

1. **Schema** — `PropertySchema(...)` entry in the tool's `make_tool` call inside `handle_tools_list`.
2. **Argument extraction** — `arg<N> = get_string_arg(args, "<param>")` line in `handle_tools_call`.
3. **Dispatch** — forward the new arg in the `dispatch_tool` branch for the tool.
4. **Tool entry point** — add the parameter to the tool's `do_<tool>` signature in its `.das` file.

`dispatch_tool`, `run_tool`, and `handle_tools_call` currently support `arg1..arg6` plus `project`. If you need a 7th positional slot, extend all three signatures and the `log_tool` format string in lockstep.

### Empty-result handling

The MCP `find_duplicates` tool distinguishes three cases when `cand_refs` is empty after filtering:

1. **Compile failed** (`n_failed > 0`) — error envelope, includes the first failed file path.
2. **No functions in file** (`candidates_pre_filter == 0`) — error envelope, "no candidate functions extracted from {paths}".
3. **All candidates were pattern-filtered** (`candidates_pre_filter > 0` but `cand_refs` empty) — *success* envelope with empty `report.candidates`, populated `candidate_functions_pre_filter` and `patterns_skipped`. Caller can read `patterns_skipped` and retry with `keep=<name>`.

When adding new failure modes, preserve this distinction. "Empty result" is not the same as "tool failed" — AI agents need that signal to make the right next call.

### Tests

Two test suites:

- `utils/find_dupes/test_find_dupes.das` — unit tests for the pipeline (tokenizer, canonicalizer, clusterer, pattern matcher, exchange, B1/B2 modes). Run via `bin/Release/daslang.exe dastest/dastest.das -- --test utils/find_dupes/test_find_dupes.das` or the MCP `run_test` tool.
- `utils/mcp/test_tools.das` — integration tests for the MCP tools, including `do_find_duplicates` and `do_export_corpus`.

When you add a pattern, add tests in *both* places: the `classify()` unit test in `test_find_dupes.das`, and at least one envelope-shape test in `test_tools.das` if the pattern materially changes MCP output (e.g. a new envelope field).

`utils/find_dupes/fixture/synth.das` and `fixture/canonical_cases.das` are hand-crafted fixtures for smoke-testing the visitor end-to-end. If you add a new canonicalization concern (a new AST node visited differently), add a one-function case to `canonical_cases.das`.

### AOT registration

If you add a new test file under `utils/find_dupes/`, register it in `tests/aot/CMakeLists.txt` — see `skills/aot_testing.md`. CI runs ALL tests with AOT enabled.

### Linting / formatting

Standard daslang rules (see `skills/das_formatting.md`). `find_dupes`-specific:

- Records (`FuncRecord`) are non-copyable due to the `sig : array<...>` field. Use `<-`, `push_clone`, or `emplace` when threading them through arrays.
- Avoid `print` outside `main.das` and the test files. Library helpers in `pipeline.das` / `patterns.das` should be quiet (the MCP tools rely on stdout staying free for JSON-RPC); when you need diagnostics, use `to_log(LOG_INFO)` or take a `verbose : bool` parameter and gate the `print`.

### Updating documentation

Every user-facing change touches three files in lockstep:

- `utils/find_dupes/README.md` — the in-tree quick reference.
- `doc/source/reference/utils/find_dupes.rst` — the published reference doc.
- `install/skills/find_dupes.md` — the install-side skill that ships in the SDK.

If a change is repo-dev-only (e.g. a new internal helper, a refactor that doesn't surface), update only this skill (`skills/find_dupes.md`) and skip the install side.
