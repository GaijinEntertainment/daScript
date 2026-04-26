options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq
require daslib/linq_boost
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
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// Compatibility mode: select_from used WITHOUT the _sql macro wrapper.
//
// `select_from(db, type<T>) : array<T>` is a regular runtime function. It
// prepares `SELECT col1, col2, ... FROM Table`, steps the cursor, and
// materializes every row via the chunk-1 `[sql_table]`-emitted
// `_sql_read_row` helper + `sqlite_read` dispatch rail. Anything chained on
// top in compat mode is plain daslib/linq running client-side over the
// already-materialized array — no SQL push-down. `_sql` is the optimization
// path; `select_from` standalone is the always-works baseline.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_compat_select_from_bare_returns_array(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- db |> select_from(type<Car>)
        t |> equal(length(cars), 5,        "5 rows materialized")
        t |> equal(cars[0].Name, "BMW",    "row 0 Name")
        t |> equal(cars[2].Name, "Tesla",  "row 2 Name")
        t |> equal(cars[4].Price, 999,     "row 4 Price")
    }
}

[test]
def test_compat_select_from_empty_table(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let cars <- db |> select_from(type<Car>)
        t |> equal(length(cars), 0, "empty table -> empty array")
    }
}

[test]
def test_compat_select_from_for_loop(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        var seen_ids : array<int>
        seen_ids |> reserve(5)
        for (car in db |> select_from(type<Car>)) {
            seen_ids |> push(car.Id)
        }
        t |> equal(length(seen_ids), 5, "for loop iterates all rows")
        t |> equal(seen_ids[0], 1, "first Id")
        t |> equal(seen_ids[4], 5, "last Id")
    }
}

[test]
def test_compat_select_from_then_linq_where(t : T?) {
    // No _sql wrapper: select_from materializes the whole table, then
    // daslib/linq_boost's _where filters client-side over the array via
    // each() iterator wrap.
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- db |> select_from(type<Car>)
        let pricey <- unsafe(each(cars)) |> _where_to_array(_.Price > 200)
        t |> equal(length(pricey), 2, "linq _where over materialized array")
        t |> equal(pricey[0].Name, "Tesla", "row 0")
        t |> equal(pricey[1].Name, "Lambo", "row 1")
    }
}

[test]
def test_compat_select_from_then_linq_select(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- db |> select_from(type<Car>)
        let names <- unsafe(each(cars)) |> _select_to_array(_.Name)
        t |> equal(length(names), 5, "linq _select over materialized array")
        t |> equal(names[0], "BMW",   "name 0")
        t |> equal(names[4], "Lambo", "name 4")
    }
}

[test]
def test_compat_select_from_chain_with_to_array_terminal(t : T?) {
    // sqlite_linq's _to_array is a regular passthrough function in compat
    // mode — it just returns the array.
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- db |> select_from(type<Car>) |> _to_array()
        t |> equal(length(cars), 5, "_to_array() in compat mode is a passthrough")
        t |> equal(cars[2].Name, "Tesla", "row 2 preserved")
    }
}

[test]
def test_compat_try_select_from_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        var r <- db |> try_select_from(type<Car>)
        t |> success(r |> is_ok, "try_select_from returns Ok on a valid table")
        let cars <- r |> unwrap
        t |> equal(length(cars), 5, "5 rows via try_ form")
    }
}

[test]
def test_compat_try_select_from_propagates_error(t : T?) {
    // Calling select_from on a Car table that doesn't exist surfaces the
    // libsqlite3 errmsg through Result rather than panicking.
    with_sqlite(":memory:") $(db) {
        // Note: we did NOT create the Cars table, so the SELECT must fail.
        var r <- db |> try_select_from(type<Car>)
        t |> success(r |> is_err, "try_select_from returns Err on missing table")
    }
}
