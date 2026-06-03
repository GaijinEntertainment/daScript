options gen2

// Each `_sql` / `_sql_text` call below is intentionally malformed and must
// produce a macro_error (40104) from the analyzer. Under Mode 2
// (canVisitArgument=true, inner call_macros expand before outer fires),
// some malformed chains additionally cascade real type errors before the
// analyzer ever runs — e.g. `_select(_.Name) |> _select(_.Price)` is a
// genuine type bug (string has no `Price` field). Those cascades are expected.
expect 30341, 30928:2, 50503:24

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq
require daslib/fts5_query   // for the bad22 case

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

[sql_table(name="Owners")]
struct Owner {
    @sql_primary_key Id : int
    CarId : int
    Name  : string
}

[export]
def main() {
    var db = SqlRunner()

    // 1. _sql(non-chain) — bare integer is not a chain
    let bad1 <- _sql(42)

    // 2. _sql(chain rooted at wrong fn) — chain root must be select_from
    let bad2 <- _sql(db |> insert(Car(Id = 1, Name = "x", Price = 0)))

    // 3. _sql with duplicate distinct() — analyzer enforces single-stage guard
    let bad3 <- _sql(db |> select_from(type<Car>) |> distinct() |> distinct())

    // 4. _sql with double _select
    let bad4 <- _sql(db |> select_from(type<Car>) |> _select(_.Name) |> _select(_.Price))

    // (bad5 removed: a computed scalar `_select(_.Price * 2)` now lowers to `SELECT ("Price") * ?`
    //  via pred_to_sql + push_computed_proj_slot. Coverage moved to test_99_computed_select_subquery.das.)

    // 6. Expressions over a column that pred_to_sql can't translate reject cleanly (rather than binding
    //    `?` and failing at bind time with `can't locate variable '_'`): bitwise XOR has no SQLite
    //    operator. (A captured runtime value with no column reference still binds — see test_99.)
    // (bad6b removed: `_where(double(_.Price) > 1.0)` now lowers to `CAST("Price" AS REAL) > ?` via the
    //  workhorse-cast rendering in fn_call_to_sql. Coverage moved to test_99_computed_select_subquery.das.)
    let bad6a <- _sql(db |> select_from(type<Car>) |> _select((V = _.Id ^ 1)))

    // 7. _sql_text with non-chain
    let bad7 = _sql_text(42)

    // _to_array() ordering is unchecked — it's a materializer-mode flag that
    // doesn't change emitted SQL, so any chain position is valid (see
    // sqlite_linq.das comment by the _to_array peel).

    // 10. _having without _group_by — HAVING is meaningless without aggregation
    let bad10 <- _sql(db |> select_from(type<Car>) |> _having(_._1 |> length > 1))

    // 11. _select after _group_by must be a NAMED tuple — bare scalar rejected
    let bad11 <- _sql(db |> select_from(type<Car>) |> _group_by(_.Name) |> _select(_._0))

    // (bad12 removed: `take(2) |> _select(_.Price) |> sum()` now lowers
    // correctly via multi-Q to `SELECT SUM("Price") FROM (SELECT "Price" FROM "Cars" LIMIT ?) AS "t0"`.
    // Coverage moved to test_33_take_skip.das as `test_take_then_select_then_aggregate_runtime`.)

    // 13. distinct() before an aggregate is degenerate — `SELECT DISTINCT
    //     SUM(...)` / `SELECT DISTINCT COUNT(*)` dedupes a single-row result.
    //     The meaningful form is `COUNT(DISTINCT col)` (DISTINCT inside the
    //     aggregate), which is not yet wired.
    let bad13 <- _sql(db |> select_from(type<Car>) |> distinct() |> count())

    // 14. Theta join (non-`==` predicate in `on`). The locked equi-join
    //     constraint requires `(l, r) => l.X == r.Y`; theta joins force
    //     O(n*m) nested-loop and would lose SQLite's planner hint —
    //     macro_error redirects users to raw SQL.
    let bad14 <- _sql(db |> select_from(type<Car>)
                        |> _join(db |> select_from(type<Owner>),
                                 $(c : Car, o : Owner) => c.Id < o.CarId,
                                 $(c : Car, o : Owner) => (Name = c.Name, Owner = o.Name)))

    // 15. Multi-key equi join (`&&`-chained equality in `on`). Backing
    //     `join` fn keys are single-column; multi-key would need a
    //     tuple-key extension — deferred. Reject loud.
    let bad15 <- _sql(db |> select_from(type<Car>)
                        |> _join(db |> select_from(type<Owner>),
                                 $(c : Car, o : Owner) => c.Id == o.CarId && c.Name == o.Name,
                                 $(c : Car, o : Owner) => (Name = c.Name, Owner = o.Name)))

    // 16. Two joins in one chain (3+ table). Chunk 5 ships single-join
    //     only — the `q.joins` slot is one-deep. The second `_join`'s
    //     `on` must be a valid single-column equi-join (`==`), or
    //     linq_boost rejects it before sqlite_linq's multi-join guard
    //     fires; here we equi-join on `.Name` (cosmetically meaningless
    //     but well-formed) so the diagnostic comes from sqlite_linq.
    let bad16 <- _sql(db |> select_from(type<Car>)
                        |> _join(db |> select_from(type<Owner>),
                                 $(c : Car, o : Owner) => c.Id == o.CarId,
                                 $(c : Car, o : Owner) => o)
                        |> _join(db |> select_from(type<Owner>),
                                 $(t : Owner, o : Owner) => t.Id == o.Id,
                                 $(t : Owner, o : Owner) => o))

    // 17. Set-op nested inside a set-op — chunk 5 restricts to one set-op
    //     per chain (chained set-ops would need parenthesization
    //     bookkeeping not yet wired).
    let bad17 <- _sql((db |> select_from(type<Car>) |> _select(_.Price))
                       |> union((db |> select_from(type<Car>) |> _select(_.Price))
                                |> union(db |> select_from(type<Car>) |> _select(_.Price))))

    // 18. Set-op LHS with `_order_by` — emitted SQL is unparenthesized
    //     `<lhs> UNION <rhs>`, so a per-side ORDER BY is a SQLite syntax
    //     error. Reject at compile time; chunk 5 doesn't yet support a
    //     trailing ORDER BY on the combined set-op result.
    let bad18 <- _sql((db |> select_from(type<Car>) |> _select(_.Price) |> _order_by(_))
                       |> union(db |> select_from(type<Car>) |> _select(_.Price)))

    // 19. Set-op RHS with `take(n)` — same reason: per-side LIMIT is not
    //     valid in unparenthesized compound query.
    let bad19 <- _sql((db |> select_from(type<Car>) |> _select(_.Price))
                       |> union(db |> select_from(type<Car>) |> _select(_.Price) |> take(5)))

    // 20. `_.Col == none()` — fixit error redirects to is_none(). The
    //     locked decision (chunk-4 D4) is "explicit-over-magic": the
    //     compiler refuses to translate the equality and points at the
    //     correct spelling. See pred_to_sql `==` handler.
    var nullable_owner = Owner(Id = 0, CarId = 0, Name = "")
    let bad20 <- _sql(db |> select_from(type<Owner>) |> _where(_.CarId == none(type<int>)))

    // 21. `_.Col != none()` — same fixit; the inverse is `is_some()`.
    let bad21 <- _sql(db |> select_from(type<Owner>) |> _where(_.CarId != none(type<int>)))

    // 22. `text_match` on a non-FTS5 struct. The translator refuses to
    //     emit MATCH against a regular [sql_table] column — SQLite would
    //     accept the syntax but match on nothing useful. The fixit
    //     suggests `contains` (LIKE-based) or `[sql_fts5]`.
    let bad22 <- _sql(db |> select_from(type<Car>) |> _where(_.Name |> text_match("foo")))

    // 23. `_distinct_by(<computed expr>)` before count() — only `<arg>.<field>`
    //     keys lower to `COUNT(DISTINCT "col")`; computed keys like `_.Price %
    //     100` would need a general expression-to-SQL emitter not yet wired.
    let bad23 <- _sql(db |> select_from(type<Car>) |> _distinct_by(_.Price % 100) |> count())

    // 24. `_distinct_by(<outer-capture>.Field)` — the key reads a captured
    //     variable instead of the lambda's bound parameter (`_`); without
    //     the arg-name check this would silently emit `COUNT(DISTINCT "Field")`
    //     against the SQL row source — wrong result.
    var capturedRow = Car(Id = 0, Name = "captured", Price = 0)
    let bad24 <- _sql(db |> select_from(type<Car>) |> _distinct_by(capturedRow.Name) |> count())

    // 25. `_group_by(_.Col - <runtime-var>)` — expression-key support inlines
    //     constants only; a runtime-var bind would need to be re-pushed at each
    //     re-use of the key fragment (SELECT, GROUP BY, ORDER BY positions).
    let runtimeOffset = 5
    let bad25 <- _sql(db |> select_from(type<Car>) |> _group_by(_.Price - runtimeOffset) |> _select((K = _._0, N = _._1 |> length)))

    // 26. `string(_.Col, true)` — the hex-formatting overload. The decimal form lowers to `CAST(... AS TEXT)`,
    //     but SQLite CAST always formats decimal, so the hex form rejects rather than silently mistranslating.
    let bad26 <- _sql(db |> select_from(type<Car>) |> _select(string(_.Id, true)))
    // (Note: the "_in subquery without _select(_.Col)" diagnostic added in
    //  this round is also belt-and-suspenders — a daslang `contains` type-
    //  check rejects mismatched element types before sqlite_linq even sees
    //  the chain in most realistic shapes. We keep the macro_error path
    //  live for the rare case where the row-vs-rowset shape type-checks
    //  but produces a multi-column subquery; constructing that test
    //  reliably is left for a future chunk.)
}
