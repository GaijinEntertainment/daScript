options gen2

require dastest/testing_boost public
require sqlite/sql_migrate

[sql_migration(version = 1, description = "alpha")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE x (a INT)")
}

[sql_migration(version = 2, description = "beta")]
def m2(db : SqlRunner) {
    db |> exec("CREATE TABLE y (b INT)")
}

[test]
def test_current_zero_on_empty_db(t : T?) {
    var inscope db = open_sqlite(":memory:")
    t |> equal(db |> current_schema_version(), 0, "fresh DB current must be 0")
}

[test]
def test_current_does_not_create_audit(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let v = db |> current_schema_version()
    t |> equal(v, 0, "first call returns 0")
    let exists = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE name='__schema_version'", type<int>)
    t |> equal(exists, 0, "current_schema_version must NOT create the audit table")
}

[test]
def test_pending_lists_all_on_fresh_db(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let p <- db |> pending_migrations()
    t |> equal(p |> length, 2, "fresh DB sees both registered migrations as pending")
    t |> equal(p[0].version, 1, "first pending is v1")
    t |> equal(p[1].version, 2, "second pending is v2")
    t |> equal(p[0].description, "alpha", "description from annotation")
}

[test]
def test_pending_empty_when_current(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let p <- db |> pending_migrations()
    t |> equal(p |> length, 0, "pending must be empty after migrate_to_latest")
}

[test]
def test_history_empty_on_fresh_db(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let h <- db |> migration_history()
    t |> equal(h |> length, 0, "fresh DB history must be empty")
    let exists = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE name='__schema_version'", type<int>)
    t |> equal(exists, 0, "migration_history must NOT create the audit table")
}

[test]
def test_inspectors_have_no_side_effects_on_fresh_db(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let _v = db |> current_schema_version()
    let _p <- db |> pending_migrations()
    let _h <- db |> migration_history()
    let exists = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE name='__schema_version'", type<int>)
    t |> equal(exists, 0, "all three inspectors combined must leave fresh DB unchanged")
}
