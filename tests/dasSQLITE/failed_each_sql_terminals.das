options gen2

// _each_sql rejects materializing terminals — _to_array, _first, _first_opt,
// and aggregates each fire a macro_error pointing at `_sql(...)` for the
// scalar / single-row form.
expect 40104:4

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
def main {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)

        // 1. _to_array — redundant terminal
        for (c in _each_sql(db |> select_from(type<Car>) |> _to_array())) {}

        // 2. _first — single-row terminal
        for (c in _each_sql(db |> select_from(type<Car>) |> _first())) {}

        // 3. _first_opt — single-row terminal
        for (c in _each_sql(db |> select_from(type<Car>) |> _first_opt())) {}

        // 4. sum() — aggregate (scalar)
        for (c in _each_sql(db |> select_from(type<Car>) |> _select(_.Price) |> sum())) {}
    }
}
