options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq
require daslib/result
require daslib/option

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",  Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi", Price = 200))
}

// ──────────────────────────────────────────────────────────────────────────
// _try_sql — non-panicking sibling of _sql. Same analyzer, wraps result
// in Result<T, string>.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_try_sql_array_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        var res <- _try_sql(db |> select_from(type<Car>))
        t |> success(res |> is_ok, "well-formed query -> ok")
        let cars <- res._value
        t |> equal(length(cars), 2, "2 rows")
    }
}

[test]
def test_try_sql_first_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let res = _try_sql(db |> select_from(type<Car>) |> _first())
        t |> success(res |> is_ok, "non-empty -> ok")
        let car = res |> unwrap
        t |> equal(car.Id, 1, "first row")
    }
}

[test]
def test_try_sql_first_err_on_empty(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let res = _try_sql(db |> select_from(type<Car>) |> _first())
        t |> success(res |> is_err, "empty + _first -> err")
    }
}

[test]
def test_try_sql_first_opt_some(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let res = _try_sql(db |> select_from(type<Car>) |> _first_opt())
        t |> success(res |> is_ok, "ok wraps the option")
        let opt = res |> unwrap
        t |> success(opt |> is_some, "non-empty -> some(...)")
    }
}

[test]
def test_try_sql_first_opt_none(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let res = _try_sql(db |> select_from(type<Car>) |> _first_opt())
        t |> success(res |> is_ok, "empty -> ok(none) (not err)")
        let opt = res |> unwrap
        t |> success(opt |> is_none, "empty -> none")
    }
}

[test]
def test_try_sql_count(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let res = _try_sql(db |> select_from(type<Car>) |> count())
        t |> success(res |> is_ok, "count is always 1 row -> ok")
        t |> equal(int(res |> unwrap), 2, "ok value")
    }
}
