options gen2

require dastest/testing_boost public
require sqlite/sql_migrate

// Versions can be gappy (1, 5, 10) — strict-forward pending detection works on gaps.

[sql_migration(version = 1, description = "first")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE a (x INT)")
}

[sql_migration(version = 5, description = "fifth")]
def m5(db : SqlRunner) {
    db |> exec("CREATE TABLE b (x INT)")
}

[sql_migration(version = 10, description = "tenth")]
def m10(db : SqlRunner) {
    db |> exec("CREATE TABLE c (x INT)")
}

[test]
def test_gappy_versions_run_in_order(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let n = db |> migrate_to_latest()
    t |> equal(n, 3, "all three migrations apply despite gaps")

    let h <- db |> migration_history()
    t |> equal(h |> length, 3, "history has 3 rows")
    t |> equal(h[0].version, 1, "ascending order: v1")
    t |> equal(h[1].version, 5, "ascending order: v5")
    t |> equal(h[2].version, 10, "ascending order: v10")

    let max_v = db |> current_schema_version()
    t |> equal(max_v, 10, "current is the gappy max")
}
