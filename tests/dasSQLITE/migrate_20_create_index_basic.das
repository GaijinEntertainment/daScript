options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    Id : int64
    Email : string
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Email TEXT NOT NULL)")
}

[sql_migration(version = 2)]
def m_002(db : SqlRunner) {
    db |> create_index(type<User>, "Email")
}

[test]
def test_auto_named_single_col_index(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let idx_count = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='index' AND tbl_name='users' AND name NOT LIKE 'sqlite_%'",
        type<int>)
    t |> equal(idx_count, 1, "exactly one user-defined index expected")
    let auto_name = db |> query_scalar(
        "SELECT name FROM sqlite_master WHERE type='index' AND tbl_name='users' AND name NOT LIKE 'sqlite_%'",
        type<string>)
    t |> equal(auto_name, "idx_users_Email",
        "auto-name follows [sql_index] convention idx_<table>_<col>")
    let is_unique = db |> query_scalar(
        "SELECT \"unique\" FROM pragma_index_list('users') WHERE name='idx_users_Email'", type<int>)
    t |> equal(is_unique, 0, "create_index defaults to non-unique")
}
