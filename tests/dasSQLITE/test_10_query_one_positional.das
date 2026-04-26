options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require daslib/result

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
}

// ──────────────────────────────────────────────────────────────────────────
// query_one — single-arg positional bind
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_query_one_single_arg_returns_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let car = db |> query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id = ?",
            type<Car>, 2)
        t |> equal(car.Id,    2,      "filtered Id")
        t |> equal(car.Name,  "Audi", "filtered Name")
        t |> equal(car.Price, 200,    "filtered Price")
    }
}

[test]
def test_query_one_zero_arg_returns_first_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let car = db |> query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars LIMIT 1",
            type<Car>)
        t |> equal(car.Id, 1, "first row Id")
    }
}

[test]
def test_try_query_one_ok_on_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let res = db |> try_query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id = ?",
            type<Car>, 3)
        t |> success(res |> is_ok, "match returns ok")
        let car = res |> unwrap
        t |> equal(car.Name, "Tesla", "ok value")
    }
}

[test]
def test_try_query_one_err_on_no_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let res = db |> try_query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id = ?",
            type<Car>, 999)
        t |> success(res |> is_err, "no rows -> err")
    }
}
