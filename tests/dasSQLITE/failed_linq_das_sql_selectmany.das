options gen2

// linq_das correlated `from c … from x in <c-referencing collection>` (SelectMany flatten) over a SQL
// source. The reader emits `_select_many`, which expands to `select_many_pair`; `_sql`'s analyze_chain
// rejects it — a correlated inner collection is a per-row value, not a `[sql_table]`, so it has no SQL
// translation. (Here the inner `[c.id]` is an inline array referencing the outer row — any correlated
// second source over SQL is rejected the same way; use a `join` on a related table instead.)
// One macro_error (50503), no cascade.
expect 50503

require daslib/linq_das
require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key id : int
    name  : string
    brand : string
}

def trigger() {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let r <- %linq! from c : Car in db from x in [c.id] select x %%
        unused(r)
    }
}
