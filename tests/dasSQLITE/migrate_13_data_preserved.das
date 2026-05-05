options gen2

require dastest/testing_boost public
require sqlite/sql_migrate

[sql_migration(version = 1, description = "create users")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
}

[sql_migration(version = 2, description = "add email default ''")]
def m2(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Email TEXT NOT NULL DEFAULT ''")
}

[test]
def test_existing_rows_preserved_after_alter(t : T?) {
    var inscope db = open_sqlite(":memory:")
    // Apply v1 only (manually, then INSERT some data).
    db |> exec("CREATE TABLE __schema_version (version INTEGER PRIMARY KEY, description TEXT NOT NULL DEFAULT '', applied_at INTEGER NOT NULL)")
    db |> exec("INSERT INTO __schema_version (version, description, applied_at) VALUES (1, 'pre', 1000)")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
    db |> exec("INSERT INTO users (Name) VALUES ('alice')")
    db |> exec("INSERT INTO users (Name) VALUES ('bob')")

    db |> migrate_to_latest()                  // applies v2

    let count = db |> query_scalar("SELECT COUNT(*) FROM users", type<int>)
    t |> equal(count, 2, "existing rows must survive ADD COLUMN")
    let alice_email = db |> query_scalar("SELECT Email FROM users WHERE Name='alice'", type<string>)
    t |> equal(alice_email, "", "default '' must apply to pre-existing rows")
}
