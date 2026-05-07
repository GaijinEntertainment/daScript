options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_table(name = "users", legacy = true)]
struct UserV1 {
    @sql_primary_key Id : int
    Name : string
    Score : int
}

[sql_table(name = "users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Score : int
}

[struct_convert]
def my_v1_to_v2(old : UserV1; var dst : User) {
    pass
}

[test]
def test_trivial_round_trip(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, Score INTEGER NOT NULL DEFAULT 0)")
    db |> exec("INSERT INTO users (Id, Name, Score) VALUES (1, 'alice', 42), (2, 'bob', 7)")
    db |> convert_and_rename(type<UserV1>, type<User>)
    let n = db |> query_scalar("SELECT COUNT(*) FROM users", type<int>)
    t |> equal(n, 2, "row count preserved")
    let alice_score = db |> query_scalar("SELECT Score FROM users WHERE Name='alice'", type<int>)
    t |> equal(alice_score, 42, "alice's Score copied verbatim")
    let bob_score = db |> query_scalar("SELECT Score FROM users WHERE Name='bob'", type<int>)
    t |> equal(bob_score, 7, "bob's Score copied verbatim")
}
