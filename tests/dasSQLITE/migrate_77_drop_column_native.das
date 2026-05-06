options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

// Bundled SQLite 3.41.2 supports DROP COLUMN natively (since 3.35.0). No 12-step recipe.

[sql_migration(version = 1, description = "create with extra")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, Legacy TEXT)")
}

[sql_migration(version = 2, description = "drop legacy")]
def m2(db : SqlRunner) {
    db |> exec("ALTER TABLE users DROP COLUMN Legacy")
}

[test]
def test_drop_column_works_natively(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let cols = db |> query_scalar("SELECT COUNT(*) FROM pragma_table_info('users')", type<int>)
    t |> equal(cols, 2, "users must have only Id+Name after DROP COLUMN")
    let has_legacy = db |> query_scalar(
        "SELECT COUNT(*) FROM pragma_table_info('users') WHERE name='Legacy'", type<int>)
    t |> equal(has_legacy, 0, "Legacy column must be gone")
}


[test]
def test_sqlite_version_at_least_3_35(t : T?) {
    // 3.35.0+ supports DROP COLUMN (the feature this file tests).
    // sqlite3_libversion_number() encodes 3.35.0 as 3*1000000 + 35*1000 + 0 = 3035000.
    let n = sqlite3_libversion_number()
    t |> success(n >= 3035000,
        "bundled SQLite must be >= 3.35.0 for DROP COLUMN (got '{sqlite_version()}', code {n})")
}
