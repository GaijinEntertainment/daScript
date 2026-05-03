options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require daslib/strings_boost
require math
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 08 — the full `_where` predicate surface.
// Each shape recognized by the `_sql` predicate analyzer must produce
// the same row set in Mode 1, Mode 2, and Mode 3.
//
// Shapes covered (NEW vs tut 04/05/07):
//   - logic op `||` mixed with string predicate
//   - `ends_with(s)`
//   - `contains(s)`
//   - case folding (`to_lower() == ...`)
//   - free-function `length(_.Field)`
//   - numeric `abs(_.Field)`
//
// `_sql_text` (debug emitter) is out of scope per API_CHECKED.md.

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
    Price : int
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "Ford",        Price = 100))
    db |> insert(Car(Id = 2, Name = "Ferrari",     Price = 9999))
    db |> insert(Car(Id = 3, Name = "Honda",       Price = 200))
    db |> insert(Car(Id = 4, Name = "Lada",        Price = 50))
    db |> insert(Car(Id = 5, Name = "Lamborghini", Price = 88888))
}

def fixture_array() : array<Car> {
    var arr : array<Car>
    arr |> emplace(Car(Id = 1, Name = "Ford",        Price = 100))
    arr |> emplace(Car(Id = 2, Name = "Ferrari",     Price = 9999))
    arr |> emplace(Car(Id = 3, Name = "Honda",       Price = 200))
    arr |> emplace(Car(Id = 4, Name = "Lada",        Price = 50))
    arr |> emplace(Car(Id = 5, Name = "Lamborghini", Price = 88888))
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
def parity_where_or_with_string(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>)
                         |> _where(_.Price < 100 || (_.Name |> starts_with("F"))))
        let m2 <- (db |> select_from(type<Car>)
                     |> _where(_.Price < 100 || (_.Name |> starts_with("F"))))
        let m3 <- (arr |> _where(_.Price < 100 || (_.Name |> starts_with("F"))))

        t |> success(cars_equal(m1, m2), "OR + starts_with: Mode 1 ≡ Mode 2")
        t |> success(cars_equal(m2, m3), "OR + starts_with: Mode 2 ≡ Mode 3")
    }
}

[test]
def parity_where_ends_with(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _where(_.Name |> ends_with("da")))
        let m2 <- (db |> select_from(type<Car>) |> _where(_.Name |> ends_with("da")))
        let m3 <- (arr |> _where(_.Name |> ends_with("da")))

        t |> success(cars_equal(m1, m2), "ends_with: Mode 1 ≡ Mode 2")
        t |> success(cars_equal(m2, m3), "ends_with: Mode 2 ≡ Mode 3")
    }
}

[test]
def parity_where_contains(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _where(_.Name |> contains("o")))
        let m2 <- (db |> select_from(type<Car>) |> _where(_.Name |> contains("o")))
        let m3 <- (arr |> _where(_.Name |> contains("o")))

        t |> success(cars_equal(m1, m2), "contains: Mode 1 ≡ Mode 2")
        t |> success(cars_equal(m2, m3), "contains: Mode 2 ≡ Mode 3")
    }
}

[test]
def parity_where_to_lower_eq(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _where(_.Name |> to_lower() == "ford"))
        let m2 <- (db |> select_from(type<Car>) |> _where(_.Name |> to_lower() == "ford"))
        let m3 <- (arr |> _where(_.Name |> to_lower() == "ford"))

        t |> success(cars_equal(m1, m2), "to_lower==: Mode 1 ≡ Mode 2")
        t |> success(cars_equal(m2, m3), "to_lower==: Mode 2 ≡ Mode 3")
    }
}

[test]
def parity_where_string_length(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _where(length(_.Name) > 5))
        let m2 <- (db |> select_from(type<Car>) |> _where(length(_.Name) > 5))
        let m3 <- (arr |> _where(length(_.Name) > 5))

        t |> success(cars_equal(m1, m2), "length(string): Mode 1 ≡ Mode 2")
        t |> success(cars_equal(m2, m3), "length(string): Mode 2 ≡ Mode 3")
    }
}

[test]
def parity_where_abs(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _where((_.Price |> abs()) > 5000))
        let m2 <- (db |> select_from(type<Car>) |> _where((_.Price |> abs()) > 5000))
        let m3 <- (arr |> _where((_.Price |> abs()) > 5000))

        t |> success(cars_equal(m1, m2), "abs(int): Mode 1 ≡ Mode 2")
        t |> success(cars_equal(m2, m3), "abs(int): Mode 2 ≡ Mode 3")
    }
}
