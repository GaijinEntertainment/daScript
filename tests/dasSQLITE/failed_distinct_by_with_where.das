options gen2

// `_distinct_by(K)` composed with `_where(P)` must reject in v1. SQL form
// `SELECT * FROM (SELECT *, MIN(pk) FROM t WHERE P GROUP BY K)` (inner WHERE)
// vs `WHERE P` outside the subquery (outer WHERE) have different semantics
// — the former filters before the bare-aggregate picks the min-pk row, the
// latter filters from the picked rows. Disambiguation is deferred to a
// future chunk; today's check_distinct_by_passthrough_gating rejects loudly.
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
        let _ <- _sql(db |> select_from(type<Car>)
                         |> _where(_.Price > 100)
                         |> _distinct_by(_.Name))
    }
}
