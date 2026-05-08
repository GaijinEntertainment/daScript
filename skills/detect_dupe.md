# detect-dupe — duplicate-function detection

Read this skill before doing duplicate-detection work — building a corpus, asking "does this function already exist?", wiring a CI gate. The full user-facing reference lives at `doc/source/reference/utils/detect_dupe.rst`.

## When to use this

- A function you're writing feels familiar — you suspect it already exists somewhere in `daslib/`, `tests/`, or `utils/`.
- You're reviewing a PR and want a "is this a copy-paste of an existing helper?" check on the diff.
- You want a CI gate that flags new structural duplicates introduced by a PR (B1 baseline mode).

What it finds: functions whose **canonical token stream** (identifiers, types, and literals all alpha-renamed and collapsed) is byte-identical or close. It is intentionally a structural matcher — `add(a, b : int)` and `add(a, b : float)` look identical to it; that's the point.

What it does **not** find: semantic duplicates with different control flow, different called helpers, or significantly different shape. Two implementations of "the same idea" written from scratch will not match.

## Workflow via MCP (preferred)

The MCP server exposes the entire pipeline. Both tools shell out to `daslang utils/detect-dupe/main.das` — daslang's `require` grammar can't take hyphenated path components, so the wrappers invoke the CLI as a subprocess:

| MCP tool | Purpose |
|---|---|
| `export_corpus` | Scan paths/dirs/globs, compile each `.das` file, write a `corpus.json` |
| `detect_duplicates` | Compare candidate file(s) against a `corpus.json`, return per-candidate matches |

### Step 1 — build a corpus

```
mcp__daslang__export_corpus(paths="daslib,utils,tests", out="corpus.json")
```

Build the corpus once over the body of code you want to compare against. Re-run when the code drifts enough that stale matches become a problem.

`workers="0"` parallelizes across hardware threads (auto). `workers="1"` keeps the run sequential. The output JSON is byte-identical across worker counts — the file list is sorted before chunking, and shards are merged in chunk-index order. Below 16 input files the export stays sequential regardless (child-process startup dominates).

`paths_file="<path>"` scopes the export to an explicit precomputed list — useful for PR-scoped runs:

```
git diff --name-only master | grep '\.das$' > /tmp/pr.txt
mcp__daslang__export_corpus(paths_file="/tmp/pr.txt", out="pr.json")
```

`paths_file` composes with `paths` (union, deduped). Use it whenever you'd otherwise hit ARG_MAX with thousands of comma-separated entries.

Envelope:

```json
{ "exit_code": 0, "out": "...", "stdout": "..." }
```

`stdout` carries the subprocess's per-file diagnostics — including which `.das` files were skipped because they contain `expect ` directives (intentionally-broken fixtures, not part of the corpus). Detailed counts (files scanned/skipped, records written) live in `stdout`, not in structured fields, since the wrapper now subprocesses `detect-dupe --export-functions`.

### Step 2 — query against a candidate

```
mcp__daslang__detect_duplicates(
  paths="path/to/new_helper.das",
  corpus="corpus.json"
)
```

Pass `paths` as comma- or newline-delimited (the latter lets you pipe `git diff --name-only` through). Records in the corpus whose `file` matches a `paths` entry are dropped first, then the candidate is freshly compiled — so the file is compared against the rest of the world, never against its own stale copy in the corpus.

Envelope:

```json
{
  "exit_code": 0,
  "report": { "candidates": [ ... ] },
  "stdout": "..."
}
```

