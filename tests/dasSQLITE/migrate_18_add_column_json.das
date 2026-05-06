options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

struct Profile {
    nickname : string
    avatar_url : string
}

[sql_table(name = "users")]
struct User {
    Id : int64
    Name : string
    @sql_json
    Profile : Option<Profile>
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
}

[sql_migration(version = 2)]
def m_002(db : SqlRunner) {
    db |> add_column(type<User>, "Profile")
}

[test]
def test_sql_json_field_emits_text_storage(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let stype = db |> query_scalar(
        "SELECT type FROM pragma_table_info('users') WHERE name='Profile'", type<string>)
    t |> equal(stype, "TEXT", "@sql_json column must use TEXT storage regardless of payload struct")
    let notnull = db |> query_scalar(
        "SELECT \"notnull\" FROM pragma_table_info('users') WHERE name='Profile'", type<int>)
    t |> equal(notnull, 0, "Option<Profile> must be nullable")
}
