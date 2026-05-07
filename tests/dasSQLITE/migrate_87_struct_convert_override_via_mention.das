options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

// Override-detection records `dst.X` ONLY when it appears on the LHS of an
// assignment (`=`, `<-`, `:=`). The visitor sees `dst.Score = …` and suppresses
// the auto-derive that would otherwise macro_error on `Option<int> → int`
// (existing rows may be NULL).

[sql_table(name = "users", legacy = true)]
struct UserV1 {
    @sql_primary_key Id : int
    Name : string
    Score : Option<int>
}

[sql_table(name = "users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Score : int
}

[struct_convert]
def my_v1_to_v2(old : UserV1; var dst : User) {
    // LHS write recorded; auto-derive suppressed; user expression resolves the Option.
    dst.Score = old.Score ?? 0
}

[test]
def test_lhs_write_suppresses_auto_derive(t : T?) {
    // The compile passing IS the test — without LHS-detection [struct_convert]
    // would macro_error on Option<T> → T. With it, the user's body resolves the
    // Option and the macro stays out of the way.
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, Score INTEGER)")
    db |> exec("INSERT INTO users (Id, Name, Score) VALUES (1, 'alice', 42)")
    db |> convert_and_rename(type<UserV1>, type<User>)
    let alice_score = db |> query_scalar("SELECT Score FROM users WHERE Name='alice'", type<int>)
    t |> equal(alice_score, 42, "alice's some(42) unwrapped via the body's resolution")
}
