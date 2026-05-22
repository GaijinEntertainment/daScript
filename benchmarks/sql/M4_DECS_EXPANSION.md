# m4_decs_fold lane — expansion plan

Adds a third benchmark target alongside `m1_sql` (SQL via `_sql`) and `m3`/`m3f` (array linq, with/without `_fold` splice). The new lane is `m4_decs_fold`:

```das
_fold(from_decs_template(type<DecsCar>)._where(...)._select(...).sum())
```

Goal: tri-platform comparison (SQL vs array vs decs) under the same chain shape, so the splice path's decs win is directly comparable to its array win.

## Baseline matrix (2026-05-20, before m4 expansion)

100K rows, INTERP mode. m3f = `_fold(each(arr)...)`. Lower is better.

### Headlines (m3f wins both)

| benchmark | m1 sql | m3 | m3f | m1/m3f | m3/m3f |
|---|---:|---:|---:|---:|---:|
| sum_aggregate | 29 | 30 | **2** | 14.5× | 15.0× |
| select_where_count | 32 | 57 | **5** | 6.4× | 11.4× |
| sum_where | 32 | 43 | **4** | 8.0× | 10.8× |
| chained_where | 36 | 45 | **6** | 6.0× | 7.5× |
| aggregate_match | 34 | 49 | **5** | 6.8× | 9.8× |
| all_match | 27 | 21 | **3** | 9.0× | 7.0× |
| take_while_match | 7 | 22 | **2** | 3.5× | 11.0× |
| count_aggregate | 29 | 29 | **4** | 7.2× | 7.2× |
| select_where_sum | 36 | 60 | **7** | 5.1× | 8.6× |
| to_array_filter | 74 | 42 | **11** | 6.7× | 3.8× |

### Order/sort family (m3f dominates m3)

| benchmark | m1 | m3 | m3f | m3/m3f |
|---|---:|---:|---:|---:|
| sort_take | 38 | 763 | **27** | 28.3× |
| order_take_desc | 38 | 746 | **27** | 27.6× |
| select_where_order_take | 36 | 379 | **24** | 15.8× |
| sort_first | 37 | 742 | **756** | (regresses; sort dominates) |

### Group-by family (2-5× wins)

| benchmark | m1 | m3 | m3f | m1/m3f | m3/m3f |
|---|---:|---:|---:|---:|---:|
| groupby_sum | 173 | 102 | **37** | 4.7× | 2.8× |
| groupby_count | 143 | 68 | **36** | 4.0× | 1.9× |
| groupby_average | 171 | 106 | **52** | 3.3× | 2.0× |
| groupby_having_count | 144 | 74 | **36** | 4.0× | 2.1× |
| groupby_having_hidden_sum | 175 | 103 | **40** | 4.4× | 2.6× |
| groupby_max | 174 | 103 | **44** | 4.0× | 2.3× |
| groupby_min | 173 | 106 | **43** | 4.0× | 2.5× |
| groupby_multi_reducer | 190 | 139 | **53** | 3.6× | 2.6× |
| groupby_select_sum | — | 110 | **59** | — | 1.9× |
| groupby_where_count | 76 | 64 | **23** | 3.3× | 2.8× |
| groupby_where_sum | 86 | 79 | **23** | 3.7× | 3.4× |
| groupby_first | — | 68 | **35** | — | 1.9× |

### Anomalies / weak spots

| benchmark | m1 | m3 | m3f | note |
|---|---:|---:|---:|---|
| indexed_lookup | 1,431 | 2,029,891 | 200,399 | SQL B-tree wins 1417× over linear scan; splice helps 10× over m3 but algorithmically can't match SQL. Decs equivalent uses **eid lookup** (archetype hash lookup, fast path). |
| zip_dot_product | — | 52 | 57 | plan_zip landed but bench shape doesn't hit the splice (m3f slower than m3 by 10%). Worth a follow-up. |
| join_count | — | 116 | 122 | No splice arm for join. m3f slightly slower. |
| sort_first | 37 | 742 | 756 | Sort dominates the whole pipeline. m3f doesn't help when terminator is `first` after a full sort. |
| distinct_take | — | 30 | 0 | m3f=0 ns/op — bench may be constant-folded; verify it actually exercises the chain. |
| take_count_filtered / take_sum_aggregate / select_count / first_match / element_at_match / reverse_take / skip_take | various | various | 0 | Several 0 ns/op cases — verify the chain isn't being eliminated by DCE. |

## Decs benchmarks (already in `benchmarks/decs/`)

