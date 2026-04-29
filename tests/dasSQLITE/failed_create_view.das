options gen2

// _create_view shape errors. Each block triggers exactly one macro_error
// (40104) from `SqlCreateViewMacro`.
expect 40104:7

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
    Price : int
}

// Right number of fields (3), correct types — for control reference.
[sql_view(name = "CarNames3")]
struct CarName3 {
    Id : int
    Name : string
    Price : int
}

// Too few — only 2 fields vs Car's 3.
[sql_view(name = "TooFew")]
struct TooFew {
    Id : int
    Name : string
}

// Too many — 4 fields vs Car's 3.
[sql_view(name = "TooMany")]
struct TooMany {
    Id : int
    Name : string
    Price : int
    Extra : int
}

// Type mismatch on column 2 — Car.Name is string, V says int.
[sql_view(name = "BadType")]
struct BadType {
    Id : int
    Name : int
    Price : int
}

// Plain struct (no [sql_view]) — must reject.
struct NotAView {
    Id : int
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)

        // 1. column count mismatch — too few fields
        db |> _create_view(type<TooFew>, db |> select_from(type<Car>))
        // 2. column count mismatch — too many fields
        db |> _create_view(type<TooMany>, db |> select_from(type<Car>))
        // 3. column type mismatch
        db |> _create_view(type<BadType>, db |> select_from(type<Car>))
        // 4. type<V> lacks [sql_view]
        db |> _create_view(type<NotAView>, db |> select_from(type<Car>))
        // 5. captured-local in chain — bound parameters not allowed in CREATE VIEW
        let cutoff = 200
        db |> _create_view(type<CarName3>,
            db |> select_from(type<Car>) |> _where(_.Price >= cutoff))
        // 6. _to_array() terminal — redundant for a view
        db |> _create_view(type<CarName3>,
            db |> select_from(type<Car>) |> _to_array())
        // 7. runtime _order_by(string) — column name would be baked at view-creation
        let runtime_col = "Price"
        db |> _create_view(type<CarName3>,
            db |> select_from(type<Car>) |> _order_by(runtime_col))
    }
}
