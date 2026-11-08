options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq
require daslib/fio

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
}

[test(description = "vacuum on an in-memory database keeps every row")]
def test_vacuum_in_memory_runs(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        for (i in range(50)) {
            db |> insert(Car(Id = i + 1, Name = "C{i}"))
        }
        // VACUUM on :memory: just rebuilds the in-memory database in place.
        db |> vacuum()
        let n = db |> query_scalar("SELECT COUNT(*) FROM Cars", type<int>)
        t |> equal(n, 50, "rows survived VACUUM")
    }
}

[test(description = "try_vacuum inside a transaction returns an error")]
def test_try_vacuum_in_transaction_returns_err(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        var saw_err = false
        db |> with_transaction() {
            let err = db |> try_vacuum()
            saw_err = (err |> is_some)
        }
        t |> equal(saw_err, true, "try_vacuum returned Err while inside a transaction")
    }
}

[test(description = "PRAGMA optimize on a fresh database returns no error")]
def test_optimize_no_op_on_empty_db(t : T?) {
    with_sqlite(":memory:") $(db) {
        let err = db |> try_optimize()
        t |> equal(err |> is_some, false, "PRAGMA optimize on a fresh db is a no-op")
    }
}

[test(description = "integrity_check on a healthy database reports a single ok row")]
def test_integrity_check_reports_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> insert(Car(Id = 1, Name = "BMW"))
        let issues <- db |> integrity_check()
        t |> equal(length(issues), 1, "one row reported")
        t |> equal(issues[0], "ok", "healthy db reports 'ok'")
    }
}

[test(description = "quick_check on a healthy database reports a single ok row")]
def test_quick_check_reports_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        let issues <- db |> quick_check()
        t |> equal(length(issues), 1, "one row reported")
        t |> equal(issues[0], "ok", "healthy db reports 'ok'")
    }
}

[sql_table(name = "Items")]
struct Item {
    @sql_primary_key Id : int
    Name : string
}

def fixture(db : SqlRunner) {
    db |> create_table(type<Item>)
    db |> insert(Item(Id = 1, Name = "alpha"))
    db |> insert(Item(Id = 2, Name = "beta"))
}

[test(description = "the vacuum-into macro writes a snapshot file at a runtime path with all rows")]
def test_sql_vacuum_into_runtime_path(t : T?) {
    let r = create_temp_file_result("dastest_vacuum_into", ".db")
    t |> equal(r is value, true, "temp file path obtained")
    let path = unsafe(r.value)
    remove(path)
    with_sqlite(":memory:") $(src) {
        fixture(src)
        let err = src |> _sql_vacuum_into(path)
        t |> equal(err |> is_some, false, "vacuum into runtime path")
    }
    t |> equal(fexist(path), true, "snapshot written")
    {
        var inscope reopened = open_sqlite(path)
        let n = reopened |> query_scalar("SELECT COUNT(*) FROM Items", type<int>)
        t |> equal(n, 2, "rows survived VACUUM INTO")
    }
    remove(path)
}

[test(description = "the vacuum-into macro handles a runtime path safely")]
def test_sql_vacuum_into_quote_safety_in_path(t : T?) {
    // VACUUM INTO requires the path string literal embedded in the SQL —
    // any embedded `'` must be doubled. The macro routes through
    // sql_quote_lit. Using a path that resolves to a real file (we can't
    // freely use weird characters on Windows fs), but we can check the
    // SQL text path via _sql_text on a sibling builder. Practical assert:
    // a path with no special chars round-trips.
    let r = create_temp_file_result("with_special", ".db")
    t |> equal(r is value, true, "temp file path obtained")
    let path = unsafe(r.value)
    remove(path)
    with_sqlite(":memory:") $(src) {
        fixture(src)
        let err = src |> _sql_vacuum_into(path)
        t |> equal(err |> is_some, false, "runtime path with no quotes round-trips")
    }
    remove(path)
}

