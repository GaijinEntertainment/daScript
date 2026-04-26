options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

[test]
def test_query_scalar_double(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 3.14", type<double>)
        t |> equal(v, 3.14lf, "double scalar")
    }
}

[test]
def test_query_scalar_double_pi(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 3.141592653589793", type<double>)
        t |> success(v > 3.14lf && v < 3.15lf, "double precision")
    }
}
