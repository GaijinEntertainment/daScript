options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require daslib/option

[test]
def test_query_scalar_opt_string_some(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> query_scalar_opt("SELECT 'hello'", type<string>)
        t |> success(res |> is_some, "some")
        t |> equal(res |> unwrap, "hello", "value")
    }
}

[test]
def test_query_scalar_opt_int_some(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> query_scalar_opt("SELECT 42", type<int>)
        t |> success(res |> is_some, "some")
        t |> equal(res |> unwrap, 42, "value")
    }
}

[test]
def test_query_scalar_opt_none_on_empty(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> exec("CREATE TABLE u (x INT)")
        let res = db |> query_scalar_opt("SELECT x FROM u WHERE x = 999", type<int>)
        t |> success(res |> is_none, "no rows -> none")
    }
}

[test]
def test_query_scalar_opt_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> query_scalar_opt("SELECT 9223372036854775807", type<int64>)
        t |> success(res |> is_some, "some")
        t |> equal(res |> unwrap, 9223372036854775807l, "int64 max")
    }
}

[test]
def test_query_scalar_opt_double(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> query_scalar_opt("SELECT 3.14", type<double>)
        t |> success(res |> is_some, "some")
        t |> equal(res |> unwrap, 3.14lf, "value")
    }
}

[test]
def test_query_scalar_opt_bool(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> query_scalar_opt("SELECT 1", type<bool>)
        t |> success(res |> is_some, "some")
        t |> equal(res |> unwrap, true, "value")
    }
}
