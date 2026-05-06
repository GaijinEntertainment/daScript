options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_migration(version = 1, description = "no flags")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY)")
}

[test]
def test_no_analyze_unless_flagged(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    // ANALYZE was not requested → sqlite_stat1 must NOT exist.
    let stat1_exists = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE name='sqlite_stat1'", type<int>)
    t |> equal(stat1_exists, 0, "sqlite_stat1 must NOT exist when no migration sets analyze=true")
}
