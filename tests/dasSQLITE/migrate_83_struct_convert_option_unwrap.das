options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

// `Option<T> → T` dispatches through the shipped ``struct_convert_field`` overload —
// NULL collapses to ``default<T>`` (0 / "" / 0.0). Override in your module for different
// NULL-handling semantics.

[sql_table(name = "users", legacy = true)]
struct UserV1 {
    @sql_primary_key Id : int
    Name : Option<string>
    Score : Option<int>
}

[sql_table(name = "users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Score : int
}

[struct_convert]
def m_v1_to_v2(old : UserV1; var dst : User) {}

[test]
def test_option_unwrap_default_on_null(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT, Score INTEGER)")
    db |> exec("INSERT INTO users (Id, Name, Score) VALUES (1, 'alice', 42)")
    db |> exec("INSERT INTO users (Id, Name, Score) VALUES (2, NULL, NULL)")
    db |> convert_and_rename(type<UserV1>, type<User>)
    let alice_name = db |> query_scalar("SELECT Name FROM users WHERE Id=1", type<string>)
    let alice_score = db |> query_scalar("SELECT Score FROM users WHERE Id=1", type<int>)
    t |> equal(alice_name, "alice", "Some(string) → string preserved")
    t |> equal(alice_score, 42, "Some(int) → int preserved")
    let bob_name = db |> query_scalar("SELECT Name FROM users WHERE Id=2", type<string>)
    let bob_score = db |> query_scalar("SELECT Score FROM users WHERE Id=2", type<int>)
    t |> equal(bob_name, "", "NULL string → default<string>=\"\"")
    t |> equal(bob_score, 0, "NULL int → default<int>=0")
}
