options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require daslib/result

[test]
def test_try_query_scalar_int(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> try_query_scalar("SELECT 7", type<int>)
        t |> success(res |> is_ok, "ok")
        t |> equal(res |> unwrap, 7, "value")
    }
}

[test]
def test_try_query_scalar_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> try_query_scalar("SELECT 12345", type<int64>)
        t |> success(res |> is_ok, "ok")
        t |> equal(res |> unwrap, 12345l, "value")
    }
}

[test]
def test_try_query_scalar_double(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> try_query_scalar("SELECT 1.5", type<double>)
        t |> success(res |> is_ok, "ok")
        t |> equal(res |> unwrap, 1.5lf, "value")
    }
}

[test]
def test_try_query_scalar_bool(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> try_query_scalar("SELECT 1", type<bool>)
        t |> success(res |> is_ok, "ok")
        t |> equal(res |> unwrap, true, "value")
    }
}

[test]
def test_try_query_scalar_int_err_on_no_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> exec("CREATE TABLE u (x INT)")
        let res = db |> try_query_scalar("SELECT x FROM u WHERE x = 999", type<int>)
        t |> success(res |> is_err, "no rows -> err")
    }
}
