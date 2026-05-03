options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 12 — `distinct()` row dedupe.
//
// Shapes:
//   - `select_from |> distinct()`                    full-row DISTINCT
//   - `_select(_.Name) |> distinct()`                single-column DISTINCT
//   - `_where |> _select |> distinct()`              composed
//
// Result-order note: SQL DISTINCT does not guarantee any order, while
// linq's `distinct` preserves first-occurrence order. To keep parity
// assertions order-independent, all tests below compare *sorted*
// results — the multiset of distinct values must agree across modes.

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
    db |> insert(Car(Id = 3, Name = "BMW",   Price = 150))
    db |> insert(Car(Id = 4, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 5, Name = "Audi",  Price = 200))
}

def fixture_array() : array<Car> {
    var arr : array<Car>
    arr |> emplace(Car(Id = 1, Name = "BMW",   Price = 100))
    arr |> emplace(Car(Id = 2, Name = "Audi",  Price = 200))
    arr |> emplace(Car(Id = 3, Name = "BMW",   Price = 150))
    arr |> emplace(Car(Id = 4, Name = "Tesla", Price = 300))
    arr |> emplace(Car(Id = 5, Name = "Audi",  Price = 200))
    return <- arr
}

def sorted_strings(var a : array<string>) : array<string> {
    a |> sort()
    return <- a
}

def sorted_int_pairs(cars : array<Car>) : array<tuple<int; int>> {
    var pairs : array<tuple<int; int>>
    for (c in cars) {
        pairs |> push((c.Id, c.Price))
    }
    pairs |> sort() <| $(a, b) {
        if (a._0 != b._0) {
            return a._0 < b._0
        }
        return a._1 < b._1
    }
    return <- pairs
}

def string_arrays_eq(a : array<string>; b : array<string>) : bool {
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

def pair_arrays_eq(a : array<tuple<int; int>>; b : array<tuple<int; int>>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (a[i]._0 != b[i]._0 || a[i]._1 != b[i]._1) {
            return false
        }
    }
    return true
}

[test]
def parity_full_row_distinct(t : T?) {
    // All PKs are unique, so DISTINCT survives every row. The point of
    // the test: full-row DISTINCT must produce the same multiset across
    // all three modes. Compare via sorted (Id, Price) pairs to ignore
    // result ordering.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> distinct())
        let m2 <- (db |> select_from(type<Car>) |> distinct())
        let m3 <- (arr |> distinct())

        let s1 <- sorted_int_pairs(m1)
        let s2 <- sorted_int_pairs(m2)
        let s3 <- sorted_int_pairs(m3)

        t |> success(pair_arrays_eq(s1, s2), "full-row distinct multiset: M1 ≡ M2")
        t |> success(pair_arrays_eq(s2, s3), "full-row distinct multiset: M2 ≡ M3")
    }
}

[test]
def parity_single_col_distinct(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        var m1 <- _sql(db |> select_from(type<Car>) |> _select(_.Name) |> distinct())
        var m2 <- (db |> select_from(type<Car>) |> _select(_.Name) |> distinct())
        var m3 <- (arr |> _select(_.Name) |> distinct())

        m1 |> sort()
        m2 |> sort()
        m3 |> sort()

        t |> equal(length(m1), 3, "Mode 1: 3 unique names")
        t |> equal(length(m2), 3, "Mode 2: 3 unique names")
        t |> equal(length(m3), 3, "Mode 3: 3 unique names")
        t |> success(string_arrays_eq(m1, m2), "single-col distinct multiset: M1 ≡ M2")
        t |> success(string_arrays_eq(m2, m3), "single-col distinct multiset: M2 ≡ M3")
    }
}

[test]
def parity_where_select_distinct(t : T?) {
    // Filter, project, dedupe. After Price > 100 we have rows 2,3,4,5
    // with names {Audi, BMW, Tesla, Audi} → distinct {Audi, BMW, Tesla}.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        var m1 <- _sql(db |> select_from(type<Car>) |> _where(_.Price > 100) |> _select(_.Name) |> distinct())
        var m2 <- (db |> select_from(type<Car>) |> _where(_.Price > 100) |> _select(_.Name) |> distinct())
        var m3 <- (arr |> _where(_.Price > 100) |> _select(_.Name) |> distinct())

        m1 |> sort()
        m2 |> sort()
        m3 |> sort()

        t |> success(string_arrays_eq(m1, m2), "where|select|distinct: M1 ≡ M2")
        t |> success(string_arrays_eq(m2, m3), "where|select|distinct: M2 ≡ M3")
    }
}
