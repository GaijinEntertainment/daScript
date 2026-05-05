options gen2

require dastest/testing_boost public
require sqlite/sql_migrate

[sql_migration(version = 1, description = "v1")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE x (a INT)")
}

[sql_migration(version = 2, description = "v2")]
def m2(db : SqlRunner) {
    db |> exec("CREATE TABLE y (b INT)")
}

[test]
def test_baseline_twice_is_idempotent(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let r1 <- db |> try_baseline(2)
    let r2 <- db |> try_baseline(2)
    t |> equal(r1 |> unwrap, 2, "first call stamps 2 rows")
    t |> equal(r2 |> unwrap, 0, "second call stamps 0 rows (idempotent)")

    let n = db |> query_scalar("SELECT COUNT(*) FROM __schema_version", type<int>)
    t |> equal(n, 2, "audit row count unchanged after idempotent rerun")
}
