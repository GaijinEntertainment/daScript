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
| 2B Ring 1 | Accumulator lane: `sum`, `min`, `max`, `average`, `long_count` with workhorse `<` / `>` for min/max scalars and `_::less` fallback for tuples/user types. `long_count` shares the count-length shortcut. | ✅ done |
| 2B Ring 2 | Early-exit lane: `first`, `first_or_default`, `any`, `all`, `contains` via `invoke($block { ... return val })`. Predicate-free `any` gets a `length(src) > 0` shortcut. | ✅ done |
| 2C Ring 3 | `take(N)` / `skip(N)` in counter/array/accumulator/early-exit lanes. Canonical chain order `[where_*][select*][skip?][take?] |> terminator`. Trailing take/skip (no explicit aggregator) → ARRAY lane with implicit `to_array`. Range-form `take(start..end)` falls through (slice operator, different semantics). Buffer-required ops (`order_by`, `distinct`, `reverse`, `group_by`, `zip`, `join`, `left_join`, `group_join`) recognized by name and emit silent fallback with future-mode markers (BufferTopN / BufferDistinct / BufferReverse / BufferGroupBy / MultiSourceZip / BufferedJoin). | ✅ done |
| 2C Ring 4 | Non-workhorse chained selects via `:=`-clone. | ⏳ |
| 2D | Fail-loudly contract — see "Planned" section below | ⏳ |
| 3+ | Buffer-required emit modes: `distinct`, `sort`/`order_by`, `reverse`, `groupby`, `zip`, `join`. Once we go array, we stay array | ⏳ |
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

**In scope:** `[where_*][select*]` (array lane) and `[where_*][select*] |> count` (counter lane). Chained `_where|_where|...` fuses via `&&`. Chained `_select|_select|...` fuses via intermediate `var v_N = projection_N` let-bindings — each next lambda's `_` is renamed straight to the prior binding's name, no expression substitution needed (which would have hit the ExprRef2Value-wrapper problem documented in `skills/das_macros.md`). Chained selects currently require all projections to be workhorse; non-workhorse intermediates would need `:=` (clone) since `<-` (move) can corrupt source for lvalue projections — deferred to Phase 2B.

**Out of scope (falls through):** `_select|_where`, `sum`, `min`, `max`, `average`, `first`, `any`, `all`, `long_count`, `_order`, `_distinct`, `_take`, `_skip`, `_zip`, `_reverse`, etc.

### Phase 2A deltas (100K rows, INTERP)

| Benchmark | Shape | m3f_old | m3f (Phase 2A) | Delta |
|---|---|---:|---:|---|
| count_aggregate | `where → count` | 5 | 4 | parity-ish (1ns improvement from `each(<array>)` peel) |
| chained_where | `where → where → count` | 17 | 6 | **2.8× faster** (fuses chained wheres into single `&&` predicate; small gain from peel + const-ref param) |
| select_count | `select → count` | 15 | 0 | **∞ faster** — when the projection is pure (`has_sideeffects == false`) and the source has length, the counter lane shortcuts to `length(src)` and elides the loop entirely. See [macro_boost::has_sideeffects](../../daslib/macro_boost.das) and `linq_fold.das:plan_loop_or_count` |
| to_array_filter | `where → select → to_array` | 11 | 10 | parity (after `each(<array>)` peel + reserve + workhorse `push`) |

Shapes outside Phase 2A scope now compile to plain linq (`m3f ≈ m3`). This is an intentional regression vs the historical `_old_fold` numbers — Boris's call ("we let it fall through unfolded, and we see performance issues. im ok being slower until we fix") as the forcing function for Phase 2B+. The previous "m3f = m3f_old (identical by construction)" baseline assumed `_fold` would dispatch to `_old_fold` on the unmatched path; Phase 2A drops that dispatch.

### Three small things that closed the to_array_filter gap

The first cut was 18% slower than the comprehension. Three independent fixes brought it to parity:

1. **Workhorse decision at macro time, not runtime.** The first emission used `static_if (typeinfo is_workhorse(projection))` inside the qmacro so the compiler picked copy- vs move-init. The projection's `_type` is already resolved when the planner runs, so the macro now reads `projection._type.isWorkhorseType` directly and emits exactly one branch — less AST, no static_if to fold away.
2. **Pre-reserve when the source has a known length.** ExprArrayComprehension lowering reserves the result array to the source's length to avoid growth reallocs; the explicit loop has to do the same explicitly. The planner emits `acc |> reserve(length(src))` when the source isn't an iterator.
3. **Peel `each(<array>)` at macro time.** The benchmark source `each(arr)` reports as `iterator<T>`, so the reserve from (2) wouldn't fire. The planner now detects `each(<expr>)` where the inner expression has length and unwraps it — the emitted loop iterates the array directly. `for (it in arr)` and `for (it in each(arr))` yield the same element refs; the wrapper iterator is incidental in fold context.

