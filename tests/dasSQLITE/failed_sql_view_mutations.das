options gen2

// Predicate-form mutations against a [sql_view] type. Every call_macro
// (_sql_update / _sql_delete / _sql_upsert and the try / returning
// variants) must macro_error at compile time with the "views are
// read-only" message — never reach the runtime panic stubs.
expect 40104:8

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
}

[sql_view(name = "CarNames")]
struct CarName {
    Id : int
    Name : string
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> _create_view(type<CarName>, db |> select_from(type<Car>))

        let dummy = CarName(Id = 1, Name = "x")

        // 1. _sql_update — predicate update against view
        _sql_update(db, type<CarName>, _.Id == 1, (Name = "y"))
        // 2. _sql_try_update
        _sql_try_update(db, type<CarName>, _.Id == 1, (Name = "y"))
        // 3. _sql_update_returning
        _sql_update_returning(db, type<CarName>, _.Id == 1, (Name = "y"))
        // 4. _sql_try_update_returning
        _sql_try_update_returning(db, type<CarName>, _.Id == 1, (Name = "y"))
        // 5. _sql_delete
        _sql_delete(db, type<CarName>, _.Id == 1)
        // 6. _sql_try_delete
        _sql_try_delete(db, type<CarName>, _.Id == 1)
        // 7. _sql_upsert — uses validate_outer_upsert_args (different gate)
        _sql_upsert(db, dummy, _.Id, (Name = "y"))
        // 8. _sql_try_upsert
        _sql_try_upsert(db, dummy, _.Id, (Name = "y"))
    }
}
