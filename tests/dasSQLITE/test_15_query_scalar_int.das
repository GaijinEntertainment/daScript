options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

[test]
def test_query_scalar_int(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 42", type<int>)
        t |> equal(v, 42, "int scalar")
    }
}

[test]
def test_query_scalar_int_negative(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT -7", type<int>)
        t |> equal(v, -7, "int scalar negative")
    }
}

[test]
def test_try_query_scalar_int_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> try_query_scalar("SELECT 100", type<int>)
        t |> success(res |> is_ok, "ok")
        t |> equal(res |> unwrap, 100, "value")
    }
}