A fourth simplification dropped `emplace` from the emission entirely. emplace **moves** out of its argument and can corrupt the source when the projection returns a ref into it (e.g. `_._field`). The safe pattern is `push` for workhorse (cheap copy) and `push_clone` for non-workhorse (deep clone). No intermediate `var v = projection; emplace(v)` is needed in either case — the planner pushes the projection expression directly.

## Phase 2B Ring 1 — Accumulator lane (2026-05-16)

`_fold` now recognizes `[where_*][select*] |> {sum,min,max,average,long_count}` and emits a single-pass loop with a typed accumulator. New private dispatch infrastructure: `LinqLane` enum + `classify_terminator(name)` route each terminator to per-lane emit helpers (`emit_counter_lane`, `emit_array_lane`, `emit_length_shortcut`, **`emit_accumulator_lane`**); `plan_loop_or_count` is now an analyzer that builds the shared `whereCond` / `intermediateBinds` / `projection` state then dispatches.

**Per-op shapes:**
- `sum` — `var acc : T = default<T>; for { ...; acc += v }; return acc`. T = projection type (or element type).
- `long_count` — `var acc : int64 = 0l; for { ...; acc++ }; return acc`. Length shortcut: `int64(length(src))` when no predicate + all pure.
- `min` / `max` — `var first_iter = true; var best : T; for { ...; let v = ...; if (first_iter) { best := v; first_iter = false } elif (compare) { best := v } }; return best`.
  - **Workhorse branch** (Boris's call: best perf > emission simplicity, see `feedback_linq_fold_perf_max`): T workhorse → direct `<` / `>`; non-workhorse → `_::less(...)` to preserve user/tuple comparator overloads.
- `average` — `var sum_acc : T = default<T>; var cnt = 0; for { ...; sum_acc += v; cnt++ }; return double(sum_acc) / double(cnt)`. Returns `double` (matches linq.das line 1358); empty source → NaN.

**Empty-source semantics** match linq.das exactly: sum/long_count → 0; min/max → `default<T>` (never assigned via the first-iter flag); average → div-by-zero NaN.

**Modifier strip on accumulator type.** The element/projection type often carries `const &` (e.g. `int const &` for array elements). The accumulator must be a mutable value, so the planner strips both `flags.constant` and `flags.ref` on the cloned `accType` before emission. Without this, `var acc : int const = ...; acc += x` fails ("numeric operator '+=' left side can't be constant").

### Phase 2B Ring 1 deltas (100K rows, INTERP)

All Ring 1 ops hit single-digit ns/op with **zero allocations** (counter-lane parity).

| Benchmark | Shape | m3f_old | m3f (Ring 1) | Delta |
|---|---|---:|---:|---|
| sum_aggregate | `select → sum` | 16 | **2** | **8× faster** |
| sum_where | `where → select → sum` | 12 | **4** | **3× faster** |
| min_aggregate | `select → min` | 25 | **6** | **4.2× faster** |
| max_aggregate | `select → max` | 23 | **6** | **3.8× faster** |
| average_aggregate | `select → average` | 20 | **5** | **4× faster** |
| long_count_aggregate (new) | `where → long_count` | 15 | **4** | **3.75× faster** |

The workhorse-branch decision is decisive for min/max: emitting `v < best` directly instead of dispatching through `_::less` cuts the per-element cost roughly in half on int columns. The non-workhorse path (tuples, user types) still goes through `_::less` to preserve overload semantics — see `test_min_non_workhorse_uses_less` in `tests/linq/test_linq_fold_ast.das`.

## Phase 2B Ring 2 — Early-exit lane (2026-05-16)

`_fold` now recognizes `[where_*][select*] |> {first,first_or_default,any,all,contains}` and emits a loop wrapped in `invoke($block { for { ...; return X }; tail }, src)`. The block-level `return` yields the invoke's value as an expression without escaping the user's enclosing function (Boris confirmed the idiom: `$() { ... }` is stack-allocated, no heap). New emit helper `emit_early_exit_lane` and a separate `emit_any_empty_shortcut` for the predicate-free `any` length-bearing shortcut.

**Per-op shapes:**
- `first` — `for { ...; return val }; panic("sequence contains no elements"); return default<T>` (matches linq.das line 2383; sentinel return makes the typer happy on the post-panic line).
- `first_or_default(d)` — `let d_bound = d; for { ...; return val }; return d_bound` — eager evaluation of `d` matches linq.das line 2397 (no lazy-vs-eager divergence on observable side effects).
- `any` — loop emission with `return true` on match, `return false` in tail. Shortcut: `length(src) > 0` when no upstream where + no per-element work + length-bearing source.
- `all(pred)` — loop with `if (!pred) return false`, tail returns `true` (vacuously true on empty source).
- `contains(v)` — `let v_bound = v; for { ...; if (it == v_bound) return true }; return false` — `v` bound once at top to avoid re-evaluating an expensive argument.

**Workhorse branch deferred for `contains`**: daslang's `==` already handles tuples and user-defined `operator ==`, so no separate non-workhorse path is needed.

### Phase 2B Ring 2 deltas (100K rows, INTERP)

All Ring 2 ops hit single-digit ns/op with **zero allocations**. Early-exit cases (first hit near the front) collapse to sub-ns per element — true O(1) behavior at scale.

| Benchmark | Shape | m3f_old | m3f (Ring 2) | Delta |
|---|---|---:|---:|---|
| first_match | `where → first` | 15 | **0** | early-exit at first hit |
| first_or_default_match (new) | `where → first_or_default(d)` | 15 | **0** | same |
| any_match | `where → first_opt`/`any` | 0 | **0** | parity (already sub-ns) |
| all_match | `count(where ¬p)==0` / `all` | 24 | **3** | **8× faster** |
| contains_match (new) | `select → contains(v)` | 15 | **2** | **7.5× faster** |

`first` / `first_or_default` collapse to sub-ns/op because the where matches near the front of the array; the early-exit returns at the first hit and per-element timing measures the loop overhead per the chunk_size (100K), not per actual iteration. The same is why `any_match` was already at 0 ns/op pre-Phase-2B — `_old_fold` and m3 also bail early on first match.

## Phase 2C Ring 3 — splice-mode take/skip (2026-05-17)

`_fold` now recognizes the chain shape `[where_*][select*][skip?][take?] |> terminator` and emits the per-element work wrapped with bounded-loop counters. Skip and take guards are spliced into the per-match block (after the optional `where` filter, before the lane-specific work and intermediate projection binds). Counters live alongside the lane's accumulator in the outer invoke block.

**Emission shape (counter lane example, `where(p).skip(K).take(N).count()`):**

```
invoke($($i(src) : ...) {
    var <skipRem> = K
    var <takenCount> = 0
    var <acc> = 0
    for (<it> in <src>) {
        if (p(<it>)) {
            if (<takenCount> == N) break          // take guard
            if (<skipRem> > 0) { <skipRem>--; continue }   // skip guard
            <takenCount>++                         // before lane work — keeps reachable
                                                   // even for early-exit terminators that
                                                   // `return X` from per-match
            <acc>++
        }
    }
    return <acc>
}, <topExpr>)
```

Same skeleton across all four lanes — the per-match payload (acc++ / push_clone / `acc += val` / `return X`) is the only thing that varies.

**Chain-shape detection.** New planner state machine on `(seenSelect, seenSkip, seenTake)` accepts the canonical order and rejects reversed forms (`where` after select/skip/take, etc.) — those return null and the chain falls through to plain linq. At most one skip and one take per chain in this phase; multiple of either is a fall-through.

**Take/skip as terminator.** When `take(N).to_array()` or `skip(K).take(N).to_array()` is the chain tail, the `to_array` strip (LinqCall `skip = true` in `linqCalls`) makes the last visible call `take` or `skip`. `classify_terminator` now routes those to ARRAY lane, with the trailing take/skip captured into `takeExpr`/`skipExpr` and the lane emitting implicit-to_array.

**Buffer-required marker arms.** `is_buffer_required_op` recognizes `order_by`/`order`/`order_descending`/`order_by_descending`, `distinct`/`distinct_by`, `reverse`, `group_by`/`group_by_lazy`, `zip`, `join`/`left_join`/`group_join`. These all return null (silent fallback) with a `// TODO Phase 2X: <FutureMode>` comment naming the future emit mode. Future PRs replace each return-null with a dedicated emit path without re-walking the chain-recognition logic. AST tests `test_order_by_take_falls_through` and `test_distinct_falls_through` pin this behavior.

**Range-form `take(start..end)` falls through.** The slice-style overload has different semantics (yield elements at indices `[start, end)`); decomposing to `skip(start).take(end-start)` is correct but introduces an arithmetic dependency the planner doesn't model today. Phase 2C handles only the int-form (`takeArg._type.baseType == Type.tInt`); range form drops to plain linq.

**Reserve refinement.** When `take(N)` is present on a length-bearing source, the array-lane reserve hint tightens from `length(src)` to `min(N, length(src))` (inline `?:` — no `math::min` dep at the user's call site). Prevents over-allocating millions of slots when take cap is small.

### Phase 2C Ring 3 deltas (100K rows, INTERP)

| Benchmark | Shape | m3f_old | m3f (Phase 2C) | Delta |
|---|---|---:|---:|---|
| take_count | `take(N).to_array` (N=1000) | 0 | 0 | parity (`_old_fold`'s take iterator already bails early at N) |
| skip_take | `skip(K).take(N).to_array` (K=10, N=1000) | 23 | **0** | bounded-loop exits after K+N iterations (small vs 100K source) |
| take_sum_aggregate (new) | `select.take(N).sum` | 14 | **0** | accumulator-lane take splice |
| take_count_filtered (new) | `where.take(N).count` | 11 | **0** | counter-lane take + where splice |

Sub-ns/op on the three improved benchmarks reflects the bounded-loop nature: per-element timing normalizes to `chunk_size = 100000`, but the actual loop runs ≤ K+N times (≤ 1010 here). The win is asymptotic — `_fold` is O(K+N), `_old_fold` is O(K+N) per inner iterator + N×O(1) wrapper push.

`_old_fold`'s `take_count` at 0 ns/op already reflects iterator-fusion at the linq-runtime layer; the Phase 2C delta there is allocation count (`_fold`: 1 alloc for the result array, `_old_fold`: same with extra take-iterator wrapper). The functional Phase 2C win for that shape is structural — the splice path now emits a single fused loop where `_old_fold` chains iterator instances.

## Planned: fail-loudly contract

The current contract: when `_fold` can't splice a chain (out-of-scope terminator, buffer-required op, multiple take/skip, range-form take/skip, etc.), it falls through to plain linq — same as today's master. This is **temporary**. The planned contract (Boris design directive 2026-05-17): `_fold` will emit `macro_error("_fold: cannot splice — <reason>")` for any unsupported shape, mirroring the sqlite_linq `_sql(...)` "splice or error" contract.

When the switch lands, every `m3f` variant currently relying on silent fallback breaks. Approximate accounting from the current benchmark suite (8 affected `m3f` variants), grouped by future emit mode that would resolve each:

| Benchmark | Future mode |
|---|---|
| `distinct_count` | BufferDistinct (hash set) |
| `sort_first` | BufferTopN (order_by + early-exit) |
| `sort_take` | BufferTopN (order_by + take/skip) |
| `select_where_order_take` | BufferTopN with predicate prefix |
| `groupby_count` | BufferGroupBy (hash multi-bucket) |
| `groupby_sum` | BufferGroupBy + nested fold inside select |
| `zip_dot_product` | MultiSourceZip (2 cursors advanced lockstep) |
| `join_count` | BufferedJoin (hash-build + probe) |

The fail-loudly PR will either (a) comment out `m3f` in the affected benchmarks until the corresponding emit mode lands, or (b) deliver one or more emit modes alongside the switch. Decision deferred to that PR.

Tracking issue: the planner's `is_buffer_required_op` recognition + the named-arm `// TODO Phase 2X: <FutureMode>` markers are the in-code TODOs.

## Operator-coverage checklist (parity tests)

The 24 benchmarks above cover the most common shapes. The end-game target is one benchmark per `_fold`-applicable scenario in the broader `tests/linq/` operator suite. Tracking the long-tail coverage below; PRs that add splice support for new operators should add a benchmark here if not already present.

| Source test file | Operator group | Covered by benchmark | Status |
|---|---|---|---|
| `test_linq.das` | comprehension basics | count_aggregate, sum_aggregate | ✅ |
| `test_linq_aggregation.das` | count/sum/min/max/avg/aggregate | count/sum/min/max/average_aggregate, sum_where, long_count_aggregate | ✅ core; `aggregate(seed, fn)` ⏳ |
| `test_linq_querying.das` | any/all/contains | any_match, all_match, contains_match | ✅ core |
| `test_linq_transform.das` | select/select_many/zip | to_array_filter, zip_dot_product | ✅ select/zip; `select_many` ⏳ |
| `test_linq_sorting.das` | order/order_by/reverse | sort_first, sort_take, select_where_order_take | ✅ ascending; `order_descending` + `reverse` ⏳ |
| `test_linq_group_by.das` | group_by/group_by_lazy/having | groupby_count, groupby_sum | ✅ basic; `having_` ⏳ |
| `test_linq_join.das` | join/left_join/right_join/full_outer/cross | join_count | ✅ inner; outer joins + cross ⏳ |
| `test_linq_partition.das` | take/skip/take_while/skip_while/chunk | take_count, skip_take, take_sum_aggregate, take_count_filtered | ✅ take/skip in splice lanes; `_while` + `chunk` ⏳ |
| `test_linq_set.das` | distinct/union/except/intersect/unique | distinct_count | ✅ distinct; set ops ⏳ |
| `test_linq_element.das` | first/last/single/element_at + _or_default | first_match, first_or_default_match | ✅ first/first_or_default; last/single/element_at ⏳ |
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
