options gen2

// linq_das join over a SQL source — the transparent-identifier boundary. A post-join `where` (or
// `orderby`, or a `group` terminal) makes the join carry a `(c, b)` whole-row tuple so later clauses can
// see both range vars; `_sql` has no row-object form and rejects the whole-row projection. Select-terminal
// joins and a pre-join `where` DO push down (see test_linq_das_sql.das) — only transparent-id joins are
// in-memory only. One macro_error (50503), no cascade. (Kept in its own file: this `_sql` infer-time
// reject aborts the pass, so it can't share a file with the `group … by` reject in failed_linq_das_sql.das.)
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

def trigger_join_transparent_id() {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> create_table(type<Brand>)
        // post-join `where` references `b` → transparent-id carry tuple → `_sql` rejects.
        let r <- %linq! from c : Car in db join b : Brand in db on c.brand equals b.brand where b.country == "USA" select c.name %%
        unused(r)
    }
}

