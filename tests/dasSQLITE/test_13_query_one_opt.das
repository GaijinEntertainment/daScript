options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
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
// query_one_opt — 0-row case returns none, no panic
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_query_one_opt_some_on_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let res = db |> query_one_opt(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id = ?",
            type<Car>, 1)
        t |> success(res |> is_some, "match -> some(...)")
        let car = res |> unwrap
        t |> equal(car.Name, "BMW", "some value")
    }
}

[test]
def test_query_one_opt_none_on_no_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let res = db |> query_one_opt(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id = ?",
            type<Car>, 999)
        t |> success(res |> is_none, "no match -> none")
    }
}

[test]
def test_query_one_opt_zero_arg(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let res = db |> query_one_opt(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars LIMIT 1",
            type<Car>)
        t |> success(res |> is_some, "non-empty -> some(first)")
    }
}

[test]
def test_query_one_opt_zero_arg_empty(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let res = db |> query_one_opt(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars LIMIT 1",
            type<Car>)
        t |> success(res |> is_none, "empty -> none")
    }
}
