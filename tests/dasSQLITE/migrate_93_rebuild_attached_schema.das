options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

// _qualify_schema must rewrite ALTER TABLE so that convert_and_rename
// rebuilds run inside an attached schema, not silently against `main`.

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

[struct_convert]
def convert_v1_to_v2(old : UserV1; var dst : User) {
    dst.Email = (old.LegacyEmail != "") ? old.LegacyEmail : "unknown@example.com"
}

[test]
def test_rebuild_runs_in_attached_schema(t : T?) {
    var inscope main = open_sqlite(":memory:")
    main |> with_attached("file:rebuild_attached_test?mode=memory&cache=shared", "att") $(att) {
        att |> exec("DROP TABLE IF EXISTS \"users\"")
        att |> exec("DROP TABLE IF EXISTS \"users_new\"")
        att |> exec("CREATE TABLE \"users\" (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, LegacyEmail TEXT NOT NULL DEFAULT '')")
        att |> exec("INSERT INTO \"users\" (Id, Name, LegacyEmail) VALUES (1, 'alice', 'alice@x')")
        att |> convert_and_rename(type<UserV1>, type<User>)
        let n_in_att = main |> query_scalar(
            "SELECT COUNT(*) FROM \"att\".sqlite_master WHERE type='table' AND name='users'",
            type<int>)
        t |> equal(n_in_att, 1, "rebuilt `users` lives in attached schema")
        let main_users = main |> query_scalar(
            "SELECT COUNT(*) FROM main.sqlite_master WHERE type='table' AND name='users'",
            type<int>)
        t |> equal(main_users, 0, "no rebuild side-effect in main")
        let landed = att |> query_scalar(
            "SELECT Email FROM users WHERE Id=1", type<string>)
        t |> equal(landed, "alice@x", "row content preserved through rebuild")
    }
}
