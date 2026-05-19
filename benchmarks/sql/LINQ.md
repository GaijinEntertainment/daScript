# linq_fold — splice-mode macro rewrite

Project notes and progress for the `daslib/linq_fold` macro family, modeled after the dispatch-macro architecture in `modules/dasSQLITE/daslib/sqlite_linq`. Other `.md` files on this subject (operator-specific design notes, post-mortems, etc.) get filed in this directory alongside this one.

## What this is

`_fold(chain)` is a three-tier cascade — always-safe, never breaks semantics, only ever faster:

1. **Splice** (tier 1): a fused for-loop with predicates and projections inlined directly. Zero per-element lambda dispatch, zero intermediate iterators. Two planners feed this tier:
   - **`plan_order_family`** — handles chains containing any of `order` / `order_by` / `order_descending` / `order_by_descending`, optionally with `take(K)` and/or a `where_*` prefilter. Dispatches to `top_n*` helpers or emits a fused prefilter loop + `order*_inplace` on a buffer.
   - **`plan_loop_or_count`** — handles `[where_*][select*][skip?][take?]` followed by a recognized terminator (count / sum / min / max / average / long_count / first / first_or_default / any / all / contains / to-array). Includes the where-after-select arm via `replaceVariablePeeling` for the peel-aware substitution required on typed AST.
2. **`fold_linq_default`** (tier 2): an array-shape pipeline (`call → array → call → array`) with `_inplace` variants reusing the same buffer and explicit `delete` of the previous stage. Used when no splice arm matches but the chain has linq operators.
3. **Raw clone** (tier 3): passthrough when `flatten_linq` finds no recognized linq operators in the chain at all.

Lambda inlining is best-effort: literal `@(x) => expr` at the call site → splice the body; otherwise → call.

See `~/.claude/plans/keen-hopping-balloon.md` and `~/.claude/plans/enumerated-baking-elephant.md` for the long-form plans.

## Phase status

