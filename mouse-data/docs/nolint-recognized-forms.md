---
slug: nolint-recognized-forms
title: What `// nolint:CODE` forms does the lint-suppression scanner actually accept?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

The C-side scanner `rtti_is_nolint_suppressed` in [src/builtin/module_builtin_rtti.cpp](src/builtin/module_builtin_rtti.cpp) (landed 2026-05-15) recognizes **all** of these forms:

| Form | Suppresses |
|---|---|
| `// nolint:LINT001` | LINT001 |
| `// nolint:LINT001,LINT002,LINT003` | each listed code (comma-separated) |
| `// nolint:LINT001 , LINT002` | both (whitespace around commas/tokens tolerated) |
| `// nolint:LINT001 nolint:LINT002` | both (repeated directives, space-separated) |
| `// nolint:PERF003 single access for operator parsing` | PERF003 (trailing prose ignored) |
| `// free prose nolint:LINT001 more prose` | LINT001 (lenient — directive anywhere after `//`) |

**Not** suppressed:
- `// nolint` (no colon, no code) → matches nothing.
- A line with no `//` at all → no suppression.
- Substring traps: `// nolint:LINT001` does NOT match a query for `LINT00` — exact-token equality.

The space-separated repeated form was discovered in production code at [modules/dasPEG/peg/parser_generator.das:543](modules/dasPEG/peg/parser_generator.das#L543) (`// nolint:LINT002 nolint:LINT003`) — pre-unification, the loose JS-style scanner accepted it; the strict comma-only first cut broke it. The shipped scanner walks the rest of the comment looking for `nolint:` occurrences and parses a comma-list after each.

## Practical implication

Prefer **`// nolint:CODE1,CODE2`** (comma form) for new code — most compact, most readable. The repeated-`nolint:` form exists for backward compatibility; don't introduce it gratuitously. Single-code is by far the most common in the tree (~50 sites in daslib alone, all single).

If you're writing tooling that *generates* nolint directives (auto-fix tools, codemods), emit the comma form. The scanner handles both, but readers expect comma.

## Questions

- What `nolint` formats does the lint scanner support?
- Can I put multiple codes on one nolint directive? What separators?
- Does `// nolint:LINT001 nolint:LINT002` (space-separated) work?
- Why doesn't `// nolint` alone (no `:CODE`) suppress anything?
- Is the nolint match strict or fuzzy on the code token (substring traps)?

## See also

- [[canonical-lint-suppression-api-rtti]] — the `is_lint_suppressed` daslang wrapper that calls this scanner
- [[paranoid-collect-slow-fileinfo-line-index]] — the FileInfo line-index that makes per-warning lookup O(1)
