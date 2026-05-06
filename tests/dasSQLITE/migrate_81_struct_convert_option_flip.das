options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate
require daslib/option public

// T → Option<T>: source has bare Score, target has Option<Score>. Auto-rule wraps with some().

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
    Score : Option<int>
}

[struct_convert]
def my_v1_to_v2(old : UserV1; var dst : User) {
    pass
}

[test]
def test_t_to_option_wraps_with_some(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, Score INTEGER NOT NULL DEFAULT 0)")
    db |> exec("INSERT INTO users (Id, Name, Score) VALUES (1, 'alice', 42)")
    db |> convert_and_rename(type<UserV1>, type<User>)
    var rows : array<User>
    rows |> reserve(2)
    for (row in db |> select_from(type<User>)) {
        rows |> emplace(row)
    }
    t |> equal(length(rows), 1, "row preserved")
    t |> success(rows[0].Score |> is_some, "Option<int> wraps to some(...)")
    t |> equal(rows[0].Score |> unwrap, 42, "wrapped value preserved")
}
