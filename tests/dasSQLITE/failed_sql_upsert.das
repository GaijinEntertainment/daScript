options gen2

// Each `_sql_upsert` call below is intentionally malformed and must emit
// a macro_error (40104) from the analyzer.
expect 50503:10

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
    Price : int
    @sql_computed = "Price * 2"
    DoublePrice : int
}

[export]
def main() {
    var db = SqlRunner()

    // 1. Wrong arity — _sql_upsert needs (row, on_conflict, do_update)
    let bad1 = db |> _sql_upsert(Car(Id = 1, Name = "x", Price = 0, DoublePrice = 0), _.Id)

    // 2. Second arg not a struct value (bare int)
    let bad2 = db |> _sql_upsert(42, _.Id, (Name = "x"))

    // 3. Bulk array<T> form is deferred — pass a single row, not an array
    var rows : array<Car>
    rows |> emplace(Car(Id = 1, Name = "x", Price = 0, DoublePrice = 0))
    let bad3 = db |> _sql_upsert(rows, _.Id, (Name = "x"))

    // 4. on_conflict is not `_.Col` — bare integer literal
    let bad4 = db |> _sql_upsert(Car(Id = 1, Name = "x", Price = 0, DoublePrice = 0),
                                  42, (Name = "x"))

    // 5. on_conflict tuple element is not `_.Col`
    let bad5 = db |> _sql_upsert(Car(Id = 1, Name = "x", Price = 0, DoublePrice = 0),
                                  tuple(_.Id, 42), (Name = "x"))

    // (Empty `tuple()` is a parse error, so the on_conflict-empty-tuple
    // validation rule is unreachable through normal source. Skipped.)

    // 7. on_conflict references column not on the struct
    let bad7 = db |> _sql_upsert(Car(Id = 1, Name = "x", Price = 0, DoublePrice = 0),
                                  _.NotAColumn, (Name = "x"))

    // 8. do_update is not a named-tuple — bare integer literal
    let bad8 = db |> _sql_upsert(Car(Id = 1, Name = "x", Price = 0, DoublePrice = 0),
                                  _.Id, 42)

    // (Empty tuple `()` is a parse error, so the do_update-empty validation
    // rule is unreachable through normal source. Skipped.)

    // 10. do_update has duplicate column names
    let bad10 = db |> _sql_upsert(Car(Id = 1, Name = "x", Price = 0, DoublePrice = 0),
                                   _.Id, (Name = "a", Name = "b"))

    // 11. do_update column is not a field of the struct
    let bad11 = db |> _sql_upsert(Car(Id = 1, Name = "x", Price = 0, DoublePrice = 0),
                                   _.Id, (NotAColumn = 1))

    // 12. do_update column is @sql_computed — SQLite rejects writes to
    //     generated columns.
    let bad12 = db |> _sql_upsert(Car(Id = 1, Name = "x", Price = 0, DoublePrice = 0),
                                   _.Id, (DoublePrice = 999))
}