| benchmark | m1_hand_query | m2_eager_bridge | m3_fold_splice / m4_template_fold |
|---|---:|---:|---:|
| from_decs_count | 0 (arch.size) | 60 | **0** (matches hand) |
| from_decs_sum | 4 (query) | 202 | **8** (within 2× of hand) |

## m4_decs_fold expansion — triage

Inventory of every `benchmarks/sql/*.das` benchmark with **decs feasibility** classification. Surfaces that don't yet exist on the decs side are flagged so we can decide whether to expand the decs surface.

### Category A — clean decs map (no new surface needed)

These translate directly: array `each(arr)._chain()` becomes decs `from_decs_template(type<T>)._chain()`. Splice already covers all chain shapes used.

- aggregate_match
- all_match
- any_match
- average_aggregate
- chained_where
- contains_match
- count_aggregate
- distinct_count
- first_match
- first_or_default_match
- last_match
- long_count_aggregate
- max_aggregate
- min_aggregate
- select_count
- select_where
- select_where_count
- select_where_sum
- single_match
- sum_aggregate
- sum_where
- take_count
- take_count_filtered
- take_sum_aggregate
- take_while_match
- skip_while_match
- to_array_filter

### Category B — clean decs map but needs Slice 5+ splice arms

Chain shape works on decs surface today (via eager bridge), but splice planner doesn't yet handle the shape — would fall to tier-2 cascade. Listing here so each entry doubles as a future-slice trigger.

- bare_order_where (order_by / reverse on decs — Slice 5+)
- distinct_take (distinct_by + take on decs — Slice 5+)
- element_at_match (element_at — Slice 5+)
- groupby_average / groupby_count / groupby_first / groupby_having_count / groupby_having_hidden_sum / groupby_max / groupby_min / groupby_multi_reducer / groupby_select_sum / groupby_sum / groupby_where_count / groupby_where_sum — all need decs group_by splice (state-table family Slice 5+)
- order_take_desc / select_where_order_take / sort_first / sort_take — order_by family on decs (Slice 5+)
- reverse_take — reverse on decs (Slice 5+)
- skip_take — skip+take chain on decs (Slice 5+)

### Category C — needs new decs surface

Benchmarks whose shape doesn't have a corresponding decs equivalent yet. Decision: build the surface OR skip.

