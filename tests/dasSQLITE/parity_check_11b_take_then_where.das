options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Multi-Q lowering parity check: outer-Q `_where` over inner-Q `take`.
//
//   `... |> _order_by(_.Price) |> take(3) |> _where(_.Price > 100)`
//
// `_sql` lowers the divergent shape to:
//   SELECT … FROM (SELECT … ORDER BY "Price" ASC LIMIT 3) AS "t0" WHERE "Price" > ?
//
// Linq positional semantics: order by price, take first 3, then filter those 3
// rows by `Price > 100`. All three modes must agree.

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
def parity_take_then_where(t : T?) {
    // Order by price (Lada=50, BMW=100, Audi=200), take first 3, filter Price > 100.
    // Result: only Audi (Lada and BMW dropped by the filter; Tesla and Lambo never reached).
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price) |> take(3) |> _where(_.Price > 100))
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Price) |> take(3) |> _where(_.Price > 100))
        let m3 <- (arr |> _order_by(_.Price) |> take(3) |> _where(_.Price > 100))

        t |> success(cars_equal(m1, m2), "take|where: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "take|where: M2 ≡ M3")
    }
}

[test]
def parity_take_then_two_wheres(t : T?) {
    // Two outer wheres AND-fold in the same outer Q (same phase, no extra divert).
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price) |> take(4)
                         |> _where(_.Price > 100)
                         |> _where(_.Name != "BMW"))
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Price) |> take(4)
                     |> _where(_.Price > 100)
                     |> _where(_.Name != "BMW"))
        let m3 <- (arr |> _order_by(_.Price) |> take(4)
                       |> _where(_.Price > 100)
                       |> _where(_.Name != "BMW"))

        t |> success(cars_equal(m1, m2), "take|where|where: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "take|where|where: M2 ≡ M3")
    }
}

[test]
def parity_inner_where_take_outer_where(t : T?) {
    // Inner Q has its own `_where` (Price > 50 keeps 4 rows), then `_order_by`,
    // then `take(3)`. Outer Q filters those 3 by `Name != "Lambo"`. Both inner
    // and outer wheres co-exist without conflict.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>)
                         |> _where(_.Price > 50)
                         |> _order_by(_.Price)
                         |> take(3)
                         |> _where(_.Name != "Lambo"))
        let m2 <- (db |> select_from(type<Car>)
                     |> _where(_.Price > 50)
                     |> _order_by(_.Price)
                     |> take(3)
                     |> _where(_.Name != "Lambo"))
        let m3 <- (arr |> _where(_.Price > 50)
                       |> _order_by(_.Price)
                       |> take(3)
                       |> _where(_.Name != "Lambo"))

        t |> success(cars_equal(m1, m2), "where|order|take|where: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "where|order|take|where: M2 ≡ M3")
    }
}

[test]
def parity_take_then_where_to_array(t : T?) {
    // Terminal `_to_array()` in outer Q.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price) |> take(3)
                         |> _where(_.Price > 100) |> _to_array())
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Price) |> take(3)
                     |> _where(_.Price > 100) |> _to_array())
        let m3 <- (arr |> _order_by(_.Price) |> take(3)
                       |> _where(_.Price > 100) |> _to_array())

        t |> success(cars_equal(m1, m2), "take|where|to_array: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "take|where|to_array: M2 ≡ M3")
    }
}

[test]
def parity_take_then_where_first_opt(t : T?) {
    // Outer single-row terminal (`_first_opt`) wraps the take|where subquery.
    // Outer LIMIT 1 must NOT collapse the inner LIMIT 3.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 = _sql(db |> select_from(type<Car>) |> _order_by(_.Price) |> take(3)
                        |> _where(_.Price > 100) |> _first_opt())
        let m2 = (db |> select_from(type<Car>) |> _order_by(_.Price) |> take(3)
                    |> _where(_.Price > 100) |> _first_opt())
        let m3 = (arr |> _order_by(_.Price) |> take(3)
                      |> _where(_.Price > 100) |> _first_opt())

        t |> success(is_some(m1), "take|where|first_opt: M1 has value (Audi)")
        t |> success(is_some(m2), "take|where|first_opt: M2 has value")
        t |> success(is_some(m3), "take|where|first_opt: M3 has value")
        t |> equal((m1 |> unwrap_or(Car(Id = 0, Name = "?", Price = 0))).Name, "Audi", "M1 first match is Audi")
        t |> equal((m2 |> unwrap_or(Car(Id = 0, Name = "?", Price = 0))).Name, "Audi", "M2 first match is Audi")
        t |> equal((m3 |> unwrap_or(Car(Id = 0, Name = "?", Price = 0))).Name, "Audi", "M3 first match is Audi")
    }
}
