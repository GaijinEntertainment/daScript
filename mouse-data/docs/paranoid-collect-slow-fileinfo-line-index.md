---
slug: paranoid-collect-slow-fileinfo-line-index
title: Why is paranoid_collect (and perf_lint_collect / style_lint_collect) slow on real files — what was the structural cost?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

**`get_file_source_line` did O(line_offset) per call**: a linear scan from byte 0 of source counting `\n` ([src/builtin/module_builtin_ast.cpp:983](src/builtin/module_builtin_ast.cpp#L983), pre-2026-05-15). No line-offset index existed on `FileInfo` ([include/daScript/simulate/debug_info.h:117](include/daScript/simulate/debug_info.h#L117)). The three lint visitors call `is_suppressed` → `get_file_source_line` per candidate warning, and style_lint's per-line scanners (`source_line_between`, `check_defer_pipe`, `scan_file_lines`) call it per line of a range — so lint cost was O(warnings × file_size) at best, O(L²) at worst on `scan_file_lines`.

**Fix (PR landing 2026-05-15):** lazy `vector<uint32_t> lineOffsets` on `FileInfo` with `buildLineIndex()` / `getLine(line, begin, len)` — O(1) line fetch after first call. `get_file_source_line` rewritten to use it; every existing caller benefits transparently.

Secondary cost #2 fixed in same PR: `reported_locations : array<uint64>` with linear `has_value` → `table<uint64>` (set form). Was O(N²) over N warnings in perf_lint + style_lint.

## Practical implication

If you're touching `FileInfo` source-access internals, call `info->getLine(...)` not `getSourceAndLength(...)` + hand-walk. If you're writing a new lint visitor and need to scan source lines (defer-pipe-style patterns), one `getLine` per line is now cheap — no need for "read whole file once, scan in memory" workarounds.

## Questions

- Why is paranoid_collect / perf_lint_collect / style_lint_collect dog-slow on big files?
- What is the cost of get_file_source_line and how is it amortized?
- How does FileInfo fetch a source line by 1-based line number after the line index landed?
- Why does scan_file_lines (style_lint comment-hygiene scan) hit O(L²) on master before the line index?

## See also

- [[canonical-lint-suppression-api-rtti]] — the `is_lint_suppressed` / `extract_lint_code` helpers in daslib/rtti.das
- [[nolint-recognized-forms]] — what `// nolint:CODE` patterns the C-side scanner accepts
