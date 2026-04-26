options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

[test]
def test_query_scalar_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 9223372036854775807", type<int64>)
        t |> equal(v, 9223372036854775807l, "int64 max")
    }
}

[test]
def test_query_scalar_int64_count(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> exec("CREATE TABLE t (x INT)")
        db |> exec("INSERT INTO t VALUES (1), (2), (3)")
        let v = db |> query_scalar("SELECT COUNT(*) FROM t", type<int64>)
        t |> equal(int(v), 3, "count(*) -> int64")
    }
}
