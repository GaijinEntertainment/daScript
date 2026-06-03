options gen2

// linq_das `group c by k` over a SQL source. C# `group … by` (no `into`) means IGrouping = all rows
// per group; SQL has no such form (a bare GROUP BY returns one arbitrary row per group, a different
// shape). `_sql` rejects the bare `_group_by` the reader emits — use an aggregate pipe form, or an
// in-memory source. Two such rejects below (bare-group + member-keeping); each is a single
// macro_error (50503), no cascade (the `_sql` macro returns null cleanly).
expect 50503   // bare `group … by` (no continuation) over SQL
// A `group … into g` continuation that KEEPS the members (identity `select g` — the whole `(key, [rows])`
// group) has no SQL form either: SQL returns flat rows, not arrays-per-row. It lowers to the same bare
// `_group_by_lazy` and rejects identically. Aggregate continuations DO push down (see the test file).
expect 50503   // group-into member-keeping (identity `select g`) over SQL

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

def trigger_group_into_member_keeping() {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        // identity `select grp` keeps the whole `(key, [rows])` group — no SQL form
        let g <- %linq! from c : Car in db group c by c.brand into grp select grp %%
        unused(g)
    }
}
