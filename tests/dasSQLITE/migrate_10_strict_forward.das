options gen2

require dastest/testing_boost public
require sqlite/sql_migrate

// Strict-forward pending detection (Scenario 2 lock): pending = registered with version > MAX(applied),
// NOT set-difference. If audit has {1,3}, v2 is silently skipped because MAX > 2.

[sql_migration(version = 1, description = "users")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY)")
}

[sql_migration(version = 2, description = "v2 — should be skipped")]
def m2(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Email TEXT NOT NULL DEFAULT ''")
}

[sql_migration(version = 3, description = "v3 — already in audit")]
def m3(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Score INTEGER NOT NULL DEFAULT 0")
}

[test]
def test_skips_gap_silently(t : T?) {
    var inscope db = open_sqlite(":memory:")
    // Set up: applied = {1, 3} (manually, simulating dev-branch divergence).
    db |> exec("CREATE TABLE __schema_version (version INTEGER PRIMARY KEY, description TEXT NOT NULL DEFAULT '', applied_at INTEGER NOT NULL)")
    db |> exec("INSERT INTO __schema_version (version, description, applied_at) VALUES (1, 'a', 1000)")
    db |> exec("INSERT INTO __schema_version (version, description, applied_at) VALUES (3, 'c', 1000)")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Score INTEGER NOT NULL DEFAULT 0)")

    let n = db |> migrate_to_latest()
    t |> equal(n, 0, "must apply 0 — strict-forward sees MAX=3, nothing > 3")

    // v2's body did NOT run — Email column must NOT exist.
    let has_email = db |> query_scalar(
        "SELECT COUNT(*) FROM pragma_table_info('users') WHERE name='Email'", type<int>)
    t |> equal(has_email, 0, "Email column must NOT exist (v2 skipped)")
}
