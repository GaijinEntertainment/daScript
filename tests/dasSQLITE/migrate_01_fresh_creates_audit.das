options gen2

require dastest/testing_boost public
require sqlite/sql_migrate

[sql_migration(version = 1, description = "create users")]
def m_users(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY)")
}

[test]
def test_audit_table_appears_after_migrate(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let exists = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='__schema_version'",
        type<int>)
    t |> equal(exists, 1, "__schema_version must exist after migrate_to_latest")
}

[test]
def test_audit_table_has_locked_3_column_shape(t : T?) {
    // Shape locked 2026-05-04: (version INTEGER PK, description TEXT NOT NULL DEFAULT '', applied_at INTEGER NOT NULL).
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let cols = db |> query_scalar(
        "SELECT COUNT(*) FROM pragma_table_info('__schema_version')", type<int>)
    t |> equal(cols, 3, "audit table must have exactly 3 columns")
    let ver_pk = db |> query_scalar(
        "SELECT pk FROM pragma_table_info('__schema_version') WHERE name='version'", type<int>)
    t |> equal(ver_pk, 1, "version column must be the PK")
    let desc_notnull = db |> query_scalar(
        "SELECT \"notnull\" FROM pragma_table_info('__schema_version') WHERE name='description'",
        type<int>)
    t |> equal(desc_notnull, 1, "description must be NOT NULL")
    let applied_notnull = db |> query_scalar(
        "SELECT \"notnull\" FROM pragma_table_info('__schema_version') WHERE name='applied_at'",
        type<int>)
    t |> equal(applied_notnull, 1, "applied_at must be NOT NULL")
}
