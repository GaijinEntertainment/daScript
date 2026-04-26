options gen2

// Each `_sql` / `_sql_text` call below is intentionally malformed and must
// produce a macro_error (40104) from the analyzer. Under Mode 2
// (canVisitArgument=true, inner call_macros expand before outer fires),
// some malformed chains additionally cascade real type errors before the
// analyzer ever runs — e.g. `_select(_.Name) |> _select(_.Price)` is a
// genuine type bug (string has no `Price` field) and `some_unknown_func(_)`
// hits the unresolved function. Those cascades are expected.
expect 40104:7, 30304:2, 30503:1

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

    // 3. _sql with unsupported chain operator (linq's _order_by, not in chunk 2)
    let bad3 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Id))

    // 4. _sql with double _select
    let bad4 <- _sql(db |> select_from(type<Car>) |> _select(_.Name) |> _select(_.Price))

    // 5. _sql with _select projection that's not a column ref (chunk 2 only supports _.Field)
    let bad5 <- _sql(db |> select_from(type<Car>) |> _select(_.Price * 2))

    // 6. _sql with _where containing an unsupported expression (function call inside)
    let bad6 <- _sql(db |> select_from(type<Car>) |> _where(some_unknown_func(_.Id)))

    // 7. _sql_text with non-chain
    let bad7 = _sql_text(42)
}
