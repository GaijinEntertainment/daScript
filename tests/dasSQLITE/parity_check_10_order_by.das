options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 10 — `_order_by` and `_order_by_descending`.
//
// Shapes:
//   - `_order_by(_.Price)`            → ORDER BY single column ASC
//   - `_order_by_descending(_.Price)` → DESC variant
//   - `_order_by((_.Price, _.Name))`  → multi-column tuple key (tie-break)
//   - composed: `_where |> _order_by |> take`
//
// Note: the tutorial fixture has Price ties (BMW=100, Tesla=100). For the
// single-column `_order_by(_.Price)` test, sort stability is *not* part of
// the parity claim — we only assert the resulting Price sequence matches
// across modes, ignoring the relative order of tied rows. The tuple-key
// test eliminates ties by design.

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
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 100))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

def fixture_array() : array<Car> {
    var arr : array<Car>
    arr |> emplace(Car(Id = 1, Name = "BMW",   Price = 100))
    arr |> emplace(Car(Id = 2, Name = "Audi",  Price = 200))
    arr |> emplace(Car(Id = 3, Name = "Tesla", Price = 100))
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

def prices_of(cars : array<Car>) : array<int> {
    var out : array<int>
    for (c in cars) {
        out |> push(c.Price)
    }
    return <- out
}

def int_array_eq(a : array<int>; b : array<int>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (a[i] != b[i]) {
            return false
        }
    }
    return true
}

[test]
def parity_order_by_single_asc(t : T?) {
    // Sort-stability is not part of the parity claim — compare Price sequence.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price))
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Price))
        let m3 <- (arr |> _order_by(_.Price))

        let p1 <- prices_of(m1)
        let p2 <- prices_of(m2)
        let p3 <- prices_of(m3)

        t |> success(int_array_eq(p1, p2), "ASC: Price sequence M1 ≡ M2")
        t |> success(int_array_eq(p2, p3), "ASC: Price sequence M2 ≡ M3")
    }
}

[test]
def parity_order_by_single_desc(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by_descending(_.Price))
        let m2 <- (db |> select_from(type<Car>) |> _order_by_descending(_.Price))
        let m3 <- (arr |> _order_by_descending(_.Price))

        let p1 <- prices_of(m1)
        let p2 <- prices_of(m2)
        let p3 <- prices_of(m3)

        t |> success(int_array_eq(p1, p2), "DESC: Price sequence M1 ≡ M2")
        t |> success(int_array_eq(p2, p3), "DESC: Price sequence M2 ≡ M3")
    }
}

[test]
def parity_order_by_tuple_key(t : T?) {
    // Tuple-key (_.Price, _.Name) eliminates ties — exact full-row order
    // must agree across all three modes.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by((_.Price, _.Name)))
        let m2 <- (db |> select_from(type<Car>) |> _order_by((_.Price, _.Name)))
        let m3 <- (arr |> _order_by((_.Price, _.Name)))

        t |> success(cars_equal(m1, m2), "tuple-key: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "tuple-key: M2 ≡ M3")
    }
}

[test]
def parity_where_orderby_take(t : T?) {
    // Composed chain: filter, sort, limit. The tuple key avoids tie ambiguity
    // in the sort step.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>)
                         |> _where(_.Price > 50)
                         |> _order_by((_.Price, _.Name))
                         |> take(3))
        let m2 <- (db |> select_from(type<Car>)
                     |> _where(_.Price > 50)
                     |> _order_by((_.Price, _.Name))
                     |> take(3))
        let m3 <- (arr |> _where(_.Price > 50)
                       |> _order_by((_.Price, _.Name))
                       |> take(3))

        t |> success(cars_equal(m1, m2), "where|order_by|take: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "where|order_by|take: M2 ≡ M3")
    }
}
