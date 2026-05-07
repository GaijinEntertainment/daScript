options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Email : string
}

[test]
def test_insert_lands_in_named_table(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> create_table(type<User>, "users_new")
    let u = User(Name = "alice", Email = "alice@x")
    let id = db |> insert(u, "users_new")
    t |> success(id > 0l, "insert returned a row id")
    let n = db |> query_scalar("SELECT COUNT(*) FROM users_new", type<int>)
    t |> equal(n, 1, "row landed in users_new, not users")
    let landed_email = db |> query_scalar(
        "SELECT Email FROM users_new WHERE Name='alice'", type<string>)
    t |> equal(landed_email, "alice@x", "row content preserved")
}
