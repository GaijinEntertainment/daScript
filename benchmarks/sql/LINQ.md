# linq_fold — splice-mode macro rewrite

Project notes and progress for the `daslib/linq_fold` macro family, modeled after the dispatch-macro architecture in `modules/dasSQLITE/daslib/sqlite_linq`. Other `.md` files on this subject (operator-specific design notes, post-mortems, etc.) get filed in this directory alongside this one.

## What this is

The current `_fold(...)` macro in linq_boost wraps LINQ pipelines into intermediate `array<T>` per stage and pattern-matches a small set of common shapes (`where+count`, `where+select`, `select+where`, `order+distinct`, `where`, `select`) for ad-hoc fusion. Everything else falls through to a default emitter that builds nested `var pass_N <- pass_(N-1) |> next(...)` — one fresh array per stage, every predicate called via lambda dispatch.

The goal is a planner-driven dispatch macro that emits one fused for-loop with predicates inlined (splice mode), materializing only when an operator genuinely needs random access. Three output modes:

1. **Splice** (default): producer body inlined into consumer's loop. Zero allocation, zero per-element dispatch.
2. **Array intermediate**: when a downstream op needs random access / multi-pass / length (`sort`, `reverse`, `distinct`, `groupby`). Once we go array, we stay array (iterating an array is faster than iterating an iterator).
3. **Helper-call fallback**: when splice can't apply at all (escape into `let`, opaque source). Emits calls to named helper functions in `linq_fold`.

Lambda inlining is best-effort: literal `@(x) => expr` at the call site → splice the body; otherwise → call.

See `~/.claude/plans/keen-hopping-balloon.md` for the long-form plan.

## Phase status

| Phase | Description | Status |
|---|---|---|
| 0 | Rename `_fold` → `_old_fold` in linq_boost; extract `_fold` and `_old_fold` into new `daslib/linq_fold.das` module; `linq_boost` `require linq_fold public` for re-export | ✅ done |
| 1 | Benchmark suite: 24 files under `benchmarks/sql/`, each 4-way (m1 `_sql` / m3 plain linq / m3f_old `_old_fold` / m3f `_fold`) at 100K rows; baseline numbers captured | ✅ done |
| 2A | Loop planner — `_fold` emits explicit for-loops for `[where_*][select?]` (array lane) and `[where_*][select?] |> count` (counter lane); anything else falls through unfolded. No comprehensions, no dispatch back to `_old_fold`. | ✅ done |
| 2B | Aggregate accumulators: `sum`, `min`, `max`, `average`, `first`, `any`, `all`, `long_count`. Also `take`/`skip` in counter/array lane and chained-`_select|_select` fusion (needs `ExprRef2Value`-aware projection substitution) | ⏳ next |
| 3+ | Buffer-required operators: `distinct`, `sort`, `reverse`, `groupby`, `zip`, `join`. Once we go array, we stay array | ⏳ |
| 4 | Final coverage pass + docs; full 4-way comparison table refresh; parity-test sweep | ⏳ |

## Baselines (100K rows, INTERP mode)

Captured 2026-05-16 on commit `e691abe1b` + foundation PR. ns/op is **per element** (chunk_size = n = 100K), so 30 ns/op means ~3ms for the full operation. Smaller is better. m3f and m3f_old are intentionally identical in this PR — they diverge once Phase 2 lands.

Notation: `—` means the variant is not applicable for this benchmark (operator has no clean form in that mode).

| Benchmark | Shape | m1 (sql) | m3 (linq) | m3f_old | m3f |
|---|---|---:|---:|---:|---:|
| count_aggregate | `where → count` | 29 | 29 | 5 | 5 |
| sum_aggregate | `select → sum` | 29 | 30 | 8 | 8 |
| sum_where | `where → select → sum` | 33 | 43 | 12 | 12 |
| min_aggregate | `select → min` | 30 | 38 | 25 | 25 |
| max_aggregate | `select → max` | 31 | 36 | 23 | 23 |
| average_aggregate | `select → average` | 30 | 34 | 20 | 20 |
| first_match | `where → first` | 0\* | 28 | 15 | 15 |
| any_match | `where → first_opt`/`any` | 0\* | 0\* | 0\* | 0\* |
| all_match | `count(where ¬p)==0` / `all` | 27 | 20 | 24 | 25 |
| to_array_filter | `where → select → to_array` | 70 | 43 | 11 | 11 |
| take_count | `take → to_array` | 3 | 0\* | 0\* | 0\* |
| skip_take | `skip → take → to_array` | 0\* | 16 | 23 | 23 |
| distinct_count | `select → distinct → to_array` | 41 | 43 | 33 | 33 |
| sort_first | `order_by → first` | 37 | 2170 | 2206 | 2238 |
| sort_take | `order_by → take` | 38 | 2188 | 2247 | 2269 |
| groupby_count | `group_by → select(_, length)` | 140 | 70 | 76 | 76 |
| groupby_sum | `group_by → select(_, sum)` | 172 | 101 | 107 | 107 |
| chained_where | `where → where → count` | 36 | 45 | 17 | 17 |
| zip_dot_product | `zip → select → sum` | — | 53 | 37 | 37 |
| join_count | `join → count` | —\*\* | 116 | 121 | 122 |
| count_aggregate (existing) | `where → count` | 29 | 29 | 5 | 5 |
| select_where (existing) | `where → to_array` | 7 | 50 | 12 | 12 |
| select_where_order_take (existing) | `where → order_by → take` | 36 | 1024 | 1007 | 1014 |
| indexed_lookup (existing) | `where id==k → count` | 1460\*\*\* | 2003299 | 336129 | 328207 |

