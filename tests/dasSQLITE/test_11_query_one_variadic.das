options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

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
    db |> insert(Car(Id = 4, Name = "Tesla",   Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// query_one — 2/3-arg positional bind (mixed types)
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_query_one_two_args(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let car = db |> query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Name = ? AND Price = ?",
            type<Car>, "Tesla", 999)
        t |> equal(car.Id,    4,       "matched Id")
        t |> equal(car.Name,  "Tesla", "matched Name")
        t |> equal(car.Price, 999,     "matched Price")
    }
}

[test]
def test_query_one_three_args(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // 3-arg form: id-range + name-prefix-like
        let car = db |> query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id >= ? AND Id <= ? AND Name = ?",
            type<Car>, 1, 5, "Audi")
        t |> equal(car.Id,    2,      "matched Id")
        t |> equal(car.Name,  "Audi", "matched Name")
    }
}

[test]
def test_try_query_one_two_args_err(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let res = db |> try_query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Name = ? AND Price = ?",
            type<Car>, "Tesla", 1)
        t |> success(res |> is_err, "no row matches Tesla+1")
    }
}
