options gen2

// linq_das `join … into g` (group-join) over a SQL source. The reader emits `_group_join`, which has no SQL
// push-down form, so `_sql` rejects it — group-join is in-memory only (array sources). One macro_error (50503),
// no cascade. (Own file: this `_sql` infer-time reject aborts the pass, so it can't share with the other
// `_sql` join reject in failed_linq_das_sql_join.das or the `group … by` reject in failed_linq_das_sql.das.)
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

[sql_table(name = "Brands")]
struct Brand {
    @sql_primary_key brand : string
    country : string
}

def trigger_join_into_sql() {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> create_table(type<Brand>)
        // `join … into g` emits `_group_join`, which has no SQL push-down → `_sql` rejects (in-memory only).
        let r <- %linq! from b : Brand in db
                        join c : Car in db on b.brand equals c.brand into g
                        select (brand = b.brand, n = g |> length) %%
        unused(r)
    }
}
