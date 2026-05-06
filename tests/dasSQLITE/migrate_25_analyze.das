options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_migration(version = 1, description = "create + analyze", analyze = true)]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
    db |> exec("CREATE INDEX idx_users_name ON users(Name)")
    db |> exec("INSERT INTO users (Name) VALUES ('alice'), ('bob'), ('charlie')")
}

[test]
def test_analyze_populates_sqlite_stat1(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    // ANALYZE creates sqlite_stat1 if missing, populates with index stats.
    let stat1_exists = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE name='sqlite_stat1'", type<int>)
    t |> equal(stat1_exists, 1, "sqlite_stat1 must exist after ANALYZE")
    let rows = db |> query_scalar("SELECT COUNT(*) FROM sqlite_stat1", type<int>)
    t |> success(rows > 0, "sqlite_stat1 must have at least one row for the index")
}
