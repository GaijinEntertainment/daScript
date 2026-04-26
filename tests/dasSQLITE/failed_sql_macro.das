options gen2

// Each `_sql` / `_sql_text` call below is intentionally malformed and must
// produce a macro_error (40104) from the analyzer. Under Mode 2
// (canVisitArgument=true, inner call_macros expand before outer fires),
// some malformed chains additionally cascade real type errors before the
// analyzer ever runs — e.g. `_select(_.Name) |> _select(_.Price)` is a
// genuine type bug (string has no `Price` field) and `some_unknown_func(_)`
// hits the unresolved function. Those cascades are expected.
expect 40104:13, 30304:2, 30503:3

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
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

    // 5. _sql with _select projection that's not a column ref (chunk 2 only supports _.Field)
    let bad5 <- _sql(db |> select_from(type<Car>) |> _select(_.Price * 2))

    // 6. _sql with _where containing an unsupported expression (function call inside)
    let bad6 <- _sql(db |> select_from(type<Car>) |> _where(some_unknown_func(_.Id)))

    // 7. _sql_text with non-chain
    let bad7 = _sql_text(42)

    // 8. _to_array() buried under _where — must be outermost terminal
    let bad8 <- _sql(db |> select_from(type<Car>) |> _to_array() |> _where(_.Id == 1))

    // 9. _to_array() after _select — must be outermost terminal
    let bad9 <- _sql(db |> select_from(type<Car>) |> _select(_.Name) |> _to_array() |> _where(_.Id == 1))

    // 10. _having without _group_by — HAVING is meaningless without aggregation
    let bad10 <- _sql(db |> select_from(type<Car>) |> _having(_._1 |> length > 1))

    // 11. _select after _group_by must be a NAMED tuple — bare scalar rejected
    let bad11 <- _sql(db |> select_from(type<Car>) |> _group_by(_.Name) |> _select(_._0))

    // 12. take(n) before an aggregate is degenerate in SQL — `LIMIT` after
    //     an aggregate is a no-op (aggregate already returns one row).
    //     User almost certainly wants the subquery form
    //     `SELECT SUM(c) FROM (SELECT p AS c FROM Cars LIMIT n)`, which lands
    //     with joins/subqueries in chunk 5+. Reject loud now.
    let bad12 <- _sql(db |> select_from(type<Car>) |> take(2) |> _select(_.Price) |> sum())

    // 13. distinct() before an aggregate is degenerate — `SELECT DISTINCT
    //     SUM(...)` / `SELECT DISTINCT COUNT(*)` dedupes a single-row result.
    //     The meaningful form is `COUNT(DISTINCT col)` (DISTINCT inside the
    //     aggregate), which is not yet wired.
    let bad13 <- _sql(db |> select_from(type<Car>) |> distinct() |> count())
}
