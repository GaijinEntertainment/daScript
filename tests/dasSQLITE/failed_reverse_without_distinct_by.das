options gen2

// `reverse()` in `_sql` is only legal when wrapped by `_distinct_by(_.K)` —
// SQL has no inherent row ordering to reverse. Bare `reverse() |> to_array`
// has no SQL form; the user should use `_order_by_descending(...)` for sort
// reversal, or `reverse() |> _distinct_by(K)` for last-per-K semantics.
expect 50503

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
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let _ <- _sql(db |> select_from(type<Car>) |> reverse())
    }
}