\* Sub-nanosecond per element — early-exit operation hits answer in O(1) regardless of N; per-element timing collapses to 0/near-0 noise.

\*\* `_sql` join requires the inner `select_from` to bind the db handle inside the `_sql` analyzer; that wiring isn't exposed for direct authoring without more sqlite_linq plumbing. Foundation PR omits m1 here; revisit if/when needed.

\*\*\* PK lookup vs linear scan — inverse-asymmetry benchmark. SQLite's b-tree index wins by ~5 orders of magnitude over the array linear scan. The interesting comparison is m3 vs m3f_old: the existing fold's `where+count` fusion already gives ~6× speedup.

### Reading the table

- **m1 vs m3** shows the SQLite-vs-in-memory-LINQ cost gap. SQL wins on `indexed_lookup` (b-tree) and on sorted-take patterns (engine partial-sort + LIMIT). Arrays win on raw aggregates where the SQL overhead exceeds the in-memory work.
- **m3 vs m3f_old** shows what the *current* `_fold` macro already achieves. Big wins on the patterns it explicitly recognizes (`where+count` 6×, `where+select+to_array` ~4×, `chained_where+count` 2.6×). Negligible difference where it falls through to the default emitter.
- **m3f vs m3f_old** is the target of Phase 2+. Each PR in the splice series adds a path for one operator family and updates this table with the new ratio.

## Phase 2A — Loop planner (2026-05-16)

`_fold` now emits explicit for-loops for two narrow shape families instead of comprehensions. Anything outside scope falls through unfolded to raw linq (no dispatch to `_old_fold` or `fold_linq_default`).

**In scope:** `[where_*][select?]` (array lane) and `[where_*][select?] |> count` (counter lane). Chained `_where|_where|...` fuses via `&&`; single `_select` composes; chained `_select|_select` falls through (needs ExprRef2Value-aware substitution, deferred to Phase 2B).

**Out of scope (falls through):** `_select|_where`, `sum`, `min`, `max`, `average`, `first`, `any`, `all`, `long_count`, `_order`, `_distinct`, `_take`, `_skip`, `_zip`, `_reverse`, etc.

### Phase 2A deltas (100K rows, INTERP)

| Benchmark | Shape | m3f_old | m3f (Phase 2A) | Delta |
|---|---|---:|---:|---|
| count_aggregate | `where → count` | 5 | 5 | parity (same counter loop) |
| chained_where | `where → where → count` | 17 | 8 | **2.1× faster** (fuses chained wheres into single `&&` predicate) |
| select_count | `select → count` | 15 | 2 | **7.5× faster** (counter lane ignores projection; no array materialization) |
| to_array_filter | `where → select → to_array` | 11 | 13 | ~18% slower (explicit loop vs comprehension lowering) |

Shapes outside Phase 2A scope now compile to plain linq (`m3f ≈ m3`). This is an intentional regression vs the historical `_old_fold` numbers — Boris's call ("we let it fall through unfolded, and we see performance issues. im ok being slower until we fix") as the forcing function for Phase 2B+. The previous "m3f = m3f_old (identical by construction)" baseline assumed `_fold` would dispatch to `_old_fold` on the unmatched path; Phase 2A drops that dispatch.

### Why `to_array_filter` regressed

Comprehensions `[for (it in src) where p; expr]` lower through the compiler's dedicated `ExprArrayComprehension` path, which appears to compose more aggressively with array growth than an emitted-by-macro explicit loop with `static_if (is_workhorse) var val = expr; arr.emplace(val)`. The 18% gap is small relative to the 2-7× wins elsewhere; Phase 2B can profile and tune (likely pre-reserving the result array or switching to `push` for workhorse).

## Operator-coverage checklist (parity tests)