`report` is the embedded per-candidate JSON from `detect-dupe --json` (the wrapper writes a temp file, reads it back, and deletes it — that's why no `json_path` is returned). `report.candidates[]` is the actionable output: each candidate with its top-N `exact_matches` (similarity 1.0) and `fuzzy_matches` (similarity in `[threshold, 1)`), each with `file`, `line`, `name`, `is_lambda`.

`stdout` carries the per-candidate counts the in-process pipeline used to surface as structured fields (`candidate_functions`, `candidate_functions_pre_filter`, `exact_clusters_kept`, `fuzzy_pairs_kept`, `patterns_skipped`). When the report ends up empty but stdout shows a non-zero `_pre_filter` count, the pattern filter consumed every candidate — retry with `keep="all"` (or a specific name from the per-pattern stdout breakdown) to bypass it.

## Pattern filter

A "pattern" is a structural shape that signals "this function is boilerplate, not real code" — a wrapper or dispatcher whose canonical token stream contains zero unique signal beyond its repetition count. Pattern-matched functions are dropped from clustering by default.

Currently shipped:

| Name | Detects | Why it's boilerplate |
|---|---|---|
| `visitor` | Class-method whose hook starts with `visit`, `preVisit`, `postVisit`, `before`, or `after` (matched by name, regardless of body) | `AstVisitor` overrides — one method per AST node type by dispatch contract, so cross-class duplication is structural. Catches the swarms in `aot_cpp`, `ast_print`, `templates_boost`, `rst_comment`, `perf_lint` |
| `dispatch` | Body is N >= 2 byte-identical top-level statement chunks | dastest's `t \|> run("X") @(t) {…}` lists, `t \|> bench(…)` lists, repeated-init blocks. Lambda bodies collapse to `ADDR` upstream, so two `run` calls look identical regardless of what the lambdas do |
| `emit` | 1..6 top-level statements, each a single trivial `CALL:foo(...)` (literal/var/field args only — no nested calls, no control flow) or a `RET ...` | Emitter shells like `def visitX(...) { write(*ss, ")") ; return that }`. Catches free-function variants that the name-based `visitor` matcher doesn't cover |

Match order is name-first (`visitor`), then body-shape (`dispatch`, `emit`). A visitor method whose body fits the `emit` shape is still classified as `visitor` — the more semantic bucket wins.

Override per-pattern with `keep="<name>"` (comma-separated for multiple), or disable filtering wholesale with `keep="all"`. Default (omit `keep`) skips every known pattern.

The filter applies to **both** corpus records and freshly-compiled candidates — if a corpus is full of dispatch shapes and you don't filter, every candidate dispatcher will fuzzy-match every other one (pure noise).

## CLI workflow (advanced)

The CLI at `utils/detect-dupe/main.das` supports modes the MCP tools don't expose. Use it for:

- **B1 baseline / CI gate** — `--baseline <corpus.json> --check`. Records absent from the baseline are tagged candidates; non-zero exit when any cluster/pair survives.
- **`--baseline-strict`** — drops clusters whose canonical was already in the baseline; only fully-new canonicals survive.
- **`--against-from-stdin`** — read newline-delimited candidate paths from stdin, e.g. piped from `git diff --name-only`.
- **`--paths-from <file>` / `--paths-stdin`** — read the *primary* file list from a file or stdin (skips blank lines and `#`-comments). Composes with `-p`. Use the file form when you'd hit ARG_MAX with thousands of entries; the stdin form to plug into `git diff --name-only` pipelines for PR-scoped corpus builds.
- **`-j / --workers N`** — parallel `--export-functions` across N child detect-dupe processes. 0 (default) = hardware threads. Output is byte-identical to a sequential run (sorted-then-chunked, shards merged in order). Below 16 files the export stays sequential.
- **`-L / --lambdas-only`** — cluster lambda bodies instead of top-level functions; useful for finding duplicated dastest `run` lambdas.
- **`--min-tokens N`** — drop trivial wrappers (default 8).
- **`--no-fuzzy`** — exact clusters only, faster on large corpora.

Quick recipes:

```sh
# one-off corpus build (commit this) — parallel by default for big runs
bin/daslang utils/detect-dupe/main.das -- -p tests --export-functions tests_baseline.json

# explicit worker count (1 = sequential)
bin/daslang utils/detect-dupe/main.das -- -p tests -j 8 --export-functions tests_baseline.json

# CI gate: flag any new structural duplicates introduced by a PR
bin/daslang utils/detect-dupe/main.das -- -p tests --baseline tests_baseline.json --check

# PR-scoped corpus build via file list (avoids ARG_MAX on big diffs)
git diff --name-only master | grep '\.das$' > /tmp/pr.txt
bin/daslang utils/detect-dupe/main.das -- --paths-from /tmp/pr.txt --export-functions pr.json

# Same thing piped via stdin
git diff --name-only master | grep '\.das$' | \
    bin/daslang utils/detect-dupe/main.das -- --paths-stdin --export-functions pr.json

# git pipeline against the baseline (detect_duplicates flow, B2 mode)
git diff --name-only master | grep '\.das$' | \
    bin/daslang utils/detect-dupe/main.das -- \
        --import-functions tests_baseline.json --against-from-stdin
```

Full flag reference: `bin/daslang utils/detect-dupe/main.das -- -?` or `doc/source/reference/utils/detect_dupe.rst`.

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

