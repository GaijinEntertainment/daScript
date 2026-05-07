options gen2

// Predicate binds a tuple value — sql_bind has no overload, the catch-all
// generic at sqlite_boost.das fires its concept_assert (40103). This is the
// happy path for "user forgot to define sql_bind for their type" and
// confirms the analyzer no longer rejects unrecognized AST shapes.
expect 31400

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let bad : tuple<int; int> = (1, 2)
        let rows <- _sql(db |> select_from(type<Car>) |> _where(_.Id == bad))
        print("rows: {length(rows)}\n")
    }
}
