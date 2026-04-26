options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",     Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",    Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla",   Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",    Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo",   Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// _sql — bare select_from
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_sql_bare_select_from_empty_table(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let cars <- _sql(db |> select_from(type<Car>))
        t |> equal(length(cars), 0, "empty table -> empty array")
    }
}

[test]
def test_sql_bare_select_from_returns_all_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>))
        t |> equal(length(cars), 5, "5 rows expected")
        t |> equal(cars[0].Id,    1,        "row 0 Id")
        t |> equal(cars[0].Name,  "BMW",    "row 0 Name")
        t |> equal(cars[0].Price, 100,      "row 0 Price")
        t |> equal(cars[4].Id,    5,        "row 4 Id")
        t |> equal(cars[4].Name,  "Lambo",  "row 4 Name")
        t |> equal(cars[4].Price, 999,      "row 4 Price")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// _to_array — explicit terminal (no-op semantically)
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_sql_to_array_explicit_terminal(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _to_array())
        t |> equal(length(cars), 5, "_to_array() yields the same array")
        t |> equal(cars[0].Name, "BMW", "row 0 Name preserved")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// _select — single-column projection
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_sql_select_string_column(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let names <- _sql(db |> select_from(type<Car>) |> _select(_.Name))
        t |> equal(length(names), 5, "5 names expected")
        t |> equal(names[0], "BMW",   "name 0")
        t |> equal(names[2], "Tesla", "name 2")
        t |> equal(names[4], "Lambo", "name 4")
    }
}

[test]
def test_sql_select_int_column(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let prices <- _sql(db |> select_from(type<Car>) |> _select(_.Price))
        t |> equal(length(prices), 5, "5 prices expected")
        t |> equal(prices[0], 100, "price 0")
        t |> equal(prices[3], 50,  "price 3")
        t |> equal(prices[4], 999, "price 4")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// _where — predicate filter with bind capture
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_sql_where_eq_literal_int(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Id == 3))
        t |> equal(length(cars), 1, "one row matches Id=3")
        t |> equal(cars[0].Name, "Tesla", "matched Tesla")
    }
}

[test]
def test_sql_where_eq_literal_string(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Name == "Audi"))
        t |> equal(length(cars), 1, "one row matches Name=Audi")
        t |> equal(cars[0].Id, 2, "matched Id 2")
    }
}

[test]
def test_sql_where_captured_var(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let target = 4
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Id == target))
        t |> equal(length(cars), 1, "one row matches captured target")
        t |> equal(cars[0].Name, "Lada", "matched Lada")
    }
}

[test]
def test_sql_where_gt_literal(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Price > 200))
        t |> equal(length(cars), 2, "two rows priced > 200 (Tesla, Lambo)")
        t |> equal(cars[0].Name, "Tesla", "row 0 Tesla")
        t |> equal(cars[1].Name, "Lambo", "row 1 Lambo")
    }
}

[test]
def test_sql_where_lt_literal(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Price < 100))
        t |> equal(length(cars), 1, "one row priced < 100 (Lada)")
        t |> equal(cars[0].Name, "Lada", "matched Lada")
    }
}

[test]
def test_sql_where_neq(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Id != 3))
        t |> equal(length(cars), 4, "four rows have Id != 3")
    }
}

[test]
def test_sql_where_and_with_two_captures(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let lo = 100
        let hi = 300
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Price >= lo && _.Price <= hi))
        t |> equal(length(cars), 3, "BMW, Audi, Tesla in [100, 300]")
        t |> equal(cars[0].Name, "BMW",   "row 0")
        t |> equal(cars[1].Name, "Audi",  "row 1")
        t |> equal(cars[2].Name, "Tesla", "row 2")
    }
}

[test]
def test_sql_where_or(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Id == 1 || _.Id == 5))
        t |> equal(length(cars), 2, "two rows: Id=1 OR Id=5")
        t |> equal(cars[0].Name, "BMW",   "row 0")
        t |> equal(cars[1].Name, "Lambo", "row 1")
    }
}

[test]
def test_sql_where_not(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(!(_.Id == 3)))
        t |> equal(length(cars), 4, "four rows have NOT Id=3")
    }
}

[test]
def test_sql_where_then_select_compound(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let names <- _sql(db |> select_from(type<Car>) |> _where(_.Price >= 200) |> _select(_.Name))
        t |> equal(length(names), 3, "three names priced >= 200")
        t |> equal(names[0], "Audi",  "row 0")
        t |> equal(names[1], "Tesla", "row 1")
        t |> equal(names[2], "Lambo", "row 2")
    }
}

[test]
def test_sql_two_where_calls_compose_as_and(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Price >= 100) |> _where(_.Price <= 300))
        t |> equal(length(cars), 3, "two _where calls compose AND-wise: BMW, Audi, Tesla")
    }
}

[test]
def test_sql_with_to_array_terminal(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Id == 2) |> _to_array())
        t |> equal(length(cars), 1, "_to_array as terminal yields a row")
        t |> equal(cars[0].Name, "Audi", "matched Audi")
    }
}