| Phase | Description | Status |
|---|---|---|
| 0 | Rename `_fold` → `_old_fold` in linq_boost; extract `_fold` and `_old_fold` into new `daslib/linq_fold.das` module; `linq_boost` `require linq_fold public` for re-export | ✅ done |
| 1 | Benchmark suite: 24 files under `benchmarks/sql/`, each 4-way (m1 `_sql` / m3 plain linq / m3f_old `_old_fold` / m3f `_fold`) at 100K rows; baseline numbers captured | ✅ done |
| 2A | Loop planner — `_fold` emits explicit for-loops for `[where_*][select?]` (array lane) and `[where_*][select?] |> count` (counter lane); anything else falls through unfolded. No comprehensions, no dispatch back to `_old_fold`. | ✅ done |
| 2B Ring 1 | Accumulator lane: `sum`, `min`, `max`, `average`, `long_count` with workhorse `<` / `>` for min/max scalars and `_::less` fallback for tuples/user types. `long_count` shares the count-length shortcut. | ✅ done |
| 2B Ring 2 | Early-exit lane: `first`, `first_or_default`, `any`, `all`, `contains` via `invoke($block { ... return val })`. Predicate-free `any` gets a `length(src) > 0` shortcut. | ✅ done |
| 2C Ring 3 | `take(N)` / `skip(N)` in counter/array/accumulator/early-exit lanes. Canonical chain order `[where_*][select*][skip?][take?] |> terminator`. Trailing take/skip (no explicit aggregator) → ARRAY lane with implicit `to_array`. Range-form `take(start..end)` falls through (slice operator, different semantics). Buffer-required ops (`order_by`, `distinct`, `reverse`, `group_by`, `zip`, `join`, `left_join`, `group_join`) recognized by name and emit silent fallback with future-mode markers (BufferTopN / BufferDistinct / BufferReverse / BufferGroupBy / MultiSourceZip / BufferedJoin). | ✅ done |
| 2C Ring 4 | Non-workhorse chained selects via `:=`-clone. | ✅ done |
| 3 Phase 0 | `<algorithm>` sort-family bindings — `partial_sort`, `nth_element`, `make_heap`/`push_heap`/`pop_heap`. Both typed (19 workhorse types) and any-cblock paths (user structs via `das_qsort_r.h` introselect + binary-heap templates). `daslib/sort_boost.das` user-facing wrappers + `q*` dispatcher macros. `daslib/linq.das` `top_n` / `top_n_by` family (array + iterator sources). C++ tests, daslang tests (53/53), 6 benchmarks, doc grouping, `33_algorithm` tutorial expansion. Unblocks BufferTopN. | ✅ done |
| 1 cascade | Retire `_old_fold`, drop the 7 `g_foldSeq` patterns, restructure `_fold` into a three-tier cascade (splice → `fold_linq_default` → raw clone). Cascade is always-safe — `_fold(chain)` is observationally equivalent to `chain`. `_select|_where` etc. cases that used to fall to raw clone now cascade to tier 2 array-shape with `_inplace` reuse. Phase 2D "fail-loudly contract" scrapped — always-safe cascade obviates it. | ✅ done |
| 3a/b/c | BufferTopN splice arm via new `plan_order_family` planner: `[where_*]* + order/order_by/order_descending/order_by_descending [+ take(K)]`. No `take` → direct call to `order*` helper (or fused prefilter loop + `order*_inplace` when where is present). With `take(K)` → dispatch to `top_n[_by][_descending]` (or fused prefilter loop + `top_n*` on the buffer when where is present). New `top_n_by_descending` + `top_n_descending` library helpers added to `daslib/linq.das` (4 functions, mirrors of `top_n_by` / `top_n` with flipped comparator). | ✅ done |
| 3d | `select + where + terminator` splice arm via `replaceVariablePeeling` (new helper in `daslib/templates_boost.das`). Substitutes the projection into the where predicate, peeling the typer-inserted ExprRef2Value wrappers that would otherwise be left orphaned around a non-reference value. Bails to tier 2 when the projection has side effects (would double-evaluate). Lands all four terminator lanes (ARRAY / COUNTER / ACCUMULATOR / EARLY_EXIT). | ✅ done |
| 3+ BufferReverse | `[where_*]?[select?] \|> reverse [\|> take(N)]? [\|> {to_array, count, first, first_or_default}]?` — single-pass prefilter into buffer + `reverse_inplace` + optional `resize(min(N,len))`; count/first lanes elide the buffer entirely (counter or last-survivor tracker). | ✅ |
| 3+ BufferDistinct | `[where_*]?[select?] \|> distinct[_by(key)] [\|> take(N)]? [\|> {to_array, count, sum, long_count, …}]?` — streaming dedup via `table<unique_key>` integrated into the prefilter loop; take(N) → break-after-N early-exit (guard placed BEFORE the consume site so non-positive N short-circuits); count terminator elides the buffer and returns `length(seen)`. | ✅ |
| 3+ BufferGroupBy | `src \|> group_by[_lazy](key) \|> select(group_proj) [\|> {to_array, count}]?` — incremental-aggregate fusion: emits `table<unique_key; tuple<KT; AccT>>` updated per element. Recognized reducers: `_._1 \|> {length, count, sum, long_count}`. Bare + named-tuple wrap `(K=_._0, V=_._1\|>reducer)` both supported. Bails to tier 2 for inner-select-sum, multi-reducer, `having_*`, **or any upstream `where_*`/`select*` between source and group_by** (upstream fusion deferred). | ✅ core |
| 3+ Buffer remainder | MultiSourceZip, BufferedJoin, inner-select-sum (groupby_sum gap), group_by min/max/first/average reducers, multi-reducer named tuples. Currently cascade to tier 2 array-shape. | ⏳ |
| 4 | Final coverage pass + docs; full 3-way comparison table refresh; parity-test sweep | ⏳ |

## Baselines (100K rows, INTERP mode)

Foundation captured 2026-05-16 on commit `e691abe1b`. Phase 1 cascade + Phase 3 splice arms (PR retiring `_old_fold`) refresh the m3f column with new splice numbers — see "Headline benchmarks" below for the refreshed delta. ns/op is **per element** (chunk_size = n = 100K), so 30 ns/op means ~3ms for the full operation. Smaller is better.

Notation: `—` means the variant is not applicable for this benchmark (operator has no clean form in that mode).

