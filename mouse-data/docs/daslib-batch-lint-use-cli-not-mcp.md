---
slug: daslib-batch-lint-use-cli-not-mcp
title: How do I lint many daslib files at once? MCP lint times out on big files.
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

**Use the CLI runner, not MCP `lint`, for batch/tree sweeps.**

```bash
bin/Release/daslang.exe utils/lint/main.das -- --quiet daslib/
```

This runs the same lint passes (paranoid + perf + style) and prints `PASS file.das` / `WARN file.das (N)` lines plus a `--- Summary ---` block at the end. A full `daslib/` sweep across ~112 files takes ~60–120s.

**Why not MCP `lint`?** It compiles each file with full module loading and shared-context teardown per call. On big files like `daslib/debug.das` (1700+ lines, many imports) a single MCP `lint` call can take ~50s — and the MCP runtime caps tool calls before that finishes, returning a timeout error. Same shape on `compile_check` for the same file.

**When MCP `lint` IS the right tool**: small targeted scans (one file, a `widgets/*.das` glob, etc.). It returns structured `WARN/PASS` + per-hit lines just like the CLI; the cost is the per-call cold start and the timeout cap.

**Tree-wide hit counts by rule:**
```bash
bin/Release/daslang.exe utils/lint/main.das -- --quiet daslib/ 2>&1 \
  | grep -oE '(STYLE|PERF|LINT)[0-9]+' | sort | uniq -c | sort -rn
```

**Per-file hit counts:**
```bash
bin/Release/daslang.exe utils/lint/main.das -- --quiet daslib/ 2>&1 \
  | grep -E '^WARN' | awk '{print $2, $3}' | sort -k2 -rn
```

The `--quiet` flag suppresses `PASS` lines and progress noise, so failures show up uncluttered. Without it you get a `checking file.das...` line per file which inflates the log.

**Pre-PR pattern:** during the make_pr step 1 check, lint `daslib/`, `dastest/`, plus any other top-level dir your PR touches in one call:
```bash
bin/Release/daslang.exe utils/lint/main.das -- --quiet daslib/ dastest/ utils/fix-lint-errors/
```

## Questions
- How do I lint many daslib files at once? MCP lint times out on big files.
