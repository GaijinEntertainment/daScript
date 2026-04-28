# detect-dupe — duplicate-function detection for daslang code

Read this skill before doing duplicate-detection work on a daslang codebase: building a corpus of canonical signatures, asking "did I just write something that already exists?", or wiring a CI gate that flags new structural duplicates. The tool ships as `utils/detect-dupe/` plus two MCP tools (`export_corpus`, `detect_duplicates`) on the daslang MCP server.

## When to use this

- A function you're writing feels familiar — you suspect it already exists somewhere in `daslib/`, `tests/`, or your project's source tree.
- You're reviewing a PR and want a quick "is this a copy-paste of an existing helper?" check on the diff.
- You want a CI gate that flags new structural duplicates introduced by a PR (B1 baseline mode).

What it finds: functions whose **canonical token stream** (identifiers, types, and literals all alpha-renamed and collapsed) is byte-identical or close to existing functions. It is intentionally a structural matcher — `add(a, b : int)` and `add(a, b : float)` look identical to it; that's the point.

What it does **not** find: semantic duplicates that have different control flow, different called helpers, or significantly different shape. Two implementations of "the same idea" written from scratch will not match.

## Workflow via MCP (preferred)

The MCP server exposes the entire pipeline — no shelling out:

| MCP tool | Purpose |
|---|---|
| `export_corpus` | Scan paths/dirs/globs, compile each `.das` file, write a `corpus.json` |
| `detect_duplicates` | Compare candidate file(s) against a `corpus.json`, return per-candidate matches |

### Step 1 — build a corpus

```
mcp__daslang__export_corpus(paths="daslib,utils,my_project", out="corpus.json")
```

You build the corpus once over the body of code you want to compare against. Re-run it when the code drifts enough that stale matches become a problem.

Output is an envelope:

```json
{ "out": "...", "files_scanned": 86, "files_skipped": 0, "records_written": 612 }
```

`files_skipped` counts `.das` files that contain `expect ` directives — those are intentionally-broken fixtures and aren't part of the corpus. If `records_written` is much lower than expected, the corpus was filtered hard by `min_tokens` (default 0 — most callers should leave it there).

### Step 2 — query against a candidate

```
mcp__daslang__detect_duplicates(
  paths="my_project/new_helper.das",
  corpus="corpus.json"
)
```

Pass `paths` as comma- or newline-delimited (the latter lets you pipe `git diff --name-only` through). Records in the corpus whose `file` matches a `paths` entry are dropped first, then the candidate is freshly compiled — so the file is compared against the rest of the world, never against its own stale copy in the corpus.

The envelope:

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

The `report.candidates[]` array is the actionable output: each candidate function with its top-N `exact_matches` (similarity 1.0) and `fuzzy_matches` (similarity in `[threshold, 1)`), each with `file`, `line`, `name`, `is_lambda`. Walk those to investigate.

### Reading the envelope

| Field | What it tells you |
|---|---|
| `candidate_functions_pre_filter` | How many candidates the file had before pattern filtering |
| `candidate_functions` | How many survived the filter and were clustered |
| `patterns_skipped` | Per-pattern count of records dropped (corpus + candidates). `{}` means the filter was a no-op |
| `exact_clusters_kept` / `fuzzy_pairs_kept` | Aggregate match counts after the candidate filter |

When `candidate_functions == 0` but `candidate_functions_pre_filter > 0`, the filter consumed every candidate. The report is empty by design; if you want to see those records anyway, retry with `keep="all"` (or with a specific pattern name from `patterns_skipped`).

## Pattern filter

A "pattern" is a structural shape that signals "this function is boilerplate, not real code" — a wrapper or dispatcher whose canonical token stream contains zero unique signal beyond its repetition count. Pattern-matched functions are dropped from clustering by default.

Currently shipped patterns:

| Name | Detects | Why it's boilerplate |
|---|---|---|
| `visitor` | Class-method whose hook starts with `visit`, `preVisit`, `postVisit`, `before`, or `after` (matched by name, regardless of body) | `AstVisitor` overrides — one method per AST node type by dispatch contract, so cross-class duplication is structural, not actionable. Catches the swarms in `aot_cpp`, `ast_print`, `templates_boost`, `rst_comment`, `perf_lint` |
| `dispatch` | Body is N >= 2 byte-identical top-level statement chunks | Test runners and dispatchers (`t \|> run("X") @(t) {…}` lists, `t \|> bench(…)` lists, repeated-init blocks). Lambda bodies are collapsed to `ADDR` upstream, so two `run` calls look identical regardless of what the lambdas actually do |
| `emit` | 1..6 top-level statements, each a single trivial `CALL:foo(...)` (literal/var/field args only — no nested calls, no control flow) or a `RET ...` | Emitter shells like `def visitX(...) { write(*ss, ")") ; return that }`. Catches free-function variants that the name-based `visitor` matcher doesn't cover |

Match order is name-first (`visitor`), then body-shape (`dispatch`, `emit`). A visitor method whose body fits the `emit` shape is still classified as `visitor` — the more semantic bucket wins.

Override per-pattern with `keep="<name>"` (comma-separated for multiple), or disable filtering wholesale with `keep="all"`. Default (omit `keep`) skips every known pattern.

The filter applies to **both** corpus records and freshly-compiled candidates. If a corpus is full of dispatch shapes and you don't filter, every candidate function will fuzzy-match every other dispatcher — pure noise. The filter is on by default for that reason; opt out only when you genuinely want to see boilerplate.

## CLI workflow (advanced)

The CLI ships at `utils/detect-dupe/main.das` and supports a few modes the MCP tools don't expose. Use it when you need:

- **B1 baseline / CI gate** — `--baseline <corpus.json> --check`. Records absent from the baseline are tagged candidates; non-zero exit when any cluster/pair survives.
- **`--baseline-strict`** — drops clusters whose canonical was already in the baseline, so only fully-new canonicals survive.
- **`--against-from-stdin`** — read newline-delimited candidate paths from stdin, e.g. piped from `git diff --name-only`.
- **`-L / --lambdas-only`** — cluster lambda bodies instead of top-level functions; useful for finding duplicated dastest `run` lambdas.
- **`--min-tokens N`** — drop trivial wrappers (default 8).
- **`--no-fuzzy`** — exact clusters only, faster on large corpora.

Quick recipes:

```sh
# one-off corpus build (commit this)
bin/daslang utils/detect-dupe/main.das -- -p src --export-functions baseline.json

# CI gate: flag any new structural duplicates introduced by a PR
bin/daslang utils/detect-dupe/main.das -- -p src --baseline baseline.json --check

# git pipeline against the baseline
git diff --name-only master | grep '\.das$' | \
    bin/daslang utils/detect-dupe/main.das -- \
        --import-functions baseline.json --against-from-stdin
```

Full flag reference: `bin/daslang utils/detect-dupe/main.das -- -?` or the published docs.

## Limitations

- **Structural matcher.** Same shape, different semantics → match. Different shape, same semantics → no match. This is by design (it's fast, deterministic, and surfaces real copy-paste), but it means the report is a "candidates worth investigating" list, not a definitive verdict.
- **No auto-fix.** Discovery only. Refactoring decisions are yours.
- **Compile-required.** The candidate file must compile. If `candidate_files_failed > 0`, fix the compile error and rerun.
- **Macro-expanded.** Canonicalization runs after macro expansion. Two functions that look different in source but collapse to the same shape after macro expansion will match. Usually this is what you want, but it can surprise you with macro-heavy daslib code.

## Tip: Iteration

When investigating, work top-down: most-similar matches first.

1. Look at every `exact_matches` entry — similarity 1.0 means structurally identical, almost always worth a close read.
2. Then `fuzzy_matches` sorted by `similarity` descending. Above 0.9 is usually a real near-duplicate. 0.7-0.9 is "investigate"; below the default threshold (0.7) the tool already filtered them out.
3. If the report is dominated by one cluster of obviously-similar functions you don't care about (e.g. all your visitor methods have the same shape), see whether they fit the `dispatch` pattern — if not, that's a candidate for a new pattern in the upstream tool.