| Benchmark | Shape | m1 (sql) | m3 (linq) | m3f (foundation) |
|---|---|---:|---:|---:|
| count_aggregate | `where → count` | 29 | 29 | 5 |
| sum_aggregate | `select → sum` | 29 | 30 | 8 |
| sum_where | `where → select → sum` | 33 | 43 | 12 |
| min_aggregate | `select → min` | 30 | 38 | 25 |
| max_aggregate | `select → max` | 31 | 36 | 23 |
| average_aggregate | `select → average` | 30 | 34 | 20 |
| first_match | `where → first` | 0\* | 28 | 15 |
| any_match | `where → first_opt`/`any` | 0\* | 0\* | 0\* |
| all_match | `count(where ¬p)==0` / `all` | 27 | 20 | 25 |
| to_array_filter | `where → select → to_array` | 70 | 43 | 11 |
| take_count | `take → to_array` | 3 | 0\* | 0\* |
| skip_take | `skip → take → to_array` | 0\* | 16 | 23 |
| distinct_count | `select → distinct → to_array` | 41 | 43 | 33 → **15** (this PR) |
| sort_first | `order_by → first` | 37 | 2170 | 2238 |
| sort_take | `order_by → take` | 38 | 2188 | 2269 |
| groupby_count | `group_by → select(_, length)` | 140 | 70 | 76 → **33** (this PR) |
| groupby_sum | `group_by → select(_, select → sum)` | 172 | 101 | 107 (deferred — inner-select-sum) |
| chained_where | `where → where → count` | 36 | 45 | 17 |
| zip_dot_product | `zip → select → sum` | — | 53 | 37 |
| join_count | `join → count` | —\*\* | 116 | 122 |
| select_where (existing) | `where → to_array` | 7 | 50 | 12 |
| select_where_order_take (existing) | `where → order_by → take` | 36 | 1024 | 1014 |
| indexed_lookup (existing) | `where id==k → count` | 1460\*\*\* | 2003299 | 328207 |

\* Sub-nanosecond per element — early-exit operation hits answer in O(1) regardless of N; per-element timing collapses to 0/near-0 noise.

\*\* `_sql` join requires the inner `select_from` to bind the db handle inside the `_sql` analyzer; that wiring isn't exposed for direct authoring without more sqlite_linq plumbing. Foundation PR omits m1 here; revisit if/when needed.

\*\*\* PK lookup vs linear scan — inverse-asymmetry benchmark. SQLite's b-tree index wins by ~5 orders of magnitude over the array linear scan. The interesting comparison is m3 vs m3f_old: the existing fold's `where+count` fusion already gives ~6× speedup.

### Reading the table

- **m1 vs m3** shows the SQLite-vs-in-memory-LINQ cost gap. SQL wins on `indexed_lookup` (b-tree) and on sorted-take patterns (engine partial-sort + LIMIT). Arrays win on raw aggregates where the SQL overhead exceeds the in-memory work.
- **m3 vs m3f** shows what `_fold`'s splice cascade gains over plain LINQ. The headline shapes that now splice (after the Phase 1+3 cascade PR): order-family + take → `top_n*` dispatch, where + order [+ take] → fused prefilter + sort, select + where → peel-substituted fused loop, plus all the existing `[where_*][select*][skip?][take?]` patterns from Phase 2.

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

## Phase 2C Ring 4 — chained-select clone via `:=` (2026-05-17)

Pre-Ring-4, the planner rejected any chained `_select|_select|...` chain whose previous projection had a non-workhorse type. Reason given in the source: `<-` (move) corrupts source for lvalue projections like `_._field`. The rejection guard (`prevWorkhorse=false → return null`) was a Phase 2B placeholder.

Resolution in Ring 4 follows from a Boris correctness observation: **`:=` is safe on every type** — byte copy on workhorse, deep-clone on non-workhorse — so a single emission shape (`var $i(bind) := $e(projection)`) covers both cases. The workhorse / non-workhorse branch in chained-bind emission is removed entirely; chained selects of any type now splice through one path.

Concretely: `each(arr)._select(ComplexType(a = [_*2]))._select(_.a[0]).sum()` previously fell through to plain linq (iterator chain + allocation); now splices to a single fused for-loop.

### Workhorse-branch audit

After Ring 4, two workhorse-type branches remain in `linq_fold.das`. Both are intentional:

