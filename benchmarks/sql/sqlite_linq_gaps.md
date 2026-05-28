# sqlite_linq lowering gaps — bench cells marked `—`

Catalogues every `—` SQL cell in [`results.md`](results.md), split by **what's
missing** rather than by bench. Each gap is either:

- **window functions** — the canonical SQL form requires `ROW_NUMBER() OVER (...)` /
  `PARTITION BY` / `LAG` / `LAG`, and `sqlite_linq` does not currently lower any
  window-function surface. One follow-up PR could open the whole group.
- **other deferred lowerings** — narrower surface gaps (`COUNT(DISTINCT
  computed-expr)`, `COUNT(*) FILTER (WHERE ...)`, `_select` after `_join`,
  LIMIT-before-WHERE semantics) where `sqlite_linq` has nothing equivalent
  today but each could land independently.
- **by design** — the chain has no faithful SQL form (zip is not relational;
  LIMIT after an aggregate is a no-op).

Cross-reference back to the `.das` bench file at each row — that's where the
gap is documented inline.

---

## ~~Window-function lowerings~~ — group fully closed

Originally catalogued 4 cells as needing `ROW_NUMBER() OVER (PARTITION BY K ORDER BY S)`.
Plan-mode probing established that all 4 lower cleanly via SQLite's bare-aggregate
optimization + composition (no window functions needed) — closed in two PRs.

**Closed in PR #2906** (chunk N — bare-aggregate foundation):
- `order_distinct_take` — closed by 1-column `Brand` table fixture (bench-only).
- `distinct_count_pred` — closed by the SQLite bare-aggregate wrap
  (`SELECT COUNT(*) FROM (SELECT *, MIN(pk) FROM t GROUP BY K) WHERE P`).
  The original gaps doc misidentified the lowering as `COUNT(*) FILTER (WHERE P)`
  over `SELECT DISTINCT K` — incorrect because `SELECT DISTINCT K` only projects
  K and P can't reference other columns. The bare-aggregate form preserves all
  `SELECT *` columns at the min-PK row, matching linq `_distinct_by` semantics.

**Closed in PR #2909** (chunk N+1 — composition + MAX(pk) + group-by-first):
- `distinct_by_order_take` — bare-aggregate + outer `ORDER BY` + `LIMIT` (no window function needed).
  `SELECT … FROM (SELECT *, MIN(pk) FROM Cars GROUP BY dealer_id) AS t0 ORDER BY price LIMIT N`.
- `distinct_by_order_to_array` — same without `LIMIT`.
- `reverse_distinct_by` — MAX(pk) variant of the bare-aggregate.
  `SELECT … FROM (SELECT *, MAX(pk) FROM Cars GROUP BY brand) AS t0`. New chain op
  `reverse()` recognized in `_sql` (only legal immediately above `_distinct_by`).
- `groupby_first` — tuple projection over bare-aggregate. `_._1 |> first()` recognized
  in grouped projections; outer SELECT lists key + expanded source columns; row builder
  reconstructs the source struct via `ExprMakeStruct` with offset reads.

Real window functions (`ROW_NUMBER`, `RANK`, `LAG`, etc.) remain unimplemented in
`sqlite_linq` — no current bench needs them. Future bench shapes like
`_distinct_by_min_by(K, S)` ("min-S row per K", not first-by-source-order) would
require them.

---

## ~~`_group_by` after `_join`~~ — closed in PR #2911

Originally catalogued 2 cells as blocked on resolving `_group_by` keys against
the join's `into` projection. Closed in PR #2911 (chunk N+2) via a central
`pred_to_sql` extension that consults a snapshot of the join's projection
registry — single hook transitively enables alias resolution in `_group_by` /
`_having` / `_order_by` / `try_translate_group_aggregate` / computed-expression
keys.

**Closed cells**:
- `join_groupby_count` — `_join |> _group_by(_.Brand) |> _select((Brand=_._0, N=_._1|>count()))`.
- `join_groupby_to_array` — `_join |> _group_by(_.Brand) |> _select((Brand=_._0, Total=_._1|>_select(_.Price)|>sum()))`.

