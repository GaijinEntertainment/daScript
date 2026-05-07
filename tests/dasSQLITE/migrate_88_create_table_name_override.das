options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_table(name = "users"), sql_index(fields = "Name")]
struct User {
    @sql_primary_key Id : int
    Name : string
}

[test]
def test_create_table_emits_with_alternate_name(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> create_table(type<User>, "users_new")
    let count = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='users_new'", type<int>)
    t |> equal(count, 1, "users_new table created via name override")
    let original_count = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='users'", type<int>)
    t |> equal(original_count, 0, "original 'users' table not auto-created")
}

[test]
def test_create_table_emits_full_schema(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> create_table(type<User>, "users_new")
    let pk = db |> query_scalar(
        "SELECT pk FROM pragma_table_info('users_new') WHERE name='Id'", type<int>)
    t |> equal(pk, 1, "Id column is PRIMARY KEY in users_new")
    let notnull = db |> query_scalar(
        "SELECT \"notnull\" FROM pragma_table_info('users_new') WHERE name='Name'", type<int>)
    t |> equal(notnull, 1, "Name column is NOT NULL")
}

[test]
def test_create_table_emits_indexes_with_runtime_name(t : T?) {
    // [sql_index] auto-name uses the runtime table name, so indexes for the staging table
    // are named `idx_users_new_<col>` — no collision with original table's `idx_users_<col>`.
    var inscope db = open_sqlite(":memory:")
    db |> create_table(type<User>, "users_new")
    let idx = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='index' AND tbl_name='users_new' AND name='idx_users_new_Name'", type<int>)
    t |> equal(idx, 1, "auto-named index targets the staging table")
}
