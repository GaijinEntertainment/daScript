options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 04 — `_sql` chain ≡ no-`_sql` chain (over the
// same SQL source) ≡ same chain over a plain `array<T>`. Three modes
// must produce identical results for every chain shape the tutorial uses.
//
// See modules/dasSQLITE/API_CHECKED.md for the equivalence-harness plan.

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
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

def cars_equal(a : array<Car>; b : array<Car>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (a[i].Id != b[i].Id || a[i].Name != b[i].Name || a[i].Price != b[i].Price) {
            return false
        }
    }
    return true
}

[test]
def parity_select_all(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>))
        let m2 <- (db |> select_from(type<Car>))

        t |> success(cars_equal(m1, m2),  "select_all: Mode 1 (_sql) ≡ Mode 2 (no _sql, SQL source)")
        t |> success(cars_equal(m2, arr), "select_all: Mode 2 (SQL source) ≡ Mode 3 (plain array)")
    }
}

[test]
def parity_where_filter(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()
        let threshold = 200

        let m1 <- _sql(db |> select_from(type<Car>) |> _where(_.Price > threshold))
        let m2 <- (db |> select_from(type<Car>) |> _where(_.Price > threshold))
        let m3 <- (arr |> _where(_.Price > threshold))

        t |> success(cars_equal(m1, m2), "where: Mode 1 (_sql) ≡ Mode 2 (no _sql, SQL source)")
        t |> success(cars_equal(m2, m3), "where: Mode 2 (SQL source) ≡ Mode 3 (plain array)")
    }
}

[test]
def parity_project_name(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _select(_.Name))
        let m2 <- (db |> select_from(type<Car>) |> _select(_.Name))
        let m3 <- (arr |> _select(_.Name))

        t |> equal(length(m1), length(m2), "project: Mode 1 length ≡ Mode 2 length")
        t |> equal(length(m2), length(m3), "project: Mode 2 length ≡ Mode 3 length")
        for (i in range(length(m1))) {
            t |> equal(m1[i], m2[i], "project[{i}]: Mode 1 ≡ Mode 2")
            t |> equal(m2[i], m3[i], "project[{i}]: Mode 2 ≡ Mode 3")
        }
    }
}
