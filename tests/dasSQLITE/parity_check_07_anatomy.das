options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require daslib/strings_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 07 — `_where` mechanics introduced beyond
// tut 04: symmetric column-vs-bind sides (`_.Price > cutoff` ≡ `cutoff < _.Price`),
// chained `_where` (composes with AND), and the `starts_with` string
// predicate inside `_where`.
//
// `_sql_text` (debug emitter) and `query_scalar` (raw SQL) are out of scope
// per API_CHECKED.md.

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
}

def fixture_array() : array<Car> {
    var arr : array<Car>
    arr |> emplace(Car(Id = 1, Name = "BMW",   Price = 100))
    arr |> emplace(Car(Id = 2, Name = "Audi",  Price = 200))
    arr |> emplace(Car(Id = 3, Name = "Tesla", Price = 300))
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
def parity_where_symmetric(t : T?) {
    // The translation should be order-independent: `_.Price > cutoff` and
    // `cutoff < _.Price` must produce the same row set across all three modes.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()
        let cutoff = 150

        let m1_a <- _sql(db |> select_from(type<Car>) |> _where(_.Price > cutoff))
        let m1_b <- _sql(db |> select_from(type<Car>) |> _where(cutoff < _.Price))
        let m2_a <- (db |> select_from(type<Car>) |> _where(_.Price > cutoff))
        let m2_b <- (db |> select_from(type<Car>) |> _where(cutoff < _.Price))
        let m3_a <- (arr |> _where(_.Price > cutoff))
        let m3_b <- (arr |> _where(cutoff < _.Price))

        t |> success(cars_equal(m1_a, m1_b), "Mode 1: column-left ≡ column-right")
        t |> success(cars_equal(m2_a, m2_b), "Mode 2: column-left ≡ column-right")
        t |> success(cars_equal(m3_a, m3_b), "Mode 3: column-left ≡ column-right")
        t |> success(cars_equal(m1_a, m2_a), "Mode 1 ≡ Mode 2 (column-left form)")
        t |> success(cars_equal(m2_a, m3_a), "Mode 2 ≡ Mode 3 (column-left form)")
    }
}

[test]
def parity_where_chained_and(t : T?) {
    // Two `_where` calls compose with AND in `_sql` (and trivially in linq —
    // each filter narrows the array further).
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>)
                         |> _where(_.Price > 100)
                         |> _where(_.Name |> starts_with("T")))
        let m2 <- (db |> select_from(type<Car>)
                     |> _where(_.Price > 100)
                     |> _where(_.Name |> starts_with("T")))
        let m3 <- (arr |> _where(_.Price > 100)
                       |> _where(_.Name |> starts_with("T")))

        t |> success(cars_equal(m1, m2), "chained-where: Mode 1 ≡ Mode 2")
        t |> success(cars_equal(m2, m3), "chained-where: Mode 2 ≡ Mode 3")
    }
}

[test]
def parity_where_starts_with(t : T?) {
    // `starts_with` as a string predicate inside `_where`. SQL emits
    // `LIKE ? ESCAPE '\'` with the appropriate pattern; the in-memory linq
    // path calls `strings_boost::starts_with`. Results must agree.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _where(_.Name |> starts_with("T")))
        let m2 <- (db |> select_from(type<Car>) |> _where(_.Name |> starts_with("T")))
        let m3 <- (arr |> _where(_.Name |> starts_with("T")))

        t |> success(cars_equal(m1, m2), "starts_with: Mode 1 ≡ Mode 2")
        t |> success(cars_equal(m2, m3), "starts_with: Mode 2 ≡ Mode 3")
    }
}
