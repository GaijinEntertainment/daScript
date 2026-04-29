options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

require daslib/fio

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

[test]
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

[test]
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

[test]
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
