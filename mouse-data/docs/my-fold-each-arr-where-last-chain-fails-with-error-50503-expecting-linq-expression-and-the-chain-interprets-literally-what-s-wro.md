---
slug: my-fold-each-arr-where-last-chain-fails-with-error-50503-expecting-linq-expression-and-the-chain-interprets-literally-what-s-wro
title: My _fold(each(arr)._where(...).last()) chain fails with error[50503] "expecting linq expression" and the chain interprets literally. What's wrong?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

The `_where(...)` / `_select(...)` / `_any(...)` / `_all(...)` etc. forms with leading underscore are **`[call_macro]` shorthand macros that live in `daslib/linq_boost`**, NOT `daslib/linq`. If your file requires only `daslib/linq` (and `daslib/linq_fold`), those macros are undefined; the `_fold` planner can't recognize the chain, returns null, and the chain falls back to literal-form interpretation which then fails because:

- `each(arr)._where(...)` parses to dot-method-on-iterator, but iterator types have no `_where` field/method → `error[30928]: can't get field '_where' of iterator<int&>`
- `_fold(...)` macro reports `error[50503]: expecting linq expression`

**Fix:** add `require daslib/linq_boost` to the top of the file. That makes `_where` / `_select` etc. resolvable and the splice planner can do its job.

| File requires only `daslib/linq` | File requires `daslib/linq` + `daslib/linq_boost` |
|---|---|
| `_fold(each(arr)._where(_ > 5).last())` → 50503 + 30928 cascade | works — splice plans the chain |

The mistake is easy to make when writing standalone smoke tests / scratch repros, because the production benchmarks and `tests/linq/test_linq_fold.das` always require `linq_boost` already. The lambda forms (`where_(arr, $(x) => ...)`) work without `linq_boost` since they're plain function calls, but you lose the `_` placeholder convenience and the splice planner uses the `_where`-shape recognition path.

**Heuristic:** if the chain uses `_<op>(_ <expr>)` placeholder forms anywhere — even one — pull in `daslib/linq_boost`. If you write all chains as `op_(arr, $(x) => ...)` with explicit lambdas, plain `daslib/linq` suffices but you lose ergonomics.

## Questions
- My _fold(each(arr)._where(...).last()) chain fails with error[50503] "expecting linq expression" and the chain interprets literally. What's wrong?
