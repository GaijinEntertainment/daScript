options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    Id : int64
    Name : string
    @sql_column = "email_addr"
    Email : Option<string>
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
}

[sql_migration(version = 2)]
def m_002(db : SqlRunner) {
    // User passes the daslang field name; emitted DDL uses the @sql_column-renamed identifier.
    db |> add_column(type<User>, "Email")
}

[test]
def test_sql_column_rename_propagates_to_alter(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let renamed_count = db |> query_scalar(
        "SELECT COUNT(*) FROM pragma_table_info('users') WHERE name='email_addr'", type<int>)
    t |> equal(renamed_count, 1, "Column must appear under the @sql_column-renamed SQL identifier")
    let daslang_count = db |> query_scalar(
        "SELECT COUNT(*) FROM pragma_table_info('users') WHERE name='Email'", type<int>)
    t |> equal(daslang_count, 0, "Column must NOT appear under the daslang field name")
}
