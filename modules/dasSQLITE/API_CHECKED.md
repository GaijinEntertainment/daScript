# dasSQLITE API Checked — Equivalence-Harness Plan

Companion to `API_REWORK.md` (master design doc) and `API_MISSING.md`
(strawman workshop). This file tracks a *verification* workstream: an
explicit, code-driven audit that the public design promise holds.

## The promise being audited

The dasSQLITE `_sql(...)` macro is, by design, a **pushdown optimizer
on top of plain linq**. As of the multi-Q lowering work (F1
resolution), this promise is unconditional for the shipped chain
shapes — `_sql` honors linq's positional chain semantics, lowering
divergent orders to nested SELECTs (`SELECT ... FROM (subquery) AS t0
...`) instead of silently reordering into canonical SQL eval order.
Same chain → same answer in all three modes.

The same chain shape:

```das
src |> _where(_.Price > 100)
    |> _group_by(_.City)
    |> _select((City=_._0, N=_._1 |> length, Total=_._1 |> select($(u:T) => u.Salary) |> sum))
```

…is meaningful in three execution modes:

1. **Inside `_sql(...)`** with a SQL source (`db |> select_from(type<T>)`).
   The macro pattern-matches the chain and emits a single
   `SELECT … FROM … WHERE … GROUP BY …` query — pushdown.
2. **Without `_sql(...)`** with a SQL source. `select_from` materializes
   the full table into `array<T>`; the same chain operators run as plain
   `linq_boost` macros over that array — client-side aggregation, same
   answer.
3. **Without `_sql(...)`** with a plain container source (`array<T>`,
   table, generator). Pure in-memory linq, identical operators, same
   answer.

Mode 1 and Mode 2 must agree numerically (this is what makes `_sql` a
safe optimizer). Mode 2 and Mode 3 must agree (this is what makes the
operator vocabulary genuinely unified, not a SQL-specific dialect).

This invariant is the load-bearing claim behind every tutorial that
shows a `_sql(...)` chain. Today it is asserted by design and by ad-hoc
testing, not by mechanical coverage.

## In scope vs out of scope

**In scope:** linq chain operators — anything that participates in a
`src |> op1 |> op2 |> ... |> terminal` chain and is meaningful both
inside and outside `_sql(...)`. `_where`, `_select`, `_group_by`,
`_having`, `_order_by`, `_join`, `_left_join`, `_first`, `_first_opt`,
`_count`, `take`, `skip`, `distinct`, set operations (`union`/`intersect`/`except`),
subqueries (`_in`/`_not_in`/`_any`/`_none`), aggregates (`length`/`sum`/`average`/`min`/`max`),
`text_match` (FTS5), `_to_array`. The parity claim is that these
operators have a single semantic, regardless of source.

