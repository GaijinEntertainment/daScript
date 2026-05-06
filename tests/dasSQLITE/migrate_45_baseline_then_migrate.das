options gen2

require dastest/testing_boost public
require sqlite/sql_migrate

[sql_migration(version = 1, description = "create users")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY)")
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
def test_baseline_then_migrate_applies_only_remainder(t : T?) {
    var inscope db = open_sqlite(":memory:")
    // Simulate existing v1 schema.
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY)")

    db |> baseline(1)
    let n = db |> migrate_to_latest()
    t |> equal(n, 2, "only v2 + v3 should run after baseline(1)")

    // History has all 3 rows: v1 baselined, v2 + v3 applied.
    let h <- db |> migration_history()
    t |> equal(h |> length, 3, "history has 3 rows total")
    t |> equal(h[0].version, 1, "first row is the baselined v1")
    t |> equal(h[1].version, 2, "second row is v2")
    t |> equal(h[2].version, 3, "third row is v3")

    // Email and Score columns must exist (v2 and v3 actually ran).
    let has_email = db |> query_scalar(
        "SELECT COUNT(*) FROM pragma_table_info('users') WHERE name='Email'", type<int>)
    let has_score = db |> query_scalar(
        "SELECT COUNT(*) FROM pragma_table_info('users') WHERE name='Score'", type<int>)
    t |> equal(has_email, 1, "Email column added by v2")
    t |> equal(has_score, 1, "Score column added by v3")
}
