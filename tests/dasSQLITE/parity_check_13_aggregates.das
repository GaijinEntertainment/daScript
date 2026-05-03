options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 13 — column aggregates: count, sum, average, min, max.
// Each is a chain terminal returning a scalar.
//
// Result-type expectation per the tutorial header:
//   - sum / min / max: column's daslang type (int for Price)
//   - average:         double (always promoted)
//   - count:           int (matches `length(arr)`)

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

def fixture_array() : array<Car> {
    var arr : array<Car>
    arr |> emplace(Car(Id = 1, Name = "BMW",   Price = 100))
    arr |> emplace(Car(Id = 2, Name = "Audi",  Price = 200))
    arr |> emplace(Car(Id = 3, Name = "Tesla", Price = 300))
    arr |> emplace(Car(Id = 4, Name = "Lada",  Price = 50))
    arr |> emplace(Car(Id = 5, Name = "Lambo", Price = 999))
    return <- arr
}

[test]
def parity_count(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 = _sql(db |> select_from(type<Car>) |> count())
        let m2 = db |> select_from(type<Car>) |> count()
        let m3 = arr |> count()

        t |> equal(m1, m2, "count: M1 ≡ M2")
        t |> equal(m2, m3, "count: M2 ≡ M3")
        t |> equal(m1, 5,  "count: 5 rows")
    }
}

[test]
def parity_sum(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> sum())
        let m2 = db |> select_from(type<Car>) |> _select(_.Price) |> sum()
        let m3 = arr |> _select(_.Price) |> sum()

        t |> equal(m1, m2, "sum: M1 ≡ M2")
        t |> equal(m2, m3, "sum: M2 ≡ M3")
        t |> equal(m1, 100 + 200 + 300 + 50 + 999, "sum: 1649")
    }
}

[test]
def parity_average(t : T?) {
    // F2 resolved: linq `average` always returns `double` (matching SQL
    // `AVG` and C# LINQ `Average`). All three modes agree on type and value.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> average())
        let m2 = db |> select_from(type<Car>) |> _select(_.Price) |> average()
        let m3 = arr |> _select(_.Price) |> average()

        t |> equal(m1, m2, "average: M1 ≡ M2")
        t |> equal(m2, m3, "average: M2 ≡ M3")
        t |> equal(m1, 329.8lf, "average: 1649 / 5 = 329.8")
    }
}

[test]
def parity_min(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> min())
        let m2 = db |> select_from(type<Car>) |> _select(_.Price) |> min()
        let m3 = arr |> _select(_.Price) |> min()

        t |> equal(m1, m2, "min: M1 ≡ M2")
        t |> equal(m2, m3, "min: M2 ≡ M3")
        t |> equal(m1, 50, "min: 50")
    }
}

[test]
def parity_max(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> max())
        let m2 = db |> select_from(type<Car>) |> _select(_.Price) |> max()
        let m3 = arr |> _select(_.Price) |> max()

        t |> equal(m1, m2, "max: M1 ≡ M2")
        t |> equal(m2, m3, "max: M2 ≡ M3")
        t |> equal(m1, 999, "max: 999")
    }
}

[test]
def parity_where_sum(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()
        let cutoff = 100

        let m1 = _sql(db |> select_from(type<Car>) |> _where(_.Price > cutoff) |> _select(_.Price) |> sum())
        let m2 = db |> select_from(type<Car>) |> _where(_.Price > cutoff) |> _select(_.Price) |> sum()
        let m3 = arr |> _where(_.Price > cutoff) |> _select(_.Price) |> sum()

        t |> equal(m1, m2, "where|sum: M1 ≡ M2")
        t |> equal(m2, m3, "where|sum: M2 ≡ M3")
        t |> equal(m1, 200 + 300 + 999, "where|sum: 1499")
    }
}
