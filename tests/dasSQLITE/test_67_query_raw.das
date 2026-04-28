options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

// ----------------------------------------------------------------------------
// Tutorial-shape "row-only" struct: [sql_table] gives us _sql_read_row, but we
// never call create_table on it. Idiomatic raw-query target.
// ----------------------------------------------------------------------------
[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
    Price : int
}

// ----------------------------------------------------------------------------
// Catalog row shapes — for exercising raw `query` against PRAGMA / sqlite_master.
// ----------------------------------------------------------------------------
[sql_table(name = "_pragma_table_info_row")]
struct PragmaTableInfoRow {
    cid : int
    name : string
    // SQL column is `type` but `type` is a daslang keyword; field name differs.
    // Reads work because column mapping is positional, not by name.
    type_str : string
    notnull : int
    dflt_value : string
    pk : int
}

[sql_table(name = "_sqlite_master_name_only")]
struct NamedRow {
    name : string
}

def populate_cars(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "Audi", Price = 52642))
    db |> insert(Car(Id = 2, Name = "Mercedes", Price = 57127))
    db |> insert(Car(Id = 3, Name = "Skoda", Price = 9000))
    db |> insert(Car(Id = 4, Name = "Volvo", Price = 29000))
}

[test]
def test_query_zero_args_returns_all_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        populate_cars(db)
        let rows <- db |> query("SELECT Id, Name, Price FROM Cars ORDER BY Id", type<Car>)
        t |> equal(length(rows), 4, "four rows")
        t |> equal(rows[0].Name, "Audi", "first row name")
        t |> equal(rows[3].Price, 29000, "last row price")
    }
}

[test]
def test_query_one_arg_bind(t : T?) {
    with_sqlite(":memory:") $(db) {
        populate_cars(db)
        let rows <- db |> query("SELECT Id, Name, Price FROM Cars WHERE Price > ? ORDER BY Id",
                                type<Car>, 30000)
        t |> equal(length(rows), 2, "two rows above 30k")
        t |> equal(rows[0].Name, "Audi", "first match")
        t |> equal(rows[1].Name, "Mercedes", "second match")
    }
}

[test]
def test_query_two_arg_bind(t : T?) {
    with_sqlite(":memory:") $(db) {
        populate_cars(db)
        let rows <- db |> query("SELECT Id, Name, Price FROM Cars WHERE Price >= ? AND Price <= ? ORDER BY Id",
                                type<Car>, 20000, 55000)
        t |> equal(length(rows), 2, "two rows in [20k, 55k]")
    }
}

[test]
def test_query_three_arg_bind(t : T?) {
    with_sqlite(":memory:") $(db) {
        populate_cars(db)
        let rows <- db |> query(
            "SELECT Id, Name, Price FROM Cars WHERE Price >= ? AND Price <= ? AND Id <> ? ORDER BY Id",
            type<Car>, 20000, 55000, 1)
        t |> equal(length(rows), 1, "one row after exclusion")
        // Cars in [20k, 55k] = {Audi(1), Volvo(4)}; excluding Id=1 leaves Volvo.
        t |> equal(rows[0].Name, "Volvo", "only Volvo left after exclusion")
    }
}

[test]
def test_query_empty_result_is_ok_not_err(t : T?) {
    with_sqlite(":memory:") $(db) {
        populate_cars(db)
        let rows <- db |> query("SELECT Id, Name, Price FROM Cars WHERE Price > ? ORDER BY Id",
                                type<Car>, 9999999)
        t |> equal(length(rows), 0, "zero rows is just empty array")
    }
}

[test]
def test_try_query_success(t : T?) {
    with_sqlite(":memory:") $(db) {
        populate_cars(db)
        let r <- db |> try_query("SELECT Id, Name, Price FROM Cars ORDER BY Id", type<Car>)
        t |> equal(r |> is_ok, true, "try_query returns Ok")
        let rows <- r |> unwrap
        t |> equal(length(rows), 4, "four rows")
    }
}

[test]
def test_try_query_prepare_error(t : T?) {
    with_sqlite(":memory:") $(db) {
        populate_cars(db)
        let r <- db |> try_query("SELECT * FROM no_such_table", type<Car>)
        t |> equal(r |> is_err, true, "try_query surfaces prepare error")
        let msg = r |> unwrap_err
        t |> equal(starts_with(msg, "query: prepare failed"), true, "err prefix unchanged")
    }
}

[test]
def test_query_pragma_table_info(t : T?) {
    with_sqlite(":memory:") $(db) {
        populate_cars(db)
        // PRAGMA table_info(Cars) returns (cid, name, type, notnull, dflt_value, pk)
        // — six positional columns. The struct field name `type_str` deliberately
        //   does not match the SQL column name because `type` is a daslang
        //   keyword; column order is positional so the read still works.
        let cols <- db |> query("PRAGMA table_info(Cars)", type<PragmaTableInfoRow>)
        t |> equal(length(cols), 3, "Cars has three columns")
        t |> equal(cols[0].name, "Id", "first column is Id")
        t |> equal(cols[0].pk, 1, "Id is the primary key")
        t |> equal(cols[1].name, "Name", "second column is Name")
        t |> equal(cols[2].name, "Price", "third column is Price")
    }
}

[test]
def test_query_sqlite_master_lists_user_tables(t : T?) {
    with_sqlite(":memory:") $(db) {
        populate_cars(db)
        let rows <- db |> query("SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%' ORDER BY name",
                                type<NamedRow>)
        t |> equal(length(rows), 1, "one user table")
        t |> equal(rows[0].name, "Cars", "Cars listed")
    }
}
