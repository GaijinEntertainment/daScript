options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq
require daslib/option

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",     Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",    Price = 200))
}

// ──────────────────────────────────────────────────────────────────────────
// _first_opt — Option<T>, none on empty (LIMIT 1)
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_first_opt_returns_some_when_present(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let res = _sql(db |> select_from(type<Car>) |> _first_opt())
        t |> success(res |> is_some, "non-empty table -> some(...)")
        let car = res |> unwrap
        t |> equal(car.Id, 1, "first row Id")
    }
}

[test]
def test_first_opt_returns_none_when_empty(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let res = _sql(db |> select_from(type<Car>) |> _first_opt())
        t |> success(res |> is_none, "empty table -> none")
    }
}

[test]
def test_first_opt_returns_none_with_no_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let target = 999
        let res = _sql(db |> select_from(type<Car>)
                         |> _where(_.Id == target)
                         |> _first_opt())
        t |> success(res |> is_none, "no matching row -> none")
    }
}

[test]
def test_first_opt_emits_limit_1(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _first_opt())
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" LIMIT 1",
        "_first_opt() emits ` LIMIT 1` suffix")
}
