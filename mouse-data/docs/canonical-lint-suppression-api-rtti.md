---
slug: canonical-lint-suppression-api-rtti
title: What is the canonical lint suppression API for a custom lint visitor (e.g. imgui_lint, decs_lint, user-written [lint_macro])?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

**`require daslib/rtti`** and call **`is_lint_suppressed(at : LineInfo; code : string) : bool`** ([daslib/rtti.das:201](daslib/rtti.das#L201), landed 2026-05-15). Pair it with **`extract_lint_code(text : string) : string`** ([daslib/rtti.das:208](daslib/rtti.das#L208)) which pulls the `LINT001` prefix off a message like `"LINT001: foo"`.

Inside a lint visitor (any `AstVisitor` subclass), the canonical form of the suppression gate is the single line:

```das
def is_suppressed(text : string; at : LineInfo) : bool {
    return is_lint_suppressed(at, extract_lint_code(text))
}
```

The three in-tree visitors ([daslib/lint.das:63](daslib/lint.das#L63), [daslib/perf_lint.das:127](daslib/perf_lint.das#L127), [daslib/style_lint.das:65](daslib/style_lint.das#L65)) all use exactly this shape after the unification PR; downstream lint modules should follow.

Under the hood: `rtti_is_nolint_suppressed(fi, line, code)` — a C-side pointer-math scanner in `src/builtin/module_builtin_rtti.cpp` — does the whole parse with no allocation. The line is fetched via the new `FileInfo` line index ([[paranoid-collect-slow-fileinfo-line-index]]), so per-warning cost is O(line_length) ≈ tens of byte-compares.

## Practical implication

Don't roll your own `is_suppressed`. The three pre-unification copies in lint.das / perf_lint.das / style_lint.das (which called `get_file_source_line` + interpreted `find` chains) were 14 lines each and slow; the unified one is 1 line and fast. When writing imgui_lint / decs_lint / user lint rules, this is the entire suppression layer.

If you need the parsed code list yourself (e.g. for tooling that reports "suppressed-but-not-fired"), you can drop down to `rtti_is_nolint_suppressed(fi, line, code)` directly — it's bound under rtti_core. The wrapper just adds the null-guards on `at.fileInfo` / `at.line`.

## Questions

- How do I add lint-suppression support to a custom AstVisitor lint pass?
- What's the canonical way for an `imgui_lint`-style module to handle `// nolint:CODE`?
- Where do I get `is_lint_suppressed` from? Which module / which require?
- What are extract_lint_code and is_lint_suppressed in daslib/rtti.das, and when to use them?

## See also

- [[paranoid-collect-slow-fileinfo-line-index]] — the FileInfo line-index that makes per-warning suppression cheap
- [[nolint-recognized-forms]] — exactly which `// nolint:CODE` forms the C-side scanner accepts
