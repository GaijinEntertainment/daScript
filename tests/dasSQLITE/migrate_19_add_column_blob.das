options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

struct AvatarPayload {
    width : int
    height : int
}

[sql_table(name = "users")]
struct User {
    Id : int64
    Name : string
    @sql_blob
    Avatar : Option<AvatarPayload>
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
}

[sql_migration(version = 2)]
def m_002(db : SqlRunner) {
    db |> add_column(type<User>, "Avatar")
}

[test]
def test_sql_blob_struct_field_emits_blob_storage(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let stype = db |> query_scalar(
        "SELECT type FROM pragma_table_info('users') WHERE name='Avatar'", type<string>)
    t |> equal(stype, "BLOB", "@sql_blob column must use BLOB storage")
    let notnull = db |> query_scalar(
        "SELECT \"notnull\" FROM pragma_table_info('users') WHERE name='Avatar'", type<int>)
    t |> equal(notnull, 0, "Option<AvatarPayload> must be nullable")
}
