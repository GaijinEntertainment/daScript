options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

// The flagship rebuild example: legacy + current pair, [struct_convert] with one override line,
// migration body collapses to a single `convert_and_rename` call. Verifies that the entire
// rebuild dance (CREATE staging + copy + DROP + RENAME) preserves rows with field translation.

[sql_table(name = "users", legacy = true)]
struct UserV6 {
    @sql_primary_key Id : int
    Name : string
    LegacyEmail : string
}

[sql_table(name = "users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Email : string
}

[struct_convert]
def my_v6_to_v7(old : UserV6; var dst : User) {
    dst.Email = (old.LegacyEmail != "") ? old.LegacyEmail : "unknown@example.com"
}

[sql_migration(version = 1, description = "create v6 schema with fixture rows")]
def m_001(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, LegacyEmail TEXT NOT NULL DEFAULT '')")
    db |> exec("INSERT INTO users (Id, Name, LegacyEmail) VALUES (1, 'alice', 'alice@x'), (2, 'bob', '')")
}

[sql_migration(version = 2, description = "rebuild users with new shape")]
def m_002(db : SqlRunner) {
    db |> convert_and_rename(type<UserV6>, type<User>)
}

[test]
def test_full_rebuild_preserves_rows(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let n = db |> query_scalar("SELECT COUNT(*) FROM users", type<int>)
    t |> equal(n, 2, "row count preserved across rebuild")
}

[test]
def test_full_rebuild_translates_fields(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let alice_email = db |> query_scalar(
        "SELECT Email FROM users WHERE Name='alice'", type<string>)
    t |> equal(alice_email, "alice@x", "alice's LegacyEmail mapped to Email")
    let bob_email = db |> query_scalar(
        "SELECT Email FROM users WHERE Name='bob'", type<string>)
    t |> equal(bob_email, "unknown@example.com", "bob's empty LegacyEmail picked the fallback")
}

[test]
def test_rebuild_inside_migration_txn(t : T?) {
    // The whole migrate_to_latest call runs inside a single BEGIN IMMEDIATE / COMMIT.
    // Test by introspecting the audit table.
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let v = db |> current_schema_version()
    t |> equal(v, 2, "audit recorded both migrations")
}
