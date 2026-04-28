# find_dupe — AI judge for detect-dupe clusters

`detect-dupe` produces clusters of suspected duplicate functions; on a real codebase those clusters mix real duplicates with false positives (boilerplate, generic accessors, shared-shape glue). `find-dupe` (directory: `utils/find-dupe/`) consumes the detect-dupe JSON and asks Claude to **partition** each cluster into real-duplicate groups + false positives, with a one-line reason.

This skill covers when and how to invoke that judge — via MCP for AI-driven workflows, or via the CLI when iterating directly. Read [skills/detect_dupe.md](detect_dupe.md) first if you haven't already; that's the upstream cluster-producing pipeline.

## When to use this

- A `detect-dupe` run produced too many clusters to walk by hand and you want a triaged list.
- You're authoring or reviewing a PR and want to know which `detect_duplicates` matches are *real* duplicates worth refactoring vs structural noise.
- You're standing up a CI gate and want a "real positives" subset of the detect-dupe output rather than the firehose.

## ⚠️ Privacy

`find-dupe` **uploads function source to Anthropic's API.** Each cluster's full bodies (verbatim, with paths and line numbers) are sent as part of every judge call. Don't run it on proprietary, confidential, or restricted code unless your data-handling policy permits.

Use `--dry-run` (CLI) or `dry_run=true` (MCP) to preview cluster count and token cost without making any API calls.

## First-time setup

The judge depends on the `anthropic/anthropic` daspkg, which is **not** linked into `daslang.exe` — it's fetched at runtime by daspkg. From the project root:

```bash
bin/daslang utils/daspkg/main.das -- install --root utils/find-dupe
```

This unpacks `das-claude` under `utils/find-dupe/modules/`. Without this step, every invocation fails at compile time with `module 'anthropic/anthropic' not found`. The MCP tools surface this as a structured error with the install command.

Set `ANTHROPIC_API_KEY` in `~/.zshenv` (not `~/.zshrc` — non-interactive subshells skip `.zshrc`):

```bash
echo 'export ANTHROPIC_API_KEY="sk-ant-..."' >> ~/.zshenv
```

Verify before any real run:

```bash
bin/daslang utils/find-dupe/main.das -- --smoke-test
```

## MCP workflow

Two tools available:

| Tool | Use when |
|---|---|
| `judge_duplicates` | You already have a `detect-dupe` JSON report (e.g. from the `detect_duplicates` MCP tool, the `detect-dupe --json` CLI, or a baseline). Pass `input=<path>`. |
| `find_dupe` | You want a one-shot run on a directory / file list / PR. Pass `paths=...`; the tool runs `detect-dupe` first and judges the result in the same call. |

Both accept `dry_run=true` (token estimate, no API calls) — the safe first step on a new corpus. Other params: `model` (`haiku`/`sonnet`), `max_clusters` (cost gate), `out` (override the temp output dir). `judge_duplicates` adds `positives_only=true` to filter the verdict report to actionable rows.

The response envelope embeds the verdict JSON via `@embed`, so you get the structured verdicts inline without re-reading a file.

## CLI workflow

Three steps when iterating outside MCP:

```bash
# 1. Run detect-dupe
bin/daslang utils/detect-dupe/main.das -- -p <paths> --json ./dupes.json

# 2. Dry-run the judge (cost preview)
bin/daslang utils/find-dupe/main.das -- --input ./dupes.json --dry-run -v

# 3. Live run
bin/daslang utils/find-dupe/main.das -- --input ./dupes.json -v
# writes ./find-dupe-out/find_dupe_verdicts.{json,md}
```

Run from the project root — `detect-dupe` records source paths relative to its cwd, and the judge extracts function bodies from disk using those paths.

## Reading the verdict envelope

Each `VerdictRow` carries:

- `verdict`: `real` | `partial` | `false_positive` | `skipped` | `error`
- `kind`: `exact` | `fuzzy`
- `members`: array of `{file, line, end_line, name, is_lambda}` — the cluster's functions
- `groups`: partition into sub-sets that ARE each real duplicates of one another (every entry is a list of member indices)
- `false_positives`: indices of members that don't match anyone else
- `reason`: one-sentence rationale from the model
- `input_tokens` / `output_tokens`: per-call usage

Invariant: every member index in `[0, N)` appears exactly once across `groups + false_positives`. `parse_verdict` enforces this; a violation surfaces as `verdict=error`.

For dedup PRs, walk the `real` rows first (single-group clusters covering all members — drop-in candidates for consolidation), then `partial` rows (sub-clusters real, others noise — partial extraction), then ignore `false_positive` and `skipped` unless tuning the upstream filter.

## Cost guardrails

- **Dry-run first** on any unfamiliar input. The output prints `est. cost: $X.XXX` so you know what a live run will spend.
- `--max-clusters N` caps how many clusters get judged after the priority sort. Combined with `--sort-by` (`default` puts fuzzy pairs first, then larger clusters), you can grab the top-N highest-value clusters per run.
- `--min-lines 6` (default) skips trivially short clusters where the judge has nothing to chew on.
- `--positives-only` writes a slim verdict file with only `real`+`partial` rows — the `summary` still reflects all clusters, so you keep the totals.
- `--parallel N` (default 8) — concurrent judge calls. Use 1 to debug; raise for speed when your API rate limits allow.

A typical 3-4 function cluster (~50 lines total) costs ~$0.0025 on Haiku 4.5; multiply by cluster count for a ballpark.

## Maintainer notes

- The CLI lives in `utils/find-dupe/main.das`; pipeline modules are `cluster_input.das`, `source_extract.das`, `judge.das`, `judge_live.das`, `report.das`. Tests in `utils/find-dupe/tests/` are hermetic (no API calls — `parse_verdict`, `classify_verdict`, `extract_source`, `read_input_report`).
- The MCP tools (`utils/mcp/tools/judge_duplicates.das`, `utils/mcp/tools/find_dupe.das`) shell out via `run_and_capture` rather than `require`-ing the CLI modules. This keeps the MCP server's compile clean of the daspkg dependency. When the subprocess fails because the package is missing, both tools detect the `module 'anthropic/anthropic' not found` signature and return a structured envelope with the install command.
- Adding a new MCP parameter: extend the dispatch in `utils/mcp/protocol.das` (both the `make_tool(...)` schema and the `get_string_arg(...)` block), then thread it through `do_judge_duplicates` / `do_find_dupe` and into `run_find_dupe_judge`. Mind the 6-arg dispatch limit — combine related options or drop low-value ones first.
- Output filenames are `find_dupe_verdicts.json` and `find_dupe_verdicts.md`; the schema-version constant is `FIND_DUPE_SCHEMA_VERSION` in `report.das`. Bump it on incompatible schema changes (and add a parser-side reject in `cluster_input.das`-style versioned readers if any consumers ship).
- Pricing values in this skill, the README, and `cost_estimate()` in `main.das` are all manually maintained — update all three when Anthropic's pricing shifts.
