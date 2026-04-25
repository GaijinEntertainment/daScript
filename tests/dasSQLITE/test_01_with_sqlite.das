options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

[test]
def test_sqlite_version_no_db(t : T?) {
    let v = sqlite_version()
    t |> success(!empty(v), "sqlite_version() must return non-empty string")
}

[test]
def test_with_sqlite_memory(t : T?) {
    var entered = false
    with_sqlite(":memory:") $(db) {
        entered = true
        let v = db |> query_scalar("SELECT SQLITE_VERSION()", type<string>)
        t |> success(!empty(v), "query_scalar must return non-empty version")
    }
    t |> success(entered, "with_sqlite block must run")
}

[test]
def test_open_close(t : T?) {
    var inscope db = open_sqlite(":memory:")
    t |> success(db.sqlite_handle != null, "open_sqlite must populate sqlite_handle")
    // finalize fires at scope exit and nulls the handle
}

[test]
def test_try_open_failure_returns_err(t : T?) {
    // /no/such/dir/x.db cannot be created on Windows or Unix
    let r = try_open_sqlite("/no/such/dir/x.db")
    t |> success(r |> is_err, "try_open_sqlite must return err(errmsg) on bad path")
}

[test]
def test_query_scalar_string(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 'hello'", type<string>)
        t |> equal(v, "hello", "scalar string round-trip")
    }
}

[test]
def test_try_query_scalar_zero_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        // SELECT WHERE FALSE returns no rows — try_ form must surface Err.
        let r = db |> try_query_scalar("SELECT 'x' WHERE 1=0", type<string>)
        t |> success(r |> is_err, "zero-rows scalar must Err")
    }
}

[test]
def test_try_exec_failure(t : T?) {
    with_sqlite(":memory:") $(db) {
        let err = db |> try_exec("THIS IS NOT VALID SQL")
        t |> success(err |> is_some, "try_exec must return some(errmsg) on bad SQL")
    }
}

[test]
def test_last_insert_rowid_zero_initially(t : T?) {
    with_sqlite(":memory:") $(db) {
        let id = db |> last_insert_rowid()
        t |> equal(id, int64(0), "last_insert_rowid is 0 before any insert")
    }
}
