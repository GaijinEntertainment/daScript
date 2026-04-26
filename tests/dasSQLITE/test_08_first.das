options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

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
    db |> insert(Car(Id = 3, Name = "Tesla",   Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",    Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo",   Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// _first — single row terminal, panics on empty (LIMIT 1)
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_first_returns_first_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let car = _sql(db |> select_from(type<Car>) |> _first())
        t |> equal(car.Id,    1,     "row 0 Id")
        t |> equal(car.Name,  "BMW", "row 0 Name")
        t |> equal(car.Price, 100,   "row 0 Price")
    }
}

[test]
def test_first_with_where_filters(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let target = 3
        let car = _sql(db |> select_from(type<Car>)
                         |> _where(_.Id == target)
                         |> _first())
        t |> equal(car.Id,    3,        "filtered row Id")
        t |> equal(car.Name,  "Tesla",  "filtered row Name")
    }
}

[test]
def test_first_emits_limit_1(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _first())
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" LIMIT 1",
        "_first() emits ` LIMIT 1` suffix")
}

[test]
def test_first_with_where_emits_limit_1(t : T?) {
    var _db = SqlRunner()
    let target = 3
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Id == target)
                          |> _first())
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Id\" = ? LIMIT 1",
        "_where + _first emit WHERE then ` LIMIT 1`")
}

[test]
def test_first_with_select_and_first(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let name = _sql(db |> select_from(type<Car>)
                          |> _select(_.Name)
                          |> _first())
        t |> equal(name, "BMW", "first projected name")
    }
}