The 24 benchmarks above cover the most common shapes. The end-game target is one benchmark per `_fold`-applicable scenario in the broader `tests/linq/` operator suite. Tracking the long-tail coverage below; PRs that add splice support for new operators should add a benchmark here if not already present.

| Source test file | Operator group | Covered by benchmark | Status |
|---|---|---|---|
| `test_linq.das` | comprehension basics | count_aggregate, sum_aggregate | ✅ |
| `test_linq_aggregation.das` | count/sum/min/max/avg/aggregate | count/sum/min/max/average_aggregate, sum_where | ✅ core; `aggregate(seed, fn)` ⏳ |
| `test_linq_querying.das` | any/all/contains | any_match, all_match | ✅ core; `contains` ⏳ |
| `test_linq_transform.das` | select/select_many/zip | to_array_filter, zip_dot_product | ✅ select/zip; `select_many` ⏳ |
| `test_linq_sorting.das` | order/order_by/reverse | sort_first, sort_take, select_where_order_take | ✅ ascending; `order_descending` + `reverse` ⏳ |
| `test_linq_group_by.das` | group_by/group_by_lazy/having | groupby_count, groupby_sum | ✅ basic; `having_` ⏳ |
| `test_linq_join.das` | join/left_join/right_join/full_outer/cross | join_count | ✅ inner; outer joins + cross ⏳ |
| `test_linq_partition.das` | take/skip/take_while/skip_while/chunk | take_count, skip_take | ✅ take/skip; `_while` + `chunk` ⏳ |
| `test_linq_set.das` | distinct/union/except/intersect/unique | distinct_count | ✅ distinct; set ops ⏳ |
| `test_linq_element.das` | first/last/single/element_at + _or_default | first_match | ✅ first; last/single/element_at ⏳ |
| `test_linq_concat.das` | concat/prepend/append | — | ⏳ |
| `test_linq_generation.das` | range/repeat/etc. | — | ⏳ |
| `test_linq_bugs.das` | regression cases | — | ⏳ as bugs surface |

## How to run

```sh
# All benchmarks at 100K (the suite default)
./bin/daslang dastest/dastest.das -- --bench --test benchmarks/sql --test-names none

# One benchmark
./bin/daslang dastest/dastest.das -- --bench --test benchmarks/sql/count_aggregate.das --test-names none

# Filter by [benchmark] function-name prefix
./bin/daslang dastest/dastest.das -- --bench --bench-names sum_ --test benchmarks/sql --test-names none
```

dastest reports `ns/op` in INTERP mode by default. To bump dataset size as the splice work progresses (the interpreter is the bottleneck; 100K is a fast-iteration target, larger N gives more accurate asymptotics), change the `n` argument in each file's `[benchmark]` entries. AOT/JIT modes will be added when the splice path lands and benefits from compilation.

## Design decisions

**`_old_fold` lives alongside `_fold` in `linq_fold`, not in `linq_boost`.** Both macros share the entire dispatch infrastructure (`linqCalls`, `g_foldSeq`, `fold_*`, `flatten_linq`, `fold_linq_default`). Keeping them in one module avoids duplication; the only difference today is the macro-name string passed into `fold_linq_default`'s recursive sub-fold call.

**Recursive macro-name is parameterized.** `fold_linq_default(expr, recursiveMacroName)` — `_fold` passes `"_fold"`, `_old_fold` passes `"_old_fold"`. This keeps the frozen baseline truly frozen once `_fold` diverges in Phase 2+: when `_fold` starts emitting splice loops, `_old_fold` keeps emitting the historical comprehension/invoke shape because its recursive sub-folds still target `_old_fold`.

**100K rows.** daslang is interpreter-first; 100K gives sub-second-per-variant benchmark turnaround and clearly shows the asymmetries we care about. Bump later if AOT/JIT numbers warrant.

**`PERF009` suppression in `fold_linq_default`.** The macro's `var pass_N = call` + later `return <- pass_N` pattern triggers PERF009 on single-pass chains (e.g. `take_count`). Rewriting to direct `return <- call` would change `_old_fold`'s baseline; we suppress inline at the qmacro_expr emission site and document why.

**Benchmark variants where SQL has no clean form.** `zip` (not a relational op), `_all(pred)` (no direct `_all` chain terminal in sqlite_linq), `join` with inner-select-from (wiring not exposed), `distinct |> count` (no `COUNT(DISTINCT col)` yet), `take/skip` before aggregate (LIMIT/OFFSET semantics conflict with aggregate-collapse). We either reformulate to a SQL-friendly shape (`count(where ¬p)` for all_match), omit the m1 column (zip, join), or terminate the chain in `to_array` instead of an aggregate (take/skip/distinct).
