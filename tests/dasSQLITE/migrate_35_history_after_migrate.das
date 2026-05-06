options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_migration(version = 1, description = "alpha")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE x (a INT)")
}

[sql_migration(version = 2, description = "beta")]
def m2(db : SqlRunner) {
    db |> exec("CREATE TABLE y (b INT)")
}

[sql_migration(version = 3, description = "gamma")]
def m3(db : SqlRunner) {
    db |> exec("CREATE TABLE z (c INT)")
}

[test]
def test_history_contains_all_applied_in_version_order(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let h <- db |> migration_history()
    t |> equal(h |> length, 3, "history must have 3 rows")
    t |> equal(h[0].version, 1, "row 0 = v1")
    t |> equal(h[1].version, 2, "row 1 = v2")
    t |> equal(h[2].version, 3, "row 2 = v3")
    t |> equal(h[0].description, "alpha", "v1 description preserved")
    t |> equal(h[2].description, "gamma", "v3 description preserved")
    t |> success(h[0].applied_at > 0l, "applied_at must be > 0")
    t |> success(h[2].applied_at >= h[0].applied_at, "applied_at must be monotonic")
}

[test]
def test_current_after_migrate(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    t |> equal(db |> current_schema_version(), 3, "current must equal MAX(version)")
}
