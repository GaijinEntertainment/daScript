options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_migration(version = 1, description = "create table; vacuum after", vacuum = true)]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE big (x BLOB)")
    db |> exec("INSERT INTO big SELECT randomblob(1024) FROM (WITH RECURSIVE n(i) AS (SELECT 1 UNION ALL SELECT i+1 FROM n WHERE i<200) SELECT i FROM n)")
    db |> exec("DELETE FROM big WHERE rowid > 100")
}

[test]
def test_vacuum_runs_post_commit(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    // After VACUUM, freelist_count is 0 (all freed pages reclaimed).
    let freelist = db |> query_scalar("PRAGMA freelist_count", type<int>)
    t |> equal(freelist, 0, "VACUUM must zero the freelist")
}

[test]
def test_audit_row_present_with_vacuum_flag_set(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let n = db |> query_scalar("SELECT COUNT(*) FROM __schema_version", type<int>)
    t |> equal(n, 1, "audit row present even though VACUUM ran")
}
