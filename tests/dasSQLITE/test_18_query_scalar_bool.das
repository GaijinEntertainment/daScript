options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

[test]
def test_query_scalar_bool_true(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 1", type<bool>)
        t |> equal(v, true, "1 -> true")
    }
}

[test]
def test_query_scalar_bool_false(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 0", type<bool>)
        t |> equal(v, false, "0 -> false")
    }
}

[test]
def test_query_scalar_bool_from_predicate(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 5 > 3", type<bool>)
        t |> equal(v, true, "5 > 3 -> true")
    }
}