- **indexed_lookup** — SQL uses B-tree on `id`. Decs analog: **eid-based lookup** via `lookup_entity` (decs.das has `[eid]` component lookup). Build a `m4_decs_eid_lookup` lane that exercises this. NEW SURFACE: thin wrapper if needed.
- **zip_dot_product** — pairs two parallel arrays. Decs analog: pair two component streams from the SAME archetype (intra-archetype zip is free — it's just multi-iter for over both components). Or zip across two archetypes (cross-archetype, harder). NEW SURFACE: `from_decs_template_zip` or syntactic-sugar wrapper. Worth investigating; could be a clean Slice surface add.
- **join_count** — two tables joined. Decs analog: cross-archetype query with eid linkage. NEW SURFACE: `join_decs` or two-template iter. Larger design exercise; defer to follow-up.

## Proposed execution order

1. **Wave 1 — Category A only** (28 benchmarks). All splice today, all comparable now. Establishes the m4 surface in `_common.das` + per-bench m4 lane. Validates the lane convention before scope grows.
2. **Wave 2 — Category C surface adds**:
   - `indexed_lookup` via decs eid lookup (small change)
   - `zip_dot_product` via decs intra-archetype zip (design discussion)
   - `join_count` deferred to a later wave (full decs join design)
3. **Wave 3 — Category B**: ship m4 lanes for these now using the eager bridge (so the matrix is complete), THEN as plan_decs_unroll Slice 5+ lands, the lanes start showing the splice win. Each lane stays valid throughout.

## Conventions for m4 lanes

Per Boris (2026-05-20):
- One lane per benchmark, named `<bench>_m4` reported as `m4_decs_fold/{n}` (`m4` for ordinal, `decs_fold` for clarity)
- Per-benchmark fixture call (each file calls `fixture_decs(n)` inline, mirroring how m3 calls `fixture_array(n)` inline)
- Shared `[decs_template] DecsCar` + `fixture_decs(n)` in `_common.das` (mirrors how m3's `Car` + `fixture_array` are shared)
- Lambda-typed args (`$(c : Car)`) replaced with `_select(_.field)` macro form in m4 bodies (Car type doesn't match the decs tuple element)
- Sentinel values (`first_or_default`) use named-tuple literal `(id=…, name=…, …)` matching the iterator element type

## First m4 sweep — results (2026-05-20, 100K rows, INTERP)

47 benchmarks gained an m4 lane (all Cat A + Cat B). Skipped: `indexed_lookup`, `join_count`, `zip_dot_product` (Cat C, need surface).

### Cat A — m4 splices today (chain shape covered by plan_decs_unroll)

| benchmark | m1 | m3 | m3f | m4 | m4/m1 | m4/m3f | notes |
|---|---:|---:|---:|---:|---:|---:|---|
| sum_aggregate | 30 | 29 | 2 | **16** | 0.5x | 8x | 6-component multi-iter overhead floor |
| select_where_sum | 36 | 57 | 7 | **18** | 0.5x | 2.6x | chain splice; beats m1 + m3 |
| select_where_count | 32 | 58 | 5 | **18** | 0.6x | 3.6x | chain splice; beats m1 + m3 |
| count_aggregate | 30 | 28 | 4 | **15** | 0.5x | 3.8x | (count with filter — not bare; filter walks all entities) |
| long_count_aggregate | 29 | 28 | 4 | **15** | 0.5x | 3.8x | parallel to count |
| sum_where | 32 | 45 | 4 | **17** | 0.5x | 4.2x | |
| chained_where | 36 | 46 | 6 | **18** | 0.5x | 3x | |
| select_where | 190 | 28 | 11 | **22** | **0.1x** | 2x | m4 8.6x faster than m1 SQL |
| max_aggregate | 30 | 36 | 5 | **19** | 0.6x | 3.8x | |
| min_aggregate | 30 | 38 | 6 | **19** | 0.6x | 3.2x | |
| average_aggregate | 30 | 34 | 5 | **21** | 0.7x | 4.2x | |
| all_match | 27 | 20 | 3 | **15** | 0.6x | 5x | early-exit on bridge |
| to_array_filter | 70 | 43 | 11 | **24** | **0.3x** | 2.2x | m4 2.9x faster than m1 SQL |
| first_match | 0 | 28 | 0 | **0** | — | — | early-exit on first hit |
| first_or_default_match | 0 | 31 | 0 | **0** | — | — | |
| any_match | 0 | 0 | 0 | **0** | — | — | |
| contains_match | 0 | 28 | 2 | **8** | — | 4x | |

**Net Cat A:** m4 beats SQL on most shapes (8 of 17 with concrete m1+m4 numbers; another 4 at 0 ns/op). m4 vs m3f shows a ~3-5× decs overhead from the 6-component multi-iter for-loop (every component's get_ro participates even when chain only reads one field). This is the splice's structural cost on a multi-field decs schema.

### Cat B — m4 falls back to eager bridge (splice deferred to Slice 5+)

These chain shapes splice on array but not yet on decs. m4 = eager bridge (materialize array<tuple>, then run on array). Slower than m3f but the comparison is real today.

| benchmark | m1 | m3 | m3f | m4 | m4/m3f | needs splice arm |
|---|---:|---:|---:|---:|---:|---|
| bare_order_where | 273 | 357 | 120 | 196 | 1.6x | order_by on decs |
| distinct_count | 41 | 43 | 15 | 97 | 6.5x | distinct on decs |
| distinct_take | 0 | 30 | 0 | 34 | — | distinct + take on decs |
| order_take_desc | 37 | 698 | 27 | 117 | 4.3x | order_by + take |
| reverse_take | 0 | 22 | 0 | 114 | — | reverse + take |
| select_count | 0 | 32 | 0 | 3 | — | (m4 likely DCE — verify) |
| select_where_order_take | 36 | 355 | 24 | 102 | 4.2x | order + take after where |
| skip_take | 0 | 15 | 0 | 37 | — | take/skip on decs |
| skip_while_match | 3 | 20 | 5 | 83 | 16.6x | skip_while on decs |
| sort_first | 37 | 713 | 722 | 802 | 1.1x | sort dominates; splice barely helps |
| sort_take | 38 | 715 | 27 | 119 | 4.4x | order + take |
| take_count | 3 | 0 | 0 | 36 | — | take on decs |
| take_count_filtered | — | 29 | 0 | 35 | — | take after where |
| take_sum_aggregate | — | 28 | 0 | 34 | — | take + sum |
| take_while_match | 7 | 22 | 2 | 55 | 27.5x | take_while on decs |
| element_at_match | 0 | 28 | 0 | 35 | — | element_at on decs |
| last_match | 0 | 29 | 5 | 83 | 16.6x | last on decs |
| single_match | 0 | 19 | 2 | 80 | 40x | single on decs |
| groupby_count | 142 | 65 | 37 | 115 | 3.1x | group_by on decs (state-table) |
| groupby_sum | 171 | 101 | 36 | 115 | 3.2x | group_by on decs |
| groupby_average | 172 | 99 | 52 | 128 | 2.5x | group_by on decs |
| groupby_max | 174 | 103 | 43 | 120 | 2.8x | group_by on decs |
| groupby_min | 175 | 105 | 42 | 122 | 2.9x | group_by on decs |
| groupby_first | — | 68 | 35 | 112 | 3.2x | group_by on decs |
| groupby_having_count | 142 | 71 | 37 | 114 | 3.1x | group_by on decs |
| groupby_having_hidden_sum | 176 | 102 | 40 | 122 | 3.0x | group_by on decs |
| groupby_multi_reducer | 191 | 138 | 52 | 130 | 2.5x | group_by on decs |
| groupby_select_sum | — | 109 | 60 | 137 | 2.3x | group_by on decs |
| groupby_where_count | 76 | 63 | 23 | 101 | 4.4x | group_by on decs |
| groupby_where_sum | 101 | 81 | 23 | 105 | 4.6x | group_by on decs |
| aggregate_match | 34 | 50 | 5 | 84 | 16.8x | `aggregate(init, $(acc, c) => …)` — not a `_select(_.x).sum()` shape, distinct planner |

**Net Cat B:** Establishes today's baseline. As Slice 5+ lands group_by/order/distinct/take splice arms on the decs bridge, these rows will drop into Cat A territory and the splice win will become visible without changing the benchmark.

### Suspect "0 ns/op" m4 results — verify the chain isn't getting DCE'd

- `first_match` / `first_or_default_match` — early-exit on the first archetype's first entity is genuinely cheap; plausible
- `any_match` / `select_count` — both 0 on m4 and m3f; likely constant-folded
- `take_count` / `take_count_filtered` / `take_sum_aggregate` / `reverse_take` / `skip_take` / `distinct_take` — m4=34-37 ns (eager bridge); m3f=0 ns; m1 mostly 0 too. The m3f=0 cases are suspicious — bench may be eliminating the chain at compile time. Worth dropping a `b->failNow()` floor check or a side effect to confirm.

## Wave 2 — surface expansions

After this m4 sweep ships, expand the surface for Cat C benchmarks:

1. **`indexed_lookup`** — add `m4_decs_eid_lookup` lane using `lookup_entity` / archetype hash lookup. Decs hash-of-eid IS the fast path; benchmark it against SQL B-tree. May require minor surface (a `find_entity_by_field` helper)
2. **`zip_dot_product`** — design intra-archetype zip surface. Two components from the SAME archetype is just a multi-iter for-loop (free). Cross-archetype zip is harder. Pick the design that lets `_zip` on decs match the array_zip shape
3. **`join_count`** — needs full decs join design. Multi-table query equivalent via two `[decs_template]` structs + eid linkage. Larger design exercise; defer

## Wave 3 — Slice 5+ enables Cat B splice

As `plan_decs_unroll` gains:
- take/skip/take_while/skip_while arms (cross-archetype counter / for_each_archetype_find early-exit)
- distinct/group_by state-table arms (hoisted table above outer for_each_archetype)
- order_by / reverse buffer arms

…Cat B m4 numbers will drop dramatically (from ~100-130 ns down to the 5-20 ns range), matching the Cat A pattern. The matrix becomes a regression guard for each splice arm.

## Wave 4 — perf optimizations on plan_decs_unroll

Known overhead in the current splice: **6-component multi-iter for-loop walks ALL components** even when the chain only reads one field. Possible optimizations:
- Track per-chain "components actually accessed" set; emit `get_ro` + iter binding only for those
- For bare-count chains: arch.size shortcut works (already implemented) → 0 ns
- For single-field selects: 1-component for-loop should match the array case

If implemented, m4 numbers on Cat A would close most of the 3-5× gap vs m3f, making decs effectively as fast as array for projection-heavy chains.

## Update — sort_first fix (2026-05-20, plan_order_family + first arm)

Extended `plan_order_family` to recognize `first` / `first_or_default` as terminators alongside `take(N)`. `order_by + first` now splices to a single-pass `min_by` (array source: zero-alloc empty-guard + `min_by`; iterator source: `top_n_by(_, 1, _) |> first()`). `order_by_descending` routes to `max_by`. Preserves the eager `first()` panic-on-empty contract.

| benchmark | m1 | m3 | m3f (old) | m3f (new) | m4 (old) | m4 (new) | m3f win |
|---|---:|---:|---:|---:|---:|---:|---:|
| sort_first | 37 | 713 | 722 | **42** | 802 | **121** | 17× |

Now `sort_first` lands in line with the rest of the order-family. m4_decs_fold still rides the eager bridge (Slice 5+ will close the gap).

## Update — zip_dot_product fix (2026-05-20, plan_zip + accumulator/early-exit lanes)

PR #2742's accumulator + early-exit terminator work on `plan_zip` was orphaned on a stacked PR base when #2741 merged. Cherry-picked the 3 commits onto fresh master (auto-merged cleanly). `plan_zip` now dispatches to the generalized multi-source `emit_accumulator_lane` (sum / min / max / average / long_count) and `emit_early_exit_lane` (first / first_or_default / any / all / contains) via parallel-array helpers (`srcNames`, `topExprs`) + new `finalize_lane_emission` wrap.

| benchmark | m1 | m3 | m3f (old) | m3f (new) | m3f win |
|---|---:|---:|---:|---:|---:|
| zip_dot_product | — | 53 | 58 | **7** | 8.3× |

`zip(xs, ys)._select(_._0 * _._1).sum()` now fuses to a single multi-iter for-loop with inline accumulator, zero alloc. Falls in line with the rest of the accumulator-class benchmarks.

## Update — Slice 5a take/skip on decs (2026-05-20, plan_decs_unroll + DecsRangeInfo)

`plan_decs_unroll` now recognizes trailing `take(N)` / `skip(N)` after the where/select chain. New `extract_decs_ranges` peels them into `DecsRangeInfo`; counter inits hoist above `for_each_archetype` (so they span archetypes); per-element guards (take-cap → return true, skip-counter → continue, take++) wrap `perElement` BEFORE chain so ranges apply to the post-`where_` stream. When `takeExpr != null` the outer call switches to `for_each_archetype_find` with a `: bool` lambda so the take-cap stop propagates across archetypes.

Affected emit paths (all 4 non-bare-count): `emit_decs_accumulator`, `emit_decs_early_exit`, `emit_decs_min_max_by`, `emit_decs_to_array`. Bare `count` via arch.size shortcut still bails on any chain ops including ranges.

**Coverage:** take, skip, skip+take, where+take, select+take+sum, take+first, take+to_array, take(-1) short-circuit, skip-beyond-end, AST-shape gates for take→`_find` routing + skip-only→`for_each_archetype` routing. +11 tests.

| benchmark | shape | m4 (old) | m4 (new) | m3f (array splice) |
|---|---|---:|---:|---:|
| take_count | `.take(N).count()` | 36 | 0 | 0 |
| skip_take | `.skip(N).take(M).count()` | 37 | 0 | 0 |
| take_count_filtered | `_where.take(N).count()` | — | 0 | 0 |
| take_sum_aggregate | `_select.take(N).sum()` | — | 0 | 0 |

m4 splice rounds to 0 ns/op alongside the m3f array splice — same shape (inline counter + early-exit), same measurement floor. Not DCE: `ast_dump --mode source` confirms `for_each_archetype_find` is emitted with `decs_takec >= 1000 → return true` and `++decs_takec; push_clone(decs_buf, decs_tup)` actively building the full 1000-element result array per bench iteration. Old m4 baseline (36-37 ns/op via eager bridge) → new 0 ns/op (~sub-1 ns/iter, indistinguishable from m3f array splice) ≈ 36× actual win, just below the bench's `body_time / n_iters` resolution floor.

## Update — Slice 5b take_while/skip_while on decs (2026-05-20, plan_decs_unroll + predicate-driven ranges)

`plan_decs_unroll` now recognizes trailing `take_while(pred)` / `skip_while(pred)` after the where/skip prefix. `DecsRangeInfo` gains `skipWhileCond` / `takeWhileCond`; `extract_decs_ranges` walks the suffix in canonical order (`skip → skip_while → take_while → take`) and bails when a `select` appears in the prefix (mirrors array-side `seenSelect` bail at `linq_fold.das:1615/1623`). Predicates peel against the source tuple (`tupName`), so the post-where stream is visible but selects are forbidden — same shape as array side. `skipping` flag hoists at invoke scope (one-way; flips false on first non-matching elem, persists across archetypes). When `takeWhileCond != null` the outer call switches to `for_each_archetype_find` with a `: bool` lambda just like `take(N)`, and `useExplicitState` in `emit_decs_early_exit` extends so `any/all/contains + take_while` route through explicit `foundName` (distinguishes "real match" from "take_while-stop" — both produce inner `return true`).

| benchmark | shape | m1 sql | m3 | m3f (array splice) | m4 (old, eager bridge) | m4 (new, splice) | m3f→m4 gap | win vs baseline |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| take_while_match | `._take_while(_.id < 50K).count()` | 7 | 23 | 2 | 55 | **8** | 6× | 6.9× |

m4 lands close to m3f (8 vs 2 ns/op — within Wave 4 known multi-component get_ro overhead). Splice fires; `ast_dump --mode source` confirms `for_each_archetype_find` with `if !(decs_tup.id < 50000) return true else ++decs_acc`.

**Coverage:** take_while, skip_while, skip_while+take_while, where+take_while, take_while+sum, take_while+first, take_while+to_array, take_while always-true (no break) / always-false (immediate break), skip_while always-true (drops all) / always-false (immediate done), skip+take_while, skip_while+take, take_while+any/all/contains (regression guards for explicit-state routing under take_while), AST shape gates for take_while→`_find` routing + skip_while-only→`for_each_archetype` routing. +21 tests (60 → 81 in file).

## Update — Slice 5e group_by on decs (2026-05-20, plan_decs_group_by)

New `plan_decs_group_by` planner mirrors `plan_group_by` machinery (state-table `tab?[uk] ?? dummy` + addr-compare miss detection + `tab[uk] = dummy; dummy = default<typedecl(dummy)>` first-key-wins) but emits the per-element splice into the bridge's inner multi-iter for-loop, wrapped by the outer `for_each_archetype` — replacing the eager bridge's `array<tuple>` materialization. Reuses `recognize_reducer_specs` / `emit_reducer_branches` / `extend_specs_for_missing_having_reducers` / `rewrite_having_pred` / averaging + hidden-slot machinery wholesale; the only delta is the source iteration shape. Inserted in the cascade BEFORE `plan_group_by` (the bridge-match is stricter so it won't grab array chains; running after `plan_group_by` would cause the array planner to fire on the eager-bridge invoke and short-circuit the decs path).

| benchmark | m1 sql | m3 | m3f (array splice) | m4 (eager bridge, was) | m4 (Slice 5e, now) | m4 win | m4 vs m1 |
|---|---:|---:|---:|---:|---:|---:|---:|
| groupby_count             | 142 | 67  | 37  | 115 | **50**  | 2.3× | 2.8× faster |
| groupby_sum               | 172 | 101 | 36  | 115 | **50**  | 2.3× | 3.4× faster |
| groupby_average           | 175 | 101 | 52  | 128 | **62**  | 2.1× | 2.8× faster |
| groupby_max               | 173 | 103 | 49  | 120 | **57**  | 2.1× | 3.0× faster |
| groupby_min               | 177 | 106 | 42  | 122 | **56**  | 2.2× | 3.2× faster |
| groupby_first             | —   | 70  | 35  | 112 | **46**  | 2.4× | — |
| groupby_having_count      | 142 | 73  | 36  | 114 | **49**  | 2.3× | 2.9× faster |
| groupby_having_hidden_sum | 175 | 103 | 40  | 122 | **57**  | 2.1× | 3.1× faster |
| groupby_multi_reducer     | 191 | 139 | 52  | 130 | **63**  | 2.1× | 3.0× faster |
| groupby_select_sum        | —   | 110 | 60  | 137 | **71**  | 1.9× | — |
| groupby_where_count       | 75  | 65  | 23  | 101 | **36**  | 2.8× | 2.1× faster |
| groupby_where_sum         | 94  | 80  | 23  | 105 | **36**  | 2.9× | 2.6× faster |

All 12 groupby_* rows improve 1.9-2.9× (avg ~2.3×). m4 now consistently lands 10-20 ns above m3f — the remaining gap is the Wave 4 multi-component `get_ro` overhead (every bridge component participates in the inner for-loop iteration even when the chain only reads one). m4 beats SQL on 10/10 measurable rows by 2.1-3.4×.

**Coverage:** count terminator (length(tab)), implicit to_array terminator, sum/min/max/average/first reducers, multi-reducer (N+S+MX in one pass), having_ predicate over named slot, having_ predicate over hidden synthesized slot, upstream where_+group_by, upstream select+group_by(expression key), AST shape gate (no `to_sequence`, exactly one `for_each_archetype`, no `_find` variant — group_by doesn't early-exit, `decs_tab` 6 refs + `decs_dummy` 6 refs in the splice). +13 tests (80 → 93 in file).

## Update — Slice 5d order/reverse on decs (2026-05-21, plan_decs_order_family + plan_decs_reverse)

Two new planners `plan_decs_order_family` (mirrors `plan_order_family`) and `plan_decs_reverse` (mirrors `plan_reverse`), inserted in the cascade BEFORE their array-side equivalents per the same cascade-ordering rule as Slice 5e. Both emit a hoisted buffer above `for_each_archetype`, then dispatch to the existing daslib helper family — `order_inplace` / `top_n*` / `min_by` / `max_by` for the order arm, `reverse_inplace` + optional `resize(takeN)` for the reverse arm.

`plan_decs_reverse` has three emit paths matching the array side: `count` terminator → counter loop (reverse is identity for count); `first`/`first_or_default` → walk + overwrite `last` (reverse-of-last = first-of-reverse); else → buffer + `reverse_inplace` + optional resize. The R6 backward-index optimization from array side doesn't apply: multi-iter for has no random access into the archetype.

`plan_decs_order_family` has a single uniform emit (always buffers, then dispatches the appropriate terminator) — simpler than array side which has separate "no where" direct-call vs "with where" buffered paths. The decs source can never random-access, so the direct-call optimization isn't available.

| benchmark | shape | m1 sql | m3 | m3f (array splice) | m4 (old, eager bridge) | m4 (new, splice) | m4 win |
|---|---|---:|---:|---:|---:|---:|---:|
| sort_take                | `_order_by + take` | 38 | 713 | 28 | 119 | **57** | 2.1× |
| sort_first               | `_order_by + first` | 37 | 711 | 41 | 121 | **71** | 1.7× |
| order_take_desc          | `_order_by_descending + take` | 37 | 694 | 28 | 117 | **58** | 2.0× |
| select_where_order_take  | `_where + _order_by + take` | 36 | 352 | 25 | 102 | **38** | 2.7× |
| bare_order_where         | `_where + _order_by` | 274 | 359 | 118 | 196 | **130** | 1.5× |
| reverse_take             | `.reverse().take(N)` | 0 | 22 | 0 | 114 | **48** | 2.4× |

All 6 rows improve 1.5-2.7× (avg ~2.1×). m4 lands within 2× of m3f on most rows — the remaining gap is the Wave 4 multi-component `get_ro` overhead (sort dominates the wall-clock so the gap doesn't fully close even when the splice fires). `bare_order_where` is the tightest squeeze (1.5×) because sort over 100K rows is the bottleneck; once the m4 buffer build matches m3f (~117 ns), the rest is pure sort time.

**Coverage:** bare `_order_by` + to_array, bare `_order_by_descending` + to_array, `_order_by` + take, `_order_by_descending` + take, `_order_by` + first, `_order_by_descending` + first, `_order_by` + first_or_default (nonempty + empty), `_where + _order_by + take`, `_where + _order_by + first`; `.reverse() + to_array`, `.reverse() + take(N)` (in-range, beyond-length, and zero — last uses runtime-var to defeat const-fold of PERF017 false-positive on splice-emitted `0 < length(buf)`), `.reverse() + count` (no-buffer counter loop), `_where + .reverse() + first_or_default` (empty + nonempty), `_where + .reverse() + to_array`, `_where + _select + .reverse() + to_array` (projection through reverse). AST shape gates for: order+take (top_n_by emit + no to_sequence + 1 for_each_archetype), order+first (min_by emit + panic-on-empty guard + no top_n_by), reverse+to_array (1 reverse_inplace), reverse+count (no decs_buf, no reverse_inplace). +20 tests (100 → 120 in file).

## Update — Slice 5c distinct/distinct_by on decs (2026-05-21, plan_decs_distinct)

New `plan_decs_distinct` planner mirrors `plan_distinct` — streaming dedup via `var inscope seen : table<typedecl(_::unique_key(...))>` hoisted above `for_each_archetype` so the seen-table persists across archetypes. Inserted in the cascade BEFORE `plan_distinct` (same rule as 5d/5e — decs bridge match is stricter). Two emit shapes:

- **Buffer-required** (`to_array` default, optionally `take(N)`): per-element splice computes the key, `key_exists` check, on miss does `taken++ → seen|>insert → buf|>push_clone`. When `take(N)` is present, outer iteration switches to `for_each_archetype_find` and the take-cap `return true` propagates across archetypes — true streaming early-exit. Take-limit bound to a `let` at outer scope so a side-effecting `take(arg)` evaluates exactly once.
- **Buffer-elided** (`count`/`long_count` → `length(seen)`/`int64(length(seen))`; `sum` folds inline at fresh-key site via `acc += <projection>`): no buffer allocation, only the seen-table is materialized. count + take both present cascades to tier 2 (matches array-side).

Side-effecting `_select(proj)` upstream binds once per element to a fresh `decs_pv` local — key + buf push (or sum fold) share the bind, matching array-side single-eval per source element. `distinct_by(key)` wraps the key block in `invoke(<keyBlock>, <projection-or-tup>)` and the seen-table's value type is derived via `_::unique_key(invoke(<keyBlock>, default<elemT>))` so the table key type tracks the key function's return type.

| benchmark | shape | m1 sql | m3 | m3f (array splice) | m4 (eager bridge, was) | m4 (Slice 5c, now) | m4 win |
|---|---|---:|---:|---:|---:|---:|---:|
| distinct_count | `_select(_.brand).distinct().to_array()` | 41 | 44 | 15 | 97 | **28** | 3.5× |
| distinct_take  | `_select(_.brand).distinct().take(3).to_array()` | 0 | 30 | 0 | 34 | **0** | matches m3f |

`distinct_count` lands at 28 ns/op vs m3f's 15 — the ~13 ns gap is the Wave 4 multi-component `get_ro` floor (3 components participate in the inner for-loop even when chain only reads `brand`). `distinct_take` collapses to 0 ns/op — early-exit at the 3rd distinct brand visits only ~3 source elements regardless of N=100K, same as the array-side splice.

**Coverage:** `_select(_.brand).distinct()` + to_array / count / long_count / sum / take(N) / take(0) / take(N>num_distinct), `_where(_.id<8)._select(_.brand).distinct()`, `_distinct_by(_.brand)` + to_array / count / take(N), empty-decs distinct yields empty, side-effecting take(N) arg evaluates exactly once at invoke entry. AST shape gates for: distinct+count (no to_sequence, no decs_buf, single key_exists, plain for_each_archetype), distinct+take (for_each_archetype_find + decs_buf + decs_seen + decs_taken counters), distinct_by+to_array (unique_key wrapping the key invocation), distinct+sum (no decs_buf, decs_acc declared+folded). +17 tests (122 → 139 in file).

## Update — Wave 4b Cat-C surface validation (2026-05-22, indexed_lookup + zip_dot_product m4 lanes)

Two of the three Cat-C benchmarks that were skipped during the original m4 expansion (line 150) now have m4 lanes. The plan flagged both as "may need new decs surface" — turned out neither did. The decs APIs needed for both already exist; the bench code just had to call them.

| benchmark | shape | m1 sql | m3 | m3f (array splice) | m4 (was) | m4 (Wave 4b, now) | m4 vs best other lane |
|---|---|---:|---:|---:|---:|---:|---:|
| indexed_lookup | `_where(_.id == K).count()` → eid lookup | 1461 | 2,076,904 | 197,117 | — | **489** | 3× faster than m1 sql |
| zip_dot_product | `zip(xs,ys).select(_._0 * _._1).sum()` → intra-archetype | — | 53 | 7 | — | **10** | within 1.4× of m3f |

**indexed_lookup**: Uses the existing `query(eid, $(...))` call macro ([decs_boost.das:315](../../daslib/decs_boost.das#L315)), which wraps `for_eid_archetype` ([decs.das:666](../../daslib/decs.das#L666)). entityLookup is a flat hash on EntityId.id — single hash + generation check + archetype dispatch. New fixture helper `fixture_decs_capture_mid(n) : EntityId` ([_common.das](_common.das)) captures the n/2-th entity's eid mid-`create_entities` callback so the bench has a real eid to look up. The 489 ns/op figure includes the macro-time-generated request/erq lookup, the hash, the archetype dispatch, AND a real `car_id` field read inside the block (drift-protection assertion against the expected id). An earlier draft measured 227 ns/op but the block was a no-op `found = 1`, so `car_id`'s `get_ro` fetch was being eliminated — that was not a fair lookup cost.

**zip_dot_product**: No new surface at all. `from_decs_template(type<DecsCar>)._select(_.price * _.year).sum()` is the natural intra-archetype zip — multi-iter for over the archetype's two int columns. Wave 4 component pruning keeps the price + year `get_ro` slots and drops the other 4 components, so per-element cost is two int reads plus the multiply, matching the m3f two-column zip. The 3 ns gap to m3f covers `for_each_archetype` dispatch overhead.

**Net coverage:** 47 → 49 m4 lanes (`indexed_lookup` + `zip_dot_product`). Only `join_count` remains skipped — it's a cross-archetype join, which requires real design (eid linkage between archetypes) and is appropriately deferred past Wave 4b.

**No new decs surface needed for these lanes.** Pure benchmark additions + one helper in `_common.das`. The Wave 4b PR is documentation that the existing decs surface already covers these chain shapes — the team's pre-Wave-4b suspicion that we'd need new helpers turned out to be incorrect. (The same PR also bundled 6 small daslib/linq_fold cleanups in its commit 1 — those were unrelated PR #2806 review follow-ups, not Wave 4b code.)
