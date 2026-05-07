options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

// ``Option<S> → Option<T>`` cross-payload — the macro emits ``struct_convert_field(dst, src)``
// and the cross-payload Option overload unwraps src, dispatches S→T inner, re-wraps.

[sql_table(name = "users", legacy = true)]
struct UserV1 {
    @sql_primary_key Id : int
    Score : Option<int>
}

[sql_table(name = "users")]
struct User {
    @sql_primary_key Id : int
    Score : Option<string>
}

[struct_convert]
def m_v1_to_v2(old : UserV1; var dst : User) {}

[test]
def test_option_cross_payload(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Score INTEGER)")
    db |> exec("INSERT INTO users (Id, Score) VALUES (1, 42), (2, NULL)")
    db |> convert_and_rename(type<UserV1>, type<User>)
    let alice = db |> query_scalar("SELECT Score FROM users WHERE Id=1", type<string>)
    t |> equal(alice, "42", "Option<int>(some) → Option<string>(some) via int→string inner")
    // SQLite returns "" for NULL when target is non-nullable string column; with Option<string>
    // target the rebuild stores actual NULL, but query_scalar collapses to default<string>.
    let n_null = db |> query_scalar(
        "SELECT COUNT(*) FROM users WHERE Id=2 AND Score IS NULL",
        type<int>)
    t |> equal(n_null, 1, "Option<int>(none) → Option<string>(none) preserved as NULL")
}
