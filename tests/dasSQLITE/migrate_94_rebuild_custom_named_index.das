options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

// `[sql_index(name = "...")]` (explicit custom name) must survive convert_and_rename.
// Earlier rebuild path created the staging table WITH indexes, which collided on the
// custom name — SQLite index names are schema-global and the original still owned it.
// Fixed by deferring index creation to after the RENAME.

[sql_table(name = "users", legacy = true)]
struct UserV1 {
    @sql_primary_key Id : int
    Name : string
    Email : string
}

[sql_table(name = "users"),
 sql_index(name = "ux_users_name", fields = "Name", unique)]
struct User {
    @sql_primary_key Id : int
    Name : string
    Email : string
}

[struct_convert]
def convert_v1_to_v2(old : UserV1; var dst : User) {}

[test]
def test_custom_named_index_survives_rebuild(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, Email TEXT NOT NULL DEFAULT '')")
    db |> exec("CREATE UNIQUE INDEX ux_users_name ON users (Name)")
    db |> exec("INSERT INTO users (Id, Name, Email) VALUES (1, 'alice', 'alice@x')")
    db |> convert_and_rename(type<UserV1>, type<User>)
    let n_idx = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='index' AND name='ux_users_name'",
        type<int>)
    t |> equal(n_idx, 1, "custom-named unique index lives after rebuild")
    let n_tab = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='users'",
        type<int>)
    t |> equal(n_tab, 1, "renamed `users` table is the only one (no `users_new` left)")
    let landed = db |> query_scalar("SELECT Name FROM users WHERE Id=1", type<string>)
    t |> equal(landed, "alice", "row preserved through rebuild")
}
