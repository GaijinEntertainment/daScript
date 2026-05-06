options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

// SQLite 3.41.2 supports RENAME COLUMN natively (since 3.25.0).

[sql_migration(version = 1, description = "create")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, OldName TEXT NOT NULL)")
    db |> exec("INSERT INTO users (OldName) VALUES ('alice')")
}

[sql_migration(version = 2, description = "rename column")]
def m2(db : SqlRunner) {
    db |> exec("ALTER TABLE users RENAME COLUMN OldName TO Name")
}

[test]
def test_rename_column_preserves_data(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let has_old = db |> query_scalar(
        "SELECT COUNT(*) FROM pragma_table_info('users') WHERE name='OldName'", type<int>)
    let has_new = db |> query_scalar(
        "SELECT COUNT(*) FROM pragma_table_info('users') WHERE name='Name'", type<int>)
    t |> equal(has_old, 0, "OldName must be gone")
    t |> equal(has_new, 1, "Name must be present")
    let alice = db |> query_scalar("SELECT Name FROM users WHERE Id=1", type<string>)
    t |> equal(alice, "alice", "row data preserved across RENAME")
}

[test]
def test_sqlite_version_at_least_3_25(t : T?) {
    // 3.25.0+ supports RENAME COLUMN (the feature this file tests).
    // sqlite3_libversion_number() encodes 3.25.0 as 3*1000000 + 25*1000 + 0 = 3025000.
    let n = sqlite3_libversion_number()
    t |> success(n >= 3025000,
        "bundled SQLite must be >= 3.25.0 for RENAME COLUMN (got '{sqlite_version()}', code {n})")
}
