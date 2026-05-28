options gen2

// `_._1 |> first()` mixed with a column aggregate (`_._1 |> length`,
// `_._1 |> select($(u:T) => u.X) |> sum/min/max/average`) in the same
// projection is rejected in v1 — would require an extra COUNT(*) AS
// alongside the bare-aggregate MIN(pk), and the row builder would have
// to interleave scalar reads with offset row reads. Deferred to a
// follow-up; today the user splits the projection into two `_sql` calls.
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
                         |> _group_by(_.Name)
                         |> _select((Brand = _._0, N = _._1 |> length, FirstCar = _._1 |> first())))
    }
}
