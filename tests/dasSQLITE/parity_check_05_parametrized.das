options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 05 — `_where(...) |> _first()` and
// `_where(...) |> _first_opt()`. Chain semantics across the three modes.
//
// The raw-SQL escape hatch (`query_one(sql, type<T>, args...)`) is NOT
// covered here — it is a non-chain code path with no Mode-3 analog.
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
}

def fixture_array() : array<Car> {
    var arr : array<Car>
    arr |> emplace(Car(Id = 1, Name = "BMW",   Price = 100))
    arr |> emplace(Car(Id = 2, Name = "Audi",  Price = 200))
    arr |> emplace(Car(Id = 3, Name = "Tesla", Price = 300))
    return <- arr
}

def car_eq(a : Car; b : Car) : bool {
    return a.Id == b.Id && a.Name == b.Name && a.Price == b.Price
}

[test]
def parity_first_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()
        let target = 3

        let m1 = _sql(db |> select_from(type<Car>) |> _where(_.Id == target) |> _first())
        let m2 = db |> select_from(type<Car>) |> _where(_.Id == target) |> _first()
        let m3 = arr |> _where(_.Id == target) |> _first()

        t |> success(car_eq(m1, m2), "first: Mode 1 (_sql) ≡ Mode 2 (no _sql, SQL source)")
        t |> success(car_eq(m2, m3), "first: Mode 2 (SQL source) ≡ Mode 3 (plain array)")
    }
}

[test]
def parity_first_opt_miss(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 = _sql(db |> select_from(type<Car>) |> _where(_.Id == 999) |> _first_opt())
        let m2 = db |> select_from(type<Car>) |> _where(_.Id == 999) |> _first_opt()
        let m3 = arr |> _where(_.Id == 999) |> _first_opt()

        t |> success(m1 |> is_none, "first_opt(miss): Mode 1 produces none")
        t |> success(m2 |> is_none, "first_opt(miss): Mode 2 produces none")
        t |> success(m3 |> is_none, "first_opt(miss): Mode 3 produces none")
    }
}

[test]
def parity_first_opt_hit(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()
        let target = 2

        let m1 = _sql(db |> select_from(type<Car>) |> _where(_.Id == target) |> _first_opt())
        let m2 = db |> select_from(type<Car>) |> _where(_.Id == target) |> _first_opt()
        let m3 = arr |> _where(_.Id == target) |> _first_opt()

        t |> success(m1 |> is_some, "first_opt(hit): Mode 1 produces some")
        t |> success(m2 |> is_some, "first_opt(hit): Mode 2 produces some")
        t |> success(m3 |> is_some, "first_opt(hit): Mode 3 produces some")
        t |> success(car_eq(m1 |> unwrap, m2 |> unwrap), "first_opt(hit): Mode 1 ≡ Mode 2 value")
        t |> success(car_eq(m2 |> unwrap, m3 |> unwrap), "first_opt(hit): Mode 2 ≡ Mode 3 value")
    }
}
