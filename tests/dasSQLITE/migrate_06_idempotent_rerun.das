options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_migration(version = 1, description = "create users")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY)")
}

[test]
def test_second_call_returns_zero(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    t |> equal(db |> migrate_to_latest(), 0, "rerun on up-to-date DB must return 0")
}

[test]
def test_second_call_does_not_add_audit_rows(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let before = db |> query_scalar("SELECT COUNT(*) FROM __schema_version", type<int>)
    db |> migrate_to_latest()
    let after = db |> query_scalar("SELECT COUNT(*) FROM __schema_version", type<int>)
    t |> equal(before, after, "rerun must not add audit rows")
}
