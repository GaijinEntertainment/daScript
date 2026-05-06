options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_table(name = "users", legacy = true)]
struct UserV1 {
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

[test]
def test_legacy_struct_select_from_works(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, LegacyEmail TEXT NOT NULL DEFAULT '')")
    db |> exec("INSERT INTO users (Id, Name, LegacyEmail) VALUES (1, 'alice', 'alice@x')")
    var rows : array<UserV1>
    rows |> reserve(2)
    for (row in db |> select_from(type<UserV1>)) {
        rows |> emplace(row)
    }
    t |> equal(length(rows), 1, "select_from on legacy struct returns rows")
    t |> equal(rows[0].LegacyEmail, "alice@x", "legacy field readable")
}

[test]
def test_legacy_struct_drop_works(t : T?) {
    // DROP TABLE IF EXISTS is read-side-enough — devs may legitimately drop a legacy table
    // during migration cleanup. Tests that the helper is emitted for legacy structs.
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, LegacyEmail TEXT NOT NULL DEFAULT '')")
    db |> drop_table_if_exists(type<UserV1>)
    let n = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='users'", type<int>)
    t |> equal(n, 0, "drop_table_if_exists works on legacy struct")
}
