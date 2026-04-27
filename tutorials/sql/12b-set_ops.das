options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 12b — Set operations: UNION / INTERSECT / EXCEPT.
//
// Set operations stack two SELECT queries with compatible projections
// and dedupe / intersect / subtract their result rows. Under `_sql(...)`:
//
//   `lhs |> union(rhs)`     →  `<lhs-sql> UNION <rhs-sql>`
//   `lhs |> intersect(rhs)` →  `<lhs-sql> INTERSECT <rhs-sql>`
//   `lhs |> except(rhs)`    →  `<lhs-sql> EXCEPT <rhs-sql>`
//
// These are the existing `daslib/linq.das` array/iterator fns —
// `union`, `intersect`, `except` — being recognized by the `_sql`
// emitter and lowered to SQL set operations.
//
// Both sides must project the same column shape (same column count
// and types). Use `_select(_.Col)` to pin the shape explicitly when
// the sources have different schemas.

[sql_table(name = "Customers")]
struct Customer {
    @sql_primary_key Id : int
    Tier : int
}

[sql_table(name = "Prospects")]
struct Prospect {
    @sql_primary_key Id : int
    Tier : int
}

def fixture(db : SqlRunner) {
    db |> create_table(type<Customer>)
    db |> create_table(type<Prospect>)
    db |> insert(Customer(Id = 1, Tier = 10))
    db |> insert(Customer(Id = 2, Tier = 20))
    db |> insert(Customer(Id = 3, Tier = 30))
    db |> insert(Prospect(Id = 100, Tier = 20))
    db |> insert(Prospect(Id = 200, Tier = 30))
    db |> insert(Prospect(Id = 300, Tier = 40))
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // UNION — distinct tags from either table.
        let all_tiers <- _sql((db |> select_from(type<Customer>) |> _select(_.Tier))
                              |> union(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        to_log(LOG_INFO, "UNION — {length(all_tiers)} distinct tiers across both tables")
        // 4 rows: 10, 20, 30, 40 (UNION dedupes).

        // INTERSECT — tags present in BOTH tables.
        let shared_tiers <- _sql((db |> select_from(type<Customer>) |> _select(_.Tier))
                                 |> intersect(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        to_log(LOG_INFO, "INTERSECT — {length(shared_tiers)} shared tiers")
        // 2 rows: 20, 30.

        // EXCEPT — Customer tags NOT seen in Prospects.
        let exclusive <- _sql((db |> select_from(type<Customer>) |> _select(_.Tier))
                              |> except(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        to_log(LOG_INFO, "EXCEPT — {length(exclusive)} customer-only tiers")
        // 1 row: 10.

        let sql = _sql_text((db |> select_from(type<Customer>) |> _select(_.Tier))
                            |> union(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        to_log(LOG_INFO, "SQL: {sql}")
    }
}
