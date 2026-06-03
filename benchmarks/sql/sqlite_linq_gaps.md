# sqlite_linq lowering gaps — bench cells marked `—`

Catalogues every `—` SQL cell in [`results.md`](results.md), split by **what's
missing** rather than by bench. Each gap is either:

- **window functions** — the canonical SQL form requires `ROW_NUMBER() OVER (...)` /
  `PARTITION BY` / `LAG` / `LAG`, and `sqlite_linq` does not currently lower any
  window-function surface. One follow-up PR could open the whole group.
- **other deferred lowerings** — narrower surface gaps (`COUNT(DISTINCT
  computed-expr)`, `COUNT(*) FILTER (WHERE ...)`) where `sqlite_linq` has
  nothing equivalent today but each could land independently. (`_select`
  after `_join`, LIMIT-before-aggregate, and computed-scalar `_select` are
  now **closed** — see below.)
- ~~**deferred (computed cast)**~~ — **closed.** Workhorse casts
  (`int*`/`uint*`/`float`/`double`/`string`) now lower to SQLite
  `CAST(x AS INTEGER/REAL/TEXT)` in `fn_call_to_sql`, so the wide-result
  dot-product sum lanes are lit (see "Computed-cast projection" below).
- **by design** — the chain has no faithful SQL form (zip is positional,
  not relational; `reverse()` has no SQL order key).

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

## ~~`_group_by` after `_join`~~ — closed in PR #2910

Originally catalogued 2 cells as blocked on resolving `_group_by` keys against
the join's `into` projection. Closed in PR #2910 (chunk N+2) via a central
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

`distinct() |> count()` after a `_select` rejects today because it needs
`COUNT(DISTINCT <expr>)` (DISTINCT *inside* the aggregate), which
`sqlite_linq`'s emitter doesn't form yet. (Computed-scalar `_select`
itself now lowers — see "Closed" below — so the remaining gap is the
aggregate-distinct shape + the chained `_select |> _select` collapse.)
Deferred to the aggregate-distinct follow-up.

### ~~`_select` after `_join`~~ — closed (into-projection `m1`)

Bench: [`join_select`](join_select.das).

A standalone `_select` after `_join` still rejects bare `_` (only the
`into` lambda's parameter names are valid receivers), but the comparable
SQL inlines the projection into the `into` lambda
(`_join(..., $(c,d) => (CarName = c.name))`) — same result. The bench
`m1` lane uses that form.

### ~~`LIMIT` before an aggregate~~ — closed (inner-subquery wrap)

Benches: [`take_count_filtered`](take_count_filtered.das),
[`take_sum_aggregate`](take_sum_aggregate.das),
[`take_where_count`](take_where_count.das).

`take(n)`/`skip(n)` before an aggregate now wraps the pre-aggregate chain
into an inner subquery so the LIMIT applies pre-aggregate
(`SELECT COUNT(*) FROM (SELECT * FROM t WHERE P LIMIT n)`) — via
`maybe_wrap_take_before_aggregate` reusing `snapshot_q_to_subquery_wrap`.
`take_where_count` (take-before-where) was already faithful via the
existing take/where phase divert. All three have `m1` lanes.

---

## Computed-scalar `_select` — closed

`_select(_.a + _.b)` (and any expression `pred_to_sql` can render) now
lowers into a single computed projection slot, so `sum`/`min`/`max`/
`average`/`count` over a computed value work.

## Computed-cast projection — closed

A computed projection containing a workhorse type cast
(`int64(_.a) * int64(_.b)`, `double(_.x) / 2`, `string(_.id)`) used to
reject with `_sql: expression over a column is not translatable to SQL:
int64(_.price)`. (The earlier note here blamed linq's `_select`
inference — that was wrong: the in-memory `_fold` lanes handled the cast
fine; the gap was purely that `fn_call_to_sql` had no entry for the cast
functions.) Now the workhorse casts lower to SQLite
`CAST(x AS INTEGER/REAL/TEXT)`, and — critically — the daslang cast drives
the **result read-back type**, so `int64(_.price) * int64(_.year) |> sum()`
reads the 64-bit `SUM` back faithfully (without the cast the int
projection truncates at n=100k, ~1e11 ≫ int32). This lit the
`zip_dot_product` / `zip_dot_product_3arg` SQL `m1` lanes.

---

## By design / deferred — no clean SQL form

### `zip` family — positional, not relational

Benches: [`zip_count_pred`](zip_count_pred.das) (degenerate `m1` added),
[`zip_dot_product`](zip_dot_product.das),
[`zip_dot_product_3arg`](zip_dot_product_3arg.das),
[`zip_reverse_to_array`](zip_reverse_to_array.das).

`zip(a, b)` pairs elements by position across two unrelated sequences;
SQL has no positional pairing (joins are key-based). The decs/SQL lanes
instead measure the degenerate **same-row** interpretation (two columns
of one row). `zip_count_pred` gets an `m1` on that basis (count of
`price*year > T`, product pushed into `_where` — the per-row product fits
int32). The dot-product **sum** lanes (`zip_dot_product` /
`zip_dot_product_3arg`) are now lit too, via the int64-cast projection
(see "Computed-cast projection" above).
`zip_reverse_to_array` stays `—`: `reverse()` has no SQL order key.

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

## Decs lane (now lit)

### `reverse_distinct_by` decs

Bench: [`reverse_distinct_by`](reverse_distinct_by.das).

The Theme 8 2a splice arm (R-2a, backward index walk `arr[length-1-k]`) requires
`array_source` — forward-only archetype walks have no random-index analog. **Closed
2026-05-31** by the complementary R-2b row (`emit_reverse_distinct_forward_keeplast`,
gated `non_array_source`): a single forward pass that OVERWRITES `table<key; (seq, val)>`
per element (keep-last) and sorts survivors by descending seq — no back-walk, no temp
reverse buffer. It's source-generic (one emit covers decs / XML / iterators), so the
decs `m4` lane is now a real number (≈ the array fast path), not `—`.