The fix surfaced one residual: HAVING with an alias-resolved aggregate in the
`_sql(...)` runtime form hits a typer-ordering quirk; `_sql_text` emit is
correct, runtime path deferred to chunk N+3.

## Other deferred lowerings (independent, each its own PR)

### `COUNT(DISTINCT computed-expr)`

Bench: [`chained_select_collapse`](chained_select_collapse.das).

`distinct() |> count()` after a `_select` rejects today because the
collapsed `_select` projects a computed expression, and SQLite's
`COUNT(DISTINCT x)` accepts only column references in its grammar (or
`COUNT(DISTINCT expr)` with parenthesization, depending on dialect).
`sqlite_linq`'s emitter doesn't form the `DISTINCT computed-expr` shape
yet. Plausible to lower if the projected expression has no aggregates,
side effects, or non-pure functions.

### `_select` after `_join` — bare `_` rejected

Bench: [`join_select`](join_select.das).

`_sql`'s `_select` after `_join` only accepts the `into` lambda's
parameter names as receivers — bare `_` and computed projections are
rejected. The fix would be to inline the projection into the `into`
lambda before lowering, or surface a separate `_select` arm that
post-projects the join result via a derived table. Surface limitation, not
a query-shape impossibility.

### `LIMIT` semantics edge cases

Benches: [`take_count_filtered`](take_count_filtered.das),
[`take_sum_aggregate`](take_sum_aggregate.das),
[`take_where_count`](take_where_count.das).

In SQL:
- `LIMIT` after `COUNT(*)` / `SUM(...)` is a no-op (the aggregate collapses
  to one row).
- `LIMIT` applies after `WHERE`, not before, so `take(N) |> _where(P) |>
  count()` ("first N rows, count matching") would need a derived-table
  form (`SELECT COUNT(*) FROM (SELECT ... LIMIT N) WHERE ...`).

`sqlite_linq` doesn't currently lower the derived-table form. Each of
these can be addressed independently, but in practice the bench cells will
likely stay `—` because the chain shape is unusual in SQL idiom (it's a
fold-fold idiom).

---

## By design — no SQL form possible

### `zip` family — not a relational operation

Benches: [`zip_count_pred`](zip_count_pred.das),
[`zip_dot_product`](zip_dot_product.das),
[`zip_dot_product_3arg`](zip_dot_product_3arg.das),
[`zip_reverse_to_array`](zip_reverse_to_array.das).

`zip(a, b)` pairs elements by position across two unrelated sequences.
SQL has no concept of positional pairing across tables (joins are
key-based, not index-based). A `ROW_NUMBER()`-joined emulation
(`a JOIN b ON a.rn = b.rn`) would technically work but would be
fundamentally different from the in-memory zip — different cost model,
no real-world SQL idiom.

These cells will stay `—` permanently.

### `decs_count_bare_pred` — decs-specific Theme 4 root-cause fix

Bench: [`decs_count_bare_pred`](decs_count_bare_pred.das).

Covers a Theme 4 fix specific to the decs lane (bare
`from_decs_template(...).count(P)` with no upstream where/select
previously bailed because `forExpr.iteratorVariables` was unpopulated).
Array-side bare `count(P)` was always reachable;
SQL `count(P)` is covered by [`count_aggregate`](count_aggregate.das)
with a where shape. The `—` Array and SQL cells aren't gaps — this bench
is decs-only by design.

---

## Decs lane absent (not a SQL gap, but for symmetry)

### `reverse_distinct_by` decs

Bench: [`reverse_distinct_by`](reverse_distinct_by.das).

The Theme 8 2a splice arm requires `array_source` (see
[`daslib/linq_fold.das:3221`](../../daslib/linq_fold.das#L3221)) because the
backward-index walk (`arr[length-1-k]`) has no natural decs analog —
archetype walks are forward-only. A decs lane would need a new
plan_decs_reverse rewrite that collects the forward walk into a temp buffer
and then back-walks it (defeating the splice's whole "saves cascade's
reverse_to_array allocation" point). Closed by design.
