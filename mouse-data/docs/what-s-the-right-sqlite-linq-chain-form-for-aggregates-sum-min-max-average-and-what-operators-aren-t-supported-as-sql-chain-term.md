---
slug: what-s-the-right-sqlite-linq-chain-form-for-aggregates-sum-min-max-average-and-what-operators-aren-t-supported-as-sql-chain-term
title: What's the right sqlite_linq chain form for aggregates (sum/min/max/average), and what operators aren't supported as `_sql` chain terminals?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

Column aggregates in `_sql` chains use the **regular linq function name** after a `_select`, NOT an `_aggregate(_.Col)` macro:

```daslang
// CORRECT — _sql analyzer recognizes `_select(_.Col) |> sum()` and emits SELECT SUM(price)
let s = _sql(db |> select_from(type<Car>) |> _select(_.price) |> sum())
let m = _sql(db |> select_from(type<Car>) |> _select(_.price) |> min())
let a = _sql(db |> select_from(type<Car>) |> _select(_.price) |> average())  // promotes to double
```

There is no `_sum` / `_min` / `_max` / `_average` chain macro. The error if you try one is `error[30838]: can't locate variable '_'` because `_sum` doesn't dispatch as a call macro.

The full set of `_sql` chain terminals is **`_to_array()`, `_first()`, `_first_opt()`, `count()`, and `sum()`/`min()`/`max()`/`average()` after a 1-column `_select`**. These are NOT supported as chain terminals:

| Chain | Why not | Workaround |
|---|---|---|
| `_any()` (no args, terminal) | not implemented | `_first_opt() \|> is_some` |
| `_all(pred)` | no SQL idiom recognized | invert: `_where(NOT pred) \|> count() == 0` |
| `take(N) \|> count()` | LIMIT-after-aggregate has no effect (aggregate collapses to 1 row) | drop count, materialize: `take(N)` returns array, take `length()` |
| `skip(M) \|> take(N) \|> count()` | same | same — terminate in to_array |
| `distinct() \|> count()` | `COUNT(DISTINCT col)` not yet implemented | `distinct()` alone, then `length()` of result array |
| `_sql(... \|> _join(select_from(type<T>), ...))` | inner `select_from` needs db handle wired inside the analyzer | omit m1 / use raw SQL string for join benchmarks |

The error messages from `sqlite_linq.das` are explicit — read them, they spell out the alternative form. Pattern matching for these lives in `modules/dasSQLITE/daslib/sqlite_linq.das` `peel_column_aggregate` and `analyze_chain`.

## Questions
- What's the right sqlite_linq chain form for aggregates (sum/min/max/average), and what operators aren't supported as `_sql` chain terminals?
- Why does `_sum(_.price)` fail in `_sql` with "can't locate variable '_'"?
- How do I express `any`/`all`/distinct-count/take-count in `_sql`?

## Questions
- What's the right sqlite_linq chain form for aggregates (sum/min/max/average), and what operators aren't supported as `_sql` chain terminals?
