options gen2

// Regression: an identity / whole-row `_select($(x) => x)` over a SQL `_join` previously crashed the
// `_sql` analyzer (SIGSEGV — `pred_to_sql` dereferenced a null rendering the bare-variable row). It must
// instead be a clean macro_error (50503): a whole-row select has no SQL column form. One reject (the
// trailing cascade from the now-untyped result is expected).
expect 50503

require daslib/sql
require daslib/linq_boost
require daslib/linq_fold
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key id : int
    name  : string
    brand : string
}

[sql_table(name = "Dealers")]
struct Dealer {
    @sql_primary_key id : int
    brand : string
    city  : string
}

def trigger() {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> create_table(type<Dealer>)
        let r <- _fold(db |> select_from(type<Car>)
            |> _join(db |> select_from(type<Dealer>),
                     $(c, d) => c.brand == d.brand,
                     $(c, d) => c.name)
            |> _select($(x) => x)
            |> to_array())
        unused(r)
    }
}