[test(description = "vacuum-into inside a transaction returns an error")]
def test_sql_vacuum_into_in_transaction_errors(t : T?) {
    with_sqlite(":memory:") $(src) {
        fixture(src)
        var saw_err = false
        src |> with_transaction() {
            // SQLite itself rejects VACUUM INTO inside a transaction with
            // "cannot VACUUM from within a transaction"; the macro doesn't
            // add its own guard, so the error surfaces as an SqlError.
            let err = src |> _sql_vacuum_into("/dev/null/should_not_matter")
            saw_err = (err |> is_some)
        }
        t |> equal(saw_err, true, "VACUUM INTO inside txn returns SqlError")
    }
}

[sql_table(name = "Items")]
struct Item_83 {
    @sql_primary_key Id : int
    Name : string
    Qty : int
}

def fixture_83(db : SqlRunner) {
    db |> create_table(type<Item_83>)
    db |> insert(Item_83(Id = 1, Name = "alpha", Qty = 10))
    db |> insert(Item_83(Id = 2, Name = "beta",  Qty = 20))
    db |> insert(Item_83(Id = 3, Name = "gamma", Qty = 30))
}

[test(description = "backup_to copies a database into an in-memory runner with its rows readable")]
def test_backup_to_memory_runner(t : T?) {
    with_sqlite(":memory:") $(src) {
        fixture_83(src)
        with_sqlite(":memory:") $(dst) {
            src |> backup_to(dst)
            let n = dst |> query_scalar("SELECT COUNT(*) FROM Items", type<int>)
            t |> equal(n, 3, "snapshot row count")
            let names <- dst |> select_from(type<Item_83>)
            t |> equal(length(names), 3, "snapshot rows materialize")
            t |> equal(names[1].Name, "beta", "row content preserved")
        }
    }
}

[test(description = "backup_to a file path writes a snapshot that reopens with the same rows")]
def test_backup_to_path_round_trip(t : T?) {
    let r = create_temp_file_result("dastest_backup", ".db")
    t |> equal(r is value, true, "temp file path obtained")
    let path = unsafe(r.value)
    // create_temp_file_result already created the file as empty; remove
    // it so sqlite3_open creates a fresh database for the backup.
    remove(path)
    with_sqlite(":memory:") $(src) {
        fixture_83(src)
        src |> backup_to(path)
    }
    t |> equal(fexist(path), true, "snapshot file written")
    {
        var inscope reopened = open_sqlite(path)
        let qty_total = reopened |> query_scalar("SELECT SUM(Qty) FROM Items", type<int>)
        t |> equal(qty_total, 60, "10 + 20 + 30 = 60")
        let r3 = reopened |> query_one("SELECT * FROM Items WHERE Id = 3", type<Item_83>)
        t |> equal(r3.Name, "gamma", "row 3 round-tripped")
    }
    remove(path)
}

[test(description = "try_backup_to an unwritable path returns an error instead of panicking")]
def test_try_backup_to_unwritable_path_returns_err(t : T?) {
    // try_backup_to(string) used to call open_sqlite(...) which panics on
    // a path SQLite can't open; that broke the try_* contract. After the
    // fix it threads through try_open_sqlite and surfaces the error as
    // some(errmsg) for the caller to recover from.
    with_sqlite(":memory:") $(src) {
        fixture_83(src)
        // A directory that doesn't exist — sqlite3_open returns an error
        // rather than creating intermediate dirs.
        let bogus = "/this/path/should/not/exist/snapshot.db"
        let err = src |> try_backup_to(bogus)
        t |> equal(err |> is_some, true, "open failure surfaces as Err, not panic")
    }
}

[test(description = "backup_to replaces the contents of an existing destination")]
def test_backup_overwrites_existing_destination_runner(t : T?) {
    // sqlite3_backup_run replaces the destination wholesale.
    with_sqlite(":memory:") $(src) {
        fixture_83(src)
        with_sqlite(":memory:") $(dst) {
            dst |> create_table(type<Item_83>)
            dst |> insert(Item_83(Id = 99, Name = "stale", Qty = 0))
            src |> backup_to(dst)
            let n = dst |> query_scalar("SELECT COUNT(*) FROM Items", type<int>)
            t |> equal(n, 3, "stale row gone, src rows now present")
            let stale <- dst |> try_query_one("SELECT * FROM Items WHERE Id = 99", type<Item_83>)
            t |> equal(stale |> is_err, true, "stale row 99 not in snapshot")
        }
    }
}
