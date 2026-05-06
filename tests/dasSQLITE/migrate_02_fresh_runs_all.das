options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_migration(version = 1, description = "create users")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
}

[sql_migration(version = 2, description = "add email")]
def m2(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Email TEXT NOT NULL DEFAULT ''")
}

[sql_migration(version = 3, description = "add score")]
def m3(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Score INTEGER NOT NULL DEFAULT 0")
}

[test]
def test_fresh_runs_all_three(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let n = db |> migrate_to_latest()
    t |> equal(n, 3, "fresh DB must apply all 3 registered migrations")
}

[test]
def test_fresh_returns_count(t : T?) {
    var inscope db = open_sqlite(":memory:")
    t |> equal(db |> migrate_to_latest(), 3, "first call applies 3")
    t |> equal(db |> migrate_to_latest(), 0, "second call applies 0 (no-op rerun)")
}

[test]
def test_fresh_runs_each_body(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let cols = db |> query_scalar("SELECT COUNT(*) FROM pragma_table_info('users')", type<int>)
    t |> equal(cols, 4, "users must have Id+Name+Email+Score (4 columns) after all migrations")
}

[test]
def test_audit_has_three_rows_with_versions(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let n = db |> query_scalar("SELECT COUNT(*) FROM __schema_version", type<int>)
    t |> equal(n, 3, "__schema_version must have one row per applied migration")
    let max_v = db |> query_scalar("SELECT MAX(version) FROM __schema_version", type<int>)
    t |> equal(max_v, 3, "MAX(version) must be 3")
}