1. **`fold_select_where` (line ~392), `static_if (typeinfo is_workhorse($e(selectExpr)))`** — used exclusively by `_old_fold`'s frozen baseline path via `g_foldSeq`. Not touched here; changing it would alter the frozen `_old_fold` output and invalidate the `m3f_old` benchmark column.
2. **`min_max_compare` (line ~746) + caller (line ~933)** — perf-critical. Workhorse types use `<` / `>` directly (single-instruction compare); non-workhorse falls back to `_::less` so user/tuple comparator overloads still apply. Boris's design directive 2026-05-16 explicitly mandates keeping this branch (≈2× per-element on int columns; see PR #2696 numbers).

No further workhorse branches in the splice path.

### Ring 4 deltas

Ring 4 is a correctness gate (chained non-workhorse selects now splice instead of falling through), not a per-benchmark improvement on the existing 100K suite. Coverage tracked via test `test_chained_non_workhorse_select` in `tests/linq/test_linq_fold.das` (3 subtests: int → ComplexType → int → sum / where + ComplexType chain + sum / workhorse → ComplexType → workhorse → max).

## Phase 1 — three-tier cascade + `_old_fold` retirement (this PR)

`_fold(chain)` is now a three-tier cascade. Tier 1 splice handles the hot patterns; tier 2 (`fold_linq_default`, the body that used to power `_old_fold`) emits an array-shape pipeline with `_inplace` reuse and explicit `delete`; tier 3 is a raw `clone_expression` passthrough. All three tiers preserve semantics — `_fold(chain)` is observationally equivalent to `chain`, just faster when patterns match. **Always safe to apply.**

This obviates the previously-planned "fail-loudly contract" (Phase 2D) — the cascade always produces a valid output, so there's no need for explicit error emission on unsupported shapes.

The 7 specific `FoldSequence` patterns (`fold_where_count`, `fold_where_select`, `fold_select_where`, `fold_where`, `fold_select`, `fold_order_distinct` × 2) and their `g_foldSeq` dispatch table are deleted — splice arms (existing + new in Phase 3 below) cover every shape they recognized. The `_old_fold` macro itself is deleted; the m3f_old benchmark column is dropped from all 29 files.

## Phase 3 — order-family splice + select+where peel (this PR)

**New planner: `plan_order_family`.** Called before `plan_loop_or_count` in the cascade. Recognizes chains containing any of `order` / `order_by` / `order_descending` / `order_by_descending`, optionally with one `take(K)`, optionally with `where_*` prefilters:

| Chain shape | Emission |
|---|---|
| `arr \|> order[_descending]?` (bare) | Direct call: `order[_descending](arr)` |
| `arr \|> order_by[_descending]?(key)` (bare) | Direct call: `order_by[_descending](arr, key)` |
| `src \|> order[_descending]? \|> take(K)` | `top_n[_descending](src, K)` (existing helpers from PR #2707) |
| `src \|> order_by[_descending]?(key) \|> take(K)` | `top_n_by[_descending](src, K, key)` (new descending helpers in this PR) |
| `src \|> where_*(p)+ \|> order[_by]?[_descending]?[(key)]` | Fused: prefilter into pre-allocated buffer, then `order*_inplace` |
| `src \|> where_*(p)+ \|> order[_by]?[_descending]?[(key)] \|> take(K)` | Fused: prefilter into buffer, then `top_n*` on the buffer |

New `top_n_by_descending` (array + iterator) + `top_n_descending` (array + iterator) added to `daslib/linq.das` — mirror `top_n_by` / `top_n` with flipped comparator (partial_sort + reversed less for array; bounded min-heap for iterator).

**New splice arm: `select + where + terminator`.** Previously rejected by `plan_loop_or_count` (where-after-select hit the ExprRef2Value substitution blocker). Unblocked via the new `replaceVariablePeeling` helper in `daslib/templates_boost.das` — substitutes the projection into the where predicate, peeling the typer-inserted `ExprRef2Value` wrapper as part of the substitution (mirrors `ast_match`'s `qm_peel_ref2value` pattern). Bails to tier 2 cascade when the projection has side effects (would double-evaluate, once in the substituted where and again at the terminator emission). Lands all four lanes: ARRAY (to_array / bare), COUNTER (count), ACCUMULATOR (sum / min / max / avg / long_count), EARLY_EXIT (first / first_or_default / any / all / contains).

**Concurrent runtime fix:** [`src/builtin/module_builtin_runtime_sort.cpp:84`](../../src/builtin/module_builtin_runtime_sort.cpp#L84) — `builtin_sort_string` switched from unqualified `sort()` (= `std::sort` via `using namespace std`) to `das_sort` (the in-tree block-partition pdqsort from PR #2707). This is the runtime path `order_by<string>` takes; on Linux/libstdc++ users see the same ~1.5× sort speedup PR #2707 delivered for typed sorts.

### Headline benchmarks (100K rows, INTERP, this PR)

Refreshed m3f column after Phase 1 cascade + Phase 3 splice arms land. m3 is plain LINQ baseline; m3f is `_fold(...)` over the same chain.

| Benchmark | Shape | m1 (sql) | m3 (linq) | m3f (this PR) | Win |
|---|---|---:|---:|---:|---:|
| order_take_desc | `order_by_desc → take(K)` | 38 | 698 | **56** | **12.5×** |
| sort_take | `order_by → take(K)` | 38 | 713 | **56** | **12.7×** |
| select_where_order_take | `where → order_by → take(K)` | 36 | 354 | **39** | **9.1×** |
| select_where_count | `select → where → count` (NEW: peel) | 32 | 57 | **5** | **11.4×** |
| select_where | `where → select → to_array` | 191 | 28 | **11** | **2.5×** |
| bare_order_where | `where → order_by` (no take) | 273 | 357 | **340** | **1.05×** |
| chained_where | `where → where → count` | 36 | 45 | **6** | **7.5×** |
| sum_where | `where → select → sum` | 32 | 44 | **4** | **11×** |

The order+take rows (`order_take_desc`, `sort_take`, `select_where_order_take`) come within ~1.5× of SQLite's index-aware plans by dispatching `_fold` directly to the `top_n_by[_descending]` partial-sort helpers from PR #2707 (asc dispatches to existing; desc is new in this PR). The `bare_order_where` win is small because full sort dominates — the splice saves only one intermediate allocation, not the sort cost.

`select_where_count` is the first **select+where** splice landing: previously rejected by the planner (the typer-inserted `ExprRef2Value` wrapper around `it` orphaned during substitution → `30921: can only dereference a reference`). The new `replaceVariablePeeling` helper in `templates_boost.das` peels the wrapper as part of the substitution, mirroring `ast_match`'s `qm_peel_ref2value`. All four terminator lanes (array / counter / accumulator / early-exit) covered.

### Headline benchmarks (100K rows, INTERP, single-eval splice PR)

Follow-up to the order-family + select+where landing. Closes two single-eval gaps the previous PR documented as `KNOWN PERF GAP`:

**Gap 1 — comparator key double-call inside `partial_sort`.** Before this PR, `top_n_by(arr, K, key)` ran `_::less(key(v1), key(v2))` per comparison → 2 indirect lambda dispatches per `cmp`. For pure single-expression keys (the common case, e.g. `$(_) => _.price`), the planner now inlines the key body twice into a comparator block and dispatches to the new `top_n_by_with_cmp(arr, K) <| <cmp_block>` library entry — zero per-comparison lambda dispatch. Descending direction is encoded by flipping the comparator arg order (`less(body[v2], body[v1])`), eliminating the secondary wrapper-lambda the `_descending` family used. Falls back to keyed `top_n_by` when the key has side effects or isn't a single-expression lambda.

**Gap 2 — projection double-eval in `select + where + terminator`.** Phase 3d inlined `projection` into `predicate` via peel-substitution; lane emitters then *also* cloned `projection` into `valueExpr` → projection evaluated twice per element on ARRAY / ACCUMULATOR / EARLY_EXIT lanes (COUNTER unaffected — no body use). Fix: the where-after-select arm now binds `projection` to a fresh local via a new `preConditionStmts` slot (evaluated per-element, OUTSIDE the if-wrap), then rewrites `projection` to reference that bind. Both predicate (via peel) and valueExpr (via clone) share the single eval. Side-effecty projections still bail to tier 2 (moving them outside the if would visibly fire side effects on filter-rejected elements). COUNTER lane is explicitly excluded — the dedup has no benefit there and the bind decl would regress the single-stmt fast path.

| Benchmark | Shape | m1 (sql) | m3 (linq) | m3f (prev PR) | m3f (this PR) | Win |
|---|---|---:|---:|---:|---:|---:|
| sort_take | `order_by → take(K)` | 38 | 710 | 56 | **27** | **2.1× over prev / 26× over m3 / faster than m1 SQL** |
| order_take_desc | `order_by_desc → take(K)` | 38 | 704 | 56 | **27** | **2.1× over prev / 26× over m3 / faster than m1 SQL** |
| select_where_order_take | `where → order_by → take(K)` | 36 | 356 | 39 | **24** | **1.6× over prev / 15× over m3 / faster than m1 SQL** |
| select_where_sum (NEW Gap 2) | `select → where → sum` | 37 | 59 | — | **7** | **8.4× over m3 / 5.3× over m1 SQL** |
| select_where_count (regression check) | `select → where → count` (COUNTER, dedup off) | 32 | 58 | 5 | **5** | unchanged (correctly excluded from dedup) |

The sort/order rows now BEAT `m1` SQLite by ~30%. PR #2707 closed the comparator-throughput gap vs SQL; this PR's inline-key splice closes the per-iteration lambda dispatch gap.

**Parser bonus:** the multi-arg `$($i(a) : T, $i(b) : T) { ... }` qmacro form failed parse with `30701: block argument is already declared MACRO``TAG` because the parser stamped every `$i(...)` in block-arg position with the literal placeholder name and dup-checked them before macro tag resolution. Fixed in `src/parser/parser_impl.cpp:885` by skipping the dup check when `name_at.tag != nullptr` (genuine post-resolution dups surface as ordinary local-lookup conflicts during type inference). General-purpose fix — usable by any macro that needs to emit a typed block with N tagged-name args.

## Phase 3+ — buffer-required splice arms (this PR)

Three new planners — `plan_reverse`, `plan_distinct`, `plan_group_by` — slot into the cascade between `plan_order_family` and `plan_loop_or_count`. Each is name-gated and rejects early on shape mismatch. `is_buffer_required_op` still lists all buffer-required ops (`order_by`, `distinct`/`distinct_by`, `reverse`, `group_by`/`group_by_lazy`, `zip`, `join`/`left_join`/`group_join`); for the first four families the marker is now pure defense-in-depth — they should never fire because the dedicated planners catch the op first — and `zip`/`join`/`left_join`/`group_join` remain as real fallthrough markers awaiting BufferZip / BufferedJoin in a future PR.

### plan_reverse

Recognized chain: `src [|> where_(p)]* [|> select(proj)]? |> reverse [|> take(N)]? [|> {to_array, count, first, first_or_default}]`.

| Terminator | Emission |
|---|---|
| `to_array` (default) | prefilter loop → push into buffer → `reverse_inplace(buf)` → `return <- buf` |
| `take(N) \|> to_array` | prefilter → push → `reverse_inplace` → `resize(min(N, length(buf)))` → `return <- buf` (semantics: "last N of source in reverse order") |
| `count` | counter loop only (no buffer, no reverse) — reverse doesn't change count |
| `first` | last-survivor tracker — keep last element seen, `panic` if none |
| `first_or_default(d)` | last-survivor tracker — keep last element seen, return `d` if none |

### plan_distinct

Recognized chain: `src [|> where_(p)]* [|> select(proj)]? |> distinct[_by(key)] [|> take(N)]? [|> terminator]`. Two emission shapes by terminator class:

**A. Buffer-required terminator** (`to_array`, with optional `take(N)`): streaming dedup. Single pass, push only if key not in seen-table, optional `take(N)` early-exit. Inside the "fresh key" branch the take guard runs BEFORE `seen.insert` / `buf.push_clone` so non-positive N short-circuits without ever mutating `seen` or `buf`:

```
if ($e(whereCond)) {
    $e(intermediateBinds)
    let `k` = _::unique_key(<keyExpr>)
    if (!`seen` |> key_exists(`k`)) {
        if (`taken` >= N) break    // only when take(N) present
        `taken`++                  // only when take(N) present
        `seen` |> insert(`k`)
        `buf` |> push_clone(<projection>)
    }
}
```

**B. Buffer-not-required terminators** (`count`, `sum`, `long_count`): elide the buffer; only the seen-table is materialized. `count` returns `length(seen)`; `sum`/`long_count` fold inline at the freshly-inserted site. `first`/`first_or_default` after `distinct` are not yet spliced — they cascade to tier 2.

### plan_group_by

Recognized chain: `src |> group_by[_lazy](key) |> select(group_proj) [|> {to_array, count}]?`. Avoids bucket-array materialization entirely by emitting `table<unique_key; tuple<KT; AccT>>` updated per element. Two-pass rewriter `try_recognize_group_reducer(body, accField)` returns `(recognized, accType, accUpdate, accInit)`:

| `group_proj` body shape | Acc | Per-element update |
|---|---|---|
| `_._1 \|> length` / `\|> count` | `int = 0` | `entry._1++` |
| `_._1 \|> long_count` | `int64 = 0l` | `entry._1++` |
| `_._1 \|> sum` (numeric bucket) | `T = default<T>` | `entry._1 += it` |

Named-tuple wrap forms `(K = _._0, V = <recognized>)` preserve the user's field-name hints by using `group_proj._type` as the table value type directly (no AST mutation; `kv` flowing out of `values(tab)` is already shaped correctly via `push_clone(kv)`). Terminators: `to_array` (default) builds the result; `count` returns `length(tab)` (number of groups).

Bails to tier 2 cascade on: inner-select-sum (`_._1 |> select(<inner>) |> sum` — see deferred follow-up below), multi-reducer named tuples, `_._1` appearing outside the recognized reducer chains, `having_*` between `group_by` and `select(group_proj)`, **any upstream `where_*`/`select*` between source and `group_by_lazy`** (upstream fusion deferred). Side-effectful key bodies are NOT a bail — the emitted code evaluates each key once per element (same as plain `group_by_lazy`).

### Headline benchmarks (100K rows, INTERP, this PR)

| Benchmark | Shape | m1 (sql) | m3 (linq) | m3f (prev) | m3f (this PR) | Win |
|---|---|---:|---:|---:|---:|---:|
| distinct_count | `select(brand) → distinct → to_array` | 41 | 43 | 33 | **15** | **2.8× over m3 / 2.2× over prev / faster than m1 SQL** |
| distinct_take (NEW) | `select(brand) → distinct → take(3) → to_array` | 0 | 43 | — | **0** | early-exit at 3 unique brands (sub-ns per element) |
| groupby_count | `group_by(brand) → select(brand, length) → to_array` | 141 | 73 | 76 | **33** | **2.2× over m3 / 2.3× over prev / 4.3× over m1 SQL** |
| reverse_take (NEW) | `each(arr) → reverse → take(10) → to_array` | 0\* | 22 | — | **34** | regression vs m3 — see footnote |
| groupby_sum (deferred) | `group_by(brand) → select(brand, select(price) → sum) → to_array` | 172 | 101 | 107 | **107** | unchanged — inner-select-sum (see follow-up) |

**`distinct_count` win**: plain LINQ materializes a full distinct array then counts. The splice fuses streaming dedup into a single pass over the source, producing the buffer once. `distinct_take` is the extreme case — with `BRAND_COUNT=5` and `TAKE_N=3`, the splice exits after the first 3 unique brands appear (~3 source elements), so the per-element cost collapses to 0.

**`groupby_count` win**: plain LINQ materializes per-bucket arrays then counts each. The splice keeps only `table<brand → int>` updated per element with `entry._1++`. The result-build loop iterates `values(tab)` (5 entries) — total work is one source pass + 5 emplaces vs tier 2's per-bucket array allocation+push+length-call. Result beats SQL by 4.3× because no engine round-trip overhead.

**`reverse_take` regression vs m3** (34 vs 22 ns/op): both variants materialize the full source into a buffer (the iterator-form `reverse()` in [linq.das:234](daslib/linq.das#L234) does it via `generator capture` + per-yield indexing; the splice does it via `push_clone` in the prefilter loop). The cost difference is the second pass: m3's generator yields `buffer[len-i-1]` lazily, so `take(TAKE_N)` only triggers N yields; the splice instead calls `reverse_inplace(buf)` (full O(length) swap) and then `resize(N)`. For TAKE_N << length the lazy-yield approach wins because the reverse is bounded to N instead of length. The splice still wins the headline shape (`reverse |> to_array` without take) where both variants pay full-length cost but the splice avoids the generator-dispatch overhead. A future optimization could detect `reverse |> take(N)` on array-typed sources and emit a backward index loop bounded to `[max(0, len-N), len)` — deferred. \* m1 SQL: `ORDER BY id DESC LIMIT 10` collapses to 0 ns/op via index.

**`groupby_sum` unchanged**: the benchmark uses `g._1 |> _select($(c : Car) => c.price) |> _sum()` — an inner-select-sum chain. The G4 recognizer covers bare `_._1 |> sum` but doesn't yet inline the inner projection into the `+=` site. Falls back to tier 2 array-shape (correct but slow). Deferred to follow-up (~80 LOC; see plan).

## Operator-coverage checklist (parity tests)

The 24 benchmarks above cover the most common shapes. The end-game target is one benchmark per `_fold`-applicable scenario in the broader `tests/linq/` operator suite. Tracking the long-tail coverage below; PRs that add splice support for new operators should add a benchmark here if not already present.

| Source test file | Operator group | Covered by benchmark | Status |
|---|---|---|---|
| `test_linq.das` | comprehension basics | count_aggregate, sum_aggregate | ✅ |
| `test_linq_aggregation.das` | count/sum/min/max/avg/aggregate | count/sum/min/max/average_aggregate, sum_where, long_count_aggregate | ✅ core; `aggregate(seed, fn)` ⏳ |
| `test_linq_querying.das` | any/all/contains | any_match, all_match, contains_match | ✅ core |
| `test_linq_transform.das` | select/select_many/zip | to_array_filter, zip_dot_product | ✅ select/zip; `select_many` ⏳ |
| `test_linq_sorting.das` | order/order_by/reverse | sort_first, sort_take, select_where_order_take, reverse_take | ✅ ascending + `order_descending` (Phase 3); ✅ `reverse` (this PR; `reverse \|> take(N)` shape is a regression vs lazy iterator — see Phase 3+ notes) |
| `test_linq_group_by.das` | group_by/group_by_lazy/having | groupby_count, groupby_sum | ✅ count/long_count/bare-sum reducers + named-tuple wrap (this PR); ⏳ inner-select-sum, min/max/first/average, multi-reducer, `having_` |
| `test_linq_join.das` | join/left_join/right_join/full_outer/cross | join_count | ✅ inner; outer joins + cross ⏳ |
| `test_linq_partition.das` | take/skip/take_while/skip_while/chunk | take_count, skip_take, take_sum_aggregate, take_count_filtered | ✅ take/skip in splice lanes; `_while` + `chunk` ⏳ |
| `test_linq_set.das` | distinct/union/except/intersect/unique | distinct_count, distinct_take | ✅ distinct + distinct_by (streaming dedup, this PR); union/except/intersect/unique ⏳ |
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

**Three-tier cascade, always safe.** `_fold(chain)` is observationally equivalent to `chain` — never breaks semantics, only ever faster. Splice arms cover the hot paths; `fold_linq_default` (tier 2) emits an array-shape pipeline with `_inplace` reuse for anything splice can't handle; raw clone (tier 3) handles the empty-chain edge. This obviates the fail-loudly contract that earlier plans considered.

**`fold_linq_default` is load-bearing.** Its array-shape emission (`var pass_0 = call0(src); var pass_1 = call1_inplace(pass_0); delete pass_0; ...`) with explicit `delete` of intermediates and `_inplace` variant routing is genuinely better than plain LINQ (one buffer at a time, reused; no iterator wrappers). Kept as the cascade's tier 2.

**100K rows.** daslang is interpreter-first; 100K gives sub-second-per-variant benchmark turnaround and clearly shows the asymmetries we care about. Bump later if AOT/JIT numbers warrant.

**`PERF009` suppression in `fold_linq_default`.** The macro's `var pass_N = call` + later `return <- pass_N` pattern triggers PERF009 on single-pass chains. The shape is load-bearing for the array-pipeline semantics (every stage binds so the next can reuse the buffer in-place), so we suppress inline at the qmacro_expr emission site and document why.

**Benchmark variants where SQL has no clean form.** `zip` (not a relational op), `_all(pred)` (no direct `_all` chain terminal in sqlite_linq), `join` with inner-select-from (wiring not exposed), `distinct |> count` (no `COUNT(DISTINCT col)` yet), `take/skip` before aggregate (LIMIT/OFFSET semantics conflict with aggregate-collapse). We either reformulate to a SQL-friendly shape (`count(where ¬p)` for all_match), omit the m1 column (zip, join), or terminate the chain in `to_array` instead of an aggregate (take/skip/distinct).
