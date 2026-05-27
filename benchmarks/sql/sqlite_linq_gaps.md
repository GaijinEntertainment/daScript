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

## Window-function lowerings

All of these chains pick "one row per group" or "the row matching
`min/max/first ...` per group", which in SQL canonically uses
`ROW_NUMBER() OVER (PARTITION BY k ORDER BY ...)` filtered by `rn = 1` (or
`LIMIT 1`). Sometimes wrapped in a derived table. None of these forms are
emitted by today's `sqlite_linq` surface.

If one PR opens window-function lowering in `sqlite_linq` (e.g. a new
`_partition_by` / `_row_number` surface, or an internal `_window_first`
helper), all five of these cells can flip in the same change.

| Bench | Chain | What SQL would lower |
|---|---|---|
| [`distinct_by_order_take`](distinct_by_order_take.das) | `_distinct_by(_.dealer_id) \|> _order_by(_.price) \|> take(N) \|> to_array` | `SELECT ... FROM (SELECT *, ROW_NUMBER() OVER (PARTITION BY dealer_id ORDER BY price) AS rn FROM Cars) WHERE rn = 1 LIMIT N` |
| [`distinct_by_order_to_array`](distinct_by_order_to_array.das) | `_distinct_by(_.dealer_id) \|> _order_by(_.price) \|> to_array` | Same as above without `LIMIT N` |
| [`groupby_first`](groupby_first.das) | `_group_by(_.brand) \|> _select(g => g.first())` | `SELECT ... FROM (SELECT *, ROW_NUMBER() OVER (PARTITION BY brand ORDER BY id) AS rn FROM Cars) WHERE rn = 1` |
| [`reverse_distinct_by`](reverse_distinct_by.das) | `each(arr) \|> reverse() \|> _distinct_by(_.brand) \|> to_array` ("last per group") | `SELECT ... FROM (SELECT *, ROW_NUMBER() OVER (PARTITION BY brand ORDER BY id DESC) AS rn FROM Cars) WHERE rn = 1` |
| [`order_distinct_take`](order_distinct_take.das) | `_order_by(_) \|> distinct() \|> take(N) \|> to_array` on bare scalar | Independent of window functions — see "Other deferred lowerings" below |

**Original TODO dates** (carried in `results.md` Notes before this doc):
`distinct_by_order_take` 2026-05-25, `groupby_first` 2026-05-23. The new
2026-05-27 entries (`reverse_distinct_by`, `distinct_by_order_to_array`)
inherit the same blocker.

---

## Other deferred lowerings (independent, each its own PR)

### `_group_by` after `_join` — group key from joined projection

Benches: [`join_groupby_count`](join_groupby_count.das),
[`join_groupby_to_array`](join_groupby_to_array.das).

`sqlite_linq`'s `_group_by` after `_join` doesn't lower today because the
group key column comes from the join's `into` projection, not from a
base-table column. The lowering would need to either emit the `JOIN` as a
subquery and `GROUP BY` over the subquery's column, or surface the
projection-column → SQL-fragment mapping into `_group_by`'s key-extractor
logic. Originally TODO'd 2026-05-25.

### `COUNT(*) FILTER (WHERE ...)` — 2-arg `count(P)` after `_distinct_by`

Bench: [`distinct_count_pred`](distinct_count_pred.das).

`_distinct_by(K) |> count(P)` would lower as
`SELECT COUNT(*) FILTER (WHERE P) FROM (SELECT DISTINCT K FROM ...)`. The
`FILTER (WHERE ...)` clause is SQLite-supported (>= 3.30) but
`sqlite_linq` doesn't currently emit it. By design today, but a real
surface candidate.

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

### `order_distinct_take` — bare key on synthesized scalar

Bench: [`order_distinct_take`](order_distinct_take.das).

`_sql`'s `_order_by` requires a `_.Field` (column-ref) key, not bare `_`.
The bench operates on a synthesized `array<int>` with no named column, so
the SQL form has no `.Field` to project. Independent of window functions
— `distinct_take` proves `_sql` does lower
`distinct() |> take(N)` when the source has a named column. Adding the
SQL lane would need a 1-column table fixture and `_order_by(_.col)`.
Cosmetic gap; low priority.

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
