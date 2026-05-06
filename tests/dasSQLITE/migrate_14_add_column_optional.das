options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    Id : int64
    Name : string
    Email : Option<string>
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
}

[sql_migration(version = 2)]
def m_002(db : SqlRunner) {
    db |> add_column(type<User>, "Email")
}

[test]
def test_option_field_is_nullable_no_default(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let notnull = db |> query_scalar(
        "SELECT \"notnull\" FROM pragma_table_info('users') WHERE name='Email'", type<int>)
    t |> equal(notnull, 0, "Option<string> field must be nullable")
    let dflt = db |> query_scalar(
        "SELECT COUNT(*) FROM pragma_table_info('users') WHERE name='Email' AND dflt_value IS NULL",
        type<int>)
    t |> equal(dflt, 1, "Option<string> field must have NULL default (i.e. no DEFAULT clause)")
    let stype = db |> query_scalar(
        "SELECT type FROM pragma_table_info('users') WHERE name='Email'", type<string>)
    t |> equal(stype, "TEXT", "string maps to TEXT storage")
}
