options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_migration(version = 1, description = "users")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY)")
}

[sql_migration(version = 2, description = "add email")]
def m2(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Email TEXT NOT NULL DEFAULT ''")
}

[test]
def test_steady_state_v1_to_v2(t : T?) {
    var inscope db = open_sqlite(":memory:")
    // Simulate user already at v1 by inserting an audit row.
    db |> exec("CREATE TABLE __schema_version (version INTEGER PRIMARY KEY, description TEXT NOT NULL DEFAULT '', applied_at INTEGER NOT NULL)")
    db |> exec("INSERT INTO __schema_version (version, description, applied_at) VALUES (1, 'pre-existing', 1000)")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY)")

    let n = db |> migrate_to_latest()
    t |> equal(n, 1, "should apply only v2")

    let max_v = db |> query_scalar("SELECT MAX(version) FROM __schema_version", type<int>)
    t |> equal(max_v, 2, "audit max must be 2")

    let has_email = db |> query_scalar(
        "SELECT COUNT(*) FROM pragma_table_info('users') WHERE name='Email'", type<int>)
    t |> equal(has_email, 1, "Email column must be present after v2")
}
