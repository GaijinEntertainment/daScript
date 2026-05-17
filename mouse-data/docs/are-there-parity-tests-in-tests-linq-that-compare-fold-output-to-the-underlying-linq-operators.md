---
slug: are-there-parity-tests-in-tests-linq-that-compare-fold-output-to-the-underlying-linq-operators
title: Are there parity tests in tests/linq/ that compare `_fold` output to the underlying linq operators?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

There's no file named "parity" or similar. The parity-test surface IS the broader [tests/linq/](tests/linq/) directory:

- `test_linq.das` — comprehension basics
- `test_linq_aggregation.das` — count/sum/min/max/avg
- `test_linq_querying.das` — any/all/contains
- `test_linq_transform.das` — select / select_many / zip
- `test_linq_sorting.das` — order / reverse
- `test_linq_group_by.das` — group_by / having
- `test_linq_join.das` — joins
- `test_linq_partition.das` — take / skip / chunk / take_while / skip_while
- `test_linq_set.das` — distinct / union / except / intersect / unique
- `test_linq_element.das` — first / last / single / element_at
- `test_linq_concat.das` — concat / prepend / append
- `test_linq_generation.das` — range / repeat
- `test_linq_bugs.das` — regressions

Each file uses `[test]` functions with `t |> run("name") @(t) { ... }` blocks asserting `t |> equal(actual, expected)`. These exercise the regular linq operators (`where_`, `select`, `count`, ...) directly — they're not split into "fold-on" vs "fold-off" variants.

Dedicated `_fold` tests live in `test_linq_fold.das` (functional output) and `test_linq_fold_ast.das` (AST-shape verification — pattern-matches the macro expansion). These DO compare `_fold(chain)` output against the plain `chain` output for the shapes the macro recognizes.

When the user says "parity tests" in linq context, treat the full `test_linq_*.das` suite as the operator-coverage map. Phase-2+ benchmark/splice PRs should add a `benchmarks/sql/` entry for each shape exercised here that isn't already covered (tracked as a checklist in `benchmarks/sql/LINQ.md`).

## Questions
- Are there parity tests in tests/linq/ that compare `_fold` output to the underlying linq operators?
- What's the "parity test" coverage surface for linq?
- Where are tests for linq operators?

## Questions
- Are there parity tests in tests/linq/ that compare `_fold` output to the underlying linq operators?
