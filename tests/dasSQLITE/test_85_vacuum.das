options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
}

[test]
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

[test]
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

[test]
def test_optimize_no_op_on_empty_db(t : T?) {
    with_sqlite(":memory:") $(db) {
        let err = db |> try_optimize()
        t |> equal(err |> is_some, false, "PRAGMA optimize on a fresh db is a no-op")
    }
}

[test]
def test_integrity_check_reports_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> insert(Car(Id = 1, Name = "BMW"))
        let issues <- db |> integrity_check()
        t |> equal(length(issues), 1, "one row reported")
        t |> equal(issues[0], "ok", "healthy db reports 'ok'")
    }
}

[test]
def test_quick_check_reports_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        let issues <- db |> quick_check()
        t |> equal(length(issues), 1, "one row reported")
        t |> equal(issues[0], "ok", "healthy db reports 'ok'")
    }
}
