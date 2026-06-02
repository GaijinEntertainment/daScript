options gen2

// linq_das `group c by k` over a SQL source. C# `group … by` (no `into`) means IGrouping = all rows
// per group; SQL has no such form (a bare GROUP BY returns one arbitrary row per group, a different
// shape). `_sql` rejects the bare `_group_by` the reader emits — use an aggregate pipe form, or an
// in-memory source. One macro_error (50503), no cascade (the `_sql` macro returns null cleanly).
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
        let g <- %linq! from c : Car in db group c by c.brand %%
        unused(g)
    }
}