**Out of scope:** raw-SQL escape-hatch helpers — `query_one`,
`query_scalar`, `exec`, `try_exec`, and their `try_`/`_opt` variants.
These are not linq chains — they take a literal SQL string + bind
arguments and return a row or scalar. They have no Mode-2 (no chain
to run "without `_sql`") and no Mode-3 (no plain-array analog of "run
this SQL string"). The parity story does not apply.

Also out of scope: `_sql_text(...)` (SQL-text debug emitter, not a
runtime operator) and DDL helpers like `create_table` / `_create_view`.

## What the harness should produce

For every tutorial example (and ideally every test) that contains a
`_sql(...)` chain over a SQL source:

- **Mode-1 vs Mode-2 equivalence.** Run the same chain `with`/`without`
  the surrounding `_sql(...)` macro and assert the materialized result is
  equal. Failures here mean either the macro is mis-translating or the
  in-memory linq path has a divergent semantic.
- **Mode-2 vs Mode-3 equivalence.** Construct the same logical dataset
  as a plain `array<T>` (no DB), run the chain, and assert the result
  matches the SQL-source-without-`_sql` run. Failures here mean an
  operator behaves differently against a SQL-backed array than a plain
  array — a leak of provider knowledge into ostensibly generic operators.
- **Negative coverage — operator unsupported on container.** If a chain
  uses an operator that intentionally only makes sense inside `_sql`
  (none planned today, but the door is open — e.g. a `_raw_sql_fragment`
  escape hatch), the Mode-3 run must produce a *clear* compile-time
  error, not a silent wrong answer or a runtime panic deep inside linq.
- **Negative coverage — operator unsupported by `_sql`.** Conversely, if
  a tutorial constructs a chain using linq operators the `_sql` macro
  cannot translate, wrapping it in `_sql(...)` must produce a clear
  `macro_error` pointing at the offending node. (This direction is
  already mostly covered — every `_sql` rejection is a `macro_error`
  today; the harness just needs to assert the rejection actually fires
  for the listed cases instead of silently falling through.)

## Scope

Every tutorial under `tutorials/sql/` whose body contains `_sql(...)` —
roughly tutorials 06 onward. The chains-without-`_sql` story isn't
documented in any tutorial today (it's stated implicitly via the
`_sql_text` debug macro), and the harness is what would let a future
tutorial point at "and here, same chain, no macro, plain array" with
confidence.

## Harness shape (sketch)

The harness is not a feature; it's an `[export]`-test pattern. Each
case has three runs and two equality assertions:

```das
// fixture: same dataset, two sources
db |> create_table(type<User>) // populated from `users_seed`
let users <- users_seed         // plain in-memory copy

// the chain — written ONCE, used three times
def make_chain(src) {
    return src |> _where(_.Price > 100)
                |> _group_by(_.City)
                |> _select((City=_._0, N=_._1 |> length))
}

// runs
let r_sql_pushdown   <- _sql(make_chain(db |> select_from(type<User>)))
let r_sql_in_memory  <- make_chain(db |> select_from(type<User>))
let r_plain_array    <- make_chain(users)

// assertions
assert(r_sql_pushdown   == r_sql_in_memory)
assert(r_sql_in_memory  == r_plain_array)
```

Open questions before this can be a real harness, not a sketch:

- Can the chain literally be factored into one `make_chain` helper, or
  does the macro require the chain to be syntactically present at the
  `_sql(...)` call site? (Suspect the latter; if so, the harness has
  to repeat the chain three times — manageable, but means the harness
  needs `dastest`-style equality fixtures, not a one-shot helper.)
- Lambda bodies inside the chain (`$(u:User) => u.X`) are types that
  cross between the macro and the linq runtime — does an
  `_sql(make_chain(...))` form fail to recognize the chain because the
  pattern matcher looks for the literal AST shape rather than expansion?
  Likely yes; this dictates harness ergonomics.
- Negative-coverage assertions need a way to assert "this file fails to
  compile with this specific `macro_error`." `dastest` has a pattern
  for this (compile-fail tests with expected error excerpt) — confirm
  applicability and standardize before rolling the harness out.

## Coverage gaps

Tutorials currently teaching a thin slice of what the API surface
should support. Tracked separately from findings — these are
not-yet-shipped capabilities, not divergences in shipped behavior.

### G1 — Tutorial 16 covers only INNER + LEFT joins

Missing: `_right_join`, `_full_outer_join`, `_cross_join` — none
implemented in `daslib/linq_boost` or `modules/dasSQLITE/daslib/sqlite_linq`.
Tutorial 16's note dismisses them as "not shipped" / "raw SQL". Issue
[#2558](https://github.com/GaijinEntertainment/daScript/issues/2558)
tracks the work: add all three as first-class chain operators in linq,
extend `_sql` emission, expand the tutorial (.das + .rst), extend the
parity test. Surfaced 2026-05-02; Boris flagged as "huge gap."

## Findings

Findings accumulate here as the harness walks the tutorials. Each entry
names the operator pattern, the modes that disagree, what each mode
produces, and the size of the impact (silent miscompute vs. clearly
divergent shapes).

**Resolution rule (Boris, 2026-05-02):** *we never just document
incorrect behaviour. should work or fail to compile.* Every finding
ends in either (a) make-it-work code change, or (c) reject-the-shape
`macro_error` that points at a canonical equivalent. (b) "work via
different emission" is allowed when (a) and (c) are both worse, but
"document and move on" is not a resolution. Tutorials demonstrating a
rejected form must be updated to the canonical form in the same PR.

### F1 — `take(n) |> skip(m)` interpreted differently in `_sql` vs linq — RESOLVED

Tutorial 11. The same chain produced different row counts across modes:

| Mode | Chain                                              | Old lowering              | Rows |
|------|----------------------------------------------------|---------------------------|------|
| 1    | `_sql(... \|> take(n) \|> skip(m))`                | `LIMIT n OFFSET m`        | n    |
| 2    | `... \|> take(n) \|> skip(m)` (no `_sql`, SQL src) | linq positional, in order | max(0, n − m) |
| 3    | `arr \|> take(n) \|> skip(m)`                      | linq positional, in order | max(0, n − m) |

`_sql` collapsed any combination of `take` and `skip` into one
`LIMIT/OFFSET` pair (skip-then-take semantics) regardless of chain order.
Plain linq honored chain order: `take(n)` first narrowed to n rows, then
`skip(m)` dropped m of those. With the tutorial's fixture
(`take(2) |> skip(1)`, 5-row table), Mode 1 returned 2 rows and Modes 2/3
returned 1 row.

**Decision (Boris, 2026-05-02):** initial plan was **(c)** — reject the
divergent shape with a `macro_error` and a fixit. After discussion, pivoted
to **(a) + (b) by lowering**: the `_sql` analyzer was rewritten to honor
linq's positional chain semantics across the board. Same chain → same
answer in all three modes. Matches the EF Core mental model
(`project_dassqlite_ef_only.md`). No fixit, no minefield.

**Fix:** multi-Q lowering in `daslib/sqlite_linq.das`. When a chain op's
SQL eval phase exceeds `q.minPhaseSeen` (e.g. `take` (phase 8) appearing
inside a `skip` (phase 7) or `_where` (phase 2)), the inner subtree is
analyzed into a fresh `SqlQuery` and snapshotted to SQL string + bind list
on the outer Q's `innerSql` / `innerBindExprs` fields. The outer SELECT's
FROM clause renders as `(<innerSql>) AS "t0"`. Bind collection
recurses inner-first via `collect_query_binds`. Phase divert checks live
in 5 peel blocks (TAKE / SKIP / ORDER_BY / ORDER_BY_DESC / DISTINCT — the
ops where chain-order divergence is real with default-row inner). WHERE
stays commutative (no divert). Set-ops / joins are unchanged for v1.

**Side-effects:** `take(n) |> _where(p)`, `take(n) |> _order_by(k)`,
`take(n) |> _order_by(k) |> take(m)` (multi-level), and
`skip(n) |> _where(p)` shapes that previously silently miscomputed in
`_sql` now lower to subqueries with the linq-correct semantics.

**Tutorial 11 (`tutorials/sql/11-take_skip.das` + `sql_11_take_skip.rst`):**
canonical pagination shape promoted to `skip(m) |> take(n)` (flat fast
SQL). The reverse `take(n) |> skip(m)` is shown as the "honest, slower
subquery" form, with the row-count change explained.

**Coverage** (`tests/dasSQLITE/`):
- `parity_check_11_take_skip.das` — divergence pin removed; replaced
  with three positive parity cases (`take|skip` 1-row, `n>m` 2-rows,
  degenerate `m>n` 0-rows).
- `parity_check_11b_take_then_where.das` — 5 cases covering basic
  outer-where, two-where AND-fold, inner+outer where coexistence,
  `_to_array` terminal, `_first_opt` terminal.
- `parity_check_11c_take_then_order.das` — 3 cases: take|order ASC,
  take|order DESC, three-level nesting.
- `parity_check_11d_skip_then_where.das` — 2 cases: skip|where, plus
  skip|where|order_desc.

**v2.1 RESOLVED:** All multi-Q lowering capabilities below now ship; v2.1 closes
the outer-WHERE-on-projected-alias gap.

**v2 capabilities — shipped:**
- **Aggregate-then-filter** — `_select` peel diverts when outer chain pinned
  WHERE (`q.minPhaseSeen <= PHASE_WHERE`). The inner Q (with `_group_by` +
  named/grouped projection) is snapshotted via `build_sql_string(q, true)`
  which forces `AS "<alias>"` on every projected column; outer Q references
  those aliases via `pred_to_sql`'s new `q.fromRowType` consult path. Outer
  Q's `selectColSqlFragments` are populated with `"<alias>"` so the outer
  SELECT clause emits `SELECT "alias", ... FROM (<innerSql>) AS "t0" WHERE …`.
  DISTINCT / TAKE / SKIP / ORDER_BY / set-op don't trigger the divert
  (they apply to projected output in both SQL and linq positional — same
  semantics, no wrap needed).
- **Multi-join (`_join(_join(A, B, …), C, …, into)`)** — the outer
  `_join`'s `process_join_call` recurses into srca first; if the inner
  recursion produced a join (`q.seenJoin == true` on return),
  `snapshot_q_to_subquery_wrap` snapshots q (carrying the inner _join's
  state) into an inner SQL string + bind list, then resets q so the outer
  can install a fresh `t0`/`t1` JoinSpec on top of the wrap. `build_sql_select`
  emits `FROM (<innerSql>) AS "t0" INNER JOIN "Ctab" AS "t1" ON …`. The outer
  `into` projection's `<lhsArg>.<alias>` resolves through
  `source_rootType_for_idx`'s new fromRowType fallback +
  `lookup_struct_field_type`'s new tuple-handling.

**v2.1 capability — shipped:**
- **Outer WHERE on a JOIN's projected alias**
  (`_join(...) |> _where(_.alias)` and `_join(_join(A,B,...), C, ...) |> _where(_.alias)`).
  The WHERE peel now snapshots the analyzed q in place via
  `snapshot_q_to_subquery_wrap(q, prog, at, fillPassthrough = true)` when
  `q.seenJoin && q.proj == NamedTuple`. Discriminator skips the
  `_select |> _where` path (`q.seenJoin` is false there — the SELECT peel
  already wrapped). Using the in-place snapshot helper instead of
  `divert_to_inner` avoids the linq Mode-2/3 typer cascade that re-analysis
  triggers; the new `fillPassthrough` mode emits passthrough column
  fragments from the synthesized `fromRowType` so the outer WHERE renders as
  `... FROM (joinSql) AS "t0" WHERE "alias" = …`. Coverage:
  `parity_check_15_join.das parity_join_with_outer_where`,
  `parity_check_15b_multi_join.das parity_three_table_join_with_outer_where[_int]`.

### F2 — `average(array<int>)` truncates in linq, promotes to double in `_sql` — RESOLVED

Tutorial 13. Same chain produced different *types and values*:

| Mode | Chain                                                        | Returns           | Value (Price=[100,200,300,50,999]) |
|------|--------------------------------------------------------------|-------------------|-------------------------------------|
| 1    | `_sql(... \|> _select(_.Price) \|> average())`               | `double`          | `329.8` (SQL `AVG` promotes)        |
| 2    | `... \|> _select(_.Price) \|> average()` (no `_sql`, SQL src) | `int` (input TT) | `329` (integer division)            |
| 3    | `arr \|> _select(_.Price) \|> average()`                     | `int`             | `329` (integer division)            |

Worse than F1: not only did the chain mean different things across
modes, the two results didn't even typecheck (`equal(double, int)`
errors at the `==` site).

The defect was on the linq side. The old `daslib/linq.das`:
```das
def average(src : array<auto(TT)>) : TT -const -& {
```
returned the input element type. For `array<int>` that was `int`, so
`(sum / count : int / int)` truncated. SQL's `AVG` always promotes to
floating point; C# LINQ's `Average(IEnumerable<int>)` returns `double`
for the same reason. daslang's linq was the outlier.

**Fix:** `daslib/linq.das` `average(iterator<TT>)` and `average(array<TT>)`
both unconditionally return `double` and accumulate in `double` from
element zero (uniform path for every numeric `TT`, no integer overflow
hazard up to ~2⁵³, more precise than naive `float` sum). The private
`average_impl` / `average_impl_const` helpers were removed — the simple
loop is the implementation. The `min_max_average` family is unchanged
(still TT-preserving).

**Caller-side fallout:** the deliberate "complex average" test case in
`tests/linq/test_linq_aggregation.das` (averaging an `array<ComplexType>`
with custom `+=` and `/= uint64` operators) was deleted; users with
custom non-numeric types must roll their own averager. All other linq
tests had assertion-type updates (`equal(query, 2)` → `equal(query, 2.0lf)`).
The Mode-1-vs-Mode-2-vs-Mode-3 parity assertion `M1 ≡ M2 ≡ M3` now
holds against `double 329.8` everywhere. `parity_check_13_aggregates.das`
flipped from divergence-pin to equality assertion.

### F4 — `_in` / `_not_in` / `_any` / `_none` shadow `_` inside `_where` — RESOLVED

Tutorial 17. The two `linq_boost` predicate base classes
(`AstCallMacro_LinqPred2` / `AstCallMacro_LinqPredII2`) synthesize a
`$(_ : T) => …` lambda for the predicate body. Whenever such a chain
nested inside another `_`-binding macro (canonical case:
`arr |> _where(_.Id._in(other |> _select(_.UserId)))`), the inner
lambda's `_` shadowed the outer's `_` and the typer rejected the
chain at `linq_boost.das:45` — far from the user's call site.

Fix: set `VariableFlags.can_shadow` on the synthesized `_`
parameter. The flag was already used elsewhere in `linq_boost.das`
for macro-generated bindings; the predicate path was missing it.
Two-line patch in both base classes' `visit` overrides.

`parity_check_17_subqueries.das` covers all four operators across
the three modes; the previously-pinned `failed_parity_check_17_…`
file has been removed.

The 1-arg `_any()` / `_none()` form (test if the subquery is
non-empty, lifts to plain `EXISTS (...)` / `NOT EXISTS (...)` under
`_sql`) remains Mode-1-only — `LinqAny` / `LinqNone` inherit
`AstCallMacro_LinqPred2` which `macro_verify`s on a 2-arg shape, so
the zero-predicate form errors with `expecting any(iterator,
predicate)` outside `_sql`. In plain linq the equivalent is
`length(...) > 0` or the function-form `any(...)`; this is a
different surface, not a parity divergence. Out of scope here.

### F3 — `_having |> _order_by` after `_group_by` failed to compile in plain linq — RESOLVED

Tutorial 14. Chains like:

```das
arr |> _group_by(_.City) |> _having(_._1 |> length >= 1) |> _order_by(_._0) |> _select(...)
```

Mode 1 (`_sql`) emitted `SELECT ... GROUP BY "City" HAVING ... ORDER BY "City"` and ran.
Modes 2/3 used to fail to compile with a deep typer error:

```
error[30304]: no matching functions or generics:
  _::clone(tuple<string;array<User>> aka TT&, tuple<string;array<User>> aka TT const&)
  d:/Work/daScript/daslib/builtin.das:999:15
  ...
  instanced from linq`order_by` ... at d:/Work/daScript/daslib/linq.das:413:8
```

The trigger was the **combination** `_having |> _order_by`. Without
`_having`, linq's fusion of `_group_by + _order_by + _select(...)`
followed a path that didn't materialize the grouped array for sorting;
once `_having` was in the chain, post-aggregate filtering forced a real
`array<tuple<K, array<T>>>` to be produced, then sorted. linq's
`order_by` called `clone_to_move` on the input
([daslib/linq.das:413](../../daslib/linq.das#L413)), which needed a
generic `_::clone` for the per-element type — and the synthesized clone
for `tuple<string, array<User>>` was emitted with the wrong constness,
so resolution failed inside `builtin.das`, far from the user's chain.

**Initial decision (Boris, 2026-05-02):** **(b)**. Add a `static_if`
guard in linq's `order_by` to surface a clear error pointing at the
user's `_order_by` call site rather than `builtin.das:999`.

**Actual resolution (2026-05-03):** **(a)** — fixed at the compiler
level by [PR #2563](https://github.com/GaijinEntertainment/daScript/pull/2563)
("synthesized tuple/variant clone constness"). `makeCloneTuple` /
`makeCloneVariant` now keep `explicitConst=true`; the lowering site
picks the flavor-per-constness, so `_::clone(tuple<K, array<T>>, ...)`
resolves cleanly. linq's `clone_to_move` no longer hits the deep error;
no `static_if` guard is needed.

**Coverage** (`tests/dasSQLITE/`):
- [parity_check_14_order_after_group.das](../../tests/dasSQLITE/parity_check_14_order_after_group.das) —
  flipped from `expect 30304:1` divergence pin to a positive 3-mode
  parity test on the exact `_having |> _order_by` chain.
- [parity_check_14_group_by.das](../../tests/dasSQLITE/parity_check_14_group_by.das) —
  `sort_by_city` post-sort workaround removed; 5 of 6 tests now use
  inline `|> _order_by(_._0)`. The multi-key test stays on post-sort
  (with a local `sort_by_city_age` helper) because the `_sql` analyzer's
  tuple-key validator only accepts `_.Field` / string expressions, not
  nested `_._0._N` walks — separate from F3.

## Walkthrough — complete (2026-05-02)

Every tutorial under `tutorials/sql/01..41` has been audited. Tests live
under `tests/dasSQLITE/parity_check_<NN>_*.das`. v2/v2.1 multi-Q parity
coverage (`14b`, `15b`, `11b`/`c`/`d`) added 2026-05-03; F3 pin flipped
to positive parity in the same sweep.

### Tutorials with parity tests

| Tutorial | File | Coverage |
|----------|------|----------|
| 04 select_all | parity_check_04_select_all.das | full row, _where, _select scalar |
| 05 parametrized | parity_check_05_parametrized.das | _first, _first_opt miss/hit |
| 06 error_handling | parity_check_06_error_handling.das | _try_sql variants |
| 07 anatomy | parity_check_07_anatomy.das | symmetric _where, chained AND, starts_with |
| 08 where | parity_check_08_where.das | OR+string, ends_with, contains, to_lower, length, abs |
| 09 select | parity_check_09_select.das | named-tuple, renamed, named-tuple+_first |
| 10 order_by | parity_check_10_order_by.das | single ASC/DESC, tuple-key, where|order|take |
| 11 take_skip | parity_check_11_take_skip.das | F1 RESOLVED (multi-Q); + 3 take|skip equivalence cases |
| 11 take|where (multi-Q) | parity_check_11b_take_then_where.das | inner-take + outer-where, multi-where, inner+outer where, terminals |
| 11 take|order (multi-Q) | parity_check_11c_take_then_order.das | inner-take + outer-order ASC/DESC, three-level nesting |
| 11 skip|where (multi-Q) | parity_check_11d_skip_then_where.das | inner-skip + outer-where, outer-where + outer-order |
| 12 distinct | parity_check_12_distinct.das | full-row, single-col, where|select|distinct |
| 12b set_ops | parity_check_12b_set_ops.das | union, intersect, except |
| 13 aggregates | parity_check_13_aggregates.das | F2 RESOLVED — average promotes to double in all modes |
| 14 group_by | parity_check_14_group_by.das | _group_by, _having, _order_by, multi-key, full reporting chain |
| 14 having\|order_by | parity_check_14_order_after_group.das | F3 RESOLVED — inline `_order_by` after `_having` works in all modes |
| 14 group_by (multi-Q) | parity_check_14b_aggregate_then_filter.das | aggregate-then-filter (v2 multi-Q lowering) |
| 15 join | parity_check_15_join.das | inner join, left/right filter |
| 15 multi-join (multi-Q) | parity_check_15b_multi_join.das | three-table multi-join with outer WHERE (v2.1) |
| 16 left_join | parity_check_16_left_join.das | _left_join + Option<TB> projection |
| 17 subqueries | parity_check_17_subqueries.das | `_in`/`_not_in`/`_any(pred)`/`_none(pred)` |
| 18 null_handling | parity_check_18_null_handling.das | round-trip, is_some, is_none, unwrap_or |
| 23 foreign_keys | parity_check_23_foreign_keys.das | _where|count, _to_array |
| 24 indexes | parity_check_24_indexes.das | _where + _order_by_descending + _to_array (int64) |
| 26 custom_types | parity_check_26_custom_types.das | enum filter (Guid+DateTime+enum), Option<DateTime> |
| 28 json | parity_check_28_json.das | JSON path WHERE/SELECT/count, nested struct path |
| 32 sql_functions | parity_check_32_sql_functions.das | `[sql_function]` transparency |
| 35 streaming | parity_check_35_streaming.das | `_each_sql` ≡ `_sql` ≡ M2 ≡ M3 |
| 36 attach | parity_check_36_attach.das | `with_attached`, manual attach + `with_schema` |
| 40 fts5 | parity_check_40_fts5.das | `text_match` AND/prefix/phrase/OR/NEAR |

### Tutorials skipped (no linq chain shape)

| # | File | Why skipped |
|---|------|-------------|
| 01 | version.das | metadata, no chain |
| 02 | insert_data.das | write-side only |
| 03 | last_row_id.das | write-side only |
| 19 | update.das | write-side only |
| 20 | delete.das | write-side only |
| 21 | upsert.das | write-side only |
| 22 | transactions.das | transaction lifecycle, no chain |
| 25 | defaults_computed.das | DDL-only |
| 27 | blob.das | adapter rail, no new chain |
| 29 | column_names.das | `column_info` is compile-time, raw PRAGMA otherwise |
| 30 | list_tables.das | raw `query("SELECT … FROM sqlite_master")` |
| 31 | views.das | DDL chain inside `_create_view`; no plain-array analog |
| 33 | pragma.das | connection admin, no chain |
| 34 | backup_vacuum.das | DB maintenance, no chain |
| 37 | bulk_operations.das | write-side perf; only chain is `_where|count` (covered by tut 13) |
| 38 | concurrency.das | thread/lock patterns, no new chain |
| 41 | triggers.das | raw `exec("CREATE TRIGGER …")`, no chain |

### Findings summary

| ID | Tutorial | Decision | Status |
|----|----------|----------|--------|
| F1 | 11 take_skip | multi-Q lowering — `take \| skip` and latent siblings (`take \| where`, `take \| order`, `skip \| where`) all preserve linq positional semantics | resolved |
| F2 | 13 aggregates | (a) fix `daslib/linq.das` `average` → always `double` | resolved |
| F3 | 14 group_by | (a) compiler-side fix for synthesized tuple/variant clone constness | resolved (PR [#2563](https://github.com/GaijinEntertainment/daScript/pull/2563)) |
| F4 | 17 subqueries | set `can_shadow` on synthesized `_` in `AstCallMacro_LinqPred2`/`LinqPredII2` | resolved (issue [#2559](https://github.com/GaijinEntertainment/daScript/issues/2559)) |
| G1 | 16 left_join | add `_right_join` / `_full_outer_join` / `_cross_join` | issue [#2558](https://github.com/GaijinEntertainment/daScript/issues/2558) |
| (lang) | 14 group_by | strongly-named tuple support (recordNames in cache key) | resolved (PR [#2565](https://github.com/GaijinEntertainment/daScript/pull/2565), 2026-05-03) |

## Next steps

- F1, F2, F4 landed as separate PRs; F3 cleanup (this PR) flips the pin
  test to a positive parity test and removes the post-sort workaround
  in `parity_check_14_group_by.das`. The (lang) tuple-strict finding
  was resolved in PR [#2565](https://github.com/GaijinEntertainment/daScript/pull/2565).
- G1 ([#2558](https://github.com/GaijinEntertainment/daScript/issues/2558))
  remains independent: `_right_join` / `_full_outer_join` / `_cross_join`
  as first-class linq + `_sql` operators, plus tutorial 16 expansion.
