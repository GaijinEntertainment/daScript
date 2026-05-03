options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Multi-Q lowering parity check: outer-Q `_order_by` over inner-Q `take`.
//
//   `... |> _order_by(_.Id) |> take(3) |> _order_by(_.Price)`
//
// Lowers under `_sql` to:
//   SELECT … FROM (SELECT … ORDER BY "Id" ASC LIMIT 3) AS "t0" ORDER BY "Price" ASC
//
// Linq positional semantics: order by Id (deterministic), take first 3, then
// re-sort those 3 by Price.

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
def parity_take_then_order(t : T?) {
    // Order by Id, take 3 (BMW=100, Audi=200, Tesla=300), re-sort by Price ascending
    // (BMW=100 < Audi=200 < Tesla=300 — same order; just covers the divert path).
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Id) |> take(3) |> _order_by(_.Price))
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Id) |> take(3) |> _order_by(_.Price))
        let m3 <- (arr |> _order_by(_.Id) |> take(3) |> _order_by(_.Price))

        t |> success(cars_equal(m1, m2), "order|take|order: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "order|take|order: M2 ≡ M3")
    }
}

[test]
def parity_take_then_order_desc(t : T?) {
    // Inner: order by Id, take 3 (BMW, Audi, Tesla). Outer: order by Price DESC
    // (Tesla=300, Audi=200, BMW=100).
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Id) |> take(3) |> _order_by_descending(_.Price))
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Id) |> take(3) |> _order_by_descending(_.Price))
        let m3 <- (arr |> _order_by(_.Id) |> take(3) |> _order_by_descending(_.Price))

        t |> success(cars_equal(m1, m2), "order|take|order_desc: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "order|take|order_desc: M2 ≡ M3")
        t |> equal(length(m1), 3, "result has 3 rows")
        t |> equal(m1[0].Name, "Tesla", "highest price first")
    }
}

[test]
def parity_three_level_nesting(t : T?) {
    // Inner Q: order by Id, take 4 (BMW, Audi, Tesla, Lada). Mid Q: order by Price ASC
    // (Lada=50, BMW=100, Audi=200, Tesla=300). Outer Q: take 2 (Lada, BMW).
    //
    // SQL:
    //   SELECT … FROM (
    //       SELECT … FROM (SELECT … ORDER BY "Id" ASC LIMIT 4) AS "t0"
    //       ORDER BY "Price" ASC
    //   ) AS "t0" LIMIT 2
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Id) |> take(4) |> _order_by(_.Price) |> take(2))
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Id) |> take(4) |> _order_by(_.Price) |> take(2))
        let m3 <- (arr |> _order_by(_.Id) |> take(4) |> _order_by(_.Price) |> take(2))

        t |> success(cars_equal(m1, m2), "three-level: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "three-level: M2 ≡ M3")
        t |> equal(length(m1), 2, "result has 2 rows")
        t |> equal(m1[0].Name, "Lada", "lowest of inner-4 first")
        t |> equal(m1[1].Name, "BMW", "second-lowest second")
    }
}
