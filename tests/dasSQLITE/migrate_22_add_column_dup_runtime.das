options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    Id : int64
    Score : int
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY)")
}

[sql_migration(version = 2)]
def m_002(db : SqlRunner) {
    db |> add_column(type<User>, "Score", 0)
}

[sql_migration(version = 3)]
def m_003(db : SqlRunner) {
    // Re-add the same column — SQLite ALTER TABLE … ADD COLUMN errors at runtime
    // ("duplicate column name: Score"). The α-shape transaction in migrate_to_latest
    // must roll back v=2 + v=3 atomically; the framework must not swallow the panic.
    db |> add_column(type<User>, "Score", 1)
}

[test]
def test_runtime_alter_failure_rolls_back_transaction(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let res = db |> try_migrate_to_latest()
    t |> equal(res |> is_err, true, "try_migrate_to_latest must surface the duplicate-column failure as Err")
    // After rollback, neither v=2 nor v=3 are applied — applied_max is still 0 (no rows).
    let max_applied = db |> query_scalar(
        "SELECT COALESCE(MAX(version), 0) FROM __schema_version", type<int>)
    t |> equal(max_applied, 0, "α-shape rollback: v=2 and v=3 must both be unrecorded after v=3 fails")
    let table_exists = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='users'", type<int>)
    t |> equal(table_exists, 0, "v=1's CREATE TABLE rolls back too — α-shape wraps the whole call")
}
