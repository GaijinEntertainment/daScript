options gen2

require dastest/testing_boost public
require sqlite/sql_migrate

// DB ahead of binary: audit has v=9 but the running binary's registered max is v=2.
// migrate_to_latest must return 0 + leave the audit unchanged + log a warning (warning
// capture isn't tested here — production observation is enough; covered by Scenario 4).

[sql_migration(version = 1, description = "v1")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY)")
}

[sql_migration(version = 2, description = "v2")]
def m2(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Email TEXT NOT NULL DEFAULT ''")
}

[test]
def test_db_ahead_of_binary_returns_zero(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE __schema_version (version INTEGER PRIMARY KEY, description TEXT NOT NULL DEFAULT '', applied_at INTEGER NOT NULL)")
    db |> exec("INSERT INTO __schema_version (version, description, applied_at) VALUES (1, 'v1', 1000)")
    db |> exec("INSERT INTO __schema_version (version, description, applied_at) VALUES (9, 'future', 1000)")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Email TEXT NOT NULL DEFAULT '')")

    let n = db |> migrate_to_latest()
    t |> equal(n, 0, "downgrade case must return 0 (no migrations applied)")
    let max_v = db |> query_scalar("SELECT MAX(version) FROM __schema_version", type<int>)
    t |> equal(max_v, 9, "audit unchanged after downgrade detection")
}

[test]
def test_current_schema_version_reads_max(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE __schema_version (version INTEGER PRIMARY KEY, description TEXT NOT NULL DEFAULT '', applied_at INTEGER NOT NULL)")
    db |> exec("INSERT INTO __schema_version (version, description, applied_at) VALUES (9, 'future', 1000)")
    t |> equal(db |> current_schema_version(), 9, "current_schema_version must return raw MAX, even when ahead of binary")
}
