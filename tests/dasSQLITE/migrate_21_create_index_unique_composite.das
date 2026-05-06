options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    Id : int64
    Email : string
    Name : string
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Email TEXT NOT NULL, Name TEXT NOT NULL)")
}

[sql_migration(version = 2)]
def m_002(db : SqlRunner) {
    db |> create_unique_index(type<User>, ("Email", "Name"), "ux_email_name")
}

[sql_migration(version = 3)]
def m_003(db : SqlRunner) {
    // Idempotence: existing name → drops; second call → no-op (SQLite IF EXISTS).
    db |> drop_index_if_exists("ux_email_name")
    db |> drop_index_if_exists("ux_email_name")
    db |> drop_index_if_exists("never_existed")
}

[test]
def test_all_three_migrations_apply(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let n = db |> migrate_to_latest()
    t |> equal(n, 3, "all three migrations applied (CREATE TABLE + CREATE UNIQUE INDEX + drop trio)")
}

[test]
def test_drop_index_if_exists_idempotent(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let idx_count = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='index' AND tbl_name='users' AND name NOT LIKE 'sqlite_%'",
        type<int>)
    t |> equal(idx_count, 0, "All three drop_index_if_exists calls succeeded silently; no indexes remain")
}

[test]
def test_unique_composite_was_created_first(t : T?) {
    // Run only v1 + v2 (skip v3 which drops the index) by walking history.
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Email TEXT NOT NULL, Name TEXT NOT NULL)")
    db |> create_unique_index(type<User>, ("Email", "Name"), "ux_email_name")
    let is_unique = db |> query_scalar(
        "SELECT \"unique\" FROM pragma_index_list('users') WHERE name='ux_email_name'", type<int>)
    t |> equal(is_unique, 1, "create_unique_index must produce a UNIQUE index")
    let col_count = db |> query_scalar(
        "SELECT COUNT(*) FROM pragma_index_info('ux_email_name')", type<int>)
    t |> equal(col_count, 2, "composite index must reference both columns")
}
