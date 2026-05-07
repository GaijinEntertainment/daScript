options gen2

// Each `_sql_update` / `_sql_delete` call below is intentionally malformed.
// 40104 = macro_error (from the analyzer).
expect 50503:7

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

[export]
def main() {
    var db = SqlRunner()

    // 1. _sql_update wrong arity (3 instead of 4) → macro_error
    let bad1 = db |> _sql_update(type<Car>, _.Id == 1)

    // 2. _sql_update second arg not type<T>
    let bad2 = db |> _sql_update(42, _.Id == 1, (Name = "x"))

    // 3. _sql_update with positional (non-named) tuple in SET clause
    let bad3 = db |> _sql_update(type<Car>, _.Id == 1, ("x", 42))

    // 4. _sql_delete wrong arity (4 instead of 3)
    let bad4 = db |> _sql_delete(type<Car>, _.Id == 1, (extra = 1))

    // 5. _sql_delete second arg not type<T>
    let bad5 = db |> _sql_delete("Cars", _.Id == 1)

    // 6. _sql_update SET column is not a field of T (typo)
    let bad6 = db |> _sql_update(type<Car>, _.Id == 1, (Nme = "x"))

    // 7. _sql_update SET clause has duplicate column
    let bad7 = db |> _sql_update(type<Car>, _.Id == 1, (Name = "a", Name = "b"))
}
