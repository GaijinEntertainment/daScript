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
| 3+ BufferDistinct | `[where_*]?[select?] \|> distinct[_by(key)] [\|> take(N)]? [\|> {to_array, count, sum, long_count}]?` — streaming dedup via `table<unique_key>` integrated into the prefilter loop; take(N) → break-after-N early-exit (guard placed BEFORE the consume site so non-positive N short-circuits); count terminator elides the buffer and returns `length(seen)`. `first`/`first_or_default`/`min`/`max`/`average` after `distinct` cascade to tier 2. | ✅ core |
| 3+ BufferGroupBy | `[where_*/select*]* \|> group_by[_lazy](key) [\|> having_(pred)]? \|> select(group_proj) [\|> {to_array, count}]?` — incremental-aggregate fusion: emits `table<unique_key; tuple<KT; AccT>>` (or N+1-slot named tuple) updated per element. Recognized reducers: bare + inner-select `{sum, min, max, first, average}` (10 shapes) + `{length, count, long_count}`. `average` uses a 2-slot per-key accumulator (sum + count, internally `tuple<double; uint64>`) with post-process division at result-build. Bare body, 2-slot named-tuple wrap, and N+1-slot multi-reducer named tuple all supported. Upstream `where_*`/`select*` chains fuse into the per-element loop with lazy semantics (selects after a where execute INSIDE that where's guard). An optional `having_(pred)` between `group_by_lazy` and `select` is rewritten to reference accumulator slots and wraps the result-build push; predicates that touch the raw bucket array or reference a reducer with no matching select slot cascade. Bails to tier 2 for key-not-at-slot-0 in named tuple, or upstream ops other than `where_*`/`select*` (e.g. distinct/order_by). | ✅ |
| 3+ BufferGroupBy — inner-select-sum (PR-A1) | Recognizes `_._1 \|> select(<lambda>) \|> sum` after `group_by_lazy(key)`. Splice peels the inner lambda body and inlines it directly into the per-element `entry._1 += <body>` site, accumulator type derived from the outer sum's resolved return type. Per-element emission split into miss-branch (init) and hit-branch (incremental update) to support this and the future min/max/first arms in PR-A2. Bare body and named-tuple wrap both supported. | ✅ |
| 3+ BufferGroupBy — min/max/first + multi-reducer (PR-A2) | Recognizer generalized to bare + inner-select `{sum, min, max, first}` (8 reducer shapes). Planner walks an `array<ReducerSpec>` so the named-tuple wrap extends from 2-slot to N+1-slot (key at slot 0, reducers at slots 1..N) — a single per-element pass fuses N reducers. min/max use direct `<`/`>` on workhorse acc types, fall back to `_::less` for non-workhorse. first emits miss-init only (no hit update); `first_or_default(d)` rejects on the 2-arg arity check and cascades. `average` and key-not-at-slot-0 cascade. | ✅ |
| 3+ BufferGroupBy — upstream where/select fusion (PR-B) | `plan_group_by` walks calls between source and `group_by_lazy`, fusing chains of `where_*` and `select*` into the per-element loop. Selects bind projected values to intermediate vars (referenced by key, table value type, and reducer splices); wheres AND-merge within a segment and guard everything that follows them — selects appearing after a where emit their binds INSIDE that where's `if`, preserving the reference's lazy `select` semantics (a projection like `_where(_!=0)._select(10/_)` only runs on survivors). Bails on any other upstream op (distinct/order_by/etc. cascade to tier 2). Closes the `arr \|> where \|> group_by \|> aggregate` shape — one of the most common shapes in the operator catalogue. | ✅ |
| 3+ BufferGroupBy — having_ filter fusion (PR-D) | `plan_group_by` accepts an optional `having_(pred)` between `group_by_lazy(key)` and `select(group_proj)`. The predicate's `<bind>._0` and `<reducer>(<bind>._1)` references are rewritten to `kv._{slot}` against the existing select-side reducer slots; the rewritten predicate then wraps the result-build push (or counter increment for the count terminator), so only buckets passing the filter materialize. Bails when the predicate references a reducer with no matching select slot, touches the raw bucket array, or references the bucket value in any other shape — those still cascade to tier 2. Closes the SQL `HAVING` shape against the splice path. | ✅ |
| 3+ BufferGroupBy — average reducer (PR-A2 follow-up) | Recognizer extends to bare + inner-select `average` (10 reducer shapes total). The slot uses a 2-tuple acc (`tuple<double; uint64>` — sum + count) instead of the user-facing `double`; the named-tuple table value type clones the user's body type and substitutes average slots, preserving field names. Result-build synthesizes an `ExprMakeTuple` programmatically, dividing average slots (`kv._{i}._0 / double(kv._{i}._1)`) and copying non-average slots verbatim. Having predicates that reference an average slot get the divide expression too, via a shared `mk_slot_output_expr` helper. Buckets are non-empty by construction (a key only enters the table after at least one source element produced it), so the count is always ≥1 and no divide-by-zero guard is needed. | ✅ |
| 3+ Buffer remainder | MultiSourceZip, BufferedJoin. Currently cascade to tier 2 array-shape. | ⏳ |
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
| groupby_sum | `group_by → select(_, select → sum)` | 172 | 101 | 36 (PR-A1 inner-select-sum) |
| groupby_min | `group_by → select((K, select → min))` | 175 | 111 | **42** (PR-A2 inner-select-min) |
| groupby_max | `group_by → select((K, select → max))` | 173 | 108 | **43** (PR-A2 inner-select-max) |
| groupby_first | `group_by → select((K, first))` | — | 71 | **36** (PR-A2 bare-first) |
| groupby_multi_reducer | `group_by → select((K, length, select → sum, select → max))` | 189 | 139 | **53** (PR-A2 multi-reducer fused pass) |
| groupby_where_count | `where → group_by → select((K, length))` | 75 | 65 | **23** (PR-B upstream where fused) |
| groupby_where_sum | `where → group_by → select((K, select → sum))` | 86 | 80 | **23** (PR-B upstream where + inner-select-sum fused) |
| groupby_select_sum | `select → group_by → select((K, sum))` | — | 110 | **58** (PR-B upstream select fused via intermediate var bind) |
| groupby_having_count | `group_by → having(len>=5) → select((K, length))` | 141 | 78 | **36** (PR-D having predicate rewritten to slot ref) |
| groupby_having_hidden_sum | `group_by → having(select(price) → sum > N) → select((K, length))` | 175 | 109 | **40** (PR-E synthesized hidden accumulator slot for the having-only inner-select-sum) |
| groupby_average | `group_by → select((K, select → average))` | 173 | 106 | **52** (PR-A2 follow-up: 2-slot acc + post-process divide) |
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

Three new planners — `plan_reverse`, `plan_distinct`, `plan_group_by` — slot into the cascade between `plan_order_family` and `plan_loop_or_count`. Each is name-gated and rejects early on shape mismatch. `is_buffer_required_op` still lists all buffer-required ops (`order_by`, `distinct`/`distinct_by`, `reverse`, `group_by`/`group_by_lazy`, `zip`, `join`/`left_join`/`group_join`); it acts as a fallback marker for the shapes the dedicated planners don't cover (`distinct().first()`, `order_by(...).first()`, etc. — terminators outside the spliced set cascade to `is_buffer_required_op` which triggers the tier-2 fallthrough). `zip`/`join`/`left_join`/`group_join` cascade unconditionally pending BufferZip / BufferedJoin.

### plan_reverse

Recognized chain: `src [|> where_(p)]* [|> select(proj)]? |> reverse [|> take(N)]? [|> {to_array, count, first, first_or_default}]`.

| Terminator | Emission |
|---|---|
| `to_array` (default) | prefilter loop → push into buffer → `reverse_inplace(buf)` → `return <- buf` |
| `take(N) \|> to_array`, **array source, no where/select** (R6) | backward index loop visiting `src[len-1 .. max(0, len-N))` directly into a `reserve(takeN)`'d buffer — skips `reverse_inplace` and the full-source push. See "Phase 3+ reverse_take backward index loop (PR-C)" section below |
| `take(N) \|> to_array`, **iterator source or with where/select** (R1-R4 fallback) | prefilter → push → `reverse_inplace` → `resize(min(N, length(buf)))` → `return <- buf` (semantics: "last N of source in reverse order") |
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

**B. Buffer-not-required terminators** (`count`, `sum`, `long_count`): elide the buffer; only the seen-table is materialized. `count` returns `length(seen)`; `long_count` returns `int64(length(seen))` (both are count-shaped — no per-element accumulator); `sum` folds inline at the freshly-inserted site with `acc += <projection>`. `first`/`first_or_default` after `distinct` are not yet spliced — they cascade to tier 2.

**Intentional divergence — `distinct[_by] |> take(N)` is streaming, not eager**: the runtime helpers (`distinct_impl` / `distinct_by_impl`) materialize the FULL distinct set first, then `take(N)` slices the head. The splice instead breaks the source loop the moment N distinct keys are observed. For pure code this is observationally identical (and strictly faster — same result, less work). For side-effecting key functions or upstream `where_`/`select` projections, the splice fires the side-effect on **fewer source elements** than the runtime would. This is by design: the splice gives `distinct().take()` the iterator-form semantics regardless of source shape (the iterator-form `distinct()` in `daslib/linq.das:668` is already a lazy generator and stops early under `take` even without the splice). If you need exact runtime parity for side-effect counts, hoist the side-effect out of the key/projection before the splice fires.

### plan_group_by

Recognized chain: `src |> group_by[_lazy](key) |> select(group_proj) [|> {to_array, count}]?`. Avoids bucket-array materialization entirely by emitting `table<unique_key; tuple<KT; AccT>>` updated per element. The `select(group_proj)` body is recognized via `is_bucket_reducer_call` ([linq_fold.das:1666](daslib/linq_fold.das#L1666)); recognized shapes are:

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
| distinct_count | `each(arr) → select(brand) → distinct → to_array` | 41 | 43 | 33 | **16** | **2.7× over m3 / 2.1× over prev / faster than m1 SQL** |
| distinct_take (NEW) | `each(arr) → select(brand) → distinct → take(3) → to_array` | 0 | 30 | — | **0** | early-exit at 3 unique brands — splice eliminates generator-dispatch overhead |
| groupby_count | `each(arr) → group_by(brand) → select(brand, length) → to_array` | 141 | 71 | 76 | **37** | **2.1× over prev / 1.9× over m3 / 3.8× over m1 SQL** |
| reverse_take (NEW) | `each(arr) → reverse → take(10) → to_array` | 0\* | 22 | — | **34** | regression vs m3 — see footnote (closed in PR-C below) |
| reverse_take (PR-C, backward index loop) | same chain on array source | 0\* | 22 | 34 | **0** | **closes the regression — backward index loop visits only last N indices** |
| groupby_sum (deferred) | `each(arr) → group_by(brand) → select(brand, select(price) → sum) → to_array` | 173 | 98 | 107 | **108** | unchanged — inner-select-sum (see follow-up) |
| groupby_sum (PR-A1, inner-select-sum) | same chain | 174 | 101 | 108 | **36** | **3× over prev / 2.8× over m3 / 4.8× over m1 SQL** — closes the deferred follow-up |

**`distinct_count` win**: plain LINQ materializes a full distinct array then counts. The splice fuses streaming dedup into a single pass over the source, producing the buffer once. **`distinct_take`** is the extreme case — both m3 and m3f use matching `each(arr)` iterator sources (so plain LINQ's lazy `distinct().take()` *also* early-exits on the iterator). The splice's win over m3 (~30 → ~0 ns/op) comes from eliminating per-yield generator-dispatch overhead in the lazy chain, not from "lazy vs eager source" comparison.

**`groupby_count` win**: plain LINQ materializes per-bucket arrays then counts each. The splice keeps only `table<brand → int>` updated per element with `entry._1++`. The result-build loop iterates `values(tab)` (5 entries) — total work is one source pass + 5 emplaces vs tier 2's per-bucket array allocation+push+length-call. Result beats SQL by 4.3× because no engine round-trip overhead.

**`reverse_take` regression vs m3** (originally 34 vs 22 ns/op, closed in PR-C — see "backward index loop" section below): both variants materialize the full source into a buffer (the iterator-form `reverse()` in [linq.das:234](daslib/linq.das#L234) does it via `generator capture` + per-yield indexing; the splice did it via `push_clone` in the prefilter loop). The cost difference was the second pass: m3's generator yields `buffer[len-i-1]` lazily, so `take(TAKE_N)` only triggered N yields; the splice instead called `reverse_inplace(buf)` (full O(length) swap) and then `resize(N)`. PR-C detects `reverse |> take(N)` on array-typed sources at macro time and emits a backward index loop bounded to `[max(0, len-N), len)`, visiting only the last N indices — m3f now lands at ~0 ns/op for TAKE_N=10 at 100K. \* m1 SQL: `ORDER BY id DESC LIMIT 10` collapses to 0 ns/op via index.

**`groupby_sum` unchanged**: the benchmark uses `g._1 |> _select($(c : Car) => c.price) |> _sum()` — an inner-select-sum chain. The G4 recognizer covers bare `_._1 |> sum` but doesn't yet inline the inner projection into the `+=` site. Falls back to tier 2 array-shape (correct but slow). Deferred to follow-up (~80 LOC; see plan).

## Phase 3+ groupby reducer expansion — inner-select-sum (PR-A1)

Closes the explicit deferred-follow-up from PR #2721. Two interlocking changes:

1. **Miss/hit emission restructure** in `plan_group_by`. The per-element splice splits on `addr(entry) == addr(dummy)`: on miss, run a `missInit` expression (acc starts at the first-element value); on hit, run a `hitUpdate` expression (incremental update). Observationally equivalent for the existing arms (`count`/`length`/`long_count`/`sum`) — same `entry._1 = 1; commit` vs `entry._1++` vs `entry._1 = it; commit` vs `entry._1 += it` — but the per-branch split lets each reducer pick its own per-side shape independently, unlocking the inner-select-sum arm below and future arms in PR-A2 (`min`/`max`/`first` need different work on miss vs hit). All G2/G3/G8 AST tests stay green with a new `count_call(body, "key_exists") == 0` assertion pinning the single-hash hot path from PR #2721.

2. **Inner-select-sum recognizer**. `is_bucket_reducer_call` extended to match `sum(select(<bind>._1, <lambda>))` (the AST shape after typer resolves `_._1 |> select(<lambda>) |> sum()`). On match, `plan_group_by` peels the inner lambda via `fold_linq_cond(innerLambda, itName)` and splices the body directly into `entry._1 += <inner_body>` (hit) / `entry._1 = <inner_body>` (miss). Accumulator type derived from the OUTER sum call's `_type` (the inner projection's result type, typer-resolved). Both bare body (`_._1 |> select(p) |> sum`) and named-tuple wrap (`(K=_._0, S=_._1 |> select(p) |> sum)`) supported.

### Headline (PR-A1)

| Benchmark | Shape | m1 (sql) | m3 (linq) | m3f (prev) | m3f (this PR) | Win |
|---|---|---:|---:|---:|---:|---:|
| groupby_sum | `each(arr) → group_by(brand) → select((brand, select(price) → sum)) → to_array` | 174 | 101 | 108 | **36** | **3× over prev / 2.8× over m3 / 4.8× over m1 SQL** |
| groupby_count (regression check) | `each(arr) → group_by(brand) → select((brand, length)) → to_array` | 142 | 71 | 37 | **36** | parity (within noise) — A.1 restructure preserves the count splice |

`groupby_sum` matches `groupby_count`'s ~36 ns/op headline: same number of allocations (1, the result array), same per-element work (single hash + entry._1 mutation), no per-bucket array materialization. The splice now beats SQL on this shape too.

### Deferred for PR-A2 (next PR in the series)

- bare `_._1 |> min` / `|> max` / `|> first` reducers + `inner-select-min` / `inner-select-max` / `inner-select-first`
- multi-reducer named tuples `(K=..., N=..|>length, S=..|>sum, M=..|>min)`
- `average` reducer (needs 2-slot per-key acc — separate follow-up after PR-A2)

## Phase 3+ groupby reducer expansion — min/max/first + multi-reducer (PR-A2)

Closes the remaining BufferGroupBy gaps from PR-A1's deferred-follow-up list. Two interlocking changes built on PR-A1's miss/hit emission shape:

1. **Reducer-spec array unifies bare + named-tuple-N planning**. The 2-slot named-tuple recognizer (key + 1 reducer) generalizes to N+1 slots (key at slot 0, reducers at slots 1..N). Bare form remains 1 spec at slot 1. The planner walks `array<ReducerSpec>` and concatenates per-slot `missInit` + `hitUpdate` statements into the per-element loop. The single fused pass replaces N separate aggregation passes for multi-reducer chains. Recognizer flags: average + key-not-at-_0 + unrecognized expression shape all bail to tier 2.

2. **Per-reducer emission for min / max / first (bare + inner-select)**. Recognizer extended to 8 reducer shapes: bare `{sum, min, max, first}` + inner-select `{sum, min, max, first}`. min/max emit `if (it < entry._{s}) entry._{s} := it` for workhorse acc types; `if (_::less(it, entry._{s})) entry._{s} := it` for non-workhorse. first emits miss-init only (`entry._{s} := it`) — subsequent same-key elements are ignored, exploiting the first-key-wins guarantee that PR #2721 baked into the emission shape. inner-select-min/max bind the projection result to a per-element temp so the inner body evaluates once per element (matches the reference's lazy `select` semantics). `first_or_default(d)` rejects on the 2-arg arity check and cascades (silently dropping `d` would be misleading even though a bucket is always non-empty).

### Headline (PR-A2)

| Benchmark | Shape | m1 (sql) | m3 (linq) | m3f (this PR) | Win |
|---|---|---:|---:|---:|---:|
| groupby_min | `group_by → select((K, select → min))` | 175 | 111 | **42** | **2.6× over m3 / 4.2× over m1 SQL** |
| groupby_max | `group_by → select((K, select → max))` | 173 | 108 | **43** | **2.5× over m3 / 4.0× over m1 SQL** |
| groupby_first | `group_by → select((K, first))` | — | 71 | **36** | **2.0× over m3** (no direct SQL aggregator for "first source-order row per group" → m1 omitted) |
| groupby_multi_reducer | `group_by → select((K, length, select → sum, select → max))` | 189 | 139 | **53** | **2.6× over m3 / 3.6× over m1 SQL** (3 reducers fused into 1 pass) |
| groupby_sum (regression check) | `group_by → select((K, select → sum))` | 175 | 102 | **36** | parity — A.3+A.4 refactor preserves the PR-A1 splice |
| groupby_count (regression check) | `group_by → select((K, length))` | 141 | 71 | **36** | parity — A.3+A.4 refactor preserves the PR-A1 splice |

All 6 splice variants land within ~36–53 ns/op — the per-element work is bounded by the single hash op + slot mutations regardless of which reducer or how many. The multi-reducer ~53 ns/op pays a small overhead per extra slot (~5 ns each), still beats SQL by 3.6×.

### Deferred for follow-ups

- `average` reducer (2-slot per-key acc + post-process division — separate follow-up)
- `having_*` between `group_by` and `select(group_proj)` (still cascades to tier 2)

## Phase 3+ groupby reducer expansion — upstream where/select fusion (PR-B)

Closes the explicit deferred-bail from PR #2721 onward: any upstream call between source and `group_by_lazy` would cascade to tier 2. PR-B walks the upstream segment and fuses chains of `where_*` / `select*` into the per-element loop.

**How it lands:**

1. **Walk upstream calls** (between `top` and `group_by_lazy`) in source-to-leaf order. For `select`: bind the previous projection (if any) to a fresh `var v_N := projection_{N-1}` intermediate, then compute the new projection peeled to reference the latest bind name. Update `elemType` to the post-projection type so the table value type witness (`default<elemType>`) matches what the key block will receive. For `where_`: bind any pending projection first (so the predicate references a stable name — no double-eval), then AND-merge the peeled predicate into `whereCond`. For anything else (`distinct`, `order_by`, etc.): bail to tier 2.

2. **Final bind** before recognizer call: if a projection is still unbound after the walk, bind it now so `itName` (the post-projection name) is stable. Inner-select reducer recognition uses this `itName` to fold the inner lambda body — without the bind, the recognizer would dangle on a name that the typer hasn't seen.

3. **Per-element emission split into core + wrapping.** The 3-stmt core (`let k = invoke(key, it); let uk = unique_key(k); unsafe { table update }`) is wrapped with `if (whereCond) { core }`, then prepended with `intermediateBinds`. The result splices into the for-loop body via `$b(bodyStmts)`.

**Bail cases (cascade to tier 2):**
- Any upstream op other than `where_*` / `select*` (distinct, order_by, take, skip, etc.)
- All recognized bail cases from PR-A1/PR-A2 still apply (having_, unrecognized reducer, key not at slot 0, etc.)

### Headline (PR-B)

| Benchmark | Shape | m1 (sql) | m3 (linq) | m3f (this PR) | Win |
|---|---|---:|---:|---:|---:|
| groupby_where_count | `each(arr) → where(price>500) → group_by(brand) → select((K, length)) → to_array` | 75 | 65 | **23** | **2.8× over m3 / 3.3× over m1 SQL** |
| groupby_where_sum | `each(arr) → where(price>500) → group_by(brand) → select((K, select(price) → sum)) → to_array` | 86 | 80 | **23** | **3.5× over m3 / 3.7× over m1 SQL** |
| groupby_select_sum | `each(arr) → select(c.price) → group_by(_ % 100) → select((K, sum)) → to_array` | — | 110 | **58** | **1.9× over m3** (100 bucket keys vs 5 for brand → result-build cost dominates; m1 omitted: `_sql` requires `_group_by(_.Field)`, no expression keys) |
| groupby_count (regression check) | `each(arr) → group_by(brand) → select((K, length)) → to_array` | 142 | 71 | **36** | parity — PR-B core refactor preserves the count splice |
| groupby_sum (regression check) | inner-select-sum chain | 170 | 101 | **36** | parity — PR-A1 splice preserved |
| groupby_multi_reducer (regression check) | 4-slot named tuple | 189 | 139 | **53** | parity — PR-A2 multi-reducer preserved |

The `where`-fused benchmarks (groupby_where_count and groupby_where_sum) drop to ~23 ns/op — even faster than the no-upstream `groupby_count` (36 ns/op) because the where filter reduces the table-update count (only ~50% of source elements pass `price > 500`). The win formula: fewer hash ops × the same per-survivor cost = lower per-element averaged time.

### Deferred for PR-C / follow-ups

- Other upstream ops: `distinct`, `order_by`, `skip`/`take` (skip/take before group_by is rare; distinct/order_by would need streaming dedup / pre-sort integration)

## Phase 3+ groupby reducer expansion — having_ filter fusion (PR-D)

Closes the SQL `HAVING` shape against the splice path: any `_having(pred)` between `group_by_lazy` and `select(group_proj)` used to cascade because plan_group_by required the `select` to sit directly on `group_by_lazy`.

**How it lands:**

1. **Pop `having_` between select and group_by_lazy.** After the existing tail pops (`count` terminator, then `select(group_proj)`), the planner now optionally pops a `_having` call before checking for `group_by_lazy`.

2. **Rewrite the predicate against accumulator slots.** `fold_linq_cond` peels the having lambda into a fresh `hb` bind, then a recursive AST walker (`rewrite_having_pred`) substitutes:
   - `<hb>._0` → `kv._0` (the key slot)
   - `<reducer>(<hb>._1)` → `kv._{spec.slot}` when the bare reducer matches an existing select-side spec by name
   - `<reducer>(select(<hb>._1, <lam>))` → `kv._{spec.slot}` for inner-select reducers (matched by reducer name; v1 doesn't structurally compare inner lambda bodies)

   Any other use of `<hb>` — raw value, `_1` outside a matched reducer, `_<other>` field — returns `null` and the planner cascades. ExprRef2Value wrappers the typer leaves around field reads are peeled at the entry of the walker, mirroring `is_bind_field_access`.

3. **Wrap result-build with the rewritten predicate.** The `to_array` lane wraps `push_clone(outputExpr)` in `if (predicate)`; the `count` terminator can no longer collapse to `length(tab)` and instead emits a counter loop with the same filter. Per-element table-update is unchanged — the per-bucket filter runs once per *distinct key*, not per source element.

**Bail cases (cascade to tier 2):**
- Having references a reducer with no matching select slot (e.g., `_._1|>sum > N` when select has only length).
- Having touches the bucket array in any non-reducer shape (e.g., `empty(_._1)`).
- All upstream bail cases from PR-A1/A2/B still apply.

### Headline (PR-D)

| Benchmark | Shape | m1 (sql) | m3 (linq) | m3f (this PR) | Win |
|---|---|---:|---:|---:|---:|
| groupby_having_count | `each(arr) → group_by(brand) → having(len>=5) → select((K, length)) → to_array` | 141 | 78 | **36** | **2.2× over m3 / 3.9× over m1 SQL** |
| groupby_count (regression check) | `each(arr) → group_by(brand) → select((K, length)) → to_array` | 141 | 71 | **36** | parity — PR-D rewrite-or-bail path preserves the no-having splice |
| groupby_where_count (regression check) | upstream-where-fused chain | 75 | 62 | **23** | parity — PR-B preserved |

`groupby_having_count` lands at the same ~36 ns/op as `groupby_count` because the per-element loop is unchanged — only the result-build adds the per-bucket filter, which runs O(num distinct keys) times rather than O(N).

### Deferred for follow-ups

- Having predicates that reference a reducer absent from the select on the **bare-form** select shape (the bare table layout uses `tuple<KeyT; AccT>` synthesized inside a qmacro with embedded `typedecl(invoke(...))` for the key type — can't be extended with a dynamic count of additional acc slots; named-tuple form is handled by PR-E below).
- Having predicates that touch the raw bucket array (would require materializing the per-bucket array, defeating the splice).

## Phase 3+ groupby reducer expansion — average reducer (PR-A2 follow-up)

Closes the explicit `average` cascade deferred since PR-A1 / PR-A2 / PR-D. `average` is structurally different from sum/count/min/max/first: each per-key accumulator needs **two slots** (running sum + element count), and result-build divides at output. The PR-A1 miss/hit emission shape generalizes cleanly — the only deltas are an extra `mk_avg_acc_type` (`tuple<double; uint64>`), per-slot output synthesis, and a having-predicate substitution that emits the divide expression instead of a plain field ref.

**How it lands:**

1. **Recognizer (`is_bucket_reducer_call`).** Adds `"average"` to both the bare arm (`<bind>._1 |> average`) and the inner-select arm (`<bind>._1 |> select(<lambda>) |> average`). The returned reducer name is `"average"` or `"average_inner_select"` and feeds into `recognize_reducer_specs` just like any other reducer.

2. **Per-slot acc type override (`slot_acc_type`).** When the reducer is average, the slot type is forced to `tuple<double; uint64>` (sum + count) regardless of the user-facing return type (`double`). The user's body type stays the buffer element type so the result array's declared shape (e.g., `tuple<K : int; Avg : double>`) is preserved.

3. **Miss/hit emission (`emit_reducer_branches`).** `entry._{slot} = (double(it), 1ul)` on miss; `entry._{slot}._0 += double(it); entry._{slot}._1 ++` on hit. The `_inner_select` arm evaluates the inner projection once into a `let vavg`-bound temp (matches reference single-eval-per-element semantics).

4. **Table value type + result-build (`plan_group_by`).** A `hasAvg` scan picks up any average slot. For the named-tuple form, the cloned `tabValueType` swaps each average slot's `argTypes[slot]` from `double` (user's view) to the `tuple<double; uint64>` acc shape (preserves the field name from the user's `argNames`). At result-build, the output expression is a programmatically-constructed `ExprMakeTuple` that copies non-average slots verbatim (`kv._{i}`) and divides average slots (`kv._{i}._0 / double(kv._{i}._1)`); for the bare form the same divide expression replaces `kv._1`. Buckets are non-empty by construction (a key only enters the table if at least one source element produced it), so the count is always ≥1 — no divide-by-zero guard needed.

5. **Having-predicate transform.** `rewrite_having_pred` routes average-slot references through a shared `mk_slot_output_expr` helper that emits the divide expression for average specs and a plain slot ref otherwise. So `_._1 |> average > 50.0` in a having predicate works the same way as `_._1 |> sum > 50` does.

**Bail cases (cascade to tier 2):** unchanged from PR-A2 — any bucket-touching shape that isn't a recognized reducer, average at slot 0 (key must be `_._0`), etc.

### Headline (PR-A2 follow-up)

| Benchmark | Shape | m1 (sql) | m3 (linq) | m3f (this PR) | Win |
|---|---|---:|---:|---:|---:|
| groupby_average | `each(arr) → group_by(brand) → select((K, select(price) → average)) → to_array` | 173 | 106 | **52** | **2.0× over m3 / 3.3× over m1 SQL** |
| groupby_count (regression check) | bare count splice | 142 | 71 | **36** | parity — average refactor preserves the count splice |
| groupby_sum (regression check) | inner-select-sum splice | 172 | 101 | **36** | parity — PR-A1 splice preserved |
| groupby_min (regression check) | inner-select-min splice | 177 | 111 | **43** | parity — PR-A2 splice preserved |
| groupby_multi_reducer (regression check) | 4-slot named tuple | 190 | 138 | **52** | parity — PR-A2 multi-reducer preserved |

`groupby_average` lands at ~52 ns/op vs `groupby_sum`'s ~36 — the extra ~16 ns/op is the 2-slot accumulator update (one extra `double(...) += ...` and `++` per source element) plus the per-bucket division at result-build (runs O(num distinct keys), not O(N)).

## Phase 3+ reverse_take backward index loop (PR-C)

Closes the documented `reverse_take` regression (34 vs 22 ns/op m3f-vs-m3 at TAKE_N=10/N=100K). The original R1–R4 emit shape pushed every source element into the buffer, ran an O(length) `reverse_inplace`, then `resize(N)` truncated everything past the first N — wasting work proportional to `length - N`.

**How it lands:**

`plan_reverse` adds an R6 arm taken when *all* of these hold:

- `take(N)` is present at the chain tail
- no upstream `where_`
- no upstream `select`
- source type is `isGoodArrayType || isArray` (need O(1) indexing — iterator/range sources don't qualify and fall through to R1-R4)

The emission becomes:

```
let __len = length(src)
let __takeN = N <= 0 ? 0 : (N < __len ? N : __len)
var buf : array<T>
buf |> reserve(__takeN)
for (k in 0 .. __takeN) {
    buf |> push_clone(src[__len - 1 - k])
}
return buf
```

Only the last `min(N, length)` indices are visited; the entire `reverse_inplace` + truncate pass is skipped. The take-bound clamp keeps the original `take(N <= 0)` and `take(N > length)` semantics.

**Bail conditions (cascade to R1-R4):**
- Iterator source (no O(1) index access). Verified by AST test `test_reverse_take_iterator_source_uses_reverse_inplace`.
- Upstream `where_` — can't predict which indices survive without visiting all elements.
- Upstream `select` — eager-projection semantics differ from per-index lazy projection; the R1-R4 path preserves current behavior.
- `reverse |> to_array` without take — full-length materialization is unavoidable.

### Headline (PR-C)

| Benchmark | Shape | m1 (sql) | m3 (linq) | m3f (prev) | m3f (this PR) | Win |
|---|---|---:|---:|---:|---:|---:|
| reverse_take | `each(arr) → reverse → take(10) → to_array` | 0\* | 22 | 34 | **0** | **closes the regression — beats m3 too** |

\* m1 SQL: `ORDER BY id DESC LIMIT 10` collapses to 0 ns/op via index. The m3f 0 ns/op result means sub-nanosecond per element averaged across 10/100K visited elements — the backward index loop runs only 10 iterations regardless of N.

## Phase 3+ groupby reducer expansion — hidden-slot reducer-in-having (PR-E)

Closes the explicit "having predicate references a reducer absent from the select" cascade case deferred since PR-D. The named-tuple select-shape now extends its internal table value type with one extra acc slot per having-only reducer; the per-element loop updates the hidden slot alongside the user-visible ones; result-build re-synthesizes the user's tuple via `ExprMakeTuple` (omitting hidden slots) while the having predicate substitutes hidden-slot references via the same `mk_slot_output_expr` helper PR-D already used for matched slots.

**How it lands:**

1. **Scan-then-rewrite the having predicate.** A new `extend_specs_for_missing_having_reducers` walks the having pred AST first. For each `<reducer>(<hb>._1)` or `<reducer>(select(<hb>._1, <lam>))` call whose reducer name doesn't match any existing select-side spec, it appends a new hidden `ReducerSpec` to the specs array with a fresh slot index (starting at `userVisibleSlotCount + 1`). Then the existing PR-D `rewrite_having_pred` runs unchanged — every reducer reference now has a matching spec to bind to.

2. **`mk_hidden_acc_type` helper.** Derives the acc TypeDecl for hidden slots from the reducer name + bucket-element type (and the peeled inner-body type for inner-select forms). Mirrors the per-reducer-name logic of `slot_acc_type` but takes the source types directly (hidden slots have no user-visible group_proj body to drill into). `length`/`count` → `int`; `long_count` → `int64`; `average` → `tuple<double; uint64>` via `mk_avg_acc_type`; `sum`/`min`/`max`/`first` → bucket element type (or inner-body type).

3. **Extend `tabValueType.argTypes` (and `argNames`).** After the existing avg-slot type swap, the named-tuple's tabValueType gets each hidden spec's accType appended at the end. argNames grow in lockstep with auto-generated `_hidden_{slot}` names to keep the named-tuple shape valid.

4. **Unify result-build ExprMakeTuple synthesis.** What was `elif (hasAvg)` becomes `elif (hasAvg || hasHidden)` — the same loop that re-synthesizes the user's tuple slot-by-slot (dividing avg slots, copying others) naturally omits hidden slots because it iterates `1 .. groupProjBody._type.argTypes |> length` (the user's slot count, NOT the internal table's extended count).

5. **Bare-form bail.** When the user's group_proj is a single bare reducer (not a named tuple) AND having needs a hidden slot, the planner returns null and the chain cascades. The bare table layout uses `tuple<KeyT; AccT>` synthesized inside a qmacro with embedded `typedecl(invoke(...))` for the key type, which can't be dynamically extended with additional acc slots. Pinned by `test_group_by_having_hidden_bare_form_cascades_to_tier2`.

**Bail cases (cascade to tier 2):**
- Bare-form select with hidden-slot reducer in having (per above).
- Inner-select reducer in having whose lambda is non-peelable (peel failure returns null).
- All upstream bail cases from PR-A1/A2/B/D still apply.

**Edge cases worth noting:**
- Same-name reducer in select AND having reuses the select slot (existing PR-D match-by-name behavior preserved). Hidden slot only added when no name match exists. The "inner-select reducer with different lambdas at select vs having" case still match-by-name in v1 — structural lambda compare deferred.
- Multiple references to the SAME hidden reducer in the predicate dedup naturally: the first walk adds the spec; subsequent walks see a matching spec and skip.
- 2+ hidden reducers fall into the same scan pass; each gets its own slot.

### Headline (PR-E)

| Benchmark | Shape | m1 (sql) | m3 (linq) | m3f (this PR) | Win |
|---|---|---:|---:|---:|---:|
| groupby_having_hidden_sum | `each(arr) → group_by(brand) → having(sum(price) > 50000) → select((K, length)) → to_array` | 175 | 109 | **40** | **2.7× over m3 / 4.4× over m1 SQL** |
| groupby_having_count (regression check) | `group_by → having(len>=5) → select((K, length))` | 141 | 92 | **36** | parity — PR-E scan-then-rewrite preserves the matching-slot splice |

`groupby_having_hidden_sum` lands at ~40 ns/op vs `groupby_having_count`'s ~36 — the extra ~4 ns/op is the hidden inner-select-sum's per-element `entry._{hidden} += c.price` (one extra add per source element) alongside the visible `length++`.

### Deferred for follow-ups

- Bare-form select with hidden slot (requires restructuring the bare-table key-type qmacro to programmatic TypeDecl construction).
- Inner-select reducer in having that uses a different lambda from a same-named select-side inner-select reducer — currently match-by-name conflates them (already a v1 limitation since PR-D; structural lambda compare TBD).

## Operator-coverage checklist (parity tests)

The benchmarks above cover the most common shapes. The end-game target is one benchmark per `_fold`-applicable scenario in the broader `tests/linq/` operator suite. Tracking the long-tail coverage below; PRs that add splice support for new operators should add a benchmark here if not already present.

| Source test file | Operator group | Covered by benchmark | Status |
|---|---|---|---|
| `test_linq.das` | comprehension basics | count_aggregate, sum_aggregate | ✅ |
| `test_linq_aggregation.das` | count/sum/min/max/avg/aggregate | count/sum/min/max/average_aggregate, sum_where, long_count_aggregate | ✅ core; `aggregate(seed, fn)` ⏳ |
| `test_linq_querying.das` | any/all/contains | any_match, all_match, contains_match | ✅ core |
| `test_linq_transform.das` | select/select_many/zip | to_array_filter, zip_dot_product | ✅ select/zip; `select_many` ⏳ |
| `test_linq_sorting.das` | order/order_by/reverse | sort_first, sort_take, select_where_order_take, reverse_take | ✅ ascending + `order_descending` (Phase 3); ✅ `reverse` (Phase 3+); ✅ `reverse \|> take(N)` backward index loop on array sources (PR-C — closes the prior regression) |
| `test_linq_group_by.das` | group_by/group_by_lazy/having | groupby_count, groupby_sum, groupby_min, groupby_max, groupby_first, groupby_multi_reducer, groupby_where_count, groupby_where_sum, groupby_select_sum, groupby_having_count, groupby_having_hidden_sum, groupby_average | ✅ count/long_count/sum + inner-select-sum (PR-A1); ✅ min/max/first + inner-select-{min,max,first} + multi-reducer (PR-A2); ✅ upstream where_/select* fusion (PR-B); ✅ `having_` with matching slot (PR-D); ✅ `average` + inner-select-average + multi-reducer-with-average (PR-A2 follow-up); ✅ hidden-slot reducer-in-having on named-tuple select shape (PR-E) |
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
